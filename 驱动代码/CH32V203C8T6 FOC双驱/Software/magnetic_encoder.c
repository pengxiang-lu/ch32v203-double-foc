#include "driver_config.h"

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     计算编码器旋转偏差 
// 参数说明     encoder_max     编码器精度  填写十进制数据
// 参数说明     now_location    当前位置信息
// 参数说明     last_location   上一次的位置信息
// 返回参数     int32_t           计算的偏差值    
// 使用示例     magnetic_uint8_t_get_offset();                                  
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static int16_t magnetic_encoder_get_offset (int32_t encoder_max, int32_t now_location, int32_t last_location)
{
    int16_t result_data = 0;
	
    if((encoder_max / 2) < func_abs(now_location - last_location))
    {
        result_data = ((encoder_max / 2) < now_location ? (now_location - encoder_max - last_location) : (now_location + encoder_max - last_location));
    }
    else
    {
        result_data = (now_location - last_location);
    }
	
    return result_data;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 MT6701 磁编码器 的 绝对值 角度数据
// 返回参数     uint16              绝对值 角度数据       
// 使用示例     uint8_t_get_absolute_data();                                   
// 备注信息     执行该函数后，可直接使用返回值 也可以通过查询对应变量获取结果
//-----------------------------------------------------------------------------------------------------------------
uint16_t encoder_get_absolute_data(encoder_struct * encoder_p)
{
	uint16_t data_last = encoder_p->encoder_value_now;
	encoder_p->encoder_value_now = encoder_p->magnetic_encoder_read();
    encoder_p->encoder_value_offset = magnetic_encoder_get_offset(ENCODER_PRECISION,encoder_p->encoder_value_now, data_last);
	return encoder_p->encoder_value_now;
}
