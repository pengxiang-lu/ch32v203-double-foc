#include "driver_config.h"
// LED灯初始化
void motor_led_init(led_struct * led_p)
{
    led_p->led_init();
    led_p->high_freq_count = (100 / DRIVER_RESPONSE_CYCLE);
    led_p->mid_freq_count  = (500 / DRIVER_RESPONSE_CYCLE);
    led_p->low_freq_count  = (1000 / DRIVER_RESPONSE_CYCLE);
}
void motor_led_loop(led_struct * led_p)
{
    key_scan();
    led_p->gpio_loop_count++;
    if(battery_struct.battery_state == ERROR_STATE)
    {
        if(led_p->gpio_loop_count>=led_p->high_freq_count)
        {
            led_p->led_toggle();
            led_p->gpio_loop_count=0;
        }
    }
    else if(led_p->encoder_state == ERROR_STATE)         // 磁编错误500ms闪一次
    {
        if(led_p->gpio_loop_count>=led_p->mid_freq_count)
        {
            led_p->led_toggle();
            led_p->gpio_loop_count=0;
        }
    }
    else if(led_p->motor_protect_state == ERROR_STATE)   // 保护模式1s闪一次
    {
        if(led_p->gpio_loop_count>=led_p->low_freq_count)
        {
            led_p->led_toggle();
            led_p->gpio_loop_count=0;
        }
    }
}
