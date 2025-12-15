/**************************************************************************************************
  Filename:       OSAL_Timers.c
  Revised:        $Date: 2010-09-17 16:25:30 -0700 (Fri, 17 Sep 2010) $
  Revision:       $Revision: 23835 $

  Description:    OSAL Timer definition and manipulation functions.


  Copyright 2004-2009 Texas Instruments Incorporated. All rights reserved.

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

/*********************************************************************
 * INCLUDES
 */

#include "comdef.h"
#include "OnBoard.h"
#include "OSAL.h"
#include "OSAL_Timers.h"
#include "hal_timer.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/**
 * @struct osalTimerRec_t
 * @brief 操作系统抽象层（OSAL）定时器记录结构体
 *        用于管理软件定时器的状态和配置
 */
typedef struct
{
  void   *next;        /**< 指向下一个定时器记录的指针（用于构建定时器链表） */
  
  uint16 timeout;      /**< 当前定时器倒计时值（单位：系统时钟滴答或毫秒）
                        *   定时器服务会定期递减该值，直到为0表示超时 */
  
  uint16 event_flag;   /**< 定时器超时时需要设置的事件标志位
                        *   通常用于任务间通信，通知关联任务定时器已触发 */
  
  uint8  task_id;      /**< 与定时器关联的任务ID
                        *   用于标识哪个任务应该接收定时器事件通知 */
  
  uint16 reloadTimeout;/**< 定时器重新加载值（用于周期性定时器）
                        *   当定时器触发后，若为周期性定时器，会重新加载此值到timeout */
} osalTimerRec_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */

osalTimerRec_t *timerHead;//定时器任务链表头

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
// Milliseconds since last reboot
static uint32 osal_systemClock;//记录自系统重启以来的毫秒数（系统时钟）。

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */
osalTimerRec_t  *osalAddTimer( uint8 task_id, uint16 event_flag, uint16 timeout );//创建一个新的定时器并添加到定时器链表中。
osalTimerRec_t *osalFindTimer( uint8 task_id, uint16 event_flag );//根据任务ID和事件标志查找已存在的定时器。
void osalDeleteTimer( osalTimerRec_t *rmTimer );//从定时器链表中移除指定的定时器并释放资源。

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osalTimerInit
 *
 * @brief   Initialization for the OSAL Timer System.
 *
 * @param   none
 *
 * @return
 */
void osalTimerInit( void )//清除历史时间戳
{
  osal_systemClock = 0;
}

/*********************************************************************
 * @fn      osalAddTimer
 *
 * @brief   向定时器列表中添加一个定时器。
 *          注意：调用此函数时必须禁用中断（Ints must be disabled）。
 *
 * @param   task_id - 任务ID，用于标识定时器所属的任务
 * @param   event_flag - 事件标志，定时器超时时触发的事件
 * @param   timeout - 超时时间（单位由系统定义，如毫秒或时钟滴答）
 *
 * @return  osalTimerRec_t * - 指向新创建的定时器的指针，失败返回
 */
osalTimerRec_t * osalAddTimer( uint8 task_id, uint16 event_flag, uint16 timeout )
{
  osalTimerRec_t *newTimer;     // 新定时器指针
  osalTimerRec_t *srchTimer;    // 用于遍历链表的临时指针

  // 首先查找是否已存在相同的定时器（通过task_id和event_flag匹配）
  newTimer = osalFindTimer( task_id, event_flag );
  if ( newTimer )
  {
    // 如果找到已存在的定时器，则更新其超时时间
    newTimer->timeout = timeout;

    return ( newTimer );
  }
  else
  {
    // 如果是全新的定时器，则分配内存
    newTimer = osal_mem_alloc( sizeof( osalTimerRec_t ) );
    // 检查内存是否分配成功
    if ( newTimer )//内存分配成功
    {
     // 填充新定时器的字段
      newTimer->task_id = task_id;
      newTimer->event_flag = event_flag;
      newTimer->timeout = timeout;
      newTimer->next = (void *)NULL;
      newTimer->reloadTimeout = 0;

      // 检查定时器链表是否为空
      if ( timerHead == NULL )
      {
        // 如果链表为空，将新定时器设为链表头
        timerHead = newTimer;
      }
      else
      {
        // 如果链表非空，将新定时器添加到链表末尾
        srchTimer = timerHead;// 从链表头开始遍历

         // 遍历到最后一个节点
        while ( srchTimer->next )//一直找到指针不存在为止
          srchTimer = srchTimer->next;

        // 将新定时器添加到链表末尾
        srchTimer->next = newTimer;
      }
      // 返回新创建的定时器指针
      return ( newTimer );
    }
    else// 内存分配失败，返回
      return ( (osalTimerRec_t *)NULL );
  }
}

