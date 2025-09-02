#ifndef _USER_KEY_HAL_H
#define _USER_KEY_HAL_H

#include "superhead.h"

//按键外设和端口IO
#define KEY1_2_PERIPH_PORT  RCM_AHB_PERIPH_GPIOA
#define KEY3_PERIPH_PORT  RCM_AHB_PERIPH_GPIOB

#define KEY1_PIN    GPIO_PIN_1
#define KEY1_PORT			GPIOA
#define KEY2_PIN    GPIO_PIN_0
#define KEY2_PORT   GPIOA
#define KEY3_PIN    GPIO_PIN_8
#define KEY3_PORT   GPIOB

//读按键cmd
#define READ_KEY1   GPIOA,GPIO_PIN_1
#define READ_KEY2   GPIOA,GPIO_PIN_0
#define READ_KEY3   GPIOB,GPIO_PIN_8

#define Down 0
#define Up 1

void HAL_KEY_Init(void);

//参数可以是 READ_KEY1 等
//返回 电平信息 0 1
uint8_t HAL_KEY_Read(GPIO_T*port,uint16_t pin);

#endif
