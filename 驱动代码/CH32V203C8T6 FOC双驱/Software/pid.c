#include "driver_config.h"
/**************************** PID参数 *************************/
#if FOC_MODE == ANGLE_LOOP_MODE
// 角度环参数
#define LEFT_ANGLE_KP               0.35                //Kp

#elif FOC_MODE == VECOCITY_LOOP_MODE
// 速度环参数
#define LEFT_VELOCITY_KP            1.00                //Kp
#define LEFT_VELOCITY_KI            0.1                 //Ki

#elif FOC_MODE == ANGLE_VECOCITY_LOOP_MODE
// 速度环参数
#define LEFT_VELOCITY_KP            1.2                 //Kp
#define LEFT_VELOCITY_KI            0.003               //Ki
// 角度环参数
#define LEFT_ANGLE_KP               0.6                 //Kp
#elif FOC_MODE == I_D_LOOP_MODE // 先调D轴参数，响应正常之后调Q轴参数和D轴一样
// 电流环D轴参数
#define LEFT_I_D_KP                 0.002               //Kp
#define LEFT_I_D_KI                 0.004               //Ki
#elif FOC_MODE == I_Q_LOOP_MODE
// 电流环Q轴参数
#define LEFT_I_Q_KP                 0.002               //Kp
#define LEFT_I_Q_KI                 0.004               //Ki
// 电流环D轴参数
#define LEFT_I_D_KP                 0.002               //Kp
#define LEFT_I_D_KI                 0.004               //Ki

#elif FOC_MODE == VECOCITY_CURRENT_LOOP_MODE
// 速度环参数
#define LEFT_VELOCITY_KP            2.0                 //Kp
#define LEFT_VELOCITY_KI            0.08                //Ki
// 电流环Q轴参数
#define LEFT_I_Q_KP                 0.002               //Kp
#define LEFT_I_Q_KI                 0.004               //Ki
// 电流环D轴参数
#define LEFT_I_D_KP                 0.002               //Kp
#define LEFT_I_D_KI                 0.004               //Ki

#elif FOC_MODE == ANGLE_CURRENT_LOOP_MODE

// 角度环参数：Kp 单位为 mA/编码器计数，Kd 单位为 mA/RPM
#define LEFT_ANGLE_KP               1.2                 // 比例系数
#define LEFT_ANGLE_KD               0.6                 // 速度阻尼系数
// 电流环Q轴参数
#define LEFT_I_Q_KP                 0.002               //Kp
#define LEFT_I_Q_KI                 0.004               //Ki
// 电流环D轴参数
#define LEFT_I_D_KP                 0.002               //Kp
#define LEFT_I_D_KI                 0.004               //Ki

#elif FOC_MODE == ANGLE_VECOCITY_CURRENT_LOOP_MODE
// 角度环参数
#define LEFT_ANGLE_KP               0.1                 //Kp
// 速度环参数
#define LEFT_VELOCITY_KP            2.0                 //Kp
#define LEFT_VELOCITY_KI            0.08                //Ki
// 电流环Q轴参数
#define LEFT_I_Q_KP                 0.002               //Kp
#define LEFT_I_Q_KI                 0.004               //Ki
// 电流环D轴参数
#define LEFT_I_D_KP                 0.002               //Kp
#define LEFT_I_D_KI                 0.004               //Ki

#endif


/**************************** PID参数 *************************/

int32_t pi_controller(pid_struct * pid_p,int32_t error)
{
    // 乘法前扩展为 64 位，避免溢出影响饱和判断。
    int64_t proportional = ((int64_t)pid_p->p_t * error) >> 16;
    int64_t integral_limit = (int64_t)pid_p->limit * 131072;
    int64_t previous_integral = pid_p->integral_acc;
    previous_integral = _constrain(previous_integral, -integral_limit, integral_limit);

    // 使用 Tustin 法积分，并保留 17 位小数。
    int64_t integral_delta =
        (int64_t)pid_p->i_t * ((int64_t)error + pid_p->error_prev);
    int64_t integral_acc = previous_integral + integral_delta;
    integral_acc = _constrain(integral_acc, -integral_limit, integral_limit);

    // 仅拒绝会使候选输出进一步进入饱和的积分增量；
    // 反方向的积分增量仍可用于退出饱和。
    int64_t candidate_output = proportional + integral_acc / 131072;
    if ((candidate_output > pid_p->limit && integral_delta > 0)
        || (candidate_output < -(int64_t)pid_p->limit && integral_delta < 0))
    {
        integral_acc = previous_integral;
    }

    int64_t output = proportional + integral_acc / 131072;
    output = _constrain(output, -(int64_t)pid_p->limit, (int64_t)pid_p->limit);

    pid_p->integral_acc = integral_acc;
    // 即使本次积分被阻止，也要更新 Tustin 法使用的历史误差。
    pid_p->error_prev = error;
    return (int32_t)output;
}
int32_t p_controller(pid_struct * pid_p,int32_t error)
{
    // P环
    int32_t proportional = pid_p->p_t * error>>16;

    int32_t output = proportional;
    output = _constrain(output, -pid_p->limit, pid_p->limit);
	
    return output;
}

