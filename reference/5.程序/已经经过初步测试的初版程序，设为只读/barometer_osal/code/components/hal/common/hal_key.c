/**************************************************************************************************
  Filename:       hal_key.c
  Revised:        $Date: 2010-09-15 19:02:45 -0700 (Wed, 15 Sep 2010) $
  Revision:       $Revision: 23815 $

  Description:    This file contains the interface to the HAL KEY Service.


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
/*********************************************************************
 NOTE: If polling is used, the hal_driver task schedules the KeyRead()
       to occur every 100ms.  This should be long enough to naturally
       debounce the keys.  The KeyRead() function remembers the key
       state of the previous poll and will only return a non-zero
       value if the key state changes.

 NOTE: If interrupts are used, the KeyRead() function is scheduled
       25ms after the interrupt occurs by the ISR.  This delay is used
       for key debouncing.  The ISR disables any further Key interrupt
       until KeyRead() is executed.  KeyRead() will re-enable Key
       interrupts after executing.  Unlike polling, when interrupts
       are enabled, the previous key state is not remembered.  This
       means that KeyRead() will return the current state of the keys
       (not a change in state of the keys).

 NOTE: If interrupts are used, the KeyRead() fucntion is scheduled by
       the ISR.  Therefore, the joystick movements will only be detected
       during a pushbutton interrupt caused by S1 or the center joystick
       pushbutton.

 NOTE: When a switch like S1 is pushed, the S1 signal goes from a normally
       high state to a low state.  This transition is typically clean.  The
       duration of the low state is around 200ms.  When the signal returns
       to the high state, there is a high likelihood of signal bounce, which
       causes a unwanted interrupts.  Normally, we would set the interrupt
       edge to falling edge to generate an interrupt when S1 is pushed, but
       because of the signal bounce, it is better to set the edge to rising
       edge to generate an interrupt when S1 is released.  The debounce logic
       can then filter out the signal bounce.  The result is that we typically
       get only 1 interrupt per button push.  This mechanism is not totally
       foolproof because occasionally, signal bound occurs during the falling
       edge as well.  A similar mechanism is used to handle the joystick
       pushbutton on the DB.  For the EB, we do not have independent control
       of the interrupt edge for the S1 and center joystick pushbutton.  As
       a result, only one or the other pushbuttons work reasonably well with
       interrupts.  The default is the make the S1 switch on the EB work more
       reliably.

*********************************************************************/

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
// 包含必要的头文件
#include "hal_mcu.h"       // 微控制器硬件抽象层
#include "hal_defs.h"      // 硬件抽象层定义
#include "hal_types.h"     // 硬件抽象层数据类型
#include "hal_board.h"     // 板级支持包
#include "hal_drivers.h"   // 硬件驱动
#include "hal_adc.h"       // ADC驱动
#include "hal_key.h"       // 键盘驱动
#include "osal.h"          // 操作系统抽象层

#include "main.h"

// 仅在HAL_KEY使能时编译以下代码，这句宏决定按键是否打开
#if defined(HAL_KEY) && (HAL_KEY == TRUE)
/*************************************************-*************************************************
*                                              MACROS
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
#define HAL_KEY_PROCESS_MAX_NUM                                     5

/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
// 定义按键边沿触发类型
//#define HAL_KEY_RISING_EDGE   0    // 上升沿触发
//#define HAL_KEY_FALLING_EDGE  1    // 下降沿触发
// 
//#define HAL_KEY_DEBOUNCE_VALUE  25 // 按键消抖延迟值(ms)
 
/* CPU端口中断定义 */
//#define HAL_KEY_CPU_PORT_0_IF P0IF // Port0中断标志
//#define HAL_KEY_CPU_PORT_2_IF P2IF // Port2中断标志
 
/* SW_6按键定义 (位于P0.1引脚) */
//#define HAL_KEY_SW_6_PORT   P0     // 端口寄存器
//#define HAL_KEY_SW_6_BIT    BV(1)  // 位掩码(第1位)
//#define HAL_KEY_SW_6_SEL    P0SEL  // 功能选择寄存器
//#define HAL_KEY_SW_6_DIR    P0DIR  // 方向寄存器
 
