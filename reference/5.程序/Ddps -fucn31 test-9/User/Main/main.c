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
Ecomode ecomode = eco_FULL;
FactoryRecover facrecover = facrecoff;

//runmode
RunMode runmode = Rstate_Run_mode;       //运行状态

//prosetcustom
ProSetCustom prosetcustom = Prosetnth_state;

//数据处理命令=========================================================
typedef enum{
  DataProcess_NoCmd=0,
		DataProcess_Zeroing,

}DataProcess_CMD;

uint8_t DataProcess_CmdTran= DataProcess_NoCmd;
//主屏显示命令=========================================================
typedef enum{
			MainDisp_NoCmd=0,
   MainDisp_Show_Presets=1,
	  MainDisp_Pressure,
			MainDisp_Init,
			MainDisp_Zeromode,
	  MainDisp_Peakvalleymode, 
	  MainDisp_PeakPressure,
			MainDisp_KeyLockmode,
	  MainDisp_KeyLockdelay,
	  MainDisp_Clear,
	  MainDisp_ClearAfterdispress,
}MainDisp_CMD;
uint8_t Main_Disp_CmdTran = MainDisp_NoCmd;
//副屏显示命令=========================================================
typedef enum{
			SecondDisp_NoCmd=0,
   SecondDisp_DispSetWich,
			SecondDisp_DispSetValue,
			SecondDisp_DispUP,
			SecondDisp_DispDOWN,
	  SecondDisp_Peakvalleymode,
	  SecondDisp_valleypressure,
	  SecondDisp_KeyLockon,
			SecondDisp_KeyLockoff,
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
uint8_t OutPut_CmdTran2 = OutPutNoCmd;
uint8_t OutPut_CmdTran1 = OutPutNoCmd;

uint8_t OutPutUpdata =0 ;
uint8_t OutPutUpdata2 =0 ;
//小屏显示命令=================================================
typedef enum {
	MiniScreen_NoCMD=0,
	MiniScreen_OFF,
	MiniScreen_ON,


}MiniScreen_CMD;
MiniScreen_CMD MiniScreen_TranCmd = MiniScreen_NoCMD;
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
#define EPSILON 1e-6f   //误差容许
float Current_pressure_value=0;	
float unitchange_pressure_value =0.0f;

float P1_Value 	= 0.0;
float P2_Value 	= 0.0;
float Hi1_Value = 0.0;
float Lo1_Value = 0.0;		
float Hi2_Value = 0.0;
float Lo2_Value = 0.0; 
//输出控制==================================================
#define NO 0     //常开
#define NC 1     //常闭
uint8_t OUT1_NONCstate = 0;   //常开常闭
uint8_t OUT2_NONCstate = 0;

uint8_t out1_state = 0;      //当前输出状态
uint8_t out2_state = 0;

uint8_t out1_statedisp = 0;      //当前输出状态
uint8_t out2_statedisp = 0;

#define OUT1_on   if(OUT1_NONCstate){HAL_OutPut_Set(OUT1,1);out1_statedisp=1;}\
	                 else{HAL_OutPut_Set(OUT1,0);out1_statedisp=0;}
																		
#define OUT1_off  if(OUT1_NONCstate){HAL_OutPut_Set(OUT1,0);out1_statedisp=0;}\
	                 else{HAL_OutPut_Set(OUT1,1);	out1_statedisp=1;}

#define OUT2_on   if(OUT2_NONCstate){HAL_OutPut_Set(OUT2,1);out2_statedisp=1;	}\
	                 else{HAL_OutPut_Set(OUT2,0);out2_statedisp=0;	}
																		
#define OUT2_off   if(OUT2_NONCstate){HAL_OutPut_Set(OUT2,0);out2_statedisp=0;}\
	                 else{HAL_OutPut_Set(OUT2,1);	out2_statedisp=1;}
																		
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

uint8_t offout =1;

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
//数据获取延时======================================================
#define Dataprocess_delayset 1000
uint16_t Dataprocess_delay_tick =0;//记录时间戳
uint8_t Dataprocess_delay_start=0;
//ECO延时======================================================
#define ECO_delayset 5000
volatile uint16_t ECO_delay_tick =0;//记录时间戳
volatile uint8_t ECO_delay_start=1;
volatile uint8_t ECO_delay_flag=0;
//通用延时======================================================
#define delayset 1000
volatile uint16_t delay_tick =0;//记录时间戳
volatile uint8_t delay_start=0;
//=========================================================
volatile uint16_t timer_10ms_execute=0;
uint8_t finish_zeroing =0;
//峰值谷值=============================================
float MINarepress = 0;
float MAXairpress = 0;
//按键锁定=============================
uint8_t Key_Lock =0;
//颜色控制=========================
typedef enum{
	ColorCtr_NOcmd=0,
 ColorCtr_init,
}ColorCtr_CMD;

ColorCtr_CMD ColorCtr_CmdTran = ColorCtr_init ;
//显示联动转换===============================
#define linkout1 1
#define linkout2 0
uint8_t Display_linked_conversion = 0; 

//串口校准标志=========================
uint8_t NOserialnum = 0;  
//color 全局变量声明=========================
uint8_t threered_pwmvalmax = 100;
uint8_t onered_pwmvalmax = 100;
uint8_t doubleged_pwmvalmax = 100;
uint8_t oneged_pwmvalmax =100;
 
uint8_t threered_pwmval = 0;
uint8_t onered_pwmval = 0;
uint8_t doubleged_pwmval = 0;
uint8_t oneged_pwmval = 0;
//串口+最小二=======================================
float x[] = {2190.0,2355.0,2519.0,2689.0,2850.0,3019.0,3186.0,3342.0,3507.0};
float y[] = {20.01,30.01,40.01,50.33,60.13,70.42,80.52,90.03,100.05};
int n = sizeof(x) / sizeof(x[0]);			//xy数组长度必须相同				

float k=0.0608, b=-113.08;  //默认值

#define maxdata 64
uint8_t testdate[maxdata]={0};
uint16_t testlen=0;


#define MAX_PRESSURE_STR 50
#define NUM_CALIBRATION_POINTS 7   //校准点数

uint8_t calibration_complete = 0;  // 校准完成标志
uint8_t error_flag = 0;  // 错误标志
uint8_t received_count = 0;  // 已接收的气压数据数量

uint8_t calibrationOrNo=0;//初始化完成标志

//=========================================================
void linearLeastSquares(float x[], float y[], int n, float *k, float *b) 
{
    float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
 
    for (int i = 0; i < n; i++) {
        sumX += x[i];
        sumY += y[i];
        sumXY += x[i] * y[i];
        sumX2 += x[i] * x[i];
    }
 
    // 计算斜率 k
    *k = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
 
    // 计算截距 b
    float meanX = sumX / n;
    float meanY = sumY / n;
    *b = meanY - (*k) * meanX;
}
void reset_calibration() 
{
    received_count = 0;
    memset(y, 0, sizeof(y));
}
void Usart_Process(void)
{
		 if(uartrxbuffer.uartfinish)
			{  
						uartrxbuffer.uartfinish=0; 
						testlen=DIV_Uart_ReadAvailable(testdate,sizeof(testdate));    //读取缓冲区
				   printf("%s\r\n",(char *)testdate); //回传接收到的数据
		
				 if (strstr((char *)testdate, "kPaG") == NULL)   //没有数据，或数据错误
					{
        error_flag = 1;  // 格式错误
        return;
					}
					
//				   DIV_Disp_Snprintf(MainScreen," CAl");    //接收到数据显示校准
//							DIV_Disp_Snprintf(SecondScreen,"mode");
 
    // 提取气压部分（"-0.00027"）
    char *kpa_pos = strstr((char *)testdate, "kPaG");
    if (kpa_pos == NULL) {
        error_flag = 1;
        return;
    }
 
    // 计算气压部分的长度
    int len = kpa_pos - (char *)testdate;
    if (len <= 0 || len >= MAX_PRESSURE_STR) {
        error_flag = 1;
        return;
    }
 
    // 临时存储气压字符串
    char pressure_str[20];
    strncpy(pressure_str, (char *)testdate, len);
    pressure_str[len] = '\0';
 
    // 转换为浮点数
    float pressure = atof(pressure_str);
 
    // 存储到数组
    if (received_count < NUM_CALIBRATION_POINTS) {
        y[received_count] = pressure;
								x[received_count] = adcData;
					   received_count++;
    }
 
    // 检查是否接收完成 接收到足够数量
    if (received_count >= NUM_CALIBRATION_POINTS) {
        calibration_complete = 1;
    }			
			}
}

void get_NowSet(void);
void Flash_Read_SetValue(void)
{
    	P1_Value=datahander.F_P1_Value;
					P2_Value=datahander.F_P2_Value;
					Hi1_Value=datahander.F_Hi1_Value;
					Lo1_Value=datahander.F_Lo1_Value;
					Hi2_Value=datahander.F_Hi2_Value;
					Lo2_Value=datahander.F_Lo2_Value;

					compsestatus1		=datahander.F_NormolmodeSetValue&0x07;
					compsestatus2		=datahander.F_NormolmodeSetValue&0x78;
					noncstatus					=datahander.F_NormolmodeSetValue&0x780;
					reactiontime			=datahander.F_NormolmodeSetValue&0x3F800;
					mainscreencolor=datahander.F_NormolmodeSetValue&0x3C0000;
					unitconversion	=datahander.F_NormolmodeSetValue&0x1FC00000;
		
					secscreen 				=datahander.F_PromodeSetValue&0x1f;
					dispeed							=datahander.F_PromodeSetValue&0xe0;
					diflevel						=datahander.F_PromodeSetValue&0xff00;
					colorreaction	=datahander.F_PromodeSetValue&0x30000;
					ecomode							=datahander.F_PromodeSetValue&0x1C0000;
					facrecover				=datahander.F_PromodeSetValue&0x600000; 
					
//					k = datahander.k;
//					b = datahander.b;

     calibrationOrNo= datahander.calibrationOrNo;
					NOserialnum = datahander.NOserialnum;
					
					get_NowSet();
}
void Flash_Write_SetValue(void)
{   
				uint8_t ifchange = 0; 

	   //模式设置配置位
					uint32_t NormolmodeSetValue = 0;
					uint32_t PromodeSetValue =0;
	
	    Datahander temp;
	
					rank_read_data(&datahander);
				
	
				if(fabsf(temp.F_P1_Value-P1_Value)>EPSILON){ifchange =1;}

				if(fabsf(temp.F_P2_Value-P2_Value)>EPSILON){ifchange =1;}

				if(fabsf(temp.F_Hi1_Value-Hi1_Value)>EPSILON){ifchange =1;}

				if(fabsf(temp.F_Lo1_Value-Lo1_Value)>EPSILON){ifchange =1;}
				
				if(fabsf(temp.F_Hi2_Value-Hi2_Value)>EPSILON){ifchange =1;}
				
				if(fabsf(temp.F_Lo2_Value-Lo2_Value)>EPSILON){ifchange =1;}
				

	   NormolmodeSetValue= compsestatus1|compsestatus2|noncstatus|reactiontime|mainscreencolor|unitconversion;
	   PromodeSetValue= secscreen|dispeed|diflevel|colorreaction|ecomode|facrecover;
	
				if(temp.F_NormolmodeSetValue!=NormolmodeSetValue){ifchange=1;}

				if(temp.F_PromodeSetValue!=PromodeSetValue){ifchange=1;}
				
				if(temp.k!=k){ifchange=1;}
				
				if(temp.b!=b){ifchange=1;}
				
				if(temp.calibrationOrNo != calibrationOrNo){ifchange=1;}
				
				if(temp.NOserialnum!=NOserialnum){ifchange=1;}
				
				
			datahander.F_P1_Value=P1_Value;
			datahander.F_P2_Value=P2_Value;
			datahander.F_Hi1_Value=Hi1_Value;
			datahander.F_Lo1_Value=Lo1_Value;
			datahander.F_Hi2_Value=Hi2_Value;
			datahander.F_Lo2_Value=Lo2_Value;

			datahander.F_NormolmodeSetValue= NormolmodeSetValue;
			datahander.F_PromodeSetValue=	PromodeSetValue;
				
			datahander.NOserialnum= NOserialnum;	
   datahander.calibrationOrNo= calibrationOrNo;
			datahander.k=k;
			datahander.b=b;     		

				if(ifchange)
				{
				  rank_write_data(&datahander);    
					ifchange =0;
				}                       
				
} 


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
    if (key_management.WichKeyDown==KEY2||key_management.WichKeyDown==KEY3) { // 按键按下
					
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
					   Flash_Write_SetValue();
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
				Flash_Write_SetValue();  //写入
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
			
			if(!NOWSETWICH)
			{
					 NOWSETWICH=NOWChoice[0];
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

void OutPut_Ctr(uint8_t * cmd)//输出控制管理
{
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
	 
		if(OutPutUpdata)
		{
			OutPutUpdata=0;
			if(out1_state){
						OUT1_on;
			}
			else
			{
						OUT1_off;
			}
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
		}
		
}
void OutPut_Ctr2(uint8_t * cmd)//输出控制管理
{
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
	 
		if(OutPutUpdata2)
		{
			OutPutUpdata2=0;
			if(compsestatus2!=compare2_off)
			{
			  if(out2_state){ 
						OUT2_on;
					}
					else
					{
								OUT2_off;
					}	
			}
		}
		
						
		switch(* cmd)
		{
			case OutPutNoCmd:{
			
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
							 out2_state =0;           
								OUT2_off;    //自动根据常开常闭设置
							 *cmd=OutPutNoCmd;
						}
			}break;
		}
		
}

void Yingcha_Comp1(void)   				//应差比较
{
	 uint8_t ifout1=0;
	
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
					}break;
					case compare1_HSY:{
											ifout1=HSY1;
					}break;
					case compare1_WCMP:{
											ifout1=WCMP1;
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
														  OutPut_CmdTran1=OutPut1_ON;//延时输出1命令
													}
							}break;
							case 1:{   
												if(unitchange_pressure_value<(P1_Value))
												{
																OutPut_CmdTran1=OutPut1_OFF;//延时输出1命令
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
																OutPut_CmdTran1=OutPut1_ON;//延时输出1命令;
														}
										}break;
										case 1:{
											  	if(unitchange_pressure_value<(Lo1_Value))
														{
																OutPut_CmdTran1=OutPut1_OFF;//延时输出1命令
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
														OutPut_CmdTran1=OutPut1_ON;//延时输出1命令;
												}
											
									}break;
									case 1:{
												 if(unitchange_pressure_value>(Hi1_Value+diflevel_value))
												{
														OutPut_CmdTran1=OutPut1_OFF;//延时输出1命令
												}
												else if(unitchange_pressure_value<(Lo1_Value))
												{
														OutPut_CmdTran1=OutPut1_OFF;//延时输出1命令
												}
									}break;
								}
			}break;
		}
			OutPut_Ctr(&OutPut_CmdTran1); //输出控制	
		
}

