#ifndef __SERIAL_H_
#define __SERIAL_H_
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
void Serial_Init(void);
void TIM3_Init(void);
void Serial_SendByte(uint8_t Byte);
int fputc(int ch, FILE *f);
#endif

