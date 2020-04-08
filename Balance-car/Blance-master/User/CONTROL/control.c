#include "control.h"

int balancePWM,velocityPWM,turnPWM;
u8 flag;



//获得角度
static void Get_Angle(void)
{ 
	
	//读取X轴陀螺仪
	ReadGyro_X(&Gyro_X);
	//读取Y轴陀螺仪
	ReadGyro_Y(&Gyro_Y);
	//读取Z轴陀螺仪
	ReadGyro_Z(&Gyro_Z);
	//读取Y轴加速度计
	ReadAccel_Y(&Accel_Y);	
	//读取X轴加速度计
	ReadAccel_X(&Accel_X);
	//读取Z轴加速度计
	ReadAccel_Z(&Accel_Z);
	
	
#if 1	
	Balance_gyro=Gyro_X;                                  //更新平衡角速度
	Accel_X=atan2(Accel_Y,Accel_Z)*180/PI;                 //计算倾角	
	Gyro_X=Gyro_X/16.4;                                    //陀螺仪量程转换
	
	Kalman_Filter(Accel_X,Gyro_X);					//卡尔曼滤波	
	
#else
	Balance_gyro=-Gyro_Y;                                  //更新平衡角速度
	Accel_Y=atan2(Accel_X,Accel_Z)*180/PI;                 //计算倾角	
	Gyro_Y=Gyro_Y/16.4;                                    //陀螺仪量程转换
	
	Kalman_Filter(Accel_Y,-Gyro_Y);					//卡尔曼滤波	

#endif
	//Balance_angle=angle;                                   //更新平衡倾角
	Balance_angle=_angle; 
	Turn_gyro=Gyro_Z;                                      //更新转向角速度

}

//赋值给PWM寄存器
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

//异常关闭电机
static u8 EmergencyStop(float angle, int voltage)
{
	u8 temp;
	//电池电压过低、倾角大于40度关闭电机
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

//直立环 PD控制
static int balance(void)
{  
	float Bias;
	int balance;
	Bias=Balance_angle-ZHONGZHI;       //求出平衡的角度中值
	balance=Balance_Kp*Bias+Balance_gyro*Balance_Kd;   //计算平衡控制的电机PWM  PD控制
	return balance;
}


//速度环 PI控制 遥控前进后退  Target_Velocity为小车速度
static int velocity(void)
{  
	static float Velocity,EncoderBias,Encoder,Movement,Encoder_Integral;
	float Target_Velocity=40;
	
	if(Flag_speed)
		Target_Velocity = 80;
	else
		Target_Velocity = 40;
	
	//接收遥控器数据，控制前进后退
	if(Flag_forward)
		Movement=-Target_Velocity;	     
	else if(Flag_back)
		Movement=Target_Velocity;        
	else
		Movement=0;	
	
	//速度PI控制器	
	EncoderBias =(nEncoder_left+nEncoder_right)-0;	//获取最新速度偏差 测量速度-目标速度
	//一阶低通滤波器
	Encoder *= 0.7;		                                                       
	Encoder += EncoderBias*0.3;
	
	Encoder_Integral +=Encoder; 					//积分出位移 积分时间：10ms
	Encoder_Integral=Encoder_Integral-Movement; 		
	
	//积分限幅
	if(Encoder_Integral>15000)
		Encoder_Integral=15000;             
	if(Encoder_Integral<-15000)
		Encoder_Integral=-15000;             
	
	Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;
	
	//电机关闭后清除积分
	if(EmergencyStop(Balance_angle,voltage))
		Encoder_Integral=0;
	
	return Velocity;
}

//转向环
static int turn()
{
	static float Turn_Target,Kp=30;
	float Turn_Convert=0.5,Turn,Kd=0;
	u8 Turn_Amplitude=25; 
	

	 //接收转向遥控数据
	if(Flag_left)
		Turn_Target -= Turn_Convert;       
	else if(Flag_right)
		Turn_Target += Turn_Convert;        
	else
		Turn_Target = 0,Kd = 1;      
	
	//转向速度限幅
	if(Turn_Target>Turn_Amplitude)
		Turn_Target = Turn_Amplitude;    
	if(Turn_Target<-Turn_Amplitude)
		Turn_Target = -Turn_Amplitude;  
                                  
  
	Turn = Turn_Target*Kp+Turn_gyro*Kd;	//结合Z轴陀螺仪进行PD控制
	return Turn;
}


//限制PWM赋值 
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
		//读取电压，求多次平均值
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

//绝对值
int m_abs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

