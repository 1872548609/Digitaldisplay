 /* Includes */             
#include "superhead.h"

//枚举==================================================
//menu1 
DeviceStatus  manu1_devicestate = status_init;   //主状态

//menu2
NormsetStatus manu2_normsetstatus = noset_status;    //副设置状态
PromodsetStatus manu2_promodesetstatus = pnoset_status;     //副设置状态

//rundisplay
RunState runstate = runstate_run;	      //运行设置显示状态

//runmode
RunMode runmode = Rstate_Run_mode;       //运行状态

//以下需要记录存储共12条设置
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

//显示run设置标志	=====================================
#define dispset_P1 	0x01           //按下mode哪个闪烁模式一下
#define dispset_P2 	0x02
#define dispset_Hi1 0x03 
#define dispset_Lo1 0x04 
#define dispset_Hi2 0x10
#define dispset_Lo2 0x20

uint8_t runsetdisplay =0;

//显示延时关闭===============================================
volatile uint8_t disp_delay_flag =0;
volatile uint16_t disp_delay_tick =0;
volatile uint8_t disp_delay_start=0;
//run设置切换=============================================
#define set_no  0x00 //开机无设置
#define set_P1 	0x01
#define set_P2 	0x02
#define set_Hi1 0x04
#define set_Lo1 0x08
#define set_Hi2 0x10
#define set_Lo2 0x20

uint8_t EASY1EASY2 		= set_no;  //在什么模式下要显示设置哪个设置     
uint8_t EASY1HSY2 			= set_no;
uint8_t EASY1WCMP2 		= set_no;
uint8_t HSY1EASY2 			= set_no;
uint8_t HSY1HSY2 				= set_no;
uint8_t HSY1OFF2 				= set_no;
uint8_t HSY1WCMP2 			= set_no;
uint8_t WCMP1EASY2 		= set_no;
uint8_t WCMP1HSY2 			= set_no;
uint8_t WCMP1WCMP2 		= set_no;
uint8_t WCMP1OFF2 			= set_no;

uint8_t NOWSETWICH = set_P1;  //正在设置哪个

//模式设置配置位
uint32_t NormolmodeSetValue = 0;
uint32_t PromodeSetValue =0;

//flash================================================
#define TEXT_LENTH sizeof(g_text_buf)
#define SIZE  TEXT_LENTH/2 + ((TEXT_LENTH%2)? 1:0)   //半字对齐必须半字读写      
#define FLASH_SAVE_STARTADDR 0x0800f000
#define FLASH_SAVE_ADDR_P1   0x0800f004
#define FLASH_SAVE_ADDR_P2   0x0800f008
#define FLASH_SAVE_ADDR_Hi1  0x0800f00c
#define FLASH_SAVE_ADDR_Lo1  0x0800f010
#define FLASH_SAVE_ADDR_Hi2  0x0800f014
#define FLASH_SAVE_ADDR_Lo2  0x0800f018

#define FLASH_SAVE_ADDR_NORMSET 0x0800f01c         //norset预设存储
#define FLASH_SAVE_ADDR_PROSET	 0x0800f020         //proset预设存储

//设定值===============================================
#define ADD  1
#define DOWN  0
float P1_Value 	= 0;
float P2_Value 	= 0;
float Hi1_Value = 0;
float Lo1_Value = 0;		
float Hi2_Value = 0;
float Lo2_Value = 0;  
//长按设置============================================
uint8_t speed=10;//变化速率 * 10ms
uint16_t turntime=500; //切换时间 * 10ms
uint32_t press_time =0;//记录按下时间
uint8_t speed_mode = 0; //加速模式标志
uint8_t Key_state= 0;//长短按状态切换标志
uint8_t addcount=0;//变化间隔

float fractional_part=0;
float integer_part=0;

//pwm==================================================
uint8_t threered_pwmval = 100;
uint8_t onered_pwmval = 0;
uint8_t doubleged_pwmval = 100;
uint8_t oneged_pwmval = 100;
//10ms执行标志=========================================
uint8_t DIV_10MS; 
//状态切换时，不能在同一个循环用，可查询延时===========================================
volatile uint32_t Encheck_Delay_Tick;
volatile uint32_t Encheck_Delay_Starttime;
volatile	uint32_t Encheck_Delay_Nowtime;
volatile uint8_t Encheck_Delay_Start;

#define Delayret(); Encheck_Delay_Start=0;Encheck_Delay_Tick=0;Encheck_Delay_Start=1;

#define Delayst();  Encheck_Delay_Start=1; Encheck_Delay_Tick=0;    \
																				Encheck_Delay_Starttime=Encheck_Delay_Tick;

#define Delayck(x)  Encheck_Delay_Nowtime=Encheck_Delay_Tick;  \
																								if(Encheck_Delay_Nowtime-Encheck_Delay_Starttime>x)  
																									
#define Delayed();		 Encheck_Delay_Start=0;Encheck_Delay_Tick=0;

//ADC======================================================
 uint32_t adcData = 0;           //ad值
 uint32_t VOL_value = 0;         //电压值，参考3.3v
float Current_pressure_value=0;			       
uint8_t adc_count = 0;//adc读取次数
uint32_t adc_sum = 0; //adc累加
uint32_t adc_average = 0;//adc平均值		

uint32_t adc_temp=0;
#define WINDOW_SIZE 16
uint16_t adc_buffer[WINDOW_SIZE] = {0};
uint8_t ad_index = 0;																								
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
#define EPSILON 1e-6f   //误差容许                                                                            
//刷新气压显示==============================================
uint8_t disp_main_flag =0;
uint16_t disp_main_parcount = 0;
uint8_t disp_main_flagEN =1;//是否显示
//校零功能===============================
float Zero_gap=0;
uint8_t Zero_Key_release =0 ;//校零时是否释放
//峰值谷值保持======================================
float MAXairpress = 0;
float MINarepress = 0;
uint8_t Switch_peakvalley_flag=0;					//显示的时候闪烁		
uint16_t Peakvally_count=0;
//按键锁定=========================================
uint8_t Key_Lock=0;//按键锁定标志
//反应时间延时====================================
uint16_t reactiontimesetchannel1=2;//默认2ms    
uint16_t reactiontimesetchannel2=2;//默认2ms


uint16_t reactiontime_tickchannel1 =0;//记录时间戳
uint8_t reactiontimestart1=0;
uint8_t reactiontimeflag1 =0;


uint16_t reactiontime_tickchannel2 =0;//记录时间戳
uint8_t reactiontimestart2=0;
uint8_t reactiontimeflag2 =0;

uint16_t reactiontime_tickchannel3 =0;//记录时间戳
uint8_t reactiontimestart3=0;
uint8_t reactiontimeflag3 =0;

uint16_t reactiontime_tickchannel4 =0;//记录时间戳
uint8_t reactiontimestart4=0;
uint8_t reactiontimeflag4 =0;
//主屏颜色状态============================================


//输出状态
#define ON    1
#define OFF   0

uint8_t out1_state = 0;
uint8_t out2_state = 0;

//==========================================================

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

//显示预设
void Show_Presets (void);

//显示当前设置并切换
void NowSetWhich(void);

//清除显示当前设置run
void CLear_NowSetSHow(void);

//短按设置
void SetByShortPress(uint8_t addordown);

//长按设置
void SetByLongPress(void);

//flash写设置
void Flash_Write_SetValue(void);

//flash读设置
void Flash_Read_SetValue(void);

//输出切换
void OutPutContrl(void);

//输出反应时间延时
void Config_reacttime_delay(void);

//主屏颜色控制
void MainColorCtr(void);



