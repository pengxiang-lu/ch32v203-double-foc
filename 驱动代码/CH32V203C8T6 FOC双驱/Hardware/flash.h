#ifndef __FLASH_H__
#define __FLASH_H__
#include <stdint.h>
#define MOTOR_FLASH_ADDR 0x0800FF00
void Flash_Erase(uint32_t add);
void Flash_WriteHalfWord(uint32_t add,uint16_t data);
uint16_t FLASH_Read(uint32_t add);
#endif
