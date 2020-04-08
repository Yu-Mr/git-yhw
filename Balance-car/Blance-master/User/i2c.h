#ifndef I2C_H
#define I2C_H

#include "stm32f10x.h"
#include "usart.h"
#include "sys.h"
#include "delay.h"



//MPU6050��AD0���Ž�GNDʱ����ַΪ0x68 ,��3.3Vʱ����ַΪ0x69
#define MPU6050_ADDR   0x68


/*********************���IICʹ�õĺ�****************************/

//
#define Soft_IIC_SDA 		GPIO_Pin_9
#define Soft_IIC_SCL 		GPIO_Pin_8
#define Soft_IIC_PORT   		GPIOB
//
#define IIC_SCL 		PBout(8)
#define IIC_SDA 		PBout(9)

#define READ_SDA		GPIO_ReadInputDataBit(Soft_IIC_PORT, Soft_IIC_SDA)


void IIC_Init(void);                //��ʼ��IIC��IO��				 
int IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
int IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
unsigned char I2C_Readkey(unsigned char I2C_Addr);

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data);
u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data);
u8 IICwriteBit(u8 dev,u8 reg,u8 bitNum,u8 data);
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data);

int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
																					

#endif 

																					

																					

																					

