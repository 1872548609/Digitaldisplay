#include "User_Tmr_Hal.h"


void HAL_TMR_Init(void)
{
    TMR_TimeBase_T timeBaseConfig;
	
				RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2);
				
				TMR_ConfigInternalClock(TMR2);
				
				timeBaseConfig.clockDivision = TMR_CKD_DIV1;
				timeBaseConfig.counterMode = TMR_COUNTER_MODE_UP;
				timeBaseConfig.period = 1000-1;  
				timeBaseConfig.div =  48-1;        /*  48/(47 + 1) = 1MHZ */
				timeBaseConfig.repetitionCounter = 0;
				TMR_ConfigTimeBase(TMR2,&timeBaseConfig);
	
				TMR_ClearStatusFlag(TMR2,TMR_INT_UPDATE);  //清除产生更新事件的标志位
			
    TMR_EnableInterrupt(TMR2, TMR_INT_UPDATE);
    NVIC_EnableIRQRequest(TMR2_IRQn, 0);      //设置中断优先级
	
	   TMR_Enable(TMR2);					//使能定时器外设
}

void TMR2_IRQHandler(void)
{
	   TMR2Isr();
}

