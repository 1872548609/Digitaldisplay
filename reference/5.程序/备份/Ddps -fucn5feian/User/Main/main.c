 /* Includes */             
#include "superhead.h"

//枚举==================================================
//menu1 
DeviceStatus  manu1_devicestate = status_init;

//menu2
NormsetStatus manu2_normsetstatus = noset_status;
PromodsetStatus manu2_promodesetstatus = pnoset_status;

//rundisplay
RunState runstate = runstate_run;	

//norset 
CompareStatus1 compsestatus1 =compare1_EASY;  //比较输出1是什么状态
CompareStatus2 compsestatus2 =compare2_EASY;  //比较输出2是什么状态
NONCStatus noncstatus = noncstatus_1o2c;
ReactionTime reactiontime = reaction_2ms;
MainScreenColor mainscreencolor = R_ON;
Unitconversion unitconversion = Unit_Mpr;

//proset
SecScreen secscreen = secscreen_off;   		
DisSpeed dispeed = dispeed_500ms;
DifLevel diflevel = diflevel_level1;
ColorReaction colorreaction = colorreaction_out1;
Ecomode ecomode = eco_std;
FactoryRecover facrecover = facrecoff;

//flash================================================
#define TEXT_LENTH sizeof(g_text_buf)
#define SIZE  TEXT_LENTH/2 + ((TEXT_LENTH%2)? 1:0)   //半字对齐必须半字读写      
#define FLASH_SAVE_ADDR 0x0800f000
const uint8_t g_text_buf[] = {"1234"};
//pwm==================================================
uint8_t threered_pwmval = 100;
uint8_t onered_pwmval = 0;
uint8_t doubleged_pwmval = 100;
uint8_t oneged_pwmval = 100;
//可查询延时===========================================
volatile uint32_t Encheck_Delay_Tick;
volatile uint32_t Encheck_Delay_Starttime;
volatile	uint32_t Encheck_Delay_Nowtime;
volatile uint8_t Encheck_Delay_Start;

#define Delayret(); Encheck_Delay_Start=0;Encheck_Delay_Tick=0;Encheck_Delay_Start=1;

#define Delayst();  Encheck_Delay_Start=1; Encheck_Delay_Tick=0;    \
																				Encheck_Delay_Starttime=Encheck_Delay_Tick;

#define Delayck(x)  Encheck_Delay_Nowtime=Encheck_Delay_Tick;  \
																								if(Encheck_Delay_Nowtime-Encheck_Delay_Starttime>x)
                                                                                
																								

volatile uint8_t disp_delay_flag =0;
volatile uint16_t disp_delay_tick =0;
volatile uint8_t disp_delay_start=0;
volatile uint8_t finish_delay =0;
																								
//10ms执行标志=========================================
uint8_t DIV_10MS;

//设定值===============================================
uint16_t P1_Value = 0;
uint16_t P2_Value = 0;
uint16_t Hi1_Value = 0;
uint16_t Lo1_Value = 0;		
uint16_t Hi2_Value = 0;
uint16_t Lo2_Value = 0;
																						
float Currentairpressure	=0.2;     //当前气压值

																						
uint8_t Set_sign; //设定应差的正负显示 							

uint8_t Set_2sLongPressEv =0;		//2是key2 3是key3	哪个按键连按了加速	

//显示run设置标志	=====================================
#define dispset_P1 	0x01           //按下mode哪个闪烁模式一下
#define dispset_P2 	0x02
#define dispset_Hi1 0x04 
#define dispset_Lo1 0x08 
#define dispset_Hi2 0x10
#define dispset_Lo2 0x20

uint8_t runsetdisplay =0;
//run设置切换=============================================
#define set_P1 	0x01
#define set_P2 	0x02
#define set_Hi1 0x04
#define set_Lo1 0x08
#define set_Hi2 0x10
#define set_Lo2 0x20

uint8_t runsetwhich 	= set_P2;  //按键一设置siwtch
uint8_t runsetwhich1 = set_Lo1;
uint8_t runsetwhich2 = set_Hi1;
uint8_t runsetwhich3 = set_Lo1;
uint8_t runsetwhich4 = set_Hi1;
uint8_t runsetwhich5 = set_Hi1;
uint8_t runsetwhich6 = set_Lo1;
uint8_t runsetwhich7 = set_Hi1;
uint8_t runsetwhich8 = set_Lo2;
uint8_t runsetwhich9 = set_Lo2;
//单位转换=============================================
#define Kpa 0x01
#define Mpr 0x02
#define Kpr 0x04
#define Kgf 0x08
#define Bar 0x10
#define Psi 0x20
#define Mmhg 0x40
#define Inhg 0x80

uint8_t Current_Unit = Kpa;
//=====================================================



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

//长按设置
uint16_t Perform_numerical_Edition(uint16_t  value);

//显示输出预设
void Show_Presets (void);

//短按设置
uint16_t Perform_numerical_Edition_ShortPress(uint8_t setmode,uint16_t value);

//设置对应选择
void Set_corresponding_option(uint8_t addsub,uint8_t runsetswitch);

//清除显示当前设置run
void Disp_NowSet(void);

//主run长按设置
void MainRun_LongPressChoice(void);

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

//====================================================================================
//=                                                                             					=
//=                                                                             				 =
//=                                                                                  =
//====================================================================================
void t10ms_isr(void)
{
  Div_Key_Scan();
		Div_Key_Respon();
		mainTask();
}
//====================================================================================
//=                                                                             					=
//=                                                                             				 =
//=                                                                                  =
//====================================================================================
void mainTask()
{	
	switch(manu1_devicestate)
	{
		case status_init:{ //开机初始化
		
			
								DIV_Disp_Snprintf(MainScreen,"RL01");  //显示款型
								DIV_Disp_Snprintf(SecondScreen,"_NPN");//显示npn款    
								DIV_Pwn_Set(&PWM_THREERED,10,threered_pwmval,PWM_ID_TR);
							 DIV_Pwn_Set(&PWM_ONERED,10,onered_pwmval,PWM_ID_OR);
							 DIV_Pwn_Set(&PWM_DOUBGED,10,doubleged_pwmval,PWM_ID_DG);
							 DIV_Pwn_Set(&PWM_ONEGED,10,oneged_pwmval,PWM_ID_OG);
			
			     
								Delayck(1000)
								{
								  	manu1_devicestate = status_run; 
								}				
							
		}break;
		case status_run:{
								
			
			     
			    DIV_Disp_FloatByString(MainScreen,"000.0");
			
			     
							Disp_NowSet();
	
				   MainRun_LongPressChoice();
				
				
				 	 

  
			
							
		}break;
		case status_normset:{
		   
		}break;
		case status_proset:{
		

			  
			
		}break;
	}
}

