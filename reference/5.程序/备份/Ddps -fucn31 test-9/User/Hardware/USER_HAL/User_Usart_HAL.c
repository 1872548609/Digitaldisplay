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
    USART_EnableInterrupt(TRXUSART, USART_INT_RXBNEIE);  //发送为空
   	USART_EnableInterrupt(TRXUSART, USART_INT_IDLEIE);   //空闲
					
    NVIC_EnableIRQRequest(USART1_IRQn, 0);

    /* Enable USART */
    USART_Enable(TRXUSART);
}


//void USART_Write(USART_T* usart, uint8_t * dat)  //发送
//{
//    /* Enable USART_INT_TXCIE */
//    USART_EnableInterrupt(TRXUSART, USART_INT_TXCIE);  //发送完成中断

//    while (*dat)     //数据存在
//    {
//					
//					
//        while (USART_ReadStatusFlag(usart, USART_FLAG_TXBE) == RESET);    //等待发送完成

//        USART_TxData(usart, *dat++);    //放入数据
//    }

//    USART_DisableInterrupt(TRXUSART, USART_INT_TXCIE);  //关闭中断
//}


void  USART_Receive_Isr(void)        //中断接收
{
    uint8_t dat;

    if (USART_ReadStatusFlag(TRXUSART, USART_FLAG_RXBNE) == SET)     //如果非空
    {
        dat = (uint8_t)USART_RxData(TRXUSART);  //获取数据
					
				    DIV_Usart_ISR(dat);     //把输入存入缓冲区

       // printf("%c", dat);     //回传测试
		
								USART_ClearStatusFlag(TRXUSART, USART_FLAG_RXBNE);  //清除中断
    }
				
				if(USART_ReadStatusFlag(TRXUSART, USART_FLAG_IDLEF))    //空闲中断
				{
						
					     uartrxbuffer.uartfinish=1;               //完成接收一帧数据
					
					  USART_ClearStatusFlag(TRXUSART, USART_FLAG_IDLEF);    //清除空闲
				}
}


void USART_Send_Isr(void)      //发送中断测试
{
    while (USART_ReadStatusFlag(TRXUSART, USART_FLAG_TXBE) == RESET);    //发送寄存器不为空

    if (USART_ReadStatusFlag(TRXUSART, USART_FLAG_TXC))   //如果传输完成
    {
        /* USART1 output this information */
       printf("Send Finish\r\n");                   //发送完成回传

        USART_ClearStatusFlag(TRXUSART, USART_FLAG_TXC);    //清除完成标志
    }
}


//打印重定向 使用标准c库的操作
#pragma import(__use_no_semihosting)  // 禁用半主机模式
struct __FILE { int handle; };  // 标准库需要的支持结构
FILE __stdout;                  // 定义标准输出流
void _sys_exit(int x) { x = x; } // 空实现，避免半主机模式调用
int fputc(int ch, FILE* f)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);     //放入数据

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);  //等待发送寄存器为空

    return (ch);  //发送成功返回
}



void USART1_IRQHandler(void)    //串口中断
{   
	USART_Receive_Isr();       //中断接收处理函数
}