/* 边沿中断设置 */
//#define HAL_KEY_SW_6_EDGEBIT  BV(0) // 边沿控制位
//#define HAL_KEY_SW_6_EDGE     HAL_KEY_FALLING_EDGE // 使用下降沿触发
 
/* SW_6中断控制定义 */
//#define HAL_KEY_SW_6_IEN      IEN1   // 中断使能寄存器
//#define HAL_KEY_SW_6_IENBIT   BV(5)  // Port0中断使能位
//#define HAL_KEY_SW_6_ICTL     P0IEN  // 端口中断控制寄存器
//#define HAL_KEY_SW_6_ICTLBIT  BV(1)  // P0.1中断使能位
//#define HAL_KEY_SW_6_PXIFG    P0IFG  // 端口中断标志寄存器
 
/* 摇杆移动定义 (位于P2.0引脚) */
//#define HAL_KEY_JOY_MOVE_PORT   P2     // 端口寄存器
//#define HAL_KEY_JOY_MOVE_BIT    BV(0)  // 位掩码(第0位)
//#define HAL_KEY_JOY_MOVE_SEL    P2SEL  // 功能选择寄存器
//#define HAL_KEY_JOY_MOVE_DIR    P2DIR  // 方向寄存器
 
/* 摇杆边沿中断设置 */
//#define HAL_KEY_JOY_MOVE_EDGEBIT  BV(3) // 边沿控制位
//#define HAL_KEY_JOY_MOVE_EDGE     HAL_KEY_FALLING_EDGE // 使用下降沿触发
 
/* 摇杆中断控制定义 */
//#define HAL_KEY_JOY_MOVE_IEN      IEN2   // 中断使能寄存器
//#define HAL_KEY_JOY_MOVE_IENBIT   BV(1)  // Port2中断使能位
//#define HAL_KEY_JOY_MOVE_ICTL     P2IEN  // 端口中断控制寄存器
//#define HAL_KEY_JOY_MOVE_ICTLBIT  BV(0)  // P2.0中断使能位
//#define HAL_KEY_JOY_MOVE_PXIFG    P2IFG  // 端口中断标志寄存器
 
/* 摇杆ADC通道定义 */
//#define HAL_KEY_JOY_CHN   HAL_ADC_CHANNEL_6 // 使用ADC通道6
 


/*************************************************-*************************************************
*                                             TYPEDEFS
**************************************************-*****************^******************************/
// 类型定义区域（当前为空）

/*************************************************-*************************************************
*                                         LOCAL VARIABLES
**************************************************-*****************^******************************/
static uint8 hal_key_save_keys[HAL_KEY_PROCESS_MAX_NUM];                  // 保存上一次按键状态（轮询模式使用）
static hal_key_callback_t p_hal_key_process_callback[HAL_KEY_PROCESS_MAX_NUM];   // 按键回调函数指针
#ifdef HAL_KEY_EVENT_TIMEOUT
uint32 hal_key_scan_timeout = HAL_KEY_EVENT_TIMEOUT;
#else
uint32 hal_key_scan_timeout = 100;
#endif

/*************************************************-*************************************************
*                                         GLOBAL VARIABLES
**************************************************-*****************^******************************/
uint16 hal_key_press_time_count[8];
uint16 hal_key_release_time_count[8];

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/


/**************************************************************************************************
 * @fn      HalKeyInit
 * @brief   初始化按键服务
 * @param   none
 * @return  None
 **************************************************************************************************/
void HalKeyInit( void )
{
    uint8 i;
    for (i = 0; i < HAL_KEY_PROCESS_MAX_NUM; i++)
    {
        /* 初始化按键状态为0 */
        hal_key_save_keys[i] = 0;
    
        /* 初始化回调函数为 */
        p_hal_key_process_callback[i] = NULL;
    }

    for (i = 0; i < 8; i++)
    {
        hal_key_press_time_count[i] = 0;
        hal_key_release_time_count[i] = 0;
    }
    
    #ifdef HAL_KEY1_BIT
    HAL_KEY1_INIT();
    #endif

    #ifdef HAL_KEY2_BIT
    HAL_KEY2_INIT();
    #endif

    #ifdef HAL_KEY3_BIT
    HAL_KEY3_INIT();
    #endif

    #ifdef HAL_KEY4_BIT
    HAL_KEY4_INIT();
    #endif

    #ifdef HAL_KEY5_BIT
    HAL_KEY5_INIT();
    #endif

    #ifdef HAL_KEY6_BIT
    HAL_KEY6_INIT();
    #endif

    #ifdef HAL_KEY7_BIT
    HAL_KEY7_INIT();
    #endif

    #ifdef HAL_KEY8_BIT
    HAL_KEY8_INIT();
    #endif
}


