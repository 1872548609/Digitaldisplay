/**************************************************************************************************
  Filename:       hal_board_cfg.h
  Revised:        $Date: 
  Revision:       $Revision: 

  Description:    

**************************************************************************************************/

#ifndef HAL_BOARD_CFG_H
#define HAL_BOARD_CFG_H


/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/
#include "hal_mcu.h"
#include "hal_defs.h"
#include "hal_types.h"
#include "board.h"

/*************************************************-*************************************************
*                                               DEFINE
**************************************************-*****************^******************************/
#define HAL_ACTIVE_LOW                                              !
#define HAL_ACTIVE_HIGH                                             !!    /* double negation forces result to be '1' */

/*************************************************-*************************************************
*                                                KEY
**************************************************-*****************^******************************/
#define HAL_KEY_MEGA                                                HAL_KEY_1
#define HAL_KEY_MEGA_INDEX                                          0
#define HAL_KEY1_BIT                                                HAL_KEY_1
#define HAL_KEY1_PORT                                               GPIOA
#define HAL_KEY1_PIN                                                GPIO_PIN_4
#define HAL_KEY1_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY1_PORT, HAL_KEY1_PIN);}
#define HAL_KEY1_READ()                                             (!std_gpio_get_input_pin(HAL_KEY1_PORT, HAL_KEY1_PIN))
    
#define HAL_KEY_MODE                                                HAL_KEY_2
#define HAL_KEY_MODE_INDEX                                          1   //-- 从0开始
#define HAL_KEY2_BIT                                                HAL_KEY_2
#define HAL_KEY2_PORT                                               GPIOA
#define HAL_KEY2_PIN                                                GPIO_PIN_12
#define HAL_KEY2_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY2_PORT, HAL_KEY2_PIN);}
#define HAL_KEY2_READ()                                             (!std_gpio_get_input_pin(HAL_KEY2_PORT, HAL_KEY2_PIN))

#define HAL_KEY_RIGHT_SUB                                           HAL_KEY_3
#define HAL_KEY_RIGHT_SUB_INDEX                                     2
#define HAL_KEY3_BIT                                                HAL_KEY_3
#define HAL_KEY3_PORT                                               GPIOA
#define HAL_KEY3_PIN                                                GPIO_PIN_11
#define HAL_KEY3_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY3_PORT, HAL_KEY3_PIN);}
#define HAL_KEY3_READ()                                             (!std_gpio_get_input_pin(HAL_KEY3_PORT, HAL_KEY3_PIN))

#define HAL_KEY_LEFT_ADD                                            HAL_KEY_4
#define HAL_KEY_LEFT_ADD_INDEX                                      3
#define HAL_KEY4_BIT                                                HAL_KEY_4
#define HAL_KEY4_PORT                                               GPIOA
#define HAL_KEY4_PIN                                                GPIO_PIN_10
#define HAL_KEY4_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY4_PORT, HAL_KEY4_PIN);}
#define HAL_KEY4_READ()                                             (!std_gpio_get_input_pin(HAL_KEY4_PORT, HAL_KEY4_PIN))

#define HAL_KEY_SET                                             	HAL_KEY_5
#define HAL_KEY_SET_INDEX                                         	4
#define HAL_KEY5_BIT                                                HAL_KEY_5
#define HAL_KEY5_PORT                                               GPIOA
#define HAL_KEY5_PIN                                                GPIO_PIN_15
#define HAL_KEY5_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY5_PORT, HAL_KEY5_PIN);}
#define HAL_KEY5_READ()                                             (!std_gpio_get_input_pin(HAL_KEY5_PORT, HAL_KEY5_PIN))
    
#define HAL_KEY_SW6                                                 HAL_KEY_6//-- 拨动开关
#define HAL_KEY6_BIT                                                HAL_KEY_6
#define HAL_KEY6_PORT                                               GPIOC
#define HAL_KEY6_PIN                                                GPIO_PIN_10
#define HAL_KEY6_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY6_PORT, HAL_KEY6_PIN);}
#define HAL_KEY6_READ()                                             (!std_gpio_get_input_pin(HAL_KEY6_PORT, HAL_KEY6_PIN))

