#ifndef TIMER4_US_H
#define TIMER4_US_H

#include "stdint.h"

/**
 * @brief 初始化定时器4为1微秒计数模式
 * 配置定时器4工作在1MHz计数频率，即每个计数单位为1微秒
 */
void TIM4_US_Init(void);

/**
 * @brief 启动计时
 * 重置计数器和溢出计数，开始计时
 */
void TIM4_Start(void);

/**
 * @brief 停止计时并返回总微秒数
 * @return 从启动到停止的时间，单位：微秒
 */
uint32_t TIM4_Stop(void);

/**
 * @brief 获取当前计时值(不停止计时)
 * 用于在计时过程中获取中间时间点，同时处理可能的溢出
 * @return 从启动到当前的时间，单位：微秒
 */
uint32_t TIM4_GetCurrentTime(void);

#endif /* TIMER4_US_H */
