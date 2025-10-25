/**************************************************************************************************
  Filename:       hal_led.c
  Revised:        $Date: 2012-02-14 12:43:32 -0800 (Tue, 14 Feb 2012) $
  Revision:       $Revision: 29281 $

  Description:    This file contains the interface to the HAL LED Service.

  Copyright 2006-2012 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License"). You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product. Other than for
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

/***************************************************************************************************
 *                                             INCLUDES
 ***************************************************************************************************/
#include "hal_mcu.h"       // 微控制器硬件抽象层头文件
#include "hal_defs.h"      // 硬件抽象层定义头文件
#include "hal_types.h"     // 硬件抽象层类型定义头文件
#include "hal_drivers.h"   // 硬件抽象层驱动头文件
#include "hal_led.h"       // LED硬件抽象层头文件
#include "osal.h"          // 操作系统抽象层头文件
#include "hal_board.h"     // 开发板硬件抽象层头文件

/***************************************************************************************************
 *                                             CONSTANTS
 ***************************************************************************************************/
// 常量定义区域（原文为空）

/***************************************************************************************************
 *                                              MACROS
 ***************************************************************************************************/
// 宏定义区域（原文为空）

/***************************************************************************************************
 *                                              TYPEDEFS
 ***************************************************************************************************/
/* LED控制结构体 */
typedef struct {
  uint8 mode;       /* 操作模式 */
  uint8 todo;       /* 剩余的闪烁次数 */
  uint8 onPct;      /* 点亮时间百分比 */
  uint16 time;      /* 亮灭周期时间（毫秒） */
  uint32 next;      /* 下次状态变化时间 */
} HalLedControl_t;

/* LED状态结构体 */
typedef struct
{
  HalLedControl_t HalLedControlTable[HAL_LED_DEFAULT_MAX_LEDS]; // LED控制表
  uint8           sleepActive;                                  // 睡眠模式激活标志
} HalLedStatus_t;

/***************************************************************************************************
 *                                           GLOBAL VARIABLES
 ***************************************************************************************************/
// 全局变量
uint8 HalLedState;              // 最后一次设置/清除/闪烁更新时的LED状态

#if HAL_LED == TRUE
static uint8 HalSleepLedState;         // 睡眠模式下的LED状态
static uint8 preBlinkState;            // 进入闪烁模式前的原始状态
                                       // bit 0,1,2,3分别代表LED 0,1,2,3
#endif

#ifdef BLINK_LEDS
  static HalLedStatus_t HalLedStatusControl; // LED状态控制结构体
#endif

/***************************************************************************************************
 *                                            LOCAL FUNCTION
 ***************************************************************************************************/
#if (HAL_LED == TRUE)
void HalLedUpdate (void);              // 更新LED状态
void HalLedOnOff (uint8 leds, uint8 mode); // 控制LED开关
#endif /* HAL_LED */

/***************************************************************************************************
 *                                            FUNCTIONS - API
 ***************************************************************************************************/

/***************************************************************************************************
 * @fn      HalLedInit
 *
 * @brief   初始化LED服务
 *
 * @param   init - 指向包含初始化值的void指针
 *
 * @return  None
 ***************************************************************************************************/
void HalLedInit(void)
{
    #if defined(HAL_LED) && (HAL_LED == TRUE)
    
	HAL_LED_Init();
	
    /* 初始化所有LED为关闭状态 */
    HalLedSet (HAL_LED_ALL, HAL_LED_MODE_OFF);
    #endif /* HAL_LED */
    
    #ifdef BLINK_LEDS
    /* 初始化sleepActive为FALSE */
    HalLedStatusControl.sleepActive = FALSE;
    #endif
}

/***************************************************************************************************
 * @fn      HalLedSet
 *
 * @brief   打开/关闭/切换指定LED
 *
 * @param   leds - 需要控制的LED位掩码
 *          mode - BLINK（闪烁）、FLASH（闪亮）、TOGGLE（切换）、ON（开）、OFF（关）
 * @return  None
 ***************************************************************************************************/
