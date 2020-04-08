#include "oled.h"


//OLED���Դ�         
u8 OLED_GRAM[128][8];   

//��OLEDд��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void SPI_WriteByte(unsigned char dat,unsigned char cmd)
{	
	u8 i;			  
	if(cmd)
		OLED_DC = 1;
	else 
		OLED_DC = 0;		  
	for(i=0;i<8;i++)
	{			  
		OLED_SCK = 0;
		if(dat&0x80)
		   OLED_SDA = 1;
		else 
		   OLED_SDA = 0;
		OLED_SCK = 1;
		dat<<=1;   
	}				 		  
	OLED_RST = 1; 

} 
void WriteCmd(unsigned char cmd)
{
    SPI_WriteByte(cmd,OLED_CMD);
}

void WriteData(unsigned char data)
{
    SPI_WriteByte(data,OLED_DATA);
}


void OLED_DLY_ms(unsigned int ms)
{                         
  unsigned int a;
  while(ms)
  {
    a=1500;
    while(a--);
    ms--;
  }
}

//��������: ��ʼ��OLEDģ��
void OLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_2|GPIO_Pin_3;	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	OLED_SCK = 1;
	OLED_RST = 0;
	OLED_DLY_ms(100);
	OLED_RST = 1;
	
    //���ϵ絽���濪ʼ��ʼ��Ҫ���㹻��ʱ�䣬���ȴ�RC��λ���
	WriteCmd(0xAE);         // Display Off (0x00)
	WriteCmd(0xD5);
	WriteCmd(0x80);         // Set Clock as 100 Frames/Sec
	WriteCmd(0xA8);
	WriteCmd(0x3F);         // 1/64 Duty (0x0F~0x3F)
	WriteCmd(0xD3);
	WriteCmd(0x00);         // Shift Mapping RAM Counter (0x00~0x3F)
	
	WriteCmd(0x40 ); 
	
	WriteCmd(0x8D);	
    WriteCmd(0x14); 
   
    WriteCmd(0x20);
    WriteCmd(0x02);         // Set Page Addressing Mode (0x00/0x01/0x02)
    WriteCmd(0xA1);
	
	WriteCmd(0xC0);  // Set COM/x Scan Direction   
    WriteCmd(0xDA);
    WriteCmd(0x12);
	
	WriteCmd(0x81);
    WriteCmd(0xCF);         // Set SEG Output Current
    
	WriteCmd(0xD9);
    WriteCmd(0xF1);         // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    WriteCmd(0xDB);
    WriteCmd(0x40);         // Set VCOM Deselect Level
    /*WriteCmd(0xA4 | 0x00);  // Disable Entire Display On (0x00/0x01)
    WriteCmd(0xA6 | 0x00);  // Disable Inverse Display On (0x00/0x01)
    WriteCmd(0xAE | 0x01);  // Display On (0x01)
*/
	WriteCmd(0xA4); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	WriteCmd(0xA6); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	WriteCmd(0xAF); //������ʾ	 
	
    OLED_Clear();  //��ʼ����   
}
/*************************************************************************/
/*��������: ��OLED�������л���                                            */
/*************************************************************************/
void OLED_ON(void)
{
    WriteCmd(0X8D);  //���õ�ɱ�
    WriteCmd(0X14);  //������ɱ�
    WriteCmd(0XAF);  //OLED����
}
/*************************************************************************/
/*��������: ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA                      */
/*************************************************************************/
void OLED_OFF(void)
{
    WriteCmd(0X8D);  //���õ�ɱ�
    WriteCmd(0X10);  //�رյ�ɱ�
    WriteCmd(0XAE);  //OLED����
}

/*************************************************************************/
/*��������: �����Դ浽OLED                                                 */
/*************************************************************************/
void OLED_Refresh_Gram(void)
{
    u8 i,n;         
    for(i=0;i<8;i++)  
    {  
        WriteCmd(0xb0+i);   //����ҳ��ַ��0~7��
        WriteCmd(0x00);      //������ʾλ�á��е͵�ַ
        WriteCmd(0x10);      //������ʾλ�á��иߵ�ַ   
        for(n=0;n<128;n++)
		WriteData(OLED_GRAM[n][i]); 
    }   
}
/*************************************************************************/
/*��������: ����                                                          */
/*************************************************************************/
void OLED_Clear(void)  
{  
    u8 i,n;  
    for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
    OLED_Refresh_Gram();//������ʾ
}
/*************************************************************************/
/*��������: ����                                                          */
/*��ڲ�����                                                             */
/*                      x��������       0~127                               */
/*                      y��������               0~63                     */
/*                      dot:0,���;1,���                                                                                */             
/*************************************************************************/
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
    u8 pos,bx,temp=0;
    if(x>127||y>63)return;//������Χ��.
    pos=7-y/8;
    bx=y%8;
    temp=1<<(7-bx);
    if(t)OLED_GRAM[x][pos]|=temp;
    else OLED_GRAM[x][pos]&=~temp;     
}
/*************************************************************************/
/*��������: ���                                                          */
/*��ڲ�����                                                                                                                          */
/*                      x1,y1,x2,y2 �������ĶԽ�����                              */
/*                      ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63                                         */
/*                      dot:0,���;1,���                                                                                */             
/*************************************************************************/
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
    u8 x,y;  
    for(x=x1;x<=x2;x++)
    {
        for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
    }                                                       
    OLED_Refresh_Gram();//������ʾ
}

