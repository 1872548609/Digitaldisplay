#include "iot_driver_led.h"

void HAL_LED_Init(void)
{
	//RED_init 三红  单红
	GPIO_Config_T gpioConfig;

	RCM_EnableAHBPeriphClock(LED_ALL_PERIPH_PORT);

	gpioConfig.pin = THREREDLED_PIN;
	gpioConfig.mode = GPIO_MODE_OUT;
	gpioConfig.outtype = GPIO_OUT_TYPE_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	gpioConfig.pupd = GPIO_PUPD_NO;
	GPIO_Config(THREREDLED_PORT, &gpioConfig);

	gpioConfig.pin = ONEREDLED_PIN;
	gpioConfig.mode = GPIO_MODE_OUT;
	gpioConfig.outtype = GPIO_OUT_TYPE_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	gpioConfig.pupd = GPIO_PUPD_NO;
	GPIO_Config(ONEREDLED_PORT, &gpioConfig);

	//GED_init  两绿  单绿
	gpioConfig.pin = DOUBGEDLED_PIN;
	gpioConfig.mode = GPIO_MODE_OUT;
	gpioConfig.outtype = GPIO_OUT_TYPE_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	gpioConfig.pupd = GPIO_PUPD_NO;
	GPIO_Config(DOUBGEDLED_PORT, &gpioConfig);

	gpioConfig.pin = ONEGEDLED_PIN;
	gpioConfig.mode = GPIO_MODE_OUT;
	gpioConfig.outtype = GPIO_OUT_TYPE_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	gpioConfig.pupd = GPIO_PUPD_NO;
	GPIO_Config(ONEGEDLED_PORT, &gpioConfig);
}

void HAL_LED_On(GPIO_T* port,uint16_t gpio_pin) {
     GPIO_SetBit(port, gpio_pin);
}

void HAL_LED_Off(GPIO_T* port,uint16_t gpio_pin) {
     GPIO_ClearBit(port, gpio_pin);
}

void HAL_LED_Toggle(GPIO_T* port,uint16_t gpio_pin){
    static uint8_t bit;
	bit = ~bit;
	(bit) ?  GPIO_SetBit(port, gpio_pin): GPIO_ClearBit(port, gpio_pin);
}
