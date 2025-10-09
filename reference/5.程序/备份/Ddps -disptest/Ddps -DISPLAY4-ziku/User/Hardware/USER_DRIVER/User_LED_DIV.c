#include "USER_LED_DIV.h"

LED_Management THREERE_M = {0,THREERED};
LED_Management ONERED_M		= {0,ONERED};
LED_Management DOUBGED_M	= {0,DOUBGED};
LED_Management ONEGED_M 	= {0,ONEGED};
LED_Management ALLLED_M 	= {0,ALLLED};

void DIV_LED_Init(void)
{
	  HAL_LED_Init();
}

void DIV_LED_TURN(LED_Management MA)
{
	  	switch(MA.LED_ID)
				{
					case THREERED:{
								  if(MA.LED_Crt.Crt_Bit.state)
										{
													THRERED_ON
										}
										else
										{
													THRERED_OFF
										}
										if(MA.LED_Crt.Crt_Bit.blink)
										{
										   THREERED_FLASH
										}
					}break;
						case ONERED:{
								  if(MA.LED_Crt.Crt_Bit.state)
										{
													ONERED_ON
										}
										else
										{
													ONERED_OFF
										}
					}break;
						case DOUBGED:{
								   if(MA.LED_Crt.Crt_Bit.state)
										{
													DOUBGED_ON
										}
										else
										{
													DOUBGED_OFF
										}
					}break;
						case ONEGED:{
								   if(MA.LED_Crt.Crt_Bit.state)
										{
													ONEGED_ON
										}
										else
										{
													ONEGED_OFF
										}
					}break;
						case ALLLED:{
						    if(MA.LED_Crt.Crt_Bit.state)
										{
													ALLLED_ON
										}
										else
										{
													ALLLED_OFF
										}
						}break;
					
				}
}

void DIV_LED_Control(LED_Management MA)
{
			 DIV_LED_TURN(MA);
}
