#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "oled.h"
#include "delay.h"
#include "adc.h"
#include "usart.h"
#include "MPU6050.h"
#include "i2c.h"
#include "sys.h"
#include "exti.h"
#include "hc_sr.h"
#include "motor.h"
#include "encoder.h"
#include "control.h"
#include "show.h"
#include "Kalman.h"


extern u16 ADC_ConvertedValue;
extern u32 Distance;
extern u16 voltage;

extern u8 Flag_forward,Flag_back,Flag_left,Flag_right,Flag_speed;  	//����ң����صı���
extern u8 Flag_stop,Flag_show;								//ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��

extern int nEncoder_left,nEncoder_right;						//���ұ��������������
extern int Motor_pwm1,Motor_pwm2;								//���PWM��

extern float Accel_Y,Accel_X,Accel_Z,Gyro_Y,Gyro_Z,Gyro_X;				//�������������
extern float Balance_angle,Balance_gyro,Turn_gyro;				//ƽ����� ƽ�������� ת��������

extern float Balance_Kp,Balance_Kd,Velocity_Kp,Velocity_Ki;			//PID����


#endif
