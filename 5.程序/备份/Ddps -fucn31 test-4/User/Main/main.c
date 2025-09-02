 /* Includes */             
#include "superhead.h"

//枚举==================================================
//menu1 
DeviceStatus  manu1_devicestate = status_init;   //主状态
//menu2
NormsetStatus manu2_normsetstatus = noset_status;    //副设置状态
PromodsetStatus manu2_promodesetstatus = pnoset_status;     //副设置状态
//norset 
CompareStatus1 compsestatus1 =compare1_EASY;  //比较输出1是什么状态
CompareStatus2 compsestatus2 =compare2_EASY;  //比较输出2是什么状态
NONCStatus noncstatus = noncstatus_1o2o;
ReactionTime reactiontime = reaction_2ms;
MainScreenColor mainscreencolor = R_ON;
Unitconversion unitconversion = Unit_Kpr;
//proset
SecScreen secscreen = secscreen_standard;   		
DisSpeed dispeed = dispeed_250ms;
DifLevel diflevel = diflevel_level1;
ColorReaction colorreaction = colorreaction_out1;
Ecomode ecomode = eco_std;
FactoryRecover facrecover = facrecoff;
//主屏显示命令=========================================================
typedef enum{
			MainDisp_NoCmd=0,
   MainDisp_Show_Presets=1,
	  MainDisp_Pressure,
			MainDisp_Init,
}MainDisp_CMD;
uint8_t Main_Disp_CmdTran = MainDisp_NoCmd;
//副屏显示命令=========================================================
typedef enum{
			SecondDisp_NoCmd=0,
   SecondDisp_DispSetWich,
			SecondDisp_DispSetValue,
			SecondDisp_DispUP,
			SecondDisp_DispDOWN,
}SecondDisp_CMD;
uint8_t Second_Disp_CmdTran = SecondDisp_NoCmd;
//输出命令============================================================
typedef enum{
	  OutPutNoCmd=0,
   OutPut1_ON,
			OutPut2_ON,
			OutPut1_OFF,
			OutPut2_OFF,
}OutPut_CMD;
uint8_t OutPut_CmdTran = OutPutNoCmd;
//设置管理命令==================================================
typedef enum{
	  SetManage_NOCMD=0,
	  SetManage_ADD,
			SetManage_DOWN,
			SetManage_CHANGENEXT,
			SetManage_KEY2FAST,
			SetManage_KEY3FAST,
	  SetManage_KEYFASTSTOP,
	
}SetManage_CMD;

SetManage_CMD SetManage_CmdTran = SetManage_NOCMD;
//ADC======================================================
uint32_t adcData = 0;           //ad值
uint32_t VOL_value = 0;         //电压值，参考3.3v
uint32_t adc_temp=0;
#define WINDOW_SIZE 100
uint16_t adc_buffer[WINDOW_SIZE] = {0};
uint8_t ad_index = 0;
uint32_t adc_sum = 0; //adc累加
//数据处理==================================================
float Current_pressure_value=0;	
float unitchange_pressure_value =0.0f;
float k=0.0608, b=-113.08; 

float P1_Value 	= 0.0;
float P2_Value 	= 0.0;
float Hi1_Value = 0.0;
float Lo1_Value = 0.0;		
float Hi2_Value = 0.0;
float Lo2_Value = 0.0; 
//输出控制==================================================
#define NO 1     //常开
#define NC 0     //常闭
uint8_t OUT1_NONCstate = 0;   //常开常闭
uint8_t OUT2_NONCstate = 0;

uint8_t out1_state = 0;      //当前输出状态
uint8_t out2_state = 0;
#define OUT1_on   if(OUT1_NONCstate){HAL_OutPut_Set(OUT1,1);}\
	                 else{HAL_OutPut_Set(OUT1,0);}
																		
#define OUT1_off  if(OUT1_NONCstate){HAL_OutPut_Set(OUT1,0);}\
	                 else{HAL_OutPut_Set(OUT1,1);	}

#define OUT2_on   if(OUT2_NONCstate){HAL_OutPut_Set(OUT2,1);	}\
	                 else{HAL_OutPut_Set(OUT2,0);	}
																		
#define OUT2_off   if(OUT2_NONCstate){HAL_OutPut_Set(OUT2,0);}\
	                 else{HAL_OutPut_Set(OUT2,1);	}
																		
//反应时间延时
uint16_t reactiontimesetchannel1=2;//默认2ms    
uint16_t reactiontimesetchannel2=2;//默认2ms
																		
uint16_t reactiontime_tickchannel1 =0;//记录时间戳
uint8_t reactiontimestart1=0;
uint16_t reactiontime_tickchannel2 =0;//记录时间戳
uint8_t reactiontimestart2=0;																	
//应差状态=================================================

#define WCMP1 0
#define HSY1 1
#define EASY1 2

#define WCMP2 3
#define HSY2 4
#define EASY2 5
#define OFF2 6	

float diflevel_value=0.0f;		

//设定管理器===============================================
#define OUT1_MODE_COUNT 3
#define OUT2_MODE_COUNT 4

#define ADD 1
#define DOWN 0

#define longpress 1 
#define shortpress 0

  // 输出1的模式
typedef enum {
    OT1_EASY=0,
    OT1_HSY,
    OT1_WCMP,
} Output1Mode;
 
// 输出2的模式
typedef enum {
    OT2_EASY=0,
    OT2_HSY,
    OT2_WCMP,
    OT2_OFF,
} Output2Mode;

Output1Mode current_out1_mode = 0;      //当前模式
Output2Mode current_out2_mode = 0;

typedef enum { false = 0, true = 1 } bool;  // 枚举模拟布尔类型

