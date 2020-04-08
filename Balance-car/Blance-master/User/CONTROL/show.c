#include "show.h"


void oled_show(void)
{
	//���� float Balance_Kp=100,Balance_Kd=0.4,Velocity_Kp=50,Velocity_Ki=0.25;
	OLED_ShowString(0,0,"bKp:",12);	
	OLED_ShowNum(24,0,Balance_Kp,3,12);
	OLED_ShowString(60,0,"bKd:",12);
	OLED_ShowNum(84,0,0,Balance_Kd,12);
	OLED_ShowChar(90,0,'.',12,1);
	OLED_ShowNum(96,0,m_abs(Balance_Kd*100)%100,3,12);
	OLED_ShowString(0,12,"vKp:",12);
	OLED_ShowNum(24,12,Velocity_Kp,3,12);
	OLED_ShowString(60,12,"vKi:",12);
	OLED_ShowNum(84,12,0,1,12);
	OLED_ShowChar(90,12,'.',12,1);
	OLED_ShowNum(96,12,Velocity_Ki*100,3,12);
//	
//	//�Ƕ�	
	if(Balance_angle<0)
		OLED_ShowString(0,24,"Angle: -",12);
	else
		OLED_ShowString(0,24,"Angle: +",12);
	OLED_ShowNum(60,24,m_abs(Balance_angle),3,12);
	OLED_ShowChar(78,24,'.',12,1);
	OLED_ShowNum(84,24,m_abs(Balance_angle*100)%100,2,12);
//	
	//��ѹ �¶�
	OLED_ShowString(0,36,"Vol:",12);	
	OLED_ShowNum(24,36,voltage,3,12);
	OLED_ShowString(60,36,"T:",12);
	OLED_ShowNum(84,36,Read_Temperature(),3,12);
//	
//	//�������	
	if( nEncoder_left<0)
		OLED_ShowString(0,48,"L: -",12);
	else
		OLED_ShowString(0,48,"L: +",12);
	OLED_ShowNum(24,48,m_abs(nEncoder_left),3,12);
	//�ұ�����
	if( nEncoder_right<0)
		OLED_ShowString(60,48,"R: -",12);
	else
		OLED_ShowString(60,48,"R: +",12);
	OLED_ShowNum(84,48,m_abs(nEncoder_right),3,12);
	
		
	OLED_Refresh_Gram();
	
}