//#define IOT_APP_OPTICAL_TEST_IO_PORT                                GPIOB
//#define IOT_APP_OPTICAL_TEST_IO_PIN                                 GPIO_PIN_2
//#define IOT_APP_OPTICAL_TEST_IO_INIT()                              {iot_driver_gpio_init_output(IOT_APP_OPTICAL_TEST_IO_PORT, IOT_APP_OPTICAL_TEST_IO_PIN);}
//#define IOT_APP_OPTICAL_TEST_IO_ON()                                {IOT_APP_OPTICAL_TEST_IO_PORT->BSC = (uint32_t)IOT_APP_OPTICAL_TEST_IO_PIN;}
//#define IOT_APP_OPTICAL_TEST_IO_OFF()                               {IOT_APP_OPTICAL_TEST_IO_PORT->BR = (uint32_t)IOT_APP_OPTICAL_TEST_IO_PIN;}
//#define IOT_APP_OPTICAL_TEST_IO_OUTPUT_STATE()                      (IOT_APP_OPTICAL_TEST_IO_PORT->ODATA & IOT_APP_OPTICAL_TEST_IO_PIN)

#define HAL_NPN_OUT_PERIPH_PORT										RCM_AHB_PERIPH_GPIOF
#define HAL_NPN_OUT_PORT											GPIOA
#define HAL_NPN_OUT_PIN												GPIO_PIN_6
#define HAL_NPN_OUT_INIT()                                          {iot_driver_gpio_init_output(HAL_NPN_OUT_PORT, HAL_NPN_OUT_PIN);}
#define HAL_NPN_OUT_ON()                                            {std_gpio_set_pin(HAL_NPN_OUT_PORT,HAL_NPN_OUT_PIN);}
#define HAL_NPN_OUT_OFF()                                           {std_gpio_reset_pin(HAL_NPN_OUT_PORT,HAL_NPN_OUT_PIN);}

#define IOT_APP_OPTICAL_ATT_PORT                                    GPIOD
#define IOT_APP_OPTICAL_ATT_PIN                                     GPIO_PIN_0
#define IOT_APP_OPTICAL_ATT_INIT()                                  {iot_driver_gpio_init_output(IOT_APP_OPTICAL_ATT_PORT, IOT_APP_OPTICAL_ATT_PIN);}
#define IOT_APP_OPTICAL_ATT_HI()                                    {IOT_APP_OPTICAL_ATT_PORT->BSR = (uint32_t)IOT_APP_OPTICAL_ATT_PIN;}
#define IOT_APP_OPTICAL_ATT_LOW()                                   {IOT_APP_OPTICAL_ATT_PORT->BR = (uint32_t)IOT_APP_OPTICAL_ATT_PIN;}
//#define IOT_APP_OPTICAL_ATT_HI()                                    {GPIO_SetBit(IOT_APP_OPTICAL_ATT_PORT, IOT_APP_OPTICAL_ATT_PIN);}
//#define IOT_APP_OPTICAL_ATT_LOW()                                   {GPIO_ClearBit(IOT_APP_OPTICAL_ATT_PORT, IOT_APP_OPTICAL_ATT_PIN);}

#define IOT_APP_OPTICAL_TRANMIT_PORT                                GPIOA
#define IOT_APP_OPTICAL_TRANMIT_PIN                                 GPIO_PIN_7
#define IOT_APP_OPTICAL_TRANMIT_INIT()                              {iot_driver_gpio_init_output(IOT_APP_OPTICAL_TRANMIT_PORT, IOT_APP_OPTICAL_TRANMIT_PIN);}
#define IOT_APP_OPTICAL_TRANMIT_HI_OFF()                            {IOT_APP_OPTICAL_TRANMIT_PORT->BSR = (uint32_t)IOT_APP_OPTICAL_TRANMIT_PIN;}
#define IOT_APP_OPTICAL_TRANMIT_LOW_ON()                            {IOT_APP_OPTICAL_TRANMIT_PORT->BR = (uint32_t)IOT_APP_OPTICAL_TRANMIT_PIN;}
//#define IOT_APP_OPTICAL_TRANMIT_HI_OFF()                            {GPIO_ClearBit(IOT_APP_OPTICAL_TRANMIT_PORT, IOT_APP_OPTICAL_TRANMIT_PIN);}
//#define IOT_APP_OPTICAL_TRANMIT_LOW_ON()                            {GPIO_SetBit(IOT_APP_OPTICAL_TRANMIT_PORT, IOT_APP_OPTICAL_TRANMIT_PIN);}

