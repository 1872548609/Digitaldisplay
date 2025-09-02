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
 //一级菜单================
typedef enum{
			status_init,
			status_run,
			status_normset,
			status_proset,
}DeviceStatus;

DeviceStatus  manu1_devicestate = status_init;  

//二级菜单================
typedef enum{
	noset_status =0,
	compareset_status,
	noncset_status,
	reactiontimeset_status,
	maincolorset_status,
	unitconversion_status,
}NormsetStatus;

NormsetStatus manu2_normsetstatus = noset_status;    

typedef  enum{
	pnoset_status=0,
 secscreendisset_status,
	disspeedset_status,
	difleveset_status,
	coloractset_status,
	ecoset_status,
	factorydatereset_status
}PromodsetStatus;

PromodsetStatus manu2_promodesetstatus = pnoset_status;


//三级菜单==================
//norset
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

CompareStatus compsestatus =compare1_off;    

typedef enum{
   noncstatus_1o2o = 0,
			noncstatus_1o2c,
			noncstatus_1c2c,
			noncstatus_1c2o,

}NONCStatus;
	
NONCStatus noncstatus = noncstatus_1o2c;

typedef enum{
     reaction_2ms = 2,
					reaction_10ms = 10,
					reaction_50ms = 50,
					reaction_100ms = 100,
					reaction_250ms = 250,
					reaction_500ms = 500,
					reaction_2000ms = 2000,

}ReactionTime;

ReactionTime reactiontime = reaction_2ms;

typedef enum{
   R_ON = 0,
			G_ON,
			AL_RED,
			AL_GREEN,

}MainScreenColor;

MainScreenColor mainscreencolor = R_ON;

typedef enum{
 Unit_Mpr=0,
	Unit_Kpr,
	Unit_Kgf,
	Unit_Bar,
	Unit_Psi,
 Unit_Mmhg,
	Unit_Inhg,
}Unitconversion;

Unitconversion unitconversion = Unit_Mpr;	

//proet

typedef enum{
 secscreen_off=0,
	secscreen_disunit,
	secscreen_disoutchoice,
 secscreen_discustom,
}SecScreen;

SecScreen secscreen = secscreen_off;

typedef enum{
	dispeed_500ms=0,
	dispeed_1000ms,
}DisSpeed;

DisSpeed dispeed = dispeed_500ms;

typedef enum{
diflevel_level1=0,
diflevel_level2,
diflevel_level3,
}DifLevel;
	
DifLevel diflevel = diflevel_level1;

typedef enum{
 colorreaction_out1=0,
	colorreaction_out2,
}ColorReaction;

ColorReaction colorreaction = colorreaction_out1;

typedef enum{
 eco_std=0,
	eco_FULL,

}Ecomode;

Ecomode ecomode = eco_std;

typedef enum{
	facrecoff=0,
	facrecon,
	
}FactoryRecover;

