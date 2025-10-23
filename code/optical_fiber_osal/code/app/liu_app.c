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

#ifdef _LIU_APP_H
	
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
*                                         FUNCTION DECLARATION
**************************************************-*****************^******************************/
// 函数声明
uint8 liu_app_key_callback(uint8 cur_keys, uint8 pre_keys, uint32 poll_time_milliseconds);


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
uint8 liu_app_task_id;
Zero_Calibration_typedef Zero_Calibration_struct;


/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/
// 局部变量定义区








/******************************************************************************
函 数 名： void Zero_Calibration_Init(Zero_Calibration_typedef *pHandle)
函数功能： 校零结构体参数初始化
入口参数：  
Zero_Calibration_typedef *pHandle ： 校零结构体指针
返    回： 无 
********************************************************************************/
void Zero_Calibration_Init(Zero_Calibration_typedef *pHandle)
{
	pHandle->Zero_Calibration_500msTimeSwitch = 0;
	pHandle->Zero_Calibration_bit = 0;
	pHandle->Zero_calibration_pressure = 0;
}




/******************************************************************************
函 数 名： void Zero_Calibration_Task(Zero_Calibration_typedef *pHandle)
函数功能： 校零(压力孔处于大气压下时，压力值被强制显示为“0”)
入口参数：  
Zero_Calibration_typedef *pHandle ： 校零结构体指针
返    回： 无 
********************************************************************************/
void Zero_Calibration_Task(Zero_Calibration_typedef *pHandle)
{
	if( pHandle->Zero_Calibration_bit == 1 )
	{
		if( pHandle->Zero_Calibration_500msTimeSwitch == 0 )
		{
			//将当前气压更新为基准气压
			pHandle->Zero_calibration_pressure = Current_pressure_value;		
		}
		
		if( ++pHandle->Zero_Calibration_500msTimeSwitch < 100 )
		{
			DIV_Disp_ClearAllPoint(MainScreen);
			main_screen_disp("%0.4d",0);
		}
		else
		{
			pHandle->Zero_Calibration_500msTimeSwitch = 0;				
			pHandle->Zero_Calibration_bit = 0;	

		}	
	}
}








/******************************************************************************
函 数 名： void liu_app_init(uint8 task_id)
函数功能： 任务初始化
入口参数：  
task_id ： 任务ID
返    回： 无 
********************************************************************************/
void liu_app_init(uint8 task_id)
{
    liu_app_task_id = task_id; // 保存任务ID
			
	// 注册事件
	osal_start_reload_timer(liu_app_task_id,LIU_APP_TIMER_EVT,LIU_APP_TIMER_INTERVAL);
	
    // 注册按键回调函数
    HalKeyCallbackRegister(liu_app_key_callback);
	
	//校零结构体参数初始化
	Zero_Calibration_Init(&Zero_Calibration_struct);
}




/******************************************************************************
函 数 名： uint16 liu_app_process_event(uint8 task_id, uint16 events)
函数功能： 任务处理
入口参数：  
task_id ： 任务ID
events  ： 
返    回：  
********************************************************************************/
uint16 liu_app_process_event(uint8 task_id, uint16 events)
{
    (void)task_id; // 显式忽略未引用参数（避免编译器警告）
     
    // 处理系统消息事件
    if ( events & SYS_EVENT_MSG )
    {
        // 接收消息队列中的消息
        uint8* msg_packet = (uint8 *)osal_msg_receive(liu_app_task_id);
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
            msg_packet = (uint8 *)osal_msg_receive(liu_app_task_id);
        }

        // 返回未处理的事件（清除已处理的SYS_EVENT_MSG）
        return (events ^ SYS_EVENT_MSG);
    }
	
	//判断他是否成功调度起来了，添加成功就会进你的事件管理
	if(events & LIU_APP_TIMER_EVT)
	{
		

		Zero_Calibration_Task(&Zero_Calibration_struct);	
		
		
		
		
		// 事件到了就执行
		//osal_stop_timerEx(liu_app_task_id,LIU_APP_TIMER_EVT);
		
		return (events ^ LIU_APP_TIMER_EVT);
	}
		
	// 丢弃未知事件
    return 0;
}




/*==============================按键处理==============================*/
void ShortPress_MODE_Key(void)	
{
	
}

void ShortPress_ADD_Key(void)	
{

}

void ShortPress_SUB_Key(void)	
{

}

void LongPress_MODE_Key_3s(void)	
{
	
}

void LongPress_MODE_Key_4s(void)	
{

}

void LongPress_ADD_SUB_Key_2s(void)	
{
	if(system_state == RUN_STATE)	
	{
		Zero_Calibration_struct.Zero_Calibration_bit = 1;
		Zero_Calibration_struct.Zero_Calibration_500msTimeSwitch = 0;	
	}
}




/******************************************************************************
函 数 名: uint8 liu_app_key_callback(uint8 cur_keys, uint8 pre_keys, uint32 poll_time_milliseconds)
函数功能: 按键按下检测
入口参数:  
cur_keys：
pre_keys:
poll_time_milliseconds：
返    回:  
********************************************************************************/
uint8 liu_app_key_callback(uint8 cur_keys, uint8 pre_keys, uint32 poll_time_milliseconds)
{	
    uint8  k;
    uint8  key_mask = HAL_KEY_1;
    uint8  scan_flag = 1;
    uint8  press_keys = 0;      // 按下的按键
    //uint8  hold_keys = 0;      // 按住的按键
	uint8  release_keys = 0;    // 释放的按键
	static uint8 islongorshortpress = 0; // 长按或短按标志
	uint8_t longpress_morethan_3s_keys = 0; // 长按超过3秒的按键
	uint8_t longpress_morethan_4s_keys = 0; // 长按超过4秒的按键
	static uint8_t ADD_SUB_longpress_morethan_2s_keys = 0;	//同时按住上下键并保持2秒
	
    // 只处理有效的按键
    cur_keys &= LIU_APP_KEY_MASK;	//（0000 0111）
    pre_keys &= LIU_APP_KEY_MASK;

    // 遍历所有按键
    for (k = 0; k < 8; k++,key_mask<<=1)
    {
        IOT_WATCHDOG_RESET(); // 喂狗

        // 跳过无效按键
        if (!(key_mask & LIU_APP_KEY_MASK))
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
            // 超长按检测（>3s）
			if (hal_key_press_time_count[k] == 30)
            {
				islongorshortpress = 2;
                longpress_morethan_3s_keys |= key_mask;
            }
            // 超长按检测（>4s）
			if (hal_key_press_time_count[k] == 50)
            {
				islongorshortpress = 2;
                longpress_morethan_4s_keys |= key_mask;
            }
			
			if (hal_key_press_time_count[k] == 20)
			{
				ADD_SUB_longpress_morethan_2s_keys |= key_mask;
			}
        }
        else
        {// 按键释放处理
			if((pre_keys & key_mask) && !(cur_keys & key_mask))
			{
				
//				if( (key_mask == 0x02) || (key_mask == 0x04) )
//				{
//					ADD_SUB_longpress_morethan_2s_keys = 0;
//				}
				
				// 短按处理
				if(islongorshortpress == 1)
				{
					press_keys |= key_mask;
				}
				
				islongorshortpress = 0;
				
				release_keys |= key_mask;
			}
        }
    }
	
	//短按
	if(press_keys & HAL_KEY_MODE)
	{
		ShortPress_MODE_Key();
	}
	
	if(press_keys & HAL_KEY_LEFT_ADD)
	{
		ShortPress_ADD_Key();
	}
	
	if(press_keys & HAL_KEY_RIGHT_SUB)
	{
		ShortPress_SUB_Key();
	}
	
	
	//长按
	if( (longpress_morethan_3s_keys & HAL_KEY_MODE)      
		&& (!(longpress_morethan_3s_keys & HAL_KEY_LEFT_ADD))  
		&& (!(longpress_morethan_3s_keys & HAL_KEY_RIGHT_SUB)) )
	{
		LongPress_MODE_Key_3s();
	}
	
	
	
	if( (longpress_morethan_4s_keys & HAL_KEY_MODE)      
		&& (!(longpress_morethan_4s_keys & HAL_KEY_LEFT_ADD))  
		&& (!(longpress_morethan_4s_keys & HAL_KEY_RIGHT_SUB)) )
	{
		LongPress_MODE_Key_4s();
	}
	
	
	
	//多键同时长按
	if( (ADD_SUB_longpress_morethan_2s_keys & HAL_KEY_LEFT_ADD) 
		&& (ADD_SUB_longpress_morethan_2s_keys & HAL_KEY_RIGHT_SUB)
		&& (!(ADD_SUB_longpress_morethan_2s_keys & HAL_KEY_MODE)) )
	{
		ADD_SUB_longpress_morethan_2s_keys = 0;
		
		LongPress_ADD_SUB_Key_2s();
	}
	
	return scan_flag;
}




/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif
