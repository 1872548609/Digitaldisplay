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
        u8 GrindBean:1;     //磨豆
        u8 Reserve_1:1;     //备用
        u8 CoffeValve:1;    //咖啡电磁阀
        u8 SteamValve:1;    //蒸汽电磁阀
        u8 HotWaterValve:1; //热水电磁阀
        u8 Plasma:1;        //等离子
        u8 Reserve_2:1;     //备用
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

//流量
typedef struct
{
    u8  FlowCtrlByte; //控制
    u8  PulsePeriod;  //脉冲周期
}FlowCtrlTypedef;

//温度传感器
typedef struct 
{
    u8  CurSteamTemp; //当前蒸汽温度
    u8  CurCoffeTemp; //当前咖啡温度
    u8  Press; //压力
    u8  AdMotor; //电机电流AD值
}AdParaTypedef;

typedef struct 
{
    u8 CurKey;
    u8 PreKey;
    u16 FilterCount;
}KeyParaTypedef;

typedef enum
{
    UartOk,
    UartFull,
    UartEmpty,
}uartStateType;

// extern uartStateType uartState;

typedef struct 
{
   u8 BeepState;
   u16 BeepDuration;//单次蜂鸣时间 单位10MS
   u16 interval;    //蜂鸣间隔 单位10MS
   u16 BeeTime;     //蜂鸣持续时间 单位10MS
}BuzCtrlType;

typedef enum
{
    status_Init,
    status_Down,
    status_Idle,
    status_Work,
    status_Steam,
    status_Clean,
    status_Set,
    status_Err,
}DeviceStatusTypedef;




#endif
