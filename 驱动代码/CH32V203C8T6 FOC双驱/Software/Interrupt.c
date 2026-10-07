#include "ch32v20x.h"
void interrupt_global_disable(void)
{
    __disable_irq();
}
void interrupt_global_enable(void)
{
    __enable_irq();
}

