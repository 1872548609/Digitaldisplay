/**************************************************************************************************
Filename:       iot_app.c
Revised:        $Date: 2019-8-20 15:35
Revision:       $Revision: V1.0.0

Description:    IoT应用程序主文件，包含任务初始化、按键处理和事件处理等功能

Copyright 2021 uwaycon. All rights reserved.
**************************************************-*****************^******************************/
	
#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _IOT_APP_H
/*************************************************-*************************************************
*                                               NOTE
**************************************************-*****************^******************************/
//-- source insight使用UTF-8编码，File->Reload As Encoding...->UTF-8
//-- source insight设置默认编码，Option->Preferences->Files->Default encoding->UTF-8

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/
// 头文件包含区域（实际使用时需要包含必要的头文件）

/*************************************************-*************************************************
*                                               MACRO
**************************************************-*****************^******************************/
// 宏定义区域

/*************************************************-*************************************************
*                                          CONSTANT DEFINED
**************************************************-*****************^******************************/
// 常量定义区域

/*************************************************-*************************************************
*                                           STRUCT DEFINED
**************************************************-*****************^******************************/
// 结构体定义区域

/*************************************************-*************************************************
*                                          GLOBAL VARIABLES
**************************************************-*****************^******************************/
// 全局变量定义区域

/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/
// 局部变量定义区域
uint8 iot_app_task_id;

uint8 iot_app_key_callback(uint8 cur_keys, uint8 pre_keys, uint32 poll_time_milliseconds)
{
    uint8  k;
    uint8  key_mask = HAL_KEY_1;
    uint8  scan_flag = 1;
    uint8  press_keys = 0;      // 按下的按键
    uint8  hold_keys = 0;       // 按住的按键
	uint8  release_keys = 0;    // 释放的按键
	static uint8 islongorshortpress = 0; // 长按或短按标志
	uint8_t longpress_morethan_3s_keys = 0; // 长按超过3秒的按键
	
    // 只处理有效的按键
    cur_keys &= IOT_APP_KEY_MASK;
    pre_keys &= IOT_APP_KEY_MASK;

    // 遍历所有按键
    for (k = 0; k < 8; k++,key_mask<<=1)
    {
        IOT_WATCHDOG_RESET(); // 喂狗

        // 跳过无效按键
        if (!(key_mask & IOT_APP_KEY_MASK))
        {
            continue;
        }
        
        // 按键按下处理
        if (cur_keys & key_mask)
        {
            // 短按检测
            if (hal_key_press_time_count[k] == 2)
            {
               islongorshortpress = 1;
            }
            // 长按检测
            else if (hal_key_press_time_count[k] > 2)
            {
                hold_keys |= key_mask;
            }
            // 超长按检测（>3s）
			if (hal_key_press_time_count[k] >= 30)
            {
                longpress_morethan_3s_keys |= key_mask;
				if(longpress_morethan_3s_keys!=HAL_KEY_SW6)
				{
					islongorshortpress = 2;
				}
            }
        }
        // 按键释放处理
        else
        {
			if((pre_keys & key_mask) && !(cur_keys & key_mask))
			{
				release_keys |= key_mask;
				
				// 短按处理
				if(islongorshortpress == 1)
				{
					press_keys |= key_mask;
				}
				islongorshortpress = 0;
			}
			
            // 拨动开关处理
            if ((key_mask & HAL_KEY_SW6) && (hal_key_press_time_count[k] >= 2))
            {
                press_keys |= key_mask;
            }
        }
    }
	
	
	
	return scan_flag;
}

void iot_app_init(uint8 task_id)
{
    iot_app_task_id = task_id; // 保存任务ID
			
    // 注册按键回调函数
    HalKeyCallbackRegister(iot_app_key_callback);
}

uint16 iot_app_process_event(uint8 task_id, uint16 events)
{
    (void)task_id; // 显式忽略未引用参数（避免编译器警告）
     
    // 处理系统消息事件
    if ( events & SYS_EVENT_MSG )
    {
        // 接收消息队列中的消息
        uint8* msg_packet = (uint8 *)osal_msg_receive(iot_app_task_id);
        while ( msg_packet )
        {
            // 根据消息事件类型分发处理
            //switch ( msg_packet->hdr.event )
            //{
            //case KEY_CHANGE://按键事件处理
            // //UserApp_HandleKeys( ((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
            // break;

            //default:
            // break;
            //}

            // 释放消息内存
            osal_msg_deallocate((uint8 *)msg_packet);

            // 接收下一条消息
            msg_packet = (uint8 *)osal_msg_receive(iot_app_task_id);
        }

        // 返回未处理的事件（清除已处理的SYS_EVENT_MSG）
        return (events ^ SYS_EVENT_MSG);
    }
	
	
	

	
	
	
	
	// 丢弃未知事件
    return 0;
}

/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif
