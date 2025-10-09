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
uint8 iot_app_task_id;   // 任务ID，由OSAL分配，用于内部任务/事件处理

// set校准有关===================================START
// 校准状态枚举
typedef enum {
    IDLE,           // 空闲状态
    FIRST_SET,      // 第一次设置
    SECOND_SET,     // 第二次设置
    DISPLAY_BAR     // 显示条
} CalibrationState;
CalibrationState state = IDLE;  		// 当前校准状态
uint16_t currentLightIntensity = 0;  	// 当前光强值
uint16_t firstSetLight = 0;        	 	// 第一次设置的光强值
bool TwoCloseEncounters = false;    	// 两次接近标志
uint8_t return_count = 0;           	// 退出计数器
bool AutoOrHeadSet = 0;             	// 0手动设置，1自动设置
static uint8_t ouserreturncount = 0; 	// 用户返回计数
static bool autoflash_state = true; // 闪烁状态
// set校准有关===================================END

static bool outputflash_state = true; 	// 输出闪烁状态
static bool dispthresholdvalue = false; // 显示阈值标志


// 冗余的全局变量，不过已经写出来了，不方便修改了，以后不用这种全局变量做互斥
uint8_t Leftandright_key_setting_mode = 0;// 0 设置阈值 1 设置输出模式

// 防止按键干涉
uint8_t addkeymodechoice = 1; 	// 1 初始化 0 锁
uint8_t subkeymodechoice = 1 ;	// 1 初始化 0 锁  
uint8_t modekeymodechoice = 1 ; // 1 初始化 0 锁 
uint8_t setkeymodechoice = 1; 	// 1 初始化 0 锁 

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/
// flash存储有关===================================================START
// 判断是否改变了数值，把设定值写入flash                                                                	
void Flash_Write_SetValue(void)
{   		
	uint8 ifchange =0;
	
	// 读取示例
	uint32_t read_buf[10];
	
	int read_len = flash_read_latest_in_page(TARGET_SECTOR_NUM, read_buf, 10,0xEEEEEEEE); 
	
	if(!read_len)
	{
		return ;
	}
	
	if(iot_app_optical_current_att!=read_buf[0])
	{
		ifchange =1;
	}
	if(thresholdValue!=read_buf[1])
	{
		ifchange =1;
	}
	if(outmodeset!=read_buf[2])
	{
		ifchange =1;
	}
	if(iot_app_optical_current_mode!=read_buf[3])
	{
		ifchange =1;
	}
	if(iot_app_optical_current_sta!=read_buf[4])
	{
		ifchange =1;
	}
	if (iot_app_optical_auto_adjust_att!=read_buf[5])
	{
	    ifchange = 1;
	}
	
	if(ifchange)
	{
		uint32_t my_data[10] = {
		(uint32_t)iot_app_optical_current_att,
		(uint32_t)thresholdValue,
		(uint32_t)outmodeset,
		(uint32_t)iot_app_optical_current_mode,
		(uint32_t)iot_app_optical_current_sta,
		(uint32_t)iot_app_optical_auto_adjust_att,
		7,8,9,0xEEEEEEEE};
		
		flash_write_page(TARGET_SECTOR_NUM, my_data, 10); // 写入第0页
	}
} 

