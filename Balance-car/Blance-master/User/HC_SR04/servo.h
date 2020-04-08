#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

#define SIGNAL_GPIO_PIN		GPIO_Pin_10
#define SIGNAL_GPIO_PORT		GPIOB
#define SIGNAL_GPIO_CLK		RCC_APB2Periph_GPIOB





void SERVO_Init(u16 arr,u16 psc)	;


#endif