void Yingcha_Comp2(void)   				//应差比较
{
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
	
		 	switch(compsestatus2)   //选择应差的模式
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
						//		强制关闭out2
											out2_statedisp=0;
											HAL_OutPut_Set(OUT2,0);
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
																		OutPut_CmdTran2=OutPut2_ON;//延时输出2命令
															}
									}break;
									case 1:{   
												if(unitchange_pressure_value<(P2_Value))
												{
																OutPut_CmdTran2=OutPut2_OFF;//延时输出2命令
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
																OutPut_CmdTran2=OutPut2_ON;//延时输出2命令
														}
										}break;
										case 1:{
											  	if(unitchange_pressure_value<(Lo2_Value))
														{
															OutPut_CmdTran2=OutPut2_OFF;//延时输出2命令
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
														OutPut_CmdTran2=OutPut2_ON;//延时输出2命令
												}
											
									}break;
									case 1:{
												 if(unitchange_pressure_value>(Hi2_Value+diflevel_value))
												{
														OutPut_CmdTran2=OutPut2_OFF;//延时输出2命令
												}
												else if(unitchange_pressure_value<(Lo2_Value))
												{
														OutPut_CmdTran2=OutPut2_OFF;//延时输出2命令
												}
									}break;
								}
			}break;
			case OFF2:{
			
			}break;
		}
		OutPut_Ctr2(&OutPut_CmdTran2); //输出控制,为了在外面也能输出		
}


																		