// 把设定值读出来                                                               	
void Flash_Read_SetValue(void)
{
	// 读取示例
	uint32_t read_buf[10];
	
	int read_len = flash_read_latest_in_page(TARGET_SECTOR_NUM, read_buf, 10,0xEEEEEEEE);    
	
	if(!read_len)
	{
		uint32_t my_data[10] = {
		100,
		200,
		0,
		IOT_APP_OPTICAL_MODE_HIGH_SPEED,
		2000,
		100,
		7,8,9,0xEEEEEEEE};
		
		flash_write_page(TARGET_SECTOR_NUM, my_data, 10); // 写入第0页
		
		read_len = flash_read_latest_in_page(TARGET_SECTOR_NUM, read_buf, 10,0xEEEEEEEE);  
	}

	iot_app_optical_current_att = (uint8)read_buf[0];
	
	thresholdValue =  (uint32)read_buf[1];
	
	outmodeset = (uint8_t)read_buf[2];
	
	iot_app_optical_current_mode = (uint8_t)read_buf[3];
	{
		iot_app_optical_set_mode(0, iot_app_optical_current_mode);
		
		switch(iot_app_optical_current_mode)
		{
			case IOT_APP_OPTICAL_MODE_FINE :{power_setting->childrenmenu = FinE_mode;}break;
			case IOT_APP_OPTICAL_MODE_HIGH_SPEED :{power_setting->childrenmenu = highspeed_mode;}break;
			case IOT_APP_OPTICAL_MODE_SUPER :{power_setting->childrenmenu = super_mode;}break;
			case IOT_APP_OPTICAL_MODE_MEGA :{power_setting->childrenmenu = mega_mode;}break;
		}
	}
	
	iot_app_optical_current_sta = (uint32_t)read_buf[4];

	iot_app_optical_auto_adjust_att = (int8)read_buf[5];
}
// flash存储有关===================================================END
// set校准有关===================================START
uint16 get_nowadvalue(void)
{
	return iot_app_optical_current_lux;
}

// 自动设置相关变量
uint16_t MAXADValue = 0;    // 最大AD值
uint16_t MINADValue = 0;    // 最小AD值
bool AutoSetStartOrStop = 0; // 0关闭，1启动自动设置


void AutoSetThresoldValueEn(uint8_t en)
{
	if(en)
	{
		AutoSetStartOrStop = 1;
	}
	else
	{
		AutoSetStartOrStop = 0;
	}
}


uint8_t IsAutoSetStartOrStop(void)
{
	return AutoSetStartOrStop;
}


void AutoSetThresoldValue(void)
{
	if(AutoSetStartOrStop)
	{
		uint16_t NOWADValue = get_nowadvalue();
		
		// 更新最大最小值
		(NOWADValue>= MAXADValue)? MAXADValue = NOWADValue : (NOWADValue<= MINADValue)? MINADValue = NOWADValue:0;
		
		// 计算阈值（最大最小值的平均值）
		thresholdValue = (MAXADValue + MINADValue) / 2;
	}
}
// set校准有关===================================END

// 输出引脚控制初始化=============================START
void NpnOutPutInit(void)
{
   GPIO_Config_T gpioConfig;
	
	// 使能GPIO时钟
	RCM_EnableAHBPeriphClock(HAL_NPN_OUT_PERIPH_PORT);

	// 配置GPIO参数
	gpioConfig.pin = HAL_NPN_OUT_PIN;
	gpioConfig.mode = GPIO_MODE_OUT;
	gpioConfig.outtype = GPIO_OUT_TYPE_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	gpioConfig.pupd = GPIO_PUPD_NO;
	GPIO_Config(HAL_NPN_OUT_PORT, &gpioConfig);
}
// 输出引脚控制初始化=============================END

#define STEP 1             // 调整步长
#define TOLERANCE 10         // 允许的误差范围

uint8 adjust_match_count = 0;
uint8 adjust_total_count = 0;