/*************************************************************************/
/*��������: ����                                                          */
/*��ڲ�����                                                                                                                          */
/*                      x:0~12                                                     */
/*                      y:0~63                                                                      */
/*                      mode:0,������ʾ;1,������ʾ                                                       */ 
/*            			size:ѡ������ 16/12                                                                           */             
/*************************************************************************/
void OLED_chinese(u8 x,u8 y,u8 *chr,u8 size,u8 mode)  
{  
	u8 temp,t,t1;
    u8 y0=y;
    u8 csize=(size/8+((size%8)?1:0))*(size/2)*2;      //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    //chr=chr-' ';//�õ�ƫ�ƺ��ֵ        
    for(t=0;t<csize;t++)
    {   
		
        if(size==12)temp=chr[t];         //����1206����
        else if(size==16)temp=chr[t];     //����1608����
        else if(size==24)temp=chr[t];     //����2412����
        else return;                                //û�е��ֿ�
        for(t1=0;t1<8;t1++)
        {
            if(temp&0x80)OLED_DrawPoint(x,y,mode);
            else OLED_DrawPoint(x,y,!mode);
            temp<<=1;
            y++;
            if((y-y0)==size)
            {
                y=y0;
                x++;
                break;
            }
        }    
    }    
}

/*************************************************************************/
/*��������: ��ָ��λ����ʾһ���ַ�,���������ַ�                             */
/*��ڲ�����                                                                                                                          */
/*                      x:0~12                                                     */
/*                      y:0~63                                                                      */
/*                      mode:0,������ʾ;1,������ʾ                                                       */ 
/*            size:ѡ������ 16/12                                        */
/*************************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{                   
    u8 temp,t,t1;
    u8 y0=y;
    u8 csize=(size/8+((size%8)?1:0))*(size/2);      //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    chr=chr-' ';//�õ�ƫ�ƺ��ֵ        
    for(t=0;t<csize;t++)
    {   
        if(size==12)temp=asc2_1206[chr][t];         //����1206����
        else if(size==16)temp=asc2_1608[chr][t];    //����1608����
        else if(size==24)temp=asc2_2412[chr][t];    //����2412����
        else return;                                //û�е��ֿ�
        for(t1=0;t1<8;t1++)
        {
            if(temp&0x80)OLED_DrawPoint(x,y,mode);
            else OLED_DrawPoint(x,y,!mode);
            temp<<=1;
            y++;
            if((y-y0)==size)
            {
                y=y0;
                x++;
                break;
            }
        }    
    }          
}
//m^n����
u32 mypow(u8 m,u8 n)
{
    u32 result=1;    
    while(n--)result*=m;    
    return result;
}                 
/*************************************************************************/
/*��������: ��ʾ2������                                                   */
/*��ڲ�����                                                             */
/*                      x,y :�������                                     */
/*                      len :���ֵ�λ��                                   */
/*                      size:�����С                                        */ 
/*            mode:ģʽ   0,���ģʽ;1,����ģʽ                             */
/*            num:��ֵ(0~4294967295)                                     */
/*************************************************************************/       
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{           
    u8 t,temp;
    u8 enshow=0;                           
    for(t=0;t<len;t++)
    {
        temp=(num/mypow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
                continue;
            }else enshow=1; 

        }
        OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
    }
} 
/*************************************************************************/
/*��������: ��ʾ�ַ���                                                                            */
/*��ڲ�����                                                                                                                          */
/*                      x,y:�������                                                */
/*                      size:�����С                                                          */
/*                      *p:�ַ�����ʼ��ַ                                                               */ 
/*************************************************************************/
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{   
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);    
        x+=size/2;
        p++;
    }  

}   