bool param_editable[OUT1_MODE_COUNT][OUT2_MODE_COUNT][6] = {
    // Output1=Easy 时，不同 Output2 模式下的可修改参数
    [OT1_EASY] = {    //p1    p2    hi1    lo1   hi2    lo2
        [OT2_EASY] = {true, true, 	false, false, false, false},  	// 仅 p1, p2
        [OT2_HSY] = 	{true, false, false, false, true, 	true},    // p1, hi2, lo2
        [OT2_WCMP] = {true, false, false, false, true, 	true},   	// p1, hi2, lo2
        [OT2_OFF] = 	{true, false, false, false, false, false},  	//仅p1
    },
    // Output1=HSY 时，不同 Output2 模式下的可修改参数
    [OT1_HSY] = {
        [OT2_EASY] = {false, true, true, true, false, false}, 		// 仅hi1, hi2 lo1 lo2
        [OT2_HSY] = 	{false, false, true, true, true, true},   // 仅 hi1, hi2 lo1 lo2
        [OT2_WCMP] = {false, false, true, true, true, true},  	//仅 hi1, hi2 lo1 lo2
        [OT2_OFF] = 	{false, false, true, true, false,false},  // 仅hi1 lo1
    },
    // Output1=WCMP 时，不同 Output2 模式下的可修改参数
    [OT1_WCMP] = {
        [OT2_EASY] = {false, true, true, true, false, false},  		// 仅 lo1, lo2
        [OT2_HSY] = 	{false, false, true, true, true, true},   	// p1, lo1
        [OT2_WCMP] = {false, false, true, true, true, true},    // hi1, hi2, lo1, lo2
        [OT2_OFF] = 	{false, false, true, true, false,false},   // 仅hi1 lo1   
    },
};


#define set_no  0x00 //开机无设置
#define set_P1 	0x01
#define set_P2 	0x02
#define set_Hi1 0x04
#define set_Lo1 0x08
#define set_Hi2 0x10
#define set_Lo2 0x20

uint8_t NOWSETWICH = set_no;  //正在设置哪个
uint8_t NOWCANSETWICH = set_no; //可以设置那些
uint8_t NOWChoice[6]={0};  //可设置选项
uint8_t NOWChoicelength=0;
//参数配置===============================================
typedef struct { // 参数配置结构体
    float *value;        // 参数地址（如 &P1_Value）
    float max_value;     // 最大值（按单位）
    float *peer_value;   // 关联参数（如 Hi1 对应 Lo1，NULL 表示无）
    uint8_t peer_check;  // 限制规则：0=无限制, 1=必须小于关联参数, 2=必须大于关联参数
} ParamConfig;

// 参数配置表（顺序与 set_P1/set_Hi1 等宏定义一致）
const ParamConfig param_table[] = {
    {&P1_Value, 	1.05, NULL, 0},       // set_P1: 无限制
    {&P2_Value, 	1.05, NULL, 0},       // set_P2: 无限制
    {&Hi1_Value, 1.05, &Lo1_Value, 1}, // set_Hi1: 必须 > Lo1
    {&Lo1_Value, 1.05, &Hi1_Value, 2}, // set_Lo1: 必须 < Hi1
    {&Hi2_Value, 1.05, &Lo2_Value, 1}, // set_Hi2: 必须 > Lo2
    {&Lo2_Value, 1.05, &Hi2_Value, 2}, // set_Lo2: 必须 < Hi2
};

#define MIN_DECIMAL_PRECISION 0.0001f   // 最小小数精度（根据实际需求调整）

//副屏显示延时=========================================================
#define seconddisp_delayset 1000
uint16_t seconddisp_delay_tick =0;//记录时间戳
uint8_t seconddisp_delay_start=0;
//主屏显示延时======================================================
#define Maindisp_delayset 1000
uint16_t Maindisp_delay_tick =0;//记录时间戳
uint8_t Maindisp_delay_start=0;

//=========================================================
volatile uint16_t timer_10ms_execute=0;

//=========================================================

uint8_t IsLastDecimalZero(float value,float current_increment)      //小数是否清除
{
	   // 计算当前值对应的整数增量个数
    int steps = (int)(value / current_increment);
	
	   return steps %10==0;

	
	#if 0
     float abs_value = fabsf(value);
    float decimal_part;
    
    // 使用 modf 分离整数和小数部分，避免精度问题
    float int_part;
    decimal_part = modff(abs_value, &int_part);
 
    // 计算当前增量对应的小数位数
    int decimal_places = 0;
    if (current_increment == 0.001f)      decimal_places = 3;
    else if (current_increment == 0.01f)  decimal_places = 2;
    else if (current_increment == 0.1f)   decimal_places = 1;
    else if (current_increment == 1.0f)   decimal_places = 0;
    else return false; // 不支持的增量
 
    // 如果没有小数部分，直接返回 true
    if (decimal_places == 0) {
        return ((int)int_part%10) == 0;
    }
 
    // 动态计算放大倍数（10^decimal_places）
    int scale = 1;
    for (int i = 0; i < decimal_places; i++) {
        scale *= 10;
    }
 
    // 放大到整数并四舍五入，避免浮点误差
    int scaled_decimal = (int)roundf(decimal_part * scale);
 
    // 检查指定位数后的所有位是否为零
    // 例如：decimal_places=2（0.01增量）时，检查 scaled_decimal 是否为整数（即小数部分在百分位后全零）
    return (scaled_decimal * scale) == (int)(decimal_part * scale * scale / scale);
		#endif
}