void Data_Processing(uint8_t cmd)   	//数据处理管理
{
	  static float Zero_gap=0;  //校零差
	
   //读取气压值
			Current_pressure_value=(float)adcData*k+b;
	
			//校0准
	  Current_pressure_value-=Zero_gap;
	
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

			 //处理命令
	  switch(cmd)
			{
				case DataProcess_NoCmd:{
				
				}break;
				case DataProcess_Zeroing:{
				
					 Current_pressure_value=(float)adcData*k+b; //读取一次气压值
														
						Zero_gap = Current_pressure_value-0.0f;    //计算校零的差
					
      Dataprocess_delay_start=1;
					 if(Dataprocess_delay_tick>=Dataprocess_delayset)//1.5s后关闭
						{ 	
								Dataprocess_delay_start=0;
								finish_zeroing=1;//完成校准让主状态回去
								DataProcess_CmdTran=DataProcess_NoCmd;
						}
				}break;
			}
	
}






 

uint32_t threered_pwmpre = 0;
uint32_t onered_pwmpre = 0;
uint32_t doubleged_pwmpre = 0;
uint32_t oneged_pwmpre = 0;
void ColorSetLit(void)
{
    // 限制PWM值不超过最大值
    if(threered_pwmval > threered_pwmvalmax) threered_pwmval = threered_pwmvalmax;
    if(onered_pwmval > onered_pwmvalmax) onered_pwmval = onered_pwmvalmax;
    if(doubleged_pwmval > doubleged_pwmvalmax) doubleged_pwmval = doubleged_pwmvalmax;
    if(oneged_pwmval > oneged_pwmvalmax) oneged_pwmval = oneged_pwmvalmax;
    
    // 设置PWM输出
    DIV_Pwn_Set(&PWM_THREERED, 10, threered_pwmval, PWM_ID_TR);
    DIV_Pwn_Set(&PWM_ONERED, 10, onered_pwmval, PWM_ID_OR);
    DIV_Pwn_Set(&PWM_DOUBGED, 10, doubleged_pwmval, PWM_ID_DG);
    DIV_Pwn_Set(&PWM_ONEGED, 10, oneged_pwmval, PWM_ID_OG);
}
 


