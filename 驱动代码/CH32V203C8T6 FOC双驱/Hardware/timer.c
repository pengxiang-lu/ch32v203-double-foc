#include "driver_config.h"

// 全局变量存储溢出次数
static uint32_t timer4_overflow = 0;

// 定时器4初始化，配置为1us计数
void TIM4_US_Init(void)
{
    // 使能定时器4时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;           // 最大计数值(65535)
    TIM_TimeBaseStructure.TIM_Prescaler = 143;            // 144MHz/(143+1)=1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    
    // 清除更新标志位
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    
    // 关闭定时器
    TIM_Cmd(TIM4, DISABLE);
}

// 启动计时
void TIM4_Start(void)
{
    timer4_overflow = 0;              // 重置溢出计数
    TIM4->CNT = 0;                    // 重置定时器计数
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);  // 清除标志位
    TIM_Cmd(TIM4, ENABLE);            // 启动定时器
}

// 停止计时并返回微秒数
uint32_t TIM4_Stop(void)
{
    uint32_t current_count;
    
    TIM_Cmd(TIM4, DISABLE);           // 停止定时器
    current_count = TIM4->CNT;        // 获取当前计数值
    
    // 检查是否有未处理的溢出
    if (TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) != RESET)
    {
        timer4_overflow++;
    }

    // 返回总微秒数(溢出次数*65536 + 当前计数值)
    return (timer4_overflow * 0x10000 + current_count);
}

// 获取当前计时值(不停止计时)
uint32_t TIM4_GetCurrentTime(void)
{
    uint32_t current_count;
    
    // 读取当前计数值
    current_count = TIM4->CNT;
    
    // 检查是否发生溢出
    if (TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) != RESET)
    {
        // 清除溢出标志
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);
        // 增加溢出计数
        timer4_overflow++;
        // 重新读取计数值(可能已经再次计数)
        current_count = TIM4->CNT;
    }
    
    return (timer4_overflow * 0x10000 + current_count);
}