void Set_NowSetPress(void)            //长按设置
{
 if (NOWSETWICH == set_no) return; // 未选择参数，直接返回
 
    static uint16_t addtime = 0;      // 按键持续时间计数器
    static uint8_t fast_mode = 0;     // 加速模式标志
    static uint8_t super_fast_mode = 0; // 超级加速模式标志
				static uint8_t upto10ms=0;
				static uint8_t addordown=0;
				static uint8_t addjiange=0;
    
    float max_val = 0; //参数设置上限 
    float increment = 0;//增量
    float base_increment = 0; // 基础增量（用于恢复）
 
    // 获取当前参数配置
    const ParamConfig *cfg = 0;
    switch(NOWSETWICH)       //选择要设定的值
    {
        case set_P1:{
            cfg = &param_table[0];
        }break;
        case set_P2:{
            cfg = &param_table[1];
        }break;
        case set_Hi1:{
            cfg = &param_table[2];
        }break;
        case set_Lo1:{
            cfg = &param_table[3];
        }break;
        case set_Hi2:{
            cfg = &param_table[4];
        }break;
        case set_Lo2:{
            cfg = &param_table[5];
        }break;
    }
    float *current_value = cfg->value;    //获取设置参数
    
    switch(unitconversion)     //选择不同单位的上限和增量
    {
        case Unit_Bar:{
            max_val = 1.05;
            increment = 0.001f;
        }break;
        case Unit_Inhg:{
            max_val = 31.0;
            increment = 0.01f;
        }break;
        case Unit_Kgf:{
            max_val = 105.0;
            increment = 0.1f;
        }break;
        case Unit_Kpr:{
            max_val = 105.0;
            increment = 0.1f;
        }break;
        case Unit_Mmhg:{
            max_val = 787.5;
            increment = 1.0f;
        }break;
        case Unit_Mpr:{
            max_val = 0.105;
            increment = 0.001f;
        }break;
        case Unit_Psi:{
            max_val = 15.22;
            increment = 0.01f;
        }break;
    }
    
    base_increment = increment; // 保存基础增量
    
    // 处理按键持续时间
    if (key_management.WichKeyDown != 0) { // 按键按下
					
					    if(key_management.WichKeyDown==KEY2)
									{
									   addordown =1;
									}
									else if(key_management.WichKeyDown==KEY3)
									{
									  addordown =0;
									}
					
					
        addtime++;
        
        // 1秒后进入快速模式（10倍速）
        if (addtime >= 100 && !fast_mode) { // 假设10ms调用一次，100次=1秒
            fast_mode = 1;
            increment *= 10; // 加速10倍
        }
        
        // 5秒后进入超级快速模式（100倍速）
        if (addtime >= 500 && !super_fast_mode && IsLastDecimalZero(*current_value,base_increment)) { // 500次=5秒
            super_fast_mode = 1;
            increment = base_increment * 100; // 加速100倍
            // 这里可以清零当前增加位（如果需要）
            // *current_value = ((int)(*current_value / base_increment)) * base_increment;
        }
 
        // 超级加速模式下，每隔一段时间跳到下一个增量位置
        if (super_fast_mode) {
            
                if(upto10ms<7){upto10ms++;}
																else{
                   upto10ms=0;
                // 计算当前值对应的整数增量个数
                int steps = (int)(*current_value / base_increment);
                
                if (addordown == 1) { // 增加
                    steps+=10;
                    *current_value = steps * base_increment;
                    // 确保不超过上限
                    if (*current_value > max_val) {
                        *current_value = max_val;
																								Second_Disp_CmdTran=SecondDisp_DispUP;
                    }
                } else { // 减少
                    steps-=10;
                    *current_value = steps * base_increment;
                    // 确保不低于下限
                    if (*current_value < -max_val) {
                        *current_value = -max_val;
																							Second_Disp_CmdTran=SecondDisp_DispDOWN;
                    }
                }
                
                // 检查关联参数限制
                if (cfg->peer_value != NULL) {
                    float peer_val = *cfg->peer_value;
                    if (cfg->peer_check == 1 && *current_value < peer_val) {
                        *current_value = peer_val;
                    } else if (cfg->peer_check == 2 && *current_value > peer_val) {
                        *current_value = peer_val;
                    }
                }
                
                
            }
												return; // 直接返回				
        }
								
							  if(fast_mode)
									{
									  float temp = *current_value;    //暂存
											
									if(addjiange<10){addjiange++;}   //设置递增间隔
									else
									{
										addjiange=0;
									  // 处理增减逻辑
										if (addordown) {  //如果增加
														if (temp < max_val) {
																		temp += increment; // 增加
														} else {
																		temp = max_val; //强制设为上限
															   Second_Disp_CmdTran=SecondDisp_DispUP;
														}
										} else {
														if (temp > -max_val) {
																		temp -= increment; // 减少
														} else {
																		temp = -max_val; // 强制设为下限
																		Second_Disp_CmdTran=SecondDisp_DispDOWN;
														}
										}  
									}    
										
										// 处理 Hi/Lo 相互限制
										if (cfg->peer_value != NULL) {
														float peer_val = *cfg->peer_value;
														if (cfg->peer_check == 1 && temp < peer_val) { // 必须大于关联参数
																		temp = peer_val;
														} else if (cfg->peer_check == 2 && temp > peer_val) { // 必须小于关联参数
																		temp = peer_val;
														}
										}
										
										// 更新参数值
										*current_value = temp;	
								
									}
    } else { // 按键释放
        addtime = 0;
        fast_mode = 0;
        super_fast_mode = 0;
        increment = base_increment; // 恢复基础增量
    }
}

