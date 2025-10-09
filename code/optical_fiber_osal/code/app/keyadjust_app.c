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

#ifdef _KEYADJUST_APP_H
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
uint8 keyadjust_app_task_id;   // 任务ID，由OSAL分配，用于内部任务/事件处理


/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/



/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/
#define MAXSETVALUE 9999

// 长按控制结构体
typedef struct {
    uint32 press_time;       	// 按键按下时间戳
    uint8 current_stage;    	// 当前长按阶段
    uint8 speed_value;       	// 当前速度值
	uint8 add_interval;			// 增量
    bool stage_triggered;   	// 阶段触发标志
} LongPressCtrl;

static LongPressCtrl mode_key_ctrl = {0};

uint32 press_duration = 0;

uint8 addorsub = 0;

uint32 addintervalcount = 0;

uint32 addattsetcount = 0;

uint8 addchoice =0 ;// 0 设置阈值 1 设置att 2 设置sta
 
// 定义时间分段阈值（单位：秒）
static const uint32 STAGE_THRESHOLDS[] = {2000, 2000, 2000, 2000};// 按下时间						

static const uint32 SPEED_INCREMENTS[] = {500, 50, 20, 1}; // 各阶段速度增量			

static const uint32 ADD_INCREMENTS[] = {1, 1, 1, 1}; // 各阶段增量			