void ECOCtr(void)
{
    if(key_management.WichKeyDown != 0)
    {
					   ECO_delay_start = 1;
					    ECO_delay_flag = 0;
        ECO_delay_tick = 0;
        
					   PWM_THREERED.compare			= threered_pwmpre ;
								PWM_ONERED.compare					= onered_pwmpre 		;
								PWM_DOUBGED.compare			 = doubleged_pwmpre;
								PWM_ONEGED.compare					= oneged_pwmpre 		;
   
					// 设置最大值，而不是声明新的局部变量
        threered_pwmvalmax = 100;
        onered_pwmvalmax = 100;
        doubleged_pwmvalmax = 100;
        oneged_pwmvalmax = 100;
				} 
    
    // 获取当前正在点亮的灯并根据eco时间判断
   
    if(ECO_delay_flag)
    {
					    ECO_delay_start = 0;
					    ECO_delay_flag=0;
					
					
        uint8_t ecolevel = 0;
        switch(ecomode)
        {
            case eco_FULL:
                ecolevel = 0;
                break;
            case eco_off:
                ecolevel = 100;
                break;
            case eco_std:
                ecolevel = 30;
                break;
        }
								// 设置最大值，而不是声明新的局部变量
        threered_pwmvalmax = ecolevel;
        onered_pwmvalmax = ecolevel;
        doubleged_pwmvalmax = ecolevel;
        oneged_pwmvalmax = ecolevel;
								
								threered_pwmpre = PWM_THREERED.compare;
					   onered_pwmpre = PWM_ONERED.compare;
					   doubleged_pwmpre = PWM_DOUBGED.compare;
					   oneged_pwmpre = PWM_ONEGED.compare;
								
         ColorSetLit();
    }
}
 
void ColorLinkTrun(void)
{
    // 显示颜色联动转换
    switch(colorreaction)     
    {
        case colorreaction_out1:
            Display_linked_conversion = linkout1;
            break;
        case colorreaction_out2:
            Display_linked_conversion = linkout2;
            break; 
    }
    
    // 主屏颜色控制
    bool isOut1 = (Display_linked_conversion == linkout1);
    bool activeState = (isOut1 && out1_state) || (!isOut1 && out2_state);
    
    switch(mainscreencolor) 
    {
        case R_ON:
            threered_pwmval = activeState ? 100 : 0;
            doubleged_pwmval = activeState ? 0 : 100;
            break;
        case G_ON:
            threered_pwmval = activeState ? 0 : 100;
            doubleged_pwmval = activeState ? 100 : 0;
            break;
        case AL_RED:
            threered_pwmval = 100;
            doubleged_pwmval = 0;
            break;
        case AL_GREEN:
            threered_pwmval = 0;
            doubleged_pwmval = 100;
            break;
    }
    
    if(threered_pwmval > threered_pwmvalmax) threered_pwmval = threered_pwmvalmax;
				if(doubleged_pwmval > doubleged_pwmvalmax) doubleged_pwmval = doubleged_pwmvalmax;
				DIV_Pwn_Set(&PWM_THREERED, 10, threered_pwmval, PWM_ID_TR);
				DIV_Pwn_Set(&PWM_DOUBGED, 10, doubleged_pwmval, PWM_ID_DG);
}
 
void ColorCtr(uint8_t cmd)
{
    switch(cmd)
    {
        case ColorCtr_NOcmd:
            // 无操作
            break;
        case ColorCtr_init:
            threered_pwmval = 100;
            onered_pwmval = 100;
            doubleged_pwmval = 100;
            oneged_pwmval = 100;
            ColorSetLit();
            ColorCtr_CmdTran = ColorCtr_NOcmd;
            break;
        default:
            break;
    }
}

void ColorDiv(void)        //pwm驱动
{	
  	DIV_Pwn_Control(&PWM_THREERED);
			DIV_Pwn_Control(&PWM_ONERED);
			DIV_Pwn_Control(&PWM_DOUBGED);
			DIV_Pwn_Control(&PWM_ONEGED);
}



void MiniScreenOutset(uint8_t cmd) //小屏输出显示
{
	 static uint8_t ifdisp =0;
	
  switch(cmd)
		{
			case MiniScreen_NoCMD:{
			
			
			}break;
			case MiniScreen_OFF:{
			  ifdisp=0;
			   MiniScreen_TranCmd = MiniScreen_NoCMD;
			}break;
		 case MiniScreen_ON :{
			
				  ifdisp=1;
				   MiniScreen_TranCmd = MiniScreen_NoCMD;
			}break;
		}
	
	 if(ifdisp)
		{
		    //out12状态显示控制
					if(out1_statedisp)
					{
							DIV_Disp_SetPoint(Out1Screen,S1);
								
					}
					else
					{
							DIV_Disp_UnSetPoint(Out1Screen,S1);
								
					}
						if(out2_statedisp)
					{
							DIV_Disp_SetPoint(Out2Screen,S2);
					}
					else
					{
							DIV_Disp_UnSetPoint(Out2Screen,S2);
					}
		}
}


void MainDispOncePress(void)
{
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
}
	
