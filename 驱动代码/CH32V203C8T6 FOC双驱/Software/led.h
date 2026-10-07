#ifndef __LED_H_
#define __LED_H_
#include "motor_control.h"
typedef enum
{
    NORMAL_STATE,                       // 正常状态
    ERROR_STATE                         // 错误状态
}protect_enum;
typedef struct
{
    uint8_t high_freq_count;
    uint8_t mid_freq_count;
    uint8_t low_freq_count;
    uint8_t gpio_loop_count;
    void (*led_init)();
    void (*led_toggle)();
    // 保护参数
    protect_enum                        motor_protect_state;        // 电机保护模式
    protect_enum                        encoder_state;              // 磁编码器状态
}led_struct;
void motor_led_init(led_struct * led_p);
void motor_led_loop(led_struct * led_p);
#endif
