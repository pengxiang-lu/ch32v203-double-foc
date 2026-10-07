#ifndef __DRIVER_CONFIG_H_
#define __DRIVER_CONFIG_H_
// 头文件包含
#include "flash.h"
#include "mt6701.h"
#include "motor_control.h"
#include "foc.h"
#include "pwm_output.h"
#include "debug.h"
#include "serial.h"
#include "pid.h"
#include "motor_uart.h"
#include "driver_gpio.h"
#include "driver_adc.h"
#include "magnetic_encoder.h"
#include "inline_current.h"
#include "timer.h"
#include "fifo.h"
#include "interrupt.h"
#include "battery.h"
#include "interface.h"
#include "watch_dog.h"


#define DRIVER_ENABLE                  	(1)             // 使能
#define YES                             (1)             // 是
#define DRIVER_DISABLE                 	(0)             // 失能
#define NO                              (0)             // 否
/**************************** 电机配置 ****************************/
#define LEFT_MOTOR_STATE            DRIVER_ENABLE
#define RIGHT_MOTOR_STATE           DRIVER_ENABLE
// PWM重装值配置
#define MOTOR_DUTY_BIT				12								// 满占空比位数
#define MOTOR_DUTY_MAX				4096							// 满占空比,长度为2的12次方
#define PWM_PRIOD_LOAD      		4000           					// PWM周期装载值
#define OUTPUT_DUTY_MAX          	PWM_PRIOD_LOAD      	 		// 占空比输出最大值，即PWM周期装载值
// 编码器配置
#define ENCODER_OUTPUT_MAX			16383							// 编码器采样最大值
#define ENCODER_PRECISION           (ENCODER_OUTPUT_MAX+1)         	// 编码器精度  32767
// 频率和周期配置，频率单位为Hz，周期单位为ms
#define DRIVER_RESPONSE_CYCLE       10        						// 驱动响应周期,用于主循环延时,建议范围 1 ~ 20ms
#define F_OPEN                		20000				          	// FOC的开环频率频率
#define F_VECOCITY				    2000							// 速度环频率，这里为2KHZ
#define F_VECOCITY_OB				6000							// 速度环观测频率
#define F_ANGLE						500								// 角度环频率
// 栅极驱动模式配置	
#define PWM_3						(0)								// 3PWM模式
#define PWM_6						(1)								// 6PWM模式
#define PRE_DRIVER_MODE				PWM_3							// 栅极驱动模式
// 使用配置
#define FIRST_USE                   NO                              // 是否第一次使用，并且烧录时不能擦除所有flash
#define WATCH_DOG                   DRIVER_DISABLE                  // 看门狗使能，时间5s
/**************************** 电机配置 ****************************/

/************************* 电流传感器配置 *************************/
// 采样基本参数配置
#define CURRENT_SENSOR						DRIVER_ENABLE	// 是否有电流传感器，如果有可以选择使能，会初始化传感器和测量电流
#define SHUNT_RESISTER						0.01			// 分流电阻,单位为Ω
#define GAIN								20				// 放大倍数
#define ADC_REF_VOLTAGE 					3.3           	// ADC电压,单位为V
#define ADC_PRECISION  						4096     		// ADC分辨率
#define ADC_RATIO							(uint16_t)(1000*ADC_REF_VOLTAGE/ADC_PRECISION/GAIN/SHUNT_RESISTER)
// 采样传感器配置
#define SENSOR_A_B							(0)				// AB相有电流传感器
#define SENSOR_A_C							(1)				// AC相有电流传感器
#define SENSOR_B_C							(2)				// BC相有电流传感器
#define SENSOR_A_B_C						(3)				// ABC相有电流传感器

#define CURRENT_SENSOR_TYPE					SENSOR_A_C
// 采样方向配置
#define POSITIVE							1				// 电流测量为正向(单相桥向外输出电流流向为N->P)
#define NEGATIVE							-1				// 电流测量为反向

#define SENSOR_A_DIR						NEGATIVE
#define SENSOR_B_DIR						POSITIVE
#define SENSOR_C_DIR						POSITIVE

/************************* 电流传感器配置 *************************/