int main(void)
{
					APM_EVAL_Init();       

	
	
					//Flash_Write_SetValue();   //烧写默认参数才打开，平时不用打开，不要放进循环千万
	
	    
	
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
			
			     Flash_Read_SetValue();//把预设值读入
			
			
								Delayck(1000)
								{    
								    Delayed();
											DIV_Disp_Snprintf(SecondScreen,"    ");  
												DIV_Disp_Snprintf(MainScreen,"    ");  
								  	manu1_devicestate = status_run; 
								}				
							
		}break;
		case status_run:{
				 
					switch(runmode)
					{
						case Rstate_Run_mode:
						{
								
							 if(Key_Lock)//按键锁定2.如果按键锁定
								{     
             //按键锁定3.按下锁定时显示不可操作
             //短按事件==========================
													if(	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV)
													{

														   disp_main_flagEN=0;//关闭气压显示避免刷新
																				
																	DIV_Disp_Snprintf(MainScreen,"LocK");//显示Lock
																 Delayst();	  //设置提示延时
														
																	key_management.PreWichKeyDown=0x00;//清除按下键值	
																	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV=0;	
													}
													//长按事件=====================
												if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV)
												{	
												 		if(key_management.WichKeyDown==(KEY1|KEY3))    //按键解除锁定4.
																{
                   DIV_Disp_ClearPoint();//把小数点删掉
																	
                   disp_main_flagEN=0;//气压显示关闭
																	
																			DIV_Disp_Snprintf(MainScreen,"LocK");//显示Lock
																			
																			DIV_Disp_Snprintf(SecondScreen,"OFF");//显示Lock 
																	
																			APM_EVAL_DelayMs(1000);
																	
																			disp_main_flagEN=1;//气压显示
																	  
																			switch(NOWSETWICH)     //恢复副屏显示
																			{
																				case set_P1:{
																									 	DIV_Disp_MultiplefloatNum(SecondScreen,P1_Value,1);	
																					}break;
																					case set_P2:{
																						    	DIV_Disp_MultiplefloatNum(SecondScreen,P2_Value,1);	
																					}break;
																					case set_Hi1:{
																								   	DIV_Disp_MultiplefloatNum(SecondScreen,Hi1_Value,1);	
																					}break;
																					case set_Lo1:{
																									  	DIV_Disp_MultiplefloatNum(SecondScreen,Lo1_Value,1);	
																					}break;
																					case set_Hi2:{
																										 	DIV_Disp_MultiplefloatNum(SecondScreen,Hi2_Value,1);	
																					}break;
																					case set_Lo2:{
																									  	DIV_Disp_MultiplefloatNum(SecondScreen,Lo2_Value,1);	
																					}break;
																			}
																			
																			Key_Lock=0;    //锁定解除
																}
															key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=0;		
												}


            Delayck(1000)//短按延时关闭lock提示
												{   
															Delayed();
														  disp_main_flagEN=1;//气压显示刷新
												}	

												
								}
								else //按键未锁定 正常执行run内容
								{
										MainColorCtr();//主屏颜色控制

									
									CLear_NowSetSHow();//清除显示runset    
													
									//读取气压值
										Current_pressure_value=(float)adcData*0.059f-109.28f;
								
										//校零功能部分	
										if(Zero_gap)  //校零2.如果设置了校零
										{
													Current_pressure_value-=Zero_gap;     //当前气压永远加上偏差
										}
										
										if(Zero_Key_release)  //校零3.在定时器检测按键释放key23长按就恢复显示气压
										{
											 Zero_Key_release=0;
												disp_main_flagEN=1;//气压显示
										}
												
											//显示气压值
										if(disp_main_flag)    //定时器刷新
										{
												if(disp_main_flagEN)  //使能
												{ 
															disp_main_flag=0;
														DIV_Disp_MultiplefloatNum(MainScreen,Current_pressure_value,1);	
														
												}
										}
								
										
								//判断输出	
									OutPutContrl();
										
										
										//短按事件==========================
													if(	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV)
													{
														
																	if(key_management.PreWichKeyDown==KEY1)
																	{
																							NowSetWhich(); //显示设定
																	}										
																	if(key_management.PreWichKeyDown==KEY2)
																	{
																							
																					SetByShortPress(ADD);//设定值		
																							
																							
																	}
																	if(key_management.PreWichKeyDown==KEY3)
																	{
																		
																					SetByShortPress(DOWN);//设定值	 
																						
																							
																	}
																	key_management.PreWichKeyDown=0x00;	
																	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV=0;	
													}
												
												//长按事件=====================
												if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV)
												{	
																if(key_management.WichKeyDown==KEY1)
																{ 	
																	manu1_devicestate=status_normset;//进入设置模式								
																	manu2_normsetstatus=noset_status;	
																	Show_Presets();
																}
																
																
																if(key_management.WichKeyDown==(KEY3|KEY2))    //1.如果在run模式下长按key12 设置校零
																{
																				Zero_Key_release=0;//校零按键未释放  
																	
																				disp_main_flagEN=0;//先关闭气压显示  等校零按键释放恢复
																	
																				Current_pressure_value=(float)adcData*0.059f-109.28f; //读取一次气压值
																	
																				Zero_gap = Current_pressure_value-0.0f;    //计算校零的差值
																				
																				DIV_Disp_Snprintf(MainScreen,"0000");//显示四个零		
																				APM_EVAL_DelayMs(1000);
																}
																
																if(key_management.WichKeyDown==(KEY1|KEY2))    //峰谷1.如果在run模式下长按key123 峰值谷值保持
																{

																			DIV_Disp_Snprintf(MainScreen,"PEAK");//显示PEAK
																			
																			DIV_Disp_Snprintf(SecondScreen,"botm");//显示botm 
																	
																			APM_EVAL_DelayMs(1000);
																	
																			runmode = Rstate_Peakvalley_mode;    //进入峰值谷值模式
																		
																}
																if(key_management.WichKeyDown==(KEY1|KEY3))    //按键锁定1.
																{
																			DIV_Disp_ClearPoint();//把小数点删掉

																	  disp_main_flagEN=0;//关闭气压显示避免刷新
																	
																			DIV_Disp_Snprintf(MainScreen,"LocK");//显示Lock
																			
																			DIV_Disp_Snprintf(SecondScreen,"ON");//显示Lock 
																	
																			APM_EVAL_DelayMs(1000);
																	
																	  disp_main_flagEN=1;//气压显示
																	
																			Key_Lock=1;    //锁定
																	
																	  	switch(NOWSETWICH)     //恢复副屏显示
																			{
																				case set_P1:{
																									 	DIV_Disp_MultiplefloatNum(SecondScreen,P1_Value,1);	
																					}break;
																					case set_P2:{
																						    	DIV_Disp_MultiplefloatNum(SecondScreen,P2_Value,1);	
																					}break;
																					case set_Hi1:{
																								   	DIV_Disp_MultiplefloatNum(SecondScreen,Hi1_Value,1);	
																					}break;
																					case set_Lo1:{
																									  	DIV_Disp_MultiplefloatNum(SecondScreen,Lo1_Value,1);	
																					}break;
																					case set_Hi2:{
																										 	DIV_Disp_MultiplefloatNum(SecondScreen,Hi2_Value,1);	
																					}break;
																					case set_Lo2:{
																									  	DIV_Disp_MultiplefloatNum(SecondScreen,Lo2_Value,1);	
																					}break;
																			}
																	
																		
																}
															key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=0;		
												}		
												
													//设置
													SetByLongPress(); //长按设置当前值
								}	
										
						
						}break;
						case Rstate_Peakvalley_mode:{//峰谷2.模式
						
							    //长按事件=====================
								if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV)
								{	
											
												if(key_management.WichKeyDown==(KEY1|KEY2))    //峰谷4.如果在run模式下长按key123 峰值谷值保持退出
												{

															
													
															DIV_Disp_Snprintf(MainScreen,"PEAK");//显示PEAK
															
															DIV_Disp_Snprintf(SecondScreen,"botm");//显示botm
													
															MAXairpress=0;//清除峰值
													
															MINarepress=0;//清除谷值
													
													  APM_EVAL_DelayMs(1000);
													
															runmode = Rstate_Run_mode;    //回到run模式
														
												}
									
											key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=0;		
								}
							
							
						     //获取气压值
										Current_pressure_value=(float)adcData*0.059f-109.28f;      //由adc转换实时获取
									
										if(Zero_gap)  //如果设置了校零，在峰谷值外设置的
										{
													Current_pressure_value-=Zero_gap;     //当前气压永远加上偏差
										}
										
										 //峰值谷值记录
											if(Current_pressure_value>MAXairpress)     
										{
													MAXairpress=Current_pressure_value;
										}
										if(Current_pressure_value<MINarepress)
										{
													MINarepress=Current_pressure_value;
										}	
										
										//显示峰值谷值
											if(disp_main_flag)    //定时器刷新
										{
												if(disp_main_flagEN)  //使能
												{ 
															disp_main_flag=0;
														  
															if(Switch_peakvalley_flag)      //在定时器取反闪烁
															{
																			
																	  DIV_Disp_MultiplefloatNum(MainScreen,MAXairpress,1);	
																		DIV_Disp_MultiplefloatNum(SecondScreen,MINarepress,1);	
															}
														 else
															{
															   	DIV_Disp_Snprintf(MainScreen,"PEAK");//显示PEAK
																	DIV_Disp_Snprintf(SecondScreen,"botm");//显示botm
															}
												}
										}
										
										//判断输出
									OutPutContrl();
										
										
										
										
						}break;
					}

   
				   	

			

			
			
			
						
		}break;
		case status_normset:{
		 	
							 //触发短按事件
								if(	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV)
								{
												if(key_management.PreWichKeyDown==KEY1)  	//短按mode键
												{
													    if(manu2_normsetstatus==unitconversion_status){
																		 manu1_devicestate=status_run;
																			manu2_normsetstatus=compareset1_status;
																	}
																	else{manu2_normsetstatus++;}
																	Show_Presets();
												}										
												if(key_management.PreWichKeyDown==KEY2)
												{
																	switch(manu2_normsetstatus){   //切换设置项	
																		case noset_status:{};break;
																		case compareset1_status :{ 
																				if(compsestatus1==compare1_HSY){compsestatus1=compare1_EASY;}
																				else{compsestatus1=compsestatus1<<1;}
																		}break;
																		case compareset2_status	:{
																					if(compsestatus2==compare2_HSY){compsestatus2=compare2_off;}
																					else{compsestatus2=compsestatus2<<1;}
																		}break;
																		case noncset_status:{
																					if(noncstatus==noncstatus_1c2o){noncstatus=noncstatus_1o2o;}
																					else{noncstatus=noncstatus<<1;}
																		}break;
																		case reactiontimeset_status:{
																					if(reactiontime ==reaction_2000ms){reactiontime=reaction_2ms;}
																					else{reactiontime=reactiontime<<1;}	
																		}break;
																		case maincolorset_status:{
																					if(mainscreencolor==AL_GREEN){mainscreencolor=R_ON;}
																					else{mainscreencolor=mainscreencolor<<1;}
																			}break;
																		case unitconversion_status:{
																					if(unitconversion==Unit_Inhg){unitconversion=Unit_Mpr;}
																		   else{unitconversion=unitconversion<<1;}
																		}break;																			
																		default:{}break;
																}
																//显示设置
																Show_Presets();	
												}
												if(key_management.PreWichKeyDown==KEY3)
												{
																     //切换设置项
													   switch(manu2_normsetstatus){   //切换设置项	
																		case noset_status:{};break;
																		case compareset1_status :{ 
																				if(compsestatus1==compare1_EASY){compsestatus1=compare1_HSY;}
																				else{compsestatus1=compsestatus1>>1;}
																		}break;
																		case compareset2_status	:{
																					if(compsestatus2==compare2_off){compsestatus2=compare2_HSY;}
																					else{compsestatus2=compsestatus2>>1;}
																		}break;
																		case noncset_status:{
																					if(noncstatus==noncstatus_1o2o){noncstatus=noncstatus_1c2o;}
																					else{noncstatus=noncstatus>>1;}
																		}break;
																		case reactiontimeset_status:{
																					if(reactiontime ==reaction_2ms){reactiontime=reaction_2000ms;}
																					else{reactiontime=reactiontime>>1;}
																					Config_reacttime_delay();//配置输出反应时间延时
																		}break;
																		case maincolorset_status:{
																					if(mainscreencolor==R_ON){mainscreencolor=AL_GREEN;}
																					else{mainscreencolor=mainscreencolor>>1;}
																			}break;
																		case unitconversion_status:{
																					if(unitconversion==Unit_Mpr){unitconversion=Unit_Inhg;}
																		   else{unitconversion=unitconversion>>1;}
																		}break;																			
																		default:{}break;
																}
																//显示设置
																Show_Presets();	
													
												}
												key_management.PreWichKeyDown=0x00;	
													key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV=0;
									}
								  //长按事件=====================
									if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV)
									{
													if(key_management.WichKeyDown==KEY1)
													{      
														manu1_devicestate=status_run;
														manu2_normsetstatus=noset_status;
														manu2_promodesetstatus=pnoset_status;			
														Show_Presets();
													} 		
													key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=0;
									}
							  	if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV)
									{
													if(key_management.WichKeyDown==KEY1)
													{      
														manu1_devicestate=status_proset;//进入设置模式	
														manu2_normsetstatus=noset_status;
														manu2_promodesetstatus=pnoset_status;			
														Show_Presets();
													} 		
													key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV=0;
									}
									
									
			
			
			
		}break;
		case status_proset:{
		
         //触发短按事件
								if(	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV)
								{
												if(key_management.PreWichKeyDown==KEY1)  	//短按mode键
												{
													    if(manu2_promodesetstatus==factorydatereset_status){
																		 manu1_devicestate=status_run;
																			manu2_promodesetstatus=pnoset_status;
																	}
																	else{manu2_promodesetstatus++;}
																	Show_Presets();
												}										
												if(key_management.PreWichKeyDown==KEY2)
												{
																
												}
												if(key_management.PreWichKeyDown==KEY3)
												{
																
													
												}
												key_management.PreWichKeyDown=0x00;	
													key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV=0;
									}
								  //长按事件=====================
									if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV)
									{
													if(key_management.WichKeyDown==KEY1)
													{      
														manu1_devicestate=status_run;	
														manu2_promodesetstatus=pnoset_status;			
														Show_Presets();
													} 		
													key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=0;
									}
		}break;
	}
}