uint8 HalLedSet (uint8 leds, uint8 mode)
{
    #if (defined (BLINK_LEDS)) && (HAL_LED == TRUE)
    uint8 led;
    HalLedControl_t *sts;

    switch (mode)
    {
    case HAL_LED_MODE_BLINK:
        /* 默认闪烁：1次，D%占空比 */
        HalLedBlink (leds, 1, HAL_LED_DEFAULT_DUTY_CYCLE, HAL_LED_DEFAULT_FLASH_TIME);
        break;

    case HAL_LED_MODE_FLASH:
        /* 默认闪亮：N次，D%占空比 */
        HalLedBlink (leds, HAL_LED_DEFAULT_FLASH_COUNT, HAL_LED_DEFAULT_DUTY_CYCLE, HAL_LED_DEFAULT_FLASH_TIME);
        break;

    case HAL_LED_MODE_ON:
    case HAL_LED_MODE_OFF:
    case HAL_LED_MODE_TOGGLE:
        led = HAL_LED_1;
        leds &= HAL_LED_ALL; // 确保只处理有效的LED
        sts = HalLedStatusControl.HalLedControlTable;

        while (leds)
        {
            if (leds & led)
            {
                if (mode != HAL_LED_MODE_TOGGLE)
                {
                    sts->mode = mode;  /* 设置为ON或OFF */
                }
                else
                {
                    sts->mode ^= HAL_LED_MODE_ON;  /* 切换状态 */
                }
                HalLedOnOff (led, sts->mode); // 实际控制LED
                leds ^= led; // 清除已处理的LED位
            }
            
            led <<= 1; // 处理下一个LED
            sts++; // 移动到下一个LED的控制结构
        }
        break;

    default:
        break;
    }

    #elif (HAL_LED == TRUE)
    HalLedOnOff(leds, mode); // 直接控制LED开关
    #else
    // HAL LED功能禁用，避免未使用参数警告
    (void) leds;
    (void) mode;
    #endif /* BLINK_LEDS && HAL_LED   */

    return ( HalLedState ); // 返回当前LED状态
}

/***************************************************************************************************
 * @fn      HalLedBlink
 *
 * @brief   控制LED闪烁
 *
 * @param   leds       - 需要闪烁的LED位掩码
 *          numBlinks  - 闪烁次数
 *          percent    - 每个周期中LED点亮的时间百分比
 *          period     - 每个周期的长度（毫秒）
 *
 * @return  None
 ***************************************************************************************************/
void HalLedBlink (uint8 leds, uint8 numBlinks, uint8 percent, uint16 period)
{
#if (defined (BLINK_LEDS)) && (HAL_LED == TRUE)
  uint8 led;//临时记录led
  HalLedControl_t *sts;

  if (leds && percent && period) // 参数有效性检查如果 leds、percent 或 period 为0，直接关闭LED（无效参数处理）。
  {
    if (percent < 100) // 只有占空比小于100%时才需要闪烁，占空比 <100%：需要闪烁逻辑。占空比 ≥100%：直接点亮LED（HalLedSet(leds, HAL_LED_MODE_ON)）。
    {
      led = HAL_LED_1;//初始临时led1，用于后续遍历
      leds &= HAL_LED_ALL; // 过滤无效LED
      sts = HalLedStatusControl.HalLedControlTable;//led状态表获取

      while (leds)//如果leds存在
      {
        if (leds & led)//leds & led 检查当前LED是否需要配置。
        {
          /* 如果LED当前不是闪烁模式，保存其原始状态 */
          if(sts->mode < HAL_LED_MODE_BLINK )
          	preBlinkState |= (led & HalLedState);//记录闪烁之前的led状态

          sts->mode  = HAL_LED_MODE_OFF;                    /* 停止之前的闪烁 */
          sts->time  = period;                              /* 设置周期时间 */
          sts->onPct = percent;                             /* 设置点亮百分比 */
          sts->todo  = numBlinks;                           /* 设置闪烁次数 */
          if (!numBlinks) sts->mode |= HAL_LED_MODE_FLASH;  /* 0表示连续闪烁 */
          sts->next = osal_GetSystemClock();                /* 记录下次状态变化时间*/
          sts->mode |= HAL_LED_MODE_BLINK;                  /* 切换启用闪烁 */
          leds ^= led; // LED位异或清除
        }
        led <<= 1; // 处理下一个LED
        sts++; // 移动到下一个LED的控制结构
      }

      osal_stop_timerEx(Hal_TaskID, HAL_LED_BLINK_EVENT);//去除旧的闪烁事件
      osal_set_event (Hal_TaskID, HAL_LED_BLINK_EVENT); // 设置新的闪烁事件
    }
    else
    {
      HalLedSet (leds, HAL_LED_MODE_ON);                    /* >=100%时直接打开 */
    }
  }
  else
  {
    HalLedSet (leds, HAL_LED_MODE_OFF);                     /* 参数无效时关闭 */
  }
#elif (HAL_LED == TRUE)//如果关闭闪烁功能，直接根据旧状态切换输出
  percent = (leds & HalLedState) ? HAL_LED_MODE_OFF : HAL_LED_MODE_ON;
  HalLedOnOff (leds, percent);                              /* 切换状态 */
#else
  // HAL LED功能禁用，避免未使用参数警告
  (void) leds;
  (void) numBlinks;
  (void) percent;
  (void) period;
#endif /* BLINK_LEDS && HAL_LED */
}

