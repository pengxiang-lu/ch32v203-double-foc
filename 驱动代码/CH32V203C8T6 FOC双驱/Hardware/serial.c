#include "driver_config.h"

// 定时器3初始化函数,用于初始化回传中断 - 配置为1ms中断一次
void TIM3_Init(void) 
{
    // 使能定时器3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    // 定时器基础配置
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 143;           				// 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 999;      				// 预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;     				// 时钟分频因子
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	// 向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    // 使能定时器3更新中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    
    // 配置NVIC中断
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  			// 定时器3中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	// 抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        	// 子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           	// 使能中断通道
    NVIC_Init(&NVIC_InitStructure);
    // 使能定时器3
    TIM_Cmd(TIM3, ENABLE);
}
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
// 定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
	{
        // 清除中断标志位
		motor_uart_data_callback();
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}
/**
  * 函    数：串口初始化
  * 参    数：无
  * 返 回 值：无
  */
void Serial_Init(void)
{
    USART_Printf_Init(460800);
}



void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
/**
  * 函    数：获取串口接收的数据
  * 参    数：无
  * 返 回 值：接收的数据，范围：0~255
  */
void USART1_IRQHandler(void)
{   
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//判断是否是USART1的接收事件触发的中断
	{	uint8_t RxData=USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		motor_driver_control_callback(RxData);
	}
}

