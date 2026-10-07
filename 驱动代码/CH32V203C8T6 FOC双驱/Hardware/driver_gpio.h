#ifndef __DRIVER_GPIO_H_
#define __DRIVER_GPIO_H_

#define L_LED_PORT	GPIOC
#define L_LED_PIN	GPIO_Pin_13
#define L_LED_ON	GPIO_ResetBits(L_LED_PORT,L_LED_PIN)	//gpio低电平代表开灯
#define L_LED_OFF	GPIO_SetBits(L_LED_PORT,L_LED_PIN)		//gpio高电平代表关灯

#define R_LED_PORT  GPIOB
#define R_LED_PIN   GPIO_Pin_0
#define R_LED_ON    GPIO_ResetBits(R_LED_PORT,R_LED_PIN)    //gpio低电平代表开灯
#define R_LED_OFF   GPIO_SetBits(R_LED_PORT,R_LED_PIN)      //gpio高电平代表关灯

#define KEY_PORT	GPIOA
#define KEY_PIN		GPIO_Pin_8
#define KEY_READ	GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN)

void gpio_key_init(void);
void left_led_toggle(void);
void left_led_init(void);
void right_led_toggle(void);
void right_led_init(void);
void key_scan(void);
#endif


