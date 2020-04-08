#ifndef __MOTOR_H
#define __MOTOR_H

#include <sys.h>	 



#define PWMA   TIM1->CCR2  //PA9
#define AIN2   PBout(15)
#define AIN1   PBout(14)
#define BIN1   PBout(13)
#define BIN2   PBout(12)
#define PWMB   TIM1->CCR4  //PA11

void MOTOR_PWM_Init(u16 arr,u16 psc);

#endif
