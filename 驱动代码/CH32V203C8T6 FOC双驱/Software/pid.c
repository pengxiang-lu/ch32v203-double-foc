#include "driver_config.h"
/**************************** PID参数 *************************/
#if FOC_MODE == ANGLE_LOOP_MODE
// 角度环参数
#define LEFT_ANGLE_KP               0.35                //Kp

#elif FOC_MODE == VECOCITY_LOOP_MODE
// 速度环参数
#define LEFT_VELOCITY_KP            2.00                //Kp
#define LEFT_VELOCITY_KI            0.12                //Ki

#elif FOC_MODE == ANGLE_VECOCITY_LOOP_MODE
// 速度环参数
#define LEFT_VELOCITY_KP            1.2                 //Kp
#define LEFT_VELOCITY_KI            0.003               //Ki
// 角度环参数
#define LEFT_ANGLE_KP               0.6                 //Kp
#elif FOC_MODE == I_D_LOOP_MODE
// 电流环D轴参数
#define LEFT_I_D_KP                 0.005               //Kp
#define LEFT_I_D_KI                 0.007               //Ki
#elif FOC_MODE == I_Q_LOOP_MODE
// 电流环Q轴参数
#define LEFT_I_Q_KP                 0.005               //Kp
#define LEFT_I_Q_KI                 0.007               //Ki
// 电流环D轴参数
#define LEFT_I_D_KP                 0.005               //Kp
#define LEFT_I_D_KI                 0.007               //Ki

#elif FOC_MODE == VECOCITY_CURRENT_LOOP_MODE
// 速度环参数
#define LEFT_VELOCITY_KP            1.2                   //Kp
#define LEFT_VELOCITY_KI            0.004                //Ki
// 电流环Q轴参数
#define LEFT_I_Q_KP                 0.004               //Kp
#define LEFT_I_Q_KI                 0.008               //Ki
// 电流环D轴参数
#define LEFT_I_D_KP                 0.004               //Kp
#define LEFT_I_D_KI                 0.008               //Ki

#elif FOC_MODE == ANGLE_CURRENT_LOOP_MODE

// 角度环参数
#define LEFT_ANGLE_KP               0.5                 //Kp
// 电流环Q轴参数
#define LEFT_I_Q_KP                 0.005               //Kp
#define LEFT_I_Q_KI                 0.007               //Ki
// 电流环D轴参数
#define LEFT_I_D_KP                 0.005               //Kp
#define LEFT_I_D_KI                 0.007               //Ki

#elif FOC_MODE == ANGLE_VECOCITY_CURRENT_LOOP_MODE
// 角度环参数
#define LEFT_ANGLE_KP               0.5                 //Kp
// 速度环参数
#define LEFT_VELOCITY_KP            3                   //Kp
#define LEFT_VELOCITY_KI            0.04                //Ki
// 电流环Q轴参数
#define LEFT_I_Q_KP                 0.005               //Kp
#define LEFT_I_Q_KI                 0.007               //Ki
// 电流环D轴参数
#define LEFT_I_D_KP                 0.005               //Kp
#define LEFT_I_D_KI                 0.007               //Ki

#endif


/**************************** PID参数 *************************/

int32_t pi_controller(pid_struct * pid_p,int32_t error)
{
    // P环
    int32_t proportional = (pid_p->p_t * error)>>16;
    // Tustin 散点积分（I环）
    int32_t integral = pid_p->integral_prev + (pid_p->i_t*(error + pid_p->error_prev)>>17);
    integral = _constrain(integral, -pid_p->limit, pid_p->limit);

    // 将P,I,D三环的计算值加起来
    int32_t output = proportional + integral;
    output = _constrain(output, -pid_p->limit, pid_p->limit);

    // 保存值（为了下一次循环）
	pid_p->integral_prev = integral;
    pid_p->error_prev = error;
    return output;
}
int32_t p_controller(pid_struct * pid_p,int32_t error)
{
    // P环
    int32_t proportional = pid_p->p_t * error>>16;

    // 将P,I,D三环的计算值加起来
    int32_t output = proportional;
    output = _constrain(output, -pid_p->limit, pid_p->limit);
	
    return output;
}
// 计算变换后的参数
void pid_parama_transform(motor_struct * motor_p)
{
    motor_p->angle_pid_struct.p_t     = (int32_t)(left_motor.angle_pid_struct.p*65536);
    motor_p->vecocity_pid_struct.p_t  = (int32_t)(left_motor.vecocity_pid_struct.p*65536);
    motor_p->vecocity_pid_struct.i_t  = (int32_t)(left_motor.vecocity_pid_struct.i*65536);
    motor_p->i_q_pid_struct.p_t       = (int32_t)(left_motor.i_q_pid_struct.p*65536);
    motor_p->i_q_pid_struct.i_t       = (int32_t)(left_motor.i_q_pid_struct.i*65536);
    motor_p->i_d_pid_struct.p_t       = (int32_t)(left_motor.i_d_pid_struct.p*65536);
    motor_p->i_d_pid_struct.i_t       = (int32_t)(left_motor.i_d_pid_struct.i*65536);

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
            0;
#endif

    // 速度环参数
    right_motor.vecocity_pid_struct.p    =
#ifdef  RIGHT_VELOCITY_KP
            RIGHT_VELOCITY_KP;
#else
            0;
#endif


    right_motor.vecocity_pid_struct.i    =
#ifdef  RIGHT_VELOCITY_KI
            RIGHT_VELOCITY_KI;
#else
            0;
#endif

    // i_q环参数
    right_motor.i_q_pid_struct.p         =
#ifdef  RIGHT_I_Q_KP
            RIGHT_I_Q_KP;
#else
            0;
#endif

    right_motor.i_q_pid_struct.i         =
#ifdef  RIGHT_I_Q_KI
            RIGHT_I_Q_KI;
#else
            0;
#endif


    // i_d环参数
    right_motor.i_d_pid_struct.p         =
#ifdef  RIGHT_I_D_KP
            RIGHT_I_D_KP;
#else
            0;
#endif

    right_motor.i_d_pid_struct.i         =
#ifdef  RIGHT_I_D_KI
            RIGHT_I_D_KI;
#else
            0;
#endif

    pid_parama_transform(&right_motor);
}
