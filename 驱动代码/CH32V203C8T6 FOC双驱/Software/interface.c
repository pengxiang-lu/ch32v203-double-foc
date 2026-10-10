#include "driver_config.h"
// 底层接口初始化绑定，将函数绑定到相应的结构体下
void interface_init()
{
    // 左电机相关

	// 设置占空比相关底层函数
	left_motor.motor_set_duty	 = left_motor_duty_set;
	left_motor.motor_output_init = left_motor_output_init;
	left_motor.motor_name        = "left_motor";
	// 编码器相关的底层函数
	left_motor.encoder_struct.magnetic_encoder_init   = MT6701_Init;
	left_motor.encoder_struct.magnetic_encoder_read   = L_MT6701_GetRawAngle;
	// 电流环相关底层函数
	left_motor.current_struct.inline_current_read_adc = left_inline_current_read_adc;
	left_motor.current_struct.inline_current_adc_init = current_sensor_adc_init;
	// LED指示灯相关函数
	left_motor.led_struct.led_toggle = left_led_toggle;
	left_motor.led_struct.led_init   = left_led_init;
	// flash读取相关函数
	left_motor.write_flash           = left_motor_flash_write;
	left_motor.read_flash            = left_motor_flash_read;


	// 右电机相关

    // 设置占空比相关底层函数
    right_motor.motor_set_duty    = right_motor_duty_set;
    right_motor.motor_output_init = right_motor_output_init;
    right_motor.motor_name        = "right_motor";
    // 编码器相关的底层函数
    right_motor.encoder_struct.magnetic_encoder_init   = MT6701_Init;
    right_motor.encoder_struct.magnetic_encoder_read   = R_MT6701_GetRawAngle;
    // 电流环相关底层函数
    right_motor.current_struct.inline_current_read_adc = right_inline_current_read_adc;
    right_motor.current_struct.inline_current_adc_init = current_sensor_adc_init;
    // LED指示灯相关函数
    right_motor.led_struct.led_toggle = right_led_toggle;
    right_motor.led_struct.led_init   = right_led_init;
    // flash读取相关函数
    right_motor.write_flash           = right_motor_flash_write;
    right_motor.read_flash            = right_motor_flash_read;

    // 电池相关
    // 电池电压检测相关函数
    battery_struct.battery_adc_init = motor_null;
    battery_struct.battery_adc_read = battery_adc_read;

}
