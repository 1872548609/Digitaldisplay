#include "User_Disp_Hal.h"

//延时
void HAL_Disp_DelayUs(uint32_t us)
{
	  	APM_EVAL_DelayUs(us);
}

//清屏
void HAL_Disp_ClearALL(void)
{
		uint16_t i;
		for(i=0;i<MAX_ADDR;i++)
		{
								HAL_Disp_W_Data(dataid,i,0x00);    
		}
}

//填充
void HAL_Disp_Fill(uint8_t data)
{
	uint16_t i;
		for(i=0;i<MAX_ADDR;i++)
		{
								HAL_Disp_W_Data(dataid,i,data);    
		}
}

//启动配置
void HAL_Disp_Config(void)
{
		//设置LCD偏置 4com
				HAL_Disp_W_Cmd(cmdid,BIAS_1_3_4_COMS);
			//打开LCD偏置
				HAL_Disp_W_Cmd(cmdid,LCD_ON);
			//打开系统时钟，默认RC256k
				HAL_Disp_W_Cmd(cmdid,SYS_EN);   
			//清屏
			HAL_Disp_ClearALL();
}

//初始化断码屏引脚
void HAL_Disp_Init(void)
{
		GPIO_Config_T gpioConfig;
		
		RCM_EnableAHBPeriphClock(DISP_ALL_PERIPH_PORT);

	 gpioConfig.pin = CS_PIN|WR_PIN|DATA_PIN;
		gpioConfig.mode = GPIO_MODE_OUT;
		gpioConfig.outtype = GPIO_OUT_TYPE_PP;
		gpioConfig.speed = GPIO_SPEED_50MHz;
		gpioConfig.pupd = GPIO_PUPD_NO;
		GPIO_Config(ALL_PORT, &gpioConfig);	

	 HAL_Disp_Config();
}


//连续写数据
void HAL_Disp_WrSeriesData(uint8_t id,uint16_t startaddress, uint8_t * data, uint16_t length)
{
	   Disp_W_CS(0);
				HAL_Disp_DelayUs(1);
				///写id
				int i;
				for(i=0;i<Id_length;i++)
				{
								Disp_W_WR(0);
								Disp_W_DATA(id & (0x04>>i));
								HAL_Disp_DelayUs(1);
								Disp_W_WR(1);
								HAL_Disp_DelayUs(1);
				}
				//写地址
				for(i=0;i<address_Length;i++)
				{
								Disp_W_WR(0);
								Disp_W_DATA(startaddress & (0x20>>i));
								HAL_Disp_DelayUs(1);
								Disp_W_WR(1);
								HAL_Disp_DelayUs(1);
				}
				for(i=0;i<length;i++)
			{
				//数据
				int j;
				for(j=0;j<Data_Length;j++)
				{		
							Disp_W_DATA(data[i] & (0x01<<j));
							HAL_Disp_DelayUs(WR_Delay_us);
							Disp_W_WR(0);
							HAL_Disp_DelayUs(WR_Delay_us);
				   Disp_W_WR(1);
							HAL_Disp_DelayUs(1);
				}				
			}
			Disp_W_CS(1);
			HAL_Disp_DelayUs(1);
}

//写命令
void HAL_Disp_W_Cmd(uint8_t id,uint16_t cmd)
{
		 Disp_W_CS(0);
			HAL_Disp_DelayUs(1);
			///写id
		 int i;
			for(i=0;i<Id_length;i++)
			{
					  Disp_W_WR(0);
							Disp_W_DATA(id & (0x04>>i));
							HAL_Disp_DelayUs(1);
				   Disp_W_WR(1);
							HAL_Disp_DelayUs(1);
			}
		 //写命令代码
			for(i=0;i<Cmd_Length;i++)
			{
					  Disp_W_WR(0);
							Disp_W_DATA(cmd & (0x80>>i));
							HAL_Disp_DelayUs(1);
				   Disp_W_WR(1);
							HAL_Disp_DelayUs(1);
			}
			Disp_W_CS(1);
			HAL_Disp_DelayUs(1);
}

//写数据
void HAL_Disp_W_Data(uint8_t id,uint16_t address,uint16_t data)
{
			Disp_W_CS(0);
			HAL_Disp_DelayUs(1);
			///写id
		 int i;
			for(i=0;i<Id_length;i++)
			{			 
							Disp_W_DATA(id & (0x04>>i));
							HAL_Disp_DelayUs(WR_Delay_us);
							Disp_W_WR(0);
							HAL_Disp_DelayUs(WR_Delay_us);
				   Disp_W_WR(1);
							HAL_Disp_DelayUs(1);
			}
		 //写地址
			for(i=0;i<address_Length;i++)
			{
					 
							Disp_W_DATA(address & (0x20>>i));
							HAL_Disp_DelayUs(WR_Delay_us);
							Disp_W_WR(0);
							HAL_Disp_DelayUs(WR_Delay_us);
				   Disp_W_WR(1);
							HAL_Disp_DelayUs(1);
			}
			
			//数据
			for(i=0;i<Data_Length;i++)
			{
					 
							Disp_W_DATA(data & (0x01<<i));
							HAL_Disp_DelayUs(WR_Delay_us);
							Disp_W_WR(0);
							HAL_Disp_DelayUs(WR_Delay_us);
				   Disp_W_WR(1);
							HAL_Disp_DelayUs(1);
			}
			Disp_W_CS(1);   
			HAL_Disp_DelayUs(1);
}

//参数可以是  W_CS 中一个
void HAL_Disp_Write_Pin(uint8_t cmd,uint8_t level)
{
	  switch(cmd)
			{
				case W_CS:{
					if(level)
					{
						CS_1;
					}
					else
					{
						CS_0;
					}
				}break;
				case W_WR:{
					if(level)
					{
						WR_1;
					}
					else
					{
						WR_0;
					}
				}break;
				case W_DATA:{
					if(level)
					{
						DATA_1;
					}
					else
					{
						DATA_0;
					}
				}break;				
			}
}
