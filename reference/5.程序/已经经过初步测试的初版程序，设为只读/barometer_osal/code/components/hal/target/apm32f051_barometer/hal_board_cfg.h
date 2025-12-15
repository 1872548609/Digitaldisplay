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
#define HAL_KEY_MODE                                                HAL_KEY_1
#define HAL_KEY_MODE_INDEX                                          0
#define HAL_KEY1_BIT                                                HAL_KEY_1
#define HAL_KEY1_PORT                                               GPIOA
#define HAL_KEY1_PIN                                                GPIO_PIN_1
#define HAL_KEY1_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY1_PORT, HAL_KEY1_PIN);}
#define HAL_KEY1_READ()                                             (!GPIO_ReadInputBit(HAL_KEY1_PORT, HAL_KEY1_PIN))
    
#define HAL_KEY_LEFT_UP                                             HAL_KEY_2
#define HAL_KEY_LEFT_UP_INDEX                                       1   //-- 从0开始
#define HAL_KEY2_BIT                                                HAL_KEY_2
#define HAL_KEY2_PORT                                               GPIOA
#define HAL_KEY2_PIN                                                GPIO_PIN_0
#define HAL_KEY2_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY2_PORT, HAL_KEY2_PIN);}
#define HAL_KEY2_READ()                                             (!GPIO_ReadInputBit(HAL_KEY2_PORT, HAL_KEY2_PIN))

#define HAL_KEY_RIGHT_DOWN                                          HAL_KEY_3
#define HAL_KEY_RIGHT_DOWN_INDEX                                    2
#define HAL_KEY3_BIT                                                HAL_KEY_3
#define HAL_KEY3_PORT                                               GPIOB
#define HAL_KEY3_PIN                                                GPIO_PIN_8
#define HAL_KEY3_INIT()                                             {iot_driver_gpio_init_input(HAL_KEY3_PORT, HAL_KEY3_PIN);}
#define HAL_KEY3_READ()                                             (!GPIO_ReadInputBit(HAL_KEY3_PORT, HAL_KEY3_PIN))

/*************************************************-*************************************************
*                                                LED
**************************************************-*****************^******************************/
#define HAL_LED_NUM                                                 4

#define HAL_LED_BLINK_DELAY()                                       st( { volatile uint32 i; for (i=0; i<0x5800; i++) { }; } )

#define HAL_THREE_RED_LED                                           HAL_LED1_BIT
#define HAL_LED1_BIT                                                0x01
#define HAL_LED1_PORT                                               GPIOA
#define HAL_LED1_PIN                                                GPIO_PIN_12
#define HAL_LED1_POLARITY                                           HAL_ACTIVE_HIGH
#define HAL_LED1_INIT()                                             st(iot_driver_gpio_init_output(HAL_LED1_PORT, HAL_LED1_PIN);)
#define HAL_LED1_TURN_OFF()                                         st(GPIO_ClearBit(HAL_LED1_PORT, HAL_LED1_PIN);)
#define HAL_LED1_TURN_ON()                                          st(GPIO_SetBit(HAL_LED1_PORT, HAL_LED1_PIN);)
#define HAL_LED1_TOGGLE()                                           st(if (HAL_LED1_STATE()){HAL_LED1_TURN_OFF();}else{HAL_LED1_TURN_ON();})
#define HAL_LED1_STATE()                                            (HAL_LED1_POLARITY(GPIO_ReadInputBit(HAL_LED1_PORT, HAL_LED1_PIN)))
#define HAL_LED1_DEFAULT_STATUS()                                   HAL_LED1_TURN_ON()

#define HAL_ONE_RED_LED                                             HAL_LED2_BIT
#define HAL_LED2_BIT                                                0x02
#define HAL_LED2_PORT                                               GPIOA
#define HAL_LED2_PIN                                                GPIO_PIN_4
#define HAL_LED2_POLARITY                                           HAL_ACTIVE_HIGH
#define HAL_LED2_INIT()                                             st(iot_driver_gpio_init_output(HAL_LED2_PORT, HAL_LED2_PIN);)
#define HAL_LED2_TURN_OFF()                                         st(GPIO_ClearBit(HAL_LED2_PORT, HAL_LED2_PIN);)
#define HAL_LED2_TURN_ON()                                          st(GPIO_SetBit(HAL_LED2_PORT, HAL_LED2_PIN);)
#define HAL_LED2_TOGGLE()                                           st(if (HAL_LED2_STATE()){HAL_LED2_TURN_OFF();}else{HAL_LED2_TURN_ON();})
#define HAL_LED2_STATE()                                            (HAL_LED2_POLARITY(GPIO_ReadInputBit(HAL_LED2_PORT, HAL_LED2_PIN)))
#define HAL_LED2_DEFAULT_STATUS()                                   HAL_LED2_TURN_ON()