FactoryRecover facrecover = facrecoff;
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
	switch(manu1_devicestate)
	{
		case status_init:{ //开机初始化
								//DIV_Disp_OneFrame(Run_Init);
							 DIV_Pwn_Set(&PWM_THREERED,10,threered_pwmval,PWM_ID_TR);
							 DIV_Pwn_Set(&PWM_ONERED,10,onered_pwmval,PWM_ID_OR);
							 DIV_Pwn_Set(&PWM_DOUBGED,10,doubleged_pwmval,PWM_ID_DG);
							 DIV_Pwn_Set(&PWM_ONEGED,10,oneged_pwmval,PWM_ID_OG);
         
			      DIV_Disp_ByString("AH01_NPN");
		     manu1_devicestate = status_run; 
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
				//短按事件==========================
	   if(	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV)
				{
				   	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV=0;
					
								if(key_management.WichKeyDown==KEY1)
								{		
											switch(manu1_devicestate){  //一级菜单 
												case status_normset:{
													switch(manu2_normsetstatus){ //二级菜单  
																case noset_status:{
																						DIV_Disp_ByString("EASYOut1");compsestatus=compare1_EASY;
																						manu2_normsetstatus = compareset_status;
																}break;
																case compareset_status:{	
																				switch(compsestatus)  //三级菜单		
																				{   					
																					case compare1_EASY:{DIV_Disp_ByString(" OFFOut2");compsestatus=compare2_off;}break;
																					case compare1_HSY:{DIV_Disp_ByString("1o2oNoNc");compsestatus=compare2_off;}break;														
																					case compare1_WCMP:{DIV_Disp_ByString("1o2oNoNc");compsestatus=compare2_off;}break;
																					//进入别的三级菜单
																					case compare2_off:{DIV_Disp_ByString("1o2oNoNc");compsestatus=compare1_off;manu2_normsetstatus=noncset_status;}break;
																					case compare2_EASY:{DIV_Disp_ByString("1o2oNoNc");compsestatus=compare1_off;manu2_normsetstatus=noncset_status;}break;
																					case compare2_HSY:{DIV_Disp_ByString("1o2oNoNc");compsestatus=compare1_off;manu2_normsetstatus=noncset_status;}break;														
																					case compare2_WCMP:{DIV_Disp_ByString("1o2oNoNc");compsestatus=compare1_off;manu2_normsetstatus=noncset_status;}break;
																					default:{}break;
																				}
																}break;
																case noncset_status:{
																					DIV_Disp_ByString("     RES");
																					DIV_Disp_floatNum(MainScreen,2.0);
																					reactiontime = reaction_2ms;
																     manu2_normsetstatus = reactiontimeset_status;
																}break;
																case reactiontimeset_status:{
				                DIV_Disp_ByString("        "); 
																	   DIV_Disp_ByString("R-oNCLoR");
																   manu2_normsetstatus =maincolorset_status;
																}break;
																case maincolorset_status:{
																    DIV_Disp_ByString("        "); 
																	   DIV_Disp_ByString(" MPRUnit");
																    manu2_normsetstatus = unitconversion_status;
																}break;
																case unitconversion_status:{
							
																}break;
                default:{}break;
													}
												}break;		
												case status_proset:{
												  switch(manu2_promodesetstatus)
														{
															case pnoset_status:{	
																DIV_Disp_ByString(" Std sub");
																manu2_promodesetstatus = secscreendisset_status;
															}break;
															case secscreendisset_status:{
																DIV_Disp_ByString(" 250 sub");
															 manu2_promodesetstatus = disspeedset_status;
															}break;
															case disspeedset_status:{
																DIV_Disp_ByString("  ||sub");
																manu2_promodesetstatus = difleveset_status;
															}break;
															case difleveset_status:{
																DIV_Disp_ByString("Out1 sub");
															 manu2_promodesetstatus = coloractset_status;
															}break;
															case coloractset_status:{
																DIV_Disp_ByString(" OFF sub");
															 manu2_promodesetstatus = ecoset_status;
															}break;
															case ecoset_status:{
																DIV_Disp_ByString(" OFF sub");
															 manu2_promodesetstatus = factorydatereset_status;
															}break;
															case factorydatereset_status:{
																DIV_Disp_ByString(" 250 sub");
															 manu2_promodesetstatus = secscreendisset_status;
															}break;																
														}
												}break;
												default:{}break;												
										}
							}											
								if(key_management.WichKeyDown==KEY2)
								{
											switch(manu1_devicestate){     //一级菜单
														case status_normset:{
															switch(manu2_normsetstatus){		//二级菜单														
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
																								DIV_Disp_ByString("     RES"); 				
																								switch(reactiontime)
																								{ 
																									case reaction_2ms:{DIV_Disp_Uint16Num(MainScreen,10);reactiontime=reaction_10ms;}break;
																									case reaction_10ms:{DIV_Disp_Uint16Num(MainScreen,50);reactiontime=reaction_50ms;}break;
																									case reaction_50ms:{DIV_Disp_Uint16Num(MainScreen,100);reactiontime=reaction_100ms;}break;
																									case reaction_100ms:{DIV_Disp_Uint16Num(MainScreen,250);reactiontime=reaction_250ms;}break;
																									case reaction_250ms:{DIV_Disp_Uint16Num(MainScreen,500);reactiontime=reaction_500ms;}break;
																									case reaction_500ms:{DIV_Disp_Uint16Num(MainScreen,2000);reactiontime=reaction_2000ms;}break;
																									case reaction_2000ms:{DIV_Disp_floatNum(MainScreen,2.0);reactiontime=reaction_2ms;}break;
																								}
																		}break;
																		case maincolorset_status:{
																			    switch(mainscreencolor)
																							{
																								case G_ON:{DIV_Disp_ByString(" REDCLoR");mainscreencolor=AL_RED;}break;
																								case R_ON:{DIV_Disp_ByString("G-oNCLoR");mainscreencolor=G_ON;}break;
																								case AL_GREEN:{DIV_Disp_ByString("R-oNCLoR");mainscreencolor=R_ON;}break;
																								case AL_RED:{DIV_Disp_ByString("GRENCLoR");mainscreencolor=AL_GREEN;}break;
																							}
																			}break;
																		case unitconversion_status:{
																		     switch(unitconversion)
																							{
																								case Unit_Bar:{DIV_Disp_ByString(" PSIUnit");unitconversion=Unit_Psi;}break;
																								case Unit_Kgf:{DIV_Disp_ByString(" bARUnit");unitconversion=Unit_Bar;}break;
																								case Unit_Kpr:{DIV_Disp_ByString(" KgFUnit");unitconversion=Unit_Kgf;}break;
																								case Unit_Mpr:{DIV_Disp_ByString(" KPRUnit");unitconversion=Unit_Kpr;}break;
																								case Unit_Psi:{DIV_Disp_ByString("MMHGUnit");unitconversion=Unit_Mmhg;}break;
																								case Unit_Mmhg:{DIV_Disp_ByString("inHGUnit");unitconversion=Unit_Inhg;}break;
																								case Unit_Inhg:{DIV_Disp_ByString(" MPRUnit");unitconversion=Unit_Mpr;}break;
																							}
																		}break;																			
																		default:{}break;
																}
														}break;
														case status_proset:{
																	switch(manu2_promodesetstatus)
																	{
																		case pnoset_status:{}break;
																		case secscreendisset_status:{
																			switch(secscreen)
																			{
																				case secscreen_off:{
																						DIV_Disp_ByString("Unit sub");
																						secscreen = secscreen_disunit;
																				}break;
																				case secscreen_disunit:{
																						DIV_Disp_ByString("Noxx sub");
																						DIV_Disp_Symbol(NIXIE2,Symbol_point);
																						secscreen = secscreen_disoutchoice;
																				}break;
																				case secscreen_disoutchoice:{
																						HAL_Disp_W_Data(dataid,0x15,0x00);
																						HAL_Disp_W_Data(dataid,0x16,0x00);
																						DIV_Disp_ByString("CuSt sub");
																						secscreen = secscreen_discustom;
																				}break;					
																				case secscreen_discustom:{
																						DIV_Disp_ByString(" OFF sub");
																						secscreen = secscreen_off;
																				}break;																	
																			}
																		}break;
																		case disspeedset_status:{
																							switch(dispeed)
																							{
																								case dispeed_500ms:{
																										DIV_Disp_ByString(" 500D-SP");
																										secscreen = secscreen_disunit;
																								}break;
																								case dispeed_1000ms:{
																										DIV_Disp_ByString("1000D-SP");
																										secscreen = secscreen_disunit;
																								}break;
																							}
																		}break;
																		case difleveset_status:{
																							switch(diflevel)
																							{
																								case diflevel_level1:{
																											DIV_Disp_ByString("  || HYS");
																											diflevel = diflevel_level2;
																								}break;
																								case diflevel_level2:{
																											DIV_Disp_ByString(" ||| HYS");
																											diflevel = diflevel_level3;
																								}break;
																								case diflevel_level3:{
																											DIV_Disp_ByString("   | HYS");
																											diflevel = diflevel_level1;
																								}break;
																							}
																		}break;
																		case coloractset_status:{
																					switch(colorreaction)
																					{
																						case colorreaction_out1:{
																										DIV_Disp_ByString("OUT2DiSP");
																										colorreaction = colorreaction_out2;
																						}break;
																						case colorreaction_out2:{
																										DIV_Disp_ByString("OUT1Disp");
																										colorreaction = colorreaction_out1;
																						}break;
																					}
																		}break;
																		case ecoset_status:{
																					switch(ecomode)
																					{
																						case eco_std:{
																										DIV_Disp_ByString("FULLDisp");
																										ecomode = eco_FULL;
																						}break;
																						case eco_FULL:{
																										DIV_Disp_ByString(" StdDisp");
																										ecomode = eco_std;
																						}break;
																					}
																		}break;
																		case factorydatereset_status:{
																					switch(facrecover)
																					{
																						case facrecoff:{
																									DIV_Disp_ByString(" OFFDisp");
																										facrecover = facrecon;
																						}break;
																						case facrecon:{
																									DIV_Disp_ByString(" OFFDisp");
																										facrecover = facrecoff;
																						}break;
																					}
																		}break;																
																	}
														}break;
														default:{}break;
											}
								}
								if(key_management.WichKeyDown==KEY3)
								{
										  switch(manu1_devicestate){   
														case status_normset:{
															  switch(manu2_normsetstatus){
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
																		     DIV_Disp_ByString("     RES"); 				
																								switch(reactiontime)
																								{ 
																									case reaction_2ms:{DIV_Disp_Uint16Num(MainScreen,2000);reactiontime=reaction_2000ms;}break;
																									case reaction_10ms:{DIV_Disp_floatNum(MainScreen,2.0);reactiontime=reaction_2ms;}break;
																									case reaction_50ms:{DIV_Disp_Uint16Num(MainScreen,10);reactiontime=reaction_10ms;}break;
																									case reaction_100ms:{DIV_Disp_Uint16Num(MainScreen,50);reactiontime=reaction_50ms;}break;
																									case reaction_250ms:{DIV_Disp_Uint16Num(MainScreen,100);reactiontime=reaction_100ms;}break;
																									case reaction_500ms:{DIV_Disp_Uint16Num(MainScreen,250);reactiontime=reaction_250ms;}break;
																									case reaction_2000ms:{DIV_Disp_Uint16Num(MainScreen,500);reactiontime=reaction_500ms;}break;
																								}
																		}break;
																		case maincolorset_status:{
																		      switch(mainscreencolor)
																							{                                
																								case G_ON:{DIV_Disp_ByString("R-oNCLoR");mainscreencolor=R_ON;}break;
																								case R_ON:{DIV_Disp_ByString("GRENCLoR");mainscreencolor=AL_GREEN;}break;
																								case AL_GREEN:{DIV_Disp_ByString(" REDCLoR");mainscreencolor=AL_RED;}break;
																								case AL_RED:{DIV_Disp_ByString("G-oNCLoR");mainscreencolor=G_ON;}break;
																							}
																		}break;																		
																		case unitconversion_status:{
																		    switch(unitconversion)
																							{
																								case Unit_Bar:{DIV_Disp_ByString(" KgFUnit");unitconversion=Unit_Kgf;}break;
																								case Unit_Kgf:{DIV_Disp_ByString(" KPRUnit");unitconversion=Unit_Kpr;}break;
																								case Unit_Kpr:{DIV_Disp_ByString(" MPRUnit");unitconversion=Unit_Mpr;}break;
																								case Unit_Mpr:{DIV_Disp_ByString("inHGUnit");unitconversion=Unit_Inhg;}break;
																								case Unit_Psi:{DIV_Disp_ByString(" BArUnit");unitconversion=Unit_Bar;}break;
																								case Unit_Mmhg:{DIV_Disp_ByString(" PSIUnit");unitconversion=Unit_Psi;}break;
																								case Unit_Inhg:{DIV_Disp_ByString("MMHGUnit");unitconversion=Unit_Mmhg;}break;
																							}
																		}break;                        
																		default:{}break;                
																	}				
														}break;
														case status_proset:{
																	switch(manu2_promodesetstatus)
																	{
																				case pnoset_status:{}break;             
																				case secscreendisset_status:{
																								switch(secscreen)               
																								{                               
																									case secscreen_off:{DIV_Disp_ByString("CuSt sub");secscreen = secscreen_discustom;}break;
																									case secscreen_disunit:{DIV_Disp_ByString(" OFF sub");secscreen = secscreen_off;}break;
																									case secscreen_disoutchoice:{HAL_Disp_W_Data(dataid,0x15,0x00);HAL_Disp_W_Data(dataid,0x16,0x00);DIV_Disp_ByString("Unit sub");secscreen = secscreen_disunit;}break;					      
																									case secscreen_discustom:{DIV_Disp_ByString("Noxx sub");DIV_Disp_Symbol(NIXIE2,Symbol_point);secscreen = secscreen_disoutchoice;}break;																	
																								}
																							}break;
																				case disspeedset_status:{
																									switch(dispeed)
																									{
																										case dispeed_500ms:{
																												DIV_Disp_ByString("1000D-SP");
																												secscreen = secscreen_disunit;
																										}break;
																										case dispeed_1000ms:{
																											 DIV_Disp_ByString(" 500D-SP");
																												secscreen = secscreen_disunit;
																										}break;
																									}
																				}break;
																				case difleveset_status:{
																									switch(diflevel)
																									{
																										case diflevel_level1:{
																													DIV_Disp_ByString(" ||| HYS");
																													diflevel = diflevel_level3;
																										}break;
																										case diflevel_level2:{
																													DIV_Disp_ByString("   | HYS");     
																													diflevel = diflevel_level1;
																										}break;
																										case diflevel_level3:{
																													DIV_Disp_ByString("  || HYS");
																													diflevel = diflevel_level2;
																										}break;
																									}
																				}break;
																				case coloractset_status:{
																							switch(colorreaction)
																							{
																								case colorreaction_out1:{
																												DIV_Disp_ByString("OUT1Disp");
																												colorreaction = colorreaction_out1;
																								}break;
																								case colorreaction_out2:{
																												 DIV_Disp_ByString("OUT2DiSP");
																												colorreaction = colorreaction_out2;
																								}break;
																							}
																				}break;
																				case ecoset_status:{
																							switch(ecomode)
																							{
																								case eco_std:{
																												DIV_Disp_ByString(" StdDisp");
																												ecomode = eco_std;
																								}break;
																								case eco_FULL:{
																												DIV_Disp_ByString("FULLDisp");
																												ecomode = eco_FULL;
																								}break;
																							}
																				}break;
																				case factorydatereset_status:{
																							switch(facrecover)
																							{
																								case facrecoff:{
																											DIV_Disp_ByString("  ONDisp");
																											facrecover = facrecon;
																								}break;
																								case facrecon:{
																											DIV_Disp_ByString(" OFFDisp");
																												facrecover = facrecoff;
																								}break;
																							}
																				}break;																
																	}
														}break;
												default:{}break;
												} 	
								}
								key_management.WichKeyDown=0x00;
							
				}
			
	  //长按事件=====================
			if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV)
			{
						key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=0;

							if(key_management.WichKeyDown==KEY1)
							{   
								
								
												//切换模式
														manu1_devicestate=status_normset; 
														
														//刷新显示
														DIV_Disp_OneFrame(NormSet_Frame);  
																				
							}
			}
			else if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV)
			{
					key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV=0;		
				
				
								//切换模式
							if(manu1_devicestate==status_run)
							{
								manu1_devicestate=status_proset;
								DIV_Disp_ByString(" PROMODE");
							}
							if(manu1_devicestate==status_normset)
							{
								manu1_devicestate=status_proset;
								DIV_Disp_ByString(" PROMODE");
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