uint8 HalKeyCallbackRegister(hal_key_callback_t cback)
{
    uint8 i = 0;
    for (i = 0; i < HAL_KEY_PROCESS_MAX_NUM; i++)
    {
        if (p_hal_key_process_callback[i])
        {
            continue;
        }

        p_hal_key_process_callback[i] = cback;
        break;
    }

    if (i < HAL_KEY_PROCESS_MAX_NUM)
    {
        /* 启动轮询事件 */
        osal_set_event(Hal_TaskID, HAL_KEY_EVENT);
        return 1;
    }
    else
    {
        return 0;
    }
}


/**************************************************************************************************
 * @fn      HalKeyRead
 * @brief   读取当前按键状态
 * @param   None
 * @return  keys - 当前按键状态
 **************************************************************************************************/
uint8 HalKeyRead ( void )
{
    uint8 keys = 0;
 
    #ifdef HAL_KEY1_BIT
    if (HAL_KEY1_READ())
    {
        keys |= HAL_KEY_1;
    }
    #endif

    #ifdef HAL_KEY2_BIT
    if (HAL_KEY2_READ())
    {
        keys |= HAL_KEY_2;
    }
    #endif

    #ifdef HAL_KEY3_BIT
    if (HAL_KEY3_READ())
    {
        keys |= HAL_KEY_3;
    }
    #endif

    #ifdef HAL_KEY4_BIT
    if (HAL_KEY4_READ())
    {
        keys |= HAL_KEY_4;
    }
    #endif

    #ifdef HAL_KEY5_BIT
    if (HAL_KEY5_READ())
    {
        keys |= HAL_KEY_5;
    }
    #endif

    #ifdef HAL_KEY6_BIT
    if (HAL_KEY6_READ())
    {
        keys |= HAL_KEY_6;
    }
    #endif

    #ifdef HAL_KEY7_BIT
    if (HAL_KEY7_READ())
    {
        keys |= HAL_KEY_7;
    }
    #endif
 
    return keys;
}


/**************************************************************************************************
 * @fn      HalKeyPoll
 * @brief   由hal_driver调用以轮询按键
 * @param   None
 * @return  None
 **************************************************************************************************/
