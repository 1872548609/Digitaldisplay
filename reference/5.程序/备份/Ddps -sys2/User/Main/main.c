 /* Includes */             
#include "superhead.h"



//flash=================================
#define TEXT_LENTH sizeof(g_text_buf)
#define SIZE  TEXT_LENTH/2 + ((TEXT_LENTH%2)? 1:0)   //半字对齐必须半字读写      
#define FLASH_SAVE_ADDR 0x0800f000



const uint8_t g_text_buf[] = {"1234"};
//pwm
uint8_t threered_pwmval = 100;
uint8_t onered_pwmval = 0;
uint8_t doubleged_pwmval = 100;
uint8_t oneged_pwmval = 100;



//状态枚举===================================================================
typedef enum{
			status_init,
			status_run,
			status_normset,
			status_proset,
}DeviceStatus;

DeviceStatus  devicestate = status_init;   //一级菜单

typedef enum{
	noset_status =0,
	compareset_status,
	noncset_status,
	reactiontimeset_status,
}NormsetStatus;

NormsetStatus normsetstatus = noset_status;    //二级菜单

typedef enum{ 
			compare1_off = 0,
			compare1_EASY = 1,
			compare1_WCMP = 2,
			compare1_HSY, 
			compare2_off ,
			compare2_EASY ,
			compare2_WCMP ,
			compare2_HSY 

}CompareStatus;

CompareStatus compsestatus =compare1_off;     //三级菜单

typedef enum{
   noncstatus_1o2o = 0,
			noncstatus_1o2c,
			noncstatus_1c2c,
			noncstatus_1c2o,

}NONCStatus;
	
NONCStatus noncstatus = noncstatus_1o2c;

float reactiontime = 25;

//menu=========================================================================
uint8_t DIV_10MS;

//主状态
void mainTask(void);

//使用内部时钟初始化为48mhz
void HAL_System_Init(void);

//主初始化
void APM_EVAL_Init(void);

//任务刷新
void t10ms_isr(void);

//按键工作
void Div_Key_Respon(void);


int main(void)
{
					APM_EVAL_Init();
	
//				 		uint16_t dataemp[TEXT_LENTH]={0};
//				stmflash_write(FLASH_SAVE_ADDR,(uint16_t*)g_text_buf,SIZE);
//	
//				stmflash_read(FLASH_SAVE_ADDR,dataemp,SIZE);
//    
																						
					while (1)             
		   {
						
							DIV_Pwn_Control(&PWM_THREERED);
							DIV_Pwn_Control(&PWM_ONERED);
							DIV_Pwn_Control(&PWM_DOUBGED);
							DIV_Pwn_Control(&PWM_ONEGED);
						
					  if(DIV_10MS)
							{
							   t10ms_isr();
										DIV_10MS=0;
							}
								
    }
}

//=====================================================================================
void t10ms_isr(void)
{
  Div_Key_Scan();
		Div_Key_Respon();
		mainTask();
}

void mainTask()
{	
	switch(devicestate)
	{
		case status_init:{ //开机初始化
								//DIV_Disp_OneFrame(Run_Init);
							 DIV_Pwn_Set(&PWM_THREERED,10,threered_pwmval,PWM_ID_TR);
							 DIV_Pwn_Set(&PWM_ONERED,10,onered_pwmval,PWM_ID_OR);
							 DIV_Pwn_Set(&PWM_DOUBGED,10,doubleged_pwmval,PWM_ID_DG);
							 DIV_Pwn_Set(&PWM_ONEGED,10,oneged_pwmval,PWM_ID_OG);
         
			      DIV_Disp_ByString("AH01_NPN");
		     devicestate = status_run; 
		}break;
		case status_run:{
	
				   		 

		}break;
		case status_normset:{
		   
		}break;
		case status_proset:{
		

			  
			
		}break;
	}
}

