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

// 定义闪烁类型枚举
typedef enum {
	FLASH_NO,
    FLASH_LEFT,
    FLASH_RIGHT,
	FLASH_ALL,
} FlashType;

static FlashType current_type = FLASH_NO;

uint8_t defaultreset = 0 ; // 是否恢复出厂设置 0 否 1 是
uint8 resetreturncont = 0;

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
		
	// 菜单设置
	{	
		// ===================================================================
		// 短按
		// ===================================================================
		// ADD按键
		if (press_keys & HAL_KEY_LEFT_ADD)
		{
			if(MenuSystem_IsActive())
			{
				uint16 level = Menu_GetCurrentLevel();
				
				if(level == 2)
				{
					// 二级设置项进入三级设置项
					Menu_Enter();
				}
				if(level == 3)
				{
					// 切换同级菜单
					Menu_Prev();
					
				}
				
				if(strcmp(menuSystem.current->text," Att set") == 0)
				{
					#ifdef _IOT_APP_OPTICAL_H
					iot_app_optical_set_att(iot_app_optical_current_mode, iot_app_optical_current_att);
					#endif
					
					keycount++;
				}
				
				if(menuSystem.current->parent == power_setting)
				{
					// 记录功率设置
					Flash_Write_SetValue();
				}
				
			}
		}
		
		// SUB按键
		if (press_keys & HAL_KEY_RIGHT_SUB)
		{
			if(MenuSystem_IsActive())
			{
				uint16 level = Menu_GetCurrentLevel();
				
				if(level == 2)
				{
					// 二级设置项进入三级设置项
					Menu_Enter();
				}
				if(level == 3)
				{
					// 切换同级菜单
					Menu_Next();
					
				}
				
				if(strcmp(menuSystem.current->text," Att set") == 0)
				{
				  
					#ifdef _IOT_APP_OPTICAL_H
					iot_app_optical_set_att(iot_app_optical_current_mode, iot_app_optical_current_att);
					#endif
				}
				
				if(menuSystem.current->parent == power_setting)
				{
					// 记录功率设置
					Flash_Write_SetValue();
				}
			
				
			}
		}
		// MODE键处理
		if (press_keys & HAL_KEY_MODE)
		{
			if(MenuSystem_IsActive())
			{
				uint16 level = Menu_GetCurrentLevel();
				
				if(level == 1)
				{
					// 下一个同级菜单
					Menu_Next();
					// 从基础设置进入二级设置项
					Menu_Enter();
					// 二级设置项进入三级设置项
					Menu_Enter();
				}
				if(level == 2)
				{
					// 下一个同级菜单
					Menu_Next();
					// 二级设置项进入三级设置项
					Menu_Enter();
				}
				else if(level == 3)
				{
					// 返回父菜单二级设置项
					Menu_Back(true);
					// 并切换下一个同级菜单
					Menu_Next();
				}	
			}
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
		
		// ===================================================================
		// 长按
		// ===================================================================
		// 恢复出厂设置有关===================================START
		if(longpress_morethan_3s_keys & HAL_KEY_SET && (longpress_morethan_3s_keys & HAL_KEY_MODE))
		{
			systemstate = 3;

			adc_update_en = 0 ;
			
			threshold_update_en = 0; 
			
			display_stringbyarea(0," rSt",0);
			
			display_stringbyarea(0,"    ",1);
			
			if(defaultreset)
			{
				display_stringbyarea(0,"  ok",1);		
			}
			else
			{
				display_stringbyarea(0,"  no",1);	
			}	
				
			osal_start_reload_timer(menu_app_task_id, MENU_APP_RESET_SETTING, 500);
		}
		// 恢复出厂设置有关===================================END
		if(longpress_morethan_3s_keys & HAL_KEY_MODE)
		{
			if(systemstate ==1)
			{
				// 进入基础设置
				if(!MenuSystem_IsActive())
				{
					
					
					MenuSystem_Start();
					
					threshold_update_en = 0;
					
					uint16 level = Menu_GetCurrentLevel();
					
					if(level == 0)
					{
						// 从根菜单进入基础设菜单
						Menu_Enter();
						// 从基础设置进入二级设置项
						Menu_Enter();
						// 二级设置项进入三级设置项
						Menu_Enter();
					}

					systemstate =2;
					
				}
			}
		}
		// ===================================================================
		// 释放
		// ===================================================================
		// SET键释放处理
		if( release_keys & HAL_KEY_SET)
		{
			
		}
	}
	
	if(systemstate==3)// 出厂设置
	{
		// ===================================================================
		// 短按
		// ===================================================================
		// ADD按键
		if (press_keys & HAL_KEY_LEFT_ADD)
		{
			defaultreset ^= 1;
			resetreturncont = 0;
		}
		// SUB按键
		if (press_keys & HAL_KEY_RIGHT_SUB)
		{
			defaultreset ^= 1;
			resetreturncont = 0;
		}
		
		// mode按键
		if (press_keys & HAL_KEY_MODE)
		{
			if(defaultreset)
			{
				defaultreset = 0;	
				
				uint32_t page_addr = STM32_FLASH_BASE + TARGET_SECTOR_NUM * STM32_SECTOR_SIZE;
				
				FMC_Unlock();
				FMC_ErasePage(page_addr);
				FMC_Lock();
				
				Flash_Read_SetValue();
			}
			
			resetreturncont = 7;
		}
	}
    return scan_flag;
}


