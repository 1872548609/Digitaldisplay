#include "iot_driver_out.h"
void Hal_Out_Init(void)
{
   GPIO_Config_T gpioConfig;
	
			RCM_EnableAHBPeriphClock(OUT1_PERIPH_PORT);
	 	RCM_EnableAHBPeriphClock(OUT2_PERIPH_PORT);

	
			gpioConfig.pin = OUT1_PIN;
			gpioConfig.mode = GPIO_MODE_OUT;
			gpioConfig.outtype = GPIO_OUT_TYPE_PP;
			gpioConfig.speed = GPIO_SPEED_50MHz;
			gpioConfig.pupd = GPIO_PUPD_NO;
			GPIO_Config(OUT1_PORT, &gpioConfig);
	
	  gpioConfig.pin = OUT2_PIN;
			gpioConfig.mode = GPIO_MODE_OUT;
			gpioConfig.outtype = GPIO_OUT_TYPE_PP;
			gpioConfig.speed = GPIO_SPEED_50MHz;
			gpioConfig.pupd = GPIO_PUPD_NO;
			GPIO_Config(OUT2_PORT, &gpioConfig);	
}

void HAL_OutPut_Set(uint8_t out,uint8_t level)
{
		if(out)
		{
					if(level)
					{
					   GPIO_SetBit(GPIOB, GPIO_PIN_1);
					}
					else
					{
					   GPIO_ClearBit(GPIOB, GPIO_PIN_1);
					}
					 
		}
		else
		{
			  if(level)
					{
					   GPIO_SetBit(GPIOA, GPIO_PIN_7);
					}
					else
					{
					   GPIO_ClearBit(GPIOA, GPIO_PIN_7);
					}
		}
}
