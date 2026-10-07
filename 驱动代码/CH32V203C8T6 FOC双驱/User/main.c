#include "driver_config.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    delay_init();                               // 延时函数初始化
    interface_init();                           // 底层接口函数与结构体的绑定初始化
    pid_parama_init();                          // pid参数初始化
    interrupt_global_disable();                 // 关闭所有中断
#if LEFT_MOTOR_STATE    ==     DRIVER_ENABLE
    motor_foc_control_init(&left_motor);        // 读取flash的零点，方向和极对数信息
    motor_led_init(&left_motor.led_struct);     // 左电机LED灯初始化
#endif

#if RIGHT_MOTOR_STATE    ==     DRIVER_ENABLE
    motor_foc_control_init(&right_motor);       // 读取flash的零点，方向和极对数信息
    motor_led_init(&right_motor.led_struct);    // 右电机LED灯初始化
#endif
    gpio_key_init();                            // 按键初始化
    motor_callback_init();                      // 回调函数初始化
    motor_uart_init();                          // 串口输出初始化
    battery_init(&battery_struct,BATTERY_ERR_VOLTAGE);      // 电池电压检测初始化
    interrupt_global_enable();                  // 开启所有中断

#if WATCH_DOG == DRIVER_ENABLE                  // 如果使能了看门狗
    watch_dog_init(781);                        // 5秒喂狗时间
#endif
    while(1)
    {
#if WATCH_DOG == DRIVER_ENABLE
        feed_dog();
#endif
        delay_ms(DRIVER_RESPONSE_CYCLE);        // 延时操作
        key_scan();                             // 扫描按键
#if LEFT_MOTOR_STATE == DRIVER_ENABLE
        motor_led_loop(&left_motor.led_struct); // LED指示灯
#endif

#if RIGHT_MOTOR_STATE == DRIVER_ENABLE
        motor_led_loop(&right_motor.led_struct);// LED指示灯
#endif

        //battery_adc_loop(&battery_struct);                     // 电池电压检测主循环
    }
}