uint8 keyadjust_app_key_callback(uint8 cur_keys, uint8 pre_keys, uint32 poll_time_milliseconds)
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
			if (hal_key_press_time_count[k] >= 10)	// 大于1s触发按键
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
				release_keys |= key_mask;
				
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
	
	if(systemstate == 2)
	{
		if(strcmp(menuSystem.current->text," Att set") == 0)
		{
			addchoice = 1;
		}
		if(strcmp(menuSystem.current->text," STA set") == 0)
		{
			addchoice = 2;
		}
		// ===================================================================
		// 短按
		// ===================================================================
		if (press_keys & HAL_KEY_RIGHT_SUB)
		{
			//-- TODO
			if(strcmp(menuSystem.current->text," Att set") == 0)
			{
				if(!Leftandright_key_setting_mode)// 设置阈值
				{
					if(iot_app_optical_current_att>0)
					{
						iot_app_optical_current_att--;
					}
					else
					{
						iot_app_optical_current_att = 0;
					}
					iot_app_optical_auto_adjust_att = iot_app_optical_current_att;
				}
	
				// 记录att
				Flash_Write_SetValue();
			}
			
			if(strcmp(menuSystem.current->text," STA set") == 0)
			{
				if(iot_app_optical_current_sta>10)
				{
					iot_app_optical_current_sta--;
				}
				else
				{
					iot_app_optical_current_sta = 10;
				}
	
				// 记录sta
				Flash_Write_SetValue();
			}
			
		}

		if (press_keys & HAL_KEY_LEFT_ADD)
		{
			//-- TODO
			if(strcmp(menuSystem.current->text," Att set") == 0)
			{
				if(!Leftandright_key_setting_mode)// 设置阈值
				{
					iot_app_optical_current_att++;
					
					if(iot_app_optical_current_att > 100)
					{
						iot_app_optical_current_att = 100;
					}

					iot_app_optical_auto_adjust_att = iot_app_optical_current_att;
				}
				
				// 记录att
				Flash_Write_SetValue();
			}
			
			if(strcmp(menuSystem.current->text," STA set") == 0)
			{
					
				iot_app_optical_current_sta++;
				
				if(iot_app_optical_current_sta > 9999)
				{
					iot_app_optical_current_sta = 9999;
				}
				
				// 记录sta
				Flash_Write_SetValue();
			}
		}
		
		// ===================================================================
		// 长按
		// ===================================================================	
		
		// 恢复出厂设置有关===================================START
		if(longpress_morethan_3s_keys & HAL_KEY_SET && (longpress_morethan_3s_keys & HAL_KEY_MODE))
		{
			systemstate = 3;
			
			return scan_flag;
		}
		// 恢复出厂设置有关===================================END
		
		if(longpress_morethan_3s_keys &&islongorshortpress==2)
		{
			if (longpress_morethan_3s_keys & HAL_KEY_RIGHT_SUB)
			{
				if(strcmp(menuSystem.current->text," Att set") == 0)
				{
					if(!Leftandright_key_setting_mode)// 设置阈值
					{
						if(mode_key_ctrl.stage_triggered == false)
						{
							mode_key_ctrl.stage_triggered = true;
							osal_start_reload_timer(keyadjust_app_task_id,KEYADJUST_APP_LONGPRESS_EVENT,1);
						}
						
						addorsub = 0;
					}
				}
				
				if(strcmp(menuSystem.current->text," STA set") == 0)
				{
					if(mode_key_ctrl.stage_triggered == false)
					{
						mode_key_ctrl.stage_triggered = true;
						osal_start_reload_timer(keyadjust_app_task_id,KEYADJUST_APP_LONGPRESS_EVENT,1);
					}
					
					addorsub = 0;
				}
			}
			
			if (longpress_morethan_3s_keys & HAL_KEY_LEFT_ADD)
			{
				if(strcmp(menuSystem.current->text," Att set") == 0)
				{
					if(!Leftandright_key_setting_mode)// 设置阈值
					{
						if(mode_key_ctrl.stage_triggered == false)
						{
							mode_key_ctrl.stage_triggered = true;
							osal_start_reload_timer(keyadjust_app_task_id,KEYADJUST_APP_LONGPRESS_EVENT,1);
						}
						
						addorsub = 1;
					}
				}
				
				if(strcmp(menuSystem.current->text," STA set") == 0)
				{
					if(mode_key_ctrl.stage_triggered == false)
					{
						mode_key_ctrl.stage_triggered = true;
						osal_start_reload_timer(keyadjust_app_task_id,KEYADJUST_APP_LONGPRESS_EVENT,1);
					}
					
					addorsub = 1;
				}
			}			
		}	
		
		
		// ===================================================================
		// 释放
		// ===================================================================	
		if(release_keys & HAL_KEY_LEFT_ADD)
		{
			press_duration = 0;
				
			addintervalcount = 0;
			
			mode_key_ctrl.current_stage = 0;											//每次松手时档位清零
			
			mode_key_ctrl.speed_value = SPEED_INCREMENTS[mode_key_ctrl.current_stage];
			
			mode_key_ctrl.stage_triggered = false;
		
			osal_stop_timerEx(keyadjust_app_task_id,KEYADJUST_APP_LONGPRESS_EVENT);
			
			iot_app_optical_set_att(iot_app_optical_current_mode, iot_app_optical_current_att);
			
			// 记录att,sta值
			Flash_Write_SetValue();
		}
		if(release_keys & HAL_KEY_RIGHT_SUB)
		{
			press_duration = 0;
				
			addintervalcount = 0;
			
			mode_key_ctrl.current_stage = 0;											//每次松手时档位清零
			
			mode_key_ctrl.speed_value = SPEED_INCREMENTS[mode_key_ctrl.current_stage];
			
			mode_key_ctrl.stage_triggered = false;
		
			osal_stop_timerEx(keyadjust_app_task_id,KEYADJUST_APP_LONGPRESS_EVENT);
			
			iot_app_optical_set_att(iot_app_optical_current_mode, iot_app_optical_current_att);
			
			// 记录att,sta值
			Flash_Write_SetValue();
		}	
	}
	
	if(systemstate == 1)
	{
		addchoice = 0;
		// ===================================================================
		// 短按
		// ===================================================================
		
		if (press_keys & HAL_KEY_LEFT_ADD)
		{
			//-- TODO
			
		}
		
		if (press_keys & HAL_KEY_RIGHT_SUB)
		{
			//-- TODO
			
		}
		
		if (press_keys & HAL_KEY_MODE)
		{
			//-- TODO
			
		}
		
		if (press_keys & HAL_KEY_SET)
		{
			//-- TODO
			
		}
		
		if (press_keys & HAL_KEY_MEGA)
		{
			//-- TODO
			
		}
		
		if (press_keys & HAL_KEY_SW6)
		{
			//-- TODO
			if (cur_keys & HAL_KEY_SW6)
			{
				//display_example();
			}
			else
			{
				
			}
		}
		
		if(!Leftandright_key_setting_mode)// 设置阈值
		{
			if(press_keys)
			{
				if (press_keys & HAL_KEY_RIGHT_SUB)
				{
					//-- TODO
					
					if(thresholdValue>0)
					{
						thresholdValue--;
					}
					else
					{
						thresholdValue = 0;
					}
				}

				if (press_keys & HAL_KEY_LEFT_ADD)
				{
					//-- TODO
					thresholdValue++;
				}
				if(thresholdValue > MAXSETVALUE)
				{
					thresholdValue = MAXSETVALUE;
				}
				
				// 记录AD阈值
				Flash_Write_SetValue();
			}
		// ===================================================================
		// 长按
		// ===================================================================
			if(longpress_morethan_3s_keys && islongorshortpress==2)
			{	
				if (longpress_morethan_3s_keys & HAL_KEY_RIGHT_SUB)
				{
					//-- TODO
					if(mode_key_ctrl.stage_triggered == false)
					{
						mode_key_ctrl.stage_triggered = true;
						osal_start_reload_timer(keyadjust_app_task_id,KEYADJUST_APP_LONGPRESS_EVENT,1);
					}
					
					addorsub = 0;

				}

				if (longpress_morethan_3s_keys & HAL_KEY_LEFT_ADD)
				{
					//-- TODO
					if(mode_key_ctrl.stage_triggered == false)
					{
						mode_key_ctrl.stage_triggered = true;
						osal_start_reload_timer(keyadjust_app_task_id,KEYADJUST_APP_LONGPRESS_EVENT,1);
					}
					
					addorsub = 1;
					
				}
			}	
		}
		// ===================================================================
		// 长按
		// ===================================================================	
		// 恢复出厂设置有关===================================START
		if(longpress_morethan_3s_keys & HAL_KEY_SET && (longpress_morethan_3s_keys & HAL_KEY_MODE))
		{
			systemstate = 3;
			
			return scan_flag;
		}
		// 恢复出厂设置有关===================================END
		
		// ===================================================================
		// 释放
		// ===================================================================	
		if(release_keys & HAL_KEY_LEFT_ADD)
		{
			press_duration = 0;
				
			addintervalcount = 0;
			
			mode_key_ctrl.current_stage = 0;											//每次松手时档位清零
			
			mode_key_ctrl.speed_value = SPEED_INCREMENTS[mode_key_ctrl.current_stage];
			
			mode_key_ctrl.stage_triggered = false;
		
			osal_stop_timerEx(keyadjust_app_task_id,KEYADJUST_APP_LONGPRESS_EVENT);
			
			// 记录AD阈值
			Flash_Write_SetValue();
		}
		if(release_keys & HAL_KEY_RIGHT_SUB)
		{
			press_duration = 0;
				
			addintervalcount = 0;
			
			mode_key_ctrl.current_stage = 0;											//每次松手时档位清零
			
			mode_key_ctrl.speed_value = SPEED_INCREMENTS[mode_key_ctrl.current_stage];
			
			mode_key_ctrl.stage_triggered = false;
		
			osal_stop_timerEx(keyadjust_app_task_id,KEYADJUST_APP_LONGPRESS_EVENT);
			
			// 记录AD阈值
			Flash_Write_SetValue();
		}	
			
	}
    return scan_flag;
}



