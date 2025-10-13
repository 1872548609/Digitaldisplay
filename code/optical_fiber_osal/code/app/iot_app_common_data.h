
#ifndef _IOT_APP_COMMON_DATA_H
#define _IOT_APP_COMMON_DATA_H

#ifdef __cplusplus
extern "C"
{
#endif

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                               MACRO
**************************************************-*****************^******************************/
#define HAL_KEY_MODE                                                HAL_KEY_1
#define HAL_KEY1_BIT                                                HAL_KEY_1
#define HAL_KEY1_PORT                                               GPIOA
#define HAL_KEY1_PIN                                                GPIO_PIN_1
#define HAL_KEY1_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY1_PORT, HAL_KEY1_PIN);}
#define HAL_KEY1_READ()                                             (!GPIO_ReadInputBit(HAL_KEY1_PORT, HAL_KEY1_PIN))
    
#define HAL_KEY_LEFT_ADD                                            HAL_KEY_2
#define HAL_KEY2_BIT                                                HAL_KEY_2
#define HAL_KEY2_PORT                                               GPIOA
#define HAL_KEY2_PIN                                                GPIO_PIN_0
#define HAL_KEY2_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY2_PORT, HAL_KEY2_PIN);}
#define HAL_KEY2_READ()                                             (!GPIO_ReadInputBit(HAL_KEY2_PORT, HAL_KEY2_PIN))

#define HAL_KEY_RIGHT_SUB                                         	HAL_KEY_3
#define HAL_KEY3_BIT                                                HAL_KEY_3
#define HAL_KEY3_PORT                                               GPIOB
#define HAL_KEY3_PIN                                                GPIO_PIN_8
#define HAL_KEY3_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY3_PORT, HAL_KEY3_PIN);}
#define HAL_KEY3_READ()                                             (!GPIO_ReadInputBit(HAL_KEY3_PORT, HAL_KEY3_PIN))

/*************************************************-*************************************************
*                                        LED Configuration
**************************************************-*****************^******************************/
#define ACTIVE_LOW                                                  !
#define ACTIVE_HIGH                                                 !!    /* double negation forces result to be '1' */

#define HAL_NUM_LEDS                                                4

#define HAL_LED_BLINK_DELAY()                                       st( { volatile uint32 i; for (i=0; i<0x5800; i++) { }; } )

#define HAL_ALL_LED 												(HAL_THREED_RED|HAL_ONE_RED|HAL_DOUBLE_GREED|HAL_ONE_GREED)

#define HAL_THREED_RED												HAL_LED_1                                             
#define HAL_LED1_PORT                                               GPIOA
#define HAL_LED1_PIN                                                GPIO_PIN_12
#define HAL_LED1_POLARITY                                           ACTIVE_HIGH

#define HAL_TURN_OFF_LED1()                                        	st(GPIO_ClearBit(HAL_LED1_PORT, HAL_LED1_PIN);)
#define HAL_TURN_ON_LED1()                                          st(GPIO_SetBit(HAL_LED1_PORT, HAL_LED1_PIN);)
#define HAL_TOGGLE_LED1()                                           st(if (HAL_STATE_LED1()){HAL_TURN_OFF_LED1();}else{HAL_TURN_ON_LED1();})
#define HAL_STATE_LED1()                                            (HAL_LED1_POLARITY(GPIO_ReadInputBit(HAL_LED1_PORT, HAL_LED1_PIN)))


#define HAL_ONE_RED													HAL_LED_2
#define HAL_LED2_PORT                                               GPIOA
#define HAL_LED2_PIN                                                GPIO_PIN_4
#define HAL_LED2_POLARITY                                           ACTIVE_HIGH

#define HAL_TURN_OFF_LED2()                                         st(GPIO_ClearBit(HAL_LED2_PORT, HAL_LED2_PIN);)
#define HAL_TURN_ON_LED2()                                          st(GPIO_SetBit(HAL_LED2_PORT, HAL_LED2_PIN);)
#define HAL_TOGGLE_LED2()                                           st(if (HAL_STATE_LED2()){HAL_TURN_OFF_LED2();}else{HAL_TURN_ON_LED2();})
#define HAL_STATE_LED2()                                            (HAL_LED2_POLARITY(GPIO_ReadInputBit(HAL_LED2_PORT, HAL_LED2_PIN)))


#define HAL_DOUBLE_GREED											HAL_LED_3
#define HAL_LED3_PORT                                               GPIOA
#define HAL_LED3_PIN                                                GPIO_PIN_15
#define HAL_LED3_POLARITY                                           ACTIVE_HIGH

#define HAL_TURN_OFF_LED3()                                         st(GPIO_ClearBit(HAL_LED3_PORT, HAL_LED3_PIN);)
#define HAL_TURN_ON_LED3()                                          st(GPIO_SetBit(HAL_LED3_PORT, HAL_LED3_PIN);)
#define HAL_TOGGLE_LED3()                                           st(if (HAL_STATE_LED3()){HAL_TURN_OFF_LED3();}else{HAL_TURN_ON_LED3();})
#define HAL_STATE_LED3()                                            (HAL_LED3_POLARITY(GPIO_ReadInputBit(HAL_LED3_PORT, HAL_LED3_PIN)))


#define HAL_ONE_GREED												HAL_LED_4                                             
#define HAL_LED4_PORT                                               GPIOA
#define HAL_LED4_PIN                                                GPIO_PIN_5
#define HAL_LED4_POLARITY                                           ACTIVE_HIGH

#define HAL_TURN_OFF_LED4()                                         st(GPIO_ClearBit(HAL_LED4_PORT, HAL_LED4_PIN);)
#define HAL_TURN_ON_LED4()                                          st(GPIO_SetBit(HAL_LED4_PORT, HAL_LED4_PIN);)
#define HAL_TOGGLE_LED4()                                           st(if (HAL_STATE_LED4()){HAL_TURN_OFF_LED4();}else{HAL_TURN_ON_LED4();})
#define HAL_STATE_LED4()                                            (HAL_LED3_POLARITY(GPIO_ReadInputBit(HAL_LED4_PORT, HAL_LED4_PIN)))

/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/



/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
#define HAL_KEY_EVENT_TIMEOUT                                       50

/*************************************************-*************************************************
*                                          GLOBAL VARIABLES
**************************************************-*****************^******************************/
#define RUN_STATE  0	// 运行状态
#define MENU_STATE 1	// 菜单状态

extern uint8_t system_state;	// 系统运行状态全局变量

#define MENU_CBK_MODE 3
#define MENU_CBK_ADD  4
#define MENU_CBK_SUB  5

/*************************************************-*************************************************
*                                             FUNCTION
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif

