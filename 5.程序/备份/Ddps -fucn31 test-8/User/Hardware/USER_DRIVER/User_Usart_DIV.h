#ifndef _USER_USART_DIV_H
#define _USER_USART_DIV_H

#include "superhead.h"

#define Serialbuffersize  256 //30字节

typedef struct {
	uint8_t uartbuffer[Serialbuffersize];
 volatile uint16_t head;//写入位置
	volatile uint16_t tail;//读取位置
	volatile uint8_t uartfinish;
}RingBuffer;

extern RingBuffer uartrxbuffer; // 接收缓冲


void DIV_Usart_Init(void);

void DIV_Usart_ISR(uint8_t dat);

// 从缓冲区读取一个字节
uint8_t DIV_Usart_ReadByte(uint8_t *data);

// 读取缓冲区中所有可用数据
uint16_t DIV_Uart_ReadAvailable(uint8_t *buffer, uint16_t max_len);

#endif

