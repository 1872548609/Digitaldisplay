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

//数据处理==================================================
#define set_no  0x00 //开机无设置
#define set_P1 	0x01
#define set_P2 	0x02
#define set_Hi1 0x04
#define set_Lo1 0x08
#define set_Hi2 0x10
#define set_Lo2 0x20

uint8_t nowsetwhichyc = set_no;  //正在设置哪个
uint8_t NOWCANSETWICH = set_no; //可以设置那些
uint8_t NOWChoice[6]={0};  //可设置选项
uint8_t NOWChoicelength=0;


#define EPSILON 1e-6f   //误差容许
#define MIN_DECIMAL_PRECISION 0.0001f   // 最小小数精度（根据实际需求调整）

float Current_pressure_value=0.0f;	
float unitchange_pressure_value =0.0f;

float P1_Value 	= 0.0;
float P2_Value 	= 0.0;
float Hi1_Value = 0.0;
float Lo1_Value = 0.0;		
float Hi2_Value = 0.0;
float Lo2_Value = 0.0; 

typedef struct { // 参数配置结构体
    float *value;        // 参数地址（如 &P1_Value）
    float max_value;     // 最大值（按单位）默认1.05
    float *peer_value;   // 关联参数（如 Hi1 对应 Lo1，NULL 表示无）
    uint8_t peer_check;  // 限制规则：0=无限制, 1=必须小于关联参数, 2=必须大于关联参数
} ParamConfig;

// 参数配置表（顺序与 set_P1/set_Hi1 等宏定义一致）
const ParamConfig param_table[] = {
    {&P1_Value, 	1.05, NULL, 		0},       // set_P1: 无限制
    {&P2_Value, 	1.05, NULL, 		0},       // set_P2: 无限制
    {&Hi1_Value, 	1.05, &Lo1_Value, 	1}, // set_Hi1: 必须 > Lo1
    {&Lo1_Value, 	1.05, &Hi1_Value, 	2}, // set_Lo1: 必须 < Hi1
    {&Hi2_Value, 	1.05, &Lo2_Value, 	1}, // set_Hi2: 必须 > Lo2
    {&Lo2_Value, 	1.05, &Hi2_Value, 	2}, // set_Lo2: 必须 < Hi2
};
// 短按设置应差参数
void short_setycvalue(uint8_t addordown)  
{
	float max_val = 0; // 参数设置上限 
	float increment = 0;// 增量
	
    // 获取当前参数配置
    const ParamConfig *cfg = 0; // 假设 set_P1=0x01 对应索引 0
    switch(nowsetwhichyc)       // 选择要设定的值
	{
		case set_P1:{
			 cfg=&param_table[0];
		}break;
		case set_P2:{
			 cfg=&param_table[1];
		}break;
		case set_Hi1:{
			 cfg=&param_table[2];
		}break;
		case set_Lo1:{
			 cfg=&param_table[3];
		}break;
		case set_Hi2:{
			 cfg=&param_table[4];
		}break;
		case set_Lo2:{
			 cfg=&param_table[5];
		}break;
	}
	// 获取设置参数
	float *current_value = cfg->value;	
	
	// 选择不同单位的上限和增量  
	switch(unitconver_status)     
	{
		case bAr:{
				max_val=1.05;
				increment=0.001f;
		}break;
		case inHG:{
				max_val=31.0;
				increment=0.01f;
		}break;
		case KgF:{
				max_val=105.0;
				increment=0.1f;
		}break;
		case KPR:{
				max_val=105.0;
				increment=0.1f;
		}break;
		case MMHG:{
				max_val=787.5;
				increment=1.0f;
		}break;
		case MPR:{
				max_val=0.105;
				increment=0.001f;
		}break;
		case PSI:{
				max_val=15.22;
				increment=0.01f;
		}break;
	}
	
	// 暂存
	float temp = *current_value;    
				
  // 处理增减逻辑
	if (addordown) {  //如果增加
		if (temp < max_val) {
			temp += increment; // 增加
		} else {
			temp = max_val;//强制设为上限
			// 显示上
		}
	} else {
		if (temp > -max_val) {
			temp -=	increment; // 减少
		} else {
			temp = -max_val; // 强制设为下限
			// 显示下限	
		}
	}		
						
    // 处理 Hi/Lo 相互限制
    if (cfg->peer_value != NULL) {
        float peer_val = *cfg->peer_value;
        if (cfg->peer_check == 1 && temp < peer_val) { // 必须大于关联参数
            temp = peer_val;
        } else if (cfg->peer_check == 2 && temp > peer_val) { // 必须小于关联参数
            temp = peer_val;
        }
    }

    // 更新参数值
    *current_value = temp;
}

