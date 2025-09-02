#include "Flow.h"

#define TIMER1_VECTOR   3       //TIMER1 Interrupt Vevtor
#define d_T0MOD         0x00    //TMOD[3:0] TIMER0 Mode setting
#define d_T1MOD         0x04    //TMOD[7:4] TIMER1 Mode setting
#define d_ET2           0x00    //TIMER2 overflow Interrupt Enable (Default 0 Disable)
#define d_ET1           0x01    //TIMER1 overflow Interrupt Enable (Default 0 Disable)
#define d_ET0           0x00    //TIMER0 overflow Interrupt Enable (Default 0 Disable)

#define d_MODEVALUE_T1      8192    //MODE0 13bit 8192
// #define d_RELOADVALUE_T1    2 //0.52ml/pulse

FlowCtrlTypedef FlowCtrl ={0};
u16 volatile CurFlow	=0;

void Flow_Init(void)
{
    IEN0 |= (d_ET2<<5)|(d_ET1<<3)|(d_ET0<<1); //IE=IEN0
    TMOD |= (d_T1MOD<<4)|(d_T0MOD);
    // TH1   = (d_MODEVALUE_T1-FlowCtrl.PulsePeriod)/32;
    // TL1   = (d_MODEVALUE_T1-FlowCtrl.PulsePeriod)%32;
    // TR1   = 1; //Timer1 Start bit     
}


void FLOWCOUNT_ISR(void) interrupt TIMER1_VECTOR   
{
    TH1   = (d_MODEVALUE_T1-FlowCtrl.PulsePeriod)/32;//定时总数减去脉冲周期写入定时器
    TL1   = (d_MODEVALUE_T1-FlowCtrl.PulsePeriod)%32;
    CurFlow++;//流控脉冲到达计数就加一
}

#define Flow_Scan   0x01
#define Flow_End    0x02

void Flow_Ctrl(void)
{
    static bit Flow_Flag = 0;
    if(FlowCtrl.FlowCtrlByte== Flow_Scan)//如果启动流控
    {
        if(Flow_Flag== 1)//标志位1
        {
            Flow_Flag =0;
            TH1   = (d_MODEVALUE_T1-FlowCtrl.PulsePeriod)/32;
            TL1   = (d_MODEVALUE_T1-FlowCtrl.PulsePeriod)%32;
            TR1 = 1;//启动计数
        }
    }
    else//关闭流控
	{
		TR1 = 0;//关闭计数
		CurFlow=0;
		Flow_Flag=1;
	}
    // {
	// 	if(Flow_Flag ==0)
	// 	{
	// 		TR1 = 0;
	// 		CurFlow= 0;
	// 		Flow_Flag =1;
	// 	}
    // }
}


