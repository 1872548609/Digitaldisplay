#include "User_Key_Hal.h"

void HAL_KEY_Init(void)
{
		 GPIO_Config_T gpioConfig;
	
			RCM_EnableAHBPeriphClock(KEY1_2_PERIPH_PORT);
	  RCM_EnableAHBPeriphClock(KEY3_PERIPH_PORT);
	
			gpioConfig.pin = KEY1_PIN;
			gpioConfig.mode = GPIO_MODE_IN;
			gpioConfig.speed = GPIO_SPEED_50MHz;
			GPIO_Config(KEY1_PORT, &gpioConfig);
	
			gpioConfig.pin = KEY2_PIN;
			gpioConfig.mode = GPIO_MODE_IN;
			gpioConfig.speed = GPIO_SPEED_50MHz;
			GPIO_Config(KEY2_PORT, &gpioConfig);	
	
			gpioConfig.pin = KEY3_PIN;
			gpioConfig.mode = GPIO_MODE_IN;
			gpioConfig.speed = GPIO_SPEED_50MHz;
			GPIO_Config(KEY3_PORT, &gpioConfig);
	
}

uint8_t HAL_KEY_Read(GPIO_T*port,uint16_t pin)
{
				return GPIO_ReadInputBit(port,pin);			
}



