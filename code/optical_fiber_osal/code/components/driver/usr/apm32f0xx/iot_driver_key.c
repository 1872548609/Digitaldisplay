#include "iot_driver_key.h"

void HAL_KEY_Init(void)
{
	GPIO_Config_T gpioConfig;
	//EINT_Config_T eintConfig;

	RCM_EnableAHBPeriphClock(KEY1_2_PERIPH_PORT);
	RCM_EnableAHBPeriphClock(KEY3_PERIPH_PORT);
	RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);
	
	
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
	
	//配置中断线
	//SYSCFG_EINTLine(SYSCFG_PORT_GPIOA, SYSCFG_PIN_1);
	//SYSCFG_EINTLine(SYSCFG_PORT_GPIOA, SYSCFG_PIN_0);
	//SYSCFG_EINTLine(SYSCFG_PORT_GPIOB, SYSCFG_PIN_8);
    //
	//配置io线
    //eintConfig.line    =  KEY1_BUTTON_EXTI_LINE;
    //eintConfig.lineCmd =  ENABLE;
    //eintConfig.mode    =  EINT_MODE_INTERRUPT;
    //eintConfig.trigger =  EINT_TRIGGER_ALL;
    //EINT_Config(&eintConfig);
	//			
	//eintConfig.line    =  KEY2_BUTTON_EXTI_LINE;
	//EINT_Config(&eintConfig);
	//			
	//eintConfig.line    =  KEY3_BUTTON_EXTI_LINE;
	//EINT_Config(&eintConfig);
	//			
	//NVIC_EnableIRQRequest(EINT0_1_IRQn, 1);
	//NVIC_EnableIRQRequest(EINT4_15_IRQn, 1);                  
}

//uint8_t HAL_KEY_Read(GPIO_T*port,uint16_t pin)
//{
//	return GPIO_ReadInputBit(port,pin);			
//}


//void EINT4_15_IRQHandler(void)
//{
//    if (EINT_ReadStatusFlag(KEY3_BUTTON_EXTI_LINE) == SET)
//    {			   
//		//KEY3_Isr();
//        EINT_ClearStatusFlag(KEY3_BUTTON_EXTI_LINE);			
//    }

//}

//void EINT0_1_IRQHandler(void)
//{
//    if (EINT_ReadStatusFlag(KEY1_BUTTON_EXTI_LINE) == SET)
//    {			
//		//KEY1_Isr();
//		EINT_ClearStatusFlag(KEY1_BUTTON_EXTI_LINE);
//    }

//    if (EINT_ReadStatusFlag(KEY2_BUTTON_EXTI_LINE) == SET)
//    {
//        //KEY2_Isr();
//		EINT_ClearStatusFlag(KEY2_BUTTON_EXTI_LINE);
//    }
//}
