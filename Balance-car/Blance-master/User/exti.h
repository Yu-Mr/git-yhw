#ifndef __EXTI_H
#define __EXIT_H	


#include "stm32f10x.h"


#define INT PBin(5)   //PB5���ӵ�MPU6050���ж�����

#define EXTI_GPIO_PORT         GPIOB
#define EXTI_GPIO_PIN          GPIO_Pin_5
#define EXTI_GPIO_CLK          RCC_APB2Periph_GPIOB

#define EXTI_PORTSOURCE   	GPIO_PortSourceGPIOB
#define EXTI_PINSOURCE    	GPIO_PinSource5
#define EXTI_LINE         	EXTI_Line5

#define EXTI_IRQ          	EXTI9_5_IRQn


void EXTI_Config(void);
void  Control_EXTI_Init(void);	//�ⲿ�жϳ�ʼ��		 					    
#endif

























