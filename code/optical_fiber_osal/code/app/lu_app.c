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

#ifdef _LU_APP_H
/*************************************************-*************************************************
*                                               NOTE
**************************************************-*****************^******************************/
//-- source insight使用UTF-8编码，File->Reload As Encoding...->UTF-8
//-- source insight设置默认编码，Option->Preferences->Files->Default encoding->UTF-8

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/
// 头文件包含区域（实际使用时需要包含必要的头文件）
#include "iot_app.h"
#include "OSAL_Timers.h"
	
/*************************************************-*************************************************
*                                               MACRO
**************************************************-*****************^******************************/
// 宏定义区域
	
#define LU_MAINTAIN_TIMER_EVT                                      0x0002 


	// 背光调整=====================================
#if 1
void lu_allbacklight_set(uint8 en)
{
	if(en)
	{
		HalLedSet(HAL_LED_ALL,HAL_LED_MODE_ON);
	}
	else
	{
		HalLedSet(HAL_LED_ALL,HAL_LED_MODE_OFF);
	}
}
void lu_mainbacklight_set(uint8 color)
{
	HalLedSet(HAL_THREED_RED|HAL_DOUBLE_GREED,HAL_LED_MODE_OFF);
	
	switch(color)
	{
		case BACKLIGHT_OFF:{
			HalLedSet(HAL_THREED_RED|HAL_DOUBLE_GREED,HAL_LED_MODE_OFF);
		}break;
		case BACKLIGHT_GREEN:{
			HalLedSet(HAL_DOUBLE_GREED,HAL_LED_MODE_ON);
		}break;
		case BACKLIGHT_RED:{
			HalLedSet(HAL_THREED_RED,HAL_LED_MODE_ON);
		}break;
		case BACKLIGHT_YELLOW:{
			HalLedSet(HAL_THREED_RED|HAL_DOUBLE_GREED,HAL_LED_MODE_ON);
		}break;
	}
}

void lu_secondbacklight_set(uint8 en)
{
	if(en)
	{
		HalLedSet(HAL_ONE_GREED,HAL_LED_MODE_ON);
	}
	else
	{
		HalLedSet(HAL_ONE_GREED,HAL_LED_MODE_OFF);
	}
}

void lu_outputbacklight_set(uint8 en)
{
	if(en)
	{
		HalLedSet(HAL_ONE_RED,HAL_LED_MODE_ON);
	}
	else
	{
		HalLedSet(HAL_ONE_RED,HAL_LED_MODE_OFF);
	}
}

void lu_backlight_levelset(uint8_t level)
{
	HalLedBlink(HAL_LED_ALL,0,level,10);
}

#endif

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
uint8 lu_app_task_id;
uint8 lu_app_state = 0;
uint8 lu_app_bit = 1;
//延时
void lu_Disp_DelayUs_temp(uint32_t us)
{
	uint32_t i;
    for (i = 0; i < us * 10; i++) {  // 10 是经验值，需根据实际主频调整
        __NOP();  // 空操作，防止被优化（可选）
    }
}
void lu_Disp_DelayMs_temp(uint32_t ms)
{
    while (ms--) {
        lu_Disp_DelayUs_temp(1000);  // 调用1000次微秒延时 = 1毫秒
    }
}

/*************************************************-*************************************************
*                                          FUNCTION PROTOTYPES
**************************************************-*****************^******************************/
// 函数声明区域

// static void Maintain_function(uint32_t current_value) 调用接口 （current_value） 当前传感器数值

/*************************************************-*************************************************
*                                          FUNCTION DEFINITIONS
**************************************************-*****************^******************************/

void lu_display_Screen(char * Mainstring, char * Secondstring){
	
		DIV_Disp_ClearAllPoint(1);
		DIV_Disp_ClearAllPoint(0);

		DIV_Disp_Snprintf(MainScreen,Mainstring);  //显示款型
		second_screen_disp(Secondstring);//显示npn款
	
}


/**
 * @brief 峰值-谷值保持主功能函数
 * @param current_value 当前传感器数值
 * @note 此函数在定时器事件中被调用，自动处理峰值谷值的记录和显示
 */