void MainDisp(uint8_t cmd)    //主屏显示
{
	if(!cmd && runmode==Rstate_Run_mode && manu1_devicestate == status_run)
	{
	   Main_Disp_CmdTran=MainDisp_Pressure;//延迟显示气压
	}
	
  switch(cmd)  //选择对应显示命令
		{
			case MainDisp_NoCmd:{
				
				//无命令清空
				
			}break;
			case MainDisp_Show_Presets:{
			   DIV_Disp_ClearAllPoint(MainScreen);//小数点清除
				  DIV_Disp_ClearAllPoint(SecondScreen);//小数点清除
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
													    if(compsestatus2!=compare2_off)
																	{
																	 switch(noncstatus){
																		case noncstatus_1c2c:{DIV_Disp_ByString(MainScreen,"1c2c");DIV_Disp_ByString(SecondScreen,"NONC");}break;
																		case noncstatus_1o2c:{DIV_Disp_ByString(MainScreen,"1o2c");DIV_Disp_ByString(SecondScreen,"NONC");}break;
																		case noncstatus_1c2o:{DIV_Disp_ByString(MainScreen,"1c2o");DIV_Disp_ByString(SecondScreen,"NONC");}break;
																		case noncstatus_1o2o:{DIV_Disp_ByString(MainScreen,"1o2o");DIV_Disp_ByString(SecondScreen,"NONC");}break;
																		default:{}break;
																		}
																	}
																	else
																	{
																	 switch(noncstatus){
																		case noncstatus_1c2c:{DIV_Disp_ByString(MainScreen," 1NC");DIV_Disp_ByString(SecondScreen,"NONC");}break;
																		case noncstatus_1o2c:{DIV_Disp_ByString(MainScreen," 1NO");DIV_Disp_ByString(SecondScreen,"NONC");}break;
																		case noncstatus_1c2o:{DIV_Disp_ByString(MainScreen," 1NC");DIV_Disp_ByString(SecondScreen,"NONC");}break;
																		case noncstatus_1o2o:{DIV_Disp_ByString(MainScreen," 1NO");DIV_Disp_ByString(SecondScreen,"NONC");}break;
																		default:{}break;
																		}
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
							switch(prosetcustom)
							{
								case ProsetCustom_state:{
								      
								}break;
								case ProsetNO_state:{
								}break;
								case Prosetnth_state:{
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
																		DIV_Disp_ByString(MainScreen," OFF");
																			DIV_Disp_ByString(SecondScreen," Sub");
													}break;
													case secscreen_disunit:{
																		DIV_Disp_ByString(MainScreen,"UnIt");
																			DIV_Disp_ByString(SecondScreen," Sub");
													}break;
													case secscreen_discustom:{  
																		DIV_Disp_ByString(MainScreen,"CuSt");
																			DIV_Disp_ByString(SecondScreen," Sub");
																		
													}break;					
													case secscreen_disNOxx:{
																		DIV_Disp_Snprintf(MainScreen,"No  "); 
									    DIV_Disp_Symbol(NIXIE3,Symbol_x);
									    DIV_Disp_Symbol(NIXIE4,Symbol_x);
													DIV_Disp_Snprintf(SecondScreen," sub");
													DIV_Disp_Symbol(NIXIE2,Symbol_point);
													}break;
													case secscreen_standard:{
																			DIV_Disp_ByString(MainScreen," Std");
																			DIV_Disp_ByString(SecondScreen," Sub");
													}break;	
												}
											}break;
											case disspeedset_status:{
																switch(dispeed)
																{
																	case dispeed_250ms:{
																						DIV_Disp_ByString(MainScreen," 250");
																							DIV_Disp_ByString(SecondScreen,"D-SP");
																	}break;
																	case dispeed_500ms:{
																						DIV_Disp_ByString(MainScreen," 500");
																							DIV_Disp_ByString(SecondScreen,"D-SP");
																	}break;
																	case dispeed_1000ms:{
																							DIV_Disp_ByString(MainScreen,"1000");
																							DIV_Disp_ByString(SecondScreen,"D-SP");
																	}break;
																}
											}break;
											case difleveset_status:{
																switch(diflevel)
																{
																	case diflevel_level1:{
																							DIV_Disp_ByString(MainScreen,"   1");
																	}break;
																	case diflevel_level2:{
																							DIV_Disp_ByString(MainScreen,"   2");
																	}break;
																	case diflevel_level3:{
																								DIV_Disp_ByString(MainScreen,"   3");
																	}break;
																	case diflevel_level4:{
																								DIV_Disp_ByString(MainScreen,"   4");
																	}break;
																	case diflevel_level5:{
																								DIV_Disp_ByString(MainScreen,"   5");
																	}break;
																	case diflevel_level6:{
																								DIV_Disp_ByString(MainScreen,"   6");
																	}break;
																	case diflevel_level7:{
																								DIV_Disp_ByString(MainScreen,"   7");
																	}break;
																	case diflevel_level8:{
																									DIV_Disp_ByString(MainScreen,"   8");
																	}break;
																}
																	DIV_Disp_ByString(SecondScreen," HYS");
											}break;
											case coloractset_status:{
														switch(colorreaction)
														{
															case colorreaction_out1:{
																						DIV_Disp_ByString(MainScreen,"Out1");
																							DIV_Disp_ByString(SecondScreen,"DiSP");
															}break;
															case colorreaction_out2:{
																						DIV_Disp_ByString(MainScreen,"Out2");
																							DIV_Disp_ByString(SecondScreen,"DiSP");
															}break;
														}
											}break;
											case ecoset_status:{
														switch(ecomode)
														{
															case eco_off:{
																						DIV_Disp_ByString(MainScreen," OFF");
																							DIV_Disp_ByString(SecondScreen," ECO");
															}break;
															case eco_std:{
																					DIV_Disp_ByString(MainScreen," StD");
																							DIV_Disp_ByString(SecondScreen," ECO");
															}break;
															case eco_FULL:{
																								DIV_Disp_ByString(MainScreen," FULL");
																							DIV_Disp_ByString(SecondScreen," ECO");
															}break;
														}
											}break;
											case factorydatereset_status:{
														switch(facrecover)
														{
															case facrecoff:{
																				DIV_Disp_ByString(MainScreen," OFF");
																							DIV_Disp_ByString(SecondScreen,"RESt");
															}break;
															case facrecon:{
																			DIV_Disp_ByString(MainScreen,"  ON");
																							DIV_Disp_ByString(SecondScreen,"RESt");
															}break;
														}
											}break;																
										}
								
								}break;
							}
								
							
								}break;
								default:{}break;
						}
						Main_Disp_CmdTran=MainDisp_NoCmd;	   //取消命令
			}break;
			case MainDisp_Pressure:{
			
				uint16_t dispspeed_set=250;//默认250ms
				
				switch(dispeed) //选择延迟
				{
					case dispeed_500ms:{dispspeed_set=500;}break;
					case dispeed_1000ms:{dispspeed_set=1000;}break;
					case dispeed_250ms:{dispspeed_set=250;}break;
					default:{}break;
				}
				
				 Maindisp_delay_start=1;
				if(Maindisp_delay_tick>=dispspeed_set)//延迟显示
				{ 	
						Maindisp_delay_start=0;
					 Maindisp_delay_tick=0;
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
				}
				
			 Main_Disp_CmdTran=MainDisp_NoCmd;	   //取消命令
			}break;
			case MainDisp_Init:{
			 //显示RH01
				DIV_Disp_Snprintf(MainScreen,"RL01");  //显示款型
				DIV_Disp_Snprintf(SecondScreen,"_NPN");//显示npn款
			 Main_Disp_CmdTran=MainDisp_NoCmd;	   //取消命令
			}break;
			case MainDisp_Zeromode:{
				DIV_Disp_ClearAllPoint(MainScreen);
			 DIV_Disp_Snprintf(MainScreen,"0000");//显示四个零
			 Maindisp_delay_start=1;
				if(Maindisp_delay_tick>=Maindisp_delayset)//1.5s后关闭
				{ 	
						Maindisp_delay_start=0;
						Main_Disp_CmdTran=MainDisp_NoCmd;	   //取消命令
				}
			}break;
				
			case MainDisp_Peakvalleymode:{
				DIV_Disp_ClearAllPoint(MainScreen);
				
				DIV_Disp_Snprintf(MainScreen,"PEAK");//显示PEAK
				

				Main_Disp_CmdTran=MainDisp_NoCmd;	   //闪回
			}break;
			case MainDisp_PeakPressure:{
				
				DIV_Disp_ClearAllPoint(MainScreen);
				DIV_Disp_MultiplefloatNum(MainScreen,MAXairpress,1);		
				
				Main_Disp_CmdTran=MainDisp_NoCmd;	   //闪回
			}break;
			case MainDisp_KeyLockmode:{
			DIV_Disp_ClearAllPoint(MainScreen);
				
				DIV_Disp_Snprintf(MainScreen,"LocK");//显示Lock
			
				Main_Disp_CmdTran=MainDisp_NoCmd;	   //取消命令
			}break;
			case MainDisp_KeyLockdelay:{
			DIV_Disp_ClearAllPoint(MainScreen);
				
				DIV_Disp_Snprintf(MainScreen,"LocK");//显示Lock
			
				Maindisp_delay_start=1;
				if(Maindisp_delay_tick>=Maindisp_delayset)//1.5s后关闭
				{ 	
						Maindisp_delay_start=0;
						Main_Disp_CmdTran=MainDisp_NoCmd;	   //取消命令
				}
			
			}break;
			case MainDisp_Clear:{
			   DIV_Disp_ByString(MainScreen,"    ");
			   DIV_Disp_ByString(SecondScreen,"    ");
				 	Main_Disp_CmdTran=MainDisp_NoCmd;	   //取消命令
			}break;
			case MainDisp_ClearAfterdispress:{
				  DIV_Disp_ByString(MainScreen,"    ");
			   DIV_Disp_ByString(SecondScreen,"    ");
			   MainDispOncePress();
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
	if(!cmd && runmode==Rstate_Run_mode && manu1_devicestate == status_run)//如果没命令才正常显示，优先显示命令要求的,并且在run模式下才显示
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
			case SecondDisp_Peakvalleymode:{
			 DIV_Disp_ClearAllPoint(SecondScreen);

				DIV_Disp_Snprintf(SecondScreen,"botm");//显示botm 
				
			Second_Disp_CmdTran=SecondDisp_NoCmd;	
			}break;
			case SecondDisp_valleypressure:{
				DIV_Disp_ClearAllPoint(SecondScreen);
				
				DIV_Disp_MultiplefloatNum(SecondScreen,MINarepress,1);
				
				Second_Disp_CmdTran=SecondDisp_NoCmd;
			}break;
			case SecondDisp_KeyLockoff:{
			 DIV_Disp_ClearAllPoint(SecondScreen);
				
				DIV_Disp_Snprintf(SecondScreen,"OFF");//显示Lock
				
				Second_Disp_CmdTran=SecondDisp_NoCmd;
				
			}break;
			case SecondDisp_KeyLockon:{
			  DIV_Disp_ClearAllPoint(SecondScreen);
				
				DIV_Disp_Snprintf(SecondScreen,"ON");//显示Lock
				
				Second_Disp_CmdTran=SecondDisp_NoCmd;
			
			}break;
			
		}
}