#define HAL_DOUBLE_GREEN_LED                                        HAL_LED3_BIT
#define HAL_LED3_BIT                                                0x04
#define HAL_LED3_PORT                                               GPIOA
#define HAL_LED3_PIN                                                GPIO_PIN_15
#define HAL_LED3_POLARITY                                           HAL_ACTIVE_HIGH
#define HAL_LED3_INIT()                                             st(iot_driver_gpio_init_output(HAL_LED3_PORT, HAL_LED3_PIN);)
#define HAL_LED3_TURN_OFF()                                         st(GPIO_ClearBit(HAL_LED3_PORT, HAL_LED3_PIN);)
#define HAL_LED3_TURN_ON()                                          st(GPIO_SetBit(HAL_LED3_PORT, HAL_LED3_PIN);)
#define HAL_LED3_TOGGLE()                                           st(if (HAL_LED3_STATE()){HAL_LED3_TURN_OFF();}else{HAL_LED3_TURN_ON();})
#define HAL_LED3_STATE()                                            (HAL_LED3_POLARITY(GPIO_ReadInputBit(HAL_LED3_PORT, HAL_LED3_PIN)))
#define HAL_LED3_DEFAULT_STATUS()                                   HAL_LED3_TURN_ON()

#define HAL_ONE_GREEN_LED                                           HAL_LED4_BIT
#define HAL_LED4_BIT                                                0x08
#define HAL_LED4_PORT                                               GPIOA
#define HAL_LED4_PIN                                                GPIO_PIN_5
#define HAL_LED4_POLARITY                                           HAL_ACTIVE_HIGH
#define HAL_LED4_INIT()                                             st(iot_driver_gpio_init_output(HAL_LED4_PORT, HAL_LED4_PIN);)
#define HAL_LED4_TURN_OFF()                                         st(GPIO_ClearBit(HAL_LED4_PORT, HAL_LED4_PIN);)
#define HAL_LED4_TURN_ON()                                          st(GPIO_SetBit(HAL_LED4_PORT, HAL_LED4_PIN);)
#define HAL_LED4_TOGGLE()                                           st(if (HAL_LED4_STATE()){HAL_LED4_TURN_OFF();}else{HAL_LED4_TURN_ON();})
#define HAL_LED4_STATE()                                            (HAL_LED4_POLARITY(GPIO_ReadInputBit(HAL_LED4_PORT, HAL_LED4_PIN)))
#define HAL_LED4_DEFAULT_STATUS()                                   HAL_LED4_TURN_ON()

/*************************************************-*************************************************
*                                                
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                               OUTPUT
**************************************************-*****************^******************************/
#define HAL_OUT1_PORT											    GPIOA
#define HAL_OUT1_PIN											    GPIO_PIN_7
#define HAL_OUT1_INIT()                                             {iot_driver_gpio_init_output(HAL_OUT1_PORT, HAL_OUT1_PIN);}
#define HAL_OUT1_ON()                                               {GPIO_SetBit(HAL_OUT1_PORT,HAL_OUT1_PIN);}
#define HAL_OUT1_OFF()                                              {GPIO_ClearBit(HAL_OUT1_PORT,HAL_OUT1_PIN);}

#define HAL_OUT2_PORT											    GPIOB
#define HAL_OUT2_PIN											    GPIO_PIN_1
#define HAL_OUT2_INIT()                                             {iot_driver_gpio_init_output(HAL_OUT2_PORT, HAL_OUT2_PIN);}
#define HAL_OUT2_ON()                                               {GPIO_SetBit(HAL_OUT2_PORT,HAL_OUT2_PIN);}
#define HAL_OUT2_OFF()                                              {GPIO_ClearBit(HAL_OUT2_PORT,HAL_OUT2_PIN);}

#define HAL_OUT_INIT()                                              {HAL_OUT1_INIT();HAL_OUT2_INIT();}

