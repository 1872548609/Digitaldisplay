/**************************************************************************************************
  Filename:       hal_drivers.h
  Revised:        $Date: 2010-10-05 11:47:04 -0700 (Tue, 05 Oct 2010) $
  Revision:       $Revision: 23996 $

  Description:    This file contains the interface to the Drivers service.


  Copyright 2005-2007 Texas Instruments Incorporated. All rights reserved.

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
  PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
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

#ifndef HAL_DRIVER_H
#define HAL_DRIVER_H

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include "main.h"

#include "hal_types.h"       // 硬件抽象层类型定义
#include "OSAL.h"            // 操作系统抽象层
#include "hal_adc.h"         // ADC驱动
#if (defined HAL_DMA) && (HAL_DMA == TRUE)
  #include "hal_dma.h"       // DMA驱动（可选）
#endif
#include "hal_key.h"         // 按键驱动
#include "hal_lcd.h"         // LCD驱动
#include "hal_led.h"         // LED驱动
#include "hal_timer.h"       // 定时器驱动
#include "hal_uart.h"        // UART驱动
#include "hal_sleep.h"       // 睡眠管理
#if (defined HAL_AES) && (HAL_AES == TRUE)
  #include "hal_aes.h"       // AES加密驱动（可选）
#endif
#if (defined HAL_SPI) && (HAL_SPI == TRUE)
  #include "hal_spi.h"       // SPI驱动（可选）
#endif
#if (defined HAL_HID) && (HAL_HID == TRUE)
  #include "usb_hid.h"       // USB HID驱动（可选）
#endif
#ifdef CC2591_COMPRESSION_WORKAROUND
  #include "mac_rx.h"        // MAC层接收修复（特定场景）
#endif

/**************************************************************************************************
 * MACROS
 **************************************************************************************************/



/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/
#define HAL_KEY_EVENT         0x0001
#define HAL_LED_BLINK_EVENT   0x0002
#define HAL_SLEEP_TIMER_EVENT 0x0004
#define PERIOD_RSSI_RESET_EVT 0x0008

#define PERIOD_RSSI_RESET_TIMEOUT           10

/**************************************************************************************************
 * TYPEDEFS
 **************************************************************************************************/


/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/
extern uint8 Hal_TaskID;

/**************************************************************************************************
 * FUNCTIONS - API
 **************************************************************************************************/

/**************************************************************************************************
 * Serial Port Initialization
 **************************************************************************************************/
extern void Hal_Init ( uint8 task_id );

/*
 * Process Serial Buffer
 */
extern uint16 Hal_ProcessEvent ( uint8 task_id, uint16 events );

/*
 * Process Polls
 */
extern void Hal_ProcessPoll (void);

/*
 * Initialize HW
 */
extern void HalDriverInit (void);

/**************************************************************************************************
**************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
