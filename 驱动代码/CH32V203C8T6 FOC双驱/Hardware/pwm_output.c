#include "driver_config.h"

void left_motor_output_init(uint16_t timer_period)
{
    /******************************闁跨喐鏋婚幏鐑芥晸閺傘倖瀚瑰宥夋晸閺傘倖瀚归柨鐔告灮閹风兘鏁撻敓锟�******************************/
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /*******************************閺冨爼鏁撻幒銉ь劜閹峰嘲顫愰柨鐔告灮閹凤拷******************************/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 ,ENABLE);
    /******************************闁跨喐鏋婚幏鐑芥晸閺傘倖瀚筆WM闁跨喐鏋婚幏鐑芥晸閺傘倖瀚�******************************/
    GPIO_InitStructure.GPIO_Pin = L_MOTOR_A_PHASE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(L_MOTOR_A_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = L_MOTOR_B_PHASE_PIN;
    GPIO_Init(L_MOTOR_B_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = L_MOTOR_C_PHASE_PIN;
    GPIO_Init(L_MOTOR_C_PHASE_PORT, &GPIO_InitStructure);

    /******************************闁跨喐鏋婚幏鐑芥晸閺傘倖瀚筆WM闁跨喐鏋婚幏鐑芥晸閺傘倖瀚�******************************/

    /*****************************闁跨喐鏋婚幏鐑芥晸閺傘倖瀚筆WM闁跨喐鏋婚幏閿嬫闁跨喐鏋婚幏锟�******************************/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up   ;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                        //闁跨喐鏋婚幏鐑芥晸閺傘倖瀚规０鎴︽晸閺傘倖瀚归柨鐔哥％瑜版捇鏁撻弬銈嗗72Mhz閺冨爼鏁撻弬銈嗗閻╂挳鏁撻弬銈嗗闁跨喐鏋婚幏鐑芥晸鐠囶偄鐣鹃弮鍫曟晸閺傘倖瀚�
    TIM_TimeBaseStructure.TIM_Period = timer_period-1; 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    #if PWM_MODE == SVPWM_5 || PWM_MODE == SVPWM_7                  // 闁跨喐鏋婚幏鐑芥晸缂佺偟娅㈤幏鐑芥晸缁插ウPWM闁跨喐鏋婚幏鐑芥晸閻欌槄缍囬幏鐑芥晸閺傘倖瀚规担鍧楁晸閺傘倖瀚归柨鐔告灮閹风兘鏁撴笟銉啇閹风兘鏁撻弬銈嗗
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    #else                                                           // 闁跨喐鏋婚幏鐑芥晸缂佺偟娅㈤幏鐑芥晸缁插WM闁跨喐鏋婚幏鐑芥晸閻欌槄缍囬幏鐑芥晸閺傘倖瀚规担鍧楁晸閻偅鍞婚幏鐑芥晸閹搭亣顔愰幏鐑芥晸閺傘倖瀚�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    #endif
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    // 闁跨喐鏋婚幏鐑芥晸閻偉顔愰幏閿嬫闁跨喐鏋婚幏锟�4闁跨喐鏋婚幏绋癢M濡€崇础
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable ;
    TIM_OCInitStructure.TIM_Pulse=0;//CCR

    // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柅姘舵晸閺傘倖瀚�1
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柅姘舵晸閺傘倖瀚�2
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柅姘舵晸閺傘倖瀚�3
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);

    // Apply buffered compare values on timer update events.
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔告灮閹烽攱妞傞柨鐔告灮閹凤拷1
    TIM_Cmd(TIM4, ENABLE);
    /*****************************闁跨喐鏋婚幏鐑芥晸閺傘倖瀚筆WM闁跨喐鏋婚幏閿嬫闁跨喐鏋婚幏锟�******************************/
}
void right_motor_output_init(uint16_t timer_period)
{
    /******************************闁跨喐鏋婚幏鐑芥晸閺傘倖瀚瑰宥夋晸閺傘倖瀚归柨鐔告灮閹风兘鏁撻敓锟�******************************/
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /*******************************閺冨爼鏁撻幒銉ь劜閹峰嘲顫愰柨鐔告灮閹凤拷******************************/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);
    /******************************闁跨喐鏋婚幏鐑芥晸閺傘倖瀚筆WM闁跨喐鏋婚幏鐑芥晸閺傘倖瀚�******************************/
    GPIO_InitStructure.GPIO_Pin = R_MOTOR_A_PHASE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(R_MOTOR_A_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = R_MOTOR_B_PHASE_PIN;
    GPIO_Init(R_MOTOR_B_PHASE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = R_MOTOR_C_PHASE_PIN;
    GPIO_Init(R_MOTOR_C_PHASE_PORT, &GPIO_InitStructure);

    /******************************闁跨喐鏋婚幏鐑芥晸閺傘倖瀚筆WM闁跨喐鏋婚幏鐑芥晸閺傘倖瀚�******************************/

    /*****************************闁跨喐鏋婚幏鐑芥晸閺傘倖瀚筆WM闁跨喐鏋婚幏閿嬫闁跨喐鏋婚幏锟�******************************/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                        //闁跨喐鏋婚幏鐑芥晸閺傘倖瀚规０鎴︽晸閺傘倖瀚归柨鐔哥％瑜版捇鏁撻弬銈嗗72Mhz閺冨爼鏁撻弬銈嗗閻╂挳鏁撻弬銈嗗闁跨喐鏋婚幏鐑芥晸鐠囶偄鐣鹃弮鍫曟晸閺傘倖瀚�
    TIM_TimeBaseStructure.TIM_Period = timer_period-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    #if PWM_MODE == SVPWM_5 || PWM_MODE == SVPWM_7                  // 闁跨喐鏋婚幏鐑芥晸缂佺偟娅㈤幏鐑芥晸缁插ウPWM闁跨喐鏋婚幏鐑芥晸閻欌槄缍囬幏鐑芥晸閺傘倖瀚规担鍧楁晸閺傘倖瀚归柨鐔告灮閹风兘鏁撴笟銉啇閹风兘鏁撻弬銈嗗
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    #else                                                           // 闁跨喐鏋婚幏鐑芥晸缂佺偟娅㈤幏鐑芥晸缁插WM闁跨喐鏋婚幏鐑芥晸閻欌槄缍囬幏鐑芥晸閺傘倖瀚规担鍧楁晸閻偅鍞婚幏鐑芥晸閹搭亣顔愰幏鐑芥晸閺傘倖瀚�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    #endif
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 闁跨喐鏋婚幏鐑芥晸閻偉顔愰幏閿嬫闁跨喐鏋婚幏锟�4闁跨喐鏋婚幏绋癢M濡€崇础
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable ;
    TIM_OCInitStructure.TIM_Pulse=0;//CCR

    // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柅姘舵晸閺傘倖瀚�1
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柅姘舵晸閺傘倖瀚�2
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柅姘舵晸閺傘倖瀚�3
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    // Apply buffered compare values on timer update events.
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);


    // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔告灮閹烽攱妞傞柨鐔告灮閹凤拷1
    TIM_Cmd(TIM2, ENABLE);
    /*****************************闁跨喐鏋婚幏鐑芥晸閺傘倖瀚筆WM闁跨喐鏋婚幏閿嬫闁跨喐鏋婚幏锟�******************************/
}
// 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚瑰鎰版晸閺傘倖瀚归崡姝屾闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔虹哺绾攱瀚归柨鐕傛嫹
void motor_callback_init()
{
    // 濞夈劑鏁撶憴锝忕窗TIM1闁跨喐鏋婚幏鐑芥晸閺傘倖瀚笰PB2闁跨喐鏋婚幏鐑芥晸閸婄噦绱濋柨鐔告灮閹风ǖIM2闁跨喐鏋婚幏绋烶B1闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归弮鍫曟晸閺傘倖瀚规担鍧楁晸閺夋媽顕滈幏宄版倱
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_initStructure;

    /****************************闁跨喐鏋婚幏鐑芥晸閺傘倖瀚筎IM1闁跨喐鏋婚幏閿嬫闁跨喐鏋婚幏閿嬫闁跨喐鏋婚幏锟�***************************/
    TIM_InternalClockConfig(TIM1);  // 娴ｅ潡鏁撻弬銈嗗闁跨喕濡拠褎瀚归弮鍫曟晸閺傘倖瀚�
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 閺冨爼鏁撻幒銉ュ殩閹风兘顣�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 闁跨喐鏋婚幏鐑芥晸鏉堝啰銆嬮幏鐑芥晸閺傘倖瀚瑰Ο鈥崇础
    TIM_TimeBaseStructure.TIM_Prescaler = 99;  // 妫板嫰鏁撻弬銈嗗妫版垿鏁撻弬銈嗗
    TIM_TimeBaseStructure.TIM_Period = 71;  // 闁跨喓娈曠拋瑙勫闁跨喐鏋婚幏鐤棅闁跨喐鏋婚幏宄扳偓锟�
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  // 闁跨喐鍩呴棃鈺傚闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔告灮閹风兘鏁撻弬銈嗗闁跨喐鏋婚幏鐑芥晸缁旑厾銆嬮幏鐑芥晸閺傘倖瀚归弮鍫曟晸閺傘倖瀚归柨鐔峰建閿濆繑瀚�
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM1, TIM_FLAG_Update);  // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔告灮閹峰嘲宕甸柨鐔活敎閿燂拷
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);  // 娴ｅ潡鏁撻弶浼存交閹风兘鏁撻弬銈嗗闁跨喎褰ㄧ拋瑙勫
    TIM_Cmd(TIM1, ENABLE);  // 娴ｅ潡鏁撻弶鎷岊啇閹烽攱妞傞柨鐔告灮閹凤拷
    /****************************闁跨喐鏋婚幏鐑芥晸閺傘倖瀚筎IM1闁跨喐鏋婚幏閿嬫闁跨喐鏋婚幏閿嬫闁跨喐鏋婚幏锟�***************************/

    /******************************闁跨喐鏋婚幏鐑芥晸閺傘倖瀚筎IM1闁跨喎褰ㄧ拋瑙勫******************************/
    NVIC_initStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  // TIM1闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔峰建鐠佽瀚归柅姘舵晸閺傘倖瀚�
    NVIC_initStructure.NVIC_IRQChannelCmd = ENABLE;  // 娴ｅ潡鏁撻弶鎵暜閹峰嘲澧犻柨鐔峰建鐠佽瀚�
    NVIC_initStructure.NVIC_IRQChannelPreemptionPriority = 0;  // 闁跨喐鏋婚幏宄板窗闁跨喐鏋婚幏鐑芥晸妤楄櫣銆嬮幏锟�
    NVIC_initStructure.NVIC_IRQChannelSubPriority = 0;  // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔笺偤绾攱瀚�
    NVIC_Init(&NVIC_initStructure);
    /******************************闁跨喐鏋婚幏鐑芥晸閺傘倖瀚筎IM1闁跨喎褰ㄧ拋瑙勫******************************/
}


void left_motor_duty_set(uint16_t a_duty, uint16_t b_duty, uint16_t c_duty)
{//閸愭瑩鏁撻弬銈嗗PWM闁跨喐鏋婚幏绋癢M 0 1 2 闁岸鏁撻弬銈嗗
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
{//閸愭瑩鏁撻弬銈嗗PWM闁跨喐鏋婚幏绋癢M 0 1 2 闁岸鏁撻弬銈嗗
    // Prevent an update event from loading a partially written triplet.
    uint16_t update_disabled = TIM2->CTLR1 & TIM_UDIS;
    TIM2->CTLR1 |= TIM_UDIS;
    TIM2->CH1CVR= a_duty;
    TIM2->CH2CVR= b_duty;
    TIM2->CH3CVR= c_duty;
    if (!update_disabled)
        TIM2->CTLR1 &= (uint16_t)~TIM_UDIS;
}







