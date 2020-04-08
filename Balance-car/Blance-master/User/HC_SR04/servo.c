#include "servo.h"
#include "stm32f10x.h"
/**************************************************************************
函数功能：定时器2通道3PWM输出
入口参数：入口参数：arr：自动重装值  psc：时钟预分频数  PWM_Init(199,7199)
返回  值：无
**************************************************************************/

void SERVO_Init(u16 arr,u16 psc)	
{	 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = SIGNAL_GPIO_PIN;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
	GPIO_Init(SIGNAL_GPIO_PORT, &GPIO_InitStructure);
	
	//初始化定时器2 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性 
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  
	TIM_Cmd(TIM2,ENABLE ); 	//使能定时器3
	
//	//初始化定时器2 TIM2	 
//	TIM_TimeBaseStructure.TIM_Period = 5000; //设定计数器自动重装值 
//	TIM_TimeBaseStructure.TIM_Prescaler =7200-1; 	//预分频器   
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
//	//中断分组初始化
//	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure); 
//	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);//允许更新中断 ,允许CC3IE捕获中断	
//	TIM_Cmd(TIM6,ENABLE ); 	//使能定时器3
}