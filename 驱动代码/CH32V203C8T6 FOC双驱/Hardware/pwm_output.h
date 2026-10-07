#ifndef _PWM_OUTPUT_H_
#define _PWM_OUTPUT_H_
#include <stdint.h>

// 左电机端口
#define L_MOTOR_A_PHASE_PORT            GPIOB                       			// 左电机A相控制引脚端口
#define L_MOTOR_A_PHASE_PIN             GPIO_Pin_8                          	// 左电机A相控制引脚号

#define L_MOTOR_B_PHASE_PORT            GPIOB                      			    // 左电机B相控制引脚端口
#define L_MOTOR_B_PHASE_PIN             GPIO_Pin_7                              // 左电机B相控制引脚号

#define L_MOTOR_C_PHASE_PORT            GPIOB                        			// 左电机C相控制引脚端口
#define L_MOTOR_C_PHASE_PIN             GPIO_Pin_6                             // 左电机C相控制引脚号
// 右电机端口
#define R_MOTOR_A_PHASE_PORT            GPIOA                                   // 左电机A相控制引脚端口
#define R_MOTOR_A_PHASE_PIN             GPIO_Pin_0                              // 左电机A相控制引脚号

#define R_MOTOR_B_PHASE_PORT            GPIOA                                   // 左电机B相控制引脚端口
#define R_MOTOR_B_PHASE_PIN             GPIO_Pin_1                              // 左电机B相控制引脚号

#define R_MOTOR_C_PHASE_PORT            GPIOA                                   // 左电机C相控制引脚端口
#define R_MOTOR_C_PHASE_PIN             GPIO_Pin_2                              // 左电机C相控制引脚号
       					     
void left_motor_duty_set        (uint16_t a_duty, uint16_t b_duty, uint16_t c_duty);
void left_motor_output_init     (uint16_t timer_period);
void right_motor_duty_set        (uint16_t a_duty, uint16_t b_duty, uint16_t c_duty);
void right_motor_output_init     (uint16_t timer_period);
void motor_callback_init(void);


#endif