void Div_Key_Respon(void)
{
	   if(	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV)
				{
				   	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV=0;
					
								if(key_management.WichKeyDown==KEY1)
								{		
											switch(devicestate){  //一级菜单 
												case status_normset:{
													switch(normsetstatus){ //二级菜单  
																case noset_status:{
																						DIV_Disp_ByString("EASYOut1");compsestatus=compare1_EASY;
																						normsetstatus = compareset_status;
																}break;
																case compareset_status:{	
																				switch(compsestatus)  //三级菜单		
																				{   					
																					case compare1_EASY:{DIV_Disp_ByString(" OFFOut2");compsestatus=compare2_off;}break;
																					case compare1_HSY:{DIV_Disp_ByString("1o2oNoNc");compsestatus=compare2_off;}break;														
																					case compare1_WCMP:{DIV_Disp_ByString("1o2oNoNc");compsestatus=compare2_off;}break;
																					//进入别的三级菜单
																					case compare2_off:{DIV_Disp_ByString("1o2oNoNc");compsestatus=compare1_off;normsetstatus=noncset_status;}break;
																					case compare2_EASY:{DIV_Disp_ByString("1o2oNoNc");compsestatus=compare1_off;normsetstatus=noncset_status;}break;
																					case compare2_HSY:{DIV_Disp_ByString("1o2oNoNc");compsestatus=compare1_off;normsetstatus=noncset_status;}break;														
																					case compare2_WCMP:{DIV_Disp_ByString("1o2oNoNc");compsestatus=compare1_off;normsetstatus=noncset_status;}break;
																					default:{}break;
																				}
																}break;
																case noncset_status:{
																					DIV_Disp_ByString("     RES");
																     normsetstatus = reactiontimeset_status;
																
																}break;
																case reactiontimeset_status:{}break;
                default:{}break;
																
													}
												}break;		
												default:{}break;												
										}
							}											
								if(key_management.WichKeyDown==KEY2)
								{
											switch(devicestate){     //一级菜单
														case status_normset:{
															switch(normsetstatus){																
																		case compareset_status :{
																						switch(compsestatus){
																									case compare1_EASY:{DIV_Disp_ByString(" HSYOut1");compsestatus=compare1_HSY;}break;																case compare1_HSY:{DIV_Disp_ByString("WCMPOut1");compsestatus=compare1_WCMP;}break;
																									case compare1_WCMP:{DIV_Disp_ByString("EASYOut1");compsestatus=compare1_EASY;}break;															case compare2_off:{DIV_Disp_ByString("EASYOut2");compsestatus=compare2_EASY;}break;
																									case compare2_EASY:{DIV_Disp_ByString(" HSYOut2");compsestatus=compare2_HSY;}break;																case compare2_HSY:{DIV_Disp_ByString("WCMPOut2");compsestatus=compare2_WCMP;}break;
																									case compare2_WCMP:{DIV_Disp_ByString(" OFFOut2");compsestatus=compare2_off;}break;                default:{}break;
																						}
																			}break;
																		case noncset_status:{
																		     switch(noncstatus){
																								case noncstatus_1c2c:{DIV_Disp_ByString("1o2cNONC");noncstatus=noncstatus_1o2c;}break;
																								case noncstatus_1o2c:{DIV_Disp_ByString("1c2cNONC");noncstatus=noncstatus_1c2o;}break;
																								case noncstatus_1c2o:{DIV_Disp_ByString("1o2oNONC");noncstatus=noncstatus_1o2o;}break;
																								case noncstatus_1o2o:{DIV_Disp_ByString("1o2cNONC");noncstatus=noncstatus_1o2c;}break;
																								default:{}break;
																							}
																		}break;
																		case reactiontimeset_status:{
																		     if(reactiontime<2000){reactiontime++;}
																						
																							DIV_Disp_ByString("     RES");  
																							
																							DIV_Disp_floatNum(MainScreen,11.33);
																		}break;
																			default:{}break;
																}
														}break;
														default:{}break;
											}
								}
								if(key_management.WichKeyDown==KEY3)
								{
										  switch(devicestate){   
														case status_normset:{
															  switch(normsetstatus){
																		case compareset_status:{
																					switch(compsestatus){
																						case compare1_EASY:{DIV_Disp_ByString("WCMPOut1");compsestatus=compare1_WCMP;}break;																		case compare1_HSY:{DIV_Disp_ByString("EASYOut1");compsestatus=compare1_EASY;}break;
																						case compare1_WCMP:{DIV_Disp_ByString("_HSYOut1");compsestatus=compare1_HSY;}break;																			case compare2_off:{DIV_Disp_ByString("WCMPOut2");compsestatus=compare2_WCMP;}break;
																						case compare2_EASY:{DIV_Disp_ByString(" OFFOut2");compsestatus=compare2_off;}break;																			case compare2_HSY:{DIV_Disp_ByString("EASYOut2");compsestatus=compare2_EASY;}break;																						case compare2_WCMP:{DIV_Disp_ByString("_HSYOut2");compsestatus=compare2_HSY;}break;
																						default:{}break;
																					}	
																		}break;
																		case noncset_status:{
																		     switch(noncstatus){																								case noncstatus_1c2c:{DIV_Disp_ByString("1o2oNONC");noncstatus=noncstatus_1o2o;}break;
																								case noncstatus_1o2c:{DIV_Disp_ByString("1c2cNONC");noncstatus=noncstatus_1c2c;}break;
																								case noncstatus_1c2o:{DIV_Disp_ByString("1o2cNONC");noncstatus=noncstatus_1o2c;}break;
																								case noncstatus_1o2o:{DIV_Disp_ByString("1c2oNONC");noncstatus=noncstatus_1c2o;}break;
																								default:{}break;
																							}
																		}break;
																		case reactiontimeset_status:{
																		     if(reactiontime<2000){reactiontime--;}
																							DIV_Disp_ByString("     RES");
																		     DIV_Disp_Uint16Num(MainScreen,reactiontime);
																		}break;
																		default:{}break;
																	}				
														}break;
														default:{}break;
												} 	
								}
								key_management.WichKeyDown=0x00;
							
				}
			
	
			if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV)
			{
						key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=0;

							if(key_management.WichKeyDown==KEY1)
							{   
								
								
												//切换模式
														devicestate=status_normset; 
														
														//刷新显示
														DIV_Disp_OneFrame(NormSet_Frame);  
																				
							}
			}
			else if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV)
			{
					key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV=0;		
				
				
								//切换模式
							if(devicestate==status_normset){devicestate=status_run;
						 DIV_Disp_OneFrame(Run_Frame);  //刷新显示
								}
							
				
					
			}	
}


//主初始化
void APM_EVAL_Init(void)
{
    /* Init delay function */
				HAL_System_Init();  
				HAL_TMR_Init();
    APM_EVAL_DelayInit();
				DIV_Key_Init();
				DIV_Disp_Init();
	
				DIV_Pwm_Init();
}



//使用内部时钟初始化为48mhz==================================================================
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

								//pwm时间戳
								PWM_THREERED.Pwm_Tick++;
								PWM_ONERED.Pwm_Tick++;  
				    PWM_DOUBGED.Pwm_Tick++;				
				    PWM_ONEGED.Pwm_Tick++;  
				
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
