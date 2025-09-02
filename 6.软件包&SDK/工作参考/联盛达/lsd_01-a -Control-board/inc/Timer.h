#ifndef __TIMER_0_H__
#define __TIMER_0_H__

// Example Code
/*
#include "Timer.h"
#include "OB38R16T1.h"

void main(void)
{ 
    TIMER0_initialize();
    TR0  = 1;
    TR1  = 1;
    EA   = 1;
    while(1);
}
*/
#include "main.h"
#define DIV_1S 400//1s
#define DIV_10MS 4//10ms

extern volatile u8 xdata  ProcessCount;


void TimerRes(u32 *tim);
void TIMER0_initialize(void);
u32 GetTime(u32 tim,u16 DIV);

 #endif

