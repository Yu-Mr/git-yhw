#include "adc.h"

u16 ADC_ConvertedValue;


static void ADCx_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );

	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

	GPIO_Init(ADC_PORT, &GPIO_InitStructure);				
}

static void ADCx_Mode_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
	
	// 复位DMA控制器
	DMA_DeInit(ADC_DMA_CHANNEL);	
	// 配置 DMA 初始化结构体
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADCx->DR ) );
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	
	// ADC 模式配置
	// 只使用一个ADC，属于单模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
	ADC_Init(ADCx, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);

	ADC_DMACmd(ADCx, ENABLE);

	ADC_Cmd(ADCx, ENABLE);
	
	ADC_ResetCalibration(ADCx);	// 初始化ADC 校准寄存器  
	
	while(ADC_GetResetCalibrationStatus(ADCx));	// 等待校准寄存器初始化完成
	
	
	ADC_StartCalibration(ADCx);		// ADC开始校准
	
	while(ADC_GetCalibrationStatus(ADCx));
	
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}


void ADCx_Init(void)
{
	ADCx_GPIO_Init();
	ADCx_Mode_Init();
}
