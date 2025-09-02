#include "User_Disp_Hal.h"



//初始化断码屏引脚
void HAL_Disp_Init(void)
{
		GPIO_Config_T gpioConfig;
		
		RCM_EnableAHBPeriphClock(LED_ALL_PERIPH_PORT);

		gpioConfig.pin = CS_PIN|WR_PIN|DATA_PIN;
		gpioConfig.mode = GPIO_MODE_OUT;
		gpioConfig.outtype = GPIO_OUT_TYPE_PP;
		gpioConfig.speed = GPIO_SPEED_50MHz;
		gpioConfig.pupd = GPIO_PUPD_PU;
		GPIO_Config(GPIOB, &gpioConfig);
}

//写命令
void HAL_Disp_W_Cmd(uint8_t id,uint16_t cmd)
{
		 Disp_W_CS(0);
			///写id
		 int i;
			for(i=0;i<Id_length;i++)
			{
					  Disp_W_WR(0);
							Disp_W_DATA(id & (0x04>>i));
							APM_EVAL_DelayUs(1);
				   Disp_W_WR(1);
							APM_EVAL_DelayUs(1);
			}
		 //写命令代码
			for(i=0;i<Cmd_Length;i++)
			{
					  Disp_W_WR(0);
							Disp_W_DATA(id & (0x200>>i));
							APM_EVAL_DelayUs(1);
				   Disp_W_WR(1);
							APM_EVAL_DelayUs(1);
			}
			Disp_W_CS(1);
}

//写数据
void HAL_Disp_W_Data(uint8_t id,uint16_t address,uint16_t data)
{
	  Disp_W_CS(0);
			///写id
		 int i;
			for(i=0;i<Id_length;i++)
			{
					  Disp_W_WR(0);
							Disp_W_DATA(id & (0x04>>i));
							APM_EVAL_DelayUs(1);
				   Disp_W_WR(1);
							APM_EVAL_DelayUs(1);
			}
		 //写地址
			for(i=0;i<address_Length;i++)
			{
					  Disp_W_WR(0);
							Disp_W_DATA(address & (0x20>>i));
							APM_EVAL_DelayUs(1);
				   Disp_W_WR(1);
							APM_EVAL_DelayUs(1);
			}
			
			//数据
			for(i=0;i<Data_Length;i++)
			{
					  Disp_W_WR(0);
							Disp_W_DATA(data & (0x8>>i));
							APM_EVAL_DelayUs(1);
				   Disp_W_WR(1);
							APM_EVAL_DelayUs(1);
			}
			Disp_W_CS(1);
}

//参数可以是  W_CS 中一个
void HAL_Disp_Write_Pin(uint8_t cmd,uint8_t level)
{
	  switch(cmd)
			{
				case W_CS:{
					if(level)
					{
						GPIO_SetBit(CS_PORT,CS_PIN);
					}
					else
					{
						GPIO_ClearBit(CS_PORT,CS_PIN);
					}
				}break;
				case W_WR:{
					if(level)
					{
						GPIO_SetBit(WR_PORT,WR_PIN);
					}
					else
					{
						GPIO_ClearBit(WR_PORT,WR_PIN);
					}
				}break;
				case W_DATA:{
					if(level)
					{
						GPIO_SetBit(DATA_PORT,DATA_PIN);
					}
					else
					{
						GPIO_ClearBit(DATA_PORT,DATA_PIN);
					}
				}break;				
			}
}
