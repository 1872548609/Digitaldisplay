/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "hal_drivers.h"     // 硬件驱动接口

/**************************************************************************************************
 *                                            MACROS
 **************************************************************************************************/
// （无宏定义）

/**************************************************************************************************
 *                                          CONSTANTS
 **************************************************************************************************/
// （无常量定义）

/**************************************************************************************************
 *                                          TYPEDEFS
 **************************************************************************************************/
// （无类型定义）

/**************************************************************************************************
 *                                      GLOBAL VARIABLES
 **************************************************************************************************/
uint8 Hal_TaskID;  // HAL任务ID，用于OSAL消息处理

extern void HalLedUpdate(void); // LED状态更新函数（内部使用，不暴露在头文件）

/**************************************************************************************************
 *                                      FUNCTIONS - API
 **************************************************************************************************/

/**************************************************************************************************
 * @fn      Hal_Init
 * @brief   HAL初始化函数
 * @param   task_id - HAL任务ID
 * @return  None
 **************************************************************************************************/
void Hal_Init(uint8 task_id)
{
    /* 注册任务ID */
    Hal_TaskID = task_id;

    #ifdef CC2591_COMPRESSION_WORKAROUND  
    /* 启动周期性RSSI重置定时器（特定场景） */
    osal_start_reload_timer(Hal_TaskID, PERIOD_RSSI_RESET_EVT, PERIOD_RSSI_RESET_TIMEOUT);
    #endif  
}

/**************************************************************************************************
 * @fn      HalDriverInit
 * @brief   初始化硬件驱动（需优先初始化）
 * @param   None
 * @return  None
 **************************************************************************************************/
void HalDriverInit(void)
{
    /* 定时器初始化（已注释掉，提示模块被移除） */
    #if (defined HAL_TIMER) && (HAL_TIMER == TRUE)
    #error "The hal timer driver module is removed."
    #endif

    /* ADC初始化 */
    #if (defined HAL_ADC) && (HAL_ADC == TRUE)
    HAL_ADC_Init();
    #endif

    /* DMA初始化（可选） */
    #if (defined HAL_DMA) && (HAL_DMA == TRUE)
    HalDmaInit(); // 必须在其他使用DMA的模块前调用
    #endif

    /* AES初始化（可选） */
    #if (defined HAL_AES) && (HAL_AES == TRUE)
    HalAesInit();
    #endif

    /* LCD初始化 */
    #if (defined HAL_LCD) && (HAL_LCD == TRUE)
    HalLcdInit();
    #endif

    /* LED初始化 */
    #if (defined HAL_LED) && (HAL_LED == TRUE)
    HalLedInit();
    #endif

    /* UART初始化 */
    #if (defined HAL_UART) && (HAL_UART == TRUE)
    HalUARTInit();
    #endif

    /* 按键初始化 */
    #if (defined HAL_KEY) && (HAL_KEY == TRUE)
    HalKeyInit();
    #endif
	
	/* 屏幕初始化 */
	#if (defined HAL_DIV) && (HAL_DIV == TRUE)
	DIV_Disp_Init();
	#endif
	
	/*输出初始化*/
	#if (defined HAL_OUT) && (HAL_OUT == TRUE)
	Hal_Out_Init();
	#endif
	
		
    /* SPI初始化（可选） */
    #if (defined HAL_SPI) && (HAL_SPI == TRUE)
    HalSpiInit();
    #endif

    /* USB HID初始化（可选） */
    #if (defined HAL_HID) && (HAL_HID == TRUE)
    usbHidInit();
    #endif

    #if defined(HAL_NIXIETTUBE_DISPLAY_H)
    iot_nixie_init();
    #endif
}

/**************************************************************************************************
 * @fn      Hal_ProcessEvent
 * @brief   HAL事件处理函数
 * @param   task_id - HAL任务ID
 *          events - 待处理的事件标志
 * @return  未处理的事件标志
 **************************************************************************************************/
uint16 Hal_ProcessEvent(uint8 task_id, uint16 events)
{
    uint8 *msgPtr;

    (void)task_id;  // 显式忽略未使用的参数

    /* 处理OSAL消息事件 */
    if (events & SYS_EVENT_MSG)
    {
        msgPtr = osal_msg_receive(Hal_TaskID); // 接收消息
        while (msgPtr)
        {
            /* 默认处理：直接释放消息内存 */
            osal_msg_deallocate(msgPtr);
            /* 继续处理下一条消息 */
            msgPtr = osal_msg_receive(Hal_TaskID);
        }
        return events ^ SYS_EVENT_MSG; // 清除已处理的事件标志
    }

    /* 处理LED闪烁事件 */
    if (events & HAL_LED_BLINK_EVENT)
    {
        #if (defined (BLINK_LEDS)) && (HAL_LED == TRUE)
        HalLedUpdate(); // 更新LED状态
        #endif /* BLINK_LEDS && HAL_LED */
        return events ^ HAL_LED_BLINK_EVENT;
    }

    /* 处理按键事件 */
    if (events & HAL_KEY_EVENT)
    {
        #if (defined HAL_KEY) && (HAL_KEY == TRUE)
        HalKeyPoll(); // 轮询按键状态
        #endif // HAL_KEY
        return events ^ HAL_KEY_EVENT;
    }

    /* 睡眠定时器事件（节能模式） */
    #ifdef POWER_SAVING
    if (events & HAL_SLEEP_TIMER_EVENT)
    {
        halRestoreSleepLevel(); // 恢复睡眠等级
        return events ^ HAL_SLEEP_TIMER_EVENT;
    }
    #endif
  
    /* 未处理的事件，直接丢弃 */
    return 0;
}

/**************************************************************************************************
 * @fn      Hal_ProcessPoll
 * @brief   供OSAL调用的轮询函数（处理UART、定时器等）
 * @param   task_id - HAL任务ID
 * @return  None
 **************************************************************************************************/
void Hal_ProcessPoll(void)
{
    /* 定时器轮询（已注释掉，提示模块被移除） */
    #if (defined HAL_TIMER) && (HAL_TIMER == TRUE)
    #error "The hal timer driver module is removed."
    #endif

    /* UART轮询 */
    #if (defined HAL_UART) && (HAL_UART == TRUE)
    HalUARTPoll();
    #endif  

    /* SPI轮询（可选） */
    #if (defined HAL_SPI) && (HAL_SPI == TRUE)
    HalSpiPoll();
    #endif

    /* USB HID事件处理（可选） */
    #if (defined HAL_HID) && (HAL_HID == TRUE)
    usbHidProcessEvents();
    #endif

    #if defined(POWER_SAVING)
    /* 允许进入睡眠模式（节能） */
    ALLOW_SLEEP_MODE();
    #endif
}