/*************************************************-*************************************************
*                                            NIXIE TUBE
**************************************************-*****************^******************************/
// 段选引脚（a~g）
#define HAL_NIXIETUBE_SEGMENT_A_PORT                                GPIOA
#define HAL_NIXIETUBE_SEGMENT_A_PIN                                 GPIO_PIN_8
#define HAL_NIXIETUBE_SEGMENT_A_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_A_PORT, HAL_NIXIETUBE_SEGMENT_A_PIN);)
#define HAL_NIXIETUBE_SEGMENT_A_ON()                                {HAL_NIXIETUBE_SEGMENT_A_PORT->BR = HAL_NIXIETUBE_SEGMENT_A_PIN;}
#define HAL_NIXIETUBE_SEGMENT_A_OFF()                               {HAL_NIXIETUBE_SEGMENT_A_PORT->BSC = HAL_NIXIETUBE_SEGMENT_A_PIN;}

#define HAL_NIXIETUBE_SEGMENT_B_PORT                                GPIOA
#define HAL_NIXIETUBE_SEGMENT_B_PIN                                 GPIO_PIN_9
#define HAL_NIXIETUBE_SEGMENT_B_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_B_PORT, HAL_NIXIETUBE_SEGMENT_B_PIN);)
#define HAL_NIXIETUBE_SEGMENT_B_ON()                                {HAL_NIXIETUBE_SEGMENT_B_PORT->BR = HAL_NIXIETUBE_SEGMENT_B_PIN;}
#define HAL_NIXIETUBE_SEGMENT_B_OFF()                               {HAL_NIXIETUBE_SEGMENT_B_PORT->BSC = HAL_NIXIETUBE_SEGMENT_B_PIN;}

#define HAL_NIXIETUBE_SEGMENT_C_PORT                                GPIOA
#define HAL_NIXIETUBE_SEGMENT_C_PIN                                 GPIO_PIN_10
#define HAL_NIXIETUBE_SEGMENT_C_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_C_PORT, HAL_NIXIETUBE_SEGMENT_C_PIN);)
#define HAL_NIXIETUBE_SEGMENT_C_ON()                                {HAL_NIXIETUBE_SEGMENT_C_PORT->BR = HAL_NIXIETUBE_SEGMENT_C_PIN;}
#define HAL_NIXIETUBE_SEGMENT_C_OFF()                               {HAL_NIXIETUBE_SEGMENT_C_PORT->BSC = HAL_NIXIETUBE_SEGMENT_C_PIN;}

#define HAL_NIXIETUBE_SEGMENT_D_PORT                                GPIOA
#define HAL_NIXIETUBE_SEGMENT_D_PIN                                 GPIO_PIN_11
#define HAL_NIXIETUBE_SEGMENT_D_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_D_PORT, HAL_NIXIETUBE_SEGMENT_D_PIN);)
#define HAL_NIXIETUBE_SEGMENT_D_ON()                                {HAL_NIXIETUBE_SEGMENT_D_PORT->BR = HAL_NIXIETUBE_SEGMENT_D_PIN;}
#define HAL_NIXIETUBE_SEGMENT_D_OFF()                               {HAL_NIXIETUBE_SEGMENT_D_PORT->BSC = HAL_NIXIETUBE_SEGMENT_D_PIN;}

#define HAL_NIXIETUBE_SEGMENT_E_PORT                                GPIOA
#define HAL_NIXIETUBE_SEGMENT_E_PIN                                 GPIO_PIN_12
#define HAL_NIXIETUBE_SEGMENT_E_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_E_PORT, HAL_NIXIETUBE_SEGMENT_E_PIN);)
#define HAL_NIXIETUBE_SEGMENT_E_ON()                                {HAL_NIXIETUBE_SEGMENT_E_PORT->BR = HAL_NIXIETUBE_SEGMENT_E_PIN;}
#define HAL_NIXIETUBE_SEGMENT_E_OFF()                               {HAL_NIXIETUBE_SEGMENT_E_PORT->BSC = HAL_NIXIETUBE_SEGMENT_E_PIN;}