#define IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PORT                    GPIOA
#define IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PIN                     GPIO_PIN_6
#define IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_INIT()                  {iot_driver_gpio_init_output(IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PORT, IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PIN);}
#define IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_ON()                    {IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PORT->BSR = (uint32_t)IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PIN;}
#define IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_OFF()                   {IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PORT->BR = (uint32_t)IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PIN;}
//#define IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_ON()                    {GPIO_SetBit(IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PORT, IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PIN);}
//#define IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_OFF()                   {GPIO_ClearBit(IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PORT, IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PIN);}

/*************************************************-*************************************************
*                                                LED
**************************************************-*****************^******************************/
#define HAL_LED_NUM                                                 0

//#define HAL_LED_BLINK_DELAY()                                       st( { volatile uint32 i; for (i=0; i<0x5800; i++) { }; } )

//#define HAL_LED1_BV                                                 BV(0)
//#define HAL_LED1_SBIT                                               
//#define HAL_LED1_DDR                                                
//#define HAL_LED1_PORT                                               GPIOB
//#define HAL_LED1_PIN                                                GPIO_PIN_0
//#define HAL_LED1_POLARITY                                           HAL_ACTIVE_HIGH
//#define HAL_LED1_INIT()                                             st(iot_driver_gpio_init_output(HAL_LED1_PORT, HAL_LED1_PIN);)
//#define HAL_LED1_TURN_OFF()                                         st(std_gpio_set_pin(HAL_LED1_PORT, HAL_LED1_PIN);)
//#define HAL_LED1_TURN_ON()                                          st(std_gpio_reset_pin(HAL_LED1_PORT, HAL_LED1_PIN);)
//#define HAL_LED1_TOGGLE()                                           st(std_gpio_toggle_pin(HAL_LED1_PORT, HAL_LED1_PIN);)
//#define HAL_LED1_STATE()                                            (HAL_LED1_POLARITY(std_gpio_get_input_pin(HAL_LED1_PORT, HAL_LED1_PIN)))


//#define HAL_LED2_BV                                                 BV(1)
//#define HAL_LED2_SBIT                                               
//#define HAL_LED2_DDR                                                
//#define HAL_LED2_PORT                                               GPIOB
//#define HAL_LED2_PIN                                                GPIO_PIN_1
//#define HAL_LED2_POLARITY                                           HAL_ACTIVE_HIGH
//#define HAL_LED2_INIT()                                             st(iot_driver_gpio_init_output(HAL_LED2_PORT, HAL_LED2_PIN);)
//#define HAL_LED2_TURN_OFF()                                         st(std_gpio_set_pin(HAL_LED2_PORT, HAL_LED2_PIN);)
//#define HAL_LED2_TURN_ON()                                          st(std_gpio_reset_pin(HAL_LED2_PORT, HAL_LED2_PIN);)
//#define HAL_LED2_TOGGLE()                                           st(std_gpio_toggle_pin(HAL_LED2_PORT, HAL_LED2_PIN);)
//#define HAL_LED2_STATE()                                            (HAL_LED2_POLARITY(std_gpio_get_input_pin(HAL_LED2_PORT, HAL_LED2_PIN)))


