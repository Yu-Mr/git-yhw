#include "hc_sr.h"

u32 Distance;
//���
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
	
	//��ʼ����ʱ��3 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM3���벶�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3; 
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;//���������˲��� ���˲�
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	//���PWM���
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��� 
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC3,ENABLE);//��������ж� ,����CC3IE�����ж�	
	TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��3
}


//���������ջز�����
void Read_Distane(void)
{   
	 PBout(1)=1;
	 delay_us(15);  
	 PBout(1)=0;	
			if(hc_sr04Val.FinishFlag)//�ɹ�������һ�θߵ�ƽ
		{
			Distance=hc_sr04Val.Period;
			Distance*=65536;					        //���ʱ���ܺ�
			Distance+=hc_sr04Val.Value;		//�õ��ܵĸߵ�ƽʱ��
			Distance=Distance*170/1000;
		//	printf("%d \r\n",Distance);
			hc_sr04Val.FinishFlag =0;			//������һ�β���
		}				
}

void TIM3_IRQHandler(void)
{	
	if(!hc_sr04Val.FinishFlag)
	{
		if ( TIM_GetITStatus( TIM3, TIM_IT_Update) != RESET )               
		{	
			//�������
			if(ServoFlag++ == 0)
				TIM_SetCompare2(TIM3,1000);
			
			else if(ServoFlag == 5)
				TIM_SetCompare2(TIM3,2200);
			
			else if(ServoFlag == 10)
				ServoFlag = 0;
						
			if(hc_sr04Val.StartFlag)
			{
				if((hc_sr04Val.Period&0X3F)==0X3F)//�ߵ�ƽ̫��
				{
					hc_sr04Val.FinishFlag = 1;//��ǳɹ�������һ��
					hc_sr04Val.Value=0XFFFF;
				}
				
				else
					hc_sr04Val.Period ++;		
				TIM_ClearITPendingBit ( TIM3, TIM_FLAG_Update );
			}				
		}

		// �����ز����ж�
		if ( TIM_GetITStatus (TIM3, TIM_IT_CC3 ) != RESET)
		{
			if ( hc_sr04Val.StartFlag == 0 )
			{
				TIM_SetCounter ( TIM3, 0 );
				hc_sr04Val.Period = 0;		
				hc_sr04Val.Value = 0;
				// ����һ�β���������֮�󣬰Ѳ����������Ϊ�½���
				TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Falling);		
				hc_sr04Val.StartFlag = 1;			
			}
			// �½��ز����ж�
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

	TIM_ClearITPendingBit ( TIM3, TIM_FLAG_Update );//����жϱ�־λ 	
	TIM_ClearITPendingBit (TIM3,TIM_IT_CC3);
}

