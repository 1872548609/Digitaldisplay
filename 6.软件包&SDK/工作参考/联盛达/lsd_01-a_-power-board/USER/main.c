#include "main.h"
 
DirectOutputType        DirectOutput       ={0};
volatile LoopUartType   LoopUart            ={0};
LampCtrlTypedef         LampCtrl            ={0};

u8 cmd_buffer[64]={0};
u16 volatile UartTimeOutCount =0; // 超时计数器
u32 ProcessCount =0;

//芯片初始化
void MCU_init(void)
{
    Gpio_init();
    Uart_Init();
    Flow_Init();
    ADC_Init();
    TIMER0_initialize();
    QueueInit(&LoopUart);
    EA  =1;
}

void OutPutCtrl(void)//输出控制
{
    static u8 step=0;
    switch (step)
    {
    case 0:
        (DirectOutput.OutputCtrlBit.Valve_1==High_ON)?(RL1=High_ON):(RL1=Low_OFF);//磨豆
        step++;
        break;
    case 1:
        (DirectOutput.OutputCtrlBit.Valve_2==High_ON)?(RL2=High_ON):(RL2=Low_OFF);//加热
        step++;
        break;
    case 2:
        (DirectOutput.OutputCtrlBit.Valve_3==High_ON)?(RL3=High_ON):(RL3=Low_OFF);//电磁阀1
        step++;
        break;
    case 3:
        (DirectOutput.OutputCtrlBit.Valve_4==High_ON)?(RL4=High_ON):(RL4=Low_OFF);//电磁阀2
        step++;
        break;
    case 4:
        (DirectOutput.OutputCtrlBit.Valve_5==High_ON)?(RL5=High_ON):(RL5=Low_OFF);//电磁阀3
        step++;
        break;
    case 5:
        (DirectOutput.OutputCtrlBit.Valve_6==High_ON)?(RL6=High_ON):(RL6=Low_OFF);//等离子
        step++;
        break;
    case 6:
        (DirectOutput.OutputCtrlBit.Pump==High_ON)?(SCR2=Low_ON):(SCR2=High_OFF);//咖啡泵
        step++;
        break;
    default:
        step =0;
        break;
    }
}

void ProcessMessage(PCTRL_MSG msg)//串口处理
{
    u8 cmdType = msg->cmd_type;
    u8 len     = msg->cmd_len;
    u8 Ack     = 0; 
    switch (cmdType)
    {
    case CMD_SteamHeat:
    memcpy(&SteamHeatCtrl,msg->param,sizeof(DutyCycleOutputType));//将蒸汽堡数据填入
    DataFrameTransmission(&Ack,CMD_SteamHeat,sizeof(Ack));//应答

        break;
    case CMD_SteamPump:
    memcpy(&SteamPumpCtrl,msg->param,sizeof(DutyCycleOutputType));//蒸汽泵的值填入
    DataFrameTransmission(&Ack,CMD_SteamPump,sizeof(Ack));//应答

        break;
    case CMD_DirOutput:
    memcpy(&DirectOutput,msg->param,sizeof(DirectOutputType));  //直接控制
    DataFrameTransmission(&Ack,CMD_DirOutput,sizeof(Ack));//应答
	
        break;
    case CMD_LampCtrl:
    memcpy(&LampCtrl,msg->param,sizeof(LampCtrlTypedef));//照明控制
    DataFrameTransmission(&Ack,CMD_LampCtrl,sizeof(Ack));//应答

        break;
    case CMD_ADSensor:
    DataFrameTransmission(&AdPara,CMD_ADSensor,sizeof(AdParaTypedef));//温度发送
    UartTimeOutCount =0;

        break;
    case CMD_Flow: 
    DataFrameTransmission(&CurFlow,CMD_Flow,sizeof(CurFlow));//返回流控计数值

        break;
    case CMD_FlowCtrl:
    memcpy(&FlowCtrl,msg->param,sizeof(FlowCtrlTypedef));//流控
    DataFrameTransmission(&Ack,CMD_FlowCtrl,sizeof(Ack));//应答
        break;
    default:
        break;
    }
}
void SerialPortDataRefresh()//串口数据刷新
{
    if(QueueFinDataFrame(&LoopUart,cmd_buffer)>0)
        ProcessMessage((PCTRL_MSG)cmd_buffer);
}



void Lamp_Ctrl()//照明控制
{
    static u8 PeriodCount =0;
    PeriodCount++;
    switch (LampCtrl.CtrlByte)
    {
    case FULL_POWER:
        LAMP =High_ON; 
        break;
    case All_Down:
        LAMP = Low_OFF; 
        break;
    case REDUCTION:
    if(PeriodCount<=LampCtrl.DutyCount)
        LAMP = High_ON;
    else if(PeriodCount<=PERIOD_20MS)
        LAMP = Low_OFF;
    else
        PeriodCount =0;
        break;
    default:
        LAMP = Low_OFF;
        break;
    }
}


void main(void)
{
    MCU_init();//初始化
    while (1)
    {
       SerialPortDataRefresh();//串口接收处理
       if(Time10MS>=DIV_10MS)//每10ms执行一次
       {
            Time10MS=0; 
            // 通讯超时关闭所有输出
            if(UartTimeOutCount>=UART_TIMEOUT_COUNT)//串口通讯超时
            {
                DirectOutput.OutputCtrlByte = 0; //清空输出
                SteamHeatCtrl.workFlag = 0;		//关闭蒸汽加热
                SteamPumpCtrl.workFlag = 0;		//关闭蒸汽水泵
                OutPutCtrl();//输出设置
            }
            else
            {
                UartTimeOutCount++;//每10ms自增一次
                Flow_Ctrl();//流控脉冲
                Lamp_Ctrl();//照明设置
                CheckSensor();//温度读取
                OutPutCtrl();//输出设置
            }
       }
    }
}

