#include "driver_config.h"
// 0.1ms计数一次
void watch_dog_init(uint16_t num)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    // 6.4ms计数一次
    // 看门狗时钟为40Khz
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    IWDG_SetReload( num );
    IWDG_ReloadCounter();
    IWDG_Enable();
}
void feed_dog()
{
    IWDG_ReloadCounter();           // 喂狗
}
