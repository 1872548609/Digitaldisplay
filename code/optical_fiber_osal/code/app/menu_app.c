/**************************************************************************************************
Filename:       iot_app.c
Revised:        $Date: 2019-8-20 15:35
Revision:       $Revision: V1.0.0

Description:

Copyright 2021 uwaycon. All rights reserved.
                                                
**************************************************-*****************^******************************/
#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _MENU_APP_H
/*************************************************-*************************************************
*                                               NOTE
**************************************************-*****************^******************************/
//-- source insight use UTF-8 to encode, File->Reload As Encoding...->UTF-8
//-- source insight set default encode, Option->Preferences->Files->Default encoding->UTF-8

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                               MACRO
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          CONSTANT DEFINED
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                           STRUCT DEFINED
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          GLOBAL VARIABLES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/
uint8 menu_app_task_id;   // 任务ID，由OSAL分配，用于内部任务/事件处理
const int8_t boot_animation_string[] = "8888A002"; // 开机文字

uint8 keycount = 0;



/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/

uint8 menu_app_key_callback(uint8 cur_keys, uint8 pre_keys, uint32 poll_time_milliseconds)
{
	uint8  k;
    uint8  key_mask = HAL_KEY_1;
    uint8  scan_flag = 1;
    uint8  press_keys = 0;      // 按下的按键
    uint8  hold_keys = 0;       // 按住的按键
	uint8  release_keys = 0;    // 释放的按键
	static uint8 islongorshortpress = 0; // 长按或短按标志
	uint8_t longpress_morethan_3s_keys = 0; // 长按超过3秒的按键
	
    cur_keys &= MENU_APP_KEY_MASK;
    pre_keys &= MENU_APP_KEY_MASK;

    for (k = 0; k < 8; k++,key_mask<<=1)
    {
        IOT_WATCHDOG_RESET();

        if (!(key_mask & MENU_APP_KEY_MASK))
        {
            continue;
        }
        
        if (cur_keys & key_mask)//-- 按键按下
        {
            if (hal_key_press_time_count[k] >= 2)
            {
                //press_keys |= key_mask;
				islongorshortpress = 1;
            }
            else if (hal_key_press_time_count[k] > 2)
            {
                hold_keys |= key_mask;
            }
			if (hal_key_press_time_count[k] >= 30)	// 大于3s触发按键
            {
                longpress_morethan_3s_keys |= key_mask;
				if(longpress_morethan_3s_keys!=HAL_KEY_SW6)
				{
					islongorshortpress = 2;
				}
				
            }
        }
        else
        {
			if((pre_keys & key_mask) && !(cur_keys & key_mask))
			{
				// 短按处理
				if(islongorshortpress == 1)
				{
					press_keys |= key_mask;
				}
				islongorshortpress = 0;
			}
			
            if ((key_mask & HAL_KEY_SW6) && (hal_key_press_time_count[k] >= 2))
            {
                //-- 拨动开关，不管打哪一边都需要有检测动作
                press_keys |= key_mask;
            }
        }
		
    }
		
	
	
    return scan_flag;
}


// 菜单回调函数
void onItem1Click(MenuItem* item) {
   
	MenuItem* tempitem = item;
	
	
	
}


void menu_app_init(uint8 task_id)
{
	menu_app_task_id = task_id;
	
//	MenuItem* root = CreateTestMenu(); // 动态创建菜单，所有菜单都在这个函数里编辑好
//    MenuSystem_Init(root);	// 初始化系统菜单
	
	
    HalKeyCallbackRegister(menu_app_key_callback);
}

uint16 menu_app_process_event(uint8 task_id, uint16 events)
{
    (void)task_id; // 显式忽略未引用参数（避免编译器警告）
    
    // 处理系统消息事件
    if ( events & SYS_EVENT_MSG )
    {
        // 接收消息队列中的消息
        uint8* msg_packet = (uint8 *)osal_msg_receive(menu_app_task_id);
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
            msg_packet = (uint8 *)osal_msg_receive(menu_app_task_id);
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

