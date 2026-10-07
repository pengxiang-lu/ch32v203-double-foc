#ifndef __DRIVER_ADC_H_
#define __DRIVER_ADC_H_
#include <stdint.h>
void current_sensor_adc_init(void);
void battery_adc_init(void);
uint16_t battery_adc_read(void);
void left_inline_current_read_adc(void);
void right_inline_current_read_adc(void);
#endif


