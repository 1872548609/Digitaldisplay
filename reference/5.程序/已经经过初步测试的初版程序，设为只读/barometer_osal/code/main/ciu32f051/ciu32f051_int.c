#include "main.h"

extern uint32_t iot_systick;

//void NMI_Handler(void)
//{
//}

//void HardFault_Handler(void)
//{
//}

//void SVC_Handler(void)
//{
//}

//void PendSV_Handler(void)
//{
//}

void SysTick_Handler(void)
{
    iot_systick++;
}

//void TMR14_IRQHandler(void)
//{
//   
//}

//void ADC1_COMP_IRQHandler(void)
//{
//    
//}

