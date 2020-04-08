#include "Kalman.h"

float K1 =0.02; 
float _angle, _gyro; 

float Q_angle=0.001;// 陀螺仪噪声的协方差 过程噪声的协方差 
float Q_gyro=0.003;//陀螺仪漂移噪声的协方差 过程噪声的协方差 为一个一行两列矩阵
float R_angle=0.5;// 加速度测量噪声的协方差 既测量偏差

float dt=0.005;// 积分时间 滤波器采样时间5ms  

char  H_0 = 1;// H矩阵一个数

float Gyro_bias, Angle_err; //陀螺仪漂移,

float k0,k1, E; //中间变量

float K_0, K_1;  //K是卡尔曼增益 Kg

float PP[4] ={0,0,0,0};  //计算P矩阵的中间变量
float P[2][2] = { { 1, 0 },{ 0, 1 } };  //P矩阵  X 的协方差


//简易卡尔曼滤波

void Kalman_Filter(float Accel_angle,float Gyro)		
{
	//X(k|k-1)=AX(k-1|k-1)+BU(k)……….先验估计
	//|angle    | = |1  -dt||angle    | + |dt|gryo
	//|Gyro_bias|	 |0   1 ||Gyro_bias|   |0 |
	_angle+=(Gyro - Gyro_bias) * dt; 
	
/*	P(k|k-1)=A P(k-1| k-1) AT+Q……….误差协方差
	Q = |cov(angle,angle) cov(angle,gyro)| = |Q_angle     0 |
	    |cov(gyro,angle)   cov(gyro,gyro)|	 |0       Q_gyro|
	P(k-1| k-1) = |a   b|
	              |c   d|
	P(k|k-1) = |a-c*dt-b*dt+d*dt*dt   b-d*dt| + |Q_angle     0 |  (dt*dt 太小可以省略）
	           |      c-d*dt            d   |   |0       Q_gyro|
	
		    = |-c-b     -d| *dt+ |a   b| + |Q_angle     0 | 
			 | -d      0 |      |c   d|   |0       Q_gyro|
*/
	PP[0] = -P[0][1] - P[1][0]; 
	PP[1] = -P[1][1];
	PP[2] = -P[1][1];
	PP[3] = 0;
	
	P[0][0] = PP[0] * dt + P[0][0] + Q_angle;   
	P[0][1] = PP[1] * dt + P[0][1];  
	P[1][0] = PP[2] * dt + P[1][0];
	P[1][1] = PP[3] * dt + P[1][1] + Q_gyro;
/*
	Kg(k)= P(k|k-1)*HT / (H*P(k|k-1) HT + R)……….计算卡尔曼增益
	Kg 对应angle和Gyro_bias的卡尔曼增益 |K_0|
								 |K_1|
	H = |1   0|   （观测变量Z 可由测量值Accel_angle直接得出，Gyro_bias和Accel_angle无关	
	R为加速度计测量出的角度值的噪声
	P(k|k-1)*HT = |a   b| * | 1 | = | a |
	              |c   d|   | 0 |   | c |
	H*P(k|k-1)*HT = |1  0| * |a   b| * | 1 | = a
                              |c   d|   | 0 |
*/
	k0 = P[0][0];  
	k1 = P[1][0];
	
	E = R_angle + P[0][0];  // 分母 
	
	K_0 = k0 / E;
	K_1 = k1 / E;
	

/*	X(k|k)= X(k|k-1) + Kg(k)(Z(k) - H*X(k|k-1))……….修正估计
	Z = Accel_angle
	H*X(k|k-1)) = |1   0| * |angle    | = angle
	                        |Gyro_bias|
	X(k|k) = |angle    | + |K_0|  * angle
	         |Gyro_bias|   |K_1|
*/
	Angle_err = Accel_angle - _angle;	//zk-先验估计
	_angle	+= K_0 * Angle_err;	 //后验估计
	Gyro_bias	+= K_1 * Angle_err;	 //后验估计
	_gyro   = Gyro - Gyro_bias;	 //输出值(后验估计)的微分=角速度
/*
	P(k|k)=（ I-Kg(k) H） P(k|k-1)……….更新误差协方差
	I = |1  0|
	    |0  1|
	Kg(k) H P(k|k-1) = |K_0| * |1  0| * |a   b| = |K_0*a   K_0*b|
                        |K_1|            |c   d|   |K_1*a   K_1*b|

*/
	
	P[0][0] -= K_0 * P[0][0];		 //后验估计误差协方差
	P[0][1] -= K_0 * P[0][1];
	P[1][0] -= K_1 * P[0][0];
	P[1][1] -= K_1 * P[0][1];
		
	
	
}

