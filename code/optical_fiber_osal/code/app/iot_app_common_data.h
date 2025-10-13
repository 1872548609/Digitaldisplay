
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