//#define HAL_LED3_BV                                                 BV(1)
//#define HAL_LED3_SBIT                                               
//#define HAL_LED3_DDR                                                
//#define HAL_LED3_PORT                                               GPIOB
//#define HAL_LED3_PIN                                                GPIO_PIN_2
//#define HAL_LED3_POLARITY                                           HAL_ACTIVE_HIGH
//#define HAL_LED3_INIT()                                             st(iot_driver_gpio_init_output(HAL_LED3_PORT, HAL_LED3_PIN);)
//#define HAL_LED3_TURN_OFF()                                         st(std_gpio_set_pin(HAL_LED3_PORT, HAL_LED3_PIN);)
//#define HAL_LED3_TURN_ON()                                          st(std_gpio_reset_pin(HAL_LED3_PORT, HAL_LED3_PIN);)
//#define HAL_LED3_TOGGLE()                                           st(std_gpio_toggle_pin(HAL_LED3_PORT, HAL_LED3_PIN);)
//#define HAL_LED3_STATE()                                            (HAL_LED3_POLARITY(std_gpio_get_input_pin(HAL_LED3_PORT, HAL_LED3_PIN)))


/*************************************************-*************************************************
*                                            NIXIE TUBE
**************************************************-*****************^******************************/
// 段选引脚（a~g）
#define HAL_NIXIETUBE_SEGMENT_A_PORT                                GPIOB
#define HAL_NIXIETUBE_SEGMENT_A_PIN                                 GPIO_PIN_8
#define HAL_NIXIETUBE_SEGMENT_A_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_A_PORT, HAL_NIXIETUBE_SEGMENT_A_PIN);)
#define HAL_NIXIETUBE_SEGMENT_A_ON()                                {HAL_NIXIETUBE_SEGMENT_A_PORT->BR = HAL_NIXIETUBE_SEGMENT_A_PIN;}
#define HAL_NIXIETUBE_SEGMENT_A_OFF()                               {HAL_NIXIETUBE_SEGMENT_A_PORT->BSR = HAL_NIXIETUBE_SEGMENT_A_PIN;}

#define HAL_NIXIETUBE_SEGMENT_B_PORT                                GPIOB
#define HAL_NIXIETUBE_SEGMENT_B_PIN                                 GPIO_PIN_9
#define HAL_NIXIETUBE_SEGMENT_B_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_B_PORT, HAL_NIXIETUBE_SEGMENT_B_PIN);)
#define HAL_NIXIETUBE_SEGMENT_B_ON()                                {HAL_NIXIETUBE_SEGMENT_B_PORT->BR = HAL_NIXIETUBE_SEGMENT_B_PIN;}
#define HAL_NIXIETUBE_SEGMENT_B_OFF()                               {HAL_NIXIETUBE_SEGMENT_B_PORT->BSR = HAL_NIXIETUBE_SEGMENT_B_PIN;}

#define HAL_NIXIETUBE_SEGMENT_C_PORT                                GPIOB
#define HAL_NIXIETUBE_SEGMENT_C_PIN                                 GPIO_PIN_10
#define HAL_NIXIETUBE_SEGMENT_C_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_C_PORT, HAL_NIXIETUBE_SEGMENT_C_PIN);)
#define HAL_NIXIETUBE_SEGMENT_C_ON()                                {HAL_NIXIETUBE_SEGMENT_C_PORT->BR = HAL_NIXIETUBE_SEGMENT_C_PIN;}
#define HAL_NIXIETUBE_SEGMENT_C_OFF()                               {HAL_NIXIETUBE_SEGMENT_C_PORT->BSR = HAL_NIXIETUBE_SEGMENT_C_PIN;}

#define HAL_NIXIETUBE_SEGMENT_D_PORT                                GPIOB
#define HAL_NIXIETUBE_SEGMENT_D_PIN                                 GPIO_PIN_11
#define HAL_NIXIETUBE_SEGMENT_D_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_D_PORT, HAL_NIXIETUBE_SEGMENT_D_PIN);)
#define HAL_NIXIETUBE_SEGMENT_D_ON()                                {HAL_NIXIETUBE_SEGMENT_D_PORT->BR = HAL_NIXIETUBE_SEGMENT_D_PIN;}
#define HAL_NIXIETUBE_SEGMENT_D_OFF()                               {HAL_NIXIETUBE_SEGMENT_D_PORT->BSR = HAL_NIXIETUBE_SEGMENT_D_PIN;}