int32_t pd_controller(pid_struct * pid_p, int32_t error, int32_t feedback_rate)
{
    // 对测量速度作微分反馈，目标角度突变时不会产生微分冲击。
    int64_t proportional = ((int64_t)pid_p->p_t * error) >> 16;
    int64_t damping = ((int64_t)pid_p->d_t * feedback_rate) >> 16;
    int64_t output = proportional - damping;
    output = _constrain(output, -(int64_t)pid_p->limit, (int64_t)pid_p->limit);
    return (int32_t)output;
}
// 计算变换后的参数
void pid_parama_transform(motor_struct * motor_p)
{
    motor_p->angle_pid_struct.p_t     = (int32_t)(motor_p->angle_pid_struct.p*65536);
    motor_p->angle_pid_struct.d_t     = (int32_t)(motor_p->angle_pid_struct.d*65536);
    motor_p->vecocity_pid_struct.p_t  = (int32_t)(motor_p->vecocity_pid_struct.p*65536);
    motor_p->vecocity_pid_struct.i_t  = (int32_t)(motor_p->vecocity_pid_struct.i*65536);
    motor_p->i_q_pid_struct.p_t       = (int32_t)(motor_p->i_q_pid_struct.p*65536);
    motor_p->i_q_pid_struct.i_t       = (int32_t)(motor_p->i_q_pid_struct.i*65536);
    motor_p->i_d_pid_struct.p_t       = (int32_t)(motor_p->i_d_pid_struct.p*65536);
    motor_p->i_d_pid_struct.i_t       = (int32_t)(motor_p->i_d_pid_struct.i*65536);

    if(FOC_MODE == ANGLE_LOOP_MODE)
    {
        motor_p->angle_pid_struct.limit = MAIN_DUTY_LIMIT;      // 位置闭环时，输出占空比限幅
    }
    else if(FOC_MODE == VECOCITY_LOOP_MODE)
    {
        motor_p->vecocity_pid_struct.limit = MAIN_DUTY_LIMIT;   // 速度闭环时，输出占空比限幅
    }
    else if(FOC_MODE == ANGLE_VECOCITY_LOOP_MODE)
    {
        motor_p->vecocity_pid_struct.limit = MAIN_DUTY_LIMIT;
        motor_p->angle_pid_struct.limit = VECOCITY_LIMIT;       // 角度外环输出速度限幅
    }
    else if(FOC_MODE == I_Q_LOOP_MODE && CURRENT_SENSOR==DRIVER_ENABLE)
    {
        motor_p->i_q_pid_struct.limit   = MAIN_DUTY_LIMIT;
        motor_p->i_d_pid_struct.limit   = MAIN_DUTY_LIMIT;      // 电流内环输出占空比限幅
    }
    else if(FOC_MODE == I_D_LOOP_MODE && CURRENT_SENSOR==DRIVER_ENABLE)
    {
        motor_p->i_d_pid_struct.limit   = MAIN_DUTY_LIMIT;      // 电流内环输出占空比限幅
    }
    else if(FOC_MODE == ANGLE_CURRENT_LOOP_MODE && CURRENT_SENSOR==DRIVER_ENABLE)
    {
        motor_p->angle_pid_struct.limit = I_Q_LIMIT;            // 角度内环输出电流限幅
        motor_p->i_q_pid_struct.limit   = MAIN_DUTY_LIMIT;
        motor_p->i_d_pid_struct.limit   = MAIN_DUTY_LIMIT;      // 电流内环输出占空比限幅
    }
    else if(FOC_MODE == VECOCITY_CURRENT_LOOP_MODE && CURRENT_SENSOR==DRIVER_ENABLE)
    {
        motor_p->vecocity_pid_struct.limit = I_Q_LIMIT;         // 速度外环输出电流限幅
        motor_p->i_q_pid_struct.limit      = MAIN_DUTY_LIMIT;
        motor_p->i_d_pid_struct.limit      = MAIN_DUTY_LIMIT;   // 电流内环输出占空比限幅
    }
    else if(FOC_MODE == ANGLE_VECOCITY_CURRENT_LOOP_MODE && CURRENT_SENSOR==DRIVER_ENABLE)
    {
        motor_p->vecocity_pid_struct.limit = I_Q_LIMIT;         // 速度中环输出电流限幅
        motor_p->angle_pid_struct.limit    = VECOCITY_LIMIT;    // 角度外环输出速度限幅
        motor_p->i_q_pid_struct.limit      = MAIN_DUTY_LIMIT;
        motor_p->i_d_pid_struct.limit      = MAIN_DUTY_LIMIT;   // 电流内环输出占空比限幅
    }


}

// pid参数初始化,如果没有对应参数就直接给0
void pid_parama_init()
{

    // 左电机参数配置

    // 角度环参数
	left_motor.angle_pid_struct.p 		=
#ifdef  LEFT_ANGLE_KP
	        LEFT_ANGLE_KP;

#else
	        0;
#endif

    left_motor.angle_pid_struct.d       =
#ifdef LEFT_ANGLE_KD
            LEFT_ANGLE_KD;
#else
            0;
#endif

	// 速度环参数
	left_motor.vecocity_pid_struct.p 	=
#ifdef  LEFT_VELOCITY_KP
            LEFT_VELOCITY_KP;
#else
            0;
#endif


	left_motor.vecocity_pid_struct.i 	=
#ifdef  LEFT_VELOCITY_KI
	        LEFT_VELOCITY_KI;
#else
            0;
#endif

	// i_q环参数
	left_motor.i_q_pid_struct.p         =
#ifdef  LEFT_I_Q_KP
	        LEFT_I_Q_KP;
#else
            0;
#endif

	left_motor.i_q_pid_struct.i         =
#ifdef  LEFT_I_Q_KI
            LEFT_I_Q_KI;
#else
            0;
#endif


	// i_d环参数
	left_motor.i_d_pid_struct.p 		=
#ifdef  LEFT_I_D_KP
            LEFT_I_D_KP;
#else
            0;
#endif

    left_motor.i_d_pid_struct.i         =
#ifdef  LEFT_I_D_KI
            LEFT_I_D_KI;
#else
            0;
#endif

	pid_parama_transform(&left_motor);



	// 右电机参数配置
    // 角度环参数
    right_motor.angle_pid_struct.p       =
#ifdef  RIGHT_ANGLE_KP
            RIGHT_ANGLE_KP;
#else
            left_motor.angle_pid_struct.p;
#endif

    right_motor.angle_pid_struct.d       =
#ifdef RIGHT_ANGLE_KD
            RIGHT_ANGLE_KD;
#else
            left_motor.angle_pid_struct.d;
#endif

    // 速度环参数
    right_motor.vecocity_pid_struct.p    =
#ifdef  RIGHT_VELOCITY_KP
            RIGHT_VELOCITY_KP;
#else
            left_motor.vecocity_pid_struct.p;
#endif


    right_motor.vecocity_pid_struct.i    =
#ifdef  RIGHT_VELOCITY_KI
            RIGHT_VELOCITY_KI;
#else
            left_motor.vecocity_pid_struct.i;
#endif

    // i_q环参数
    right_motor.i_q_pid_struct.p         =
#ifdef  RIGHT_I_Q_KP
            RIGHT_I_Q_KP;
#else
            left_motor.i_q_pid_struct.p;
#endif

    right_motor.i_q_pid_struct.i         =
#ifdef  RIGHT_I_Q_KI
            RIGHT_I_Q_KI;
#else
            left_motor.i_q_pid_struct.i;
#endif


    // i_d环参数
    right_motor.i_d_pid_struct.p         =
#ifdef  RIGHT_I_D_KP
            RIGHT_I_D_KP;
#else
            left_motor.i_d_pid_struct.p;
#endif

    right_motor.i_d_pid_struct.i         =
#ifdef  RIGHT_I_D_KI
            RIGHT_I_D_KI;
#else
            left_motor.i_d_pid_struct.i;
#endif

    pid_parama_transform(&right_motor);
}