// 如果解除这个注释就会显示完整的数码管，因为这两个脚是用来下载程序的，我懒得等初始化就注释了
#ifndef IOT_DEBUG_PIN
#define HAL_NIXIETUBE_SEGMENT_F_PORT                                GPIOA
#define HAL_NIXIETUBE_SEGMENT_F_PIN                                 GPIO_PIN_13
#define HAL_NIXIETUBE_SEGMENT_F_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_F_PORT, HAL_NIXIETUBE_SEGMENT_F_PIN);)
#define HAL_NIXIETUBE_SEGMENT_F_ON()                                {HAL_NIXIETUBE_SEGMENT_F_PORT->BR = HAL_NIXIETUBE_SEGMENT_F_PIN;}
#define HAL_NIXIETUBE_SEGMENT_F_OFF()                               {HAL_NIXIETUBE_SEGMENT_F_PORT->BSC = HAL_NIXIETUBE_SEGMENT_F_PIN;}

#define HAL_NIXIETUBE_SEGMENT_G_PORT                                GPIOA
#define HAL_NIXIETUBE_SEGMENT_G_PIN                                 GPIO_PIN_14
#define HAL_NIXIETUBE_SEGMENT_G_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_G_PORT, HAL_NIXIETUBE_SEGMENT_G_PIN);)
#define HAL_NIXIETUBE_SEGMENT_G_ON()                                {HAL_NIXIETUBE_SEGMENT_G_PORT->BR = HAL_NIXIETUBE_SEGMENT_G_PIN;}
#define HAL_NIXIETUBE_SEGMENT_G_OFF()                               {HAL_NIXIETUBE_SEGMENT_G_PORT->BSC = HAL_NIXIETUBE_SEGMENT_G_PIN;}
#else
#define HAL_NIXIETUBE_SEGMENT_F_PORT                                GPIOA
#define HAL_NIXIETUBE_SEGMENT_F_PIN                                 0
#define HAL_NIXIETUBE_SEGMENT_F_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_F_PORT, HAL_NIXIETUBE_SEGMENT_F_PIN);)
#define HAL_NIXIETUBE_SEGMENT_F_ON()                                {HAL_NIXIETUBE_SEGMENT_F_PORT->BR = HAL_NIXIETUBE_SEGMENT_F_PIN;}
#define HAL_NIXIETUBE_SEGMENT_F_OFF()                               {HAL_NIXIETUBE_SEGMENT_F_PORT->BSC = HAL_NIXIETUBE_SEGMENT_F_PIN;}

#define HAL_NIXIETUBE_SEGMENT_G_PORT                                GPIOA
#define HAL_NIXIETUBE_SEGMENT_G_PIN                                 0
#define HAL_NIXIETUBE_SEGMENT_G_INIT()                              st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_G_PORT, HAL_NIXIETUBE_SEGMENT_G_PIN);)
#define HAL_NIXIETUBE_SEGMENT_G_ON()                                {HAL_NIXIETUBE_SEGMENT_G_PORT->BR = HAL_NIXIETUBE_SEGMENT_G_PIN;}
#define HAL_NIXIETUBE_SEGMENT_G_OFF()                               {HAL_NIXIETUBE_SEGMENT_G_PORT->BSC = HAL_NIXIETUBE_SEGMENT_G_PIN;}
#endif  

#define HAL_NIXIETUBE_SEGMENT_LED_PORT                              GPIOA
#define HAL_NIXIETUBE_SEGMENT_LED_PIN                               GPIO_PIN_15
#define HAL_NIXIETUBE_SEGMENT_LED_INIT()                            st(iot_driver_gpio_init_output(HAL_NIXIETUBE_SEGMENT_LED_PORT, HAL_NIXIETUBE_SEGMENT_LED_PIN);)
#define HAL_NIXIETUBE_SEGMENT_LED_ON()                              {HAL_NIXIETUBE_SEGMENT_LED_PORT->BR = HAL_NIXIETUBE_SEGMENT_LED_PIN;}
#define HAL_NIXIETUBE_SEGMENT_LED_OFF()                             {HAL_NIXIETUBE_SEGMENT_LED_PORT->BSC = HAL_NIXIETUBE_SEGMENT_LED_PIN;}

#define HAL_NIXIETUBE_SEGMENT_ALL_PIN_OFF()                         {GPIOA->BSC = (uint32_t)(HAL_NIXIETUBE_SEGMENT_A_PIN|HAL_NIXIETUBE_SEGMENT_B_PIN|HAL_NIXIETUBE_SEGMENT_C_PIN|HAL_NIXIETUBE_SEGMENT_D_PIN|HAL_NIXIETUBE_SEGMENT_E_PIN|HAL_NIXIETUBE_SEGMENT_F_PIN|HAL_NIXIETUBE_SEGMENT_G_PIN|HAL_NIXIETUBE_SEGMENT_LED_PIN);}

