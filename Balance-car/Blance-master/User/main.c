#include "main.h"



u16 voltage;								//��ص�ѹ

u8 Flag_forward,Flag_back,Flag_left,Flag_right,Flag_speed=1;  //����ң����صı���
u8 Flag_stop=0,Flag_show=0;					//ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��

int nEncoder_left,nEncoder_right;				//���ұ��������������
int Motor_pwm1,Motor_pwm2;					//���PWM��

float Accel_Y,Accel_X,Accel_Z,Gyro_Y,Gyro_Z,Gyro_X;		//�������������
float Balance_angle,Balance_gyro,Turn_gyro; //ƽ����� ƽ�������� ת��������

float Balance_Kp=300,Balance_Kd=1.2,Velocity_Kp=80,Velocity_Ki=0.4;//PID���� 

u8 show;  //��ʾ��־

int main(void)
{
		//first commit-yhw
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

	//SERVO_Init(199,7199);  //20ms������  һ����0.1  0.5-2.5   22-10
	while(1)
	{
		if(show == 10)
		{
			oled_show();
			show = 0;
		}
	}
}

//�жϺ��������Ƴ���
void EXTI9_5_IRQHandler(void)
{
  //ȷ���Ƿ������EXTI Line�ж�
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
//	OLED_ShowNum(72,5,MPU6050_getDeviceID(),3,16);//��ʾ2������		
//	OLED_Refresh_Gram();


//Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //��ȡY��������
//		Gyro_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L);    //��ȡZ��������
//		Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //��ȡX����ٶȼ�
//	  	Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L); //��ȡZ����ٶȼ�
//		
//		
//		//Gyro_Balance=-Gyro_Y;                                  //����ƽ����ٶ�
//	   	Accel_Y=atan2(Accel_X,Accel_Z)*180/3.1415926f;                 //�������	
//		Gyro_Y=Gyro_Y/16.4;                                    //����������ת��	
//		Kalman_Filter(Accel_Y,-Gyro_Y);//�������˲�	
//		
//		//printf("Pitch:%.4f  Roll:%.4f  Yaw:%.4f  Temp:%d\n",Pitch,Roll,Yaw,Read_Temperature());
//		
//		printf("Kalman_Filter::%.4f          :%.4f\n",angle,angle_dot);
//		delay_ms(5);

