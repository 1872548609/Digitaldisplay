#ifndef UART_H_
#define UART_H_

#include "main.h"

#define  CRC16_ENABLE 1
#define  CMD_HEAD    0XEE
#define  CMD_TAIL    0XFFFCFFFF

//CMD
#define CMD_SteamPump   0x01 //蒸汽水泵
#define CMD_SteamHeat   0x02 //蒸汽加热
#define CMD_LampCtrl    0x08 //照明控制
#define CMD_DirOutput   0x04 //直接输出

#define CMD_ADSensor    0x10 //AD传感器
#define CMD_Flow        0x20 //流量计
#define CMD_FlowCtrl    0x40 //流量控制

#define TX_8(P1) SEND_DATA((P1)&0xFF)                    //
#define TX_8N(P,N) SendNU8((uint8 *)P,N)                 //
#define TX_16(P1) TX_8((P1)>>8);TX_8(P1)                 //
#define TX_16N(P,N) SendNU16((uint16 *)P,N)              //
#define TX_32(P1) TX_16((P1)>>16);TX_16((P1)&0xFFFF)     //

void SendChar(unsigned char n_temp0);
void Uart_Init(void);
bit ISEmpty(const LoopUartType *uart);
bit ISFull(const LoopUartType *uart);
uartStateType PushQueue(LoopUartType *uart,u8 Data);
uartStateType PopQueue(LoopUartType *uart,u8 *Data);
void QueueInit(LoopUartType *uart);
void DataFrameTransmission(const void *Data,u8 cmd,u8 len);
u16 QueueFinDataFrame(LoopUartType *uart,u8 *buffer);


#endif // !UART_H_
