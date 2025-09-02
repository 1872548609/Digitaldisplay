
#include "uart.h"

//16MHZ 娉㈢壒鐜�9600
#define d_S0RELH    0x03
#define d_S0RELL    0xCC    



#if(CRC16_ENABLE)

//#define TX_7(P1) SEND_DATA((P1)&0xFF)                    //锟斤拷锟酵碉拷锟斤拷锟街斤拷
//#define TX_7N(P,N) SendNU8((uint8 *)P,N)                 //锟斤拷锟斤拷N锟斤拷锟街斤拷
//#define TX_15(P1) TX_8((P1)>>8);TX_8(P1)                 //锟斤拷锟斤拷16位锟斤拷锟斤拷
//#define TX_15N(P,N) SendNU16((uint16 *)P,N)              //锟斤拷锟斤拷N锟斤拷16位锟斤拷锟斤拷
//#define TX_31(P1) TX_16((P1)>>16);TX_16((P1)&0xFFFF)     //锟斤拷锟斤拷32位锟斤拷锟斤拷


static uint16 _crc16 = 0xffff;
/*! 
*  \brief  检查数据是否符合CRC16校验
*  \param buffer 待校验的数据
*  \param n 数据长度，包含CRC16
*  \param pcrc 校验码
*/
static void AddCRC16(uint8 *buffer,uint16 n,uint16 *pcrc)
{
    uint16 i,j,carry_flag,a;

    for (i=0; i<n; i++)
    {
        *pcrc=*pcrc^buffer[i];
        for (j=0; j<8; j++)
        {
            a=*pcrc;
            carry_flag=a&0x0001;
            *pcrc=*pcrc>>1;
            if (carry_flag==1)
                *pcrc=*pcrc^0xa001;
        }
    }
}
/*! 
*  \brief  检查数据是否符合CRC16校验
*  \param buffer 待校验的数据，末尾存储CRC16
*  \param n 数据长度，包含CRC16
*  \return 校验通过返回1，否则返回0
*/
uint16 CheckCRC16(uint8 *buffer,uint16 n)
{
    uint16 crc0 = 0x0;
    uint16 crc1 = 0xffff;

    if(n>=2)
    {
        crc0 = ((buffer[n-2]<<8)|buffer[n-1]);
        AddCRC16(buffer,n-2,&crc1);
    }

    return (crc0==crc1);
}
/*! 
*  \brief  发送一个字节
*  \param  c 
*/
void SEND_DATA(uint8 c)
{
    AddCRC16(&c,1,&_crc16);
    SendChar(c);
}
/*! 
*  \brief  帧头
*/
void BEGIN_CMD()
{
    TX_8(0XEE);
    _crc16 = 0XFFFF;                      //开始计算CRC16
}
/*! 
*  \brief  帧尾
*/
void END_CMD()
{
    uint16 crc16 = _crc16;
    TX_16(crc16);                         //发送CRC16
    TX_32(0XFFFCFFFF);
}

#else

#define SEND_DATA(P) SendChar(P)          //锟斤拷锟斤拷一锟斤拷锟街斤拷
#define BEGIN_CMD() TX_8(0XEE)            //帧头
#define END_CMD() TX_32(0XFFFCFFFF)       //帧尾

#endif



bit bU0TX =0;
u16 cmd_pos =0;
u32 cmd_state=0;

void Uart_Init(void)
{
    S0RELH   = d_S0RELH;
    S0RELL   = d_S0RELL;
    AUX     |= 0x80;            // BRS = 1
    PCON    |= 0x80;            // SMOD =1
    IEN0    |= 0x90;            // EA=1, ES0=1, interrupt enable
    S0CON    = 0x50;            // mode 1, SM20=0, Revice enable
    TI       = 0;
}

void UART0_ISR(void) interrupt d_UART0_Vector
{
    unsigned char RXBUF0;
    if (RI)                     // INT_RX
    {
        RI = 0;                 // RI clear
        RXBUF0 = S0BUF;         // Read BUF, user code...
        PushQueue(&LoopUart,RXBUF0);
    }
    else                        // INT_TX
    {
        bU0TX  = 0;
        TI     = 0;             // TI clear
    }
}
void SendChar(unsigned char n_temp0)
{
    bU0TX = 1;
    S0BUF = n_temp0;
    while(bU0TX)
    {
        ;
    }
}


void QueueInit(LoopUartType *uart)
{
    u8 i;
    uart->rear = uart->front =0;
	cmd_pos=0;
	cmd_state=0;
    for(i=0;i<RX_BUFF_SIZE;i++)
        uart->rx_buf[i]=0;
	
}

// 鍒ょ┖
bit ISEmpty(const LoopUartType *uart)
{
    return uart->rear == uart->front;
}

//鍒ゆ弧
bit ISFull(const LoopUartType *uart)
{
    return (uart->rear + 1)%RX_BUFF_SIZE == uart->front;
}

//鍏ラ槦
uartStateType PushQueue(LoopUartType *uart,u8 Data)
{
    if(ISFull(uart)) 
        return UartFull;
    uart->rx_buf[uart->rear] = Data;
    uart->rear = (uart->rear+1)%RX_BUFF_SIZE;
    return UartOk;
}

//鍑洪槦
uartStateType PopQueue(LoopUartType *uart,u8 *Data)
{
    if(ISEmpty(uart))
        return UartEmpty;
    *Data= uart->rx_buf[uart->front];
    uart->front = (uart->front+1)%RX_BUFF_SIZE;
    return UartOk;
}

u8 SizeQueue(const LoopUartType *uart)
{
    return ((uart->rear+RX_BUFF_SIZE-uart->front)%RX_BUFF_SIZE);
}


u16 QueueFinDataFrame(LoopUartType *uart,u8 *buffer)
{
	u16 cmd_size =0;
    u8 _data =0;
    while(SizeQueue(uart)>0)
    {
        PopQueue(uart,&_data);
        if((cmd_pos==0)&&(_data != CMD_HEAD))
            continue;
        if(cmd_pos < RX_BUFF_SIZE)
            buffer[cmd_pos++] = _data;
        cmd_state= ((cmd_state<<8)|_data);
        if(cmd_state == CMD_TAIL)		
		{
			cmd_size=cmd_pos;
			cmd_state=0;
			cmd_pos=0;
#if(CRC16_ENABLE)
            //去掉指令头尾EE，尾FFFCFFFF共计5个字节，只计算数据部分CRC
            if(!CheckCRC16(buffer+1,cmd_size-5))                      //CRC校验
                return 0;

            cmd_size -= 2;                                            //去掉CRC16（2字节）
#endif
            return cmd_size;
		}
    }
    return 0;
}


//注意数据不能出现帧尾0xFFFCFFFF
void DataFrameTransmission(const void *Data,u8 cmd,u8 len)
{
    u8 i=0;
    BEGIN_CMD();
    SEND_DATA(cmd);
    SEND_DATA(len);
    for(i=0;i<len;i++)
        SEND_DATA(((u8*)Data)[i]);
    END_CMD();
}