//====================================================================================
//=                                                                             					=
//=   主屏颜色控制                                                              	=
//=                                                                                  =
//====================================================================================
void MainColorCtr(void)
{
		switch(mainscreencolor)
		{
			case R_ON:{
								if(out1_state||out2_state) 
								{
												DIV_Pwn_Set(&PWM_THREERED,10,100,PWM_ID_TR);
												DIV_Pwn_Set(&PWM_DOUBGED,10,0,PWM_ID_DG);
								}
								else
								{
											DIV_Pwn_Set(&PWM_THREERED,10,0,PWM_ID_TR);
											DIV_Pwn_Set(&PWM_DOUBGED,10,100,PWM_ID_DG);
								}
			}break;
			case G_ON:{
								if(out1_state||out2_state) 
								{
												DIV_Pwn_Set(&PWM_THREERED,10,0,PWM_ID_TR);
												DIV_Pwn_Set(&PWM_DOUBGED,10,100,PWM_ID_DG);
								}
								else
								{
											DIV_Pwn_Set(&PWM_THREERED,10,100,PWM_ID_TR);
											DIV_Pwn_Set(&PWM_DOUBGED,10,0,PWM_ID_DG);
								}
			}break;
			case AL_RED:{
			
			}break;
			case AL_GREEN:{
			
			
			}break;
		}	
}
//====================================================================================
//=                                                                             					=
//=   配置反应延时选项                                                              	=
//=                                                                                  =
//====================================================================================
void Config_reacttime_delay(void)
{
		switch(reactiontime)
		{
			case reaction_100ms		:{reactiontimesetchannel1=100;reactiontimesetchannel2=100;}break;
			case reaction_10ms			:{reactiontimesetchannel1=10;reactiontimesetchannel2=10;}break;
				case reaction_2000ms:{reactiontimesetchannel1=2000;reactiontimesetchannel2=2000;}break;
			case reaction_250ms		:{reactiontimesetchannel1=250;reactiontimesetchannel2=250;}break;
			case reaction_2ms				:{reactiontimesetchannel1=2;reactiontimesetchannel2=2;}break;
			case reaction_500ms		:{reactiontimesetchannel1=500;reactiontimesetchannel2=500;}break;
			case reaction_50ms			:{reactiontimesetchannel1=50;reactiontimesetchannel2=50;}break;
		}
}

