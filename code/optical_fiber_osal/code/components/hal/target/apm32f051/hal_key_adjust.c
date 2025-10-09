#include "hal_key_adjust.h"

uint16 current_value = 0;			//当前设定值
uint8 current_rate = 0;				//当前速率档位
uint8 current_key = 0;				//当前按下的按键
uint16 keypress_starttime = 0;		//按键按下的起始时间
uint8 key_adjust_taskid = 0;		//任务id

bool key_adjust_add = false;		//加键按下标志
bool key_adjust_subtract = false;	//减键按下标志

//检测按键状态
void key_adjust_check(void)
{
	//当加按键按下
	if(!(HAL_KEY3_READ()))
	{
		//如果当前按键未使能时
		if(!key_adjust_add)
		{
			key_adjust_add = true;							//使能按键
			keypress_starttime = osal_GetSystemClock();		//获取当前时间
		}
	}
	else
	{
		key_adjust_add = false;
	}
	//当减按键按下
	if(!(HAL_KEY4_READ()))
	{
		if(!key_adjust_subtract)
		{
			key_adjust_subtract = true;						//使能按键
			keypress_starttime = osal_GetSystemClock();		//获取当前时间
		}
	}
	else
	{
		key_adjust_subtract = false;
	}
}

//按键长按处理
void key_adjust_longpress(void)
{
	if(key_adjust_add || key_adjust_subtract)							//当加键或减键按下时
	{
		uint32 current_time = osal_GetSystemClock();					//获取现在的时间
		uint32 difference = current_time - keypress_starttime;			//计算按下时长
		uint16 temp = 0;
		
		//根据长按的时间选择速率
		if(difference <= KEYLONGPRESS_THRESHOLD_6)						//长按时间在0-6s时速率为1档
		{
			temp = RATE_LEVEL_1;
		}
		else if(difference <= KEYLONGPRESS_THRESHOLD_14)				//长按时间在6-14s时速率为2档
		{
			temp = RATE_LEVEL_2;
		}
		else if(difference <= KEYLONGPRESS_THRESHOLD_20)				//长按时间在14-20s时速率为3档
		{
			temp = RATE_LEVEL_3;
		}
		
		//调整当前值
		if(key_adjust_add) 
		{
			current_value += temp;
		}
		if(key_adjust_subtract)
		{
			current_value -= temp;
		}
		if(current_value >= VALUE_MAX) current_value = VALUE_MAX;
		else if(current_value <= VALUE_MIN) current_value = VALUE_MIN;
//		current_value = (current_value > VALUE_MAX) ? VALUE_MAX : (current_value < VALUE_MIN) ? VALUE_MIN : current_value;
	}
}

//长按加键和减键的初始化
void key_adjust_init(uint8 taskId)
{
	key_adjust_taskid = taskId;
	osal_start_timerEx(key_adjust_taskid,KEY_ADJUST_EVENT,KEY_CHECKTIME);
}

//输入当前设定值
void key_adjust_setvalue(uint16 value)
{
	current_value = (value > VALUE_MAX) ? VALUE_MAX : (value < VALUE_MIN) ? VALUE_MIN : value;
}

//获取当前设定值
uint16 key_adjust_getvalue(void)
{
	return current_value;
}

//长按加键和减键的任务处理
uint16 key_adjust_process_event(uint8 taskId,uint16 events)
{
	(void)taskId;
	if(events & KEY_ADJUST_EVENT)
	{
		key_adjust_check();															//检测按键
		key_adjust_longpress();														//进行长按处理
		osal_start_timerEx(taskId,KEY_ADJUST_EVENT,KEY_CHECKTIME);					//重启定时器，形成循环检测
		return (events ^ KEY_ADJUST_EVENT);											//清除事件标志位
	}
	return 0;
}

