#ifndef __PWM_H__
#define __PWM_H__

// Example Code
/*
#include "PWM.h"
#include "OB38R08T1.h"

void main(void) //Main Function Start
{
    PWM_initialize();         //Call PWM Initial Subroutine
    PWM_Output(g_PWMD0, g_PWMD1, g_PWMD2, g_PWMD3);
    while(1)
    {
        ;
    }
    //PWM_Disable();
}
*/
#define BeepDown	0x01
#define BeepOnce    0x02
#define BeepLoop    0x04
#define Buz_500MS   50
#define Buz_3S      300
#define Buz_1S      100

#include "main.h"

extern BuzCtrlType buzctrl;

void PWM_initialize(void);
void PWM_Output(void);
void PWM_Disable(void);
void BeepCtrl(void);

#endif
