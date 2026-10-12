#include "driver_config.h"
static volatile uint16_t adc_dma_value[5];
static volatile uint16_t adc_value[5];
static uint8_t adc_initialized;
static uint8_t adc_busy;
// ADC输入脚:I_A->PA0,I_B->PA1,IC->PA7,这里默认初始化三路电流传感器
void current_sensor_adc_init()
{
	if (adc_initialized)
	{
		return;
	}

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//开启ADC1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		//开启DMA1的时钟
	
	/*设置ADC时钟*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);						//选择时钟6分频，ADCCLK = 72MHz / 6 = 12MHz

	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA0、PA1引脚初始化为模拟输入
	
	/*规则组通道配置*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);	//规则组序列0的位置，配置为通道1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_55Cycles5); //规则组序列1的位置，配置为通道2
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_55Cycles5); //规则组序列7的位置，配置为通道3
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 4, ADC_SampleTime_55Cycles5);	//规则组序列1的位置，配置为通道2
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 5, ADC_SampleTime_55Cycles5);	//规则组序列7的位置，配置为通道3
	/*ADC初始化*/
	
	ADC_InitTypeDef ADC_InitStructure;											
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;							
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;			
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;							
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;								
	ADC_InitStructure.ADC_NbrOfChannel = 5;
	ADC_Init(ADC1, &ADC_InitStructure);											
	
	/*DMA初始化*/
	DMA_InitTypeDef DMA_InitStructure;											
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->RDATAR;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adc_dma_value;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							
	DMA_InitStructure.DMA_BufferSize = 5;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);								
		
	/*DMA和ADC使能*/
	DMA_Cmd(DMA1_Channel1, ENABLE);							//DMA1的通道1使能
	ADC_DMACmd(ADC1, ENABLE);								//ADC1触发DMA1的信号使能
	ADC_Cmd(ADC1, ENABLE);									//ADC1使能
	
	/*ADC校准*/
	ADC_ResetCalibration(ADC1);								//固定流程，内部有电路会自动执行校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
	
    adc_initialized = 1;
    adc_sample_start();
}

// Called only by startup calibration (IRQs off) or the TIM1 sampling service.
void adc_sample_start(void)
{
    if (!adc_initialized || adc_busy)
        return;
    DMA_ClearFlag(DMA1_FLAG_GL1);
    adc_busy = 1;
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

uint8_t adc_sample_publish(void)
{
    if (!adc_initialized || !adc_busy
        || DMA_GetFlagStatus(DMA1_FLAG_TE1) != RESET
        || DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET)
        return 0;

    // Single scan has stopped. Copy the entire frame before restarting.
    for (uint8_t i = 0; i < 5; ++i)
        adc_value[i] = adc_dma_value[i];
    DMA_ClearFlag(DMA1_FLAG_GL1);
    adc_busy = 0;
    return 1;
}

void battery_adc_init(void)
{
    current_sensor_adc_init();
}

uint16_t battery_adc_read(void)
{
    return adc_value[4];
}

void left_inline_current_read_adc(void)
{
    left_motor.current_struct.inline_current_adc_value[I_A] = adc_value[0];
    left_motor.current_struct.inline_current_adc_value[I_C] = adc_value[1];
}

void right_inline_current_read_adc(void)
{
    right_motor.current_struct.inline_current_adc_value[I_A] = adc_value[2];
    right_motor.current_struct.inline_current_adc_value[I_C] = adc_value[3];
}