void Set_NowSet(uint8_t addordown)  //短按设置参数
{
    if (NOWSETWICH == set_no) return; // 未选择参数，直接返回

		
				float max_val = 0; //参数设置上限 
	   float	increment = 0;//增量
	
    // 获取当前参数配置
    const ParamConfig *cfg = 0; // 假设 set_P1=0x01 对应索引 0
     switch(NOWSETWICH)       //选择要设定的值
				{
					case set_P1:{
					     cfg=&param_table[0];
					}break;
					case set_P2:{
					     cfg=&param_table[1];
					}break;
					case set_Hi1:{
					     cfg=&param_table[2];
					}break;
					case set_Lo1:{
					     cfg=&param_table[3];
					}break;
					case set_Hi2:{
					     cfg=&param_table[4];
					}break;
					case set_Lo2:{
					     cfg=&param_table[5];
					}break;
				}
	   float *current_value = cfg->value;	//获取设置参数
	  
	   switch(unitconversion)     //选择不同单位的上限和增量
				{
					case Unit_Bar:{
					     max_val=1.05;
										increment=0.001f;
					}break;
					case Unit_Inhg:{
					     max_val=31.0;
						    increment=0.01f;
					}break;
					case Unit_Kgf:{
									  max_val=105.0;
						    increment=0.1f;
					}break;
					case Unit_Kpr:{
						    max_val=105.0;
						    increment=0.1f;
					}break;
					case Unit_Mmhg:{
          max_val=787.5;
						    increment=1.0f;
					}break;
					case Unit_Mpr:{
          max_val=0.105;
						    increment=0.001f;
					}break;
					case Unit_Psi:{
						    max_val=15.22;
						    increment=0.01f;
					}break;
				}
	
				float temp = *current_value;    //暂存
				
					  // 处理增减逻辑
						if (addordown) {  //如果增加
										if (temp < max_val) {
														temp += increment; // 增加
										} else {
															temp = max_val;//强制设为上限
															Second_Disp_CmdTran=SecondDisp_DispUP;
															// 显示上限

										}
						} else {
										if (temp > -max_val) {
														temp -=	increment; // 减少
										} else {
														temp = -max_val; // 强制设为下限
														Second_Disp_CmdTran=SecondDisp_DispDOWN;
														// 显示下限
												
										}
						}		
						
 
    // 处理 Hi/Lo 相互限制
    if (cfg->peer_value != NULL) {
        float peer_val = *cfg->peer_value;
        if (cfg->peer_check == 1 && temp < peer_val) { // 必须大于关联参数
            temp = peer_val;
        } else if (cfg->peer_check == 2 && temp > peer_val) { // 必须小于关联参数
            temp = peer_val;
        }
    }
 
    // 更新参数值
    *current_value = temp;
}
void get_NowSet(void) 					//选择可修改值
{
		//选择模式
   if(compsestatus1==compare1_EASY){current_out1_mode=OT1_EASY;} 
			if(compsestatus1==compare1_WCMP){current_out1_mode=OT1_WCMP;}	
			if(compsestatus1==compare1_HSY)	{current_out1_mode=OT1_HSY;}			
			
			if(compsestatus2==compare2_EASY){current_out2_mode=OT2_EASY;} 
			if(compsestatus2==compare2_WCMP){current_out2_mode=OT2_WCMP;}	
			if(compsestatus2==compare2_HSY)	{current_out2_mode=OT2_HSY;}
			if(compsestatus2==compare2_off)	{current_out2_mode=OT2_OFF;}
			
			NOWCANSETWICH=0;
			NOWChoicelength = 0;
			
			int i=0; 
			const uint8_t param_masks[6] = {set_P1, set_P2, set_Hi1, set_Lo1, set_Hi2, set_Lo2};
			for(i=0;i<6;i++)
			{
								if(param_editable[current_out1_mode][current_out2_mode][i])
								{
										  NOWCANSETWICH |= param_masks[i];          // 更新位掩码
            NOWChoice[NOWChoicelength++] = param_masks[i]; // 存储宏定义值
								}	
			}
}
void SetManage(uint8_t cmd)//设置管理
{
	
	 get_NowSet(); //获取当前可设定
	
	 Set_NowSetPress();//长按设置
	
		switch(cmd)
		{
			case SetManage_NOCMD:{
			
			}break;
			case SetManage_ADD:{
			
				     Set_NowSet(ADD);
				
				     SetManage_CmdTran=SetManage_NOCMD;
			}break;
			case SetManage_DOWN:{
				
			     Set_NowSet(DOWN);
				     
				SetManage_CmdTran=SetManage_NOCMD;
			}break;
			case SetManage_CHANGENEXT:{
			       static uint8_t choice;
										choice++;
				      choice%=NOWChoicelength;
										NOWSETWICH=NOWChoice[choice];
				      SetManage_CmdTran=SetManage_NOCMD;
			}break;
		}
}
void Yingcha_Comp(void)   				//应差比较
{
	 uint8_t ifout1=0;
	 uint8_t ifout2=0;
	
	  switch(diflevel)     //应差选择
					{
						case diflevel_level1:{
							    switch(unitconversion)
											{
												case Unit_Bar	:{diflevel_value=0.001;}break;
												case Unit_Inhg:{diflevel_value=0.1;}break;
												case Unit_Kgf	:{diflevel_value=0.001;}break;
												case Unit_Kpr	:{diflevel_value=0.1;}break;
												case Unit_Mmhg:{diflevel_value=1.0;}break;
												case Unit_Mpr	:{diflevel_value=0.001;}break;
												case Unit_Psi	:{diflevel_value=0.01;}break;
											}    
						}break;
						case diflevel_level2:{
							    switch(unitconversion)
											{
												case Unit_Bar	:{diflevel_value=0.002;}break;
												case Unit_Inhg:{diflevel_value=0.2;}break;
												case Unit_Kgf	:{diflevel_value=0.002;}break;
												case Unit_Kpr	:{diflevel_value=0.2;}break;
												case Unit_Mmhg:{diflevel_value=2.0;}break;
												case Unit_Mpr	:{diflevel_value=0.002;}break;
												case Unit_Psi	:{diflevel_value=0.02;}break;
											}
							
						     
						
						}break;
						case diflevel_level3:{
						      switch(unitconversion)
											{
												case Unit_Bar	:{diflevel_value=0.003;}break;
												case Unit_Inhg:{diflevel_value=0.3;}break;
												case Unit_Kgf	:{diflevel_value=0.003;}break;
												case Unit_Kpr	:{diflevel_value=0.3;}break;
												case Unit_Mmhg:{diflevel_value=3.0;}break;
												case Unit_Mpr	:{diflevel_value=0.003;}break;
												case Unit_Psi	:{diflevel_value=0.03;}break;
											}
						
						
						}break;
						case diflevel_level4:{
						      switch(unitconversion)
											{
												case Unit_Bar	:{diflevel_value=0.004;}break;
												case Unit_Inhg:{diflevel_value=0.4;}break;
												case Unit_Kgf	:{diflevel_value=0.004;}break;
												case Unit_Kpr	:{diflevel_value=0.4;}break;
												case Unit_Mmhg:{diflevel_value=4.0;}break;
												case Unit_Mpr	:{diflevel_value=0.004;}break;
												case Unit_Psi	:{diflevel_value=0.04;}break;
											}
						
						}break;
						case diflevel_level5:{
						    switch(unitconversion)
											{
												case Unit_Bar	:{diflevel_value=0.005;}break;
												case Unit_Inhg:{diflevel_value=0.5;}break;
												case Unit_Kgf	:{diflevel_value=0.005;}break;
												case Unit_Kpr	:{diflevel_value=0.5;}break;
												case Unit_Mmhg:{diflevel_value=5.0;}break;
												case Unit_Mpr	:{diflevel_value=0.005;}break;
												case Unit_Psi	:{diflevel_value=0.05;}break;
											}
						
						}break;
						case diflevel_level6:{
						    switch(unitconversion)
											{
												case Unit_Bar	:{diflevel_value=0.006;}break;
												case Unit_Inhg:{diflevel_value=0.6;}break;
												case Unit_Kgf	:{diflevel_value=0.006;}break;
												case Unit_Kpr	:{diflevel_value=0.6;}break;
												case Unit_Mmhg:{diflevel_value=6.0;}break;
												case Unit_Mpr	:{diflevel_value=0.006;}break;
												case Unit_Psi	:{diflevel_value=0.06;}break;
											}
						
						
						}break;
						case diflevel_level7:{
						    switch(unitconversion)
											{
												case Unit_Bar	:{diflevel_value=0.007;}break;
												case Unit_Inhg:{diflevel_value=0.7;}break;
												case Unit_Kgf	:{diflevel_value=0.007;}break;
												case Unit_Kpr	:{diflevel_value=0.7;}break;
												case Unit_Mmhg:{diflevel_value=7.0;}break;
												case Unit_Mpr	:{diflevel_value=0.007;}break;
												case Unit_Psi	:{diflevel_value=0.07;}break;
											}
						
						}break;
						case diflevel_level8:{
						    switch(unitconversion)
											{
												case Unit_Bar	:{diflevel_value=0.008;}break;
												case Unit_Inhg:{diflevel_value=0.8;}break;
												case Unit_Kgf	:{diflevel_value=0.008;}break;
												case Unit_Kpr	:{diflevel_value=0.8;}break;
												case Unit_Mmhg:{diflevel_value=8.0;}break;
												case Unit_Mpr	:{diflevel_value=0.008;}break;
												case Unit_Psi	:{diflevel_value=0.08;}break;
											}
						
						}break;
					}
	
		 	switch(compsestatus1)   //选择应差的模式
		{
					case compare1_EASY:{
						    ifout1=EASY1;
									switch(compsestatus2)
									{
										case compare2_EASY:{																												
												    ifout2=EASY2;
										}break;
										case compare2_HSY:{
											     ifout2=HSY2;
										}break;
										case compare2_WCMP:{
																ifout2=WCMP2;
										}break;
										case compare2_off:{
										      ifout2=OFF2;
										}break;
									}
					}break;
					case compare1_HSY:{
																ifout1=HSY1;
									switch(compsestatus2)
									{
										case compare2_EASY:{
														  ifout2=EASY2;
										}break;
										case compare2_HSY:{
											    	ifout2=HSY2; 
										}break;
										case compare2_off:{
															 ifout2=OFF2;
										}break;
										case compare2_WCMP:{
													   ifout2=WCMP2;
										}break;
									}
					}break;
					case compare1_WCMP:{
											ifout1=WCMP1;
										switch(compsestatus2)
									{
										case compare2_EASY:{
													 ifout2=EASY2;	  
										}break;
										case compare2_HSY:{
													 ifout2=HSY2; 	
										}break;
										case compare2_off:{
								      ifout2=OFF2;
										}break;
										case compare2_WCMP:{
												   ifout2=WCMP2; 
										}break;
									}
					}break;
		}

		
		switch(ifout1)
		{
			case EASY1:{
			  switch(out1_state)
						{
							case 0:{
							      if(unitchange_pressure_value>(P1_Value+diflevel_value))//大于应差才输出
													{
														  OutPut_CmdTran=OutPut1_ON;//延时输出1命令
													}
							}break;
							case 1:{   
												if(unitchange_pressure_value<(P1_Value))
												{
																OutPut_CmdTran=OutPut1_OFF;//延时输出1命令
												}
							}break;
						}
			}break;
			case HSY1:{
			    switch(out1_state)
									{
										case 0:{
													if(unitchange_pressure_value>(Hi1_Value))
														{
																OutPut_CmdTran=OutPut1_ON;//延时输出1命令;
														}
										}break;
										case 1:{
											  	if(unitchange_pressure_value<(Lo1_Value))
														{
																OutPut_CmdTran=OutPut1_OFF;//延时输出1命令
														}
										}break;
									} 
			}break;
			case WCMP1:{
			      switch(out1_state)
								{
									case 0:{
									   if(unitchange_pressure_value<(Hi1_Value+diflevel_value)&&unitchange_pressure_value>(Lo1_Value+diflevel_value))
												{
														OutPut_CmdTran=OutPut1_ON;//延时输出1命令;
												}
											
									}break;
									case 1:{
												 if(unitchange_pressure_value>(Hi1_Value+diflevel_value))
												{
														OutPut_CmdTran=OutPut1_OFF;//延时输出1命令
												}
												else if(unitchange_pressure_value<(Lo1_Value))
												{
														OutPut_CmdTran=OutPut1_OFF;//延时输出1命令
												}
									}break;
								}
			}break;
		}
			
		switch(ifout2)
		{
			case EASY2:{
			     switch(out2_state)
								{
									case 0:{
															if(unitchange_pressure_value>(P2_Value+diflevel_value))//大于应差才输出
															{
																		OutPut_CmdTran=OutPut2_ON;//延时输出2命令
															}
									}break;
									case 1:{   
												if(unitchange_pressure_value<(P2_Value))
												{
																OutPut_CmdTran=OutPut2_OFF;//延时输出2命令
												}
									}break;
								}
			}break;
			case HSY2:{
			     switch(out2_state)
									{
										case 0:{
													if(unitchange_pressure_value>(Hi2_Value))
														{
																OutPut_CmdTran=OutPut2_ON;//延时输出2命令
														}
										}break;
										case 1:{
											  	if(unitchange_pressure_value<(Lo2_Value))
														{
															OutPut_CmdTran=OutPut2_OFF;//延时输出2命令
														}
										}break;
									}  
			}break;
			case WCMP2:{
			     switch(out2_state)
								{
									case 0:{
									    if(unitchange_pressure_value<(Hi2_Value+diflevel_value)&&unitchange_pressure_value>(Lo2_Value+diflevel_value))
												{
														OutPut_CmdTran=OutPut2_ON;//延时输出2命令
												}
											
									}break;
									case 1:{
												 if(unitchange_pressure_value>(Hi2_Value+diflevel_value))
												{
														OutPut_CmdTran=OutPut2_OFF;//延时输出2命令
												}
												else if(unitchange_pressure_value<(Lo2_Value))
												{
														OutPut_CmdTran=OutPut2_OFF;//延时输出2命令
												}
									}break;
								}
			}break;
			case OFF2:{
			
			}break;
		}	
}	
																		
