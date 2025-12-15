#include "hal_usart.h"

RingBuffer uartrxbuffer={0};

/* USART Write Data */
void USART_Write(USART_T* usart, uint8_t* dat);


void hal_usart_init(void)
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
					
    NVIC_EnableIRQRequest(USART1_IRQn, 0);

    /* Enable USART */
    USART_Enable(TRXUSART);
}

void  usart_receive_isr(void)       
{
    uint8_t dat;

    if (USART_ReadStatusFlag(TRXUSART, USART_FLAG_RXBNE) == SET)     
    {
        dat = (uint8_t)USART_RxData(TRXUSART); 
        
        uint16_t next_head = (uartrxbuffer.head+1) % Serialbuffersize;
		
		if(next_head!=uartrxbuffer.tail)
		{
            uartrxbuffer.uartbuffer[uartrxbuffer.head]=dat;
			uartrxbuffer.head=next_head;
		}
       
        //printf("%c", dat);     //回传测试
		
        USART_ClearStatusFlag(TRXUSART, USART_FLAG_RXBNE);  
    }
				
    if(USART_ReadStatusFlag(TRXUSART, USART_FLAG_IDLEF))    
    {
            
        uartrxbuffer.uartfinish=1;               
    
        USART_ClearStatusFlag(TRXUSART, USART_FLAG_IDLEF);   
    }
}

void USART1_IRQHandler(void)    
{   
	usart_receive_isr();       
}

uint8_t usart_readbyte(uint8_t *data)
{
	if(uartrxbuffer.head==uartrxbuffer.tail)
	{
		return 0;
	}
	
	*data = uartrxbuffer.uartbuffer[uartrxbuffer.tail];
	uartrxbuffer.tail=(uartrxbuffer.tail+1)%Serialbuffersize;
	return 1;
}

uint16_t usart_readavailable(uint8_t *buffer, uint16_t max_len) {
    uint16_t bytes_read = 0;
    uint8_t data;
  
    while (bytes_read < max_len && usart_readbyte(&data)) {
        buffer[bytes_read++] = data;
    }
	
    return bytes_read;
}


#pragma import(__use_no_semihosting)  
struct __FILE { int handle; };  
FILE __stdout;                
void _sys_exit(int x) { x = x; } 
int fputc(int ch, FILE* f)
{
    USART_TxData(DEBUG_USART, (uint8_t)ch);     
   
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);  
    
    return (ch);  
}






