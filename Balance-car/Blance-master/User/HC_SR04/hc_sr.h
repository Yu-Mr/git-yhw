#ifndef __HCSR_H
#define __HCSR_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

#define ECHO_GPIO_PIN		GPIO_Pin_0
#define ECHO_GPIO_PORT		GPIOB
#define ECHO_GPIO_CLK		RCC_APB2Periph_GPIOB

#define TRIG_GPIO_PIN		GPIO_Pin_1
#define TRIG_GPIO_PORT		GPIOB
#define TRIG_GPIO_CLK		RCC_APB2Periph_GPIOB

#define SIGNAL_GPIO_PIN		GPIO_Pin_7
#define SIGNAL_GPIO_PORT		GPIOA
#define SIGNAL_GPIO_CLK		RCC_APB2Periph_GPIOA

typedef struct
{   
	u8   FinishFlag;   // ���������־λ
	u8   StartFlag;    // ����ʼ��־λ
	u16  Value;     	// ����Ĵ�����ֵ
	u16  Period;       // �Զ���װ�ؼĴ������±�־ 
}HCSR_Value;

//extern HCSR_Value hc_sr04Val;
extern u32 Distance;

void HCSR04_Init(u16 arr,u16 psc);
void Read_Distane(void);
//void TIM3_IRQHandler(void);
#endif