int adjust_light_intensity(int current_intensity) 
{
    uint8  adjust_finish = 0;
    uint32 target_sta = TOLERANCE;

    if (iot_app_optical_current_sta > TOLERANCE)
    {
        target_sta = iot_app_optical_current_sta - TOLERANCE;
    }

    // 调整光强
    if (current_intensity < target_sta)
    {
        // 光强不足，增加到sta

        if (iot_app_optical_auto_adjust_att == 100)
        {
            adjust_finish = 1;
        }

        iot_app_optical_auto_adjust_att += STEP;
        if (iot_app_optical_auto_adjust_att > 100)
        {
            iot_app_optical_auto_adjust_att = 100;
        }

        iot_app_optical_set_att(iot_app_optical_current_mode, iot_app_optical_auto_adjust_att);
    } 
    else if (current_intensity > target_sta)
    {
        // 光强过大，减少到sta

        if (iot_app_optical_auto_adjust_att == -100)
        {
            adjust_finish = 1;
        }

        iot_app_optical_auto_adjust_att -= STEP;
        if (iot_app_optical_auto_adjust_att < -100)
        {
            iot_app_optical_auto_adjust_att = -100;
        }

        iot_app_optical_set_att(iot_app_optical_current_mode, iot_app_optical_auto_adjust_att);
    }

    // 检查是否在允许的误差范围内
    if (abs(current_intensity - (int32)iot_app_optical_current_sta) <= TOLERANCE) 
    {
        if (++adjust_match_count > 10)
        {
            adjust_finish = 1;
        }
    }
    else
    {
        adjust_match_count = 0;
    }

    if (++adjust_total_count >= 150)
    {
        adjust_finish = 1;
    }

    return adjust_finish;
}

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
	
	if(systemstate == 1)
	{
		// ===================================================================
		// 短按
		// ===================================================================
		// ADD按键
		if (press_keys & HAL_KEY_LEFT_ADD)
		{
			// 输出模式设定有关===================================START
			if(addkeymodechoice)
			{
				// 设置输出模式
				if(Leftandright_key_setting_mode)// 设置输出模式
				{
					// 切换输出模式
					outmodeset ^= 1;
					
					// 记录输出模式
					Flash_Write_SetValue();
					
					// 关闭刷新显示
					threshold_update_en = 0;
					adc_update_en = 0;
					
					// 返回计数清零
					ouserreturncount = 0;
					
					// 显示新的输出模式
					if(!outmodeset)
						display_string("L-on    ");
					else
						display_string("    D-on");
					
					// 输出模式切换闪烁
					osal_start_reload_timer(iot_app_task_id, IOT_APP_SET_OUTMODECHANGE_EVENT, 500);
				}
			}
			// 输出模式设定有关===================================END
		}
		
		// SUB按键
		if (press_keys & HAL_KEY_RIGHT_SUB)
		{
			// 输出模式设定有关===================================START
			if(addkeymodechoice)
			{
				if(Leftandright_key_setting_mode)
				{
					// 切换输出模式
					outmodeset ^= 1;
					
					// 记录输出模式
					Flash_Write_SetValue();
					
					threshold_update_en = 0;
					adc_update_en = 0;
					ouserreturncount = 0;
					
					// 显示新的输出模式
					if(!outmodeset)
						display_string("L-on    ");
					else
						display_string("    D-on");
					
					// 重启输出模式切换事件
					osal_start_reload_timer(iot_app_task_id, IOT_APP_SET_OUTMODECHANGE_EVENT, 500);
				}
			}
			// 输出模式设定有关===================================END
		}
		
		// MODE键处理
		if (press_keys & HAL_KEY_MODE)
		{
			// 输出模式设定有关===================================START
			if(modekeymodechoice)
			{
				setkeymodechoice = 0;// set锁掉
				
				if(!Leftandright_key_setting_mode)
				{
					// 进入输出模式设置
					Leftandright_key_setting_mode = 1;
					
					threshold_update_en = 0;
					adc_update_en = 0;
					ouserreturncount = 0;
					
					// 显示当前输出模式
					if(!outmodeset)
						display_string("L-on    ");
					else
						display_string("    D-on");
					
					// 启动输出模式切换事件
					osal_start_reload_timer(iot_app_task_id, IOT_APP_SET_OUTMODECHANGE_EVENT, 500);
				}
				else
				{
					// 退出输出模式设置
					Leftandright_key_setting_mode = 0;
					
					threshold_update_en = 1;
					adc_update_en = 1;
					ouserreturncount = 0;
					
					outputflash_state = true;
					
					osal_stop_timerEx(iot_app_task_id, IOT_APP_SET_OUTMODECHANGE_EVENT); 
					
					setkeymodechoice = 1;// set恢复校准功能
				}
			}
				// 输出模式设定有关===================================END
		}
		
		// SET键短按处理
		if (press_keys & HAL_KEY_SET)
		{
			if(setkeymodechoice)
			{
				// set校准有关===================================START
				AutoOrHeadSet = 0; // 设置为手动模式
				// set校准有关===================================END
			}
			
		}
		
		// MEGA键处理
		if (press_keys & HAL_KEY_MEGA)
		{
		    adjust_total_count = 0;
			osal_start_reload_timer(iot_app_task_id,IOT_APP_TIMER_EVT,IOT_APP_TIMER_INTERVAL);
		}

		// SW6开关处理
		if (press_keys & HAL_KEY_SW6)
		{
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
			
			return scan_flag;
		}
		// 恢复出厂设置有关===================================END
		
		// SET键长按处理（>3s）
		if(longpress_morethan_3s_keys & HAL_KEY_SET )
		{
			if(systemstate==1)
			{
				// set校准有关===================================START
				if(setkeymodechoice)
				{
					{//直接结束两点
						state = IDLE; // 返回空闲状态
						threshold_update_en = 1; // 启用阈值更新
						firstSetLight = 0; // 重置第一次设置值
						TwoCloseEncounters = false; // 重置接近标志
						return_count = 0;
						osal_stop_timerEx(iot_app_task_id, IOT_APP_NIXIETUBE_FLASH_EVENT);
					}
					
					
					AutoOrHeadSet = 1;       // 设置为自动模式
					
					threshold_update_en = 0; // 禁用阈值更新
					
					// 如果自动设置未启动
					if(!IsAutoSetStartOrStop()){
						MAXADValue = 0;  // 重置最大值
						MINADValue = 0;  // 重置最小值
						AutoSetThresoldValueEn(1); // 启动自动设置
						
						// 启动显示设置事件
						osal_start_timerEx(iot_app_task_id, IOT_APP_NIXIETUBE_DISPSETMS_EVENT, 
										  IOT_APP_NIXIETUBE_DISPSETMS_TIMER_INTERVAL);
					}
		
				}
				// set校准有关===================================END
			}
			
		}
		// ===================================================================
		// 释放
		// ===================================================================
		// SET键释放处理
		if( release_keys & HAL_KEY_SET)
		{
			// set校准有关===================================START
			if(setkeymodechoice)
			{
				if(AutoOrHeadSet) // 自动模式
				{
					dispthresholdvalue = 1; // 显示阈值标志
				}
				else // 手动模式
				{
					threshold_update_en = 0; // 禁用阈值更新
					
					// 根据状态机处理
					switch (state) {
						case IDLE:
							// 第一次按下SET键
							return_count = 0;
							firstSetLight = get_nowadvalue(); // 记录当前光强
							display_stringbyarea(0," set",0); // 显示"set"
							state = FIRST_SET; // 切换到第一次设置状态
							break;
						case FIRST_SET:
							// 第二次按下SET键
							return_count = 0;
							// 计算阈值（两次设置的平均值）
							thresholdValue = (firstSetLight + get_nowadvalue()) / 2;
							
							// 检查两次设置是否接近
							if (abs(40 - firstSetLight) < 30) {
								TwoCloseEncounters = true;
							} else {
								TwoCloseEncounters = false;
							}
							
							// 启动显示设置事件
							osal_start_timerEx(iot_app_task_id, IOT_APP_NIXIETUBE_DISPSETMS_EVENT, 
									  IOT_APP_NIXIETUBE_DISPSETMS_TIMER_INTERVAL);
							break;
						default:break;
					}
				}
				
				// 记录AD阈值
				Flash_Write_SetValue();
			}
			// set校准有关===================================END
		}
	}	

	// ===================================================================
	
	return scan_flag;
}