void start_nixietube_flash(FlashType type) {
   
	display_modeset(1);
	
    // 如果类型改变，重置闪烁状态
    if (current_type != type) {
        current_type = type;
    }
	
	// 根据当前闪烁类型设置显示
	switch(current_type) {
		case FLASH_LEFT:
			display_modeset(2);
			break;
		case FLASH_RIGHT:
			display_modeset(3);
			break;
		case FLASH_ALL:
			display_modeset(1);
			break;
		default:break;
	}
    
    osal_start_reload_timer(menu_app_task_id, MENU_APP_NIXIETUBE_FLASH_EVENT, 
                          MENU_APP_NIXIETUBE_FLASH_TIMER_INTERVAL);
}
 
void stop_nixietube_flash() {
    osal_stop_timerEx(menu_app_task_id, MENU_APP_NIXIETUBE_FLASH_EVENT);
    // 确保数码管显示最终状态
	display_modeset(1);
    display_set_left_en(true);
    display_set_right_en(true);
}
bool dissta =0 ;// 临时参数
// 菜单回调函数
void onItem1Click(MenuItem* item) {
   
	MenuItem* tempitem = item;
	
	if(item->whichcallback == 1)// 如果是进入回调
	{
		// 判断是哪个回调了
		if(strcmp(tempitem->text,"FinE1234") == 0)
		{
			display_stringbyarea(0,"FinE1234",0);
			
			iot_app_optical_current_mode = IOT_APP_OPTICAL_MODE_FINE;

			#ifdef _IOT_APP_OPTICAL_H
			iot_app_optical_set_mode(0, iot_app_optical_current_mode);
			#endif
		
			start_nixietube_flash(FLASH_LEFT); // 只闪烁左侧
		}
		else if(strcmp(tempitem->text," hsp1234") == 0)
		{
			display_stringbyarea(0," hsp1234",0);
			
			iot_app_optical_current_mode = IOT_APP_OPTICAL_MODE_HIGH_SPEED;

			#ifdef _IOT_APP_OPTICAL_H
			iot_app_optical_set_mode(0, iot_app_optical_current_mode);
			#endif
			
			start_nixietube_flash(FLASH_LEFT);
		}	
		else if(strcmp(tempitem->text,"mega1234") == 0)
		{
			display_stringbyarea(0,"mega1234",0);
			
			iot_app_optical_current_mode = IOT_APP_OPTICAL_MODE_MEGA;

			#ifdef _IOT_APP_OPTICAL_H
			iot_app_optical_set_mode(0, iot_app_optical_current_mode);
			#endif
			
			start_nixietube_flash(FLASH_LEFT);
		}
		else if(strcmp(tempitem->text,"supr1234") == 0)
		{
			display_stringbyarea(0,"supr1234",0);
			
			iot_app_optical_current_mode = IOT_APP_OPTICAL_MODE_SUPER;

			#ifdef _IOT_APP_OPTICAL_H
			iot_app_optical_set_mode(0, iot_app_optical_current_mode);
			#endif
			
			start_nixietube_flash(FLASH_LEFT);
		}
		else if(strcmp(tempitem->text," Att set") == 0)
		{
			display_stringbyarea(0," Att set",0);
			
			osal_start_reload_timer(menu_app_task_id, MENU_APP_ATTSET_FLASH, 
                          MENU_APP_ATTSET_FLASH_TIMER_INTERVAL);
		}
		else if(strcmp(tempitem->text," STA set") == 0)
		{
			display_stringbyarea(0," STA set",0);
			
			adc_update_en = 0 ;
			dissta =1 ;// 临时参数
			osal_start_reload_timer(menu_app_task_id, MENU_APP_FLASHDISP_COMMON_EVENT, 
                          MENU_APP_FLASHDISP_COMMON_TIMER_EVT);
		}
		else if(strcmp(tempitem->text," SET std") == 0)
		{
			display_string(" SET std");
			
			start_nixietube_flash(FLASH_RIGHT);
		}
		else if(strcmp(tempitem->text," SETSETP") == 0)
		{
			display_string(" SETSETP");
			
			start_nixietube_flash(FLASH_RIGHT);
		}
		else if(strcmp(tempitem->text," SETOSET") == 0)
		{
			display_string(" SETOSET");
			
			start_nixietube_flash(FLASH_RIGHT);
		}
		else if(strcmp(tempitem->text,"     End") == 0)
		{
			
			adc_update_en = 0;
			threshold_update_en = 0;
			
			display_string("     End");
			
			start_nixietube_flash(FLASH_RIGHT);
		}
		else if(strcmp(tempitem->text,"   End  ") == 0)
		{
			adc_update_en = 0;
			threshold_update_en = 0;
			
			display_string("   End  ");
			
			osal_start_timerEx(menu_app_task_id, MENU_APP_INTOEND_END ,MENU_APP_INTOEND_END_TIMER_INTERVAL);
		}
	}
	else if(item->whichcallback == 2)
	{
		// 判断是哪个回调了
		if(strcmp(tempitem->text,"FinE1234") == 0)
		{
			stop_nixietube_flash();
		}
		else if(strcmp(tempitem->text," hsp1234") == 0)
		{
			stop_nixietube_flash();
		}	
		else if(strcmp(tempitem->text,"mega1234") == 0)
		{
			stop_nixietube_flash();
		}
		else if(strcmp(tempitem->text,"supr1234") == 0)
		{
			stop_nixietube_flash();
		}
		else if(strcmp(tempitem->text," SET std") == 0)
		{
			stop_nixietube_flash();
		}
		else if(strcmp(tempitem->text," SETSETP") == 0)
		{
			stop_nixietube_flash();
		}
		else if(strcmp(tempitem->text," SETOSET") == 0)
		{
			stop_nixietube_flash();
		}
		else if(strcmp(tempitem->text,"     End") == 0)
		{
			adc_update_en = 1;
			threshold_update_en = 1;
			
			stop_nixietube_flash();
		}
		else if(strcmp(tempitem->text," Att set") == 0)
		{
			osal_stop_timerEx(menu_app_task_id, MENU_APP_ATTSET_FLASH);
			stop_nixietube_flash();
		}
		else if(strcmp(tempitem->text," STA set") == 0)
		{
			osal_stop_timerEx(menu_app_task_id, MENU_APP_FLASHDISP_COMMON_EVENT);
			dissta =0 ;// 临时参数
			stop_nixietube_flash();
			
		}
	}
	
	item->whichcallback = 0;
}


