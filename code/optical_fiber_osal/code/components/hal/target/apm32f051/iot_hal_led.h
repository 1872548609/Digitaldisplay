#ifndef __USER_LED_HAL_H
#define __USER_LED_HAL_H

//需要变量头文件
//需要io平台的初始化文件
#include "main.h"

//更改LED硬件IO
#define LED_ALL_PERIPH_PORT  RCM_AHB_PERIPH_GPIOA

#define THREREDLED_PIN    			GPIO_PIN_12
#define THREREDLED_PORT   			GPIOA

#define ONEREDLED_PIN        GPIO_PIN_4
#define ONEREDLED_PORT 	     GPIOA
                             
#define DOUBGEDLED_PIN	      GPIO_PIN_15
#define DOUBGEDLED_PORT      GPIOA

#define ONEGEDLED_PIN        GPIO_PIN_5
#define ONEGEDLED_PORT 	     GPIOA




void HAL_LED_Init(void);
void HAL_LED_On(GPIO_T* port,uint16_t gpio_pin);
void HAL_LED_Off(GPIO_T* port,uint16_t gpio_pin);
void HAL_LED_Toggle(GPIO_T* port,uint16_t gpio_pin);
					

#endif