void RunState_Keyprocess(void)
{
	if(Key_Lock)
	{
	  //短按设置
			if(	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV)
			{
				   Main_Disp_CmdTran=MainDisp_KeyLockdelay;
						
							key_management.PreWichKeyDown=0x00;	
							key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV=0;	
			}
			//长按事件========================================================
				if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV)
				{	
								if(key_management.WichKeyDown==(KEY1|KEY3))    //run模式按键锁定1.
								{

										  runmode=Rstate_KeyLock_mode;//进入按键锁定或退出按键锁定

								}
							key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=0;		
				}
			if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV)         //防止退出模式异常
				{	
											key_management.PreWichKeyDown=0x00;
									key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV=0;		
				}				
	}
	else
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
									   ColorCtr_CmdTran=ColorCtr_init;    //初始化颜色
												Main_Disp_CmdTran=MainDisp_Show_Presets;	   //显示预设
								}
								if(key_management.WichKeyDown==KEY2)      
								{ 	
									  	
								}
								if(key_management.WichKeyDown==KEY3)     
								{ 	
									  
								}
								if(key_management.WichKeyDown==(KEY3|KEY2))    //1.如果在run模式下长按key23 设置校零
								{
									   //显示0000  mode
												Main_Disp_CmdTran=MainDisp_Zeromode;
									
												//校0设置
												DataProcess_CmdTran=DataProcess_Zeroing;
									
									   //进入校零模式
												runmode=Rstate_Zeroing_mode;
									
								}
								if(key_management.WichKeyDown==(KEY1|KEY2))    //峰谷1.如果在run模式下长按key12 峰值谷值保持
								{
											runmode = Rstate_Peakvalley_mode;    //进入峰值谷值模式
										
								}
								if(key_management.WichKeyDown==(KEY1|KEY3))    //run模式按键锁定1.
								{

										  runmode=Rstate_KeyLock_mode;//进入按键锁定或退出按键锁定

								}
							key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=0;		
				}	
				if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV)         //防止退出模式异常
				{	
											key_management.PreWichKeyDown=0x00;
									key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV=0;		
				}
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
									  Flash_Write_SetValue();//写入设定
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
												   OutPutUpdata=1;
												   OutPutUpdata2=1;
														if(compsestatus2!=compare2_off)
														{
														  	if(noncstatus==noncstatus_1c2o){noncstatus=noncstatus_1o2o;}
														else{noncstatus=noncstatus<<1;}
														}
													 else
														{
																 	if(noncstatus==noncstatus_1c2o){noncstatus=noncstatus_1o2o;}
																		else{noncstatus=noncstatus_1c2o;}
														}
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
												OutPutUpdata=1;
												OutPutUpdata2=1;
												if(compsestatus2!=compare2_off)
												{
												   	if(noncstatus==noncstatus_1o2o){noncstatus=noncstatus_1c2o;}
														else{noncstatus=noncstatus>>1;}
												}
												else
												{
												    if(noncstatus==noncstatus_1o2o){noncstatus=noncstatus_1c2o;}
														else{noncstatus=noncstatus_1o2o;}
												}	
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
						  	manu1_devicestate=status_run;
									manu2_normsetstatus=noset_status;
						} 

						Flash_Write_SetValue();     //退出写入设定
						
						key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=0;
		}
			if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV)
		{
						if(key_management.WichKeyDown==KEY1)
						{      
						  manu1_devicestate=status_proset;//进入pro设置模式	
								manu2_normsetstatus=noset_status;
								manu2_promodesetstatus=pnoset_status;	
							Main_Disp_CmdTran=MainDisp_Show_Presets;	   //显示预设
						} 		
						key_management.Key_Event_Ctr.Key_Event_Bits.Key_4sLongPressEV=0;
		}
}
void Proset_Keyprocess(void)
{
	switch(prosetcustom)
	{
		case Prosetnth_state:{
													//触发短按事件
						if(	key_management.Key_Event_Ctr.Key_Event_Bits.key_pressEV)
						{
										if(key_management.PreWichKeyDown==KEY1)  	//短按mode键
										{
															if(manu2_promodesetstatus==factorydatereset_status){
																	manu1_devicestate=status_run;
																	manu2_promodesetstatus=pnoset_status;
																 Flash_Write_SetValue();
															}
															else{manu2_promodesetstatus++;}
														Main_Disp_CmdTran=MainDisp_Show_Presets;	   //显示预设	
										}										
										if(key_management.PreWichKeyDown==KEY2)
										{
															switch(manu2_promodesetstatus){   //切换设置项	
																case pnoset_status:{
		
																};break;
																case secscreendisset_status :{ 
																		if(secscreen==secscreen_disNOxx){secscreen=secscreen_off;}   //去除自定义设置这个
																		else{secscreen=secscreen<<1;}
																}break;
																case disspeedset_status	:{
																			if(dispeed==dispeed_1000ms){dispeed=dispeed_250ms;}
																			else{dispeed=dispeed<<1;}
																}break;
																case difleveset_status:{
																			if(diflevel==diflevel_level8){diflevel=diflevel_level1;}
																			else{diflevel=diflevel<<1;}
																}break;
																case coloractset_status:{
																			if(colorreaction ==colorreaction_out2){colorreaction=colorreaction_out1;}
																			else{colorreaction=colorreaction<<1;}	
																}break;
																case ecoset_status:{
																			if(ecomode==eco_FULL){ecomode=eco_off;}
																			else{ecomode=ecomode<<1;}
																	}break;
																case factorydatereset_status:{
																			if(facrecover==facrecon){facrecover=facrecoff;}
																			else{facrecover=facrecon;}
																}break;																			
																default:{}break;
														}
														//显示设置
														Main_Disp_CmdTran=MainDisp_Show_Presets;	   //显示预设
										}
										if(key_management.PreWichKeyDown==KEY3)
										{
															switch(manu2_promodesetstatus){   //切换设置项	
																case pnoset_status:{
																			
																};break;
																case secscreendisset_status :{ 
																		if(secscreen==secscreen_off){secscreen=secscreen_disNOxx;}//去除自定义设置这个
																		else{secscreen=secscreen>>1;}
																}break;
																case disspeedset_status	:{
																			if(dispeed==dispeed_250ms){dispeed=dispeed_1000ms;}
																			else{dispeed=dispeed>>1;}
																}break;
																case difleveset_status:{
																			if(diflevel==diflevel_level1){diflevel=diflevel_level8;}
																			else{diflevel=diflevel>>1;}
																}break;
																case coloractset_status:{
																			if(colorreaction ==colorreaction_out2){colorreaction=colorreaction_out1;}
																			else{colorreaction=colorreaction_out2;}	
																}break;
																case ecoset_status:{
																			if(ecomode==eco_off){ecomode=eco_FULL;}
																			else{ecomode=ecomode>>1;}
																	}break;
																case factorydatereset_status:{
																			if(facrecover==facrecon){facrecover=facrecoff;}
																			else{facrecover=facrecon;}
																}break;																			
																default:{}break;
														}
														//显示设置
													Main_Disp_CmdTran=MainDisp_Show_Presets;	   //显示预设
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
												   Flash_Write_SetValue();
											} 
											if(key_management.WichKeyDown==(KEY3|KEY2))
											{   
													
											}																		
											key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=0;
							}
		}break;
		default:break;
	}
}

