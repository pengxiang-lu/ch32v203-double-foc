#include "driver_config.h"
#include "inline_current.h"


#if   CURRENT_SENSOR_TYPE == SENSOR_A_B_C	
#define I_A_ENABLE				 			
#define I_B_ENABLE				
#define I_C_ENABLE		 		

#elif CURRENT_SENSOR_TYPE == SENSOR_A_C	
#define I_A_ENABLE				 			
#define I_C_ENABLE							

#elif CURRENT_SENSOR_TYPE == SENSOR_A_B	
#define I_A_ENABLE				 			
#define I_B_ENABLE							

#elif CURRENT_SENSOR_TYPE == SENSOR_B_C	
#define I_B_ENABLE				 			
#define I_C_ENABLE							
#endif

// 查找 ADC 零偏移量的函数
void inline_current_calibrate_offsets(current_struct * current_p)
{
    // 读数1000次
    for (int i = 0; i < 1000; i++)
	{
		current_p->inline_current_read_adc();
		delay_ms(1);
        #ifdef I_A_ENABLE				
			current_p->offset_ia += current_p->inline_current_adc_value[I_A];
		#endif
		
		#ifdef I_B_ENABLE			
			current_p->offset_ib += current_p->inline_current_adc_value[I_B];
		#endif
		
		#ifdef I_C_ENABLE				
			current_p->offset_ic += current_p->inline_current_adc_value[I_C];
		#endif

    }
    // 求平均，得到误差，单位是ADC采样精度12位
	#ifdef I_A_ENABLE				
		current_p->offset_ia = current_p->offset_ia / 1000;
	#endif
	
	#ifdef I_B_ENABLE				
		current_p->offset_ib = current_p->offset_ib / 1000;
	#endif
	
	#ifdef I_C_ENABLE				
		current_p->offset_ic = current_p->offset_ic / 1000;
	#endif

}
//内置电流传感器初始化
void inline_currentsense_init(current_struct * current_p)
{
    current_p->inline_current_adc_init();

    inline_current_calibrate_offsets(current_p); 		// 校准
}


// 读取两相电流
void inline_current_get_current(current_struct * current_p)
{
	current_p->inline_current_read_adc();
	#ifdef I_A_ENABLE				
		current_p->i_a = SENSOR_A_DIR*ADC_RATIO*((int32_t)current_p->inline_current_adc_value[I_A] - (int32_t)current_p->offset_ia);
	#endif
	
	#ifdef I_B_ENABLE				
		current_p->i_b = SENSOR_B_DIR*ADC_RATIO*((int32_t)current_p->inline_current_adc_value[I_B] - (int32_t)current_p->offset_ib);
	#endif
	
	#ifdef I_C_ENABLE				
		current_p->i_c = SENSOR_C_DIR*ADC_RATIO*((int32_t)current_p->inline_current_adc_value[I_C] - (int32_t)current_p->offset_ic);
	#endif
}

void current_duty_calculate(foc_struct *foc_p, current_struct*current_p,int32_t now_encoder_data)
{
	static int32_t 	location_temp_a = 0;
	static int32_t 	location_temp_b = 0;
	static int32_t 	location_temp_c = 0;
	// 计算电角度 
	foc_p->encoder_el = foc_p->motor_rotation_direction*foc_p->motor_pole_pairs*now_encoder_data-foc_p->motor_zero_location;
	// 归一化
	foc_p->encoder_el = normalize(foc_p->encoder_el);
	// 计算三个角度
	location_temp_a = foc_p->encoder_el - ENCODER_PRECISION_6;
	
	location_temp_b = foc_p->encoder_el;
	
	location_temp_c = foc_p->encoder_el + ENCODER_PRECISION_6;
	// 归一化
	location_temp_a = normalize(location_temp_a);
	
	location_temp_b = normalize(location_temp_b);
	
	location_temp_c = normalize(location_temp_c);
	
	// Q轴和D轴电流计算
	#if CURRENT_SENSOR_TYPE == SENSOR_A_C	
	
	current_p->i_q = -(current_p->i_a*fast_cos(location_temp_a)+current_p->i_c*fast_cos(location_temp_b))/1732;	// 相当于乘2除根号3再除2000*38>>16;
	
	current_p->i_d = -(current_p->i_a*fast_sin(location_temp_a)+current_p->i_c*fast_sin(location_temp_b))/1732;	// 考虑了表的放缩	
	
	#elif CURRENT_SENSOR_TYPE == SENSOR_A_B	
	
	current_p->i_q = (current_p->i_a*fast_cos(location_temp_c)+current_p->i_b*fast_cos(location_temp_b))/1732;	// 相当于乘2除根号3再除2000*38>>16;
	
	current_p->i_d = (current_p->i_a*fast_sin(location_temp_c)+current_p->i_b*fast_sin(location_temp_b))/1732;	// 考虑了表的放缩	
	
	#elif CURRENT_SENSOR_TYPE == SENSOR_B_C

	current_p->i_q = (current_p->i_b*fast_cos(location_temp_a)-current_p->i_c*fast_cos(location_temp_c))/1732;	// 相当于乘2除根号3再除2000*38>>16;
	
	current_p->i_d = (current_p->i_b*fast_sin(location_temp_a)-current_p->i_c*fast_sin(location_temp_c))/1732;	// 考虑了表的放缩	

	#elif CURRENT_SENSOR_TYPE == SENSOR_A_B_C	
	
	current_p->i_q = (-current_p->i_a*fast_sin(location_temp_b)+current_p->i_b*fast_sin(location_temp_c)+current_p->i_c*fast_sin(location_temp_a))/3000;	// 相当于乘2除根号3再除2000*38>>16;
	
	current_p->i_d = (current_p->i_a*fast_cos(location_temp_b)-current_p->i_b*fast_cos(location_temp_c)-current_p->i_c*fast_cos(location_temp_a))/3000;		// 考虑了表的放缩	
	
	#endif
}


