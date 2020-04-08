#include "control.h"

int balancePWM,velocityPWM,turnPWM;
u8 flag;



//��ýǶ�
static void Get_Angle(void)
{ 
	
	//��ȡX��������
	ReadGyro_X(&Gyro_X);
	//��ȡY��������
	ReadGyro_Y(&Gyro_Y);
	//��ȡZ��������
	ReadGyro_Z(&Gyro_Z);
	//��ȡY����ٶȼ�
	ReadAccel_Y(&Accel_Y);	
	//��ȡX����ٶȼ�
	ReadAccel_X(&Accel_X);
	//��ȡZ����ٶȼ�
	ReadAccel_Z(&Accel_Z);
	
	
#if 1	
	Balance_gyro=Gyro_X;                                  //����ƽ����ٶ�
	Accel_X=atan2(Accel_Y,Accel_Z)*180/PI;                 //�������	
	Gyro_X=Gyro_X/16.4;                                    //����������ת��
	
	Kalman_Filter(Accel_X,Gyro_X);					//�������˲�	
	
#else
	Balance_gyro=-Gyro_Y;                                  //����ƽ����ٶ�
	Accel_Y=atan2(Accel_X,Accel_Z)*180/PI;                 //�������	
	Gyro_Y=Gyro_Y/16.4;                                    //����������ת��
	
	Kalman_Filter(Accel_Y,-Gyro_Y);					//�������˲�	

#endif
	//Balance_angle=angle;                                   //����ƽ�����
	Balance_angle=_angle; 
	Turn_gyro=Gyro_Z;                                      //����ת����ٶ�

}

//��ֵ��PWM�Ĵ���
static void Set_Pwm(void)
{
	int DeadBand=400;
	if(Motor_pwm1<0)
	{
		AIN2=0;
		AIN1=1;
	}
	else 
	{
		AIN2=1;
		AIN1=0;
	}
	PWMA = m_abs(Motor_pwm1)+ DeadBand;
	if(Motor_pwm2<0)
	{
		BIN1=0;
		BIN2=1;
	}
	else 
	{
		BIN1=1;
		BIN2=0;
	}
	PWMB = m_abs(Motor_pwm2)+ DeadBand;	
}

//�쳣�رյ��
static u8 EmergencyStop(float angle, int voltage)
{
	u8 temp;
	//��ص�ѹ���͡���Ǵ���40�ȹرյ��
	if(angle<-30||angle>30||Flag_stop==1||voltage<1)
	{	                                                 
		temp = 1;                                           
		AIN1 = 0;                                            
		AIN2 = 0;
		BIN1 = 0;
		BIN2 = 0;
      } 
	else
		temp = 0;
	return temp;			
}

//ֱ���� PD����
static int balance(void)
{  
	float Bias;
	int balance;
	Bias=Balance_angle-ZHONGZHI;       //���ƽ��ĽǶ���ֵ
	balance=Balance_Kp*Bias+Balance_gyro*Balance_Kd;   //����ƽ����Ƶĵ��PWM  PD����
	return balance;
}


//�ٶȻ� PI���� ң��ǰ������  Target_VelocityΪС���ٶ�
static int velocity(void)
{  
	static float Velocity,EncoderBias,Encoder,Movement,Encoder_Integral;
	float Target_Velocity=40;
	
	if(Flag_speed)
		Target_Velocity = 80;
	else
		Target_Velocity = 40;
	
	//����ң�������ݣ�����ǰ������
	if(Flag_forward)
		Movement=-Target_Velocity;	     
	else if(Flag_back)
		Movement=Target_Velocity;        
	else
		Movement=0;	
	
	//�ٶ�PI������	
	EncoderBias =(nEncoder_left+nEncoder_right)-0;	//��ȡ�����ٶ�ƫ�� �����ٶ�-Ŀ���ٶ�
	//һ�׵�ͨ�˲���
	Encoder *= 0.7;		                                                       
	Encoder += EncoderBias*0.3;
	
	Encoder_Integral +=Encoder; 					//���ֳ�λ�� ����ʱ�䣺10ms
	Encoder_Integral=Encoder_Integral-Movement; 		
	
	//�����޷�
	if(Encoder_Integral>15000)
		Encoder_Integral=15000;             
	if(Encoder_Integral<-15000)
		Encoder_Integral=-15000;             
	
	Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;
	
	//����رպ��������
	if(EmergencyStop(Balance_angle,voltage))
		Encoder_Integral=0;
	
	return Velocity;
}

//ת��
static int turn()
{
	static float Turn_Target,Kp=30;
	float Turn_Convert=0.5,Turn,Kd=0;
	u8 Turn_Amplitude=25; 
	

	 //����ת��ң������
	if(Flag_left)
		Turn_Target -= Turn_Convert;       
	else if(Flag_right)
		Turn_Target += Turn_Convert;        
	else
		Turn_Target = 0,Kd = 1;      
	
	//ת���ٶ��޷�
	if(Turn_Target>Turn_Amplitude)
		Turn_Target = Turn_Amplitude;    
	if(Turn_Target<-Turn_Amplitude)
		Turn_Target = -Turn_Amplitude;  
                                  
  
	Turn = Turn_Target*Kp+Turn_gyro*Kd;	//���Z�������ǽ���PD����
	return Turn;
}


//����PWM��ֵ 
static void PWM_limit(void)
{	
	int Amplitude=6800;    
	if(Motor_pwm1<-Amplitude) 
		Motor_pwm1 = -Amplitude;	
	if(Motor_pwm1>Amplitude)
		Motor_pwm1 = Amplitude;	
	if(Motor_pwm2<-Amplitude)
		Motor_pwm2 = -Amplitude;	
	if(Motor_pwm2>Amplitude)  
		Motor_pwm2 = Amplitude;			
}

static void Get_Voltage(void)
{
	float v;
	v = ADC_ConvertedValue*3.3f/4096;
	v = v * 5 - 7.5f;
	v = v/0.9f;
	voltage = v*100;
}

void Balance_control(void)
{
	u8 v_count;
	u16 v_sum;
	flag = !flag;
	if(flag == 0)
	{	
		Get_Angle();
		//��ȡ��ѹ������ƽ��ֵ
		Get_Voltage();
		v_count++;
		v_sum += voltage;
		if(v_count == 50)
		{
			voltage = voltage/50;
			v_count = 0;
			v_sum = 0;
		}
		return;
	}
	
	nEncoder_left = -Read_Encoder(2);
	nEncoder_right = Read_Encoder(4);
	
	Get_Angle();
	//Read_Distane();
	
	balancePWM = balance();
	velocityPWM = velocity();
	turnPWM = turn();
	
	Motor_pwm1 = balancePWM + velocityPWM - turnPWM;
	Motor_pwm2 = balancePWM + velocityPWM + turnPWM;
	PWM_limit();
	
	if(!EmergencyStop(Balance_angle,voltage))
		Set_Pwm();
}

//����ֵ
int m_abs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

