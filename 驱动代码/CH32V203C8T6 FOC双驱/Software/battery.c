#include "driver_config.h"

// 一阶低通系数；约10ms更新一次时，时间常数约95ms。
#define BATTERY_VOLTAGE_FILTER_ALPHA 0.1f

battery_value_struct battery_struct;         

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电池电压检查
// 参数说明     void
// 返回参数     battery_state_enum  
// 使用示例     battery_check();
// 备注信息     返回当前电压对应的电池状态
//-------------------------------------------------------------------------------------------------------------------
protect_enum battery_check(battery_value_struct * battery_p)
{
    if(battery_p->battery_state == NORMAL_STATE)
    {   if(battery_p->battery_voltage < battery_p->battery_err_voltage)
        {
            battery_p->error_count_num ++;
        }
        else
        {
            battery_p->error_count_num = 0;
        }
        if(battery_p->error_count_num >=50)            // 连续50次小于阈值电压
        {
            battery_p->error_count_num = 0;
            return ERROR_STATE;
        }
        return NORMAL_STATE;
    }
    return ERROR_STATE;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     驱动的 ADC循环检测函数
// 参数说明     void
// 返回参数     void  
// 使用示例     driver_adc_loop();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void battery_adc_loop(battery_value_struct * battery_p)
{
    uint16_t adc_data = battery_p->battery_adc_read();
    float voltage = adc_data * CONVERSION_COEFFICIENT * BATTERY_RECTIFY_COEFFICIENT;

    if (!battery_p->voltage_filter_initialized)
    {
        // 首次采样直接赋值，避免滤波从零开始造成启动欠压误判。
        battery_p->battery_voltage = voltage;
        battery_p->voltage_filter_initialized = 1;
    }
    else
    {
        battery_p->battery_voltage += BATTERY_VOLTAGE_FILTER_ALPHA
                                   * (voltage - battery_p->battery_voltage);
    }

#if BATTERY_PROTECT == DRIVER_ENABLE
    // 使用滤波后的电压进行原有的连续欠压判断。
    battery_p->battery_state = battery_check(battery_p);
#endif
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     驱动的 ADC 功能初始化
// 参数说明     void
// 返回参数     void  
// 使用示例     driver_adc_init();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void battery_init(battery_value_struct * battery_p,float battery_err_voltage)
{
    battery_p->battery_adc_init();

    battery_p->battery_err_voltage = battery_err_voltage;                       // 阈值电压

    battery_p->battery_voltage = 0.0f;
    battery_p->voltage_filter_initialized = 0;
    battery_p->error_count_num = 0;
    battery_p->battery_state = NORMAL_STATE;
}















