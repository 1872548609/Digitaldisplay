#include "User_Key_Div.h"

Key_Management key_management = {0,KEY_STATE_RELEASED};

void DIV_KEY_Init(void)
{
		 HAL_KEY_Init();
			key_management.Key_start=1;
}

void DIV_key_read(void)
{
		 //按键记录时间戳====================================	
				if(key_management.Key_start)//如果开始计时
				{   
								key_management.Key_tick++;//时间戳自增
								switch(key_management.Key_State)
								{
									case KEY_STATE_PRESSED:{

										
													if(key_management.Key_tick>1000)
													{   
																key_management.PreWichKeyDown=0;   //如果进入长按就清掉短按的，不然长按进了别的模式短按是多个按键记录就不触发了
																key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=1;
																key_management.Key_State=KEY_STATE_LONGPRESS_2S_MODE;
													}
									}break;
									case KEY_STATE_LONGPRESS_2S_MODE:{
										

														if(key_management.Key_tick>3000)
													{
																key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV=1;
																key_management.Key_State=KEY_STATE_LONGPRESS_4S_MODE;
													}
									}break;
									case KEY_STATE_LONGPRESS_4S_MODE:{

									}break;
								}
					
							if(key_management.WichKeyDown==0){    //如果按键都释放了
											switch(key_management.Key_State)
											{
												case KEY_STATE_PRESSED:{ 
																			
																	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV=1;

													
																	key_management.Key_State=KEY_STATE_PRESSED;
												}break;
												case KEY_STATE_LONGPRESS_2S_MODE:{
													
													
																key_management.Key_State=KEY_STATE_PRESSED;
												}break;
												case KEY_STATE_LONGPRESS_4S_MODE:{
																key_management.Key_State=KEY_STATE_PRESSED;
												}break;
											}

								key_management.Key_start=0;      //关闭计时
								key_management.Key_tick=0;
								}
								
				}
}


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