void OutPut_Ctr(uint8_t * cmd)//输出控制管理
{
	 Yingcha_Comp();
	
	 switch(noncstatus)    //选择输出按常开还是常闭
		{
			case noncstatus_1c2c:{OUT1_NONCstate=NC;OUT2_NONCstate =NC;}break;
			case noncstatus_1c2o:{OUT1_NONCstate=NC;OUT2_NONCstate =NO;}break;
			case noncstatus_1o2c:{OUT1_NONCstate=NO;OUT2_NONCstate =NC;}break;
			case noncstatus_1o2o:{OUT1_NONCstate=NO;OUT2_NONCstate =NO;}break;
		}

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
	
		switch(* cmd)
		{
			case OutPutNoCmd:{
			
			}break;
			case OutPut1_ON:{
			  reactiontimestart1=1;
					if(reactiontime_tickchannel1>=reactiontimesetchannel1)
						{ 	
								reactiontimestart1=0;
								OUT1_on;    //自动根据常开常闭设置
							 out1_state =1;
							 *cmd=OutPutNoCmd;
						}
						
			}break;
			case OutPut1_OFF:{
			   reactiontimestart1=1;
					if(reactiontime_tickchannel1>=reactiontimesetchannel1)
						{ 	
								reactiontimestart1=0;
								OUT1_off;    //自动根据常开常闭设置
								out1_state =0;
							 *cmd=OutPutNoCmd;
						}
			}break;
		 case OutPut2_ON:{
			    reactiontimestart2=1;
					if(reactiontime_tickchannel2>=reactiontimesetchannel2)
						{ 	
								reactiontimestart2=0;
								OUT2_on;    //自动根据常开常闭设置
								out2_state =1;
							 *cmd=OutPutNoCmd;
						}
			}break;
			case OutPut2_OFF:{
			    reactiontimestart2=1;
					if(reactiontime_tickchannel2>=reactiontimesetchannel2)
						{ 	
								reactiontimestart2=0;
								OUT2_off;    //自动根据常开常闭设置
							 	out2_state =0;
							 *cmd=OutPutNoCmd;
						}
			}break;
		}
		
}