//====================================================================================
//=                                                                             					=
//=   按下mode的第一个预设准备                                                              	=
//=                                                                                  =
//====================================================================================
void FirstPreset(void)
{
		switch(compsestatus1)   
	{
				case compare1_EASY:{
								switch(compsestatus2)
								{
									case compare2_EASY:{																												
													NOWSETWICH=EASY1EASY2;
									}break;
									case compare2_HSY:{
													switch(EASY1HSY2)
														{
															case set_no:{            
														        
																							runsetdisplay=dispset_P1;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																							EASY1HSY2=set_P1; 
														    
														}break;
															case set_P1:{
																							runsetdisplay=dispset_Hi2;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																							EASY1HSY2=set_Hi2;  
															}break;
															case set_Hi2:{
																								runsetdisplay=dispset_Lo2;
																								DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																								EASY1HSY2=set_Lo2;
															}break;
															case set_Lo2:{
																					runsetdisplay=dispset_P1;
																					DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																				EASY1HSY2=set_P1;
															}break;
														}
														NOWSETWICH = EASY1HSY2;
									}break;
									case compare2_off:{
																runsetdisplay=dispset_P1;
																DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																NOWSETWICH=set_P1; //设置的时候设置p1
									}break;
									case compare2_WCMP:{
													switch(EASY1WCMP2)
														{
															case set_no:{            
														        
																							runsetdisplay=dispset_P1;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																							EASY1WCMP2=set_P1; 
														    
														}break;
															case set_P1:{  
																		
																							runsetdisplay=dispset_Hi2;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																							EASY1WCMP2=set_Hi2; 
																						
															}break;
															case set_Hi2:{
																				
																			
																								runsetdisplay=dispset_Lo2;
																								DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																								EASY1WCMP2=set_Lo2;
																				
															}break;
															case set_Lo2:{
																			
																			
																									runsetdisplay=dispset_P1;
																									DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																									EASY1WCMP2=set_P1;
																				
															}break;
														}
									     NOWSETWICH = EASY1WCMP2;
									}break;
								}
				}break;
				case compare1_HSY:{
								switch(compsestatus2)
								{
									case compare2_EASY:{
														switch(HSY1EASY2)
														{
															case set_no:{            
														        
																							runsetdisplay=dispset_P2;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)2);
																							HSY1EASY2=set_P2; 
														    
														}break;
															case set_P2:{
																					
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							HSY1EASY2=set_Hi1; 
																					
															}break; 
															case set_Hi1:{
																						
																							runsetdisplay=set_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																							HSY1EASY2=set_Lo1; 
																			
																}break;
															case set_Lo1:{
																			
																							runsetdisplay=dispset_P2;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)2);
																							HSY1EASY2=set_P2; 
															}break;
														}	
															NOWSETWICH=HSY1EASY2;
									}break;
									case compare2_HSY:{
											switch(HSY1HSY2)
											{
												case set_no:{            
														        
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							HSY1HSY2=set_Hi1; 
														    
														}break;
												case set_Hi1:{
																			    runsetdisplay=dispset_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																							HSY1HSY2=set_Lo1; 
																							 
																				
												}break; 
												case set_Lo1:{
																		
																						runsetdisplay=dispset_Hi2;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																							HSY1HSY2=set_Hi2;	
																					
												}break;
												case set_Hi2:{
																			
																						runsetdisplay=dispset_Lo2;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																							HSY1HSY2=set_Lo2; 	
																				
												}break;
										
												case set_Lo2:{
																				
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							HSY1HSY2=set_Hi1; 
																					
												}break;	
											}																											
									  NOWSETWICH=HSY1HSY2;
									}break;
									case compare2_off:{
														switch(HSY1OFF2)
														{
															case set_Hi1:{
																				
																							runsetdisplay=dispset_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																							HSY1OFF2=set_Lo1; 
																					
															}break;
															case set_Lo1:{
																			
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							HSY1OFF2=set_Hi1; 
																					
															}break;
														}																														
									
									}break;
									case compare2_WCMP:{
													switch(HSY1WCMP2)
													{
															case set_no:{            
														        
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							HSY1WCMP2=set_Hi1; 
														    
														}break;
														case set_Hi1:{
																			
																								runsetdisplay=dispset_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																							HSY1WCMP2=set_Lo1;
																						
														}break;
														case set_Lo1:{
																			
																							runsetdisplay=dispset_Hi2;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																							HSY1WCMP2=set_Hi2; 
																						
														}break;
														
														case set_Hi2:{
																			
																							runsetdisplay=dispset_Lo2;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																							HSY1WCMP2=set_Lo2; 
																						
														}break;
														case set_Lo2:{
																		
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							HSY1WCMP2=set_Hi1; 
																						
														}break;
													}
													NOWSETWICH=HSY1WCMP2;
									}break;
								}
				}break;
				case compare1_WCMP:{
									switch(compsestatus2)
								{
									case compare2_EASY:{
													switch(WCMP1EASY2)
													{
															case set_no:{            
														        
																							runsetdisplay=dispset_P2;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)2);
																							WCMP1EASY2=set_P2; 
														    
														}break;
														case set_P2:{
																				
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							WCMP1EASY2=set_Hi1; 
																						
														}break;
														case set_Hi1:{
																					
																							runsetdisplay=dispset_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																							WCMP1EASY2=set_Lo1; 
																					
														
														}break;
														case set_Lo1:{
																						
																							runsetdisplay=dispset_P2;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)2);
																							WCMP1EASY2=set_P2; 
														}break;
													}
													NOWSETWICH=WCMP1EASY2;
									}break;
									case compare2_HSY:{
													switch(WCMP1HSY2)
													{
														case set_no:{            
														        
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							WCMP1HSY2=set_Hi1;  
														    
														}break;
														case set_Hi1:{
																						 runsetdisplay=dispset_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo1-%d",(int8_t)1);
																							WCMP1HSY2=set_Lo1;
																							 
																					
														}break; 
														case set_Lo1:{
																				   runsetdisplay=dispset_Hi2;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																							WCMP1HSY2=set_Hi2;
																							 
																					
														}break;
														case set_Hi2:{
																						 runsetdisplay=dispset_Lo2;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																							WCMP1HSY2=set_Lo2;
														}break;
														case set_Lo2:{
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							WCMP1HSY2=set_Hi1; 
														}break;
													}
													NOWSETWICH=WCMP1HSY2;
									}break;
									case compare2_off:{
												switch(WCMP1OFF2)
												{
													case set_no:{            
														        
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							WCMP1OFF2=set_Hi1;  
														    
														}break;
													case set_Hi1:{
																						
																							runsetdisplay=dispset_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																							WCMP1OFF2=set_Lo1; 
																					
													}break;
													case set_Lo1:{
																					
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							WCMP1OFF2=set_Hi1; 
																					
													}break;
												}
												NOWSETWICH=WCMP1OFF2;
									}break;
									case compare2_WCMP:{
													switch(WCMP1WCMP2)
													{
														case set_no:{            
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							WCMP1WCMP2=set_Hi1; 
														}break;
														case set_Hi1:{
																						 runsetdisplay=dispset_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo1-%d",(int8_t)2);
																							WCMP1WCMP2=set_Lo1;
																						 
																					
														}break;
														case set_Hi2:{
																					  runsetdisplay=dispset_Lo2;
																							DIV_Disp_Snprintf(SecondScreen,"Lo2-%d",(int8_t)2);
																							WCMP1WCMP2=set_Lo2; 
																							 
																					
														}break;
														case set_Lo1:{
																					  	runsetdisplay=dispset_Hi2;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																							WCMP1WCMP2=set_Hi2;
																							
																					
														}break;
														case set_Lo2:{
																			
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							WCMP1WCMP2=set_Hi1; 
																					
														}break;
													}
									    NOWSETWICH=WCMP1WCMP2;
									}break;
								}
				}break;
	}
	
}


