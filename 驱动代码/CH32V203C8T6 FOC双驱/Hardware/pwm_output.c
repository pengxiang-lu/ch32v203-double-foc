#include "driver_config.h"

void left_motor_output_init(uint16_t timer_period)
{
    // 左电机 PWM 输出初始化
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // 使能 GPIOB 和 TIM4 外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 ,ENABLE);
    // 配置左电机三相 PWM 引脚
    GPIO_InitStructure.GPIO_Pin = L_MOTOR_A_PHASE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(L_MOTOR_A_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = L_MOTOR_B_PHASE_PIN;
    GPIO_Init(L_MOTOR_B_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = L_MOTOR_C_PHASE_PIN;
    GPIO_Init(L_MOTOR_C_PHASE_PORT, &GPIO_InitStructure);

    // 左电机三相 PWM 引脚配置完成

    // 配置 TIM4 时基
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up   ;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                        // 定时器不分频
    TIM_TimeBaseStructure.TIM_Period = timer_period-1; 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    #if PWM_MODE == SVPWM_5 || PWM_MODE == SVPWM_7                  // SVPWM 使用中心对齐计数模式
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    #else                                                           // SPWM 使用向上计数模式
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    #endif
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    // 配置 TIM4 输出比较通道
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable ;
    TIM_OCInitStructure.TIM_Pulse=0;// 初始比较值为 0

    // 初始化 TIM4 通道 1
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    // 初始化 TIM4 通道 2
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    // 初始化 TIM4 通道 3
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);

    // 开启比较寄存器预装载，在更新事件时统一生效
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    // 使能 TIM4
    TIM_Cmd(TIM4, ENABLE);
    // 左电机 PWM 输出初始化完成
}
void right_motor_output_init(uint16_t timer_period)
{
    // 右电机 PWM 输出初始化
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // 使能 GPIOA 和 TIM2 外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);
    // 配置右电机三相 PWM 引脚
    GPIO_InitStructure.GPIO_Pin = R_MOTOR_A_PHASE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(R_MOTOR_A_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = R_MOTOR_B_PHASE_PIN;
    GPIO_Init(R_MOTOR_B_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = R_MOTOR_C_PHASE_PIN;
    GPIO_Init(R_MOTOR_C_PHASE_PORT, &GPIO_InitStructure);

    // 右电机三相 PWM 引脚配置完成

    // 配置 TIM2 时基
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                        // 定时器不分频
    TIM_TimeBaseStructure.TIM_Period = timer_period-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    #if PWM_MODE == SVPWM_5 || PWM_MODE == SVPWM_7                  // SVPWM 使用中心对齐计数模式
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    #else                                                           // SPWM 使用向上计数模式
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    #endif
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 配置 TIM2 输出比较通道
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable ;
    TIM_OCInitStructure.TIM_Pulse=0;// 初始比较值为 0

    // 初始化 TIM2 通道 1
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    // 初始化 TIM2 通道 2
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    // 初始化 TIM2 通道 3
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    // 开启比较寄存器预装载，在更新事件时统一生效
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);


    // 使能 TIM2
    TIM_Cmd(TIM2, ENABLE);
    // 右电机 PWM 输出初始化完成
}
// 初始化电机控制回调定时器
void motor_callback_init()
{
    // TIM1 位于 APB2，总线分频后定时器时钟自动倍频
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_initStructure;

    // 配置 TIM1 时基
    TIM_InternalClockConfig(TIM1);  // 使用内部时钟
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟不再分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式
    TIM_TimeBaseStructure.TIM_Prescaler = 99;  // 144MHz 经 100 分频得到 1.44MHz
    TIM_TimeBaseStructure.TIM_Period = 71;  // 计数 72 次产生更新事件，控制频率为 20kHz
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  // 每个计数周期均产生更新事件
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM1, TIM_FLAG_Update);  // 清除更新标志
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);  // 使能更新中断
    TIM_Cmd(TIM1, ENABLE);  // 启动 TIM1
    // TIM1 时基配置完成

    // 配置 TIM1 更新中断
    NVIC_initStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  // 选择 TIM1 更新中断通道
    NVIC_initStructure.NVIC_IRQChannelCmd = ENABLE;  // 使能中断通道
    NVIC_initStructure.NVIC_IRQChannelPreemptionPriority = 1;  // 最高抢占优先级，避免控制环被低优先级任务阻塞
    NVIC_initStructure.NVIC_IRQChannelSubPriority = 0;  // 子优先级为 0
    NVIC_Init(&NVIC_initStructure);
    // TIM1 更新中断配置完成
}


void left_motor_duty_set(uint16_t a_duty, uint16_t b_duty, uint16_t c_duty)
{// 左电机通道映射：A 相为 TIM4_CH3，B 相为 TIM4_CH2，C 相为 TIM4_CH1
    // 暂停更新事件，避免装载只写入一部分的三相比较值
    uint16_t update_disabled = TIM4->CTLR1 & TIM_UDIS;
    TIM4->CTLR1 |= TIM_UDIS;
    TIM4->CH3CVR= a_duty;
    TIM4->CH2CVR= b_duty;
    TIM4->CH1CVR= c_duty;
    if (!update_disabled)
        TIM4->CTLR1 &= (uint16_t)~TIM_UDIS;
}
void right_motor_duty_set(uint16_t a_duty, uint16_t b_duty, uint16_t c_duty)
{// 右电机通道映射：A 相为 TIM2_CH1，B 相为 TIM2_CH2，C 相为 TIM2_CH3
    // 暂停更新事件，避免装载只写入一部分的三相比较值
    uint16_t update_disabled = TIM2->CTLR1 & TIM_UDIS;
    TIM2->CTLR1 |= TIM_UDIS;
    TIM2->CH1CVR= a_duty;
    TIM2->CH2CVR= b_duty;
    TIM2->CH3CVR= c_duty;
    if (!update_disabled)
        TIM2->CTLR1 &= (uint16_t)~TIM_UDIS;
}