#define HAL_NIXIETUBE_SEGMENT_E_PORT                                GPIOB
#define HAL_NIXIETUBE_SEGMENT_E_PIN                                 GPIO_PIN_12
#define HAL_NIXIETUBE_SEGMENT_E_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_E_PORT, HAL_NIXIETUBE_SEGMENT_E_PIN);)
#define HAL_NIXIETUBE_SEGMENT_E_ON()                                {HAL_NIXIETUBE_SEGMENT_E_PORT->BR = HAL_NIXIETUBE_SEGMENT_E_PIN;}
#define HAL_NIXIETUBE_SEGMENT_E_OFF()                               {HAL_NIXIETUBE_SEGMENT_E_PORT->BSR = HAL_NIXIETUBE_SEGMENT_E_PIN;}

#define HAL_NIXIETUBE_SEGMENT_F_PORT                                GPIOB
#define HAL_NIXIETUBE_SEGMENT_F_PIN                                 GPIO_PIN_13
#define HAL_NIXIETUBE_SEGMENT_F_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_F_PORT, HAL_NIXIETUBE_SEGMENT_F_PIN);)
#define HAL_NIXIETUBE_SEGMENT_F_ON()                                {HAL_NIXIETUBE_SEGMENT_F_PORT->BR = HAL_NIXIETUBE_SEGMENT_F_PIN;}
#define HAL_NIXIETUBE_SEGMENT_F_OFF()                               {HAL_NIXIETUBE_SEGMENT_F_PORT->BSR = HAL_NIXIETUBE_SEGMENT_F_PIN;}

#define HAL_NIXIETUBE_SEGMENT_G_PORT                                GPIOB
#define HAL_NIXIETUBE_SEGMENT_G_PIN                                 GPIO_PIN_14
#define HAL_NIXIETUBE_SEGMENT_G_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_G_PORT, HAL_NIXIETUBE_SEGMENT_G_PIN);)
#define HAL_NIXIETUBE_SEGMENT_G_ON()                                {HAL_NIXIETUBE_SEGMENT_G_PORT->BR = HAL_NIXIETUBE_SEGMENT_G_PIN;}
#define HAL_NIXIETUBE_SEGMENT_G_OFF()                               {HAL_NIXIETUBE_SEGMENT_G_PORT->BSR = HAL_NIXIETUBE_SEGMENT_G_PIN;}

#define HAL_NIXIETUBE_SEGMENT_LED_PORT                              GPIOB
#define HAL_NIXIETUBE_SEGMENT_LED_PIN                               GPIO_PIN_15
#define HAL_NIXIETUBE_SEGMENT_LED_INIT()                            st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_LED_PORT, HAL_NIXIETUBE_SEGMENT_LED_PIN);)
#define HAL_NIXIETUBE_SEGMENT_LED_ON()                              {HAL_NIXIETUBE_SEGMENT_LED_PORT->BR = HAL_NIXIETUBE_SEGMENT_LED_PIN;}
#define HAL_NIXIETUBE_SEGMENT_LED_OFF()                             {HAL_NIXIETUBE_SEGMENT_LED_PORT->BSR = HAL_NIXIETUBE_SEGMENT_LED_PIN;}

#define HAL_NIXIETUBE_SEGMENT_ALL_PIN_OFF()                         {GPIOB->BSR = (uint32_t)(HAL_NIXIETUBE_SEGMENT_A_PIN|HAL_NIXIETUBE_SEGMENT_B_PIN|HAL_NIXIETUBE_SEGMENT_C_PIN|HAL_NIXIETUBE_SEGMENT_D_PIN|HAL_NIXIETUBE_SEGMENT_E_PIN|HAL_NIXIETUBE_SEGMENT_F_PIN|HAL_NIXIETUBE_SEGMENT_G_PIN|HAL_NIXIETUBE_SEGMENT_LED_PIN);}

// 位选引脚（8个数码管）
#define HAL_NIXIETUBE_DIG_1_PORT                                    GPIOB
#define HAL_NIXIETUBE_DIG_1_PIN                                     GPIO_PIN_0
#define HAL_NIXIETUBE_DIG_1_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_1_PORT, HAL_NIXIETUBE_DIG_1_PIN);)
#define HAL_NIXIETUBE_DIG_1_ON()                                    {HAL_NIXIETUBE_DIG_1_PORT->BR = HAL_NIXIETUBE_DIG_1_PIN;}
#define HAL_NIXIETUBE_DIG_1_OFF()                                   {HAL_NIXIETUBE_DIG_1_PORT->BSR = HAL_NIXIETUBE_DIG_1_PIN;}