// 背光调整=====================================
void iot_allbacklight_set(uint8 en)
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
void iot_mainbacklight_set(uint8 color)
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

void iot_secondbacklight_set(uint8 en)
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

void iot_outputbacklight_set(uint8 en)
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

void iot_backlight_levelset(uint8_t level)
{
	HalLedBlink(HAL_LED_ALL,0,level,10);
}

// 任务区=====================================
uint8 iot_app_key_callback(uint8 cur_keys, uint8 pre_keys, uint32 poll_time_milliseconds)
{
    uint8  k;
    uint8  key_mask = HAL_KEY_1;
    uint8  scan_flag = 1;
    uint8  press_keys = 0;      // 按下的按键
    //uint8  hold_keys = 0;       // 按住的按键
	uint8  release_keys = 0;    // 释放的按键
	static uint8 islongorshortpress = 0; // 长按或短按标志
	uint8_t longpress_morethan_3s_keys = 0; // 长按超过3秒的按键
	uint8_t longpress_morethan_4s_keys = 0;
	
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
            // 超长按检测（>3s）
			if (hal_key_press_time_count[k] == 30)
            {
				islongorshortpress = 2;
                longpress_morethan_3s_keys |= key_mask;
            }
			if (hal_key_press_time_count[k] == 50)
            {
				islongorshortpress = 2;
                longpress_morethan_4s_keys |= key_mask;
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
	
	if(press_keys & HAL_KEY_MODE)
	{
		if(system_state == MENU_STATE)
		{
			Menu_Execute(MENU_CBK_MODE);
		}
	}
	
	if(press_keys & HAL_KEY_LEFT_ADD)
	{
		if(system_state == MENU_STATE)
		{
			Menu_Execute(MENU_CBK_ADD);
		}
	}
	
	if(press_keys & HAL_KEY_RIGHT_SUB)
	{
		if(system_state == MENU_STATE)
		{
			Menu_Execute(MENU_CBK_SUB);
		}
	}
	
	if(longpress_morethan_3s_keys & HAL_KEY_MODE)
	{
		if(system_state == RUN_STATE)
		{
			MenuSystem_Start();		// 启动菜单
			Menu_Enter(); // 进入根下第一个菜单
			Menu_Execute(INCALLBACK);
			system_state = MENU_STATE; 
		}
		else if(system_state == MENU_STATE)
		{
			MenuSystem_Stop();		// 关闭菜单
			Menu_Execute(OUTCALLBACK);
			system_state = RUN_STATE; 
		}
		
	}
	
	if(longpress_morethan_4s_keys & HAL_KEY_MODE)
	{
		if(system_state == MENU_STATE)
		{
			if((strcmp(menuSystem.current->text,"NORMSET") == 0))
			{
				Menu_Next();
				Menu_Execute(INCALLBACK);
			}
		}
	}
	
	return scan_flag;
}

void iot_app_init(uint8 task_id)
{
    iot_app_task_id = task_id; // 保存任务ID
	
	DIV_Disp_Snprintf(MainScreen,"RL01");  //显示款型
	DIV_Disp_Snprintf(SecondScreen," NPN");//显示npn款
	
	iot_allbacklight_set(BACKLIGHT_ON);
	
	
	MenuItem* root = CreateTestMenu(); // 动态创建菜单，所有菜单都在这个函数里编辑好
    MenuSystem_Init(root);	// 初始化系统菜单

	osal_start_reload_timer(iot_app_task_id,IOT_APP_TIMER_EVT,IOT_APP_TIMER_INTERVAL);
	
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
	
	
	
	if(events & IOT_APP_TIMER_EVT)
	{
		
		
		return (events ^ IOT_APP_TIMER_EVT);
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
