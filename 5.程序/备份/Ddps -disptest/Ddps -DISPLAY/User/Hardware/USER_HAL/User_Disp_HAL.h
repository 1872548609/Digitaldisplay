#ifndef __USER_DISP_HAL_H
#define __USER_DISP_HAL_H

#include "superhead.h"

#define CS_PIN   	GPIO_PIN_7
#define CS_PORT  	GPIOB


#define WR_PIN  		GPIO_PIN_6
#define WR_PORT 		GPIOB

#define DATA_PIN 	GPIO_PIN_5
#define DATA_PORT GPIOB

#define ALL_PORT  GPIOB

//cmd
#define W_CS   0
#define W_WR   1
#define W_DATA 2

//²Ù×÷¶ÏÂëÆÁÒý½Å
#define Disp_W_CS(x) 		HAL_Disp_Write_Pin(W_CS,x)
#define Disp_W_WR(x) 		HAL_Disp_Write_Pin(W_WR,x)
#define Disp_W_DATA(x) HAL_Disp_Write_Pin(W_DATA,x)

//×Ö¶Î³¤
#define Id_length 3
#define Cmd_Length 9  

#define address_Length 6
#define Data_Length 4
  
#define cmdid 			0x4
#define dataid   0x5
		
#define TONE_ON 	0x09
#define TONE_OFF 0x08
#define TEST 				0xE3

void HAL_Disp_Write_Pin(uint8_t cmd,uint8_t level);

void HAL_Disp_Init(void);

void HAL_Disp_W_Cmd(uint8_t id,uint16_t cmd);

void HAL_Disp_W_Data(uint8_t id,uint16_t address,uint16_t data);


#endif
