
#include "usart.h"	  



void uart_init(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA9
   
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA10  


	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); 
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	USART_Cmd(USART1, ENABLE);                  

}

void uart3_init(u32 bound)
{  	 
	  //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能UGPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3时钟
	//USART3_TX  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
   
	//USART3_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure);     //初始化串口3
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);                    
}


void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
int fputc(int ch, FILE *f)
{      
	/* 发送一个字节数据到串口 */
	USART_SendData(USART3, (uint8_t) ch);
		
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);		
	return (ch);
	

}


///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

u8 res;
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据
	{	  
		//static int uart_receive=0;//蓝牙接收相关变量
		static u8 Flag_PID,i,j,Receive[50];
		static float Data;
		//uart_receive=USART_ReceiveData(USART3); 
		res=USART_ReceiveData(USART3);
	
		if(res==0x59)  Flag_speed=0;  //低速
		if(res==0x58)  Flag_speed=1;  //高速
		
			
		if(res==0x5A)
			Flag_forward=0,Flag_back=0,Flag_left=0,Flag_right=0;	//刹车Z
			
		else if(res==0x41)	
			Flag_forward=1,Flag_back=0,Flag_left=0,Flag_right=0;	//前A
			
		else if(res==0x43)	
			Flag_forward=0,Flag_back=1,Flag_left=0,Flag_right=0;	//后C
			
		else if(res==0x42)	
			/*Flag_forward=0,Flag_back=0,*/Flag_left=0,Flag_right=1;	//右D
			
		else if(res==0x44)	    
			/*Flag_forward=0,Flag_back=0,*/Flag_left=1,Flag_right=0;	//左C
		else 
			Flag_forward=0,Flag_back=0,Flag_left=0,Flag_right=0;	//刹车


		if(res==0x7B) Flag_PID=1;  //指令起始位
		if(res==0x7D) Flag_PID=2;  //指令停止位
		 if(Flag_PID==1)                      //记录参数
		 {
			Receive[i]=res;
			i++;
		 }
		 if(Flag_PID==2)   //执行
		 {
			if(Receive[1]!=0x23)                    //更新PID参数
			{								
				for(j=i;j>=4;j--)
					Data+=(Receive[j-1]-48)*pow(10,i-j);

				switch(Receive[1])
				{
					case 0x30: 
						Balance_Kp=Data/100;break;
					case 0x31:  
						Balance_Kd=Data/100;break;
					case 0x32:  
						Velocity_Kp=Data/100;break;
					case 0x33:  
						Velocity_Ki=Data/100;break;
					
					case 0x34:  break;
					case 0x35:  break;
					case 0x36:  break;
					case 0x37:  break;
					case 0x38:  break;
				}
			}				 
			Flag_PID=0;   
			i=0;
			j=0;
			Data=0;
			memset(Receive, 0, sizeof(u8)*50);
		 } 
	}  											 
} 










