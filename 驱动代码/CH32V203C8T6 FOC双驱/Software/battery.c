#include "driver_config.h"

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
    if(battery_p->battery_state == ERROR_STATE)
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

// ADC采集附带均值滤波
uint16_t battery_read_adc_value(battery_value_struct * battery_p,uint8_t n)
{
    uint32_t sum = 0;
    for(uint8_t i = 0;i < n;i++)
    {
        sum += battery_p->battery_adc_read();
    }

    // 返回转换结果
    return sum/n;
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
    uint16_t adc_data = 0;                                                  	// 定义 临时 ADC 采集数据 存储位置
    
    adc_data = battery_read_adc_value(battery_p,5);                             			// 5次均值采样
    
    battery_struct.battery_voltage  = adc_data * CONVERSION_COEFFICIENT * BATTERY_RECTIFY_COEFFICIENT;     	                // 计算实际电压  ADC数据 * 转换系数 * 矫正系数
    
#if BATTERY_PROTECT == DRIVER_ENABLE
    battery_struct.battery_state    = battery_check(&battery_struct);                                                       // 根据电压判断当前电池状态
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

    battery_struct.battery_state = NORMAL_STATE;
}















