 /* Includes */             
#include "superhead.h"
#include "User_Disp_Font.h"

//使用内部时钟初始化为48mhz
void HAL_System_Init(void)
{
				//默认初始化
		  /** Set HSIEN bit */   
    RCM->CTRL1_B.HSIEN = BIT_SET;
    /** Reset SCLKSEL, AHBPSC, APB1PSC, APB2PSC, ADCPSC and COC bits */
    RCM->CFG1 &= (uint32_t)0x08FFB80CU;
    /** Reset HSEEN, CSSEN and PLLEN bits */
    RCM->CTRL1 &= (uint32_t)0xFEF6FFFFU;
    /** Reset HSEBCFG bit */
    RCM->CTRL1_B.HSEBCFG = BIT_RESET;
    /** Reset PLLSRCSEL, PLLHSEPSC, PLLMULCFG bits */
    RCM->CFG1 &= (uint32_t)0xFFC0FFFFU;
    /** Reset PREDIV[3:0] bits */
    RCM->CFG1 &= (uint32_t)0xFFFFFFF0U;
    /** Reset USARTSW[1:0], I2CSW, CECSW and ADCSW bits */
    RCM->CFG3 &= (uint32_t)0xFFFFFEAC;
    /** Reset  HSI14 bit */
    RCM->CTRL2_B.HSI14EN = BIT_RESET;
    /** Disable all interrupts */
    RCM->INT = 0x00000000U;
	
				//设置时钟源为HSI内部RC输出48MHZ到HCLK
				uint32_t i;

    RCM->CTRL1_B.HSEEN= BIT_SET;

				//等待HSI时钟稳定
    for (i = 0; i < HSI_STARTUP_TIMEOUT; i++)
    {
        if (RCM->CTRL1_B.HSIRDYFLG)
        {
            break;
        }
    }
    //如果时钟稳定了才开始设置
    if (RCM->CTRL1_B.HSIRDYFLG)
    {
        /** Enable Prefetch Buffer */
        FMC->CTRL1_B.PBEN = BIT_SET;
        /** Flash 1 wait state */
        FMC->CTRL1_B.WS = 1;

        /** HCLK = SYSCLK */
        RCM->CFG1_B.AHBPSC= 0X00;

        /** PCLK = HCLK */
        RCM->CFG1_B.APB1PSC = 0X00;

        /** PLL: HSI /2 * 12 */
        RCM->CFG1_B.PLLSRCSEL = 0;
        RCM->CFG1_B.PLLMULCFG = 0xA;

        /** Enable PLL */
        RCM->CTRL1_B.PLLEN = 1;

        /** Wait PLL Ready */
        while (RCM->CTRL1_B.PLLRDYFLG == BIT_RESET);

        /** Select PLL as system clock source */
        RCM->CFG1_B.SCLKSEL = 2;

        /** Wait till PLL is used as system clock source */
        while (RCM->CFG1_B.SCLKSWSTS!= 0x02);
						}
}

//主初始化
void APM_EVAL_Init(void)
{
    /* Init delay function */
				HAL_System_Init();
    APM_EVAL_DelayInit();
				HAL_TMR_Init();
				HAL_Disp_Init();
	   HAL_Disp_WrSeriesData(dataid,0x00,Disp_OneFrame,Disp_Max_Length);
}

void t10ms_isr(void)
{

    

}


void DIV_periodvalid(void)
{
    HAL_PWM_SetOutPut(PWM_THREREDLED_PORT,PWM_THREREDLED_PIN,1);
}

void DIV_periodinvalid(void)
{
	   HAL_PWM_SetOutPut(PWM_THREREDLED_PORT,PWM_THREREDLED_PIN,0);
}

PWM_Management pwn_management1 = {0};

int main(void)
{
    APM_EVAL_Init();
	
	
	   	DIV_Pwm_Init();
				DIV_Pwn_Set(10,100);
	   pwn_management1.compare=10;
					pwn_management1.period=10;
					pwn_management1.Pwm_Tick=0;
	
	
    while (1)
    {						
					 DIV_Pwn_Control();

					if(pwn_management1.Pwm_Tick<pwn_management1.period){
								if(pwn_management1.Pwm_Tick<pwn_management1.compare){
												HAL_PWM_SetOutPut(PWM_DOUBGEDLED_PORT,PWM_DOUBGEDLED_PIN,1);
								}else{
											HAL_PWM_SetOutPut(PWM_DOUBGEDLED_PORT,PWM_DOUBGEDLED_PIN,0);
								}
							}else{
									pwn_management1.Pwm_Tick=0;
							}
					
    }
}

void TMR2Isr(void)
{  	
			if (TMR_ReadIntFlag(TMR2, TMR_INT_FLAG_UPDATE) == SET)
			{
        TMR_ClearIntFlag(TMR2, TMR_INT_FLAG_UPDATE);
				    
				
								//按键记录时间戳		
								key_management.Key_tick++;
								//PWM时间戳
								pwn_management.Pwm_Tick++;
								pwn_management1.Pwm_Tick++;
				
								//10ms刷新任务
        static uint32_t tick =0; 
								tick++;
								if(tick>=10)
								{
									tick=0;
									t10ms_isr();	
								}	
								
			}	
}