/*********************************************************************
 * @fn      osalFindTimer
 *
 * @brief   在定时器列表中查找指定的定时器。
 *          注意：调用此函数时必须禁用中断（Ints must be disabled）。
 *
 * @param   task_id - 要查找的定时器所属的任务ID
 * @param   event_flag - 要查找的定时器的事件标志
 *
 * @return  osalTimerRec_t * - 返回找到的定时器指针，未找到则返回
 */
osalTimerRec_t *osalFindTimer( uint8 task_id, uint16 event_flag )
{
  osalTimerRec_t *srchTimer;  // 用于遍历定时器链表的临时指针

  // 从定时器链表头部开始查找
  srchTimer = timerHead;

  // 遍历链表，直到找到匹配的定时器或到达链表末尾
  while ( srchTimer )
  {
    // 检查当前定时器的任务ID和事件标志是否匹配
    if ( srchTimer->event_flag == event_flag &&
         srchTimer->task_id == task_id )
      break;  // 找到匹配的定时器，退出循环

    // 当前定时器不匹配，继续检查下一个
    srchTimer = srchTimer->next;
  }

  // 返回找到的定时器指针（可能为）
  return ( srchTimer );
}
/*********************************************************************
 * @fn      osalDeleteTimer
 *
 * @brief   从定时器列表中删除指定的定时器。
 *          注意：此函数不直接释放内存，而是通过将事件标志清零，
 *          由后续的 `osalTimerUpdate()` 函数实际移除定时器。
 *
 * @param   rmTimer - 指向要删除的定时器的指针
 *
 * @return  none
 */
void osalDeleteTimer( osalTimerRec_t *rmTimer )
{
  // 检查定时器指针是否有效（非空）
  if ( rmTimer )
  {
    // 将事件标志清零，osalTimerUpdate() 会在后续处理中
    // 检测到此标志变化并从链表中移除该定时器
    rmTimer->event_flag = 0;
  }
}
/**
 * @brief 启动一个扩展定时器（可指定任务ID、事件ID和超时时间）
 * @param taskID 目标任务ID（定时器到期后事件将发送到此任务）
 * @param event_id 定时器到期后触发的事件ID
 * @param timeout_value 定时器超时时间（毫秒）
 * @return 成功返回SUCCESS，失败返回NO_TIMER_AVAIL（无可用定时器资源）
 */
uint8 osal_start_timerEx( uint8 taskID, uint16 event_id, uint16 timeout_value )
{
    halIntState_t intState;          // 保存中断状态的变量
    osalTimerRec_t *newTimer;        // 新定时器记录指针

    // 进入临界区（关闭中断），防止定时器列表被并发修改
    HAL_ENTER_CRITICAL_SECTION( intState );

    // 调用底层函数添加定时器到系统定时器列表
    // 参数包括：目标任务ID、事件ID、超时时间
    newTimer = osalAddTimer( taskID, event_id, timeout_value );

    // 退出临界区（恢复中断状态）
    HAL_EXIT_CRITICAL_SECTION( intState );

    // 返回操作结果：成功（newTimer非空）或失败（无可用定时器）
    return ( (newTimer !=NULL) ? SUCCESS : NO_TIMER_AVAIL );
}

/*********************************************************************
 * @fn      osal_start_reload_timer
 *
 * @brief
 *
 *   此函数用于启动一个**周期性定时器**，在指定的毫秒时间后触发，
 *   并在每次触发后自动重新加载超时值，实现周期性通知。
 *   当定时器到期时，调用任务（taskID）将收到指定的事件（event_id）。
 *
 * @param   uint8 taskID - 需要设置定时器的任务ID
 * @param   uint16 event_id - 定时器到期时通知的事件标识
 * @param   uint16 timeout_value - 超时时间（毫秒）
 *
 * @return  
 *   SUCCESS - 定时器创建成功  
 *   NO_TIMER_AVAIL - 定时器资源不足，创建失败
 */
uint8 osal_start_reload_timer( uint8 taskID, uint16 event_id, uint16 timeout_value )
{
  halIntState_t intState;          // 保存中断状态的变量
  osalTimerRec_t *newTimer;        // 新定时器记录指针

  HAL_ENTER_CRITICAL_SECTION( intState );  // 关闭中断，保护临界区

  // 添加定时器到链表（单次超时）
  newTimer = osalAddTimer( taskID, event_id, timeout_value );
  if ( newTimer )
  {
    // 如果是周期性定时器，额外设置重新加载的超时值
    newTimer->reloadTimeout = timeout_value;
  }

  HAL_EXIT_CRITICAL_SECTION( intState );   // 恢复中断

  // 返回操作状态：成功或资源不足
  return ( (newTimer !=NULL) ? SUCCESS : NO_TIMER_AVAIL );
}
/*********************************************************************
 * @fn      osal_stop_timerEx
 *
 * @brief
 *
 *   此函数用于停止一个已启动的定时器。  
 *   如果操作成功（SUCCESS），函数将取消定时器，并阻止与该定时器关联的事件  
 *   被设置到调用任务的事件队列中。
 *
 * @param   uint8 task_id - 需要停止定时器的任务ID
 * @param   uint16 event_id - 需要停止的定时器的事件标识符
 *
 * @return  
 *   SUCCESS - 定时器成功停止  
 *   INVALID_EVENT_ID - 未找到匹配的定时器（可能参数无效或定时器不存在）
 */
