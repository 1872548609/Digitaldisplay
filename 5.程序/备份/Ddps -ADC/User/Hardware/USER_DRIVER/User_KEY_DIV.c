#include "User_Key_Div.h"

Key_Management key_management = {0,KEY_STATE_RELEASED};

void Div_Key_Scan(void)
{
	  if(HAL_KEY_Read(READ_KEY1)==Down)
			{
				   switch(key_management.Key1_State)
							{
								case  KEY_STATE_RELEASED:
								{
									      key_management.Key1_State=KEY_STATE_DEBOUNCING;
															key_management.Key_tick=0;
															
								}break;
								case KEY_STATE_DEBOUNCING:
								{
										     if(++key_management.debounceCount>=DEBOUNCE_TIME/10)
															{
																	key_management.Key1_State= KEY_STATE_PRESSED;
																 key_management.Key1_StartPressTime=key_management.Key_tick;
																 key_management.debounceCount=0;
														}
								}break;							
								case KEY_STATE_PRESSED:
								{
										uint32_t duration  = key_management.Key_tick-key_management.Key1_StartPressTime;
										if(duration>=LONG_PRESS_2S)
											{
														key_management.Key1_State=KEY_STATE_LONGPRESS_2S_MODE;
														THRERED_ON
											}
								}break;
								case KEY_STATE_LONGPRESS_2S_MODE:
								{
								  	uint32_t duration  = key_management.Key_tick-key_management.Key1_StartPressTime;
											if(duration>=LONG_PRESS_4S)
											{
														key_management.Key1_State=KEY_STATE_LONGPRESS_4S_MODE;
														THRERED_OFF
											}
								}break;
								case KEY_STATE_LONGPRESS_4S_MODE:
								{
								
								}break;
							}
			}
			else
			{
				    switch(key_management.Key1_State)
								{
										case KEY_STATE_PRESSED:{
										
														key_management.Key1_State=KEY_STATE_RELEASED;
										}break;
											case KEY_STATE_DEBOUNCING:
										{
																		key_management.debounceCount=0;
										}break;
									case KEY_STATE_LONGPRESS_2S_MODE:
									{
												
															key_management.Key1_State=KEY_STATE_RELEASED;
									}break;		
									case KEY_STATE_LONGPRESS_4S_MODE:
									{
									      	key_management.Key1_State=KEY_STATE_RELEASED;
									}break;									
								}								
			}
}