void keyadjust_app_init(uint8 task_id)
{
    keyadjust_app_task_id = task_id;
	
	if (!press_duration) { 
		mode_key_ctrl.stage_triggered = false;
		mode_key_ctrl.speed_value = SPEED_INCREMENTS[mode_key_ctrl.current_stage];
		mode_key_ctrl.add_interval = ADD_INCREMENTS[mode_key_ctrl.current_stage];
	}
	
    HalKeyCallbackRegister(keyadjust_app_key_callback);
}

uint16 keyadjust_app_process_event(uint8 task_id, uint16 events)
{
    (void)task_id; // 显式忽略未引用参数（避免编译器警告）
     
    // 处理系统消息事件
    if ( events & SYS_EVENT_MSG )
    {
        // 接收消息队列中的消息
        uint8* msg_packet = (uint8 *)osal_msg_receive(keyadjust_app_task_id);
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
            msg_packet = (uint8 *)osal_msg_receive(keyadjust_app_task_id);
        }

        // 返回未处理的事件（清除已处理的SYS_EVENT_MSG）
        return (events ^ SYS_EVENT_MSG);
    }

	if(events & KEYADJUST_APP_LONGPRESS_EVENT)
	{
		press_duration ++;
		addintervalcount ++;
		
		if (press_duration >= STAGE_THRESHOLDS[mode_key_ctrl.current_stage]) {
			press_duration = 0;
			addintervalcount = 0;  
			
			
			if(mode_key_ctrl.current_stage< 3)
			{
				mode_key_ctrl.current_stage++;
			}
		
			mode_key_ctrl.speed_value = SPEED_INCREMENTS[mode_key_ctrl.current_stage];
			mode_key_ctrl.add_interval = ADD_INCREMENTS[mode_key_ctrl.current_stage];

		}
		if(addchoice == 0)
		{
			if(addintervalcount>mode_key_ctrl.speed_value)
			{
				addintervalcount=0;
				if(addorsub)
				{
				
					thresholdValue +=mode_key_ctrl.add_interval;
						
				}
				else
				{
				
					if(thresholdValue>mode_key_ctrl.add_interval)
					{
						thresholdValue -=mode_key_ctrl.add_interval;
					}
					else
					{
						thresholdValue = 0;
					}
				
				}
				
				if(thresholdValue > MAXSETVALUE)
				{
					thresholdValue = MAXSETVALUE;
				}
			}
		}
		if(addchoice==1)
		{
			addattsetcount++;
				
			if(addattsetcount>200)
			{
				addattsetcount=0;
				if(addorsub)
				{
					iot_app_optical_current_att +=1;
					
					if(iot_app_optical_current_att>=100)
					{
						iot_app_optical_current_att = 100;
					}
				}
				else
				{
					if(iot_app_optical_current_att>1)
					{
						iot_app_optical_current_att -=1;
					}
					else
					{
						iot_app_optical_current_att = 0;
					}
				}

                iot_app_optical_auto_adjust_att = iot_app_optical_current_att;
			}
		}
		if(addchoice == 2)
		{
			if(addintervalcount>mode_key_ctrl.speed_value)
			{
				addintervalcount=0;
				if(addorsub)
				{
				
					iot_app_optical_current_sta +=mode_key_ctrl.add_interval;
						
				}
				else
				{
				
					if(iot_app_optical_current_sta>mode_key_ctrl.add_interval)
					{
						iot_app_optical_current_sta -=mode_key_ctrl.add_interval;
					}
					else
					{
						iot_app_optical_current_sta = 0;
					}
				
				}
				
				if(iot_app_optical_current_sta > 9999)
				{
					iot_app_optical_current_sta = 9999;
				}
			}
		}
		return(events ^ KEYADJUST_APP_LONGPRESS_EVENT);
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

