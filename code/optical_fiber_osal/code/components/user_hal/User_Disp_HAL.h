#ifndef __USER_DISP_HAL_H
#define __USER_DISP_HAL_H

#include "main.h"


//引脚定义
#define CS_PIN   	GPIO_PIN_7
#define CS_PORT  	GPIOB

#define WR_PIN  		GPIO_PIN_6
#define WR_PORT 		GPIOB

#define DATA_PIN 	GPIO_PIN_5
#define DATA_PORT GPIOB

#define ALL_PORT  GPIOB

#define DISP_ALL_PERIPH_PORT  RCM_AHB_PERIPH_GPIOB

//引脚操作
#define CS_0    GPIO_ClearBit(CS_PORT,CS_PIN)
#define CS_1    GPIO_SetBit(CS_PORT,CS_PIN)
#define WR_0    GPIO_ClearBit(WR_PORT,WR_PIN)
#define WR_1    GPIO_SetBit(WR_PORT,WR_PIN)
#define DATA_0  GPIO_ClearBit(DATA_PORT,DATA_PIN)
#define DATA_1  GPIO_SetBit(DATA_PORT,DATA_PIN)

//操作断码屏引脚
#define Disp_W_CS(x) 		HAL_Disp_Write_Pin(W_CS,x)
#define Disp_W_WR(x) 		HAL_Disp_Write_Pin(W_WR,x)
#define Disp_W_DATA(x) HAL_Disp_Write_Pin(W_DATA,x)

//cmd
#define W_CS   0
#define W_WR   1
#define W_DATA 2

//字段长
#define Id_length 3
#define Cmd_Length 9  

#define address_Length 6
#define Data_Length 4

//ID
#define cmdid 			0x4
#define dataid   0x5
		
//命令
#define SYS_DIS 0x00     //上电默认关闭
#define SYS_EN  0x01
#define LCD_OFF 0x02
#define LCD_ON 	0x03
#define TIMERS_DIS 0x04
#define TIMER_EN 0x06
#define RC_256K 	0x18   //上电默认内部RC

#define BIAS_1_2_2_COMS  0x20
#define BIAS_1_2_3_COMS  0x24
#define BIAS_1_2_4_COMS  0x28

#define BIAS_1_3_2_COMS		0x21
#define BIAS_1_3_3_COMS  0x25
#define BIAS_1_3_4_COMS  0x29

#define TEST 				0xE0
#define NORMAL 0xE3

//RAM
#define MAX_ADDR 0x31 //RAM最大地址

#define WR_Delay_us 5


void HAL_Disp_Write_Pin(uint8_t cmd,uint8_t level);

void HAL_Disp_Init(void);

//写命令  命令ID  八位命令
void HAL_Disp_W_Cmd(uint8_t id,uint16_t cmd);

//写某位RAM   数据ID   6位地址0x00~0x3f  4位数据0x00~0x0f
void HAL_Disp_W_Data(uint8_t id,uint16_t address,uint16_t data);

//连续写地址自增   数据ID  起始地址 数据数组 长度
void HAL_Disp_WrSeriesData(uint8_t id,uint16_t startaddress, uint8_t * data, uint16_t length);

//填满
void HAL_Disp_Fill(uint8_t data);

//清屏
void HAL_Disp_ClearALL(void);


#endif
