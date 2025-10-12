/**************************************************************************************************
  Filename:       OSAL_UserApp.c
  Revised:        $Date: 2008-02-07 12:10:00 -0800 (Thu, 07 Feb 2008) $
  Revision:       $Revision: 16360 $

  Description:    This file contains all the settings and other functions
                  that the user should set and change.


  Copyright 2004-2007 Texas Instruments Incorporated. All rights reserved.

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
  PROVIDED  AS IS  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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

/*********************************************************************
 * INCLUDES
 */

// 基础定义头文件（包含数据类型、常量等）
#include "ZComDef.h"
// 硬件抽象层驱动（按键、LED等外设）
#include "hal_drivers.h"
// 操作系统抽象层（任务调度、事件管理等）
#include "OSAL.h"
// OSAL任务列表（需与tasksArr同步）
#include "OSAL_Tasks.h"

// 用户自定义应用头文件
#include "main.h"

/*********************************************************************
 * GLOBAL VARIABLES
 */

// 任务事件处理函数指针数组
// 注意：数组顺序必须与osalInitTasks中的初始化顺序完全一致！
const pTaskEventHandlerFn tasksArr[] = 
{
    Hal_ProcessEvent,               // 硬件抽象层任务
    iot_app_process_event,          // 用户自定义任务（必须存在）
		hu_app_process_event,						// 嘉诚的事件管理
};

// 计算任务总数（用于动态分配事件数组）
const uint8 tasksCnt = sizeof( tasksArr ) / sizeof( tasksArr[0] );
// 存储各任务事件标志位的数组（每个任务占1个uint16）
uint16 *tasksEvents;

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/
void osalPoll(void)
{

	
}

/*********************************************************************
 * @fn      osalInitTasks
 *
 * @brief   初始化所有任务：
 *          1. 动态分配事件数组内存
 *          2. 清零事件数组
 *          3. 按顺序调用每个任务的初始化函数
 *
 * @param   void
 *
 * @return  none
 */
void osalInitTasks( void )
{
    // 当前分配的任务ID（从0开始递增）
    uint8 taskID = 0;

    /* 动态分配事件数组内存 */
    // 每个任务需要1个uint16存储事件标志位
    tasksEvents = (uint16 *)osal_mem_alloc( sizeof( uint16 ) * tasksCnt);

    /* 初始化事件数组为0 */
    osal_memset( tasksEvents, 0, (sizeof( uint16 ) * tasksCnt));

    /* 按顺序初始化所有任务 */
    // 注意：taskID在每次调用后会自动递增
    Hal_Init( taskID++ );       // 硬件抽象层初始化
    iot_app_init( taskID++ );
		hu_app_init ( taskID++ ); // 	嘉诚的任务初始化
}

/*********************************************************************
*********************************************************************/