#if (HAL_LED == TRUE)
/***************************************************************************************************
 * @fn      HalLedUpdate
 *
 * @brief   更新LED状态以实现闪烁效果
 *
 * @param   none
 *
 * @return  none
 ***************************************************************************************************/
void HalLedUpdate (void)
{
  uint8 led;//当前处理的LED（从 HAL_LED_1 开始，左移遍历所有LED）。
  uint8 pct;
  uint8 leds;//所有LED的位掩码（HAL_LED_ALL）。
  HalLedControl_t *sts;//指向LED控制状态表（HalLedControl_t 结构体数组）。
  uint32 time;//当前系统时间（由 osal_GetSystemClock() 获取）。
  uint16 next;//记录最早的下一个定时器等待时间。
  uint16 wait;//当前LED的剩余等待时间。

  next = 0;
  led  = HAL_LED_1;
  leds = HAL_LED_ALL;
  sts = HalLedStatusControl.HalLedControlTable;

  /* 检查是否处于睡眠模式 */
  if (!HalLedStatusControl.sleepActive)
  {
    while (leds) // 遍历所有LED
    {
      if (leds & led)
      {
        if (sts->mode & HAL_LED_MODE_BLINK) // 只处理闪烁模式的LED，所以这个函数是只为闪烁服务的
        {
          time = osal_GetSystemClock(); // 获取当前系统时间
          if (time >= sts->next) // 检查是否需要更新状态
          {
            if (sts->mode & HAL_LED_MODE_ON) // 当前是点亮状态
            {
              pct = 100 - sts->onPct;               /* 计算熄灭时间百分比 */
              sts->mode &= ~HAL_LED_MODE_ON;        /* 标记为熄灭状态 */
              HalLedOnOff (led, HAL_LED_MODE_OFF);  /* 实际关闭LED */

              if (!(sts->mode & HAL_LED_MODE_FLASH)) // 如果不是连续闪烁
              {
                sts->todo--;                        /* 减少剩余闪烁次数 */
              }
            }            
            else if ( (!sts->todo) && !(sts->mode & HAL_LED_MODE_FLASH) ) // 闪烁完成且不是连续模式
            {
              sts->mode ^= HAL_LED_MODE_BLINK;      /* 停止闪烁 */  
            }            
            else // 需要再次点亮
            {
              pct = sts->onPct;                     /* 计算点亮时间百分比 */
              sts->mode |= HAL_LED_MODE_ON;         /* 标记为点亮状态 */
              HalLedOnOff (led, HAL_LED_MODE_ON);   /* 实际打开LED */
            }
            if (sts->mode & HAL_LED_MODE_BLINK) // 如果需要闪烁
            {
              wait = (((uint32)pct * (uint32)sts->time) / 100); // 计算等待时间
              sts->next = time + wait; // 设置下次状态变化时间
            }
            else // 闪烁结束
            {
              wait = 0; // 不需要等待
              /* 闪烁结束后，将LED恢复到闪烁前的状态 */
              HalLedSet (led, ((preBlinkState & led)!=0)?HAL_LED_MODE_ON:HAL_LED_MODE_OFF);
              /* 清除保存的位 */
              preBlinkState &= (led ^ 0xFF);
            }
          }
          else // 不需要立即更新
          {
            wait = sts->next - time;  /* 计算剩余等待时间 */
          }

          // 找出最早的等待时间，这样下次进来就是最早触发闪烁哪个先处理
          if (!next || ( wait && (wait < next) ))
          {
            next = wait;
          }
        }
        leds ^= led; // 清除已处理的LED位
      }
      led <<= 1; // 处理下一个LED
      sts++; // 移动到下一个LED的控制结构
    }

    if (next) // 如果需要等待
    {
      osal_start_timerEx(Hal_TaskID, HAL_LED_BLINK_EVENT, next);   /* 设置定时器事件 */
    }
  }
}

