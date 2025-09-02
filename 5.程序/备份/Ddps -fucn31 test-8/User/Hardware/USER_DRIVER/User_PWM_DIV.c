#include "User_Pwm_DIV.h"

PWM_Management pwm0_management; PWM_Management pwm0_management = {0};

PWM_Management pwm1_management = {0};

PWM_Management pwm2_management = {0};
 
PWM_Management pwm3_management = {0};


void DIV_Pwm_Init(void)
{	
		  HAL_Pwm_Init();
}

__weak void DIV_periodvalid(void) {}

__weak void DIV_periodinvalid(void) {}

void DIV_Pwn_Set(PWM_Management* management,uint32_t period,float duty,uint8_t pwmid) 
{
		duty/=100;
	 management->compare=period*duty;
		management->period=period;
		management->Pwm_Tick=0;
		management->pwm_id = pwmid;
}

void DIV_Pwn_Control(PWM_Management *pwm_management)
{
		
			if(pwm_management->Pwm_Tick<pwm_management->compare)
			{
				
						//有效
						 if(pwm_management->pwm_id==PWM_ID_TR){HAL_PWM_SetOutPut(PWM_THREREDLED_PORT,PWM_THREREDLED_PIN,1);}
							if(pwm_management->pwm_id==PWM_ID_OR){HAL_PWM_SetOutPut(PWM_ONEREDLED_PORT,PWM_ONEREDLED_PIN,1);}
							if(pwm_management->pwm_id==PWM_ID_DG){HAL_PWM_SetOutPut(PWM_DOUBGEDLED_PORT,PWM_DOUBGEDLED_PIN,1);}
							if(pwm_management->pwm_id==PWM_ID_OG){HAL_PWM_SetOutPut(PWM_ONEGEDLED_PORT,PWM_ONEGEDLED_PIN,1);}
			}else{
				  //无效
						 if(pwm_management->pwm_id==PWM_ID_TR){HAL_PWM_SetOutPut(PWM_THREREDLED_PORT,PWM_THREREDLED_PIN,0);}
							if(pwm_management->pwm_id==PWM_ID_OR){HAL_PWM_SetOutPut(PWM_ONEREDLED_PORT,PWM_ONEREDLED_PIN,0);}
							if(pwm_management->pwm_id==PWM_ID_DG){HAL_PWM_SetOutPut(PWM_DOUBGEDLED_PORT,PWM_DOUBGEDLED_PIN,0);}
							if(pwm_management->pwm_id==PWM_ID_OG){HAL_PWM_SetOutPut(PWM_ONEGEDLED_PORT,PWM_ONEGEDLED_PIN,0);}
			}				
		if(pwm_management->Pwm_Tick>pwm_management->period)
		{
				pwm_management->Pwm_Tick=0;
		}	
}
