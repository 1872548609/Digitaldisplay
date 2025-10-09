#ifndef __SUPERHEAD_H
#define __SUPERHEAD_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes */
#include "Board.h"		
#include "apm32f0xx_syscfg.h"			
#include "apm32f0xx.h"
#include "apm32f0xx_eint.h"
#include "apm32f0xx_rcm.h"
#include "apm32f0xx_gpio.h"
#include "apm32f0xx_usart.h"
#include "apm32f0xx_misc.h"
#include "apm32f0xx_tmr.h"
#include "apm32f0xx_adc.h"
#include "apm32f0xx_fmc.h"
#include "bsp_delay.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "math.h"
#include "Board.h"	
#include "menu.h"		

#include	"user_led_hal.h"
#include "user_led_div.h"
#include "User_Tmr_Hal.h"
#include "User_Key_Hal.h"
#include "User_Key_Div.h"
#include "User_ADC_Hal.h"
#include "User_ReadValue_DIV.h"
#include "User_Disp_Hal.h"
#include "User_Disp_Div.h"
#include "User_Pwm_Hal.h"
#include "User_Pwm_DIV.h"
#include "User_Flash_Hal.h"
#include "User_Out_Hal.h"
#include "User_Usart_HAL.h"
#include "User_Usart_DIV.h"


//APM_EVAL_DelayMs(1000);   ÑÓÊ±º¯Êý
#define 	PB7_PIN   							GPIO_PIN_7
#define 	PB7_GPIO_PORT    GPIOB
#define 	PB7_GPIO_CLK    	RCM_AHB_PERIPH_GPIOB



#ifdef __cplusplus
}
#endif

#endif