uint8 osal_stop_timerEx( uint8 task_id, uint16 event_id )
{
  halIntState_t intState;          // 保存中断状态的变量
  osalTimerRec_t *foundTimer;      // 用于查找定时器的指针

  HAL_ENTER_CRITICAL_SECTION( intState );  // 关闭中断，保护临界区

  // 查找匹配的定时器（根据task_id和event_id）
  foundTimer = osalFindTimer( task_id, event_id );
  if ( foundTimer )
  {
    // 如果找到定时器，则从链表中删除它（通过标记event_flag=0实现）
    osalDeleteTimer( foundTimer );
  }

  HAL_EXIT_CRITICAL_SECTION( intState );   // 恢复中断

  // 返回操作状态：成功或未找到定时器
  return ( (foundTimer !=NULL) ? SUCCESS : INVALID_EVENT_ID );
}
/*********************************************************************
 * @fn      osal_get_timeoutEx
 *
 * @brief
 *   此函数用于查询指定任务和事件ID对应的定时器的剩余时间（Tick数）。  
 *   如果找到匹配的定时器，返回其剩余Tick数；否则返回0。
 *
 * @param   uint8 task_id - 需要查询的任务ID
 * @param   uint16 event_id - 需要查询的事件ID
 *
 * @return  
 *   >0 - 定时器的剩余Tick数（找到定时器）  
 *   0   - 未找到匹配的定时器
 */
uint16 osal_get_timeoutEx( uint8 task_id, uint16 event_id )
{
  halIntState_t intState;      // 保存中断状态的变量
  uint16 rtrn = 0;             // 默认返回值（未找到）
  osalTimerRec_t *tmr;         // 定时器记录指针

  HAL_ENTER_CRITICAL_SECTION( intState );  // 关闭中断，保护临界区

  // 查找匹配的定时器
  tmr = osalFindTimer( task_id, event_id );
  if ( tmr )
  {
    rtrn = tmr->timeout;       // 返回剩余Tick数
  }

  HAL_EXIT_CRITICAL_SECTION( intState );   // 恢复中断

  return rtrn;
}
/*********************************************************************
 * @fn      osal_timer_num_active
 *
 * @brief
 *   此函数统计当前系统中所有**未触发**的定时器数量。  
 *   适用于调试或资源监控场景。
 *
 * @return  uint8 - 当前活跃的定时器总数
 */
uint8 osal_timer_num_active( void )
{
  halIntState_t intState;      // 保存中断状态的变量
  uint8 num_timers = 0;        // 定时器计数器
  osalTimerRec_t *srchTimer;   // 用于遍历定时器链表的指针

  HAL_ENTER_CRITICAL_SECTION( intState );  // 关闭中断，保护临界区

  // 从定时器链表头开始遍历
  srchTimer = timerHead;

  // 遍历链表并计数
  while ( srchTimer != NULL)
  {
    num_timers++;             // 计数器递增
    srchTimer = srchTimer->next; // 移动到下一个定时器
  }

  HAL_EXIT_CRITICAL_SECTION( intState );   // 恢复中断

  return num_timers;
}

/*********************************************************************
 * @fn      osalTimerUpdate
 *
 * @brief   Update the timer structures for a timer tick.
 *
 * @param   updateTime - 时间更新值（毫秒），通常为系统时钟节拍周期
 *
 * @return  none
 *********************************************************************/