void Peakvalleymode_Keyprocess(void)
{
		//长按事件=====================
					if(key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV)
					{	
									if(key_management.WichKeyDown==(KEY1|KEY2))    //峰谷4.如果在run模式下长按key123 峰值谷值保持退出
									{
												runmode = Rstate_Run_mode;    //回到run模式	
									}
								key_management.Key_Event_Ctr.Key_Event_Bits.Key_2sLongPressEV=0;		
					}
}
void Poweron_initialization(void)
{
	     ColorCtr_CmdTran=ColorCtr_init;    //初始化颜色
	
	    Main_Disp_CmdTran=MainDisp_Init;	   //显示初始化界面
	
	    MiniScreen_TranCmd=MiniScreen_OFF;  //关闭小屏
	
					 rank_read_data(&datahander); //读取预设

				if (isnan(datahander.F_P1_Value)) //如果没写过
				{
								Datahander temp={
								0,
								0.0,
								0.0,
								0.0,
								0.0,
								0.0,
								0.0,
								0x00920089,   //norset
								0x00298022,   //proset
								0.0608,       //k
								-113.08,      //b
								1,  //校准完成标志
								1}; 
								rank_write_data(&temp);
								
				}
				
				Flash_Read_SetValue();   //载入
	
					delay_start=1;		
					if(delay_tick>=delayset)//1s后
					{ 	
							delay_start=0;
							APM_EVAL_DelayMs(500);
						 Main_Disp_CmdTran=MainDisp_ClearAfterdispress;	 
							if(calibrationOrNo)
							{
								 offout=0;//打开应差判断才能输出
									manu1_devicestate=status_run;
									MiniScreen_TranCmd=MiniScreen_ON;  //小屏
									runmode = Rstate_Run_mode;
							}
							else
							{
							  	manu1_devicestate= status_barometric;     //进入校准
							}
					}
}
void barometric(void)  //串口校准
{
			if(calibrationOrNo)   ///初始化成功就跳转
			{
				   offout=0;//打开应差判断才能输出
							manu1_devicestate=status_run;
				   MiniScreen_TranCmd=MiniScreen_ON;  //小屏
				   runmode = Rstate_Run_mode;
			}
			
			
			Usart_Process(); //接收校准气压数据
			if (error_flag)  	// 格式错误
			{
							printf("格式错误！请重新发送数据。\r\n");
				   reset_calibration();  // 重置校准状态
							error_flag = 0;
							DIV_Disp_ClearAllPoint(MainScreen);
							DIV_Disp_Snprintf(MainScreen," EOO");   //显示错误
							DIV_Disp_Snprintf(SecondScreen," ERR");				
			}
			if (calibration_complete) // 校准完成
			{
				
							linearLeastSquares(x,y,n,&k,&b);   //计算曲线最小二
				
							printf("校准成功！接收到的气压值：\n");
							for (int i = 0; i < NUM_CALIBRATION_POINTS; i++) {
											printf("%d: %.5f kPa %0.f Adv\n", i + 1, y[i],x[i]);
							}
							printf("校准曲线=%fX%f",k,b);  //打印曲线
							
							calibrationOrNo=1;  //校准完成
							calibration_complete = 0;  // 重置标志
								//写入flash
							Flash_Write_SetValue();  //写
			}
}



