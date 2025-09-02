 /* Includes */             
#include "superhead.h"
#include "User_Disp_Font.h"




//flash=================================
#define TEXT_LENTH sizeof(g_text_buf)
#define SIZE  TEXT_LENTH/2 + ((TEXT_LENTH%2)? 1:0)   //半字对齐必须半字读写      
#define FLASH_SAVE_ADDR 0x0800f000

const uint8_t g_text_buf[] = {"1234"};

//状态枚举=================================
typedef enum{
			status_init,
			status_run,
			status_set,
			status_proset,
}DeviceStatusTypedef;

DeviceStatusTypedef  DeviceStatus;

uint8_t DIV_10MS;

//主状态
void mainTask(void);

//使用内部时钟初始化为48mhz
void HAL_System_Init(void);

//主初始化
void APM_EVAL_Init(void);

//任务刷新
void t10ms_isr(void);

//==============================================================
int main(void)
{
    APM_EVAL_Init();
	
				 		uint16_t dataemp[TEXT_LENTH]={0};
				stmflash_write(FLASH_SAVE_ADDR,(uint16_t*)g_text_buf,SIZE);
	
				stmflash_read(FLASH_SAVE_ADDR,dataemp,SIZE);
    
																						
					while (1)             
		   {
					  if(DIV_10MS)
							{
							   t10ms_isr();
										DIV_10MS=0;
							}
								
    }
}

//==============================================================
void t10ms_isr(void)
{
  Div_Key_Scan(); 
		//DIV_Disp_Control();
		mainTask();
}

void mainTask()
{
		static uint16_t mode = status_init;
	switch(mode)
	{
		case status_init:{ //开机初始化
     //  HAL_Disp_WrSeriesData(dataid,0x00,Disp_OneFrame,Disp_Max_Length);
			
			    APM_EVAL_DelayMs(1200);
		     mode = status_run; 
		}break;
		case status_run:{
			
						 uint8_t OneFrame[26] = {
								0xd,0x0,0xd			,   //7     fha      0x00~0x02
								0x0,0x0,0xc			,	  //6     fha      0x03~0x05
								0xa,0x6,0x5			,   //5     fha      0x06
								0xd,0x6,0x8			,   //4     ahf      0x09
								0xd,0x0,0xb			,   //3     ahf      0x12
								0xc,0x6,0x3			,   //2     ahf      0x15
								0xd,0x6,0x3			,			//1     ahf      0x18
								0x1,              //p1   	no							0x21
								0x1,										    //p2				no							0x22
								0x3,0x9,0xc   				//8     fha     
						};
					
				HAL_Disp_WrSeriesData(dataid,0x00,OneFrame,26);
					//	HAL_Disp_W_Data(dataid,0x00,0xd);
						
		}break;
	}
}


//主初始化
void APM_EVAL_Init(void)
{
    /* Init delay function */
				HAL_System_Init();
    APM_EVAL_DelayInit();
				//DIV_LED_Init();
				//DIV_Pwm_Init();
				HAL_TMR_Init();
				HAL_Disp_Init();
}
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

void TMR2Isr(void)
{  	
			if (TMR_ReadIntFlag(TMR2, TMR_INT_FLAG_UPDATE) == SET)
			{
        TMR_ClearIntFlag(TMR2, TMR_INT_FLAG_UPDATE);
				    
				
								//按键记录时间戳		
								key_management.Key_tick++;							
				
								//10ms刷新任务
        static uint32_t tick =0; 
								tick++;
								if(tick>=10)
								{
									tick=0;
									DIV_10MS=1;	
								}	
								
			}	
}
