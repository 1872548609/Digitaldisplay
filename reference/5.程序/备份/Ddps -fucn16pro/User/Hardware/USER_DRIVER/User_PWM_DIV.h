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

//PWM==================================
#define PWM_THREERED     pwm0_management
#define PWM_ONERED       pwm1_management
#define PWM_DOUBGED      pwm2_management
#define PWM_ONEGED       pwm3_management

#define PWM_ID_TR   0
#define PWM_ID_OR   1
#define PWM_ID_DG   2
#define PWM_ID_OG   3



typedef struct
{
		uint32_t Pwm_Tick;
		uint32_t compare;
		uint32_t period;  
		uint8_t 	pwm_id;
}PWM_Management;

extern PWM_Management pwm0_management;
extern PWM_Management pwm1_management;
extern PWM_Management pwm2_management;
extern PWM_Management pwm3_management;


void DIV_periodvalid(void);

void DIV_periodinvalid(void);


void DIV_Pwm_Init(void);
void DIV_Pwn_Set(PWM_Management* management,uint32_t period,float duty,uint8_t pwmid); 
void DIV_Pwn_Control(PWM_Management *pwm_management) ;

#endif