void HalKeyPoll (void)
{
    uint8 pre_keys = 0;
    uint8 keys = 0;
    uint8 scan_flag = 0;

    #ifdef HAL_KEY1_BIT
    if (HAL_KEY1_READ())
    {
        keys |= HAL_KEY_1;
    }
    #endif

    #ifdef HAL_KEY2_BIT
    if (HAL_KEY2_READ())
    {
        keys |= HAL_KEY_2;
    }
    #endif

    #ifdef HAL_KEY3_BIT
    if (HAL_KEY3_READ())
    {
        keys |= HAL_KEY_3;
    }
    #endif

    #ifdef HAL_KEY4_BIT
    if (HAL_KEY4_READ())
    {
        keys |= HAL_KEY_4;
    }
    #endif

    #ifdef HAL_KEY5_BIT
    if (HAL_KEY5_READ())
    {
        keys |= HAL_KEY_5;
    }
    #endif

    #ifdef HAL_KEY6_BIT
    if (HAL_KEY6_READ())
    {
        keys |= HAL_KEY_6;
    }
    #endif

    #ifdef HAL_KEY7_BIT
    if (HAL_KEY7_READ())
    {
        keys |= HAL_KEY_7;
    }
    #endif

    uint8 key_mask = HAL_KEY_1;
    for (uint8 k = 0; k < 8; k++,key_mask<<=1)
    {
        IOT_WATCHDOG_RESET();
        
        if (keys & key_mask)//-- 按键按下
        {
            if (hal_key_press_time_count[k] < 0xffff)
            {
                hal_key_press_time_count[k]++;//-- 每加1表示一个轮询，时间为：次数 x poll_time_milliseconds
            }
        }
    }

    for (uint8 i = 0; i < HAL_KEY_PROCESS_MAX_NUM; i++)
    {
        if (p_hal_key_process_callback[i] == NULL)
        {
            continue;
        }

        pre_keys = hal_key_save_keys[i];
        hal_key_save_keys[i] = keys;

        uint8 scan_flag_temp = p_hal_key_process_callback[i](keys, pre_keys, hal_key_scan_timeout);
        if (!scan_flag_temp)
        {
            hal_key_save_keys[i] = 0;
        }

        scan_flag |= scan_flag_temp;
    }

    key_mask = HAL_KEY_1;
    for (uint8 k = 0; k < 8; k++,key_mask<<=1)
    {
        IOT_WATCHDOG_RESET();
        
        if (!(keys & key_mask))//-- 按键松开
        {
            hal_key_press_time_count[k] = 0;
        }
    }

    for (uint8 i = 0; i < 8; i++)
    {
        if(hal_key_press_time_count[i] == 0)    //-- 按键已经松开，需要在应用层处理此变量
        {
            if(hal_key_release_time_count[i] < 0xffff)
            {
                hal_key_release_time_count[i]++;//-- 每加1就是一个KEY的轮询时间
            }
        }
        else
        {
            hal_key_release_time_count[i]=0;
        }
    }
    
    if (scan_flag)
    {
        osal_start_timerEx(Hal_TaskID, HAL_KEY_EVENT, hal_key_scan_timeout);
    }
    else
    {
        osal_stop_timerEx(Hal_TaskID, HAL_KEY_EVENT);
        osal_clear_event( Hal_TaskID, HAL_KEY_EVENT);
    }
}
 
/**************************************************************************************************
 * @fn      halGetJoyKeyInput
 * @brief   将ADC值映射到对应的摇杆按键
 * @param   None
 * @return  keys - 当前摇杆按键状态
 **************************************************************************************************/
uint8 halGetJoyKeyInput(void)
{
    /* 摇杆控制编码为模拟电压值
    * 读取JOY_LEVEL模拟值并映射到摇杆移动方向
    */
    uint8 adc;
    uint8 ksave0 = 0;
    uint8 ksave1;

    /* 持续读取ADC值，直到两次连续读取结果相同（消抖） */
    do
    {
        ksave1 = ksave0;    /* 保存上一次读取结果 */

        /* 读取ADC值（8位分辨率） */
        //adc = HalAdcRead (HAL_KEY_JOY_CHN, HAL_ADC_RESOLUTION_8);

        /* 根据ADC值范围判断按键方向 */
        if ((adc >= 2) && (adc <= 38))
        {
            ksave0 |= HAL_KEY_UP;      // 上
        }
        else if ((adc >= 74) && (adc <= 88))
        {
            ksave0 |= HAL_KEY_RIGHT;   // 右
        }
        else if ((adc >= 60) && (adc <= 73))
        {
            ksave0 |= HAL_KEY_LEFT;    // 左
        }
        else if ((adc >= 39) && (adc <= 59))
        {
            ksave0 |= HAL_KEY_DOWN;    // 下
        }
        else if ((adc >= 89) && (adc <= 100))
        {
            ksave0 |= HAL_KEY_CENTER;  // 中间
        }
    } while (ksave0 != ksave1);    // 直到两次读取结果相同

    return ksave0;
}
 
/**************************************************************************************************
 * @fn      halProcessKeyInterrupt
 * @brief   检查有效按键中断，保存中断驱动的按键状态，并通过消抖延迟处理
 * @param   None
 * @return  None
 **************************************************************************************************/
