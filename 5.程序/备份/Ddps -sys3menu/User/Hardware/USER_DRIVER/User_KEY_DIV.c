#include "User_Key_Div.h"

Key_Management key_management = {0,KEY_STATE_RELEASED};

void Div_Key_Scan(void)
{
	  if(HAL_KEY_Read(READ_KEY1)==Down||HAL_KEY_Read(READ_KEY2)==Down||HAL_KEY_Read(READ_KEY3)==Down)
			{
				   switch(key_management.Key_State)
							{
								case  KEY_STATE_RELEASED:
								{
									      key_management.Key_State=KEY_STATE_DEBOUNCING;
															key_management.Key_tick=0;
															
								}break;
								case KEY_STATE_DEBOUNCING:
								{
										     if(++key_management.debounceCount>=DEBOUNCE_TIME/10)
															{
																	key_management.Key_State= KEY_STATE_PRESSED;
																
																	if(HAL_KEY_Read(READ_KEY1)==Down){key_management.WichKeyDown|=KEY1;}
																	if(HAL_KEY_Read(READ_KEY2)==Down){key_management.WichKeyDown|=KEY2;}
																	if(HAL_KEY_Read(READ_KEY3)==Down){key_management.WichKeyDown|=KEY3;}
																	
																 key_management.Key_StartPressTime=key_management.Key_tick;
																 key_management.debounceCount=0;
														}
								}break;							
								case KEY_STATE_PRESSED:
								{
										uint32_t duration  = key_management.Key_tick-key_management.Key_StartPressTime;
										if(duration>=LONG_PRESS_2S)
											{
														key_management.Key_State=KEY_STATE_LONGPRESS_2S_MODE;
														key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=1;
														
														if(HAL_KEY_Read(READ_KEY1)==Down){key_management.WichKeyDown|=KEY1;}
														if(HAL_KEY_Read(READ_KEY2)==Down){key_management.WichKeyDown|=KEY2;}
														if(HAL_KEY_Read(READ_KEY3)==Down){key_management.WichKeyDown|=KEY3;}
											}
								}break;
								case KEY_STATE_LONGPRESS_2S_MODE:
								{
								  	uint32_t duration  = key_management.Key_tick-key_management.Key_StartPressTime;
											if(duration>=LONG_PRESS_4S)	{	
												key_management.Key_State=KEY_STATE_LONGPRESS_4S_MODE;
											key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV=1;

												if(HAL_KEY_Read(READ_KEY1)==Down){key_management.WichKeyDown|=KEY1;}
												if(HAL_KEY_Read(READ_KEY2)==Down){key_management.WichKeyDown|=KEY2;}
												if(HAL_KEY_Read(READ_KEY3)==Down){key_management.WichKeyDown|=KEY3;}

											}
													
								}break;
								case KEY_STATE_LONGPRESS_4S_MODE:
								{
								      
								}break;
							}
			}
			else
			{
				    switch(key_management.Key_State)
								{
										case KEY_STATE_PRESSED:{
										    
														key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV=1;
														key_management.Key_State=KEY_STATE_RELEASED;
										}break;
										case KEY_STATE_DEBOUNCING:
										{
														key_management.WichKeyDown=0x00;
														key_management.debounceCount=0;
														key_management.Key_State=KEY_STATE_RELEASED;
										}break;
									case KEY_STATE_LONGPRESS_2S_MODE:
									{
												   key_management.WichKeyDown=0x00;
															key_management.Key_State=KEY_STATE_RELEASED;
									}break;		
									case KEY_STATE_LONGPRESS_4S_MODE:
									{
																key_management.WichKeyDown=0x00;
									      	key_management.Key_State=KEY_STATE_RELEASED;
									}break;									
								}								
			}
}

void DIV_Key_Init(void)
{
	HAL_KEY_Init();
}
