#ifndef __MAGNETIC_ENCODER_H_
#define __MAGNETIC_ENCODER_H_
#include <stdint.h>
typedef struct
{
	int32_t encoder_value_now;				        // 绝对编码器原始数据
	int32_t  encoder_value_offset;		            // 绝对编码器数据变化量
	int32_t  encoder_offset_integral;               // 绝对编码器数据变化量积分
	void (*magnetic_encoder_init)(void);		    // 定义编码器初始化底层函数
	uint16_t (*magnetic_encoder_read)(void);		// 定义读取编码器底层函数
}encoder_struct;

uint16_t encoder_get_absolute_data(encoder_struct * encoder_p);
#endif





