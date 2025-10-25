#include "iot_hal_usart.h"

RingBuffer uartrxbuffer={0};

void DIV_Usart_Init(void)
{
		HAL_Usarf_Init();
}

//串口中断把字节存入缓冲区
void DIV_Usart_ISR(uint8_t dat)    
{
		//计算下一个写入位置
   uint16_t next_head = (uartrxbuffer.head+1) % Serialbuffersize;
		
	  //检查缓冲区是否满了
		if(next_head!=uartrxbuffer.tail)
		{
				uartrxbuffer.uartbuffer[uartrxbuffer.head]=dat;
			uartrxbuffer.head=next_head;
		}

}

// 从缓冲区读取一个字节
uint8_t DIV_Usart_ReadByte(uint8_t *data)
{
	if(uartrxbuffer.head==uartrxbuffer.tail)
	{
		return 0;
	}
	
	*data = uartrxbuffer.uartbuffer[uartrxbuffer.tail];
	uartrxbuffer.tail=(uartrxbuffer.tail+1)%Serialbuffersize;
	return 1;
}

// 读取缓冲区中所有可用数据
uint16_t DIV_Uart_ReadAvailable(uint8_t *buffer, uint16_t max_len) {
    uint16_t bytes_read = 0;
    uint8_t data;
  
    while (bytes_read < max_len && DIV_Usart_ReadByte(&data)) {
        buffer[bytes_read++] = data;
    }

				
    return bytes_read;
}
