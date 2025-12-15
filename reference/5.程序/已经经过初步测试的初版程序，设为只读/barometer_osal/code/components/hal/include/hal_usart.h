#ifndef _HAL_USART_H
#define _HAL_USART_H

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include "hal_board.h"
/**************************************************************************************************
 * DEFINES
 **************************************************************************************************/

#define TX_GPIO_PORT    GPIOA
#define TX_SOURCE       GPIO_PIN_SOURCE_9  
#define TX_AF           GPIO_AF_PIN1

#define TX_GPIO_PIN     GPIO_PIN_9

#define RX_GPIO_PORT    GPIOA
#define RX_SOURCE       GPIO_PIN_SOURCE_10 
#define RX_AF           GPIO_AF_PIN1        
    
#define RX_GPIO_PIN     GPIO_PIN_10

#define TRXUSART        USART1

#define DEBUG_USART  USART1

#define Serialbuffersize  256 //30字节
/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/
typedef struct {
	uint8_t uartbuffer[Serialbuffersize];
    volatile uint16_t head;//写入位置
	volatile uint16_t tail;//读取位置
	volatile uint8_t uartfinish;
}RingBuffer;

extern RingBuffer uartrxbuffer; // 接收缓冲

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/
void hal_usart_init(void);
void usart_receive_isr(void);

uint16_t usart_readavailable(uint8_t *buffer, uint16_t max_len);
/**************************************************************************************************
**************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