void mainTask(void)
{	
		 switch(manu1_devicestate)
			{
				case status_init:{
				
				 Poweron_initialization();//初始化
				
				}break;
				case status_run:{
					switch(runmode)
					{
						case Rstate_Run_mode:{
			
								SetManage(SetManage_CmdTran);//设定管理
							
							 ColorLinkTrun();//主模式气压显示联动颜色转换

								RunState_Keyprocess();//主模式按键处理
						}break;
						case Rstate_Peakvalley_mode:{
							
								static uint8_t flashflag=0;
							
							
							 delay_start=1;		
								if(delay_tick>=delayset)//1s后
								{ 	
										delay_tick=0;
									 flashflag=~flashflag;
								}
								
								if(flashflag)   //闪烁
								{
								    //显示峰谷值模式
											Main_Disp_CmdTran=MainDisp_Peakvalleymode;
									  Second_Disp_CmdTran=SecondDisp_Peakvalleymode;
								}
								else
								{
											Main_Disp_CmdTran=MainDisp_PeakPressure;		 //显示气压
									  Second_Disp_CmdTran=SecondDisp_valleypressure;
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
							
							
							Peakvalleymode_Keyprocess(); //按键处理
						}break;
						case Rstate_Zeroing_mode:{
							 if(finish_zeroing)
								{
										finish_zeroing=0;
										runmode=Rstate_Run_mode;
								}
						}break;
						case Rstate_KeyLock_mode:{
						  
							Main_Disp_CmdTran=MainDisp_KeyLockmode;
							if(Key_Lock)
							{
								   
							    Second_Disp_CmdTran=SecondDisp_KeyLockoff;
							}
							else
							{
								  
							   Second_Disp_CmdTran=SecondDisp_KeyLockon;
							} 
							
							delay_start=1;		
							if(delay_tick>=delayset)//1s后
							{ 	
									delay_start=0;
								 Key_Lock=~Key_Lock;
									runmode = Rstate_Run_mode;
							}
							
						}break;
		    default:break;
					}
				}break;
					case status_normset:{

						Normset_Keyprocess();//设定模式按键处理
					}break;
					case status_proset:{
									
     Proset_Keyprocess();
					}break;
					case status_barometric:{
						barometric();					
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
	DIV_Pwm_Init();
	DIV_ReadValue_Init();  
	Hal_Out_Init();
	DIV_Disp_Init();
	Flash_Page_Init(&manager);
	DIV_Usart_Init();
//	 HAL_IWDT_Init();
	
	while (1)             
	{		 
		if(timer_10ms_execute)
		{
			timer_10ms_execute=0;  
			mainTask();  					//主任务执行 
		}
			Data_Processing(DataProcess_CmdTran);//数据处理
			Yingcha_Comp1();
			Yingcha_Comp2();
			ColorCtr(ColorCtr_CmdTran);//颜色设置命令
			MiniScreenOutset(MiniScreen_TranCmd);//小屏输出显示
			MainDisp(Main_Disp_CmdTran);	//主屏幕显示
			SecondDisp(Second_Disp_CmdTran);//副屏显示
			DIV_disp_out12andpoint(); //刷新小数点显示
		
		//ECOCtr(); //Eco
	}
}



void timer_IWDT_function(void)
{   
	  static uint16_t feed_dog_tick=0;

			if(feed_dog_tick<2000){feed_dog_tick++;}
			else{feed_dog_tick=0;	HAL_IWDT_Feed();}
}


void timer_delay_function(void)
{
  if(delay_start){
							if(delay_tick<delayset){delay_tick++;}
				}else{delay_tick=0;}
}
void timer_ECOdelay_function(void)
{
  if(ECO_delay_start){
							if(ECO_delay_tick<ECO_delayset){ECO_delay_tick++;}
							else{ECO_delay_flag=1;}
				}
}
		
void timer_dataprocess_function(void)
{ 
		if(Dataprocess_delay_start){
							if(Dataprocess_delay_tick<Dataprocess_delayset){Dataprocess_delay_tick++;}
				}else{Dataprocess_delay_tick=0;}
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
				    timer_dataprocess_function();//数据获取延时
				    timer_delay_function();//通用延时
				    timer_IWDT_function();//feeddog
				    timer_ECOdelay_function();//eco
	       DIV_key_read();
			}
}  

void TMR3Isr(void)
{
		 if (TMR_ReadIntFlag(TMR3, TMR_INT_FLAG_UPDATE) == SET)
			{
      TMR_ClearIntFlag(TMR3, TMR_INT_FLAG_UPDATE);  
				 

						PWM_THREERED.Pwm_Tick++;
						
						PWM_ONERED.Pwm_Tick++;
					
						PWM_DOUBGED.Pwm_Tick++;

						PWM_ONEGED.Pwm_Tick++;
      ColorDiv();			//主屏颜色驱动
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
