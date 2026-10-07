#include "driver_config.h"

void left_motor_output_init(uint16_t timer_period)
{
    /******************************定义局部参数******************************/
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /*******************************时钟初始化******************************/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 ,ENABLE);
    /******************************配置PWM引脚******************************/
    GPIO_InitStructure.GPIO_Pin = L_MOTOR_A_PHASE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(L_MOTOR_A_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = L_MOTOR_B_PHASE_PIN;
    GPIO_Init(L_MOTOR_B_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = L_MOTOR_C_PHASE_PIN;
    GPIO_Init(L_MOTOR_C_PHASE_PORT, &GPIO_InitStructure);

    /******************************配置PWM引脚******************************/

    /*****************************配置PWM定时器******************************/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up   ;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                        //不分频，相当于72Mhz时钟直接输入定时器
    TIM_TimeBaseStructure.TIM_Period = timer_period-1; 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    #if PWM_MODE == SVPWM_5 || PWM_MODE == SVPWM_7                  // 如果使用SVPWM调制，则使用中心对齐
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    #else                                                           // 如果使用SPWM调制，则使用边沿对齐
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    #endif
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    // 配置定时器4的PWM模式
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable ;
    TIM_OCInitStructure.TIM_Pulse=0;//CCR

    // 配置通道1
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    // 配置通道2
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    // 配置通道3
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    // 启动定时器1
    TIM_Cmd(TIM4, ENABLE);
    /*****************************配置PWM定时器******************************/
}
void right_motor_output_init(uint16_t timer_period)
{
    /******************************定义局部参数******************************/
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /*******************************时钟初始化******************************/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);
    /******************************配置PWM引脚******************************/
    GPIO_InitStructure.GPIO_Pin = R_MOTOR_A_PHASE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(R_MOTOR_A_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = R_MOTOR_B_PHASE_PIN;
    GPIO_Init(R_MOTOR_B_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = R_MOTOR_C_PHASE_PIN;
    GPIO_Init(R_MOTOR_C_PHASE_PORT, &GPIO_InitStructure);

    /******************************配置PWM引脚******************************/

    /*****************************配置PWM定时器******************************/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                        //不分频，相当于72Mhz时钟直接输入定时器
    TIM_TimeBaseStructure.TIM_Period = timer_period-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    #if PWM_MODE == SVPWM_5 || PWM_MODE == SVPWM_7                  // 如果使用SVPWM调制，则使用中心对齐
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    #else                                                           // 如果使用SPWM调制，则使用边沿对齐
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    #endif
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 配置定时器4的PWM模式
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable ;
    TIM_OCInitStructure.TIM_Pulse=0;//CCR

    // 配置通道1
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    // 配置通道2
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    // 配置通道3
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);


    // 启动定时器1
    TIM_Cmd(TIM2, ENABLE);
    /*****************************配置PWM定时器******************************/
}
// 电机回调中断函数初始化
void motor_callback_init()
{
    // 注意：TIM1属于APB2外设，与TIM2（APB1）的时钟使能不同
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_initStructure;

    /****************************配置TIM1定时器时钟***************************/
    TIM_InternalClockConfig(TIM1);  // 使用内部时钟
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式
    TIM_TimeBaseStructure.TIM_Prescaler = 99;  // 预分频器
    TIM_TimeBaseStructure.TIM_Period = 71;  // 自动重装载值
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  // 重复计数器（仅高级定时器有）
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM1, TIM_FLAG_Update);  // 清除更新标志
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);  // 使能更新中断
    TIM_Cmd(TIM1, ENABLE);  // 使能定时器
    /****************************配置TIM1定时器时钟***************************/

    /******************************配置TIM1中断******************************/
    NVIC_initStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  // TIM1更新中断通道
    NVIC_initStructure.NVIC_IRQChannelCmd = ENABLE;  // 使能当前中断
    NVIC_initStructure.NVIC_IRQChannelPreemptionPriority = 1;  // 抢占优先级
    NVIC_initStructure.NVIC_IRQChannelSubPriority = 0;  // 子优先级
    NVIC_Init(&NVIC_initStructure);
    /******************************配置TIM1中断******************************/
}


void left_motor_duty_set(uint16_t a_duty, uint16_t b_duty, uint16_t c_duty)
{//写入PWM到PWM 0 1 2 通道
    TIM4->CH3CVR=(PWM_PRIOD_LOAD + a_duty) / 2;
    TIM4->CH2CVR=(PWM_PRIOD_LOAD + b_duty) / 2;
    TIM4->CH1CVR=(PWM_PRIOD_LOAD + c_duty) / 2;
}
void right_motor_duty_set(uint16_t a_duty, uint16_t b_duty, uint16_t c_duty)
{//写入PWM到PWM 0 1 2 通道
    TIM2->CH1CVR=(PWM_PRIOD_LOAD + a_duty) / 2;
    TIM2->CH2CVR=(PWM_PRIOD_LOAD + b_duty) / 2;
    TIM2->CH3CVR=(PWM_PRIOD_LOAD + c_duty) / 2;
}