// 位选引脚（8个数码管）
#define HAL_NIXIETUBE_DIG_1_PORT                                    GPIOB
#define HAL_NIXIETUBE_DIG_1_PIN                                     GPIO_PIN_3
#define HAL_NIXIETUBE_DIG_1_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_1_PORT, HAL_NIXIETUBE_DIG_1_PIN);)
#define HAL_NIXIETUBE_DIG_1_ON()                                    {HAL_NIXIETUBE_DIG_1_PORT->BR = HAL_NIXIETUBE_DIG_1_PIN;}
#define HAL_NIXIETUBE_DIG_1_OFF()                                   {HAL_NIXIETUBE_DIG_1_PORT->BSC = HAL_NIXIETUBE_DIG_1_PIN;}

#define HAL_NIXIETUBE_DIG_2_PORT                                    GPIOB
#define HAL_NIXIETUBE_DIG_2_PIN                                     GPIO_PIN_4
#define HAL_NIXIETUBE_DIG_2_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_2_PORT, HAL_NIXIETUBE_DIG_2_PIN);)
#define HAL_NIXIETUBE_DIG_2_ON()                                    {HAL_NIXIETUBE_DIG_2_PORT->BR = HAL_NIXIETUBE_DIG_2_PIN;}
#define HAL_NIXIETUBE_DIG_2_OFF()                                   {HAL_NIXIETUBE_DIG_2_PORT->BSC = HAL_NIXIETUBE_DIG_2_PIN;}

#define HAL_NIXIETUBE_DIG_3_PORT                                    GPIOB
#define HAL_NIXIETUBE_DIG_3_PIN                                     GPIO_PIN_5
#define HAL_NIXIETUBE_DIG_3_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_3_PORT, HAL_NIXIETUBE_DIG_3_PIN);)
#define HAL_NIXIETUBE_DIG_3_ON()                                    {HAL_NIXIETUBE_DIG_3_PORT->BR = HAL_NIXIETUBE_DIG_3_PIN;}
#define HAL_NIXIETUBE_DIG_3_OFF()                                   {HAL_NIXIETUBE_DIG_3_PORT->BSC = HAL_NIXIETUBE_DIG_3_PIN;}

#define HAL_NIXIETUBE_DIG_4_PORT                                    GPIOB
#define HAL_NIXIETUBE_DIG_4_PIN                                     GPIO_PIN_6
#define HAL_NIXIETUBE_DIG_4_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_4_PORT, HAL_NIXIETUBE_DIG_4_PIN);)
#define HAL_NIXIETUBE_DIG_4_ON()                                    {HAL_NIXIETUBE_DIG_4_PORT->BR = HAL_NIXIETUBE_DIG_4_PIN;}
#define HAL_NIXIETUBE_DIG_4_OFF()                                   {HAL_NIXIETUBE_DIG_4_PORT->BSC = HAL_NIXIETUBE_DIG_4_PIN;}

#define HAL_NIXIETUBE_DIG_5_PORT                                    GPIOB
#define HAL_NIXIETUBE_DIG_5_PIN                                     GPIO_PIN_7
#define HAL_NIXIETUBE_DIG_5_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_5_PORT, HAL_NIXIETUBE_DIG_5_PIN);)
#define HAL_NIXIETUBE_DIG_5_ON()                                    {HAL_NIXIETUBE_DIG_5_PORT->BR = HAL_NIXIETUBE_DIG_5_PIN;}
#define HAL_NIXIETUBE_DIG_5_OFF()                                   {HAL_NIXIETUBE_DIG_5_PORT->BSC = HAL_NIXIETUBE_DIG_5_PIN;}

#define HAL_NIXIETUBE_DIG_6_PORT                                    GPIOB
#define HAL_NIXIETUBE_DIG_6_PIN                                     GPIO_PIN_8     
#define HAL_NIXIETUBE_DIG_6_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_6_PORT, HAL_NIXIETUBE_DIG_6_PIN);)
#define HAL_NIXIETUBE_DIG_6_ON()                                    {HAL_NIXIETUBE_DIG_6_PORT->BR = HAL_NIXIETUBE_DIG_6_PIN;}
#define HAL_NIXIETUBE_DIG_6_OFF()                                   {HAL_NIXIETUBE_DIG_6_PORT->BSC = HAL_NIXIETUBE_DIG_6_PIN;}

