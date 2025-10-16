#ifndef _IOT_DRIVER_READADC_H
#define _IOT_DRIVER_READADC_H

#include "main.h"

#ifndef HAL_ADC
#define HAL_ADC TRUE
#endif

void HAL_ADC_Init(void);
extern void ADC_Isr(void);

#endif
