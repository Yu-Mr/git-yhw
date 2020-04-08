#include "main.h"



u16 voltage;								//电池电压

u8 Flag_forward,Flag_back,Flag_left,Flag_right,Flag_speed=1;  //蓝牙遥控相关的变量
u8 Flag_stop=0,Flag_show=0;					//停止标志位和 显示标志位 默认停止 显示打开

int nEncoder_left,nEncoder_right;				//左右编码器的脉冲计数
int Motor_pwm1,Motor_pwm2;					//电机PWM变

float Accel_Y,Accel_X,Accel_Z,Gyro_Y,Gyro_Z,Gyro_X;		//陀螺仪六轴参数
float Balance_angle,Balance_gyro,Turn_gyro; //平衡倾角 平衡陀螺仪 转向陀螺仪

float Balance_Kp=300,Balance_Kd=1.2,Velocity_Kp=80,Velocity_Ki=0.4;//PID参数 

u8 show;  //显示标志

int main(void)
{
		
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
	delay_init(72);
	//uart_init(115200);
	uart3_init(4800);
	OLED_Init();
	IIC_Init();
	ADCx_Init();
	MPU6050_Init();
	DMP_Init();
	Control_EXTI_Init();
	MOTOR_PWM_Init(7199,0);
	HCSR04_Init(0XFFFF,72-1);
	Encoder_Init(); 

	//SERVO_Init(199,7199);  //20ms的脉冲  一个脉0.1  0.5-2.5   22-10
	while(1)
	{
		if(show == 10)
		{
			oled_show();
			show = 0;
		}
	}
}

//中断函数，控制程序
void EXTI9_5_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(EXTI_LINE) != RESET) 
	{
		show++;
		Balance_control();
		EXTI_ClearITPendingBit(EXTI_LINE);     
	}  
}

//	OLED_chinese(0,5,dian,16,1);
//	OLED_chinese(16,5,chi,16,1);
//	OLED_chinese(32,5,dian,16,1);
//	OLED_chinese(48,5,liang,16,1);
//	OLED_ShowChar(64,5,':',16,1);
//	v = ADC_ConvertedValue*100/4096;
//	//v = ADC_ConvertedValue;	
//	OLED_ShowNum(72,5,MPU6050_getDeviceID(),3,16);//显示2个数字		
//	OLED_Refresh_Gram();


//Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //读取Y轴陀螺仪
//		Gyro_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L);    //读取Z轴陀螺仪
//		Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //读取X轴加速度计
//	  	Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L); //读取Z轴加速度计
//		
//		
//		//Gyro_Balance=-Gyro_Y;                                  //更新平衡角速度
//	   	Accel_Y=atan2(Accel_X,Accel_Z)*180/3.1415926f;                 //计算倾角	
//		Gyro_Y=Gyro_Y/16.4;                                    //陀螺仪量程转换	
//		Kalman_Filter(Accel_Y,-Gyro_Y);//卡尔曼滤波	
//		
//		//printf("Pitch:%.4f  Roll:%.4f  Yaw:%.4f  Temp:%d\n",Pitch,Roll,Yaw,Read_Temperature());
//		
//		printf("Kalman_Filter::%.4f          :%.4f\n",angle,angle_dot);
//		delay_ms(5);