#define HAL_NIXIETUBE_DIG_2_PORT                                    GPIOB
#define HAL_NIXIETUBE_DIG_2_PIN                                     GPIO_PIN_1
#define HAL_NIXIETUBE_DIG_2_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_2_PORT, HAL_NIXIETUBE_DIG_2_PIN);)
#define HAL_NIXIETUBE_DIG_2_ON()                                    {HAL_NIXIETUBE_DIG_2_PORT->BR = HAL_NIXIETUBE_DIG_2_PIN;}
#define HAL_NIXIETUBE_DIG_2_OFF()                                   {HAL_NIXIETUBE_DIG_2_PORT->BSR = HAL_NIXIETUBE_DIG_2_PIN;}

#define HAL_NIXIETUBE_DIG_3_PORT                                    GPIOB
#define HAL_NIXIETUBE_DIG_3_PIN                                     GPIO_PIN_2
#define HAL_NIXIETUBE_DIG_3_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_3_PORT, HAL_NIXIETUBE_DIG_3_PIN);)
#define HAL_NIXIETUBE_DIG_3_ON()                                    {HAL_NIXIETUBE_DIG_3_PORT->BR = HAL_NIXIETUBE_DIG_3_PIN;}
#define HAL_NIXIETUBE_DIG_3_OFF()                                   {HAL_NIXIETUBE_DIG_3_PORT->BSR = HAL_NIXIETUBE_DIG_3_PIN;}

#define HAL_NIXIETUBE_DIG_4_PORT                                    GPIOB
#define HAL_NIXIETUBE_DIG_4_PIN                                     GPIO_PIN_3
#define HAL_NIXIETUBE_DIG_4_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_4_PORT, HAL_NIXIETUBE_DIG_4_PIN);)
#define HAL_NIXIETUBE_DIG_4_ON()                                    {HAL_NIXIETUBE_DIG_4_PORT->BR = HAL_NIXIETUBE_DIG_4_PIN;}
#define HAL_NIXIETUBE_DIG_4_OFF()                                   {HAL_NIXIETUBE_DIG_4_PORT->BSR = HAL_NIXIETUBE_DIG_4_PIN;}

#define HAL_NIXIETUBE_DIG_5_PORT                                    GPIOB
#define HAL_NIXIETUBE_DIG_5_PIN                                     GPIO_PIN_4
#define HAL_NIXIETUBE_DIG_5_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_5_PORT, HAL_NIXIETUBE_DIG_5_PIN);)
#define HAL_NIXIETUBE_DIG_5_ON()                                    {HAL_NIXIETUBE_DIG_5_PORT->BR = HAL_NIXIETUBE_DIG_5_PIN;}
#define HAL_NIXIETUBE_DIG_5_OFF()                                   {HAL_NIXIETUBE_DIG_5_PORT->BSR = HAL_NIXIETUBE_DIG_5_PIN;}

#define HAL_NIXIETUBE_DIG_6_PORT                                    GPIOB
#define HAL_NIXIETUBE_DIG_6_PIN                                     GPIO_PIN_5    
#define HAL_NIXIETUBE_DIG_6_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_6_PORT, HAL_NIXIETUBE_DIG_6_PIN);)
#define HAL_NIXIETUBE_DIG_6_ON()                                    {HAL_NIXIETUBE_DIG_6_PORT->BR = HAL_NIXIETUBE_DIG_6_PIN;}
#define HAL_NIXIETUBE_DIG_6_OFF()                                   {HAL_NIXIETUBE_DIG_6_PORT->BSR = HAL_NIXIETUBE_DIG_6_PIN;}

