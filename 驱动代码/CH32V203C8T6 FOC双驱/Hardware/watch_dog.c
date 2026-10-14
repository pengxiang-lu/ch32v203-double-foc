#include "driver_config.h"

static volatile uint32_t control_progress;
static uint32_t last_fed_progress;
static uint8_t watchdog_started;

void watch_dog_init(uint16_t reload)
{
    // 40 kHz nominal LSI / 256: 6.4 ms per tick; reload is 12 bits.
    if (reload > 0x0FFFU)
        reload = 0x0FFFU;
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    IWDG_SetReload(reload);
    IWDG_ReloadCounter();
    IWDG_Enable();
    last_fed_progress = control_progress;
    watchdog_started = 1;
}

void watch_dog_control_progress(void)
{
    ++control_progress;
}

void feed_dog(void)
{
    uint32_t progress = control_progress;
    if (watchdog_started && progress != last_fed_progress)
    {
        IWDG_ReloadCounter();
        last_fed_progress = progress;
    }
}

void watch_dog_calibration_progress(void)
{
    if (watchdog_started)
    {
        IWDG_ReloadCounter();
        last_fed_progress = control_progress;
    }
}
