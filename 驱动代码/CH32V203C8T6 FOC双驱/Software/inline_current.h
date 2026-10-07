#ifndef __INLINE_CURRENT_H_
#define __INLINE_CURRENT_H_
#include "foc.h"
// A，B，C电流数组序号
#define I_A				0
#define I_B				2
#define I_C				1
typedef struct
{
	uint16_t inline_current_adc_value[3];			// 三路电流采样的ADC原始值
	int32_t i_a,i_b,i_c;							// 单位是mA，有正负之分		
	int32_t offset_ia,offset_ib,offset_ic;			// 电流基准偏差，为零电流通过时的ADC稳态值
	int32_t i_q,i_d,i_q_filter,i_d_filter;			// Q轴电流，D轴电流，滤波后的Q轴电流，D轴电流
	void (*inline_current_read_adc)(void);			// 读取电流的底层函数
	void (*inline_current_adc_init)(void);			// 初始化电流ADC采样的底层函数
}current_struct;
extern uint16_t inline_current_adc_value[3];
void inline_current_get_current(current_struct * current_p);
void inline_currentsense_init(current_struct*current_p);
void current_duty_calculate(foc_struct *foc_p, current_struct*current_p,int32_t now_encoder_data);
#endif


