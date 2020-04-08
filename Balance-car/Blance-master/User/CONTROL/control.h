#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"

#define PI 3.14159265
#define ZHONGZHI 2.7f

int m_abs(int a);
void Balance_control(void);
static void Get_Voltage(void);
static void PWM_limit(void);
static int velocity(void);
static int balance(void);
static u8 EmergencyStop(float angle, int voltage);
static void Set_Pwm(void);
static void Get_Angle(void);
#endif