void menu_app_init(uint8 task_id)
{
	menu_app_task_id = task_id;
	
	MenuItem* root = CreateTestMenu(); // 动态创建菜单，所有菜单都在这个函数里编辑好
    MenuSystem_Init(root);	// 初始化系统菜单
	
	// 开机动画事件
	osal_start_timerEx(menu_app_task_id,MENU_APP_DISP_BOOT_ANIMATION,MENU_APP_ANIMATION_TIMER_INTERVAL);
	
	
	
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

	// 恢复出厂设置
	if(events & MENU_APP_RESET_SETTING)
	{
		static uint8 flash_state =0;
		
		flash_state = !flash_state; // 切换闪烁状态
		
		display_stringbyarea(0,"    ",0); // 清除显示
		
		display_stringbyarea(0," rst",0);
		
		if(resetreturncont<6)
		{
			resetreturncont++;
			if(flash_state)
			{
				if(defaultreset)
				{
					display_stringbyarea(0,"  ok",1);		
				}
				else
				{
					display_stringbyarea(0,"  no",1);	
				}	
			}
			else
			{
				display_stringbyarea(0,"    ",1); // 清除显示
			}
		}
		else
		{
			if(defaultreset)
			{
				defaultreset = 0;	
					
				uint32_t page_addr = STM32_FLASH_BASE + TARGET_SECTOR_NUM * STM32_SECTOR_SIZE;
				
				FMC_Unlock();
				FMC_ErasePage(page_addr);
				FMC_Lock();
				
				Flash_Read_SetValue();
			}
			
			resetreturncont = 0;
			
			systemstate = 1;

			adc_update_en = 1 ;
			
			threshold_update_en = 1; 
			
			osal_stop_timerEx(menu_app_task_id, MENU_APP_RESET_SETTING);
			
			display_string("        "); // 清除显示
		}	
		
		return (events ^ MENU_APP_RESET_SETTING);
	}
	
	// 公共闪烁事件
	if(events & MENU_APP_FLASHDISP_COMMON_EVENT)
	{
		static bool flash_state = false;
		
		flash_state = !flash_state;
		
		char str[20];
 
		if(flash_state)
		{
			
		}
		else
		{
		
			
		}
		
		// 临时放置
		if(iot_app_optical_current_sta>=1000 && iot_app_optical_current_sta < 9999)
		{
			sprintf(str, "%d", iot_app_optical_current_sta);
		}
		else if(iot_app_optical_current_sta>=100 && iot_app_optical_current_sta < 1000)
		{
			sprintf(str, " %d", iot_app_optical_current_sta);
		}
		else if(iot_app_optical_current_sta>=10 && iot_app_optical_current_sta < 100)
		{
			sprintf(str, "  %d", iot_app_optical_current_sta);
		}
		else if(iot_app_optical_current_sta < 10)
		{
			sprintf(str, "   %d", iot_app_optical_current_sta);
		}
		
		
		display_stringbyarea(0,str,1);
		
		if(!dissta){display_stringbyarea(0," End",1);}
		
		return (events ^ MENU_APP_FLASHDISP_COMMON_EVENT);
	}
	
	
	// 退出菜单事件
	if(events & MENU_APP_INTOEND_END)
	{
		
		if(MenuSystem_IsActive())
		{
			 MenuSystem_Stop(); // 停止菜单
			
			threshold_update_en = 1;
			
			adc_update_en = 1;
			
			// 显示预设
			display_string(menuSystem.current->text);	
		
			systemstate =1;
		}
		
		return (events ^ MENU_APP_INTOEND_END);
	}
	
	// att设置闪烁事件
	if(events & MENU_APP_ATTSET_FLASH)
	{
		static bool flash_state = false;
		
		flash_state = !flash_state;
		
		char str[20];
 
		display_stringbyarea(0,"    ",0);
		
		if(flash_state)
		{
			
			
			display_stringbyarea(0,menuSystem.current->text,0);
		}
		else
		{
			if(iot_app_optical_current_att == 100)
			{
				sprintf(str, " %d", iot_app_optical_current_att);
			}
			if(iot_app_optical_current_att>=10 && iot_app_optical_current_att < 100)
			{
				sprintf(str, "  %d", iot_app_optical_current_att);
			}
			else if(iot_app_optical_current_att < 10)
			{
				sprintf(str, "   %d", iot_app_optical_current_att);
			}
			
			
			display_stringbyarea(0,str,0);
			
		}	
		return (events ^ MENU_APP_ATTSET_FLASH);
	}
	
	
	
	
	// 数码管闪烁
	if (events & MENU_APP_NIXIETUBE_FLASH_EVENT) {

		static bool flash_state = false;
		
		flash_state = !flash_state;
		
		// 根据当前闪烁类型设置显示
		switch(current_type) {
			case FLASH_LEFT:
				display_set_left_en(flash_state);
				break;
			case FLASH_RIGHT:
				display_set_right_en(flash_state);
				break;
			case FLASH_ALL:
				display_set_global_enable(flash_state);
			break;
			default:break;
		}
		
		return (events ^ MENU_APP_NIXIETUBE_FLASH_EVENT);
	}
	
	// 显示开机动画
	if(events & MENU_APP_DISP_BOOT_ANIMATION)
	{
		// 动画状态变量
		static int8_t right_index = 7;  // 右侧当前要显示的字符索引（4-7）
		static int8_t left_index = 3;   // 左侧当前要显示的字符索引（0-3）
		static uint8_t right_done = 0;  // 右侧是否显示完毕
		static uint8_t left_done = 0;   // 左侧是否显示完毕

		static char tempstring[sizeof(boot_animation_string)] = {' ',' ',' ',' ',' ',' ',' ',' '};
		
		
        // 更新右侧字符（如果尚未显示完毕）
        if (!right_done && right_index >= 4) {
            tempstring[right_index] = boot_animation_string[right_index];
            right_index--;
            if (right_index < 4) {
                right_done = 1;
            }
        }
 
        // 更新左侧字符（如果尚未显示完毕）
        if (!left_done && left_index >= 0) {
            tempstring[left_index] = boot_animation_string[left_index];
            left_index--;
            if (left_index < 0) {
                left_done = 1;
            }
        }
		
		// 显示字符
        display_string(tempstring);
		
        // 检查是否所有字符都已显示完毕
        if (!right_done && !left_done) {
			
            // 重新设置定时器
			osal_start_timerEx(menu_app_task_id, MENU_APP_DISP_BOOT_ANIMATION, MENU_APP_ANIMATION_TIMER_INTERVAL);

        }
		else
		{
			
			osal_start_timerEx(menu_app_task_id, MENU_APP_FLASHDISP_AFTEROPEN_EVENT, 500);
		}
		
		return (events ^ MENU_APP_DISP_BOOT_ANIMATION);
    }
		
	// 启动阈值和adc的刷新
	if(events & MENU_APP_FLASHDISP_AFTEROPEN_EVENT)
	{
		adc_update_en = 1 ;
		
		threshold_update_en = 1;
		
		return (events ^ MENU_APP_FLASHDISP_AFTEROPEN_EVENT);
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