//====================================================================================
//=                                                                             					=
//=   输出控制                                                                	=
//=                                                                                  =
//====================================================================================
void OutPutContrl(void)
{
		switch(compsestatus1)   //在run模式下按下mode显示不同的设置选项
		{
					case compare1_EASY:{
									switch(compsestatus2)
									{
										case compare2_EASY:{																												
															//双EASY应差模式
											    if(Current_pressure_value>P1_Value)
															{
																		out1_state=1;
																
																		reactiontimestart2=0;
																
																		reactiontimestart1=1;//开始on延时
																		if(reactiontime_tickchannel1==reactiontimesetchannel1)
																		{
																			 OUT1_on;
																		}
															}
															else
															{
																			out1_state =0;
																
																			reactiontimestart1=0;//关闭on延时
																
																			reactiontimestart2=1;
																			if(reactiontime_tickchannel2==reactiontimesetchannel1)
																			{
																					OUT1_off;
																			}
															}
															if(Current_pressure_value>P2_Value)
															{
																			out2_state=1;
																
																				reactiontimestart4=0;
																
																		reactiontimestart3=1;//开始on延时
																		if(reactiontime_tickchannel3==reactiontimesetchannel2)
																		{
																			 OUT2_on;
																		}
															}
															else
															{
			                 out2_state=0;
																
																				reactiontimestart3=0;//关闭on延时
																
																			reactiontimestart4=1;
																			if(reactiontime_tickchannel4==reactiontimesetchannel2)
																			{
																					OUT2_off;
																			}
																	
															}
										}break;
										case compare2_HSY:{
													
										}break;
										case compare2_WCMP:{
														
													
										}break;
										case compare2_off:{
										
										}break;
									}
					}break;
					case compare1_HSY:{
									switch(compsestatus2)
									{
										case compare2_EASY:{
														
										}break;
										case compare2_HSY:{
											
										}break;
										case compare2_off:{
															
										}break;
										case compare2_WCMP:{
													
										}break;
									}
					}break;
					case compare1_WCMP:{
										switch(compsestatus2)
									{
										case compare2_EASY:{
													
										}break;
										case compare2_HSY:{
													
										}break;
										case compare2_off:{
											
													NOWSETWICH=WCMP1OFF2;
										}break;
										case compare2_WCMP:{
												
										}break;
									}
					}break;
		}
}




//====================================================================================
//=                                                                             					=
//=   判断是否改变了数值，把设定值写入flash                                                                	=
//=                                                                                  =
//====================================================================================
void Flash_Write_SetValue(void)
{   
		  float temp = 0;
				uint8_t ifchange = 0; 

				uint32_t temp1 =0;
	
	   temp= *(float*)FLASH_SAVE_ADDR_P1;
				if(fabsf(temp-P1_Value)>EPSILON){ifchange =1;}
				
	    temp = *(float*)FLASH_SAVE_ADDR_P2;
				if(fabsf(temp-P2_Value)>EPSILON){ifchange =1;}
				
				 temp = *(float*)FLASH_SAVE_ADDR_Hi1;
				if(fabsf(temp-Hi1_Value)>EPSILON){ifchange =1;}
				
				 temp = *(float*)FLASH_SAVE_ADDR_Lo1;
				if(fabsf(temp-Lo1_Value)>EPSILON){ifchange =1;}
				
				 temp = *(float*)FLASH_SAVE_ADDR_Hi2;
				if(fabsf(temp-Hi2_Value)>EPSILON){ifchange =1;}
				
	    temp = *(float*)FLASH_SAVE_ADDR_Lo2;
				if(fabsf(temp-Lo2_Value)>EPSILON){ifchange =1;}
				
				

	   NormolmodeSetValue= compsestatus1|compsestatus2|noncstatus|reactiontime|mainscreencolor|unitconversion;
	   PromodeSetValue= secscreen|dispeed|diflevel|colorreaction|ecomode|facrecover;
	
				temp1 = *(volatile uint32_t*)FLASH_SAVE_ADDR_NORMSET;
				if(temp1!=NormolmodeSetValue){ifchange=1;}
				temp1 = *(volatile uint32_t*)FLASH_SAVE_ADDR_PROSET;
				if(temp1!=PromodeSetValue){ifchange=1;}
				
				
				
				if(ifchange)
				{
				   FMC_Unlock(); /* FLASH 解锁 */
	
							FMC_ErasePage(FLASH_SAVE_STARTADDR);//清除页
					
							FMC_ProgramWord(FLASH_SAVE_ADDR_P1,	*(uint32_t *)&P1_Value);//用整型的方式读取浮点数存储到32位里
					  FMC_ProgramWord(FLASH_SAVE_ADDR_P2,	*(uint32_t *)&P2_Value);
				   FMC_ProgramWord(FLASH_SAVE_ADDR_Hi1,*(uint32_t *)&Hi1_Value);
							FMC_ProgramWord(FLASH_SAVE_ADDR_Lo1,*(uint32_t *)&Lo1_Value);
							FMC_ProgramWord(FLASH_SAVE_ADDR_Hi2,*(uint32_t *)&Hi2_Value);
							FMC_ProgramWord(FLASH_SAVE_ADDR_Lo2,*(uint32_t *)&Lo2_Value);
					
					  FMC_ProgramWord(FLASH_SAVE_ADDR_NORMSET,*(uint32_t *)&NormolmodeSetValue);
							FMC_ProgramWord(FLASH_SAVE_ADDR_PROSET,*(uint32_t *)&PromodeSetValue);
					
							FMC_Lock(); /* 上锁 */		
				}
				
				ifchange =0;
} 
//====================================================================================
//=                                                                             					=
//=   把设定值读出来                                                               	=
//=                                                                                  =
//====================================================================================
void Flash_Read_SetValue(void)
{
	
	   P1_Value= *(volatile 	float *)FLASH_SAVE_ADDR_P1;
	   P2_Value= *(volatile 	float *)FLASH_SAVE_ADDR_P2;
	   Hi1_Value= *(volatile float *)FLASH_SAVE_ADDR_Hi1;
	   Lo1_Value= *(volatile float *)FLASH_SAVE_ADDR_Lo1;
	   Hi2_Value= *(volatile float *)FLASH_SAVE_ADDR_Hi2;
	   Lo2_Value= *(volatile float *)FLASH_SAVE_ADDR_Lo2;
	
				compsestatus1		=(*(volatile uint32_t*)FLASH_SAVE_ADDR_NORMSET)&0x07;
				compsestatus2		=(*(volatile uint32_t*)FLASH_SAVE_ADDR_NORMSET)&0x78;
	   noncstatus					=(*(volatile uint32_t*)FLASH_SAVE_ADDR_NORMSET)&0x780;
				reactiontime			=(*(volatile uint32_t*)FLASH_SAVE_ADDR_NORMSET)&0x3F800;
	   mainscreencolor=(*(volatile uint32_t*)FLASH_SAVE_ADDR_NORMSET)&0x3C0000;
	   unitconversion	=(*(volatile uint32_t*)FLASH_SAVE_ADDR_NORMSET)&0x1FC00000;
	
	   secscreen 				=(*(volatile uint32_t*)FLASH_SAVE_ADDR_PROSET)&0x0f;
	   dispeed							=(*(volatile uint32_t*)FLASH_SAVE_ADDR_PROSET)&0x30;
	   diflevel						=(*(volatile uint32_t*)FLASH_SAVE_ADDR_PROSET)&0x1C0;
	   colorreaction	=(*(volatile uint32_t*)FLASH_SAVE_ADDR_PROSET)&0x600;
	   ecomode							=(*(volatile uint32_t*)FLASH_SAVE_ADDR_PROSET)&0x1800;
    facrecover				=(*(volatile uint32_t*)FLASH_SAVE_ADDR_PROSET)&0x6000;                                                      
}
//====================================================================================
//=                                                                             					=
//=   长按设定                                                                	=
//=                                                                                  =
//====================================================================================

