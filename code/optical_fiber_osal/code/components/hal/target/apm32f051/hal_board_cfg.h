/**************************************************************************************************
  Filename:       hal_board_cfg.h
  Revised:        $Date: 2012-03-29 12:09:02 -0700 (Thu, 29 Mar 2012) $
  Revision:       $Revision: 29943 $

  Description:    Declarations for the CC2530EM used on the SmartRF05EB.


  Copyright 2006-2010 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED 揂S IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

#ifndef HAL_BOARD_CFG_H
#define HAL_BOARD_CFG_H


/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/
#include "hal_mcu.h"
#include "hal_defs.h"
#include "hal_types.h"
#include "apm32f0xx.h"

/*************************************************-*************************************************
*                                               DEFINE
**************************************************-*****************^******************************/
#define ACTIVE_LOW                                                  !
#define ACTIVE_HIGH                                                 !!    /* double negation forces result to be '1' */

/*************************************************-*************************************************
*                                        LED Configuration
**************************************************-*****************^******************************/
#define HAL_NUM_LEDS                                                3

#define HAL_LED_BLINK_DELAY()                                       st( { volatile uint32 i; for (i=0; i<0x5800; i++) { }; } )

#define HAL_LED1_BV                                                 BV(0)
#define HAL_LED1_SBIT                                               
#define HAL_LED1_DDR                                                
#define HAL_LED1_PORT                                               GPIOB
#define HAL_LED1_PIN                                                GPIO_PIN_3
#define HAL_LED1_POLARITY                                           ACTIVE_HIGH

#define HAL_TURN_OFF_LED1()                                         st(GPIO_SetBit(HAL_LED1_PORT, HAL_LED1_PIN);)
#define HAL_TURN_ON_LED1()                                          st(GPIO_ClearBit(HAL_LED1_PORT, HAL_LED1_PIN);)
#define HAL_TOGGLE_LED1()                                           st(if (HAL_STATE_LED1()){HAL_TURN_OFF_LED1();}else{HAL_TURN_ON_LED1();})
#define HAL_STATE_LED1()                                            (HAL_LED1_POLARITY(GPIO_ReadInputBit(HAL_LED1_PORT, HAL_LED1_PIN)))


#define HAL_LED2_BV                                                 BV(1)
#define HAL_LED2_SBIT                                               
#define HAL_LED2_DDR                                                
#define HAL_LED2_PORT                                               GPIOB
#define HAL_LED2_PIN                                                GPIO_PIN_4
#define HAL_LED2_POLARITY                                           ACTIVE_HIGH

#define HAL_TURN_OFF_LED2()                                         st(GPIO_SetBit(HAL_LED2_PORT, HAL_LED2_PIN);)
#define HAL_TURN_ON_LED2()                                          st(GPIO_ClearBit(HAL_LED2_PORT, HAL_LED2_PIN);)
#define HAL_TOGGLE_LED2()                                           st(if (HAL_STATE_LED2()){HAL_TURN_OFF_LED2();}else{HAL_TURN_ON_LED2();})
#define HAL_STATE_LED2()                                            (HAL_LED2_POLARITY(GPIO_ReadInputBit(HAL_LED2_PORT, HAL_LED2_PIN)))


#define HAL_LED3_BV                                                 BV(1)
#define HAL_LED3_SBIT                                               
#define HAL_LED3_DDR                                                
#define HAL_LED3_PORT                                               GPIOB
#define HAL_LED3_PIN                                                GPIO_PIN_5
#define HAL_LED3_POLARITY                                           ACTIVE_HIGH

#define HAL_TURN_OFF_LED3()                                         st(GPIO_SetBit(HAL_LED3_PORT, HAL_LED3_PIN);)
#define HAL_TURN_ON_LED3()                                          st(GPIO_ClearBit(HAL_LED3_PORT, HAL_LED3_PIN);)
#define HAL_TOGGLE_LED3()                                           st(if (HAL_STATE_LED3()){HAL_TURN_OFF_LED3();}else{HAL_TURN_ON_LED3();})
#define HAL_STATE_LED3()                                            (HAL_LED3_POLARITY(GPIO_ReadInputBit(HAL_LED3_PORT, HAL_LED3_PIN)))



#define HAL_TURN_OFF_LED4()                                         HAL_TURN_OFF_LED1()
#define HAL_TURN_ON_LED4()                                          HAL_TURN_ON_LED1()
#define HAL_TOGGLE_LED4()                                           HAL_TOGGLE_LED1()
#define HAL_STATE_LED4()                                            HAL_STATE_LED1()

/*************************************************-*************************************************
*                                     Push Button Configuration
**************************************************-*****************^******************************/
#define HAL_BOARD_INIT()                                         \
{                                                                \
  asm("NOP");                                /* 插入空操作指令，芯片bug workaround（硬件缺陷修复） */ \
}


/* ----------- Debounce ---------- */
#define HAL_DEBOUNCE(expr)    { int i; for (i=0; i<500; i++) { if (!(expr)) i = 0; } }

/* ----------- Push Buttons ---------- */
#define HAL_PUSH_BUTTON1()        (PUSH1_POLARITY (PUSH1_SBIT))
#define HAL_PUSH_BUTTON2()        (PUSH2_POLARITY (PUSH2_SBIT))
#define HAL_PUSH_BUTTON3()        (0)
#define HAL_PUSH_BUTTON4()        (0)
#define HAL_PUSH_BUTTON5()        (0)
#define HAL_PUSH_BUTTON6()        (0)

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
//#ifndef HAL_FLASH
//#define HAL_FLASH FALSE
//#endif

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

#if (!defined BLINK_LEDS) && (HAL_LED == TRUE)
#define BLINK_LEDS
#endif

/* Set to TRUE enable KEY usage, FALSE disable it */
#ifndef HAL_KEY
#define HAL_KEY FALSE
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






