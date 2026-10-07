#ifndef  __MT6701_H
#define  __MT6701_H
#include <stdint.h>
#define GPIO_Port GPIOB
#define RCC_GPIO  RCC_APB2Periph_GPIOB
#define CLK_Pin   GPIO_Pin_13
#define DO_Pin    GPIO_Pin_14
#define L_CSN_Port GPIOB
#define L_CSN_Pin GPIO_Pin_12
#define L_CSN_H	  GPIO_SetBits(GPIO_Port,L_CSN_Pin)
#define L_CSN_L	  GPIO_ResetBits(GPIO_Port,L_CSN_Pin)
#define R_CSN_Port GPIOB
#define R_CSN_Pin GPIO_Pin_11
#define R_CSN_H   GPIO_SetBits(GPIO_Port,R_CSN_Pin)
#define R_CSN_L   GPIO_ResetBits(GPIO_Port,R_CSN_Pin)
void MT6701_Init(void);
uint16_t L_MT6701_GetRawAngle(void);
uint16_t R_MT6701_GetRawAngle(void);
#endif


