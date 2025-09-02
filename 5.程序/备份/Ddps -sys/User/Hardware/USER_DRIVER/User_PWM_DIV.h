#ifndef _USER_PWM_DIV_H
#define _USER_PWM_DIV_H

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

typedef struct
{
		uint32_t Pwm_Tick;
		uint32_t compare;
		uint32_t period;
}PWM_Management;

extern PWM_Management pwn_management;


void DIV_periodvalid(void);

void DIV_periodinvalid(void);

void DIV_Pwm_Init(void);
void DIV_Pwn_Set(uint32_t period,float duty);
void DIV_Pwn_Control(void);

#endif
