#ifndef __USER_OUT_HAL_H
#define __USER_OUT_HAL_H

#include "superhead.h"


#define OUT1_PORT  GPIOA
#define OUT1_PIN   GPIO_PIN_7


#define OUT2_PORT  GPIOB
#define OUT2_PIN   GPIO_PIN_1

#define OUT1_PERIPH_PORT     RCM_AHB_PERIPH_GPIOA
#define OUT2_PERIPH_PORT     RCM_AHB_PERIPH_GPIOB


#define OUT1 1
#define OUT2 0

#define OUT1_on   HAL_OutPut_Set(OUT1,1)
#define OUT1_off   HAL_OutPut_Set(OUT1,0)

#define OUT2_on   HAL_OutPut_Set(OUT2,1)
#define OUT2_off   HAL_OutPut_Set(OUT2,0)

void Hal_Out_Init(void);

void HAL_OutPut_Set(uint8_t out,uint8_t level);

#endif

