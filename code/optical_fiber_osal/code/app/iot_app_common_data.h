
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

extern float Current_pressure_value;	// 当前气压值 kpa
extern float unitchange_pressure_value;	// 单位后的气压值

extern float P1_Value ;
extern float P2_Value ;
extern float Hi1_Value; 
extern float Lo1_Value;		
extern float Hi2_Value;
extern float Lo2_Value; 

// 菜单回调命令
#define MENU_CBK_MODE 3
#define MENU_CBK_ADD  4
#define MENU_CBK_SUB  5

// 菜单选项全局变量
#define		compare1_EASY 		1
#define		compare1_HSY 		2
#define		compare1_WCMP 		3

#define   	compare2_off	 	1 
#define		compare2_EASY 		2
#define		compare2_HSY 		3
#define		compare2_WCMP 		4

#define  	noncstatus_1o2o     0
#define		noncstatus_1o2c     1
#define		noncstatus_1c2o     2
#define		noncstatus_1c2c     3
#define 	out1nc  1
#define     out1no  0
#define 	out1   2
#define 	out2   1

#define 	reaction_2ms 	1
#define 	reaction_10ms 	2
#define 	reaction_50ms 	3
#define 	reaction_100ms 	4
#define 	reaction_250ms 	5
#define 	reaction_500ms 	6
#define 	reaction_2000ms	7

#define  	R_ON 	     1
#define		G_ON 	     2
#define		AL_RED 	     3
#define		AL_GREEN     4

#define    bAr    	1
#define    KgF    	2
#define    KPR    	3
#define    MPR    	4
#define    PSI    	5
#define    MMHG    	6
#define    inHG    	7

#define    Std    1
#define    OFF    2
#define    UnIt    3
#define    No    4
#define    CuSt    5

#define    dispeed_250ms    1
#define    dispeed_500ms    2
#define    dispeed_1000ms    3

#define    diflevel_level1   1
#define    diflevel_level2   2
#define    diflevel_level3   3
#define    diflevel_level4   4
#define    diflevel_level5   5
#define    diflevel_level6   6
#define    diflevel_level7   7
#define    diflevel_level8   8

#define    colorreaction_out1    1
#define    colorreaction_out2    2

#define    eco_off    1
#define    eco_std    2
#define    eco_FULL    3

#define    copy_off    1
#define    copy_on    2
#define    copy_on_l    3
#define    copy_redy    4

#define    facrecoff    1
#define    facrecon    2

extern uint32_t out1compare_status;	
extern uint32_t out2compare_status;	
extern uint32_t outnonc_status;
extern uint32_t maincolor_status;
extern uint32_t unitconver_status; 
extern uint32_t reactime_status;
extern uint32_t secscreen_status;
extern uint32_t dispeed_status;
extern uint32_t diflevel_status;
extern uint32_t coloract_status;
extern uint32_t eco_status;
extern uint32_t copy_status;
extern uint32_t facrecover_status;


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

