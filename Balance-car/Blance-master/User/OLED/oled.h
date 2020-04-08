#ifndef __OLED_H
#define __OLED_H

/* includes */
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "oledfont.h"
#include "sys.h"
#include "delay.h"


#if 0
#define     RCC_APB2Periph_OLED_PORT        RCC_APB2Periph_GPIOA

#define     OLED_PORT                       GPIOC
#define     OLED_RD_PORT                       GPIOA
#define     OLED_RST_PIN                    GPIO_Pin_0

#define     OLED_RST_L                      GPIO_ResetBits(GPIOA, GPIO_Pin_12)
#define     OLED_RST_H                      GPIO_SetBits(GPIOA, GPIO_Pin_12)

#define     OLED_DC_PIN                     GPIO_Pin_1

#define	    OLED_DC_L                       GPIO_ResetBits(GPIOA, GPIO_Pin_11);
#define     OLED_DC_H                       GPIO_SetBits(GPIOA, GPIO_Pin_11);

#define  	OLED_SCK_PIN					GPIO_Pin_5
#define		OLED_SDA_PIN					GPIO_Pin_7
#endif

#if 1
#define OLED_CMD 0   //��������
#define OLED_DATA 1 //��������
/*
#define OLED_RST_Clr 	PAout(2)=0   //RST
#define OLED_RST = 1 	PAout(2)=1   //RST

#define OLED_SDA_Clr  PAout(1)=0   //SDA
#define OLED_SDA = 1  PAout(1)=1   //SDA

#define OLED_DC = 0 	PAout(3)=0    //DC
#define OLED_DC = 1 	PAout(3)=1    //DC

#define OLED_SCLK_Clr  PAout(0)=0  //SCL
#define OLED_SCK = 1  PAout(0)=1   //SCL
*/
#define OLED_SCK	PAout(2)
#define OLED_SDA 	PAout(3)
#define OLED_RST	PAout(4)
#define OLED_DC 	PAout(5) 

#endif

 
void SPI_WriteByte(uint8_t addr,uint8_t data); //��Ĵ�����ַдһ��byte������
void WriteCmd(unsigned char cmd); //д����
void WriteData(unsigned char data); //д���� 
void OLED_Init(void);//��ʼ��OLED
void OLED_ON(void);//����OLED
void OLED_OFF(void);//OLED����
void OLED_Refresh_Gram(void);//�����Դ浽OLED
void OLED_Clear(void);//����
void OLED_DrawPoint(u8 x,u8 y,u8 t);//����
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);//���
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);//��ʾ�ַ�
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);//��ʾ2������
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);//��ʾ�ַ���
void OLED_chinese(u8 x,u8 y,u8*chr,u8 size,u8 mode);//��ʾ����



#endif