/**
 * @brief 非阻塞的峰值-谷值保持功能函数
 */
static void Maintain_function_nonblocking(void)
{
    static float max_value = 0;
    static float min_value = 0;
    static uint8_t first_run = 1;
    static uint8_t display_state = 0;
    
    if (first_run) {
        max_value = unitchange_pressure_value;
        min_value = unitchange_pressure_value;
        first_run = 0;
			  
    }
    
    if (unitchange_pressure_value > max_value) max_value = unitchange_pressure_value;
    if (unitchange_pressure_value < min_value) min_value = unitchange_pressure_value;
    
    if (display_state == 0) {

        lu_display_Screen("PERK", "botm");
    } else {
			   
        // 根据单位显示对应的最大最小值格式
        switch(unitconver_status)
        {
            case bAr:{
                main_screen_dispfloat("%0.3f", max_value);
                second_screen_dispfloat("%0.3f", min_value);
            }break;
            case inHG:{
                main_screen_dispfloat("%0.1f", max_value);
                second_screen_dispfloat("%0.1f", min_value);
            }break;
            case KgF:{
                main_screen_dispfloat("%0.3f", max_value);
                second_screen_dispfloat("%0.3f", min_value);
            }break;
            case KPR:{
                main_screen_dispfloat("%0.1f", max_value);
                second_screen_dispfloat("%0.1f", min_value);
            }break;
            case MMHG:{
                main_screen_dispfloat("%0.f", max_value);
                second_screen_dispfloat("%0.f", min_value);
            }break;
            case MPR:{
                main_screen_dispfloat("%0.3f", max_value);
                second_screen_dispfloat("%0.3f", min_value);
            }break;
            case PSI:{
                main_screen_dispfloat("%0.2f", max_value);
                second_screen_dispfloat("%0.2f", min_value);
            }break;
        }
				
    }
    
    display_state = !display_state;
}

/**
 * @brief 重置峰值谷值功能状态
 */
static void Reset_Maintain_function(void)
{
    // 这里可以添加重置峰值谷值状态的代码
    // 例如重置最大值最小值等
}