/***************************************************************************************************
 * @fn      HalLedOnOff
 *
 * @brief   打开或关闭指定的LED
 *
 * @param   leds - LED位掩码
 *          mode - LED_ON（开）、LED_OFF（关）
 *
 * @return  none
 ***************************************************************************************************/
void HalLedOnOff (uint8 leds, uint8 mode)
{
    // 控制LED1
    if (leds & HAL_LED_1)
    {
        if (mode == HAL_LED_MODE_ON)
        {
            HAL_TURN_ON_LED1(); // 打开LED1
        }
        else
        {
            HAL_TURN_OFF_LED1(); // 关闭LED1
        }
    }

    // 控制LED2
    if (leds & HAL_LED_2)
    {
        if (mode == HAL_LED_MODE_ON)
        {
            HAL_TURN_ON_LED2(); // 打开LED2
        }
        else
        {
            HAL_TURN_OFF_LED2(); // 关闭LED2
        }
    }

    // 控制LED3
    if (leds & HAL_LED_3)
    {
        if (mode == HAL_LED_MODE_ON)
        {
            HAL_TURN_ON_LED3(); // 打开LED3
        }
        else
        {
            HAL_TURN_OFF_LED3(); // 关闭LED3
        }
    }

    // 控制LED4
    if (leds & HAL_LED_4)
    {
        if (mode == HAL_LED_MODE_ON)
        {
            HAL_TURN_ON_LED4(); // 打开LED4
        }
        else
        {
            HAL_TURN_OFF_LED4(); // 关闭LED4
        }
    }

    /* 记录当前状态 */
    if (mode) // 如果是打开操作
    {
        HalLedState |= leds; // 设置对应位
    }
    else // 如果是关闭操作
    {
        HalLedState &= (leds ^ 0xFF); // 清除对应位
    }
}
#endif /* HAL_LED */

/***************************************************************************************************
 * @fn      HalGetLedState
 *
 * @brief   获取LED状态
 *
 * @param   none
 *
 * @return  led状态
 ***************************************************************************************************/
uint8 HalLedGetState ()
{
#if (HAL_LED == TRUE)
  return HalLedState; // 返回当前LED状态
#else
  return 0; // LED功能禁用时返回0
#endif
}

/***************************************************************************************************
 * @fn      HalLedEnterSleep
 *
 * @brief   进入睡眠模式前保存LED状态
 *
 * @param   none
 *
 * @return  none
 ***************************************************************************************************/
void HalLedEnterSleep( void )
{
#ifdef BLINK_LEDS
  /* 标记进入睡眠模式 */
  HalLedStatusControl.sleepActive = TRUE;
#endif /* BLINK_LEDS */

#if defined(HAL_LED) && (HAL_LED == TRUE)
  /* 保存每个LED的状态 */
  HalSleepLedState = 0;
  HalSleepLedState |= HAL_STATE_LED1(); // 保存LED1状态
  HalSleepLedState |= HAL_STATE_LED2() << 1; // 保存LED2状态（左移1位）
  HalSleepLedState |= HAL_STATE_LED3() << 2; // 保存LED3状态（左移2位）
  HalSleepLedState |= HAL_STATE_LED4() << 3; // 保存LED4状态（左移3位）

  /* 关闭所有LED以节省电源 */
  HalLedOnOff (HAL_LED_ALL, HAL_LED_MODE_OFF);
#endif /* HAL_LED */
}

/***************************************************************************************************
 * @fn      HalLedExitSleep
 *
 * @brief   退出睡眠模式后恢复LED状态 *
 * @param   none
 *
 * @return  none
 ***************************************************************************************************/
void HalLedExitSleep( void )
{
#if (HAL_LED == TRUE)
  /* 恢复保存的LED状态 */
  HalLedOnOff(HalSleepLedState, HAL_LED_MODE_ON);

  /* 重新启动LED处理（处理闪烁LED） */
  HalLedUpdate();
#endif /* HAL_LED */

#ifdef BLINK_LEDS
  /* 标记退出睡眠模式 */
  HalLedStatusControl.sleepActive = FALSE;
#endif /* BLINK_LEDS */
}

/***************************************************************************************************
***************************************************************************************************/