#define HAL_NIXIETUBE_DIG_7_PORT                                    GPIOB
#define HAL_NIXIETUBE_DIG_7_PIN                                     GPIO_PIN_6
#define HAL_NIXIETUBE_DIG_7_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_7_PORT, HAL_NIXIETUBE_DIG_7_PIN);)
#define HAL_NIXIETUBE_DIG_7_ON()                                    {HAL_NIXIETUBE_DIG_7_PORT->BR = HAL_NIXIETUBE_DIG_7_PIN;}
#define HAL_NIXIETUBE_DIG_7_OFF()                                   {HAL_NIXIETUBE_DIG_7_PORT->BSR = HAL_NIXIETUBE_DIG_7_PIN;}

#define HAL_NIXIETUBE_DIG_8_PORT                                    GPIOB
#define HAL_NIXIETUBE_DIG_8_PIN                                     GPIO_PIN_7
#define HAL_NIXIETUBE_DIG_8_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_8_PORT, HAL_NIXIETUBE_DIG_8_PIN);)
#define HAL_NIXIETUBE_DIG_8_ON()                                    {HAL_NIXIETUBE_DIG_8_PORT->BR = HAL_NIXIETUBE_DIG_8_PIN;}
#define HAL_NIXIETUBE_DIG_8_OFF()                                   {HAL_NIXIETUBE_DIG_8_PORT->BSR = HAL_NIXIETUBE_DIG_8_PIN;}

#define HAL_NIXIETUBE_DIG_ALL_PIN_OFF()                             {GPIOB->BSR = (uint32_t)(HAL_NIXIETUBE_DIG_1_PIN | HAL_NIXIETUBE_DIG_2_PIN | HAL_NIXIETUBE_DIG_3_PIN | HAL_NIXIETUBE_DIG_4_PIN | HAL_NIXIETUBE_DIG_5_PIN | HAL_NIXIETUBE_DIG_6_PIN | HAL_NIXIETUBE_DIG_7_PIN | HAL_NIXIETUBE_DIG_8_PIN);}

/*************************************************-*************************************************
*                                     Push Button Configuration
**************************************************-*****************^******************************/
#define HAL_BOARD_INIT()                                            \
{                                                                   \
    ASM_NOP;                                                        /* 插入空操作指令，芯片bug workaround（硬件缺陷修复） */ \
}

/*************************************************-*************************************************
*                                              Debounce
**************************************************-*****************^******************************/
#define HAL_DEBOUNCE(expr)                                          { int i; for (i=0; i<500; i++) { if (!(expr)) i = 0; } }

/*************************************************-*************************************************
*                                              Push Buttons
**************************************************-*****************^******************************/
//#define HAL_PUSH_BUTTON1()                                          (PUSH1_POLARITY (PUSH1_SBIT))
//#define HAL_PUSH_BUTTON2()                                          (PUSH2_POLARITY (PUSH2_SBIT))
//#define HAL_PUSH_BUTTON3()                                          (0)
//#define HAL_PUSH_BUTTON4()                                          (0)
//#define HAL_PUSH_BUTTON5()                                          (0)
//#define HAL_PUSH_BUTTON6()                                          (0)

/*************************************************-*************************************************
*                             OSAL NV implemented by internal flash pages.
**************************************************-*****************^******************************/
// Flash is partitioned into 8 banks of 32 KB or 16 pages.
#define HAL_FLASH_PAGE_PER_BANK                                     0
// Flash is constructed of 64 pages of 1 KB.
#define HAL_FLASH_PAGE_SIZE                                         512
#define HAL_FLASH_WORD_SIZE                                         4

// CODE banks get mapped into the XDATA range 0x08000000 - 0x0800FFFF
#define HAL_FLASH_PAGE_MAP                                          0x08000000

#define HAL_NV_PAGE_CNT                                             2                                   //-- page count
#define HAL_NV_PAGE_END                                             127                                 //-- page end
#define HAL_NV_PAGE_BEG                                             (HAL_NV_PAGE_END-HAL_NV_PAGE_CNT+1) //-- page begin


#define HAL_FLASH_IEEE_SIZE                                         8
#define HAL_FLASH_IEEE_PAGE                                         (HAL_NV_PAGE_END+1)
#define HAL_FLASH_IEEE_OSET                                         (HAL_FLASH_PAGE_SIZE - HAL_FLASH_LOCK_BITS - HAL_FLASH_IEEE_SIZE)
#define HAL_INFOP_IEEE_OSET                                         0xC

