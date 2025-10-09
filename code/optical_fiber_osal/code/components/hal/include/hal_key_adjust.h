#ifndef __HAL_KEY_ADJUST_H__
#define __HAL_KEY_ADJUST_H__

#include "main.h"

//按键定义
#define HAL_KEY_ADD						HAL_KEY_3
#define HAL_KEY3_BIT					HAL_KEY_3
#define HAL_KEY3_PORT					GPIOA
#define HAL_KEY3_PIN					GPIO_PIN_1
#define HAL_KEY3_INIT()					{iot_driver_gpio_init_input(HAL_KEY3_PORT, HAL_KEY3_PIN);}
#define HAL_KEY3_READ()					(!GPIO_ReadInputBit(HAL_KEY3_PORT, HAL_KEY3_PIN))

#define HAL_KEY_SUBTRACT				HAL_KEY_4
#define HAL_KEY4_BIT					HAL_KEY_4
#define HAL_KEY4_PORT					GPIOA
#define HAL_KEY4_PIN					GPIO_PIN_0
#define HAL_KEY4_INIT()					{iot_driver_gpio_init_input(HAL_KEY4_PORT, HAL_KEY4_PIN);}
#define HAL_KEY4_READ()					(!GPIO_ReadInputBit(HAL_KEY4_PORT, HAL_KEY4_PIN))

//速率档位
#define RATE_LEVEL_1					50				// ±50/s
#define RATE_LEVEL_2					500				// ±500/s
#define RATE_LEVEL_3					1000			// ±1000/s

// 阈值定义
#define KEYLONGPRESS_THRESHOLD_6		6000			//6s阈值
#define KEYLONGPRESS_THRESHOLD_14		14000			//14s阈值
// 2档的时间若为6-15s无法实现在20s内实现从0-9999的变化
//#define KEYLONGPRESS_THRESHOLD_15		15000			//15s阈值
#define KEYLONGPRESS_THRESHOLD_20		20000			//20s阈值

#define VALUE_MIN						0				//最小数值
#define VALUE_MAX						9999			//最大数值

#define KEY_ADJUST_EVENT				0x0009
#define KEY_CHECKTIME					100

void key_adjust_init(uint8 taskId);

void key_adjust_setvalue(uint16 value);

uint16 key_adjust_getvalue(void);

uint16 key_adjust_process_event(uint8 taskId,uint16 events);

#endif

