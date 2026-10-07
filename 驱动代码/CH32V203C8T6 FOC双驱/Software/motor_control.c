#include "driver_config.h"		
#include "math.h"
motor_struct left_motor;											// 定义左电机的结构体
motor_struct right_motor;                                           // 定义右电机的结构体
// 空函数
void motor_null()
{

}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电机 FLASH 参数 读取
// 参数说明     void
// 返回参数     void
// 使用示例     motor_flash_read();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void left_motor_flash_read(void)
{
    // 左电机数据读取
    left_motor.zero_location=FLASH_Read(MOTOR_FLASH_ADDR);
    left_motor.rotation_direction=FLASH_Read(MOTOR_FLASH_ADDR+2);
    left_motor.pole_pairs=FLASH_Read(MOTOR_FLASH_ADDR+4);
}
void right_motor_flash_read(void)
{
    // 右电机数据读取
    right_motor.zero_location=FLASH_Read(MOTOR_FLASH_ADDR+6);
    right_motor.rotation_direction=FLASH_Read(MOTOR_FLASH_ADDR+8);
    right_motor.pole_pairs=FLASH_Read(MOTOR_FLASH_ADDR+10);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电机 FLASH 参数 写入
// 参数说明     void
// 返回参数     void
// 使用示例     motor_flash_write();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void left_motor_flash_write()
{
    // 先读取右电机数据
    uint16_t temp[3];
    temp[0] = FLASH_Read(MOTOR_FLASH_ADDR+6);
    temp[1] = FLASH_Read(MOTOR_FLASH_ADDR+8);
    temp[2] = FLASH_Read(MOTOR_FLASH_ADDR+10);
    // 擦除
    Flash_Erase(MOTOR_FLASH_ADDR);
    // 左电机数据写入
    Flash_WriteHalfWord(MOTOR_FLASH_ADDR,left_motor.zero_location);
    Flash_WriteHalfWord(MOTOR_FLASH_ADDR+2,left_motor.rotation_direction);
    Flash_WriteHalfWord(MOTOR_FLASH_ADDR+4,left_motor.pole_pairs);
    // 右电机数据写入
    Flash_WriteHalfWord(MOTOR_FLASH_ADDR+6,temp[0]);
    Flash_WriteHalfWord(MOTOR_FLASH_ADDR+8,temp[1]);
    Flash_WriteHalfWord(MOTOR_FLASH_ADDR+10,temp[2]);
}
void right_motor_flash_write()
{
    // 先读取左电机数据
    uint16_t temp[3];
    temp[0] = FLASH_Read(MOTOR_FLASH_ADDR);
    temp[1] = FLASH_Read(MOTOR_FLASH_ADDR+2);
    temp[2] = FLASH_Read(MOTOR_FLASH_ADDR+4);
    // 擦除
    Flash_Erase(MOTOR_FLASH_ADDR);
    // 左电机数据写入
    Flash_WriteHalfWord(MOTOR_FLASH_ADDR,temp[0]);
    Flash_WriteHalfWord(MOTOR_FLASH_ADDR+2,temp[1]);
    Flash_WriteHalfWord(MOTOR_FLASH_ADDR+4,temp[2]);
    // 右电机数据写入
    Flash_WriteHalfWord(MOTOR_FLASH_ADDR+6,right_motor.zero_location);
    Flash_WriteHalfWord(MOTOR_FLASH_ADDR+8,right_motor.rotation_direction);
    Flash_WriteHalfWord(MOTOR_FLASH_ADDR+10,right_motor.pole_pairs);
}
// 三相自检，检测MOS和预驱的功能是否正常
void motor_phase_check(motor_struct * motor_p)
{
    uint16_t check_period;
    uint16_t delay_time = 100;
    
    // 第一声  A相上桥 → B相下桥
    check_period = 36000;                       // 2.222khz
    
    motor_p->motor_output_init(check_period);
    
    motor_p->motor_set_duty(check_period / 50, 0, 0);
    
    delay_ms(delay_time);
    
    motor_p->motor_set_duty(0, 0, 0);
    
    delay_ms(50);
    
    // 第二声  B相上桥 → C相下桥
    check_period = 35000;                       // 2.285khz
    
    motor_p->motor_output_init(check_period);
    
    motor_p->motor_set_duty(0, check_period / 50, 0);
    
    delay_ms(delay_time);
    
    motor_p->motor_set_duty(0, 0, 0);
    
    delay_ms(50);
    
    // 第三声  C相上桥 → A相下桥
    check_period = 34000;                       // 2.352khz
    
    motor_p->motor_output_init(check_period);
    
    motor_p->motor_set_duty(0, 0, check_period / 50);
    
    delay_ms(delay_time);
    
    motor_p->motor_set_duty(0, 0, 0);
    
	delay_ms(50);

}

void motor_zero_calibration(motor_struct * motor_p)
{        
    int16_t  rotation_direction = 0;
    int32_t  encoder_data_integral = 0;
    interrupt_global_disable();                 // 关闭所有中断

    if(motor_p->motor_speed_filter != 0)      // 如果电机正在旋转 则刹车
    {
        motor_p->motor_set_duty(0, 0, 0);                                           // 左侧电机刹车
                                        
        delay_ms(1000);                                                             // 刹车等待
    }
    motor_p->encoder_struct.magnetic_encoder_init();                                // 初始化相应的编码器

    motor_phase_check(motor_p);                                                     // 三相 MOS 及 预驱 功能检测 由于没有检测三相电流 因此需要人为判断是否响三声
    
    motor_p->motor_output_init(PWM_PRIOD_LOAD);                                     // 左侧三相 PWM 输出重新初始化
    
    // 矫正零点时默认恢复电机工作保护状态
    motor_p->led_struct.motor_protect_state  = NORMAL_STATE;
    
    // 初始化默认配置参数
    foc_init(&motor_p->foc_cal_struct,1, 0, 1);
    
    // 开环定位 20% 占空比
    foc_calculate(&motor_p->foc_cal_struct, 0,500,0);
    // 输出占空比到电机
    motor_p->motor_set_duty( motor_p->foc_cal_struct.output_duty[0], 
                    motor_p->foc_cal_struct.output_duty[1],
                    motor_p->foc_cal_struct.output_duty[2]);
    
    // 延时 200ms 等待电机回到零点
    delay_ms(1000); 
    motor_p->zero_location = encoder_get_absolute_data(&motor_p->encoder_struct)-ENCODER_PRECISION/4;

    // 读取当前磁编数据
    encoder_get_absolute_data(&motor_p->encoder_struct);  
    
    // 开环牵引 100 次 
    for(uint16_t i = 0; i <= 100; i ++)                                                                         
    {
        // 以 10% 占空比循环牵引
        foc_calculate(&motor_p->foc_cal_struct,ENCODER_OUTPUT_MAX*i/100,500,0);
        
        // 输出占空比到电机
        motor_p->motor_set_duty( motor_p->foc_cal_struct.output_duty[0], 
                        motor_p->foc_cal_struct.output_duty[1],
                        motor_p->foc_cal_struct.output_duty[2]);
        
        // 单次牵引间隔为（默认5ms） 
        delay_ms(5);  

        encoder_get_absolute_data(&motor_p->encoder_struct);// 获取当前磁编码器数值
        // 累计单次牵引的旋转方向
        if(motor_p->encoder_struct.encoder_value_offset > 0)
        {
            rotation_direction ++;
        }
        else if(motor_p->encoder_struct.encoder_value_offset < 0)
        {
            rotation_direction --;
        }
        
        // 累积单次牵引的旋转数值
        encoder_data_integral += motor_p->encoder_struct.encoder_value_offset;
    }                                                               
       
    
    // 关闭输出
    motor_p->motor_set_duty(0, 0, 0);                                                        
    // 判断是否正确读取到磁编数据 开环牵引积分小于 1000 则认为电机没有旋转   折算下来为11°意味着超过 32 对极的电机无法检测
    if(func_abs(encoder_data_integral) < 1000)
    {
        // 更改磁编码器状态标志
        motor_p->led_struct.encoder_state = ERROR_STATE;

        // 修正默认参数
        motor_p->zero_location = 0;
        motor_p->rotation_direction = -1;
        motor_p->pole_pairs = 7;

        // 错误信息打印到 串口
        printf("%s magnetic encoder error!!\r\n",motor_p->motor_name);

    }
    else
    {
        // 更改磁编码器状态标志
        motor_p->led_struct.encoder_state = NORMAL_STATE;
        
        // 旋转方向归一化 正转则为 1  反转则为 -1
        motor_p->rotation_direction = rotation_direction / (rotation_direction > 0 ? rotation_direction : -rotation_direction);
        
        // 计算极对数  磁编最大值 除以 单圈电角度积分值 四舍五入
        motor_p->pole_pairs = (int16_t)round(ENCODER_OUTPUT_MAX/encoder_data_integral*motor_p->rotation_direction);

        motor_p->zero_location = motor_p->rotation_direction*motor_p->pole_pairs*motor_p->zero_location%ENCODER_PRECISION;
        
        motor_p->zero_location = motor_p->zero_location>0?motor_p->zero_location:motor_p->zero_location+ENCODER_PRECISION;
        // 按照计算的数据重新初始化fast_foc参数
        foc_init(&motor_p->foc_cal_struct,motor_p->pole_pairs, motor_p->zero_location, motor_p->rotation_direction);
        // 成功初始化电机指示
        printf("%s init successfully\r\n",motor_p->motor_name);
        // 矫正信息打印到 串口
        printf("motor zero:%d, dir:%s, pole pairs:%d\r\n", motor_p->foc_cal_struct.motor_zero_location, 
                                                          motor_p->foc_cal_struct.motor_rotation_direction == 1 ? "forward" : "reverse",
                                                          motor_p->pole_pairs);
        motor_p->write_flash();

    }
    delay_ms(1000);
    interrupt_global_enable();              //开启所有中断
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电机FOC控制初始化
// 参数说明     void
// 返回参数     void
// 使用示例     motor_foc_control_init();
// 备注信息       
//-------------------------------------------------------------------------------------------------------------------
void motor_foc_control_init(motor_struct * motor_p)
{ 
    motor_p->led_struct.motor_protect_state  = NORMAL_STATE;          	    // 初始化默认电机为正常工作模式
  
    motor_p->led_struct.encoder_state        = NORMAL_STATE;           	// 初始化默认磁编码器为正常工作模式
    
    motor_p->encoder_struct.magnetic_encoder_init();            // 磁编码器初始化

    # if FIRST_USE == YES                                       // 如果是第一次使用，此时flash里面没有数据，读取会出bug
    // 这时候给默认参数
    motor_p->pole_pairs = 7;
    motor_p->zero_location = 0;
    motor_p->rotation_direction = 1;
    motor_p->write_flash();
    #else
    motor_p->read_flash();
    #endif
    foc_init(&motor_p->foc_cal_struct,motor_p->pole_pairs, motor_p->zero_location, motor_p->rotation_direction);     // 左侧电机 FAST_FOC 功能初始化
    
    motor_p->motor_output_init(PWM_PRIOD_LOAD);     			// 电机三相 PWM 输出初始化

	#if	CURRENT_SENSOR == DRIVER_ENABLE							// 带电流环则需要相应的初始化
	
	inline_currentsense_init(&motor_p->current_struct);

	#endif
	
	motor_p->motor_angle =  motor_p->rotation_direction * encoder_get_absolute_data(&motor_p->encoder_struct);
}
#define SPEED_RATIO					(60*F_VECOCITY_OB/ENCODER_PRECISION)            // 这样算不准,但是为了速度损失精度
#define VECOCITY_OB_COUNT			(F_OPEN / F_VECOCITY_OB)
#define VECOCITY_LOOP_COUNT			(F_OPEN / F_VECOCITY)
#define ANGLE_LOOP_COUNT			(F_OPEN / F_ANGLE)
// 电机执行回调函数
void motor_callback(motor_struct * motor_p)
{
	encoder_get_absolute_data(&motor_p->encoder_struct); 		                    // 采集电机磁编码器数值,偏移量等
	
	motor_p->encoder_struct.encoder_offset_integral += motor_p->encoder_struct.encoder_value_offset; // 偏移值积分 用于速度计算
	
	#if	CURRENT_SENSOR == DRIVER_ENABLE												// 使能电流传感器
	
	inline_current_get_current(&motor_p->current_struct);																			// 获取电流值
			
	current_duty_calculate(&motor_p->foc_cal_struct,&motor_p->current_struct,motor_p->encoder_struct.encoder_value_now);			// 计算当前的i_q,i_d

	motor_p->current_struct.i_q_filter = (motor_p->current_struct.i_q_filter*9 + motor_p->current_struct.i_q)/10;			// i_q滤波

	motor_p->current_struct.i_d_filter = (motor_p->current_struct.i_d_filter*9 + motor_p->current_struct.i_d)/10;			// i_d滤波
		
    #ifdef FOC_MODE_WITH_CURRENT																		// 带电流环
        #if FOC_MODE != I_D_LOOP_MODE
        motor_p->u_q_duty = pi_controller(&motor_p->i_q_pid_struct,motor_p->i_q_set - motor_p->current_struct.i_q_filter);		// Q轴PID计算
        #endif
        motor_p->u_d_duty = pi_controller(&motor_p->i_d_pid_struct,motor_p->i_d_set - motor_p->current_struct.i_d_filter);		// D轴PID计算

    #endif
		
	#endif
	if(++ motor_p->speed_observe_count >= VECOCITY_OB_COUNT)									    // 速度观测频率一定要大于速度环频率
	{
	    motor_p->speed_observe_count = 0;
		
		motor_p->motor_speed = (int32_t)(motor_p->encoder_struct.encoder_offset_integral * SPEED_RATIO)*motor_p->rotation_direction;   // 速度数据拟合,单位RPM(转每分钟),要注意方向，正力矩正方向为正方向
		
		motor_p->motor_speed_filter = (motor_p->motor_speed_filter*19+motor_p->motor_speed)/20;			                // 速度数据低通滤波
		
		motor_p->motor_angle += motor_p->rotation_direction*motor_p->encoder_struct.encoder_offset_integral;			// 角度的计算,需要带上方向
		
		motor_p->encoder_struct.encoder_offset_integral = 0;                            // 偏移积分归零
	}
	if(++ motor_p->angle_count >= ANGLE_LOOP_COUNT)
	{
	    motor_p->angle_count = 0;
		
		if(FOC_MODE == ANGLE_LOOP_MODE)													// 位置闭环
		{
			motor_p->u_q_duty = p_controller(&motor_p->angle_pid_struct,motor_p->motor_angle_set-motor_p->motor_angle);
		}
		else if(FOC_MODE == ANGLE_VECOCITY_LOOP_MODE)									// 角度环串速度环，角度外环
		{
			motor_p->motor_speed_set = p_controller(&motor_p->angle_pid_struct,motor_p->motor_angle_set-motor_p->motor_angle);
		}
		else if(FOC_MODE == ANGLE_CURRENT_LOOP_MODE && CURRENT_SENSOR==DRIVER_ENABLE)	// 电流环串角度环，角度外环
		{
			motor_p->i_q_set = p_controller(&motor_p->angle_pid_struct,motor_p->motor_angle_set-motor_p->motor_angle);
		}
		else if(FOC_MODE == ANGLE_VECOCITY_CURRENT_LOOP_MODE && CURRENT_SENSOR==DRIVER_ENABLE)
		{
			motor_p->motor_speed_set = p_controller(&motor_p->angle_pid_struct,motor_p->motor_angle_set-motor_p->motor_angle);
		}
	}
	if(++ motor_p->speed_count >= VECOCITY_LOOP_COUNT)                               			// 每半毫秒拟合一次速度
	{
		
	    motor_p->speed_count = 0;
		
		#if MOTOR_LOCKED_PROTECT == DRIVER_ENABLE                                   	  										// 输出保护计次
		
		if(func_abs(motor_p->motor_speed_filter) < 10 && func_abs(motor_p->u_q_duty) >= MOTOR_LOCKED_DUTY_MAX)  	// 判断是否堵转  占空比大于10%并且无转速数据持续 500ms
		{
		    motor_p->protect_count ++;
		
			if(motor_p->protect_count > MOTOR_LOCKED_TIME)
			{
			    motor_p->protect_count = MOTOR_LOCKED_TIME;
				
				motor_p->led_struct.motor_protect_state = ERROR_STATE;                  										// 条件成立  进入输出保护状态
			}
		}
		else
		{
		    motor_p->protect_count = 0;                                            												// 否则清空保护计次
		}
		#endif
		
		if(FOC_MODE == VECOCITY_LOOP_MODE || FOC_MODE == ANGLE_VECOCITY_LOOP_MODE)	            // 速度闭环 或 位置+速度闭环
		{	
			motor_p->u_q_duty = pi_controller(&motor_p->vecocity_pid_struct,motor_p->motor_speed_set-motor_p->motor_speed_filter);
		}
		else if(FOC_MODE == VECOCITY_CURRENT_LOOP_MODE && CURRENT_SENSOR==DRIVER_ENABLE)		// 速度环串电流环，速度外环
		{
			motor_p->i_q_set = pi_controller(&motor_p->vecocity_pid_struct,motor_p->motor_speed_set-motor_p->motor_speed_filter);
		}
		else if(FOC_MODE == ANGLE_VECOCITY_CURRENT_LOOP_MODE && CURRENT_SENSOR==DRIVER_ENABLE)			
		{
			motor_p->i_q_set = pi_controller(&motor_p->vecocity_pid_struct,motor_p->motor_speed_set-motor_p->motor_speed_filter);
		}
	}
	  
	foc_calculate(&motor_p->foc_cal_struct,motor_p->encoder_struct.encoder_value_now,motor_p->u_q_duty,motor_p->u_d_duty);  		// FOC计算
	
	// 保护状态错误

	if(motor_p->led_struct.motor_protect_state == ERROR_STATE
	   || motor_p->led_struct.encoder_state == ERROR_STATE
	   || battery_struct.battery_state == ERROR_STATE)
	{
		motor_p->motor_set_duty(0, 0, 0);
	}
	else
	{
		// 输出占空比到电机
		motor_p->motor_set_duty(motor_p->foc_cal_struct.output_duty[0],
						        motor_p->foc_cal_struct.output_duty[1],
						        motor_p->foc_cal_struct.output_duty[2]);
	}
}


void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
// 定时器2中断服务函数
void TIM1_UP_IRQHandler(void)
{
    // 检查TIM1的更新中断标志位
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
#if LEFT_MOTOR_STATE    ==     DRIVER_ENABLE
        motor_callback(&left_motor);
#endif
#if RIGHT_MOTOR_STATE    ==     DRIVER_ENABLE
        motor_callback(&right_motor);
#endif
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }

}

