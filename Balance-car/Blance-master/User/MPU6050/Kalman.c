#include "Kalman.h"

float K1 =0.02; 
float _angle, _gyro; 

float Q_angle=0.001;// ������������Э���� ����������Э���� 
float Q_gyro=0.003;//������Ư��������Э���� ����������Э���� Ϊһ��һ�����о���
float R_angle=0.5;// ���ٶȲ���������Э���� �Ȳ���ƫ��

float dt=0.005;// ����ʱ�� �˲�������ʱ��5ms  

char  H_0 = 1;// H����һ����

float Gyro_bias, Angle_err; //������Ư��,

float k0,k1, E; //�м����

float K_0, K_1;  //K�ǿ��������� Kg

float PP[4] ={0,0,0,0};  //����P������м����
float P[2][2] = { { 1, 0 },{ 0, 1 } };  //P����  X ��Э����


//���׿������˲�

void Kalman_Filter(float Accel_angle,float Gyro)		
{
	//X(k|k-1)=AX(k-1|k-1)+BU(k)������.�������
	//|angle    | = |1  -dt||angle    | + |dt|gryo
	//|Gyro_bias|	 |0   1 ||Gyro_bias|   |0 |
	_angle+=(Gyro - Gyro_bias) * dt; 
	
/*	P(k|k-1)=A P(k-1| k-1) AT+Q������.���Э����
	Q = |cov(angle,angle) cov(angle,gyro)| = |Q_angle     0 |
	    |cov(gyro,angle)   cov(gyro,gyro)|	 |0       Q_gyro|
	P(k-1| k-1) = |a   b|
	              |c   d|
	P(k|k-1) = |a-c*dt-b*dt+d*dt*dt   b-d*dt| + |Q_angle     0 |  (dt*dt ̫С����ʡ�ԣ�
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
	Kg(k)= P(k|k-1)*HT / (H*P(k|k-1) HT + R)������.���㿨��������
	Kg ��Ӧangle��Gyro_bias�Ŀ��������� |K_0|
								 |K_1|
	H = |1   0|   ���۲����Z ���ɲ���ֵAccel_angleֱ�ӵó���Gyro_bias��Accel_angle�޹�	
	RΪ���ٶȼƲ������ĽǶ�ֵ������
	P(k|k-1)*HT = |a   b| * | 1 | = | a |
	              |c   d|   | 0 |   | c |
	H*P(k|k-1)*HT = |1  0| * |a   b| * | 1 | = a
                              |c   d|   | 0 |
*/
	k0 = P[0][0];  
	k1 = P[1][0];
	
	E = R_angle + P[0][0];  // ��ĸ 
	
	K_0 = k0 / E;
	K_1 = k1 / E;
	

/*	X(k|k)= X(k|k-1) + Kg(k)(Z(k) - H*X(k|k-1))������.��������
	Z = Accel_angle
	H*X(k|k-1)) = |1   0| * |angle    | = angle
	                        |Gyro_bias|
	X(k|k) = |angle    | + |K_0|  * angle
	         |Gyro_bias|   |K_1|
*/
	Angle_err = Accel_angle - _angle;	//zk-�������
	_angle	+= K_0 * Angle_err;	 //�������
	Gyro_bias	+= K_1 * Angle_err;	 //�������
	_gyro   = Gyro - Gyro_bias;	 //���ֵ(�������)��΢��=���ٶ�
/*
	P(k|k)=�� I-Kg(k) H�� P(k|k-1)������.�������Э����
	I = |1  0|
	    |0  1|
	Kg(k) H P(k|k-1) = |K_0| * |1  0| * |a   b| = |K_0*a   K_0*b|
                        |K_1|            |c   d|   |K_1*a   K_1*b|

*/
	
	P[0][0] -= K_0 * P[0][0];		 //����������Э����
	P[0][1] -= K_0 * P[0][1];
	P[1][0] -= K_1 * P[0][0];
	P[1][1] -= K_1 * P[0][1];
		
	
	
}