uint8 lu_app_key_callback(uint8 cur_keys, uint8 pre_keys, uint32 poll_time_milliseconds)
{
    uint8  k;
    uint8  key_mask = HAL_KEY_1;
    uint8  scan_flag = 1;
    uint8  press_keys = 0;      // 按下的按键
    uint8  release_keys = 0;    // 释放的按键
    static uint8 islongorshortpress = 0; // 长按或短按标志
    uint8_t longpress_morethan_3s_keys = 0; // 长按超过3秒的按键
    
    // 添加K2长按检测变量
    static uint8_t k1_k2_combo_press_time = 0; // K1和K2同时按下的时间计数
    static uint8_t last_k1_k2_state = 0;       // 上一次K1和K2同时按下的状态
    static uint8_t k1_released = 0;            // K1释放检测标志

    // 只处理有效的按键
    cur_keys &= HU_APP_KEY_MASK;
    pre_keys &= HU_APP_KEY_MASK;

    // 检测K1是否松开
    if ((pre_keys & HAL_KEY_MODE) && !(cur_keys & HAL_KEY_MODE)) {
        // K1从按下变为松开
        k1_released = 1;
    } else if (cur_keys & HAL_KEY_MODE) {
        // K1被按下，重置释放标志
        k1_released = 0;
    }

    // 检测K1和K2同时按下
    uint8_t current_k1_k2_combo = ((cur_keys & HAL_KEY_MODE) && (cur_keys & HAL_KEY_2)) ? 1 : 0;
    
    if (current_k1_k2_combo) {
        // K1和K2同时按下
        if (last_k1_k2_state) {
            // 持续按下，增加时间计数
            k1_k2_combo_press_time++;
            
            // 检测是否达到2秒（假设poll_time_milliseconds为100ms，20次为2秒）
            if (k1_k2_combo_press_time >= 20) {
                // K1和K2同时按下超过2秒
                longpress_morethan_3s_keys |= (HAL_KEY_MODE | HAL_KEY_2);
            }
        } else {
            // 刚开始同时按下，重置时间计数
            k1_k2_combo_press_time = 1;
            last_k1_k2_state = 1;
        }
    } else {
        // K1和K2没有同时按下，重置状态
        k1_k2_combo_press_time = 0;
        last_k1_k2_state = 0;
    }

    // 遍历所有按键
    for (k = 0; k < 8; k++,key_mask<<=1)
    {
        IOT_WATCHDOG_RESET(); // 喂狗

        // 跳过无效按键
        if (!(key_mask & HU_APP_KEY_MASK))
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
        }
        // 按键释放处理
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
                
                release_keys |= key_mask;
            }
        }
    }

    // 修改为检测K1和K2同时长按超过2秒
    if((longpress_morethan_3s_keys & (HAL_KEY_MODE | HAL_KEY_2)) == (HAL_KEY_MODE | HAL_KEY_2))
    {
        if(system_state == PEAKTOVALLEY_STATE && lu_app_bit)
        {
            lu_app_state = !lu_app_state;  // 切换状态
            
            if (lu_app_state) {
                // 进入峰值模式 - 启动500ms定时器
                lu_mainbacklight_set(BACKLIGHT_RED);
                Reset_Maintain_function(); // 重置峰值模式状态
                osal_start_reload_timer(lu_app_task_id, LU_MAINTAIN_TIMER_EVT, 500); // 500ms重载定时器
            } else {
                // 退出峰值模式 - 停止定时器
                lu_mainbacklight_set(BACKLIGHT_GREEN);
                osal_stop_timerEx(lu_app_task_id, LU_MAINTAIN_TIMER_EVT);
				
				system_state=RUN_STATE;
				second_screen_tranfromevt(SECONDSCREEN_DISPSETVALUE);// 副屏刷新设定值
            }
            
            lu_app_bit = 0;
            
            // 重置组合按键状态，防止重复触发
            k1_k2_combo_press_time = 0;
            last_k1_k2_state = 0;
        }
    }
    
    // 根据K1释放状态来重置lu_app_bit
    if (k1_released) {
        // K1松开后，可以重新允许状态切换
        lu_app_bit = 1;
        k1_released = 0; // 重置释放标志
		
		if(system_state == RUN_STATE)
		{
			iot_app_keylock=0;
			
		}
		
    }
    
    // 移除阻塞的延时调用
    // if(lu_app_state){Maintain_function(); lu_Disp_DelayMs_temp(500);}
        
    return scan_flag;
}


void lu_app_init(uint8 task_id)
{
    lu_app_task_id = task_id; // 保存任务ID
	
    lu_allbacklight_set(BACKLIGHT_ON);		// 打开背光
	
    // 注册事件
    //osal_start_reload_timer(lu_app_task_id,LU_APP_TIMER_EVT,HU_APP_TIMER_INTERVAL);
	
    // 注册按键回调函数
    HalKeyCallbackRegister(lu_app_key_callback);
	
}

uint16 lu_app_process_event(uint8 task_id, uint16 events)
{
    (void)task_id; // 显式忽略未引用参数（避免编译器警告）
     
    // 处理系统消息事件
    if ( events & SYS_EVENT_MSG )
    {
        // 接收消息队列中的消息
        uint8* msg_packet = (uint8 *)osal_msg_receive(lu_app_task_id);
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
            msg_packet = (uint8 *)osal_msg_receive(lu_app_task_id);
        }

        // 返回未处理的事件（清除已处理的SYS_EVENT_MSG）
        return (events ^ SYS_EVENT_MSG);
    }
    
    // 处理峰值模式定时器事件 - 每500ms自动切换显示
    if(events & LU_MAINTAIN_TIMER_EVT) {
        if(lu_app_state) {
            Maintain_function_nonblocking(); // 非阻塞的显示切换
        }
        return (events ^ LU_MAINTAIN_TIMER_EVT);
    }
	
	//判断他是否成功调度起来了，添加成功就会进你的事件管理
	if(events & HU_APP_TIMER_EVT)
	{
		// 事件到了就执行
		osal_stop_timerEx(lu_app_task_id,HU_APP_TIMER_EVT);
		
		return (events ^ HU_APP_TIMER_EVT);
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

