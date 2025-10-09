#include "User_Usart_HAL.h"


#define DEBUG_USART  USART1


/* USART Write Data */
void USART_Write(USART_T* usart, uint8_t* dat);


void HAL_Usarf_Init(void)
{
    GPIO_Config_T gpioConfig;
    USART_Config_T usartConfigStruct;

    /* Enable GPIO clock */
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_GPIOA);

    /* Enable  clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_USART1);

    /* Connect PXx to USARTx_Tx */
    GPIO_ConfigPinAF(TX_GPIO_PORT, TX_SOURCE, TX_AF);

    /* Connect PXx to USARRX_Rx */
    GPIO_ConfigPinAF(RX_GPIO_PORT, RX_SOURCE, RX_AF);

    /* Configure USART Tx as alternate function push-pull */
    gpioConfig.mode = GPIO_MODE_AF;
    gpioConfig.pin = TX_GPIO_PIN;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.outtype = GPIO_OUT_TYPE_PP;
    gpioConfig.pupd = GPIO_PUPD_PU;
    GPIO_Config(TX_GPIO_PORT, &gpioConfig);

    /* Configure USART Rx as input floating */
    gpioConfig.pin  = RX_GPIO_PIN;
    GPIO_Config(RX_GPIO_PORT, &gpioConfig);

    /* MINI_USARTs configured as follow: */
    /* BaudRate = 115200 baud */
    usartConfigStruct.baudRate = 115200;
    /* Receive and transmit enabled */
    usartConfigStruct.mode     = USART_MODE_TX_RX;
    /* Hardware flow control disabled (RTS and CTS signals) */
    usartConfigStruct.hardwareFlowCtrl = USART_FLOW_CTRL_NONE;
    /* No parity */
    usartConfigStruct.parity   = USART_PARITY_NONE;
    /* One Stop Bit */
    usartConfigStruct.stopBits =  USART_STOP_BIT_1;
    /* Word Length = 8 Bits */
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;
    /* USART_Config */
    USART_Config(TRXUSART, &usartConfigStruct);

    /* Enable USART_Interrupt_RXBNEIE */
    USART_EnableInterrupt(TRXUSART, USART_INT_RXBNEIE);
   	USART_EnableInterrupt(TRXUSART, USART_INT_IDLEIE);
					
    NVIC_EnableIRQRequest(USART1_IRQn, 3);

    /* Enable USART */
    USART_Enable(TRXUSART);
}

/*!
 * @brief        serial port tramsimt data
 *
 * @param        pointer to date that need to be sent
 *
 * @retval       None
 *
 * @note
 */
void USART_Write(USART_T* usart, uint8_t * dat)
{
    /* Enable USART_INT_TXCIE */
    USART_EnableInterrupt(TRXUSART, USART_INT_TXCIE);  //发送完成中断

    while (*dat)
    {
					
					
        while (USART_ReadStatusFlag(usart, USART_FLAG_TXBE) == RESET);

        USART_TxData(usart, *dat++);
    }

    USART_DisableInterrupt(TRXUSART, USART_INT_TXCIE);
}

/*!
 * @brief        This function handles USART1 RX interrupt Handler
 *
 * @param        None
 *
 * @retval       None
 *
 * @note        This function need to put into  void USART1_IRQHandler(void)
 */
void  USART_Receive_Isr(void)
{
    uint8_t dat;

    if (USART_ReadStatusFlag(TRXUSART, USART_FLAG_RXBNE) == SET)     //如果非空
    {
        dat = (uint8_t)USART_RxData(TRXUSART);
					
				    DIV_Usart_ISR(dat);

       // printf("%c", dat);
					
					   
					
								USART_ClearStatusFlag(TRXUSART, USART_FLAG_RXBNE);
    }
				
				if(USART_ReadStatusFlag(TRXUSART, USART_FLAG_IDLEF))
				{
						
					     uartrxbuffer.uartfinish=1;
					
					  USART_ClearStatusFlag(TRXUSART, USART_FLAG_IDLEF);
				}
}

/*!
 * @brief        This function handles USART1 TXCIE interrupt Handler
 *
 * @param        None
 *
 * @retval       None
 *
 * @note        This function need to put into  void USART1_IRQHandler(void)
 */
void USART_Send_Isr(void)
{
    while (USART_ReadStatusFlag(TRXUSART, USART_FLAG_TXBE) == RESET);    //发送寄存器不为空

    if (USART_ReadStatusFlag(TRXUSART, USART_FLAG_TXC))   //如果传输完成
    {
        /* USART1 output this information */
       printf("Send Finish\r\n");

        USART_ClearStatusFlag(TRXUSART, USART_FLAG_TXC);    //清除完成标志
    }
}

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       ch:  The characters that need to be send.
*
* @param       *f:  pointer to a FILE that can recording all information
*              needed to control a stream
*
* @retval      The characters that need to be send.
*
* @note
*/

#pragma import(__use_no_semihosting)  // 禁用半主机模式

struct __FILE { int handle; };  // 标准库需要的支持结构
FILE __stdout;                  // 定义标准输出流
void _sys_exit(int x) { x = x; } // 空实现，避免半主机模式调用

// 重定向 _ttywrch
void _ttywrch(int ch) {
     /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);  //发送寄存器为空就
}

int fputc(int ch, FILE* f)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);  //发送寄存器为空就

    return (ch);
}



void USART1_IRQHandler(void)
{   
	USART_Receive_Isr(); 
}