/**************************** 模式配置 ****************************/
// 电机工作模式
#define OPEN_LOOP_MODE						(0)				// 开环模式                                
#define VECOCITY_LOOP_MODE					(1)				// 速度环模式
#define ANGLE_LOOP_MODE						(2)				// 角度环模式
#define ANGLE_VECOCITY_LOOP_MODE			(3)				// 速度环串 角度环模式
// 如果没有电流传感器,就不能选择以下带电流环的模式
#define I_Q_LOOP_MODE					    (4)				// I_Q和I_D闭环模式,默认I_D为0
#define I_D_LOOP_MODE                       (5)             // I_D闭环模式,此时I_Q没有闭环
#define VECOCITY_CURRENT_LOOP_MODE			(6)				// 速度环串 电流环模式
#define ANGLE_CURRENT_LOOP_MODE				(7)				// 角度环串 电流环模式
#define ANGLE_VECOCITY_CURRENT_LOOP_MODE	(8)				// 角度环串 速度环串 电流环模式

#define FOC_MODE 							VECOCITY_CURRENT_LOOP_MODE

// 调制模式
#define SPWM								(0)
#define SVPWM_7								(1)
#define SVPWM_5								(2)				// 不建议使用，不好用
#define PWM_MODE							SVPWM_7			// 调制方式

//// 电机旋转方向
//#define DIR_CW								(0)				// 旋转方向正向
//#define DIR_CCW								(1)				// 旋转方向反向
//#define MOTOR_DIR							DIR_CW

// 调制方式

#if	FOC_MODE == 4 || FOC_MODE == 5 || FOC_MODE == 6 || FOC_MODE == 7 || FOC_MODE == 8
#define  FOC_MODE_WITH_CURRENT
#endif
/**************************** 模式配置 ****************************/

/**************************** 数据回传 ****************************/
#define  DATA_PRINTF					DRIVER_ENABLE				// 回传数据总使能开关
#define  ANGLE_PRINTF				    DRIVER_DISABLE				// 磁编数据回传
#define  VECOCITY_PRINTF				DRIVER_ENABLE				// 回传速度
#define  CURRENT_SENSOR_PRINTF			DRIVER_DISABLE				// 传感器电流原始数据回传
#define  U_Q_PRINTF						DRIVER_DISABLE				// U_Q回传
#define  I_Q_I_D_PRINTF					DRIVER_ENABLE				// i_q,i_d数据回传
#define  BATTERY_PRINTF					DRIVER_DISABLE				// 电池电压回传
#define  TIME_PRINTF					DRIVER_ENABLE				// 单次执行周期回传
/**************************** 数据回传 ****************************/


/**************************** 电池配置 ****************************/
#define R_P                             33.2f                                               // 高侧电阻阻值，单位kΩ
#define R_N                             10.0f                                               // 低测电阻阻值，单位kΩ
#define VOLTAGE_RATIO                   (R_N/(R_P+R_N))                                     // 电池分压比
#define CONVERSION_COEFFICIENT          ADC_REF_VOLTAGE/ADC_PRECISION/VOLTAGE_RATIO         // 电池电压转换系数 12位ADC采集值 直接乘以该值则可得出电池电压
#define BATTERY_RECTIFY_COEFFICIENT     (0.9059968)          // 电池电压校准系数 根据实际硬件调整
#define BATTERY_PROTECT                 DRIVER_DISABLE      // 是否开启电池保护功能(默认开启)      DRIVER_ENABLE: 开启电池保护    DRIVER_DISABLE: 不开启电池保护
#define BATTERY_ERR_VOLTAGE             11.3f               // 错误状态电压，低于此电压时电池状态错误
/**************************** 电池保护 ****************************/


/**************************** 堵转保护 ****************************/
#define MOTOR_LOCKED_PROTECT           DRIVER_DISABLE   // 是否开启堵转保护功能(默认开启)      DRIVER_ENABLE: 开启堵转保护    DRIVER_DISABLE: 不开启堵转保护
#define MOTOR_LOCKED_DUTY_MAX          (0.2f)          // 堵转检测最大占空比(默认20%)  若超过此值并且电机未旋转 则认为可能堵转
#define MOTOR_LOCKED_TIME              (500)           // 堵转检测时长(默认500ms)      若认为电机可能堵转 并且持续时长超过此值 则认定为堵转状态
/**************************** 堵转保护 *************************/
					
// 限幅配置
#define MAIN_DUTY_LIMIT             2000                            // 占空比限幅
#define I_Q_LIMIT                   2000                            // 内环输出i_q最大值限幅
#define VECOCITY_LIMIT              1000                            // 内环输出速度限幅



#endif


