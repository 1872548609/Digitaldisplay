#ifndef _USER_TMR_HAL_H
#define _USER_TMR_HAL_H

#include "superhead.h"

void HAL_TMR_Init(void);

extern void TMR2Isr(void);	 //在别处重写这个函数

#endif
