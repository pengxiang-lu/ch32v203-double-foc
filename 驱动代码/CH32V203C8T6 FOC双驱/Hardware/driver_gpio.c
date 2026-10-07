#include "driver_config.h"
//PB12:开关
void gpio_key_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=KEY_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(KEY_PORT,&GPIO_InitStructure);
}
//开关扫描
void key_scan()
{
    static uint8_t key_count = 0;
    if(KEY_READ==0)
    {
        key_count++;
        if(key_count>=20)               //需要开关长按一段时间才能触发
        {
#if LEFT_MOTOR_STATE    ==     DRIVER_ENABLE
            motor_zero_calibration(&left_motor);            //校正零点，将零点信息存在flash中
            L_LED_ON;
#endif
#if RIGHT_MOTOR_STATE    ==     DRIVER_ENABLE
            motor_zero_calibration(&right_motor);            //校正零点，将零点信息存在flash中
            R_LED_ON;
#endif
            key_count = 0;
        }
    }
}
//本文件用于存放和指示灯以及开关有关的GPIO初始化操作
//PC13:指示灯
void left_led_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=L_LED_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(L_LED_PORT,&GPIO_InitStructure);
	L_LED_ON;
}

void left_led_toggle()
{
	if(GPIO_ReadOutputDataBit(L_LED_PORT,L_LED_PIN)==0)
	{
		GPIO_SetBits(L_LED_PORT,L_LED_PIN);
	}
	else
	{
		GPIO_ResetBits(L_LED_PORT,L_LED_PIN);
	}
}
void right_led_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=R_LED_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(R_LED_PORT,&GPIO_InitStructure);
    R_LED_ON;
}

void right_led_toggle()
{
    if(GPIO_ReadOutputDataBit(R_LED_PORT,R_LED_PIN)==0)
    {
        GPIO_SetBits(R_LED_PORT,R_LED_PIN);
    }
    else
    {
        GPIO_ResetBits(R_LED_PORT,R_LED_PIN);
    }
}