#define HAL_NIXIETUBE_DIG_7_PORT                                    GPIOF
#define HAL_NIXIETUBE_DIG_7_PIN                                     GPIO_PIN_0
#define HAL_NIXIETUBE_DIG_7_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_7_PORT, HAL_NIXIETUBE_DIG_7_PIN);)
#define HAL_NIXIETUBE_DIG_7_ON()                                    {HAL_NIXIETUBE_DIG_7_PORT->BR = HAL_NIXIETUBE_DIG_7_PIN;}
#define HAL_NIXIETUBE_DIG_7_OFF()                                   {HAL_NIXIETUBE_DIG_7_PORT->BSC = HAL_NIXIETUBE_DIG_7_PIN;}

#define HAL_NIXIETUBE_DIG_8_PORT                                    GPIOF
#define HAL_NIXIETUBE_DIG_8_PIN                                     GPIO_PIN_1
#define HAL_NIXIETUBE_DIG_8_INIT()                                  st(iot_driver_gpio_init_output(HAL_NIXIETUBE_DIG_8_PORT, HAL_NIXIETUBE_DIG_8_PIN);)
#define HAL_NIXIETUBE_DIG_8_ON()                                    {HAL_NIXIETUBE_DIG_8_PORT->BR = HAL_NIXIETUBE_DIG_8_PIN;}
#define HAL_NIXIETUBE_DIG_8_OFF()                                   {HAL_NIXIETUBE_DIG_8_PORT->BSC = HAL_NIXIETUBE_DIG_8_PIN;}

#define HAL_NIXIETUBE_DIG_ALL_PIN_OFF()                             {GPIOB->BSC = (uint32_t)(HAL_NIXIETUBE_DIG_1_PIN | HAL_NIXIETUBE_DIG_2_PIN | HAL_NIXIETUBE_DIG_3_PIN | HAL_NIXIETUBE_DIG_4_PIN | HAL_NIXIETUBE_DIG_5_PIN | HAL_NIXIETUBE_DIG_6_PIN);GPIOF->BSC = (uint32_t)(HAL_NIXIETUBE_DIG_7_PIN | HAL_NIXIETUBE_DIG_8_PIN);}

/*************************************************-*************************************************
*                                     Push Button Configuration
**************************************************-*****************^******************************/
#define HAL_BOARD_INIT()                                            \
{                                                                   \
    HAL_OUT_INIT();                                                 \
    ASM_NOP;                                                        \
}

/*************************************************-*************************************************
*                                              Debounce
**************************************************-*****************^******************************/
#define HAL_DEBOUNCE(expr)                                          { int i; for (i=0; i<500; i++) { if (!(expr)) i = 0; } }

/*************************************************-*************************************************
*                             OSAL NV implemented by internal flash pages.
**************************************************-*****************^******************************/
// Flash is partitioned into 8 banks of 32 KB or 16 pages.
#define HAL_FLASH_PAGE_PER_BANK                                     4

// Flash is constructed of 64 pages of 1 KB.
#define HAL_FLASH_PAGE_SIZE                                         1024
#define HAL_FLASH_WORD_SIZE                                         4

// CODE banks get mapped into the XDATA range 0x08000000 - 0x0800FFFF
#define HAL_FLASH_PAGE_MAP                                          0x08000000

#define HAL_NV_PAGE_CNT                                             2                                   //-- page count
#define HAL_NV_PAGE_END                                             63                                  //-- page end
#define HAL_NV_PAGE_BEG                                             (HAL_NV_PAGE_END-HAL_NV_PAGE_CNT+1) //-- page begin

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

//#ifndef HAL_NIXIETUBE
//#define HAL_NIXIETUBE FALSE
//#endif

/* Set to TRUE enable SEGMENTSCREEN usage, FALSE disable it */
#ifndef HAL_SEGMENT
#define HAL_SEGMENT TRUE
#endif

#ifndef HAL_ADC
#define HAL_ADC TRUE
#endif

#ifndef HAL_READFLASH
#define HAL_READFLASH TRUE
#endif

#ifndef HAL_USART
#define HAL_USART TRUE
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


/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#endif






