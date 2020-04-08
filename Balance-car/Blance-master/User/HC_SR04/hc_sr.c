#include "hc_sr.h"

u32 Distance;
//舵机
u8  ServoFlag = 0;

HCSR_Value hc_sr04Val = {0,0,0,0};

void HCSR04_Init(u16 arr,u16 psc)	
{	 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|SIGNAL_GPIO_CLK, ENABLE);  
	
	
	GPIO_InitStructure.GPIO_Pin  = ECHO_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  
	GPIO_Init(ECHO_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = TRIG_GPIO_PIN;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;     
	GPIO_Init(TRIG_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = SIGNAL_GPIO_PIN;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
	GPIO_Init(SIGNAL_GPIO_PORT, &GPIO_InitStructure);
	
	//初始化定时器3 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM3输入捕获参数
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3; 
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;//配置输入滤波器 不滤波
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	//舵机PWM输出
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性 
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC3,ENABLE);//允许更新中断 ,允许CC3IE捕获中断	
	TIM_Cmd(TIM3,ENABLE ); 	//使能定时器3
}


//超声波接收回波函数
void Read_Distane(void)
{   
	 PBout(1)=1;
	 delay_us(15);  
	 PBout(1)=0;	
			if(hc_sr04Val.FinishFlag)//成功捕获到了一次高电平
		{
			Distance=hc_sr04Val.Period;
			Distance*=65536;					        //溢出时间总和
			Distance+=hc_sr04Val.Value;		//得到总的高电平时间
			Distance=Distance*170/1000;
		//	printf("%d \r\n",Distance);
			hc_sr04Val.FinishFlag =0;			//开启下一次捕获
		}				
}

void TIM3_IRQHandler(void)
{	
	if(!hc_sr04Val.FinishFlag)
	{
		if ( TIM_GetITStatus( TIM3, TIM_IT_Update) != RESET )               
		{	
			//舵机控制
			if(ServoFlag++ == 0)
				TIM_SetCompare2(TIM3,1000);
			
			else if(ServoFlag == 5)
				TIM_SetCompare2(TIM3,2200);
			
			else if(ServoFlag == 10)
				ServoFlag = 0;
						
			if(hc_sr04Val.StartFlag)
			{
				if((hc_sr04Val.Period&0X3F)==0X3F)//高电平太长
				{
					hc_sr04Val.FinishFlag = 1;//标记成功捕获了一次
					hc_sr04Val.Value=0XFFFF;
				}
				
				else
					hc_sr04Val.Period ++;		
				TIM_ClearITPendingBit ( TIM3, TIM_FLAG_Update );
			}				
		}

		// 上升沿捕获中断
		if ( TIM_GetITStatus (TIM3, TIM_IT_CC3 ) != RESET)
		{
			if ( hc_sr04Val.StartFlag == 0 )
			{
				TIM_SetCounter ( TIM3, 0 );
				hc_sr04Val.Period = 0;		
				hc_sr04Val.Value = 0;
				// 当第一次捕获到上升沿之后，把捕获边沿配置为下降沿
				TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Falling);		
				hc_sr04Val.StartFlag = 1;			
			}
			// 下降沿捕获中断
			else 
			{
				hc_sr04Val.Value = TIM_GetCapture3 (TIM3);
				TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Rising);	
				hc_sr04Val.StartFlag = 0;		
				hc_sr04Val.FinishFlag = 1;		
			}

			TIM_ClearITPendingBit (TIM3,TIM_IT_CC3);	    
		}
	}

	TIM_ClearITPendingBit ( TIM3, TIM_FLAG_Update );//清除中断标志位 	
	TIM_ClearITPendingBit (TIM3,TIM_IT_CC3);
}

