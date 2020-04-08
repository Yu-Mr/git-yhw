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
#define OLED_CMD 0   //命令声明
#define OLED_DATA 1 //数据声明
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

 
void SPI_WriteByte(uint8_t addr,uint8_t data); //向寄存器地址写一个byte的数据
void WriteCmd(unsigned char cmd); //写命令
void WriteData(unsigned char data); //写数据 
void OLED_Init(void);//初始化OLED
void OLED_ON(void);//唤醒OLED
void OLED_OFF(void);//OLED休眠
void OLED_Refresh_Gram(void);//更新显存到OLED
void OLED_Clear(void);//清屏
void OLED_DrawPoint(u8 x,u8 y,u8 t);//画点
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);//填充
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);//显示字符
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);//显示2个数字
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);//显示字符串
void OLED_chinese(u8 x,u8 y,u8*chr,u8 size,u8 mode);//显示汉字



#endif