void SetByLongPress(void)
{  
	   	float temp=0;
					uint8_t flash_ifwrite=0;
	
	    switch(NOWSETWICH)       //选择要设定的值
				{
					case set_P1:{
					     temp=P1_Value;
										
					}break;
					case set_P2:{
					     temp=P2_Value;
					}break;
					case set_Hi1:{
					
					}break;
					case set_Lo1:{
					
					}break;
					case set_Hi2:{
					
					}break;
					case set_Lo2:{
					
					}break;
				}
	
   	if(key_management.WichKeyDown)//按键没松开,连按设置
			{
				if(key_management.WichKeyDown==KEY2&&key_management.WichKeyDown!=KEY3)        //如果是按键2连按
				{  
							
							if(press_time<turntime){press_time++;}    //计时
							if(addcount<speed){addcount++;}
								else{
					   
								if(press_time>100&&press_time<turntime){  //1s后连按  
												flash_ifwrite=1;   //结束后写flash
												addcount=0;
												temp+=0.1f;
								}
								if(press_time==turntime)   //判断长按时间到了没
								{  
											addcount=0;
											temp+=1.0f;
								}
							 DIV_Disp_MultiplefloatNum(SecondScreen,temp,1);//显示
						}
				}
				if(key_management.WichKeyDown!=KEY2&&key_management.WichKeyDown==KEY3)        //如果是按键2连按
				{  
							
							if(press_time<turntime){press_time++;}    //计时
							if(addcount<speed){addcount++;}
								else{
					   
								if(press_time>100&&press_time<turntime){  //1s后连按    
											flash_ifwrite=1; //结束后写flash
												addcount=0;                                        
												temp-=0.1f;
								}
								if(press_time==turntime)   //判断长按时间到了没
								{  
											addcount=0;
											temp-=1.0f;
								}
							 DIV_Disp_MultiplefloatNum(SecondScreen,temp,1);//显示
						}
				}
			}				
			else//按键释放
			{
						press_time=0;
						addcount=0;
						if(flash_ifwrite)
						{
								flash_ifwrite=0;
							// Flash_Write_SetValue();//如果改变就写入设定值
						}
			}	
			
				switch(NOWSETWICH)       //赋值给设定的值
				{
					case set_P1:{
					    P1_Value=temp;
										
					}break;
					case set_P2:{
					     P2_Value=temp;
					}break;
					case set_Hi1:{
					
					}break;
					case set_Lo1:{
					
					}break;
					case set_Hi2:{
					
					}break;
					case set_Lo2:{
					
					}break;
				}
}

//====================================================================================
//=                                                                             					=
//=   短按设定                                                                	=
//=                                                                                  =
//====================================================================================
void SetByShortPress(uint8_t addordown)
{
	   float testvalue =0 ;   
	
	   switch(NOWSETWICH)       //选择要设定的值
				{
					case set_P1:{
					     testvalue=P1_Value;
										
					}break;
					case set_P2:{
					     testvalue=P2_Value;
					}break;
					case set_Hi1:{
					
					}break;
					case set_Lo1:{
					
					}break;
					case set_Hi2:{
					
					}break;
					case set_Lo2:{
					
					}break;
				}
				 
				if(addordown) //加法
				{
						 //增加设定值
							if(testvalue<105.0f){testvalue+=0.1f;}
							DIV_Disp_MultiplefloatNum(SecondScreen,testvalue,1);//显示
				}
				else
				{
				   //减少设定值
							if(testvalue>-105.0f){testvalue-=0.1f;	
							float temp=fabsf(testvalue);
							if(temp>105.0f){testvalue=-105.0f;}
							}
							DIV_Disp_MultiplefloatNum(SecondScreen,testvalue,1);//显示
				}
				
				switch(NOWSETWICH)       //赋值给设定的值
				{
					case set_P1:{
					    P1_Value=testvalue;
										
					}break;
					case set_P2:{
					     P2_Value=testvalue;
					}break;
					case set_Hi1:{
					
					}break;
					case set_Lo1:{
					
					}break;
					case set_Hi2:{
					
					}break;
					case set_Lo2:{
					
					}break;
				}

			//	Flash_Write_SetValue();
			
}

