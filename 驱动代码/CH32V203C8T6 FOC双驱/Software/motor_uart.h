#ifndef __MOTOR_UART_H_
#define __MOTOR_UART_H_
#include <stdint.h>
void motor_uart_init(void);
void motor_uart_data_callback(void);
void motor_driver_control_callback(uint8_t receive_data);
#endif


