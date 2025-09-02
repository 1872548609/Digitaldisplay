#ifndef DATATYPE_H_
#define DATATYPE_H_

typedef unsigned char  u8;
typedef unsigned int  u16;
typedef unsigned long u32;

typedef unsigned char       uchar;
typedef unsigned char       uint8;
typedef unsigned short int  uint16;
typedef unsigned long       uint32;
typedef short int           int16;
typedef long                int32;

#define RX_BUFF_SIZE    128 


//占空输出
typedef struct 
{
    u16 Duty;
    u16 Period;
    u8  workFlag; 
}DutyCycleOutputType;

//照明控制
typedef struct 
{
    u8 CtrlByte;
    u8 DutyCount;
}LampCtrlTypedef;

//直接输出输出
typedef union 
{
    u8 OutputCtrlByte;
    struct 
    {
        u8 Valve_1:1;  //磨豆
        u8 Valve_2:1;  //加热
        u8 Valve_3:1;  //电磁阀1
        u8 Valve_4:1;  //电磁阀2
        u8 Valve_5:1;  //电磁阀3
        u8 Valve_6:1;  //等离子
        u8 Pump:1;     //咖啡泵
    }OutputCtrlBit;
}DirectOutputType;


//环形串口接收
typedef struct 
{
    u16 front;
    u16 rear;
    u8 rx_buf[RX_BUFF_SIZE];
}LoopUartType;


typedef struct 
{
    u8 cmd_head;
    u8 cmd_type;
    u8 cmd_len;
    u8 param[32];
    u8 cmd_tail[4];
}CTRL_MSG,*PCTRL_MSG;


typedef enum
{
    UartOk,
    UartFull,
    UartEmpty,
}uartStateType;


typedef struct
{
    u8  FlowCtrlByte; //控制
    u8  PulsePeriod;  //脉冲周期
}FlowCtrlTypedef;

typedef struct 
{
    u8  Temp1; //温度1
    u8  Temp2; //温度2
    u8  Press; //压力
    u8  AdMotor; //电机电流AD值
}AdParaTypedef;


#endif
