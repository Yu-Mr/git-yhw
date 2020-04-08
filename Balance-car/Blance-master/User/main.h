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

extern u8 Flag_forward,Flag_back,Flag_left,Flag_right,Flag_speed;  	//蓝牙遥控相关的变量
extern u8 Flag_stop,Flag_show;								//停止标志位和 显示标志位 默认停止 显示打开

extern int nEncoder_left,nEncoder_right;						//左右编码器的脉冲计数
extern int Motor_pwm1,Motor_pwm2;								//电机PWM变

extern float Accel_Y,Accel_X,Accel_Z,Gyro_Y,Gyro_Z,Gyro_X;				//陀螺仪六轴参数
extern float Balance_angle,Balance_gyro,Turn_gyro;				//平衡倾角 平衡陀螺仪 转向陀螺仪

extern float Balance_Kp,Balance_Kd,Velocity_Kp,Velocity_Ki;			//PID参数


#endif
