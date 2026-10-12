#include "driver_config.h"

void left_motor_output_init(uint16_t timer_period)
{
    /******************************閿熸枻鎷烽敓鏂ゆ嫹妞嶉敓鏂ゆ嫹閿熸枻鎷烽敓锟�******************************/
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /*******************************鏃堕敓鎺ョ鎷峰閿熸枻鎷�******************************/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 ,ENABLE);
    /******************************閿熸枻鎷烽敓鏂ゆ嫹PWM閿熸枻鎷烽敓鏂ゆ嫹******************************/
    GPIO_InitStructure.GPIO_Pin = L_MOTOR_A_PHASE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(L_MOTOR_A_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = L_MOTOR_B_PHASE_PIN;
    GPIO_Init(L_MOTOR_B_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = L_MOTOR_C_PHASE_PIN;
    GPIO_Init(L_MOTOR_C_PHASE_PORT, &GPIO_InitStructure);

    /******************************閿熸枻鎷烽敓鏂ゆ嫹PWM閿熸枻鎷烽敓鏂ゆ嫹******************************/

    /*****************************閿熸枻鎷烽敓鏂ゆ嫹PWM閿熸枻鎷锋椂閿熸枻鎷�******************************/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up   ;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                        //閿熸枻鎷烽敓鏂ゆ嫹棰戦敓鏂ゆ嫹閿熸磥褰撻敓鏂ゆ嫹72Mhz鏃堕敓鏂ゆ嫹鐩撮敓鏂ゆ嫹閿熸枻鎷烽敓璇畾鏃堕敓鏂ゆ嫹
    TIM_TimeBaseStructure.TIM_Period = timer_period-1; 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    #if PWM_MODE == SVPWM_5 || PWM_MODE == SVPWM_7                  // 閿熸枻鎷烽敓缁炵櫢鎷烽敓绲奦PWM閿熸枻鎷烽敓鐙★綇鎷烽敓鏂ゆ嫹浣块敓鏂ゆ嫹閿熸枻鎷烽敓渚ヨ鎷烽敓鏂ゆ嫹
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    #else                                                           // 閿熸枻鎷烽敓缁炵櫢鎷烽敓绲奝WM閿熸枻鎷烽敓鐙★綇鎷烽敓鏂ゆ嫹浣块敓鐭唻鎷烽敓鎴鎷烽敓鏂ゆ嫹
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    #endif
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    // 閿熸枻鎷烽敓鐭鎷锋椂閿熸枻鎷�4閿熸枻鎷稰WM妯″紡
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable ;
    TIM_OCInitStructure.TIM_Pulse=0;//CCR

    // 閿熸枻鎷烽敓鏂ゆ嫹閫氶敓鏂ゆ嫹1
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    // 閿熸枻鎷烽敓鏂ゆ嫹閫氶敓鏂ゆ嫹2
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    // 閿熸枻鎷烽敓鏂ゆ嫹閫氶敓鏂ゆ嫹3
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);

    // Apply buffered compare values on timer update events.
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    // 閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷锋椂閿熸枻鎷�1
    TIM_Cmd(TIM4, ENABLE);
    /*****************************閿熸枻鎷烽敓鏂ゆ嫹PWM閿熸枻鎷锋椂閿熸枻鎷�******************************/
}
void right_motor_output_init(uint16_t timer_period)
{
    /******************************閿熸枻鎷烽敓鏂ゆ嫹妞嶉敓鏂ゆ嫹閿熸枻鎷烽敓锟�******************************/
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /*******************************鏃堕敓鎺ョ鎷峰閿熸枻鎷�******************************/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);
    /******************************閿熸枻鎷烽敓鏂ゆ嫹PWM閿熸枻鎷烽敓鏂ゆ嫹******************************/
    GPIO_InitStructure.GPIO_Pin = R_MOTOR_A_PHASE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(R_MOTOR_A_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = R_MOTOR_B_PHASE_PIN;
    GPIO_Init(R_MOTOR_B_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = R_MOTOR_C_PHASE_PIN;
    GPIO_Init(R_MOTOR_C_PHASE_PORT, &GPIO_InitStructure);

    /******************************閿熸枻鎷烽敓鏂ゆ嫹PWM閿熸枻鎷烽敓鏂ゆ嫹******************************/

    /*****************************閿熸枻鎷烽敓鏂ゆ嫹PWM閿熸枻鎷锋椂閿熸枻鎷�******************************/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                        //閿熸枻鎷烽敓鏂ゆ嫹棰戦敓鏂ゆ嫹閿熸磥褰撻敓鏂ゆ嫹72Mhz鏃堕敓鏂ゆ嫹鐩撮敓鏂ゆ嫹閿熸枻鎷烽敓璇畾鏃堕敓鏂ゆ嫹
    TIM_TimeBaseStructure.TIM_Period = timer_period-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    #if PWM_MODE == SVPWM_5 || PWM_MODE == SVPWM_7                  // 閿熸枻鎷烽敓缁炵櫢鎷烽敓绲奦PWM閿熸枻鎷烽敓鐙★綇鎷烽敓鏂ゆ嫹浣块敓鏂ゆ嫹閿熸枻鎷烽敓渚ヨ鎷烽敓鏂ゆ嫹
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    #else                                                           // 閿熸枻鎷烽敓缁炵櫢鎷烽敓绲奝WM閿熸枻鎷烽敓鐙★綇鎷烽敓鏂ゆ嫹浣块敓鐭唻鎷烽敓鎴鎷烽敓鏂ゆ嫹
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    #endif
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 閿熸枻鎷烽敓鐭鎷锋椂閿熸枻鎷�4閿熸枻鎷稰WM妯″紡
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable ;
    TIM_OCInitStructure.TIM_Pulse=0;//CCR

    // 閿熸枻鎷烽敓鏂ゆ嫹閫氶敓鏂ゆ嫹1
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    // 閿熸枻鎷烽敓鏂ゆ嫹閫氶敓鏂ゆ嫹2
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    // 閿熸枻鎷烽敓鏂ゆ嫹閫氶敓鏂ゆ嫹3
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    // Apply buffered compare values on timer update events.
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);


    // 閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷锋椂閿熸枻鎷�1
    TIM_Cmd(TIM2, ENABLE);
    /*****************************閿熸枻鎷烽敓鏂ゆ嫹PWM閿熸枻鎷锋椂閿熸枻鎷�******************************/
}
// 閿熸枻鎷烽敓鏂ゆ嫹姘愰敓鏂ゆ嫹鍗歌櫨閿熸枻鎷烽敓鏂ゆ嫹閿熺粸纭锋嫹閿燂拷
void motor_callback_init()
{
    // 娉ㄩ敓瑙ｏ細TIM1閿熸枻鎷烽敓鏂ゆ嫹APB2閿熸枻鎷烽敓鍊燂紝閿熸枻鎷稵IM2閿熸枻鎷稟PB1閿熸枻鎷烽敓鏂ゆ嫹鏃堕敓鏂ゆ嫹浣块敓鏉拌鎷峰悓
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_initStructure;

    /****************************閿熸枻鎷烽敓鏂ゆ嫹TIM1閿熸枻鎷锋椂閿熸枻鎷锋椂閿熸枻鎷�***************************/
    TIM_InternalClockConfig(TIM1);  // 浣块敓鏂ゆ嫹閿熻妭璇ф嫹鏃堕敓鏂ゆ嫹
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 鏃堕敓鎺ュ嚖鎷烽
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 閿熸枻鎷烽敓杈冪》鎷烽敓鏂ゆ嫹妯″紡
    TIM_TimeBaseStructure.TIM_Prescaler = 99;  // 棰勯敓鏂ゆ嫹棰戦敓鏂ゆ嫹
    TIM_TimeBaseStructure.TIM_Period = 71;  // 閿熺殕璁规嫹閿熸枻鎷疯閿熸枻鎷峰€�
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  // 閿熸埅闈╂嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓绔》鎷烽敓鏂ゆ嫹鏃堕敓鏂ゆ嫹閿熷彨锝忔嫹
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM1, TIM_FLAG_Update);  // 閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷峰崵閿熻锟�
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);  // 浣块敓鏉伴潻鎷烽敓鏂ゆ嫹閿熷彨璁规嫹
    TIM_Cmd(TIM1, ENABLE);  // 浣块敓鏉拌鎷锋椂閿熸枻鎷�
    /****************************閿熸枻鎷烽敓鏂ゆ嫹TIM1閿熸枻鎷锋椂閿熸枻鎷锋椂閿熸枻鎷�***************************/

    /******************************閿熸枻鎷烽敓鏂ゆ嫹TIM1閿熷彨璁规嫹******************************/
    NVIC_initStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  // TIM1閿熸枻鎷烽敓鏂ゆ嫹閿熷彨璁规嫹閫氶敓鏂ゆ嫹
    NVIC_initStructure.NVIC_IRQChannelCmd = ENABLE;  // 浣块敓鏉扮鎷峰墠閿熷彨璁规嫹
    NVIC_initStructure.NVIC_IRQChannelPreemptionPriority = 0;  // 閿熸枻鎷峰崰閿熸枻鎷烽敓楗虹》鎷�
    NVIC_initStructure.NVIC_IRQChannelSubPriority = 0;  // 閿熸枻鎷烽敓鏂ゆ嫹閿熼ズ纭锋嫹
    NVIC_Init(&NVIC_initStructure);
    /******************************閿熸枻鎷烽敓鏂ゆ嫹TIM1閿熷彨璁规嫹******************************/
}


