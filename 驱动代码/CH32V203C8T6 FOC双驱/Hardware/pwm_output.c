#include "driver_config.h"

void left_motor_output_init(uint16_t timer_period)
{
    /******************************锟斤拷锟斤拷植锟斤拷锟斤拷锟�******************************/
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /*******************************时锟接筹拷始锟斤拷******************************/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 ,ENABLE);
    /******************************锟斤拷锟斤拷PWM锟斤拷锟斤拷******************************/
    GPIO_InitStructure.GPIO_Pin = L_MOTOR_A_PHASE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(L_MOTOR_A_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = L_MOTOR_B_PHASE_PIN;
    GPIO_Init(L_MOTOR_B_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = L_MOTOR_C_PHASE_PIN;
    GPIO_Init(L_MOTOR_C_PHASE_PORT, &GPIO_InitStructure);

    /******************************锟斤拷锟斤拷PWM锟斤拷锟斤拷******************************/

    /*****************************锟斤拷锟斤拷PWM锟斤拷时锟斤拷******************************/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up   ;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                        //锟斤拷锟斤拷频锟斤拷锟洁当锟斤拷72Mhz时锟斤拷直锟斤拷锟斤拷锟诫定时锟斤拷
    TIM_TimeBaseStructure.TIM_Period = timer_period-1; 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    #if PWM_MODE == SVPWM_5 || PWM_MODE == SVPWM_7                  // 锟斤拷锟绞癸拷锟絊VPWM锟斤拷锟狡ｏ拷锟斤拷使锟斤拷锟斤拷锟侥讹拷锟斤拷
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    #else                                                           // 锟斤拷锟绞癸拷锟絊PWM锟斤拷锟狡ｏ拷锟斤拷使锟矫憋拷锟截讹拷锟斤拷
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    #endif
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    // 锟斤拷锟矫讹拷时锟斤拷4锟斤拷PWM模式
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable ;
    TIM_OCInitStructure.TIM_Pulse=0;//CCR

    // 锟斤拷锟斤拷通锟斤拷1
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    // 锟斤拷锟斤拷通锟斤拷2
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    // 锟斤拷锟斤拷通锟斤拷3
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);

    // Apply buffered compare values on timer update events.
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    // 锟斤拷锟斤拷锟斤拷时锟斤拷1
    TIM_Cmd(TIM4, ENABLE);
    /*****************************锟斤拷锟斤拷PWM锟斤拷时锟斤拷******************************/
}
void right_motor_output_init(uint16_t timer_period)
{
    /******************************锟斤拷锟斤拷植锟斤拷锟斤拷锟�******************************/
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /*******************************时锟接筹拷始锟斤拷******************************/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);
    /******************************锟斤拷锟斤拷PWM锟斤拷锟斤拷******************************/
    GPIO_InitStructure.GPIO_Pin = R_MOTOR_A_PHASE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(R_MOTOR_A_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = R_MOTOR_B_PHASE_PIN;
    GPIO_Init(R_MOTOR_B_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = R_MOTOR_C_PHASE_PIN;
    GPIO_Init(R_MOTOR_C_PHASE_PORT, &GPIO_InitStructure);

    /******************************锟斤拷锟斤拷PWM锟斤拷锟斤拷******************************/

    /*****************************锟斤拷锟斤拷PWM锟斤拷时锟斤拷******************************/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                        //锟斤拷锟斤拷频锟斤拷锟洁当锟斤拷72Mhz时锟斤拷直锟斤拷锟斤拷锟诫定时锟斤拷
    TIM_TimeBaseStructure.TIM_Period = timer_period-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    #if PWM_MODE == SVPWM_5 || PWM_MODE == SVPWM_7                  // 锟斤拷锟绞癸拷锟絊VPWM锟斤拷锟狡ｏ拷锟斤拷使锟斤拷锟斤拷锟侥讹拷锟斤拷
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    #else                                                           // 锟斤拷锟绞癸拷锟絊PWM锟斤拷锟狡ｏ拷锟斤拷使锟矫憋拷锟截讹拷锟斤拷
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    #endif
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 锟斤拷锟矫讹拷时锟斤拷4锟斤拷PWM模式
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable ;
    TIM_OCInitStructure.TIM_Pulse=0;//CCR

    // 锟斤拷锟斤拷通锟斤拷1
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    // 锟斤拷锟斤拷通锟斤拷2
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    // 锟斤拷锟斤拷通锟斤拷3
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    // Apply buffered compare values on timer update events.
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);


    // 锟斤拷锟斤拷锟斤拷时锟斤拷1
    TIM_Cmd(TIM2, ENABLE);
    /*****************************锟斤拷锟斤拷PWM锟斤拷时锟斤拷******************************/
}
// 锟斤拷锟斤拷氐锟斤拷卸虾锟斤拷锟斤拷锟绞硷拷锟�
void motor_callback_init()
{
    // 注锟解：TIM1锟斤拷锟斤拷APB2锟斤拷锟借，锟斤拷TIM2锟斤拷APB1锟斤拷锟斤拷时锟斤拷使锟杰诧拷同
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_initStructure;

    /****************************锟斤拷锟斤拷TIM1锟斤拷时锟斤拷时锟斤拷***************************/
    TIM_InternalClockConfig(TIM1);  // 使锟斤拷锟节诧拷时锟斤拷
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时锟接凤拷频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 锟斤拷锟较硷拷锟斤拷模式
    TIM_TimeBaseStructure.TIM_Prescaler = 99;  // 预锟斤拷频锟斤拷
    TIM_TimeBaseStructure.TIM_Period = 71;  // 锟皆讹拷锟斤拷装锟斤拷值
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  // 锟截革拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟竭硷拷锟斤拷时锟斤拷锟叫ｏ拷
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM1, TIM_FLAG_Update);  // 锟斤拷锟斤拷锟斤拷卤锟街�
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);  // 使锟杰革拷锟斤拷锟叫讹拷
    TIM_Cmd(TIM1, ENABLE);  // 使锟杰讹拷时锟斤拷
    /****************************锟斤拷锟斤拷TIM1锟斤拷时锟斤拷时锟斤拷***************************/

    /******************************锟斤拷锟斤拷TIM1锟叫讹拷******************************/
    NVIC_initStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  // TIM1锟斤拷锟斤拷锟叫讹拷通锟斤拷
    NVIC_initStructure.NVIC_IRQChannelCmd = ENABLE;  // 使锟杰碉拷前锟叫讹拷
    NVIC_initStructure.NVIC_IRQChannelPreemptionPriority = 1;  // 锟斤拷占锟斤拷锟饺硷拷
    NVIC_initStructure.NVIC_IRQChannelSubPriority = 0;  // 锟斤拷锟斤拷锟饺硷拷
    NVIC_Init(&NVIC_initStructure);
    /******************************锟斤拷锟斤拷TIM1锟叫讹拷******************************/
}


void left_motor_duty_set(uint16_t a_duty, uint16_t b_duty, uint16_t c_duty)
{//写锟斤拷PWM锟斤拷PWM 0 1 2 通锟斤拷
    TIM4->CH3CVR= a_duty;
    TIM4->CH2CVR= b_duty;
    TIM4->CH1CVR= c_duty;
}
void right_motor_duty_set(uint16_t a_duty, uint16_t b_duty, uint16_t c_duty)
{//写锟斤拷PWM锟斤拷PWM 0 1 2 通锟斤拷
    TIM2->CH1CVR= a_duty;
    TIM2->CH2CVR= b_duty;
    TIM2->CH3CVR= c_duty;
}






