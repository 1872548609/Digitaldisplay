#include "User_Pwm_DIV.h"

PWM_Management pwn_management = {0};

void DIV_Pwm_Init(void)
{	
		  HAL_Pwm_Init();
}

__weak void DIV_periodvalid(void) {}

__weak void DIV_periodinvalid(void) {}

void DIV_Pwn_Set(uint32_t period,float duty)
{
		duty/=100;
	 pwn_management.compare=period*duty;
		pwn_management.period=period;
		pwn_management.Pwm_Tick=0;
}

void DIV_Pwn_Control(void)
{
		if(pwn_management.Pwm_Tick<pwn_management.period){
			if(pwn_management.Pwm_Tick<pwn_management.compare){
						 DIV_periodvalid();
			}else{
				   DIV_periodinvalid();
			}
		}else{
				pwn_management.Pwm_Tick=0;
		}
		
		
}
