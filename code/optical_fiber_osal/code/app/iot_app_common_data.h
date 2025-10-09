
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
#define HAL_KEY_MEGA                                                HAL_KEY_1
#define HAL_KEY1_BIT                                                HAL_KEY_1
#define HAL_KEY1_PORT                                               GPIOA
#define HAL_KEY1_PIN                                                GPIO_PIN_3
#define HAL_KEY1_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY1_PORT, HAL_KEY1_PIN);}
#define HAL_KEY1_READ()                                             (!GPIO_ReadInputBit(HAL_KEY1_PORT, HAL_KEY1_PIN))
    
#define HAL_KEY_MODE                                                HAL_KEY_2
#define HAL_KEY2_BIT                                                HAL_KEY_2
#define HAL_KEY2_PORT                                               GPIOA
#define HAL_KEY2_PIN                                                GPIO_PIN_2
#define HAL_KEY2_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY2_PORT, HAL_KEY2_PIN);}
#define HAL_KEY2_READ()                                             (!GPIO_ReadInputBit(HAL_KEY2_PORT, HAL_KEY2_PIN))

#define HAL_KEY_RIGHT_SUB                                           HAL_KEY_3
#define HAL_KEY3_BIT                                                HAL_KEY_3
#define HAL_KEY3_PORT                                               GPIOA
#define HAL_KEY3_PIN                                                GPIO_PIN_1
#define HAL_KEY3_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY3_PORT, HAL_KEY3_PIN);}
#define HAL_KEY3_READ()                                             (!GPIO_ReadInputBit(HAL_KEY3_PORT, HAL_KEY3_PIN))

#define HAL_KEY_LEFT_ADD                                            HAL_KEY_4
#define HAL_KEY4_BIT                                                HAL_KEY_4
#define HAL_KEY4_PORT                                               GPIOA
#define HAL_KEY4_PIN                                                GPIO_PIN_0
#define HAL_KEY4_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY4_PORT, HAL_KEY4_PIN);}
#define HAL_KEY4_READ()                                             (!GPIO_ReadInputBit(HAL_KEY4_PORT, HAL_KEY4_PIN))

#define HAL_KEY_SET                                             	HAL_KEY_5
#define HAL_KEY5_BIT                                                HAL_KEY_5
#define HAL_KEY5_PORT                                               GPIOB
#define HAL_KEY5_PIN                                                GPIO_PIN_2
#define HAL_KEY5_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY5_PORT, HAL_KEY5_PIN);}
#define HAL_KEY5_READ()                                             (!GPIO_ReadInputBit(HAL_KEY5_PORT, HAL_KEY5_PIN))
    
#define HAL_KEY_SW6                                                 HAL_KEY_6//-- 拨动开关
#define HAL_KEY6_BIT                                                HAL_KEY_6
#define HAL_KEY6_PORT                                               GPIOA
#define HAL_KEY6_PIN                                                GPIO_PIN_4
#define HAL_KEY6_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY6_PORT, HAL_KEY6_PIN);}
#define HAL_KEY6_READ()                                             (!GPIO_ReadInputBit(HAL_KEY6_PORT, HAL_KEY6_PIN))
			
#define HAL_NPN_OUT_PERIPH_PORT										RCM_AHB_PERIPH_GPIOF
#define HAL_NPN_OUT_PORT											GPIOA
#define HAL_NPN_OUT_PIN												GPIO_PIN_6
#define HAL_NPN_OUT_ON()                                            GPIO_SetBit(HAL_NPN_OUT_PORT,HAL_NPN_OUT_PIN)
#define HAL_NPN_OUT_OFF()                                           GPIO_ClearBit(HAL_NPN_OUT_PORT,HAL_NPN_OUT_PIN)

/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
#define IOT_APP_OPTICAL_MODE_MEGA                                   0
#define IOT_APP_OPTICAL_MODE_SUPER                                  1
#define IOT_APP_OPTICAL_MODE_FINE                                   2
#define IOT_APP_OPTICAL_MODE_HIGH_SPEED                             3
#define IOT_APP_OPTICAL_MODE_MAX                                    4
	
#define IOT_APP_POWERSELECT_DEFULTMODE	IOT_APP_POWERSELECT_FINEMODE			// 默认功率模式		
#define IOT_APP_POWERSELECT_FINEMODE								0x1			// FINE模式
#define IOT_APP_POWERSELECT_HSPMODE									0x2			// HSP模式
#define IOT_APP_POWERSELECT_MEGAMODE								0x4			// MEGA模式
#define IOT_APP_POWERSELECT_SUPRMODE								0x8			// SUPR模式

/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
#define HAL_KEY_EVENT_TIMEOUT                                       50

/*************************************************-*************************************************
*                                          GLOBAL VARIABLES
**************************************************-*****************^******************************/
extern uint8   iot_app_optical_current_mode;
extern int8    iot_app_optical_current_att;
extern int8    iot_app_optical_auto_adjust_att;
extern uint16  iot_app_optical_current_adc_value;
extern uint16  iot_app_optical_delta_adc_value;
extern uint32  iot_app_optical_delta_voltage_mv;
extern uint32  iot_app_optical_current_lux;
extern uint16  iot_app_vdd_current_adc_value;
extern uint16_t thresholdValue;	// adc阈值
extern uint8_t adc_update_en ;// 是否显示阈值
extern uint8_t threshold_update_en;// 是否刷新阈值显示
extern uint8_t outmodeset ; // 输出模式切换，0 入光 1遮光
extern uint32_t iot_app_optical_current_sta;// 0~ 9999




extern uint8_t systemstate ; // 1 输出模式设置 2 设置校准 3 菜单 4 阈值设置
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

