#ifndef __BATTERY_H_
#define __BATTERY_H_
#include "driver_config.h"
#include "led.h"

typedef struct
{

    float                       battery_voltage;                        // 电池当前电压

    protect_enum                battery_state;                          // 电池当前状态

    uint32_t                    error_count_num;

    float                       battery_err_voltage;                    // 电池电压阈值
    // 电池检测ADC初始化函数和ADC读取函数
    uint16_t                    (*battery_adc_read)();
    void                        (*battery_adc_init)();
}battery_value_struct;

extern battery_value_struct battery_struct;

void battery_adc_loop(battery_value_struct * battery_p);

void battery_init(battery_value_struct * battery_p,float battery_err_voltage);


#endif
