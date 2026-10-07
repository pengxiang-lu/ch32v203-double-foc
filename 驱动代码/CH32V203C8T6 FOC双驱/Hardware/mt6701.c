#include "driver_config.h"


void MT6701_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;
    
    // 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_GPIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // 配置SPI1引脚
    // SCK, MOSI 推挽输出
    GPIO_InitStructure.GPIO_Pin = CLK_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽输出
    GPIO_Init(GPIO_Port, &GPIO_InitStructure);
    
    // MISO 浮空输入
    GPIO_InitStructure.GPIO_Pin = DO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // 浮空输入
    GPIO_Init(GPIO_Port, &GPIO_InitStructure);
    
    // 配置片选引脚
    GPIO_InitStructure.GPIO_Pin = L_CSN_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
    GPIO_Init(L_CSN_Port, &GPIO_InitStructure);
    // 配置片选引脚
    GPIO_InitStructure.GPIO_Pin = R_CSN_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
    GPIO_Init(R_CSN_Port, &GPIO_InitStructure);
    // 初始片选拉高
    L_CSN_H;
    R_CSN_H;
     // 配置SPI1
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  		// 仅接收模式
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                  				// 主机模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;            				// 16位数据
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                    				// 时钟极性，根据从机设置
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                  				// 时钟相位，根据从机设置
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                      				// 软件控制片选
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  		// MT6701最大支持SPI时钟频率16MHz，这里设置为9MHz
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;             				// 高位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;                       				// CRC无用，随便设置
    SPI_Init(SPI2, &SPI_InitStructure);
    
    // 使能SPI1
    SPI_Cmd(SPI2, ENABLE);
}

uint16_t L_MT6701_GetRawAngle()
{
	static uint16_t receivedData = 0;

    L_CSN_L;
	  // 等待发送缓冲区为空
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	
	SPI_I2S_SendData(SPI2, 0x0000);

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    
    receivedData = SPI_I2S_ReceiveData(SPI2);
    
    L_CSN_H;
   
    return (receivedData>>2);
}
uint16_t R_MT6701_GetRawAngle()
{
    static uint16_t receivedData = 0;

    R_CSN_L;
      // 等待发送缓冲区为空
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

    SPI_I2S_SendData(SPI2, 0x0000);

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

    receivedData = SPI_I2S_ReceiveData(SPI2);

    R_CSN_H;

    return (receivedData>>2);
}