void halProcessKeyInterrupt (void)
{
//  bool valid=FALSE;
// 
//  /* 检查SW_6按键中断标志 */
//  if (HAL_KEY_SW_6_PXIFG & HAL_KEY_SW_6_BIT)  /* 中断标志已设置 */
//  {
//    HAL_KEY_SW_6_PXIFG = ~(HAL_KEY_SW_6_BIT); /* 清除中断标志 */
//    valid = TRUE;
//  }
// 
//  /* 检查摇杆移动中断标志 */
//  if (HAL_KEY_JOY_MOVE_PXIFG & HAL_KEY_JOY_MOVE_BIT)  /* 中断标志已设置 */
//  {
//    HAL_KEY_JOY_MOVE_PXIFG = ~(HAL_KEY_JOY_MOVE_BIT); /* 清除中断标志 */
//    valid = TRUE;
//  }
// 
//  /* 如果有有效中断，启动消抖定时器 */
//  if (valid)
//  {
//    osal_start_timerEx (Hal_TaskID, HAL_KEY_EVENT, HAL_KEY_DEBOUNCE_VALUE);
//  }
}
 
/**************************************************************************************************
 * @fn      HalKeyEnterSleep
 * @brief   进入睡眠模式前调用
 * @param   None
 * @return  None
 **************************************************************************************************/
void HalKeyEnterSleep ( void )
{
    /* 当前为空实现 */
}
 
/**************************************************************************************************
 * @fn      HalKeyExitSleep
 * @brief   睡眠结束后调用
 * @param   None
 * @return  - 返回保存的按键状态
 **************************************************************************************************/
uint8 HalKeyExitSleep ( void )
{
    /* 唤醒并读取按键状态 */
    return ( HalKeyRead () );
}
 
/***************************************************************************************************
 *                                    INTERRUPT SERVICE ROUTINE
 ***************************************************************************************************/
 
/**************************************************************************************************
 * @fn      halKeyPort0Isr
 * @brief   Port0中断服务程序
 * @param   None
 * @return  None
 **************************************************************************************************/
//HAL_ISR_FUNCTION( halKeyPort0Isr, P0INT_VECTOR )
//{
//  HAL_ENTER_ISR();  // 进入中断
// 
//  /* 检查SW_6按键中断 */
//  if (HAL_KEY_SW_6_PXIFG & HAL_KEY_SW_6_BIT)
//  {
//    halProcessKeyInterrupt(); // 处理按键中断
//  }
// 
//  /* 清除Port0的CPU中断标志
//   * 注意：必须先清除PxIFG再清除PxIF
//   */
//  HAL_KEY_SW_6_PXIFG = 0;
//  HAL_KEY_CPU_PORT_0_IF = 0;
//  
//  CLEAR_SLEEP_MODE(); // 清除睡眠模式
//  HAL_EXIT_ISR();     // 退出中断
//}
 
/**************************************************************************************************
 * @fn      halKeyPort2Isr
 * @brief   Port2中断服务程序
 * @param   None
 * @return  None
 **************************************************************************************************/
//HAL_ISR_FUNCTION( halKeyPort2Isr, P2INT_VECTOR )
//{
//  HAL_ENTER_ISR();  // 进入中断
//  
//  /* 检查摇杆移动中断 */
//  if (HAL_KEY_JOY_MOVE_PXIFG & HAL_KEY_JOY_MOVE_BIT)
//  {
//    halProcessKeyInterrupt(); // 处理按键中断
//  }
// 
//  /* 清除Port2的CPU中断标志
//   * 注意：必须先清除PxIFG再清除PxIF
//   * 备注：P2_1和P2_2是调试线
//   */
//  HAL_KEY_JOY_MOVE_PXIFG = 0;
//  HAL_KEY_CPU_PORT_2_IF = 0;
// 
//  CLEAR_SLEEP_MODE(); // 清除睡眠模式
//  HAL_EXIT_ISR();     // 退出中断
//}
 
#else /* HAL_KEY未使能时的空实现 */
 
void HalKeyInit(void){}
void HalKeyConfig(bool interruptEnable, hal_key_callback_t cback){}
uint8 HalKeyRead(void){ return 0;}
void HalKeyPoll(void){}
 
#endif /* HAL_KEY */





/**************************************************************************************************
**************************************************************************************************/