//====================================================================================
//=                                                                             					=
//=   按键响应                                                                       =
//=                                                                                  =
//====================================================================================
void Div_Key_Respon(void)
{
				//短按事件==========================
	   if(	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV)
				{
				   	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV=0;
					
								if(key_management.WichKeyDown==KEY1)
								{		
											switch(manu1_devicestate){  //一级菜单 
												case status_run:{  
													
																	switch(compsestatus1)   //在run模式下按下mode显示不同的设置选项
																{
																			case compare1_EASY:{
																							switch(compsestatus2)
																							{
																								case compare2_EASY:{																												
																									   switch(runsetwhich)
																												{
																													case set_P1:{
																																						
																																				  runsetdisplay=dispset_P2;
																																						DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)2);
																																					 runsetwhich=set_P2; 
																													}break;
																													case set_P2:{
																				                  
																																				  runsetdisplay=dispset_P1;
																																						DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																																					 runsetwhich=set_P1; 
																													
																													}break;
																												}
																								}break;
																								case compare2_HSY:{
																								    switch(runsetwhich8)
																													{
																														case set_P1:{  

																																				  runsetdisplay=dispset_Hi2;
																																						DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																																					 runsetwhich8=set_Hi2;  
																														}break;
																														case set_Hi2:{
																															    
                                       runsetdisplay=dispset_Lo2;
																																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																																			    runsetwhich8=set_Lo2;
																																			
																														}break;
																														case set_Lo2:{
																															   
																										
																																				    runsetdisplay=dispset_P1;
																															    DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																																			    runsetwhich8=set_P1;
																																			
																														}break;
																													}
																								}break;
																								case compare2_off:{
																													  runsetdisplay=dispset_P1;
																															DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																															runsetwhich=set_P1;
																								}break;
																								case compare2_WCMP:{
																								    switch(runsetwhich9)
																													{
																														case set_P1:{  
																																	
																																				  runsetdisplay=dispset_Hi2;
																																						DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																																					 runsetwhich9=set_Hi2; 
																																			  
																														}break;
																														case set_Hi2:{
																															    
																															   
                                       runsetdisplay=dispset_Lo2;
																																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																																			    runsetwhich9=set_Lo2;
																																			
																														}break;
																														case set_Lo2:{
																															   
																															   
																																				    runsetdisplay=dispset_P1;
																																								DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																																								runsetwhich9=set_P1;
																																			
																														}break;
																													}
																								}break;
																							}
																								DIV_Disp_ClearPoint();
																								
																			}break;
																			case compare1_HSY:{
																							switch(compsestatus2)
																							{
																								case compare2_EASY:{
																									    switch(runsetwhich1)
																													{
																														case set_Lo1:{
																														   	
																																				  runsetdisplay=dispset_P2;
																																						DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)2);
																																					 runsetwhich1=set_P2; 
																																			
																														}break;
																														case set_P2:{
																														      
																																				  runsetdisplay=dispset_Hi1;
																																						DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																																					 runsetwhich1=set_Hi1; 
																																			 
																														}break;
																															case set_Hi1:{
																															      
																																				  runsetdisplay=set_Lo1;
																																						DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																																					 runsetwhich1=set_Lo1; 
																																	 
																															}break;
																													}																														
																								}break;
																								case compare2_HSY:{
																								  switch(runsetwhich2)
																										{
																											case set_Hi1:{
																											       
																																				  runsetdisplay=dispset_Hi2;
																																						DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																																					 runsetwhich2=set_Hi2; 
																																		 
																											}break;
																										 case set_Hi2:{
																											       
																																				  runsetdisplay=dispset_Lo1;
																																						DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																																					 runsetwhich2=set_Lo1; 
																																		 
																											}break;
																											case set_Lo1:{
																											      
																																				  runsetdisplay=dispset_Lo2;
																																						DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																																					 runsetwhich2=set_Lo2; 
																																			 
																											}break;
																											case set_Lo2:{
																											        
																																				  runsetdisplay=dispset_Hi1;
																																						DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																																					 runsetwhich2=set_Hi1; 
																																			 
																											}break;	
																										}																											
																								
																								}break;
																								case compare2_off:{
																								     switch(runsetwhich3)
																													{
																														case set_Hi1:{
																														     
																																				  runsetdisplay=dispset_Lo1;
																																						DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																																					 runsetwhich3=set_Lo1; 
																																			 
																														}break;
																														case set_Lo1:{
																														    
																																				  runsetdisplay=dispset_Hi1;
																																						DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																																					 runsetwhich3=set_Hi1; 
																																			 
																														}break;
																													}																														
																								
																								}break;
																								case compare2_WCMP:{
																												switch(runsetwhich4)
																												{
																													case set_Hi1:{
																													     
																																				   runsetdisplay=dispset_Lo1;
																																						DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																																					 runsetwhich4=set_Lo1;
																																			  
																													}break;
																													case set_Lo1:{
																													     
																																				  runsetdisplay=dispset_Hi2;
																																						DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																																					 runsetwhich4=set_Hi2; 
																																			  
																													}break;
																													
																													case set_Hi2:{
																													     
																																				  runsetdisplay=dispset_Lo2;
																																						DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																																					 runsetwhich4=set_Lo2; 
																																			  
																													}break;
																													case set_Lo2:{
																													    
																																				  runsetdisplay=dispset_Hi1;
																																						DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																																					 runsetwhich4=set_Hi1; 
																																			  
																													}break;
																												}
																								}break;
																							}
																			   	DIV_Disp_ClearPoint();
																					
																			}break;
																			case compare1_WCMP:{
																				    switch(compsestatus2)
																							{
																								case compare2_EASY:{
																								    switch(runsetwhich5)
																												{
																													case set_P2:{
																													      
																																				  runsetdisplay=dispset_Hi1;
																																						DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																																					 runsetwhich1=set_Hi1; 
																																			  
																													}break;
																													case set_Hi1:{
																													       
																																				  runsetdisplay=dispset_Lo1;
																																						DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																																					 runsetwhich1=set_Lo1; 
																																			 
																													
																													}break;
																													case set_Lo1:{
																													        
																																				  runsetdisplay=dispset_P2;
																																						DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)2);
																																					 runsetwhich1=set_P2; 
																																			 
																													}break;
																												}
																								}break;
																								case compare2_HSY:{
																								    switch(runsetwhich6)
																												{
																													case set_Hi1:{
																													        
																																				  runsetdisplay=dispset_Hi2;
																																						DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																																					 runsetwhich1=set_Hi2; 
																																			 
																													}break;
																													case set_Hi2:{
																													        
																																				  runsetdisplay=dispset_Lo1;
																																						DIV_Disp_Snprintf(SecondScreen,"Lo1-%d",(int8_t)1);
																																					 runsetwhich1=set_Lo1; 
																																			
																													}break;
																													case set_Lo1:{
																													      
																																				  runsetdisplay=dispset_Lo2;
																																						DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																																					 runsetwhich1=set_Lo2; 
																																			 
																													}break;
																													case set_Lo2:{
																													      
																																				  runsetdisplay=dispset_Hi1;
																																						DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																																					 runsetwhich1=set_Hi1; 
																																			 
																													}break;
																												}
																								}break;
																								case compare2_off:{
																								   switch(runsetwhich)
																											{
																												case set_Hi1:{
																												         
																																				  runsetdisplay=dispset_Lo1;
																																						DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																																					 runsetwhich1=set_Lo1; 
																																			 
																												}break;
																												case set_Lo1:{
																												        
																																				  runsetdisplay=dispset_Hi1;
																																						DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																																					 runsetwhich1=set_Hi1; 
																																			 
																												}break;
																											}
																								}break;
																								case compare2_WCMP:{
																								    switch(runsetwhich7)
																												{
																													case set_Hi1:{
																													        
																																				  runsetdisplay=dispset_Hi2;
																																						DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																																					 runsetwhich1=set_Hi2; 
																																			 
																													}break;
																													case set_Hi2:{
																													       
																																				  runsetdisplay=dispset_Lo1;
																																						DIV_Disp_Snprintf(SecondScreen,"Lo1-%d",(int8_t)2);
																																					 runsetwhich1=set_Lo1; 
																																			 
																													}break;
																													case set_Lo1:{
																													       
																																				  runsetdisplay=dispset_Lo2;
																																						DIV_Disp_Snprintf(SecondScreen,"Lo2-%d",(int8_t)2);
																																					 runsetwhich1=set_Lo2; 
																																			 
																													}break;
																													case set_Lo2:{
																													     
																																				  runsetdisplay=dispset_Hi1;
																																						DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																																					 runsetwhich1=set_Hi1; 
																																			 
																													}break;
																												}
																								
																								}break;
																							}
																		  DIV_Disp_ClearPoint();
																					
																			}break;
																}	
																	disp_delay_tick=0;				
												}break;
												case status_normset:{   						//进入norset模式
													switch(manu2_normsetstatus){ //处于刚入norset模式  
																case noset_status:{       //刚进入显示OUT1的比较输出设置		 
																			manu2_normsetstatus=compareset1_status; //进入输出1比较输出模式设置    
																}break;
																case compareset1_status:{	  //处于比较输出1设置模式下
																			manu2_normsetstatus=compareset2_status;  //进入输出2比较输出模式设置
																}break;
																case compareset2_status:{    //处于比较输出2设置模式下																  
																				manu2_normsetstatus=noncset_status;  //进入常开常闭
																}break;
																case noncset_status:{    //处于常开常闭设置
																     manu2_normsetstatus = reactiontimeset_status;//进入
																}break;
																case reactiontimeset_status:{  //处于反应时间设置
																   manu2_normsetstatus =maincolorset_status; //进入
																}break;
																case maincolorset_status:{     //处于主颜色设置     
																    manu2_normsetstatus = unitconversion_status;  //进入
																}break;
																case unitconversion_status:{  //处于单位设置      
																				manu2_normsetstatus = noset_status;  //进入
																    manu1_devicestate = status_run;  //进入
																}break;
                default:{}break;
													}
													 Show_Presets();
												}break;		
												case status_proset:{
												  switch(manu2_promodesetstatus)
														{
															case pnoset_status:{	
																DIV_Disp_ByString(MainScreen," Std");
																DIV_Disp_ByString(SecondScreen," sub");
																manu2_promodesetstatus = secscreendisset_status;
															}break;
															case secscreendisset_status:{
																DIV_Disp_ByString(MainScreen," 250");
																DIV_Disp_ByString(SecondScreen," sub");
															 manu2_promodesetstatus = disspeedset_status;
															}break;
															case disspeedset_status:{
																DIV_Disp_ByString(MainScreen,"  ||");
																DIV_Disp_ByString(SecondScreen," sub");
																manu2_promodesetstatus = difleveset_status;
															}break;
															case difleveset_status:{
																DIV_Disp_ByString(MainScreen,"Out1");
																DIV_Disp_ByString(SecondScreen," sub");
															 manu2_promodesetstatus = coloractset_status;
															}break;
															case coloractset_status:{
																DIV_Disp_ByString(MainScreen," OFF");
																DIV_Disp_ByString(SecondScreen," sub");
															 manu2_promodesetstatus = ecoset_status;
															}break;
															case ecoset_status:{
																DIV_Disp_ByString(MainScreen," OFF sub");
																DIV_Disp_ByString(SecondScreen," OFF sub");
															 manu2_promodesetstatus = factorydatereset_status;
															}break;
															case factorydatereset_status:{
																DIV_Disp_ByString(MainScreen," 250");
																DIV_Disp_ByString(SecondScreen," sub");
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
														case status_run:{
															 	switch(compsestatus1)
																{
																			case compare1_EASY:{
																							switch(compsestatus2)
																						{
																						  case compare2_EASY:{Set_corresponding_option(1,runsetwhich);}break;
																								case compare2_HSY:{Set_corresponding_option(1,runsetwhich);}break;
																								case compare2_off:{Set_corresponding_option(1,runsetwhich);}break;
																								case compare2_WCMP:{Set_corresponding_option(1,runsetwhich);}break;
																						}
																			
																			}break;
																			case compare1_HSY:{
																							switch(compsestatus2){
																								case compare2_EASY:{Set_corresponding_option(1,runsetwhich1);}break;
																								case compare2_HSY:{Set_corresponding_option(1,runsetwhich2);}break;
																								case compare2_off:{Set_corresponding_option(1,runsetwhich3);}break;
																								case compare2_WCMP:{Set_corresponding_option(1,runsetwhich4);}break;
																							}
																			}break;
																			case compare1_WCMP:{
																					switch(compsestatus2){
																					case compare2_EASY:{Set_corresponding_option(1,runsetwhich5);}break;
																					case compare2_HSY:{Set_corresponding_option(1,runsetwhich6);}break;
																					case compare2_off:{Set_corresponding_option(1,runsetwhich);}break;
																					case compare2_WCMP:{Set_corresponding_option(1,runsetwhich7);}break;
																				}
																			}break;
																	}		 
														}break;
														case status_normset:{       //按键2normset模式功能
															switch(manu2_normsetstatus){				//选择设置哪个设置页										
																		case compareset1_status :{    //比较输出1的设置
																						switch(compsestatus1){    //设置比较输出1为哪个模式
																							case compare1_EASY:{compsestatus1=compare1_HSY;}break;																
																							case compare1_HSY:{compsestatus1=compare1_WCMP;}break;
																							case compare1_WCMP:{compsestatus1=compare1_EASY;}break;		
																							default:{}break;
																						}
																						Show_Presets();
																		}break;
																		case compareset2_status:{
																		    switch(compsestatus2){
																									case compare2_EASY:{compsestatus2=compare2_HSY;}break;																
																									case compare2_HSY:{compsestatus2=compare2_WCMP;}break;
																									case compare2_WCMP:{compsestatus2=compare2_off;}break;
																									case compare2_off:{compsestatus2=compare2_EASY;}break;																									
																						   default:{}break;
																						}
																		    Show_Presets();
																		}break;
																		case noncset_status:{
																		     switch(noncstatus){
																								case noncstatus_1c2c:{noncstatus=noncstatus_1o2c;}break;
																								case noncstatus_1o2c:{noncstatus=noncstatus_1c2o;}break;
																								case noncstatus_1c2o:{noncstatus=noncstatus_1o2o;}break;
																								case noncstatus_1o2o:{noncstatus=noncstatus_1o2c;}break;
																								default:{}break;
																							}
																							Show_Presets();
																		}break;
																		case reactiontimeset_status:{		
																								DIV_Disp_ClearPoint(); 				
																								switch(reactiontime)
																								{ 
																									case reaction_2ms:{reactiontime=reaction_10ms;}break;
																									case reaction_10ms:{reactiontime=reaction_50ms;}break;
																									case reaction_50ms:{reactiontime=reaction_100ms;}break;
																									case reaction_100ms:{reactiontime=reaction_250ms;}break;
																									case reaction_250ms:{reactiontime=reaction_500ms;}break;
																									case reaction_500ms:{reactiontime=reaction_2000ms;}break;
																									case reaction_2000ms:{reactiontime=reaction_2ms;}break;
																								}
																								Show_Presets();
																		}break;
																		case maincolorset_status:{
																			    switch(mainscreencolor)
																							{
																								case G_ON:{mainscreencolor=AL_RED;}break;
																								case R_ON:{mainscreencolor=G_ON;}break;
																								case AL_GREEN:{mainscreencolor=R_ON;}break;
																								case AL_RED:{mainscreencolor=AL_GREEN;}break;
																							}
																							Show_Presets();
																			}break;
																		case unitconversion_status:{
																		     switch(unitconversion)
																							{
																								case Unit_Bar:{unitconversion=Unit_Psi;}break;
																								case Unit_Kgf:{unitconversion=Unit_Bar;}break;
																								case Unit_Kpr:{unitconversion=Unit_Kgf;}break;
																								case Unit_Mpr:{unitconversion=Unit_Kpr;}break;
																								case Unit_Psi:{unitconversion=Unit_Mmhg;}break;
																								case Unit_Mmhg:{unitconversion=Unit_Inhg;}break;
																								case Unit_Inhg:{unitconversion=Unit_Mpr;}break;
																							}
																							Show_Presets();
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
																						DIV_Disp_ByString(MainScreen,"Unit");
																					 DIV_Disp_ByString(SecondScreen," sub");
																						secscreen = secscreen_disunit;
																				}break;
																				case secscreen_disunit:{
																						DIV_Disp_ByString(MainScreen,"Noxx");
																					 DIV_Disp_ByString(SecondScreen," sub");
																						DIV_Disp_Symbol(NIXIE2,Symbol_point);
																						secscreen = secscreen_disoutchoice;
																				}break;
																				case secscreen_disoutchoice:{
																						HAL_Disp_W_Data(dataid,0x15,0x00);
																						HAL_Disp_W_Data(dataid,0x16,0x00);
																						DIV_Disp_ByString(MainScreen,"CuSt");
																					 DIV_Disp_ByString(SecondScreen," sub");
																						secscreen = secscreen_discustom;
																				}break;					
																				case secscreen_discustom:{
																						DIV_Disp_ByString(MainScreen," OFF");
																						DIV_Disp_ByString(SecondScreen," sub");
																						secscreen = secscreen_off;
																				}break;																	
																			}
																		}break;
																		case disspeedset_status:{
																							switch(dispeed)
																							{
																								case dispeed_500ms:{
																										DIV_Disp_ByString(MainScreen," 500");
																										DIV_Disp_ByString(SecondScreen,"D-SP");
																										secscreen = secscreen_disunit;
																								}break;
																								case dispeed_1000ms:{
																										DIV_Disp_ByString(MainScreen,"1000");
																									 DIV_Disp_ByString(SecondScreen,"D-SP");
																										secscreen = secscreen_disunit;
																								}break;
																							}
																		}break;
																		case difleveset_status:{
																							switch(diflevel)
																							{
																								case diflevel_level1:{
																											DIV_Disp_ByString(MainScreen,"  ||");
																									  DIV_Disp_ByString(SecondScreen," HYS");
																											diflevel = diflevel_level2;
																								}break;
																								case diflevel_level2:{
																											DIV_Disp_ByString(MainScreen," |||");
																									  DIV_Disp_ByString(SecondScreen," HYS");
																											diflevel = diflevel_level3;
																								}break;
																								case diflevel_level3:{
																											DIV_Disp_ByString(MainScreen,"   |");
																									  DIV_Disp_ByString(SecondScreen," HYS");
																											diflevel = diflevel_level1;
																								}break;
																							}
																		}break;
																		case coloractset_status:{
																					switch(colorreaction)
																					{
																						case colorreaction_out1:{
																										DIV_Disp_ByString(MainScreen,"OUT2");
																							   DIV_Disp_ByString(SecondScreen,"DiSP");
																										colorreaction = colorreaction_out2;
																						}break;
																						case colorreaction_out2:{
																										DIV_Disp_ByString(MainScreen,"OUT1");
																							   DIV_Disp_ByString(SecondScreen,"DiSP");
																										colorreaction = colorreaction_out1;
																						}break;
																					}
																		}break;
																		case ecoset_status:{
																					switch(ecomode)
																					{
																						case eco_std:{
																										DIV_Disp_ByString(MainScreen,"FULL");
																							   DIV_Disp_ByString(SecondScreen,"Disp");
																										ecomode = eco_FULL;
																						}break;
																						case eco_FULL:{
																										DIV_Disp_ByString(MainScreen," Std");
																							   DIV_Disp_ByString(SecondScreen,"Disp");
																										ecomode = eco_std;
																						}break;
																					}
																		}break;
																		case factorydatereset_status:{
																					switch(facrecover)
																					{
																						case facrecoff:{
																									DIV_Disp_ByString(MainScreen," OFF");
																							   DIV_Disp_ByString(SecondScreen,"Disp");
																										facrecover = facrecon;
																						}break;
																						case facrecon:{
																									DIV_Disp_ByString(MainScreen," OFF");
																							   DIV_Disp_ByString(SecondScreen,"Disp");
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
														case status_run:{        
															   switch(compsestatus1)
																{
																			case compare1_EASY:{
																			   switch(compsestatus2)
																						{
																						  case compare2_EASY:{Set_corresponding_option(0,runsetwhich);}break;
																								case compare2_HSY:{Set_corresponding_option(0,runsetwhich);}break;
																								case compare2_off:{Set_corresponding_option(0,runsetwhich);}break;
																								case compare2_WCMP:{Set_corresponding_option(0,runsetwhich);}break;
																						}
																			}break;
																			case compare1_HSY:{
																							switch(compsestatus2){
																								case compare2_EASY:{Set_corresponding_option(0,runsetwhich1);}break;
																								case compare2_HSY:{Set_corresponding_option(0,runsetwhich2);}break;
																								case compare2_off:{Set_corresponding_option(0,runsetwhich3);}break;
																								case compare2_WCMP:{Set_corresponding_option(0,runsetwhich4);}break;
																							}
																			}break;
																			case compare1_WCMP:{
																					switch(compsestatus2){
																					case compare2_EASY:{Set_corresponding_option(0,runsetwhich5);}break;
																					case compare2_HSY:{Set_corresponding_option(0,runsetwhich6);}break;
																					case compare2_off:{Set_corresponding_option(0,runsetwhich);}break;
																					case compare2_WCMP:{Set_corresponding_option(0,runsetwhich7);}break;
																				}
																			}break;
																	}		 
														}break;
														case status_normset:{
															  switch(manu2_normsetstatus){
																		case compareset1_status:{
																					switch(compsestatus1){
																								case compare1_EASY:{compsestatus1=compare1_WCMP;}break;																		
																								case compare1_HSY:{compsestatus1=compare1_EASY;}break;
																								case compare1_WCMP:{compsestatus1=compare1_HSY;}break;
																					default:{}break;
																					}	
																		}break;
																		case compareset2_status:{
																				switch(compsestatus2)
																				{
																						case compare2_EASY:{compsestatus2=compare2_off;}break;
																						case compare2_HSY:{compsestatus2=compare2_EASY;}break;
																						case compare2_off:{compsestatus2=compare2_WCMP;}break;
																						case compare2_WCMP:{compsestatus2=compare2_HSY;}break;
																			}
																		}break;
																		case noncset_status:{
																		     switch(noncstatus){
																								case noncstatus_1c2c:{noncstatus=noncstatus_1o2o;}break;
																								case noncstatus_1o2c:{noncstatus=noncstatus_1c2c;}break;
																								case noncstatus_1c2o:{noncstatus=noncstatus_1o2c;}break;
																								case noncstatus_1o2o:{noncstatus=noncstatus_1c2o;}break;
																								default:{}break;
																							}
																		}break;
																		case reactiontimeset_status:{
																								DIV_Disp_ClearPoint();
																								switch(reactiontime)
																								{ 
																									case reaction_2ms:{reactiontime=reaction_2000ms;}break;
																									case reaction_10ms:{reactiontime=reaction_2ms;}break;
																									case reaction_50ms:{reactiontime=reaction_10ms;}break;
																									case reaction_100ms:{reactiontime=reaction_50ms;}break;
																									case reaction_250ms:{reactiontime=reaction_100ms;}break;
																									case reaction_500ms:{reactiontime=reaction_250ms;}break;
																									case reaction_2000ms:{reactiontime=reaction_500ms;}break;
																								}
																		}break;
																		case maincolorset_status:{
																		      switch(mainscreencolor)
																							{                                
																								case G_ON:{mainscreencolor=R_ON;}break;
																								case R_ON:{mainscreencolor=AL_GREEN;}break;
																								case AL_GREEN:{mainscreencolor=AL_RED;}break;
																								case AL_RED:{mainscreencolor=G_ON;}break;
																							}
																		}break;																		
																		case unitconversion_status:{
																		    switch(unitconversion)
																							{
																								case Unit_Bar:{unitconversion=Unit_Kgf;}break;
																								case Unit_Kgf:{unitconversion=Unit_Kpr;}break;
																								case Unit_Kpr:{unitconversion=Unit_Mpr;}break;
																								case Unit_Mpr:{unitconversion=Unit_Inhg;}break;
																								case Unit_Psi:{unitconversion=Unit_Bar;}break;
																								case Unit_Mmhg:{unitconversion=Unit_Psi;}break;
																								case Unit_Inhg:{unitconversion=Unit_Mmhg;}break;
																							}
																		}break;                        
																		default:{}break;                
																	}	
																	Show_Presets();
														}break;
														case status_proset:{
																	switch(manu2_promodesetstatus)
																	{
																				case pnoset_status:{}break;             
																				case secscreendisset_status:{
																								switch(secscreen)               
																								{                               
																									case secscreen_off:{DIV_Disp_ByString(MainScreen,"CuSt");DIV_Disp_ByString(SecondScreen," sub");secscreen = secscreen_discustom;}break;
																									case secscreen_disunit:{DIV_Disp_ByString(MainScreen," OFF");DIV_Disp_ByString(SecondScreen," sub");secscreen = secscreen_off;}break;
																									case secscreen_disoutchoice:{HAL_Disp_W_Data(dataid,0x15,0x00);HAL_Disp_W_Data(dataid,0x16,0x00);DIV_Disp_ByString(MainScreen,"Unit");DIV_Disp_ByString(SecondScreen," sub");secscreen = secscreen_disunit;}break;					      
																									case secscreen_discustom:{DIV_Disp_ByString(MainScreen,"Noxx");DIV_Disp_ByString(SecondScreen," Sub");DIV_Disp_Symbol(NIXIE2,Symbol_point);secscreen = secscreen_disoutchoice;}break;																	
																								}
																							}break;
																				case disspeedset_status:{
																									switch(dispeed)
																									{
																										case dispeed_500ms:{
																												DIV_Disp_ByString(MainScreen,"1000");
																											 DIV_Disp_ByString(SecondScreen,"D-SP");
																												secscreen = secscreen_disunit;
																										}break;
																										case dispeed_1000ms:{
																											 DIV_Disp_ByString(MainScreen," 500");
																												DIV_Disp_ByString(SecondScreen,"D-SP");
																												secscreen = secscreen_disunit;
																										}break;
																									}
																				}break;
																				case difleveset_status:{
																									switch(diflevel)
																									{
																										case diflevel_level1:{
																													DIV_Disp_ByString(MainScreen," |||");
																											  DIV_Disp_ByString(SecondScreen," HYS");
																													diflevel = diflevel_level3;
																										}break;
																										case diflevel_level2:{
																														DIV_Disp_ByString(MainScreen,"   |");
																											  DIV_Disp_ByString(SecondScreen," HYS");
																											  diflevel = diflevel_level1;
																										}break;
																										case diflevel_level3:{
																													DIV_Disp_ByString(MainScreen,"   ||");
																											  DIV_Disp_ByString(SecondScreen," HYS");
																													diflevel = diflevel_level2;
																										}break;
																									}
																				}break;
																				case coloractset_status:{
																							switch(colorreaction)
																							{
																								case colorreaction_out1:{
																												DIV_Disp_ByString(MainScreen,"OUT1");
																									   DIV_Disp_ByString(SecondScreen,"Disp");
																												colorreaction = colorreaction_out1;
																								}break;
																								case colorreaction_out2:{
																												 	DIV_Disp_ByString(MainScreen,"OUT2");
																									   DIV_Disp_ByString(SecondScreen,"Disp");
																												colorreaction = colorreaction_out2;
																								}break;
																							}
																				}break;
																				case ecoset_status:{
																							switch(ecomode)
																							{
																								case eco_std:{
																												DIV_Disp_ByString(MainScreen," Std");
																									   DIV_Disp_ByString(SecondScreen,"Disp");
																												ecomode = eco_std;
																								}break;
																								case eco_FULL:{
																												DIV_Disp_ByString(MainScreen,"FULL");
																									   DIV_Disp_ByString(SecondScreen,"Disp");
																												ecomode = eco_FULL;
																								}break;
																							}
																				}break;
																				case factorydatereset_status:{
																							switch(facrecover)
																							{
																								case facrecoff:{
																											DIV_Disp_ByString(MainScreen,"  ON");
																									  DIV_Disp_ByString(SecondScreen,"Disp");
																											facrecover = facrecon;
																								}break;
																								case facrecon:{
																											DIV_Disp_ByString(MainScreen," OFF");
																									  DIV_Disp_ByString(SecondScreen,"Disp");
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
												if(manu1_devicestate==status_run)
												{
																manu1_devicestate=status_normset;  
													//刷新显示
														DIV_Disp_OneFrame(NormSet_Frame);  
												}
												else if(manu1_devicestate==status_normset)
												{
													manu1_devicestate=status_run;
													manu2_normsetstatus=noset_status;
													Show_Presets();
												}
												else if(manu1_devicestate==status_proset)
												{
													manu1_devicestate=status_run;
													Show_Presets();
												}
														
														
																				
							}
							if(key_management.WichKeyDown==KEY2){   Set_2sLongPressEv=2;}
							if(key_management.WichKeyDown==KEY3){   Set_2sLongPressEv=3;}
			}
			else if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV)
			{
					key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV=0;		
				
				    if(key_management.WichKeyDown==KEY1)
								{
								   		//切换模式
											if(manu1_devicestate==status_run)
											{
												manu1_devicestate=status_proset;
												DIV_Disp_ByString(MainScreen," PRO");
												DIV_Disp_ByString(SecondScreen,"MODE");
											}
											else if(manu1_devicestate==status_normset)
											{
												manu1_devicestate=status_proset;
												DIV_Disp_ByString(MainScreen," PRO");
												DIV_Disp_ByString(SecondScreen,"MODE");
											}
								}
						
				
					
			}
			else if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressKEY12EV)
			{
					  key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressKEY12EV=0;
				
				     
			}
}


//====================================================================================
//=                                                                             					=
//=                                                                             				 =
//=                                                                                  =
//====================================================================================
void APM_EVAL_Init(void)
{
    /* Init delay function */
				HAL_System_Init();  
				HAL_TMR_Init();
    APM_EVAL_DelayInit();
				DIV_Key_Init();    
				DIV_Pwm_Init();
				DIV_Disp_Init();
	   Delayst();
				
}

//====================================================================================
//=                                                                             					=
//=                           使用内部时钟初始化为48mhz                             	 =
//=                                                                                  =
//====================================================================================
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
//====================================================================================
//=                                                                             					=
//=                 选择设置                                                   							=
//=                                                                                  =
//====================================================================================
void Set_corresponding_option(uint8_t addsub,uint8_t runsetswitch)
{
			int i=0;
			if(addsub){i=1;}
   switch(runsetswitch)
			{
				case set_P1:{
									P1_Value=Perform_numerical_Edition_ShortPress(i,P1_Value);
				}break;
				case set_P2:{
									P2_Value=Perform_numerical_Edition_ShortPress(i,P2_Value);
				}break;
				case set_Hi2:{
									Hi2_Value=Perform_numerical_Edition_ShortPress(i,Hi2_Value);
				}break;
				case set_Lo2:{
									Lo2_Value=Perform_numerical_Edition_ShortPress(i,Lo2_Value);
				}break;
				case set_Hi1:{
										Hi1_Value=Perform_numerical_Edition_ShortPress(i,Hi1_Value);
				}break;
				case set_Lo1:{
								Lo1_Value=	Perform_numerical_Edition_ShortPress(i,Lo1_Value);
				}break;
			}
}

//====================================================================================
//=                                                                             					=
//=                 短按按键设置                                                   		=
//=                                                                                  =
//====================================================================================
uint16_t Perform_numerical_Edition_ShortPress(uint8_t setmode,uint16_t value)
{
	if(setmode)  //加
	{
	 if(Set_sign){
			if(value<1050){(value)++;}
		}else{
			if(value>0){(value)--;}else{Set_sign=1;value=1;}
		} 
		DIV_Disp_SecondDispSetwithHunit(Set_sign,value,Kpa);
	}
	else //减法
	{
		 if(Set_sign){
						if(value>0){(value)--;}else{Set_sign=0;value=1;}
				}else{
						if(value<1050){(value)++;}
				}
				DIV_Disp_SecondDispSetwithHunit(Set_sign,value,Kpa);
	}		
	return value;
}
//====================================================================================
//=                                                                             					=
//=                 显示输出预设                                                    		=
//=                                                                                  =
//====================================================================================
void Show_Presets (void)
{
	  if(reactiontime!=reaction_2ms||status_normset!=reactiontimeset_status){DIV_Disp_ClearPoint();}
	
			switch(manu1_devicestate){     //一级菜单
				case status_run:{
					    DIV_Pwn_Set(&PWM_THREERED,10,100,PWM_ID_TR);
									DIV_Pwn_Set(&PWM_DOUBGED,10,100,PWM_ID_DG);
					    DIV_Disp_ByString(MainScreen,"    ");         //显示输出1的预设
									DIV_Disp_ByString(SecondScreen,"    ");	
				}break;
				case status_normset:{       
					switch(manu2_normsetstatus){													
								case compareset1_status :{  
									   DIV_Pwn_Set(&PWM_THREERED,10,100,PWM_ID_TR);//红色提示输出1
												DIV_Pwn_Set(&PWM_DOUBGED,10,0,PWM_ID_DG);
												switch(compsestatus1){    
															case compare1_EASY:{  
																   	DIV_Disp_ByString(MainScreen,"EASY");         //显示输出1的预设
																	   DIV_Disp_ByString(SecondScreen,"Out1");	
															}break;																
															case compare1_HSY:{
														      DIV_Disp_ByString(MainScreen," HSY");         //显示输出1的预设
																	   DIV_Disp_ByString(SecondScreen,"Out1");	
															}break;
															case compare1_WCMP:{
															     DIV_Disp_ByString(MainScreen,"WCMP");         //显示输出1的预设
																	   DIV_Disp_ByString(SecondScreen,"Out1");
															}break;		
															default:{}break;
												}
									}break;
								case compareset2_status:{  
													  	DIV_Pwn_Set(&PWM_THREERED,10,0,PWM_ID_TR);//绿色提示输出2
																DIV_Pwn_Set(&PWM_DOUBGED,10,100,PWM_ID_DG);
								    switch(compsestatus2){
															case compare2_EASY:{  
																   	DIV_Disp_ByString(MainScreen,"EASY");         //显示输出1的预设
																	   DIV_Disp_ByString(SecondScreen,"Out2");	
															}break;																
															case compare2_HSY:{
														      DIV_Disp_ByString(MainScreen," HSY");         //显示输出1的预设
																	   DIV_Disp_ByString(SecondScreen,"Out2");
															}break;
															case compare2_WCMP:{
															     DIV_Disp_ByString(MainScreen,"WCMP");         //显示输出1的预设
																	   DIV_Disp_ByString(SecondScreen,"Out2");
															}break;	
															case compare2_off:{
															     DIV_Disp_ByString(MainScreen," OFF");         //显示输出1的预设
																	   DIV_Disp_ByString(SecondScreen,"Out2");
															}break;
															default:{}break;
												}
								}break;
								case noncset_status:{
													switch(noncstatus){
														case noncstatus_1c2c:{DIV_Disp_ByString(MainScreen,"1c2c");DIV_Disp_ByString(SecondScreen,"NONC");}break;
														case noncstatus_1o2c:{DIV_Disp_ByString(MainScreen,"1o2c");DIV_Disp_ByString(SecondScreen,"NONC");}break;
														case noncstatus_1c2o:{DIV_Disp_ByString(MainScreen,"1c2o");DIV_Disp_ByString(SecondScreen,"NONC");}break;
														case noncstatus_1o2o:{DIV_Disp_ByString(MainScreen,"1o2o");DIV_Disp_ByString(SecondScreen,"NONC");}break;
														default:{}break;
													}
								}break;
								case reactiontimeset_status:{		
														DIV_Disp_ByString(MainScreen,"    ");    
														switch(reactiontime)
														{ 
															case reaction_10ms:	{DIV_Disp_Uint16Num(MainScreen,10);}break;
															case reaction_50ms:	{DIV_Disp_Uint16Num(MainScreen,50);}break;
															case reaction_100ms:{DIV_Disp_Uint16Num(MainScreen,100);}break;
															case reaction_250ms:{DIV_Disp_Uint16Num(MainScreen,250);}break;
															case reaction_500ms:{DIV_Disp_Uint16Num(MainScreen,500);}break;
															case reaction_2000ms:{DIV_Disp_Uint16Num(MainScreen,2000);}break;  
															case reaction_2ms:		{DIV_Disp_floatNum(MainScreen,2.0);}break;
														}   
														DIV_Disp_ByString(SecondScreen," RES");
								}break;
								case maincolorset_status:{
													switch(mainscreencolor)                    
													{                                             
														case G_ON:{DIV_Disp_ByString(MainScreen,"G-oN");DIV_Disp_ByString(SecondScreen,"CLoR");}break;
														case R_ON:{DIV_Disp_ByString(MainScreen,"R-oN");DIV_Disp_ByString(SecondScreen,"CLoR");}break;
														case AL_GREEN:{DIV_Disp_ByString(MainScreen,"GREN");DIV_Disp_ByString(SecondScreen,"CLoR");}break;
														case AL_RED:{DIV_Disp_ByString(MainScreen," RED");DIV_Disp_ByString(SecondScreen,"CLoR");}break;
													}
									}break;
								case unitconversion_status:{
													switch(unitconversion)                         
													{
														case Unit_Bar:{DIV_Disp_ByString(MainScreen," bAr");DIV_Disp_ByString(SecondScreen,"Unit");}break;
														case Unit_Kgf:{DIV_Disp_ByString(MainScreen," KgF");DIV_Disp_ByString(SecondScreen,"Unit");}break;
														case Unit_Kpr:{DIV_Disp_ByString(MainScreen," KPR");DIV_Disp_ByString(SecondScreen,"Unit");}break;
														case Unit_Mpr:{DIV_Disp_ByString(MainScreen," MPR");DIV_Disp_ByString(SecondScreen,"Unit");}break;
														case Unit_Psi:{DIV_Disp_ByString(MainScreen," PSI");DIV_Disp_ByString(SecondScreen,"Unit");}break;
														case Unit_Mmhg:{DIV_Disp_ByString(MainScreen,"MMHG");DIV_Disp_ByString(SecondScreen,"Unit");}break;
														case Unit_Inhg:{DIV_Disp_ByString(MainScreen,"inHG");DIV_Disp_ByString(SecondScreen,"Unit");}break;
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
												
										}break;
										case secscreen_disunit:{
												
										}break;
										case secscreen_disoutchoice:{
											
										}break;					
										case secscreen_discustom:{
										
										}break;																	
									}
								}break;
								case disspeedset_status:{
													switch(dispeed)
													{
														case dispeed_500ms:{
														
														}break;
														case dispeed_1000ms:{
															
														}break;
													}
								}break;
								case difleveset_status:{
													switch(diflevel)
													{
														case diflevel_level1:{
														
														}break;
														case diflevel_level2:{
																	
														}break;
														case diflevel_level3:{
														
														}break;
													}
								}break;
								case coloractset_status:{
											switch(colorreaction)
											{
												case colorreaction_out1:{
														
												}break;
												case colorreaction_out2:{
															
												}break;
											}
								}break;
								case ecoset_status:{
											switch(ecomode)
											{
												case eco_std:{
															
												}break;
												case eco_FULL:{
														
												}break;
											}
								}break;
								case factorydatereset_status:{
											switch(facrecover)
											{
												case facrecoff:{
															
												}break;
												case facrecon:{
													
												}break;
											}
								}break;																
							}
				}break;
				default:{}break;
		}
}
//====================================================================================
//=                                                                             					=
//=                 run长按选项                                                    		=
//=                                                                                  =
//====================================================================================
void MainRun_LongPressChoice(void)
{
		 switch(compsestatus1)   //长按设置
							{
										case compare1_EASY:{
														switch(compsestatus2)
														{
															case compare2_EASY:{
																			switch(runsetwhich)
																			{
																					case set_P2:{
																					  		//设置当前的p值
				                        P2_Value= Perform_numerical_Edition(P2_Value);  
																						
																					} break;
																					case set_P1:{
																					       P1_Value= Perform_numerical_Edition(P1_Value);
																					}break;
																			}
																			
															}break;
															case compare2_HSY:{
																    switch(runsetwhich8)
																			{
															      case set_P1:{
																						    P1_Value= Perform_numerical_Edition(P1_Value); 
																					} break;
																					case set_Hi2:{
																						    Hi2_Value= Perform_numerical_Edition(Hi2_Value); 
																					}break;
															      case set_Lo2:{
																						   Lo2_Value= Perform_numerical_Edition(Lo2_Value);
																					}break;
															     }
															}break;
															case compare2_off:{
																     P1_Value= Perform_numerical_Edition(P1_Value); 
															}break;
															case compare2_WCMP:{
															     switch(runsetwhich9)
																			{
															      case set_P1:{
																						 P1_Value= Perform_numerical_Edition(P1_Value); 
																						
																					} break;
																					case set_Hi2:{
																						Hi2_Value= Perform_numerical_Edition(Hi2_Value); 
																					}break;
															      case set_Lo2:{
																						Hi2_Value= Perform_numerical_Edition(Hi2_Value); 
																					}break;
															     }
															}break;
														}
										}break;
										case compare1_HSY:{
														switch(compsestatus2)
														{
															case compare2_EASY:{
																				switch(runsetwhich1)
																				{
																					case set_Lo1:{
																								 Lo1_Value= Perform_numerical_Edition(Lo1_Value);
																					}break;
																					case set_P2:{
																									 P2_Value= Perform_numerical_Edition(P2_Value);
																					}break;
																						case set_Hi1:{
																									 Hi1_Value= Perform_numerical_Edition(Hi1_Value);
																						}break;
																				}																														
															}break;
															case compare2_HSY:{
																	switch(runsetwhich2)
																	{
																		case set_Hi1:{
																						   Hi1_Value= Perform_numerical_Edition(Hi1_Value);
																		}break;
																		case set_Hi2:{
																								 Hi2_Value= Perform_numerical_Edition(Hi2_Value);
																		}break;
																		case set_Lo1:{
																						   Lo1_Value= Perform_numerical_Edition(Lo1_Value);
																		}break;
																		case set_Lo2:{
																							 Lo2_Value= Perform_numerical_Edition(Lo2_Value);
																		}break;	
																	}																											
															
															}break;
															case compare2_off:{
																				switch(runsetwhich3)
																				{
																					case set_Hi1:{
																								 Hi1_Value= Perform_numerical_Edition(Hi1_Value);
																					}break;
																					case set_Lo1:{
																							   Lo1_Value= Perform_numerical_Edition(Lo1_Value);
																					}break;
																				}																														
															
															}break;
															case compare2_WCMP:{
																			switch(runsetwhich4)
																			{
																				case set_Hi1:{
																						     Hi1_Value= Perform_numerical_Edition(Hi1_Value);
																				}break;
																				case set_Lo1:{
																								  Lo1_Value= Perform_numerical_Edition(Lo1_Value);
																							
																				}break;
																				
																				case set_Hi2:{
																								 Hi2_Value= Perform_numerical_Edition(Hi2_Value);
																				}break;
																				case set_Lo2:{
																							  Lo2_Value= Perform_numerical_Edition(Lo2_Value);
																				}break;
																			}
															}break;
														}
										}break;
										case compare1_WCMP:{
											      switch(compsestatus2)
																	{
																		case compare2_EASY:{
																						switch(runsetwhich5)
																						{
																							case set_P2:{
																											  P2_Value= Perform_numerical_Edition(P2_Value);
																							}break;
																							case set_Hi1:{
																											 Hi1_Value= Perform_numerical_Edition(Hi1_Value);
																							
																							}break;
																							case set_Lo1:{
																											 Lo1_Value= Perform_numerical_Edition(Lo1_Value);
																							}break;
																						}
																		}break;
																		case compare2_HSY:{
																						switch(runsetwhich6)
																						{
																							case set_Hi1:{
																											 Hi1_Value= Perform_numerical_Edition(Hi1_Value);

																							}break;
																							case set_Hi2:{
																												 Hi2_Value= Perform_numerical_Edition(Hi2_Value);
																							}break;
																							case set_Lo1:{
																											 Lo1_Value= Perform_numerical_Edition(Lo1_Value);

																							}break;
																							case set_Lo2:{
																											 Lo2_Value= Perform_numerical_Edition(Lo1_Value);

																							}break;
																						}
																		}break;
																		case compare2_off:{
																					switch(runsetwhich)
																					{
																						case set_Hi1:{
																											 Hi1_Value= Perform_numerical_Edition(Hi1_Value);
	
																						}break;
																						case set_Lo1:{
																											 Lo1_Value= Perform_numerical_Edition(Lo1_Value);

																						}break;
																					}
																		}break;
																		case compare2_WCMP:{
																						switch(runsetwhich7)
																						{
																							case set_Hi1:{
																											 Hi1_Value= Perform_numerical_Edition(Hi1_Value);

																							}break;
																							case set_Hi2:{
																												 Hi2_Value= Perform_numerical_Edition(Hi2_Value);

																							}break;
																							case set_Lo1:{
																											 Lo1_Value= Perform_numerical_Edition(Lo1_Value);

																							}break;
																							case set_Lo2:{
																											 Lo2_Value= Perform_numerical_Edition(Lo2_Value);

																							}break;
																						}
																		
																		}break;
																	}
											
										}break;
						}
}


//====================================================================================
//=                                                                             					=
//=   清除显示当前设置run                                                                      =
//=                                                                                  =
//====================================================================================
void Disp_NowSet(void)
{
			if(disp_delay_flag)  //关闭显示周期
			{    
			   		switch(runsetdisplay)   //按下mode显示设置哪个口
								{
									case dispset_P1:{
                DIV_Disp_ByString(SecondScreen,"    ");
																DIV_Disp_SecondDispSetwithHunit(Set_sign,P1_Value,Kpa);
																runsetdisplay&=~(dispset_P1);

									}break;
									case dispset_P2:{

                DIV_Disp_ByString(SecondScreen,"    ");
																DIV_Disp_SecondDispSetwithHunit(Set_sign,P2_Value,Kpa);
																runsetdisplay &= ~(dispset_P2);


									}break;
									case dispset_Hi1:{
                DIV_Disp_ByString(SecondScreen,"    ");
																DIV_Disp_SecondDispSetwithHunit(Set_sign,Hi1_Value,Kpa);
																runsetdisplay &= ~(dispset_Hi1);

									}break;
									case dispset_Lo1:{
               DIV_Disp_ByString(SecondScreen,"    ");
																DIV_Disp_SecondDispSetwithHunit(Set_sign,Lo1_Value,Kpa);
																runsetdisplay &= ~(dispset_Lo1);

									}break;
									case dispset_Hi2:{
                DIV_Disp_ByString(SecondScreen,"    ");
																DIV_Disp_SecondDispSetwithHunit(Set_sign,Hi2_Value,Kpa);
																runsetdisplay &= ~(dispset_Hi2);

									}break;
									case dispset_Lo2:{
                DIV_Disp_ByString(SecondScreen,"    ");
																DIV_Disp_SecondDispSetwithHunit(Set_sign,Lo2_Value,Kpa);
																runsetdisplay &= ~(dispset_Lo2);

									}break;
								}
			TMR_DisableInterrupt(TMR2, TMR_INT_UPDATE);  
				disp_delay_flag=0;
			TMR_EnableInterrupt(TMR2, TMR_INT_UPDATE);	
			}	
			
}

//====================================================================================
//=                                                                             					=
//=                 长按按键设置p                                                    =
//=                                                                                  =
//====================================================================================

uint16_t Perform_numerical_Edition(uint16_t value)
{
			 static uint16_t press_tick=0;
					static uint16_t addtime =0;
	    static uint8_t state = 0;
	
			if(Set_2sLongPressEv==2)   //按键2连按
			{   
							switch(state)
							{
								case 0:{
										if(press_tick<500)
										{press_tick++;}
										
										
													if(addtime<10)
														{addtime++;}
														else{
															addtime=0;				
															
																if(Set_sign){                   //低速 正号自增到最大
																				if(value<1050){(value)++;}           
																	}else{
																				if(value>0){(value)--;}else{Set_sign=1;value=1;}												
																	}
														}
										if(press_tick==500&&!(value%10))
										{
												state =1;
										}
								}break;
								case 1:{
														if(addtime<10)
															{addtime++;}
														else{
																addtime=0;
																if(Set_sign){               //高速 正号自增10到最大
																			if(value<1050){value+=10;
																			 if(value>1050){value=1050;}  //防止溢出
																			}
																}else{                //高速负号 自减
																					if(value>0){(value)-=10;if(value>1050){value=1050;}}else{Set_sign=1;value=10;}
																}
														}
								}break;
							}	
     DIV_Disp_SecondDispSetwithHunit(Set_sign,value,Current_Unit);	
			}
	else if(Set_2sLongPressEv==3)   //按键3连按
	{		
				switch(state)
				{
					case 0:{
									if(press_tick<500)
									{press_tick++;}
									
									if(addtime<10)
											{addtime++;}
											else{
														addtime=0;
														if(Set_sign){
																	if(value>0){(value)--;}else{Set_sign=0;value=1;}
															}else{
																			if(value<1050){(value)++;}
															}
											}
											
									if(press_tick==500&& (!(value%10)))
									{		
												state =1;
									}
					}break;
					case 1:{
											if(addtime<10)
											{addtime++;}
											else{
														addtime=0;
														if(Set_sign){
																	if(value>0){value-=10;if(value>1050){value=1050;}}else{Set_sign=0;value=10;}
															}else{
																			if(value<1050){value+=10;if(value>1050){value=1050;}}
															}
											}
					}break;
				}
				DIV_Disp_SecondDispSetwithHunit(Set_sign,value,Current_Unit);
				}
    if(Set_2sLongPressEv&&key_management.Key_State==KEY_STATE_RELEASED)
				{	
					  	state =0 ;
							press_tick=0;
							addtime=0;
							Set_2sLongPressEv=0;
				}		
			  return value;
}
//====================================================================================
//=                                                                             					=
//=                                                                                	 =
//=                                                                                  =
//====================================================================================
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
				
				
								//可查询延时时间戳
								if(Encheck_Delay_Start)
								{
								   Encheck_Delay_Tick++;
								} 
								
         if(runsetdisplay)
									{
									   	disp_delay_tick++;
												if(disp_delay_tick>1000){    //到达1s
														
																			disp_delay_flag=1;             //关闭显示
												}
									} else if(disp_delay_flag==0&&runsetdisplay==0)
									{
															    disp_delay_tick=0;
												   
									}
								
							
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
