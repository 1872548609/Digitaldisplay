#ifndef _USER_PWM_HAL_H
#define _USER_PWM_HAL_H

#include "superhead.h"

//¸ü¸ÄLEDÓ²¼þIO
#define PWM_LED_ALL_PERIPH_PORT  RCM_AHB_PERIPH_GPIOA

#define PWM_THREREDLED_PIN    			GPIO_PIN_12
#define PWM_THREREDLED_PORT   			GPIOA

#define PWM_ONEREDLED_PIN        GPIO_PIN_4
#define PWM_ONEREDLED_PORT 	     GPIOA
                             
#define PWM_DOUBGEDLED_PIN	      GPIO_PIN_15
#define PWM_DOUBGEDLED_PORT      GPIOA

#define PWM_ONEGEDLED_PIN        GPIO_PIN_5
#define PWM_ONEGEDLED_PORT 	     GPIOA

#define PWM_LED_ALL_PORT 							GPIOA

void HAL_Pwm_Init(void);

void HAL_PWM_SetOutPut(GPIO_T* port,uint16_t gpio_pin,uint8_t level);

#endif