void Data_Processing(void)   	//数据处理管理
{
   //读取气压值
			Current_pressure_value=(float)adcData*k+b;
	
	  //不同单位换算 
	  unitchange_pressure_value=Current_pressure_value;
			switch(unitconversion)
			{
				case Unit_Bar:{
									unitchange_pressure_value*=0.01f;
										
				}break;
				case Unit_Inhg:{
									unitchange_pressure_value*=0.2953;
									
				}break;
				case Unit_Kgf:{
									unitchange_pressure_value*=0.02089;
											
				}break;
				case Unit_Kpr:{
						
				}break;
				case Unit_Mmhg:{
										unitchange_pressure_value*=7.5;
							
				}break;
				case Unit_Mpr:{
										unitchange_pressure_value*=0.001;
								
				}break;
				case Unit_Psi:{
										unitchange_pressure_value*=0.145;
				}break;
			}			

			
	
	
}
void MainDisp(uint8_t cmd)    //主屏显示
{
  switch(cmd)  //选择对应显示命令
		{
			case MainDisp_NoCmd:{
				
				//无命令清空
				
			}break;
			case MainDisp_Show_Presets:{
			   DIV_Disp_ClearAllPoint(MainScreen);//小数点清除
						switch(manu1_devicestate)  //显示对应设定的预设
						{     //一级菜单
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
								}break;
								default:{}break;
						}
						Main_Disp_CmdTran=MainDisp_NoCmd;	   //取消命令
			}break;
			case MainDisp_Pressure:{
			
			 // 显示气压值
					switch(unitconversion)
				{
					case Unit_Bar:{
										DIV_Disp_MultiplefloatNum(MainScreen,unitchange_pressure_value,3);	 	
					}break;
					case Unit_Inhg:{
										DIV_Disp_MultiplefloatNum(MainScreen,unitchange_pressure_value,1);	 
					}break;
					case Unit_Kgf:{
										DIV_Disp_MultiplefloatNum(MainScreen,unitchange_pressure_value,3);	  
					}break;
					case Unit_Kpr:{
										DIV_Disp_MultiplefloatNum(MainScreen,unitchange_pressure_value,1);	  //显示一位小数 kpa
					}break;
					case Unit_Mmhg:{
										DIV_Disp_MultiplefloatNum(MainScreen,unitchange_pressure_value,0);
					}break;
					case Unit_Mpr:{
										DIV_Disp_MultiplefloatNum(MainScreen,unitchange_pressure_value,3);
					}break;
					case Unit_Psi:{
										DIV_Disp_MultiplefloatNum(MainScreen,unitchange_pressure_value,2);
					}break;
				}
			 Main_Disp_CmdTran=MainDisp_NoCmd;	   //取消命令
			}break;
			case MainDisp_Init:{
			 //显示RH01
				DIV_Disp_Snprintf(MainScreen,"RL01");  //显示款型
				DIV_Disp_Snprintf(SecondScreen,"_NPN");//显示npn款
			 Main_Disp_CmdTran=MainDisp_NoCmd;	   //取消命令
			}break;
			default:break;
		}


}


