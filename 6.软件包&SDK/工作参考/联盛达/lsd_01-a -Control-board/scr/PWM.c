#include "OB38R16T1.h"
#include "PWM.h"

#define PWM_VECTOR  8           //PWM Interrupt Vevtor
#define d_PWMCS     0x05        //PWMCS[2:0] (PWM clock select 0~7)
#define d_PWMXEN    0x04        //PWM Channel Enable (0~F)
#define d_PWM0PS    0x00        //PWM0 Channel polarity select
#define d_PWM1PS    0x00        //PWM1 Channel polarity select
#define d_PWM2PS    0x00        //PWM2 Channel polarity select
#define d_PWM3PS    0x00        //PWM3 Channel polarity select
#define d_PWMIE     0x00        //PWM Interrupt Enable bit
#define d_PWMOMS    0x00
#define d_DTP0      0x00
#define d_DT0       0x00
#define d_DTP1      0x00
#define d_DT1       0x00

#define PWMMD       0x03FF        //PWMMD[9:0]=Period (PWM Max Data Register 0~1023)
unsigned int g_PWMD2 = 0x01ff;        //PWMD2[9:0]=Duty (PWM Channel 2 Data Register 0~1023)
BuzCtrlType             buzctrl             ={0};

void PWM_initialize(void)  //Initialize PWM
{
    EA     = 0;             //Disable All Interrupt Function
    IEPWM  = (d_PWMIE);     //Enable PWM Interrupt Function
    PWMMDH = PWMMD >> 8;
    PWMMDL = PWMMD;
    PWMDT0 = (d_DTP0<<6) | d_DT0;
    PWMDT1 = (d_DTP1<<6) | d_DT1;
    PWMC2  = (d_PWM3PS<<3) | (d_PWM2PS<<2) | (d_PWM1PS<<1) | (d_PWM0PS) ;
    EA     = 1;             //Enable All Interrupt
}

void PWM_Output()
{
    // PWMD0H = (PWMD0>>8);
    // PWMD0L = (PWMD0);
    // PWMD1H = (PWMD1>>8);
    // PWMD1L = (PWMD1);
    PWMD2H = (g_PWMD2>>8);
    PWMD2L = (g_PWMD2);
    // PWMD3H = (PWMD3>>8);
    // PWMD3L = (PWMD3);
    PWMC   = PWMC|(d_PWMCS<<5)|(d_PWMOMS<<4)|(d_PWMXEN);
}

void PWM_Disable(void)
{
    IEPWM = 0;    //Disable PWM Interrupt
    PWMC  = 0;    //Disable PWM Function
}

void PWM_ISR(void) interrupt PWM_VECTOR //PWM Interrupt Subroutine
{
    PWMIF = 0;
}


//蜂鸣器控制
void BeepCtrl()
{
	static u16 BeepCount=0;
    static u16 buzTime  =0;
    switch (buzctrl.BeepState)
    {
    case BeepDown:
		BeepCount=0;
        PWM_Disable();
        buzctrl.BeepState =0;
        break;
    case BeepOnce:
		BeepCount++;
		if(BeepCount<=buzctrl.BeepDuration)
			PWM_Output();
		else 
		{
			BeepCount=0;
			PWM_Disable();
			buzctrl.BeepState =0;
		}
        break;
    case BeepLoop:
        buzTime++;
		BeepCount++;
        if(buzTime<buzctrl.BeeTime)
        {
		if(BeepCount<buzctrl.BeepDuration)
			PWM_Output();
		else if(BeepCount<(buzctrl.BeepDuration+buzctrl.interval))
			PWM_Disable();
		else 
			BeepCount=0;
        }
        else
        {
            buzTime=0;
			BeepCount=0;
			PWM_Disable();
			buzctrl.BeepState =0;
        }
        break;
    default:
		BeepCount=0;
		PWM_Disable();
		buzctrl.BeepState =0;
        break;
    }
}