void osalTimerUpdate( uint16 updateTime )
{
  halIntState_t intState;          // 保存中断状态的变量
  osalTimerRec_t *srchTimer;       // 当前遍历的定时器指针
  osalTimerRec_t *prevTimer;       // 前一个定时器指针（用于链表操作）

  // 进入临界区，防止中断干扰系统时钟更新
  HAL_ENTER_CRITICAL_SECTION( intState );
  // 更新系统全局时钟（累计运行时间）
  osal_systemClock += updateTime;
  // 退出临界区，恢复中断
  HAL_EXIT_CRITICAL_SECTION( intState );

  // 检查定时器链表是否非空,表头不为空
  if ( timerHead != NULL)
  {
    // 初始化遍历指针：从头节点开始
    srchTimer = timerHead;
    prevTimer = (void *)NULL;      // 前驱节点初始为空

    // 遍历整个定时器链表
    while ( srchTimer )
    {
      osalTimerRec_t *freeTimer = NULL;  // 标记需要释放的定时器
     
      // 再次进入临界区，保护链表操作
      HAL_ENTER_CRITICAL_SECTION( intState );
      
      // 更新当前定时器的剩余时间
      if (srchTimer->timeout <= updateTime)
      {
        srchTimer->timeout = 0;    // 超时置零
      }
      else
      {
        srchTimer->timeout -= updateTime; // 递减剩余时间
      }
      
      /* 处理周期性定时器（自动重载） */
      if ( (srchTimer->timeout == 0) && (srchTimer->reloadTimeout) && (srchTimer->event_flag) )
      {
        // 触发超时事件到对应任务
        osal_set_event( srchTimer->task_id, srchTimer->event_flag );
        
        // 重载定时器（设置下一次超时时间）
        srchTimer->timeout = srchTimer->reloadTimeout;
      }
      
      /* 处理需要删除的定时器（超时或主动停止） */
      if ( srchTimer->timeout == 0 || srchTimer->event_flag == 0 )
      {
        // 从链表中移除当前定时器
        if ( prevTimer == NULL)
          timerHead = srchTimer->next;  // 删除的是头节点
        else
          prevTimer->next = srchTimer->next; // 跳过当前节点

        freeTimer = srchTimer;     // 标记为待释放
        srchTimer = srchTimer->next; // 移动到下一个节点
      }
      else//没有超时
      {
        // 正常移动到下一个节点
        prevTimer = srchTimer;
        srchTimer = srchTimer->next;
      }
      
      // 退出临界区
      HAL_EXIT_CRITICAL_SECTION( intState );
      
      /* 释放已移除的定时器内存 */
      //并且触发事件任务
      if ( freeTimer )
      {
        // 确保超时事件触发（处理临界区外的事件设置）
        if ( freeTimer->timeout == 0 )
        {
          osal_set_event( freeTimer->task_id, freeTimer->event_flag );
        }
        // 释放定时器内存
        osal_mem_free( freeTimer );
      }
    }
  }
}

//低功耗（POWER_SAVING）相关
#ifdef POWER_SAVING
/*********************************************************************
 * @fn      osal_adjust_timers
 *
 * @brief   调整所有活动定时器的剩余时间，补偿系统进入低功耗模式（如睡眠）期间错过的时间滴答（ticks）。
 *
 * @param   none
 *
 * @return  none
 *********************************************************************/
void osal_adjust_timers(void)
{
  uint16 eTime;  // 记录 elapsed time（流逝的时间）

  if (timerHead !=NULL)  // 检查是否有活动的定时器
  {
    // 计算自上次唤醒以来流逝的时间（单位：tick）
    eTime = TimerElapsed() / TICK_COUNT;//返回硬件定时器（如 SysTick）在低功耗模式下流逝的原始计数值（如微秒或硬件时钟周期）。

    if (eTime)  // 如果有时间流逝（eTime > 0）
      osalTimerUpdate(eTime);  // 更新所有定时器的剩余时间
  }
}

/*********************************************************************
 * @fn      osal_next_timeout
 *
 * @brief
 *
 * @brief   遍历所有活动定时器，返回 下一个最近要超时的定时器的剩余时间。适用于系统从低功耗模式唤醒后，同步定时器状态。
 *
 * @param   none
 *
 * @return  none
 *********************************************************************/
uint16 osal_next_timeout(void)
{
  uint16 nextTimeout;
  osalTimerRec_t *srchTimer;

  if (timerHead !=NULL)  // 如果有活动的定时器
  {
    srchTimer = timerHead;  // 从链表头开始搜索
    nextTimeout = OSAL_TIMERS_MAX_TIMEOUT;  // 初始化为最大值

    // 遍历链表，寻找最小的 timeout
    while (srchTimer !=NULL)
    {
      if (srchTimer->timeout < nextTimeout)
      {
        nextTimeout = srchTimer->timeout;  // 更新最小值
      }
      srchTimer = srchTimer->next;  // 检查下一个定时器
    }
  }
  else
  {
    nextTimeout = 0;  // 无定时器
  }

  return nextTimeout;
}
#endif // POWER_SAVING

/*********************************************************************
 * @fn      osal_GetSystemClock()
 *
 * @brief   Read the local system clock.
 *
 * @param   none
 *
 * @return  local clock in milliseconds
 */
uint32 osal_GetSystemClock( void )
{
  return ( osal_systemClock );
}

/*********************************************************************
*********************************************************************/