//====================================================================================
//=                                                                             					=
//=   显示当前设置run并选择设置哪个                                                    	=
//=                                                                                  =
//====================================================================================
void NowSetWhich(void)
{
	DIV_Disp_ClearPoint();
		switch(compsestatus1)   //在run模式下按下mode显示不同的设置选项
	{
				case compare1_EASY:{
								switch(compsestatus2)
								{
									case compare2_EASY:{																												
													switch(EASY1EASY2)
													{
														case set_no:{            //开机的初始化，对功能无影响，放在初始化的时候把空的设置项变成第一个选项用于防止开始的时候进不去选项，并且保证每次进来都是从第一个选项开始0-》1-》2-》1
														        
																							runsetdisplay=dispset_P1;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																							EASY1EASY2=set_P1; 
														    
														}break;
														case set_P1:{
																							
																							runsetdisplay=dispset_P2;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)2);
																							EASY1EASY2=set_P2; 
														}break;
														case set_P2:{
																							
																							runsetdisplay=dispset_P1;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																							EASY1EASY2=set_P1; 
														
														}break;
													}
													NOWSETWICH = EASY1EASY2;
									}break;
									case compare2_HSY:{
													switch(EASY1HSY2)
														{
															case set_no:{            
														        
																							runsetdisplay=dispset_P1;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																							EASY1HSY2=set_P1; 
														    
														}break;
															case set_P1:{
																							runsetdisplay=dispset_Hi2;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																							EASY1HSY2=set_Hi2;  
															}break;
															case set_Hi2:{
																								runsetdisplay=dispset_Lo2;
																								DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																								EASY1HSY2=set_Lo2;
															}break;
															case set_Lo2:{
																					runsetdisplay=dispset_P1;
																					DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																				EASY1HSY2=set_P1;
															}break;
														}
														NOWSETWICH = EASY1HSY2;
									}break;
									case compare2_off:{
																runsetdisplay=dispset_P1;
																DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																NOWSETWICH=set_P1; //设置的时候设置p1
									}break;
									case compare2_WCMP:{
													switch(EASY1WCMP2)
														{
															case set_no:{            
														        
																							runsetdisplay=dispset_P1;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																							EASY1WCMP2=set_P1; 
														    
														}break;
															case set_P1:{  
																		
																							runsetdisplay=dispset_Hi2;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																							EASY1WCMP2=set_Hi2; 
																						
															}break;
															case set_Hi2:{
																				
																			
																								runsetdisplay=dispset_Lo2;
																								DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																								EASY1WCMP2=set_Lo2;
																				
															}break;
															case set_Lo2:{
																			
																			
																									runsetdisplay=dispset_P1;
																									DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)1);
																									EASY1WCMP2=set_P1;
																				
															}break;
														}
									     NOWSETWICH = EASY1WCMP2;
									}break;
								}
				}break;
				case compare1_HSY:{
								switch(compsestatus2)
								{
									case compare2_EASY:{
														switch(HSY1EASY2)
														{
															case set_no:{            
														        
																							runsetdisplay=dispset_P2;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)2);
																							HSY1EASY2=set_P2; 
														    
														}break;
															case set_P2:{
																					
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							HSY1EASY2=set_Hi1; 
																					
															}break; 
															case set_Hi1:{
																						
																							runsetdisplay=set_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																							HSY1EASY2=set_Lo1; 
																			
																}break;
															case set_Lo1:{
																			
																							runsetdisplay=dispset_P2;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)2);
																							HSY1EASY2=set_P2; 
															}break;
														}	
															NOWSETWICH=HSY1EASY2;
									}break;
									case compare2_HSY:{
											switch(HSY1HSY2)
											{
												case set_no:{            
														        
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							HSY1HSY2=set_Hi1; 
														    
														}break;
												case set_Hi1:{
																			    runsetdisplay=dispset_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																							HSY1HSY2=set_Lo1; 
																							 
																				
												}break; 
												case set_Lo1:{
																		
																						runsetdisplay=dispset_Hi2;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																							HSY1HSY2=set_Hi2;	
																					
												}break;
												case set_Hi2:{
																			
																						runsetdisplay=dispset_Lo2;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																							HSY1HSY2=set_Lo2; 	
																				
												}break;
										
												case set_Lo2:{
																				
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							HSY1HSY2=set_Hi1; 
																					
												}break;	
											}																											
									  NOWSETWICH=HSY1HSY2;
									}break;
									case compare2_off:{
														switch(HSY1OFF2)
														{
															case set_Hi1:{
																				
																							runsetdisplay=dispset_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																							HSY1OFF2=set_Lo1; 
																					
															}break;
															case set_Lo1:{
																			
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							HSY1OFF2=set_Hi1; 
																					
															}break;
														}																														
									
									}break;
									case compare2_WCMP:{
													switch(HSY1WCMP2)
													{
															case set_no:{            
														        
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							HSY1WCMP2=set_Hi1; 
														    
														}break;
														case set_Hi1:{
																			
																								runsetdisplay=dispset_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																							HSY1WCMP2=set_Lo1;
																						
														}break;
														case set_Lo1:{
																			
																							runsetdisplay=dispset_Hi2;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																							HSY1WCMP2=set_Hi2; 
																						
														}break;
														
														case set_Hi2:{
																			
																							runsetdisplay=dispset_Lo2;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																							HSY1WCMP2=set_Lo2; 
																						
														}break;
														case set_Lo2:{
																		
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							HSY1WCMP2=set_Hi1; 
																						
														}break;
													}
													NOWSETWICH=HSY1WCMP2;
									}break;
								}
				}break;
				case compare1_WCMP:{
									switch(compsestatus2)
								{
									case compare2_EASY:{
													switch(WCMP1EASY2)
													{
															case set_no:{            
														        
																							runsetdisplay=dispset_P2;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)2);
																							WCMP1EASY2=set_P2; 
														    
														}break;
														case set_P2:{
																				
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							WCMP1EASY2=set_Hi1; 
																						
														}break;
														case set_Hi1:{
																					
																							runsetdisplay=dispset_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																							WCMP1EASY2=set_Lo1; 
																					
														
														}break;
														case set_Lo1:{
																						
																							runsetdisplay=dispset_P2;
																							DIV_Disp_Snprintf(SecondScreen," P-%d",(int8_t)2);
																							WCMP1EASY2=set_P2; 
														}break;
													}
													NOWSETWICH=WCMP1EASY2;
									}break;
									case compare2_HSY:{
													switch(WCMP1HSY2)
													{
														case set_no:{            
														        
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							WCMP1HSY2=set_Hi1;  
														    
														}break;
														case set_Hi1:{
																						 runsetdisplay=dispset_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo1-%d",(int8_t)1);
																							WCMP1HSY2=set_Lo1;
																							 
																					
														}break; 
														case set_Lo1:{
																				   runsetdisplay=dispset_Hi2;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																							WCMP1HSY2=set_Hi2;
																							 
																					
														}break;
														case set_Hi2:{
																						 runsetdisplay=dispset_Lo2;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)2);
																							WCMP1HSY2=set_Lo2;
														}break;
														case set_Lo2:{
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							WCMP1HSY2=set_Hi1; 
														}break;
													}
													NOWSETWICH=WCMP1HSY2;
									}break;
									case compare2_off:{
												switch(WCMP1OFF2)
												{
													case set_no:{            
														        
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							WCMP1OFF2=set_Hi1;  
														    
														}break;
													case set_Hi1:{
																						
																							runsetdisplay=dispset_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo-%d",(int8_t)1);
																							WCMP1OFF2=set_Lo1; 
																					
													}break;
													case set_Lo1:{
																					
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							WCMP1OFF2=set_Hi1; 
																					
													}break;
												}
												NOWSETWICH=WCMP1OFF2;
									}break;
									case compare2_WCMP:{
													switch(WCMP1WCMP2)
													{
														case set_no:{            
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							WCMP1WCMP2=set_Hi1; 
														}break;
														case set_Hi1:{
																						 runsetdisplay=dispset_Lo1;
																							DIV_Disp_Snprintf(SecondScreen,"Lo1-%d",(int8_t)2);
																							WCMP1WCMP2=set_Lo1;
																						 
																					
														}break;
														case set_Hi2:{
																					  runsetdisplay=dispset_Lo2;
																							DIV_Disp_Snprintf(SecondScreen,"Lo2-%d",(int8_t)2);
																							WCMP1WCMP2=set_Lo2; 
																							 
																					
														}break;
														case set_Lo1:{
																					  	runsetdisplay=dispset_Hi2;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)2);
																							WCMP1WCMP2=set_Hi2;
																							
																					
														}break;
														case set_Lo2:{
																			
																							runsetdisplay=dispset_Hi1;
																							DIV_Disp_Snprintf(SecondScreen,"H1-%d",(int8_t)1);
																							WCMP1WCMP2=set_Hi1; 
																					
														}break;
													}
									    NOWSETWICH=WCMP1WCMP2;
									}break;
								}
				}break;
	}
	
}


