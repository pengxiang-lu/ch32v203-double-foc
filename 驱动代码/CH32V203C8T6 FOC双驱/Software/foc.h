#ifndef __FOC_H_
#define __FOC_H_
#include <stdint.h>
#define FOC_ARRAY_LENGTH		4096									// 生成的查表数组长度为编码器精度，省去的放缩的操作
#define FOC_ARRAY_MAX			1000
#define FOC_MIDDLE_DUTY			(OUTPUT_DUTY_MAX/2)
#define ENCODER_TO_ARRAY_RATIO	FOC_ARRAY_LENGTH/ENCODER_PRECISION
#define ENCODER_PRECISION_6		(ENCODER_PRECISION/6)
#define ENCODER_PRECISION_4		(ENCODER_PRECISION/4)
#define ENCODER_PRECISION_2		(ENCODER_PRECISION/2)
#define ARRAY_TO_OUTPUT_RATIO	OUTPUT_DUTY_MAX/2/FOC_ARRAY_MAX
#define SVPWM_K					3547>>10									// SVPWM比例系数2xsqrt(3)			
typedef struct
{
	uint16_t ts,ta,tb,tc;				// 总时间
	int32_t tx,ty;						// 分别为先发生的矢量时间和或发生的矢量时间
	uint8_t sector_num;					// 扇区编号
}svpwm_struct;
typedef struct 
{
	int32_t	u_a,u_b,u_c;                // 三相电压
	int32_t	u_1,u_2,u_3;				// 三个参考电压
	int32_t u_q,u_d;			        // Q轴和D轴占空比
	int32_t encoder_el;					// 电角度,磁编单位未转化为角度
    int32_t motor_pole_pairs;           // 电机极对数
    int32_t motor_zero_location;        // 电机零点
    int32_t motor_rotation_direction;   // 电机方向
    uint16_t output_duty[3];			// 三相PWM自动重装值
	svpwm_struct svpwm;
}foc_struct;


int32_t fast_sin(int32_t x);
int32_t fast_cos(int32_t x);
int32_t normalize(int32_t location_temp);
void foc_direct_calculate(foc_struct *foc_pointer, float angle_el, float output_duty_max);
void foc_calculate(foc_struct *foc_pointer, int32_t now_encoder_data, int32_t u_q_duty,int32_t u_d_duty);
void foc_init(foc_struct *foc_pointer, uint32_t motor_pole_pairs, int32_t zero_location, int32_t rotation_direction);


#endif


