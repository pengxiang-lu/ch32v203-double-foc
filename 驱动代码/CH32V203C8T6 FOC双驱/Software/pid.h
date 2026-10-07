#ifndef __PID_H_
#define __PID_H_
#include <stdint.h>
typedef struct
{
	float p;					// 参数p
	int32_t p_t;				// 转化后的参数p
	float i;					// 参数i
	int32_t i_t;				// 转化后的参数i
	float d;					// 参数d
	int32_t d_t;				// 转化后的参数d
	int32_t integral_prev;		// 记录的积分
	int32_t error_prev;			// 记录的误差
	int32_t limit;				// 积分限幅
}pid_struct;
int32_t pi_controller(pid_struct * pid_p,int32_t error);
int32_t p_controller(pid_struct * pid_p,int32_t error);
void pid_parama_init(void);
#endif


