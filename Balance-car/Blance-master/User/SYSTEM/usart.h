#ifndef __USART_H
#define __USART_H
//#include "stdio.h"	
 
#include "stm32f10x.h"
#include <stdio.h>
#include "main.h"


void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void uart_init(u32 bound);
void uart3_init(u32 bound);
#endif


