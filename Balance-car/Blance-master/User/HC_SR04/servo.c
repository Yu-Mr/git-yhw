#include "servo.h"
#include "stm32f10x.h"
/**************************************************************************
�������ܣ���ʱ��2ͨ��3PWM���
��ڲ�������ڲ�����arr���Զ���װֵ  psc��ʱ��Ԥ��Ƶ��  PWM_Init(199,7199)
����  ֵ����
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
	
	//��ʼ����ʱ��2 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��� 
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  
	TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��3
	
//	//��ʼ����ʱ��2 TIM2	 
//	TIM_TimeBaseStructure.TIM_Period = 5000; //�趨�������Զ���װֵ 
//	TIM_TimeBaseStructure.TIM_Prescaler =7200-1; 	//Ԥ��Ƶ��   
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//	//�жϷ����ʼ��
//	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure); 
//	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);//��������ж� ,����CC3IE�����ж�	
//	TIM_Cmd(TIM6,ENABLE ); 	//ʹ�ܶ�ʱ��3
}