/**************************************************************************************************
  Filename:       hal_key.h
  Revised:        $Date: 2007-07-06 10:42:24 -0700 (Fri, 06 Jul 2007) $
  Revision:       $Revision: 13579 $

  Description:    This file contains the interface to the KEY Service.


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

#ifndef HAL_KEY_H
#define HAL_KEY_H

#ifdef __cplusplus
extern "C"
{
#endif

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/
#include "hal_board.h"

/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
/* Interrupt option - Enable or disable */
#define HAL_KEY_INTERRUPT_DISABLE                                   0x00  // 禁用按键中断（轮询模式）
#define HAL_KEY_INTERRUPT_ENABLE                                    0x01  // 启用按键中断（事件驱动模式）

/* Key state - shift or normal */
#define HAL_KEY_STATE_NORMAL                                        0x00  // 普通按键状态
#define HAL_KEY_STATE_SHIFT                                         0x01  // 组合键状态（如Shift+按键）

#define HAL_KEY_1                                                   0x01
#define HAL_KEY_2                                                   0x02
#define HAL_KEY_3                                                   0x04
#define HAL_KEY_4                                                   0x08
#define HAL_KEY_5                                                   0x10
#define HAL_KEY_6                                                   0x20
#define HAL_KEY_7                                                   0x40

#define HAL_KEY_UP                                                  0x01  // 摇杆上键（同HAL_KEY_SW_1）
#define HAL_KEY_RIGHT                                               0x02  // 摇杆右键（同HAL_KEY_SW_2）
#define HAL_KEY_CENTER                                              0x04  // 摇杆中心键（同HAL_KEY_SW_5）
#define HAL_KEY_LEFT                                                0x08  // 摇杆左键（同HAL_KEY_SW_4）
#define HAL_KEY_DOWN                                                0x10  // 摇杆下键（同HAL_KEY_SW_3）

/*************************************************-*************************************************
*                                              TYPEDEFS
**************************************************-*****************^******************************/
typedef uint8 (*hal_key_callback_t) (uint8 cur_keys, uint8 pre_keys, uint32 poll_time_milliseconds);

/*************************************************-*************************************************
*                                           GLOBAL VARIABLES
**************************************************-*****************^******************************/
extern uint16 hal_key_press_time_count[8];
extern uint16 hal_key_release_time_count[8];

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/
extern void  HalKeyInit(void);
extern uint8 HalKeyRead(void);
extern void  HalKeyEnterSleep(void);
extern uint8 HalKeyExitSleep(void);
extern void  HalKeyPoll(void);
extern bool  HalKeyPressed(void);
extern uint8 HalKeyCallbackRegister(hal_key_callback_t cback);

/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/

#ifdef __cplusplus
}
#endif

#endif
