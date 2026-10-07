#ifndef __MOTOR_CONTROL_H_
#define __MOTOR_CONTROL_H_
#include <stdint.h>
#include "inline_current.h"
#include "battery.h"
#include "foc.h"
#include "magnetic_encoder.h"
#include "pid.h"
#include "led.h"
#include "flash.h"
//初始变量及函数定义
#define func_abs(x)             ((x) >= 0 ? (x): -(x))
#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define fast_foc_limit_ab(x, a, b)  ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))         // FAST_FOC 宏定义限幅函数
typedef enum
{
    REVERSE = -1,                       // 反转
    FORWARD = 1,                        // 正转
}motor_dir_enum;


typedef struct
{
    int16_t                             zero_location;              // 电机零点位置
    int16_t                             rotation_direction;         // 磁编与相位输出方向
    int16_t                             pole_pairs;                 // 电机极对数
    char *                              motor_name;                 // 电机名
    // 开环参数
    int32_t                             u_q_duty;                   // 电机q轴当前占空比
	int32_t                             u_d_duty; 					// 电机d轴当前占空比 
    foc_struct                          foc_cal_struct;             // 定义计算结构体指针
	// 速度参数
	int32_t								motor_speed_set;			// 电机设定转速            	转速单位：RPM
    int32_t                             motor_speed;                // 电机当前转速            	转速单位：RPM   
    int32_t                             motor_speed_filter;         // 电机当前转速(滤波)      	转速单位：RPM   
    // 角度参数
	int32_t								motor_angle_set;			// 电机设定角度				角度单位：磁编精度
	int32_t								motor_angle;				// 电机当前角度				角度单位：磁编精度
	// 电流参数
	int32_t								i_q_set;					// 电机设定q轴电流
	int32_t								i_d_set;					// 电机设定q轴电流
    current_struct						current_struct;				// 定义电机电流结构体
	// 编码器参数
    encoder_struct						encoder_struct;				// 编码器结构体
    // LED指示灯
    led_struct                          led_struct;                 // LED指示灯
    int32_t                             single_calculation_use_time;// 软件单次计算耗时     
    // pid参数
	pid_struct							angle_pid_struct;			// 角度环pid结构体
	pid_struct   						vecocity_pid_struct;		// 速度环pid结构体
	pid_struct							i_q_pid_struct;				// i_q环pid结构体
	pid_struct							i_d_pid_struct;				// i_d环pid结构体

	// 保护计次变量
	uint8_t                             speed_count;                // 速度环计次
    uint8_t                             speed_observe_count;        // 速度观测计次
    uint8_t                             angle_count;                // 角度环计次
    uint32_t                            protect_count;              // 保护计次
	// flash函数指针
	void                                (*write_flash)(void);           // 写入flash函数
	void                                (*read_flash)(void);            // 读取flash函数
	// PWM底层函数指针
	void 				(*motor_set_duty)(uint16_t,uint16_t,uint16_t);	// 设置占空比底层函数
	void 				(*motor_output_init)(uint16_t);					// 占空比初始化底层函数
}motor_struct;
extern motor_struct left_motor;                                            // 定义主电机的结构体
extern motor_struct right_motor;                                            // 定义主电机的结构体
void motor_flash_read(void);
void motor_zero_calibration(motor_struct * motor_p);
void motor_foc_control_init(motor_struct * motor_p);
void motor_control_init(void);
void left_motor_flash_read(void);
void right_motor_flash_read(void);
void left_motor_flash_write();
void right_motor_flash_write();
void motor_null(void);
#endif