void left_motor_duty_set(uint16_t a_duty, uint16_t b_duty, uint16_t c_duty)
{//鍐欓敓鏂ゆ嫹PWM閿熸枻鎷稰WM 0 1 2 閫氶敓鏂ゆ嫹
    // Prevent an update event from loading a partially written triplet.
    uint16_t update_disabled = TIM4->CTLR1 & TIM_UDIS;
    TIM4->CTLR1 |= TIM_UDIS;
    TIM4->CH3CVR= a_duty;
    TIM4->CH2CVR= b_duty;
    TIM4->CH1CVR= c_duty;
    if (!update_disabled)
        TIM4->CTLR1 &= (uint16_t)~TIM_UDIS;
}
void right_motor_duty_set(uint16_t a_duty, uint16_t b_duty, uint16_t c_duty)
{//鍐欓敓鏂ゆ嫹PWM閿熸枻鎷稰WM 0 1 2 閫氶敓鏂ゆ嫹
    // Prevent an update event from loading a partially written triplet.
    uint16_t update_disabled = TIM2->CTLR1 & TIM_UDIS;
    TIM2->CTLR1 |= TIM_UDIS;
    TIM2->CH1CVR= a_duty;
    TIM2->CH2CVR= b_duty;
    TIM2->CH3CVR= c_duty;
    if (!update_disabled)
        TIM2->CTLR1 &= (uint16_t)~TIM_UDIS;
}