void iot_app_init(uint8 task_id)
{
    iot_app_task_id = task_id; // 保存任务ID
	
	NpnOutPutInit(); // 初始化NPN输出
	
	// 阈值刷显示有关=================================START
	// 启动阈值显示刷新事件
	osal_start_timerEx(iot_app_task_id,IOT_APP_FLASH_THRESHOLD_EVENT,10);
	// 阈值刷显示有关=================================END
	
	
	// 初始化存储内容
	Flash_Read_SetValue();
	
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
	
	
	
	
	// 自动调整光强 测试中
	if(events & IOT_APP_TIMER_EVT)
	{
		if (adjust_light_intensity(get_nowadvalue()))
		{
		    osal_stop_timerEx(iot_app_task_id,IOT_APP_TIMER_EVT);
	        iot_app_optical_current_att = (iot_app_optical_auto_adjust_att < 0)?0:iot_app_optical_auto_adjust_att;
		    
		    Flash_Write_SetValue();
		}
		
		return (events ^ IOT_APP_TIMER_EVT);
	}
	
	
	// 输出模式设定有关===================================START
	// 输出模式切换事件处理
	if(events & IOT_APP_SET_OUTMODECHANGE_EVENT)
	{
		if(ouserreturncount < 6) // 闪烁6次
		{
			ouserreturncount++;
		}
		else // 闪烁结束
		{
			Leftandright_key_setting_mode = 0; // 退出设置模式
			outputflash_state = true;
			threshold_update_en = 1; // 启用阈值更新
			adc_update_en = 1; // 启用ADC更新
			osal_stop_timerEx(iot_app_task_id, IOT_APP_SET_OUTMODECHANGE_EVENT); 
			ouserreturncount = 0;
			
			setkeymodechoice = 1;// set恢复校准功能
		}
		
		outputflash_state = !outputflash_state; // 切换闪烁状态
		
		if(!outputflash_state) // 闪烁间隔清除显示
		{
			display_string("        ");
		}
		else // 显示当前输出模式
		{
			if(!outmodeset)
				display_stringbyarea(0,"L-on",0);
			else
				display_stringbyarea(0,"D-on",1);
		}
	
		return (events ^ IOT_APP_SET_OUTMODECHANGE_EVENT);
	}
	// 输出模式设定有关===================================END

	// set校准有关===================================START
	// 数码管显示设置事件处理
	if(events & IOT_APP_NIXIETUBE_DISPSETMS_EVENT)
	{
		// 显示"set"标识
		display_stringbyarea(0," set",0);
		
		// 根据设置模式启动不同的闪烁事件
		if(AutoOrHeadSet) // 自动模式
		{
			osal_start_reload_timer(iot_app_task_id, IOT_APP_THRESHOLDVALUE_AUTOSETBLINK_EVENT, 500);
		}
		else // 手动模式
		{
			osal_start_reload_timer(iot_app_task_id, IOT_APP_NIXIETUBE_FLASH_EVENT, 
                          IOT_APP_NIXIETUBE_FLASH_TIMER_INTERVAL);
		}
		
		return (events ^ IOT_APP_NIXIETUBE_DISPSETMS_EVENT);
	}
	// 自动设置阈值闪烁事件处理
	if(events & IOT_APP_THRESHOLDVALUE_AUTOSETBLINK_EVENT)
	{
		
		autoflash_state = !autoflash_state; // 切换闪烁状态
		
		char str[20];
		
		if(autoflash_state)
		{
			if(dispthresholdvalue) // 如果需要显示阈值
			{
				static uint8_t count = 0;
			
				if(count < 6) // 显示6次
				{
					count++;
					
					// 格式化阈值显示
					if(thresholdValue >= 1000 && thresholdValue<10000)
						sprintf(str, "%d", thresholdValue);
					else if(thresholdValue >= 100 && thresholdValue<1000)
						sprintf(str, " %d", thresholdValue);
					else if(thresholdValue>=10 && thresholdValue < 100)
						sprintf(str, "  %d", thresholdValue);
					else if(thresholdValue < 10)
						sprintf(str, "   %d", thresholdValue);
				
					display_stringbyarea(0,str,0);	
				}
				else // 显示结束
				{
					count = 0;
					threshold_update_en = 1; // 启用阈值更新
					dispthresholdvalue = 0;
					AutoSetThresoldValueEn(0); // 禁用自动设置
					osal_stop_timerEx(iot_app_task_id, IOT_APP_THRESHOLDVALUE_AUTOSETBLINK_EVENT);
				}
			}
			else // 显示"auto"
			{
				display_stringbyarea(0,"auto",0);
			}
		}
		else // 清除显示
		{
			display_stringbyarea(0,"    ",0);
		}
		
		return (events ^ IOT_APP_THRESHOLDVALUE_AUTOSETBLINK_EVENT);
	}
	// 数码管闪烁事件处理（两点设置）
	if(events & IOT_APP_NIXIETUBE_FLASH_EVENT)
	{
		static bool flash_state = true;
		
		flash_state = !flash_state; // 切换闪烁状态
		
		char str[20];

		display_stringbyarea(0,"    ",0); // 清除显示
		
		if(!flash_state) // 闪烁间隔显示
		{
			if(TwoCloseEncounters)
				display_string("        "); // 两次接近显示空格
			else
				display_stringbyarea(0,"    ",0);
		}
		else // 显示阈值或横线
		{
			if(TwoCloseEncounters)
				display_string("--------"); // 两次接近显示横线
			else
			{
				// 格式化阈值显示
				if(thresholdValue >= 1000 && thresholdValue<10000)
					sprintf(str, "%d", thresholdValue);
				else if(thresholdValue >= 100 && thresholdValue<1000)
					sprintf(str, " %d", thresholdValue);
				else if(thresholdValue>=10 && thresholdValue < 100)
					sprintf(str, "  %d", thresholdValue);
				else if(thresholdValue < 10)
					sprintf(str, "   %d", thresholdValue);
			
				display_stringbyarea(0,str,0);
			}
		}
		
		if(return_count < 6) // 闪烁6次
		{
			return_count++;
		}
		else // 闪烁结束
		{
			flash_state = 1;
			state = IDLE; // 返回空闲状态
			threshold_update_en = 1; // 启用阈值更新
			firstSetLight = 0; // 重置第一次设置值
			TwoCloseEncounters = false; // 重置接近标志
			return_count = 0;
			osal_stop_timerEx(iot_app_task_id, IOT_APP_NIXIETUBE_FLASH_EVENT);
		}
		
		return (events ^ IOT_APP_NIXIETUBE_FLASH_EVENT);
	}
	// set校准有关===================================END
	
	// 阈值刷显示有关=================================START
	// 阈值显示刷新事件处理
	if(events & IOT_APP_FLASH_THRESHOLD_EVENT)
	{
		if(threshold_update_en) // 如果启用阈值更新
		{
			char str[20];
		
			// 格式化阈值显示（根据数值大小调整显示格式）
			if(thresholdValue >= 1000 && thresholdValue<10000)
				sprintf(str, "%d", thresholdValue);
			else if(thresholdValue >= 100 && thresholdValue<1000)
				sprintf(str, " %d", thresholdValue);
			else if(thresholdValue>=10 && thresholdValue < 100)
				sprintf(str, "  %d", thresholdValue);
			else if(thresholdValue < 10)
				sprintf(str, "   %d", thresholdValue);
		
			display_stringbyarea(0,str,0); // 显示阈值
		}	
			
		// 重新启动定时器
		osal_start_timerEx(iot_app_task_id,IOT_APP_FLASH_THRESHOLD_EVENT,10);
			
		return (events ^ IOT_APP_FLASH_THRESHOLD_EVENT);
	}
	// 阈值刷显示有关=================================END
	
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