#define HAL_FLASH_DEV_PRIVATE_KEY_OSET                              0x7D2
#define HAL_FLASH_CA_PUBLIC_KEY_OSET                                0x7BC
#define HAL_FLASH_IMPLICIT_CERT_OSET                                0x78C


// Used by DMA macros to shift 1 to create a mask for DMA registers.
#define HAL_NV_DMA_CH                                               0
#define HAL_DMA_CH_RX                                               3
#define HAL_DMA_CH_TX                                               4

#define HAL_NV_DMA_GET_DESC()                                       HAL_DMA_GET_DESC0()
#define HAL_NV_DMA_SET_ADDR(a)                                      HAL_DMA_SET_ADDR_DESC0((a))

/*************************************************-*************************************************
*                                      Driver Configuration
**************************************************-*****************^******************************/
/* Set to TRUE enable H/W TIMER usage, FALSE disable it */
//#ifndef HAL_TIMER
//#define HAL_TIMER FALSE
//#endif

/* Set to TRUE enable ADC usage, FALSE disable it */
//#ifndef HAL_ADC
//#define HAL_ADC FALSE
//#endif

/* Set to TRUE enable DMA usage, FALSE disable it */
//#ifndef HAL_DMA
//#define HAL_DMA FALSE
//#endif

/* Set to TRUE enable Flash access, FALSE disable it */
#ifndef HAL_FLASH
#define HAL_FLASH TRUE
#endif

/* Set to TRUE enable AES usage, FALSE disable it */
//#ifndef HAL_AES
//#define HAL_AES FALSE
//#endif

//#ifndef HAL_AES_DMA
//#define HAL_AES_DMA FALSE
//#endif

/* Set to TRUE enable LCD usage, FALSE disable it */
//#ifndef HAL_LCD
//#define HAL_LCD FALSE
//#endif

/* Set to TRUE enable LED usage, FALSE disable it */
#ifndef HAL_LED
#define HAL_LED TRUE
#endif

#if (!defined HAL_LED_BLINK) && (HAL_LED == TRUE)
#define HAL_LED_BLINK
#endif

/* Set to TRUE enable KEY usage, FALSE disable it */
#ifndef HAL_KEY
#define HAL_KEY TRUE
#endif

#ifndef HAL_NIXIETUBE
#define HAL_NIXIETUBE TRUE
#endif

/* Set to TRUE enable UART usage, FALSE disable it */
//#ifndef HAL_UART
//#if (defined ZAPP_P1) || (defined ZAPP_P2) || (defined ZTOOL_P1) || (defined ZTOOL_P2)
//#define HAL_UART TRUE
//#else
//#define HAL_UART FALSE
//#endif
//#endif

//#if HAL_UART
//#ifndef HAL_UART_DMA
//#if HAL_DMA
//#if (defined ZAPP_P2) || (defined ZTOOL_P2)
//#define HAL_UART_DMA  2
//#else
//#define HAL_UART_DMA  1
//#endif
//#else
//#define HAL_UART_DMA  0
//#endif
//#endif

//#ifndef HAL_UART_ISR
//#if HAL_UART_DMA           // Default preference for DMA over ISR.
//#define HAL_UART_ISR  0
//#elif (defined ZAPP_P2) || (defined ZTOOL_P2)
//#define HAL_UART_ISR  2
//#else
//#define HAL_UART_ISR  1
//#endif
//#endif

//#if (HAL_UART_DMA && (HAL_UART_DMA == HAL_UART_ISR))
//#error HAL_UART_DMA & HAL_UART_ISR must be different.
//#endif

// Used to set P2 priority - USART0 over USART1 if both are defined.
//#if ((HAL_UART_DMA == 1) || (HAL_UART_ISR == 1))
//#define HAL_UART_PRIPO             0x00
//#else
//#define HAL_UART_PRIPO             0x40
//#endif

//#else
//#define HAL_UART_DMA  0
//#define HAL_UART_ISR  0
//#endif

/* USB is not used for CC2530 configuration */
//#define HAL_UART_USB  0
#endif

/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/






