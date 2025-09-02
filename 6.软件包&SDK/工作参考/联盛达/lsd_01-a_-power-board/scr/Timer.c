#include "Timer.h"

#define TIMER0_VECTOR  1    //TIMER0 Interrupt Vevtor
#define d_T0MOD        0x01 //TMOD[3:0] TIMER0 Mode setting
#define d_T1MOD        0x00 //TMOD[7:4] TIMER1 Mode setting
#define d_ET2          0x00 //TIMER2 overflow Interrupt Enable (Default 0 Disable)
#define d_ET1          0x00 //TIMER1 overflow Interrupt Enable (Default 0 Disable)
#define d_ET0          0x01 //TIMER0 overflow Interrupt Enable (Default 0 Disable)

#define d_MODEVALUE_T0      65536     //MODE1 16bit 65536
#define d_RELOADVALUE_T0    3333//User Define Reload Value

volatile u32 TimeCount=0;
volatile u8  Time10MS =0; 

DutyCycleOutputType     SteamHeatCtrl       ={0}; //蒸汽加热控制
DutyCycleOutputType     SteamPumpCtrl       ={0}; //蒸汽水泵控制
volatile u16 SteamHeatCount =0;
volatile u16 SteamPumpCount =0;

void TIMER0_initialize(void)  //Initialize TIMER0
{
    IEN0 |= (d_ET2<<5)|(d_ET1<<3)|(d_ET0<<1); //IE=IEN0
    TMOD |= (d_T1MOD<<4)|(d_T0MOD);
    TH0   = (d_MODEVALUE_T0-d_RELOADVALUE_T0)/256;    //MODE1 16bit 65536 默认12分频 
    TL0   = (d_MODEVALUE_T0-d_RELOADVALUE_T0)%256;
    TR0   = 1;    //Timer0 Start bit
}

void TIMER0_ISR(void) interrupt TIMER0_VECTOR//2.5ms中断
{
    TH0 = (d_MODEVALUE_T0-d_RELOADVALUE_T0)/256;    //MODE1 16bit 65536
    TL0 = (d_MODEVALUE_T0-d_RELOADVALUE_T0)%256;
    TimeCount++;
    Time10MS++; 
    UartTimeOutCount++;
    Lamp_Ctrl();
    //蒸汽加热控制占空比
    if(SteamHeatCtrl.workFlag)
    {
        (SteamHeatCount >=SteamHeatCtrl.Period)?(SteamHeatCount=0):(SteamHeatCount++);
        if(SteamHeatCount<=SteamHeatCtrl.Duty)
        SCR1 =Low_ON;
        else
        SCR1 =High_OFF;
    }
    else
        SCR1 = High_OFF;
    //蒸汽水泵控制占空比
    if(SteamPumpCtrl.workFlag)
    {
        (SteamPumpCount>=SteamPumpCtrl.Period)?(SteamPumpCount=0):(SteamPumpCount++);
        if(SteamPumpCount<=SteamPumpCtrl.Duty)
        SCR3 =Low_ON;
        else
        SCR3 =High_OFF;
    }
    else
        SCR3 = High_OFF;   
}

void TimerRes(u32 *tim)
{
    TR0 =0;
    (*tim) = TimeCount;
    TR1 =0;
}

u32 GetTime(u32 tim,u16 DIV)
{
    TR0 = 0;
    if(TimeCount>=tim)
    {
        TR0 = 1;
        return (TimeCount-tim)/(DIV);
    }
    else
    {
        TR0 = 1;
        return (TimeCount+(~tim))/(DIV);
    }
}