void UnitDisp(uint8_t screen,float value) //显示数据按照单位要求
{
		  switch(unitconversion)
				{
					case Unit_Bar:{
					    
										DIV_Disp_MultiplefloatNum(screen,value,3);	 	
					}break;
					case Unit_Inhg:{
					     
										DIV_Disp_MultiplefloatNum(screen,value,1);	 
					}break;
					case Unit_Kgf:{
					     
										DIV_Disp_MultiplefloatNum(screen,value,3);	  
					}break;
					case Unit_Kpr:{
										DIV_Disp_MultiplefloatNum(screen,value,1);	  //显示一位小数 kpa
					}break;
					case Unit_Mmhg:{
					     
										DIV_Disp_MultiplefloatNum(screen,value,0);
					}break;
					case Unit_Mpr:{
					      
										DIV_Disp_MultiplefloatNum(screen,value,3);
					}break;
					case Unit_Psi:{
					     
										DIV_Disp_MultiplefloatNum(screen,value,2);
					}break;
				}
}
void SecondDisp(uint8_t cmd)  //副屏显示
{
	if(!cmd)//如果没命令才正常显示，优先显示命令要求的
	{
		DIV_Disp_ClearAllPoint(SecondScreen);
	  switch(secscreen)     //正常显示
		{
			case secscreen_discustom:{//显示单位
				
			
			
			}break;
			case secscreen_disNOxx:{//显示编号
				
			
			
			}break;
			case secscreen_disunit:{  //显示单位
			 switch(unitconversion)
				{
					case Unit_Bar:{DIV_Disp_Snprintf(SecondScreen," bAr");}break;
					case Unit_Inhg:{DIV_Disp_Snprintf(SecondScreen,"inHG");}break;
					case Unit_Kgf:{DIV_Disp_Snprintf(SecondScreen," KgF");}break;
					case Unit_Kpr:{DIV_Disp_Snprintf(SecondScreen," KPA");}break;
					case Unit_Mmhg:{DIV_Disp_Snprintf(SecondScreen,"MMHG");}break;
					case Unit_Mpr:{DIV_Disp_Snprintf(SecondScreen," MPA");}break;
					case Unit_Psi:{DIV_Disp_Snprintf(SecondScreen," PSI");}break;
				}
			
			}break;
			case secscreen_off:{//run模式下5s后关闭副屏
				
			}break;
			case secscreen_standard:{ //显示设定值
				
		   switch(NOWSETWICH)     //恢复副屏显示
				{
				case set_P1:{
											UnitDisp(SecondScreen,P1_Value);	
					}break;
					case set_P2:{
											UnitDisp(SecondScreen,P2_Value);	
					}break;
					case set_Hi1:{
												UnitDisp(SecondScreen,Hi1_Value);	
					}break;
					case set_Lo1:{
												UnitDisp(SecondScreen,Lo1_Value);	
					}break;
					case set_Hi2:{
												UnitDisp(SecondScreen,Hi2_Value);	
					}break;
					case set_Lo2:{
												UnitDisp(SecondScreen,Lo2_Value);	
					}break;
			}
			
			}break;
		}
	}
  
		
		switch(cmd)  //按下后会关闭的
		{
			case SecondDisp_NoCmd:{
			
			
			}break;
			case SecondDisp_DispSetWich:{
				
										DIV_Disp_ClearAllPoint(SecondScreen);
				      switch(NOWSETWICH)      //显示当前设定值
										{
											case set_P1:{	DIV_Disp_Snprintf(SecondScreen," P-1");}break;
											case set_P2:{	DIV_Disp_Snprintf(SecondScreen," P-2");}break;
											case set_Hi1:{	DIV_Disp_Snprintf(SecondScreen,"Hi-1");}break;
											case set_Lo1:{	DIV_Disp_Snprintf(SecondScreen,"Lo-1");}break;
											case set_Hi2:{	DIV_Disp_Snprintf(SecondScreen,"Hi-2");}break;
											case set_Lo2:{	DIV_Disp_Snprintf(SecondScreen,"Lo-2");}break;
										}  
							
										seconddisp_delay_start=1;
										if(seconddisp_delay_tick>=seconddisp_delayset)//1.5s后关闭
										{ 	
												seconddisp_delay_start=0;
											 Second_Disp_CmdTran=SecondDisp_NoCmd;
										}
			}break;
			case SecondDisp_DispSetValue:{
			
			
			}break;
			case SecondDisp_DispUP:{
				      	DIV_Disp_ClearAllPoint(SecondScreen);
				      DIV_Disp_Snprintf(SecondScreen,"  UP");
			      	seconddisp_delay_start=1;
										if(seconddisp_delay_tick>=seconddisp_delayset)//1.5s后关闭
										{ 	
												seconddisp_delay_start=0;
											 Second_Disp_CmdTran=SecondDisp_NoCmd;
										}
			}break;
			case SecondDisp_DispDOWN:{
				       	DIV_Disp_ClearAllPoint(SecondScreen);
			        DIV_Disp_Snprintf(SecondScreen,"DOWN");
			      	seconddisp_delay_start=1;
										if(seconddisp_delay_tick>=seconddisp_delayset)//1.5s后关闭
										{ 	
												seconddisp_delay_start=0;
											 Second_Disp_CmdTran=SecondDisp_NoCmd;
										}
			}break;
		}
}

