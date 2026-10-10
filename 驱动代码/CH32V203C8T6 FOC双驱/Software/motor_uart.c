#include "driver_config.h"
fifo_struct     motor_driver_fifo;              // 串口通讯的 FIFO 结构体

uint8_t  driver_fifo_buffer[128];        // FIFO 指向的缓冲数组

uint8_t  read_buffer[128];               // 解析时，读取数据的缓冲数组

int16_t  receive_enter_flag = 0;         // 回车标志位

//封装底层串口和定时器初始化
void motor_uart_init()
{
	Serial_Init();
	TIM3_Init();
	fifo_init(&motor_driver_fifo,FIFO_DATA_8BIT, driver_fifo_buffer, 128);     // 初始化 fifo 结构体
}
//数据回传
void motor_uart_data_callback()
{	
// 打印使能
#if DATA_PRINTF == DRIVER_ENABLE
// 打印角度
	#if ANGLE_PRINTF == DRIVER_ENABLE
        #if LEFT_MOTOR_STATE == DRIVER_ENABLE
		printf("%d,",left_motor.motor_angle);
        #endif
        #if RIGHT_MOTOR_STATE == DRIVER_ENABLE
        printf("%d,",right_motor.motor_angle);
        #endif
	#endif
// 打印速度
	#if VECOCITY_PRINTF == DRIVER_ENABLE
        #if LEFT_MOTOR_STATE == DRIVER_ENABLE
        printf("%d,",left_motor.motor_speed_filter);
        #endif
        #if RIGHT_MOTOR_STATE == DRIVER_ENABLE
        printf("%d,",right_motor.motor_speed_filter);
        #endif
	#endif
// 打印电流传感器电流
	#if CURRENT_SENSOR_PRINTF == DRIVER_ENABLE
        #if LEFT_MOTOR_STATE == DRIVER_ENABLE
        printf("%d,%d,",left_motor.current_struct.i_a,left_motor.current_struct.i_c);
        #endif
        #if RIGHT_MOTOR_STATE == DRIVER_ENABLE
        printf("%d,%d,",right_motor.current_struct.i_a,right_motor.current_struct.i_c);
        #endif
	#endif
// 打印占空比U_Q
	#if U_Q_PRINTF == DRIVER_ENABLE
        #if LEFT_MOTOR_STATE == DRIVER_ENABLE
        printf("%d,",left_motor.u_q_duty);
        #endif
        #if RIGHT_MOTOR_STATE == DRIVER_ENABLE
        printf("%d,",right_motor.u_q_duty);
        #endif
	#endif

// 打印计算出来的I_Q,I_D
	#if I_Q_I_D_PRINTF == DRIVER_ENABLE
        #if LEFT_MOTOR_STATE == DRIVER_ENABLE
        printf("%d,%d,",left_motor.current_struct.i_q_filter,left_motor.current_struct.i_d_filter);
        #endif
        #if RIGHT_MOTOR_STATE == DRIVER_ENABLE
        printf("%d,%d,",right_motor.current_struct.i_q_filter,right_motor.current_struct.i_d_filter);
        #endif
	#endif

// 打印电池电压
	#if BATTERY_PRINTF == DRIVER_ENABLE
		printf("%.2f,",battery_struct.battery_voltage);
	#endif
//	#if TIME_PRINTF == DRIVER_ENABLE
//		printf("%d",left_motor.single_calculation_use_time);
//	#endif

	printf("0\r\n");
#endif
}
void motor_driver_fifo_clear(uint32_t clear_length)
{
    fifo_read_buffer(&motor_driver_fifo, read_buffer, &clear_length, FIFO_READ_AND_CLEAN);
}
void motor_driver_parse_statement(uint8_t *statement_buffer)
{
	if(FOC_MODE==OPEN_LOOP_MODE)			// 如果是开环模式，控制的是电压占空比
	{
		left_motor.u_q_duty=((int16_t)statement_buffer[1] << 8) | (int)statement_buffer[2];
		right_motor.u_q_duty=((int16_t)statement_buffer[3] << 8) | (int)statement_buffer[4];
	}
	else if(FOC_MODE==VECOCITY_LOOP_MODE || FOC_MODE==VECOCITY_CURRENT_LOOP_MODE)	// 如果是速度模式，控制的是电机的速度
	{
		left_motor.motor_speed_set=(int16_t)(((uint16_t)statement_buffer[1] << 8) | (uint16_t)statement_buffer[2]);
		right_motor.motor_speed_set=(int16_t)(((uint16_t)statement_buffer[3] << 8) | (uint16_t)statement_buffer[4]);
	}
	else if(FOC_MODE==ANGLE_LOOP_MODE || FOC_MODE == ANGLE_VECOCITY_LOOP_MODE || FOC_MODE == ANGLE_CURRENT_LOOP_MODE)	//如果是角度模式，控制的是角度
	{
		left_motor.motor_angle_set=((int16_t)statement_buffer[1] << 8) | (int)statement_buffer[2];
		right_motor.motor_angle_set=((int16_t)statement_buffer[3] << 8) | (int)statement_buffer[4];
	}
	else if(FOC_MODE==I_Q_LOOP_MODE)	// 如果是I_Q电流闭环模式，控制的是Q轴电流
	{
		left_motor.i_q_set=((int16_t)statement_buffer[1] << 8) | (int)statement_buffer[2];
		right_motor.i_q_set=((int16_t)statement_buffer[3] << 8) | (int)statement_buffer[4];
	}
    else if(FOC_MODE==I_D_LOOP_MODE)    // 如果是I_D电流闭环模式，控制的是Q轴电流
    {
        left_motor.i_d_set=((int16_t)statement_buffer[1] << 8) | (int)statement_buffer[2];
        right_motor.i_d_set=((int16_t)statement_buffer[3] << 8) | (int)statement_buffer[4];
    }
}
void motor_driver_control_loop()
{
	uint32_t read_length = 0;                                                     // 定义读取长度变量
    uint8_t  check_data = 0;                                                      // 定义数据校验变量
	if(fifo_used(&motor_driver_fifo) >= 6)                                        // 判断 FIFO 缓冲区长度是否大于4(去掉两个包头)
	{
		read_length = 6;                                                     	  // 成功判断到帧头 仅读取一包数据(长度为5位)
		
		fifo_read_buffer(&motor_driver_fifo, read_buffer, &read_length, FIFO_READ_ONLY);
		
		for(int i = 0; i < 5; i ++)                                         	  // 和校验拟合
		{
			check_data += read_buffer[i];
		}
		if(check_data == read_buffer[5]&&read_buffer[0]==0xA5)                                    // 判断和校验
		{
			read_length= 6;                                                 // 成功通过和校验 从 FIFO 读取并擦除 一包数据(长度为5位)
			
			fifo_read_buffer(&motor_driver_fifo, read_buffer, &read_length, FIFO_READ_AND_CLEAN);
			
			motor_driver_parse_statement(read_buffer);        // 调用数据解析      
		}
		else
		{
			motor_driver_fifo_clear(1);                                     // 和校验未通过 清除一个缓冲区数据（状态机机制）
		}
	}
}
void motor_driver_control_callback(uint8_t receive_data)
{
    fifo_write_element(&motor_driver_fifo, receive_data);                   // 将数据写入 fifo
        
	if(receive_data == '\n')                                                // 若接收到特殊指令 回车：'\n'  则将回车标志位置位 用于后续解析
	{
		receive_enter_flag = 1;
	}
        
    motor_driver_control_loop();                                			// 调用数据解析 该函数也可在其他位置调用
}
