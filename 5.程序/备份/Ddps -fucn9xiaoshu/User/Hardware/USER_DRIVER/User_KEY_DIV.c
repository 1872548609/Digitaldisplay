#include "User_Key_Div.h"

Key_Management key_management = {0,KEY_STATE_RELEASED};

void KEY1_Isr(void)       
{
			APM_EVAL_DelayMs(10);//消抖
			if(HAL_KEY_Read(KEY1_PORT,KEY1_PIN)==Down)
			{
			    key_management.WichKeyDown |= KEY1;//KEY1按下
							key_management.PreWichKeyDown|=KEY1;
							key_management.Key_start=1;//开始计时
							key_management.Key_State=KEY_STATE_PRESSED;//变成按下状态
			}
   else    //释放按键
			{
				   key_management.WichKeyDown &= ~(KEY1);//KEY1释放
			}
			
			
}

void KEY2_Isr(void)
{
	  APM_EVAL_DelayMs(10);//消抖
			if(HAL_KEY_Read(KEY2_PORT,KEY2_PIN)==Down)
			{
			    key_management.WichKeyDown |= KEY2;//KEY2按下
							key_management.PreWichKeyDown|=KEY2;
							key_management.Key_start=1;//开始计时
								key_management.Key_State=KEY_STATE_PRESSED;//变成按下状态
			}
   else    //释放按键
			{
				   key_management.WichKeyDown &= ~(KEY2);//KEY2释放
			}
}


void KEY3_Isr(void)
{

   APM_EVAL_DelayMs(10);//消抖
			if(HAL_KEY_Read(KEY3_PORT,KEY3_PIN)==Down)
			{
			    key_management.WichKeyDown |= KEY3;//KEY3按下
				   key_management.PreWichKeyDown|=KEY3;
							key_management.Key_start=1;//开始计时
							key_management.Key_State=KEY_STATE_PRESSED;//变成按下状态
			}
   else    //释放按键
			{
				   key_management.WichKeyDown &= ~(KEY3);//KEY3释放
			}
}