void RunState_Keyprocess(void)
{
	 //按键===========================================================================
		//短按设置
			if(	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV)
			{
				
							if(key_management.PreWichKeyDown==KEY1)    //按下模式按键
							{
								   SetManage_CmdTran=SetManage_CHANGENEXT;//改变选项
											Second_Disp_CmdTran=SecondDisp_DispSetWich;//副屏显示要设定的选项	
							}										
							if(key_management.PreWichKeyDown==KEY2)
							{
												SetManage_CmdTran=SetManage_ADD;//增加
							}
							if(key_management.PreWichKeyDown==KEY3)
							{
								    SetManage_CmdTran=SetManage_DOWN;//增加
							}
							key_management.PreWichKeyDown=0x00;	
							key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV=0;	
			}
		//长按事件========================================================
				if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV)
				{	
								if(key_management.WichKeyDown==KEY1)      //在run模式下长按mode键进入模式设置
								{ 	
									   manu1_devicestate=status_normset;//进入设置模式								
												manu2_normsetstatus=noset_status;	
												Main_Disp_CmdTran=MainDisp_Show_Presets;	   //显示预设
								}
								if(key_management.WichKeyDown==KEY2)      
								{ 	
									  	
								}
								if(key_management.WichKeyDown==KEY3)     
								{ 	
									  
								}
							key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=0;		
				}	
				if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV)         //防止退出模式异常
				{	
											key_management.PreWichKeyDown=0x00;
									key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV=0;		
				}
				
	 
}



void Normset_Keyprocess(void)
{
  //触发短按事件
	if(	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV)
	{
					if(key_management.PreWichKeyDown==KEY1)  	//短按mode键
					{
									if(manu2_normsetstatus==unitconversion_status){   //如果超出了设定菜单就退回运行模式
											manu1_devicestate=status_run;      						//回到运行模式
											manu2_normsetstatus=compareset1_status;  //重置设定模式的状态
									}
									else{manu2_normsetstatus++;}
									Main_Disp_CmdTran=MainDisp_Show_Presets;	   //显示预设
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
										Main_Disp_CmdTran=MainDisp_Show_Presets;	   //显示预设
					}
					if(key_management.PreWichKeyDown==KEY3)
					{
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
									Main_Disp_CmdTran=MainDisp_Show_Presets;	   //显示预设
					}
					key_management.PreWichKeyDown=0x00;	
						key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV=0;
		}
			//长按事件=====================
		if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV)
		{
						if(key_management.WichKeyDown==KEY1)     //退出设置模式
						{      
						
						} 		
						key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=0;
		}
			if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV)
		{
						if(key_management.WichKeyDown==KEY1)
						{      
						
						} 		
						key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV=0;
		}
}


void Poweron_initialization(void)
{
	    Main_Disp_CmdTran=MainDisp_Init;	   //显示初始化界面
	
	
					APM_EVAL_DelayMs(1000);
					manu1_devicestate= status_run;
}

void mainTask(void)
{
		 switch(manu1_devicestate)
			{
				case status_init:{
				
				 Poweron_initialization();//初始化
				
				}break;
				case status_run:{
					
					Main_Disp_CmdTran=MainDisp_Pressure;//显示气压 
			
					SetManage(SetManage_CmdTran);//设定管理
					
					OutPut_Ctr(&OutPut_CmdTran); //输出控制
					
					RunState_Keyprocess();//主模式按键处理
				}break;
					case status_normset:{
						
						
						
						
						Normset_Keyprocess();//设定模式按键处理
					}break;
					case status_proset:{
									

					}break;
					default:break;
			}
}




int main(void)
{
	 HAL_System_Init();
  APM_EVAL_DelayInit();
		HAL_TMR_Init();
	 DIV_KEY_Init();
		DIV_ReadValue_Init();  
		Hal_Out_Init();
		DIV_Disp_Init();

	while (1)             
	{		
		   if(timer_10ms_execute)
					{
						timer_10ms_execute=0;
						 mainTask();  					//主任务执行  
							Data_Processing();//数据处理
						 MainDisp(Main_Disp_CmdTran);	//主屏幕显示
						 SecondDisp(Second_Disp_CmdTran);//副屏显示
							DIV_disp_out12andpoint(); //刷新小数点显示
					}
	}
}




void timer_10ms_execute_function(void)
{
		 static uint32_t tick =0; 
			tick++;
			if(tick>=10)
			{
				tick=0;
				timer_10ms_execute=1;	
			}	
}

void timer_reaction_excute_function(void)
{
			if(reactiontimestart1){
							if(reactiontime_tickchannel1<reactiontimesetchannel1){reactiontime_tickchannel1++;}
				}else{reactiontime_tickchannel1=0;}
				
				if(reactiontimestart2){
							if(reactiontime_tickchannel2<reactiontimesetchannel1){reactiontime_tickchannel2++;}
				}else{reactiontime_tickchannel2=0;}
}
void timer_maindisp_function(void)
{
    if(Maindisp_delay_start){
							if(Maindisp_delay_tick<Maindisp_delayset){Maindisp_delay_tick++;}
				}else{Maindisp_delay_tick=0;}
}

void timer_seconddisp_function(void)
{
	 	if(seconddisp_delay_start){
							if(seconddisp_delay_tick<seconddisp_delayset){seconddisp_delay_tick++;}
				}else{seconddisp_delay_tick=0;}
}


//timer
void TMR2Isr(void)
{
	  if (TMR_ReadIntFlag(TMR2, TMR_INT_FLAG_UPDATE) == SET)
			{
        TMR_ClearIntFlag(TMR2, TMR_INT_FLAG_UPDATE);
	       timer_maindisp_function();//主屏显示延时
								timer_10ms_execute_function();//刷新10ms执行
	       timer_reaction_excute_function();//反应时间延迟
				    timer_seconddisp_function();//副屏显示延时
	       DIV_key_read();	
			}
}

//adcisr
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

        VOL_value=(adcData * 3300)/4095;
    }
}
