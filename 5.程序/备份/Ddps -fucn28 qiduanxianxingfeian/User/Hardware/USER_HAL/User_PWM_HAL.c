#include "User_Pwm_Hal.h"


void HAL_Pwm_Init(void)
{	
		//RED_init Èýºì  µ¥ºì
   GPIO_Config_T gpioConfig;
	
			RCM_EnableAHBPeriphClock(PWM_LED_ALL_PERIPH_PORT);
	
			gpioConfig.pin = PWM_THREREDLED_PIN|PWM_ONEREDLED_PIN|PWM_DOUBGEDLED_PIN|ONEGEDLED_PIN;
			gpioConfig.mode = GPIO_MODE_OUT;
			gpioConfig.outtype = GPIO_OUT_TYPE_PP;
			gpioConfig.speed = GPIO_SPEED_50MHz;
			gpioConfig.pupd = GPIO_PUPD_NO;
			GPIO_Config(PWM_LED_ALL_PORT, &gpioConfig);
}

void HAL_PWM_SetOutPut(GPIO_T* port,uint16_t gpio_pin,uint8_t level) {
	(level)?GPIO_SetBit(port, gpio_pin): GPIO_ClearBit(port, gpio_pin);  
}
