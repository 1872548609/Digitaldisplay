#ifndef _USER_USART_HAL_H
#define _USER_USART_HAL_H

#include "superhead.h"




#define  TX_GPIO_PORT  GPIOA
#define 	TX_SOURCE   GPIO_PIN_SOURCE_9  
#define 	TX_AF       GPIO_AF_PIN1

#define TX_GPIO_PIN GPIO_PIN_9



#define 	RX_GPIO_PORT GPIOA
#define 	RX_SOURCE   GPIO_PIN_SOURCE_10 
#define 	RX_AF      GPIO_AF_PIN1         


#define RX_GPIO_PIN GPIO_PIN_10

#define TRXUSART  USART1

void HAL_Usarf_Init(void);
void USART_Write(USART_T* usart, uint8_t * dat);
void  USART_Receive_Isr(void);
void USART_Send_Isr(void);

#endif