//====================================================================================
//=                                                                             					=
//=   清除显示当前设置run                                                             =
//=                                                                                  =
//====================================================================================
void CLear_NowSetSHow(void)
{
			if(disp_delay_flag)  //关闭显示周期
			{    
			   		switch(runsetdisplay)   //按下mode显示设置哪个口
								{
									case dispset_P1:{
                DIV_Disp_ByString(SecondScreen,"    ");
																runsetdisplay&=~(dispset_P1);

									}break;
									case dispset_P2:{

                DIV_Disp_ByString(SecondScreen,"    ");
																runsetdisplay &= ~(dispset_P2);


									}break;
									case dispset_Hi1:{
                DIV_Disp_ByString(SecondScreen,"    ");
																runsetdisplay &= ~(dispset_Hi1);

									}break;
									case dispset_Lo1:{
               DIV_Disp_ByString(SecondScreen,"    ");
																runsetdisplay &= ~(dispset_Lo1);

									}break;
									case dispset_Hi2:{
                DIV_Disp_ByString(SecondScreen,"    ");
																runsetdisplay &= ~(dispset_Hi2);

									}break;
									case dispset_Lo2:{
                DIV_Disp_ByString(SecondScreen,"    ");
																runsetdisplay &= ~(dispset_Lo2);

									}break;
								} 
								DIV_Disp_ClearPoint();
				disp_delay_flag=0;
			}	
			
}
//====================================================================================
//=                                                                             					=
//=    显示预设                                                    														=
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
						case noset_status:{
											 DIV_Disp_ByString(MainScreen,"NoRM");         //显示输出1的预设
												DIV_Disp_ByString(SecondScreen,"Mode");	
						}break;
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
								case pnoset_status:{
								    DIV_Disp_ByString(MainScreen," PRO");         //显示输出1的预设
												DIV_Disp_ByString(SecondScreen,"Mode");	
								}break;
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
//=                                                                             				 =
//=                                                                                  =
//====================================================================================
void APM_EVAL_Init(void)
{
    /* Init delay function */
				HAL_System_Init();   
				APM_EVAL_DelayInit();
				DIV_ReadValue_Init();
				HAL_TMR_Init();
				HAL_KEY_Init();    
				DIV_Pwm_Init();
	   Hal_Out_Init();
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
//=                                                                                	 =
//=                                                                                  =
//====================================================================================
void TMR2Isr(void)
{  	
			if (TMR_ReadIntFlag(TMR2, TMR_INT_FLAG_UPDATE) == SET)
			{
        TMR_ClearIntFlag(TMR2, TMR_INT_FLAG_UPDATE);
				    
				
								//按键记录时间戳====================================	
								if(key_management.Key_start)//如果开始计时
								{   
								    key_management.Key_tick++;//时间戳自增
									   switch(key_management.Key_State)
												{
													case KEY_STATE_PRESSED:{
			
														
																	if(key_management.Key_tick>1000)
																	{   
																				key_management.PreWichKeyDown=0;   //如果进入长按就清掉短按的，不然长按进了别的模式短按是多个按键记录就不触发了
																				key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=1;
																			 key_management.Key_State=KEY_STATE_LONGPRESS_2S_MODE;
																	}
													}break;
													case KEY_STATE_LONGPRESS_2S_MODE:{
														

													     if(key_management.Key_tick>3000)
																	{
																				key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV=1;
																				key_management.Key_State=KEY_STATE_LONGPRESS_4S_MODE;
																	}
													}break;
													case KEY_STATE_LONGPRESS_4S_MODE:{

													}break;
												}
									
									  if(key_management.WichKeyDown==0){    //如果按键都释放了
												   switch(key_management.Key_State)
															{
																case KEY_STATE_PRESSED:{ 
																							
																					key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV=1;
																								
																					disp_delay_tick=0;//清除一下显示的延时计数
																	
																					key_management.Key_State=KEY_STATE_PRESSED;
																}break;
																case KEY_STATE_LONGPRESS_2S_MODE:{
																	
																	    Zero_Key_release=1;//释放校零按键
																	
																	
																				key_management.Key_State=KEY_STATE_PRESSED;
																}break;
																case KEY_STATE_LONGPRESS_4S_MODE:{
																				key_management.Key_State=KEY_STATE_PRESSED;
																}break;
															}

												
												key_management.Key_start=0;      //关闭计时
												key_management.Key_tick=0;
												}
												
								}
									//显示runestshow延时==============================
         if(runsetdisplay)
									{
									   	disp_delay_tick++;
												if(disp_delay_tick>1000){    //到达1s
														
																			disp_delay_flag=1;             //关闭显示
												}
									} else if(disp_delay_flag==0&&runsetdisplay==0)    // 不用一直计数
									{
															    disp_delay_tick=0;
									}	

								//pwm时间戳==================================================
								PWM_THREERED.Pwm_Tick++;
								PWM_ONERED.Pwm_Tick++;  
				    PWM_DOUBGED.Pwm_Tick++;				
				    PWM_ONEGED.Pwm_Tick++;  
				
				
								//可查询延时时间戳=========================================
								if(Encheck_Delay_Start)
								{
								   Encheck_Delay_Tick++;
								} 
								//刷新显示============================================
								disp_main_parcount++;
								if(disp_main_parcount==10)
								{
										 disp_main_flag=1;
									   disp_main_parcount=0;
								}
								
								//峰谷3.峰值谷值闪烁0.5s取反===============================
								Peakvally_count++;
								if(Peakvally_count==500)
								{
									 Peakvally_count=0;
										Switch_peakvalley_flag=~Switch_peakvalley_flag;//取反闪烁0.5s
								}
								//反应时间延时=======================================
								if(reactiontimestart1){
											if(reactiontime_tickchannel1<reactiontimesetchannel1){reactiontime_tickchannel1++;}
								}else{reactiontime_tickchannel1=0;}
								
								if(reactiontimestart2){
											if(reactiontime_tickchannel2<reactiontimesetchannel1){reactiontime_tickchannel2++;}
								}else{reactiontime_tickchannel2=0;}
								
								if(reactiontimestart3){
							 			if(reactiontime_tickchannel3<reactiontimesetchannel2){reactiontime_tickchannel3++;}
								}else{reactiontime_tickchannel3=0;}  
								
								if(reactiontimestart4){
        			if(reactiontime_tickchannel4<reactiontimesetchannel2){reactiontime_tickchannel4++;}
								}else{reactiontime_tickchannel4=0;}


								
								//10ms刷新任务========================================
        static uint32_t tick =0; 
								tick++;
								if(tick>=10)
								{
									tick=0;
									DIV_10MS=1;	
								}	
								
			}	
}
//====================================================================================
//=                                                                             					=
//=   ADC中断                                                                             	 =
//=                                                                                  =
//====================================================================================
 void ADC_Isr()
{
    if (ADC_ReadIntFlag(ADC_INT_FLAG_CS) == SET)
    {
        ADC_ClearIntFlag(ADC_INT_FLAG_CS);
					
        
								
					   adc_temp =ADC_ReadConversionValue();
					   adc_sum -= adc_buffer[ad_index];          // 移除旧值
								adc_buffer[ad_index] = adc_temp;    // 存入新值
								adc_sum += adc_temp;                 // 累加新值
								ad_index = (ad_index + 1) % WINDOW_SIZE; // 循环缓冲区
					   adcData=(uint16_t)(adc_sum / WINDOW_SIZE);
					
					
					
					
//					    adc_temp =ADC_ReadConversionValue();
//								if(adc_temp>adcData&&adc_temp-adcData>3)
//								{
//											adc_sum += adc_temp;  
//											adc_count++;
//								}
//					   else if(adc_temp<adcData&&adcData-adc_temp>3)
//								{
//										 	adc_sum += adc_temp;  
//											adc_count++;
//								}
//								if(adc_count==10)
//								{
//										adc_count=0;
//										adcData= adc_sum/10;
//										adc_sum=0;
//								}
								
						
        VOL_value=(adcData * 3300)/4095;
    }
}
