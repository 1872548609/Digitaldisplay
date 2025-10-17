/**************************************************************************************************
Filename:       iot_app.c
Revised:        $Date: 2019-8-20 15:35
Revision:       $Revision: V1.0.0

Description:    IoT应用程序主文件，包含任务初始化、按键处理和事件处理等功能

Copyright 2021 uwaycon. All rights reserved.
**************************************************-*****************^******************************/
	
#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _IOT_APP_H
/*************************************************-*************************************************
*                                               NOTE
**************************************************-*****************^******************************/
//-- source insight使用UTF-8编码，File->Reload As Encoding...->UTF-8
//-- source insight设置默认编码，Option->Preferences->Files->Default encoding->UTF-8

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/
// 头文件包含区域（实际使用时需要包含必要的头文件）

/*************************************************-*************************************************
*                                               MACRO
**************************************************-*****************^******************************/
// 宏定义区域

/*************************************************-*************************************************
*                                          CONSTANT DEFINED
**************************************************-*****************^******************************/
// 常量定义区域

/*************************************************-*************************************************
*                                           STRUCT DEFINED
**************************************************-*****************^******************************/
// 结构体定义区域

/*************************************************-*************************************************
*                                          GLOBAL VARIABLES
**************************************************-*****************^******************************/
// 全局变量定义区域

/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/
// 局部变量定义区域
uint8 iot_app_task_id;

// 轮询任务
void iot_app_Poll(void)
{
	pressure_readalways();
	
	output_screen_disupdate();
	
	DIV_disp_out12andpoint();
	
	Yingcha_Comp();
	
	output_ctr_update();
}
// 气压读取处理================================================
#if 1

#define WINDOW_SIZE 100
uint16_t adc_buffer[WINDOW_SIZE] = {0};
uint8_t ad_index = 0;
uint32_t adc_sum = 0; 			// adc累加
uint32_t adcData = 0;           // ad值
uint32_t VOL_value = 0;         // 电压值，参考3.3v
uint32_t adc_temp=0;

float k=0.0608, b=-113.08;  //默认值

// 气压传感器adc读取中断
void ADC_Isr(void)
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

// 返回当前气压值 kpa
float pressure_read_once(void)
{
	char data1[30]={0};
	
	float tempvalue=0;
	
	tempvalue=(float)adcData*k+b;
	
	snprintf(data1,sizeof(data1),"%0.3f",tempvalue);
		
	tempvalue = atof(data1);	
	
	return tempvalue;
}


void pressure_readalways(void)
{
	Current_pressure_value = pressure_read_once();
	
	unitchange_pressure_value = unitconversion(Current_pressure_value,unitconver_status);
}

#endif
// 输出控制=================================================
#if 1

#define WCMP1 0
#define HSY1 1
#define EASY1 2

#define WCMP2 3
#define HSY2 4
#define EASY2 5
#define OFF2 6	

float diflevel_value=0.0f;		// 应差值

uint16_t reactiontimeset=2;		//默认2ms    

uint8_t out1_state = 1;      //当前输出状态
uint8_t out2_state = 1;


uint8_t OUT1_state = 1;		//真实输出状态
uint8_t OUT2_state = 1;


#define OUT1_on()   HAL_OutPut_Set(OUT1,1)
																		
#define OUT1_off()  HAL_OutPut_Set(OUT1,0)

#define OUT2_on()   HAL_OutPut_Set(OUT2,1)
																		
#define OUT2_off()  HAL_OutPut_Set(OUT2,0)


#define OUT1ON 1
#define OUT1OFF 0

#define OUT2ON 2
#define OUT2OFF 3

uint16 OUT1_CMD = 0;
uint16 OUT2_CMD = 0;

void output_delay_set(uint8 cmd,uint32 time)
{
	if(cmd>1)
	{
		if(osal_msg_find(iot_app_task_id,IOT_APP_DELAYOUT2_EVT)==NULL)
		{
			OUT2_CMD |= cmd;
			
			osal_start_timerEx(iot_app_task_id,IOT_APP_DELAYOUT2_EVT,time);
		}	
	}
	else
	{
		if(osal_msg_find(iot_app_task_id,IOT_APP_DELAYOUT1_EVT)==NULL)
		{
			OUT1_CMD |= cmd;
			
			osal_start_timerEx(iot_app_task_id,IOT_APP_DELAYOUT1_EVT,time);
		}
	}
}

void reactiontime_choice(void)
{
	switch(reactime_status)
	{
		case reaction_100ms		:{reactiontimeset=100;	}break;
		case reaction_10ms		:{reactiontimeset=10;	}break;
		case reaction_2000ms	:{reactiontimeset=2000;	}break;
		case reaction_250ms		:{reactiontimeset=250;	}break;
		case reaction_2ms		:{reactiontimeset=2;	}break;
		case reaction_500ms		:{reactiontimeset=500;	}break;
		case reaction_50ms		:{reactiontimeset=50;	}break;
	}
}
void diflevel_choice(void)
{
	 switch(diflevel_status)     //应差选择
	{
		case diflevel_level1:{
				switch(unitconver_status)
				{
					case bAr	:{diflevel_value=0.001;}break;
					case inHG	:{diflevel_value=0.1;}break;
					case KgF	:{diflevel_value=0.001;}break;
					case KPR	:{diflevel_value=0.1;}break;
					case MMHG	:{diflevel_value=1.0;}break;
					case MPR	:{diflevel_value=0.001;}break;
					case PSI	:{diflevel_value=0.01;}break;
				}    
		}break;
		case diflevel_level2:{
				switch(unitconver_status)
				{
					case bAr	:{diflevel_value=0.002;}break;
					case inHG	:{diflevel_value=0.2;}break;
					case KgF	:{diflevel_value=0.002;}break;
					case KPR	:{diflevel_value=0.2;}break;
					case MMHG	:{diflevel_value=2.0;}break;
					case MPR	:{diflevel_value=0.002;}break;
					case PSI	:{diflevel_value=0.02;}break;
				}
			
			 
		
		}break;
		case diflevel_level3:{
			 switch(unitconver_status)
			{
				case bAr	:{diflevel_value=0.003;}break;
				case inHG	:{diflevel_value=0.3;}break;
				case KgF	:{diflevel_value=0.003;}break;
				case KPR	:{diflevel_value=0.3;}break;
				case MMHG	:{diflevel_value=3.0;}break;
				case MPR	:{diflevel_value=0.003;}break;
				case PSI	:{diflevel_value=0.03;}break;
			}
		
		
		}break;
		case diflevel_level4:{
			 switch(unitconver_status)
			{
				case bAr	:{diflevel_value=0.004;}break;
				case inHG	:{diflevel_value=0.4;}break;
				case KgF	:{diflevel_value=0.004;}break;
				case KPR	:{diflevel_value=0.4;}break;
				case MMHG	:{diflevel_value=4.0;}break;
				case MPR	:{diflevel_value=0.004;}break;
				case PSI	:{diflevel_value=0.04;}break;
			}
		
		}break;
		case diflevel_level5:{
			switch(unitconver_status)
			{
				case bAr	:{diflevel_value=0.005;}break;
				case inHG	:{diflevel_value=0.5;}break;
				case KgF	:{diflevel_value=0.005;}break;
				case KPR	:{diflevel_value=0.5;}break;
				case MMHG	:{diflevel_value=5.0;}break;
				case MPR	:{diflevel_value=0.005;}break;
				case PSI	:{diflevel_value=0.05;}break;
			}
		
		}break;
		case diflevel_level6:{
			switch(unitconver_status)
			{
				case bAr	:{diflevel_value=0.006;}break;
				case inHG	:{diflevel_value=0.6;}break;
				case KgF	:{diflevel_value=0.006;}break;
				case KPR	:{diflevel_value=0.6;}break;
				case MMHG	:{diflevel_value=6.0;}break;
				case MPR	:{diflevel_value=0.006;}break;
				case PSI	:{diflevel_value=0.06;}break;
			}
		
		
		}break;
		case diflevel_level7:{
			switch(unitconver_status)
			{
				case bAr	:{diflevel_value=0.007;}break;
				case inHG	:{diflevel_value=0.7;}break;
				case KgF	:{diflevel_value=0.007;}break;
				case KPR	:{diflevel_value=0.7;}break;
				case MMHG	:{diflevel_value=7.0;}break;
				case MPR	:{diflevel_value=0.007;}break;
				case PSI	:{diflevel_value=0.07;}break;
			}
		
		}break;
		case diflevel_level8:{
			switch(unitconver_status)
			{
				case bAr	:{diflevel_value=0.008;}break;
				case inHG	:{diflevel_value=0.8;}break;
				case KgF	:{diflevel_value=0.008;}break;
				case KPR	:{diflevel_value=0.8;}break;
				case MMHG	:{diflevel_value=8.0;}break;
				case MPR	:{diflevel_value=0.008;}break;
				case PSI	:{diflevel_value=0.08;}break;
			}
		
		}break;
	}
}

void Yingcha_Comp(void)   				//应差比较
{
	uint8_t ifout1=0;
	uint8_t ifout2=0;
	
	diflevel_choice();				// 应差值选中
	
	reactiontime_choice();			// 延迟时间选中

	switch(out1compare_status)   //选择应差的模式
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
	
	switch(out2compare_status)   //选择应差的模式
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
				out2_state=0;
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
						if(!out1_state)
						{
							out1_state=1;	
							
							output_delay_set(OUT1ON,reactiontimeset);
						}
					}
				}break;
				case 1:{   
					if(unitchange_pressure_value<(P1_Value))
					{
						if(out1_state)
						{
							out1_state=0;	
							output_delay_set(OUT1OFF,reactiontimeset);			
						}
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
							if(!out1_state)
							{
								out1_state=1;	
								
								output_delay_set(OUT1ON,reactiontimeset);
							}
					}
				}break;
				case 1:{
					if(unitchange_pressure_value<(Lo1_Value))
					{
							if(out1_state)
							{
								out1_state=0;	
								output_delay_set(OUT1OFF,reactiontimeset);			
							}
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
							if(!out1_state)
							{
								out1_state=1;	
								
								output_delay_set(OUT1ON,reactiontimeset);
							}
					}
						
				}break;
				case 1:{
					 if(unitchange_pressure_value>(Hi1_Value+diflevel_value))
					{
							if(out1_state)
							{
								out1_state=0;	
								output_delay_set(OUT1OFF,reactiontimeset);			
							}
					}
					else if(unitchange_pressure_value<(Lo1_Value))
					{
							if(out1_state)
							{
								out1_state=0;	
								output_delay_set(OUT1OFF,reactiontimeset);			
							}
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
									if(!out2_state)
									{
										out2_state=1;	
										
										output_delay_set(OUT2ON,reactiontimeset);
									}
						}
				}break;
				case 1:{   
						if(unitchange_pressure_value<(P2_Value))
						{
									if(out2_state)
									{
										out2_state=0;	
										output_delay_set(OUT2OFF,reactiontimeset);			
									}
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
									if(!out2_state)
									{
										out2_state=1;	
										
										output_delay_set(OUT2ON,reactiontimeset);
									}
							}
					}break;
					case 1:{
						if(unitchange_pressure_value<(Lo2_Value))
						{
									if(out2_state)
									{
										out2_state=0;	
										output_delay_set(OUT2OFF,reactiontimeset);			
									}
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
								if(!out2_state)
								{
									out2_state=1;	
									
									output_delay_set(OUT2ON,reactiontimeset);
								}
					}
						
				}break;
				case 1:{
					 if(unitchange_pressure_value>(Hi2_Value+diflevel_value))
					{
								if(out2_state)
								{
									out2_state=0;	
									output_delay_set(OUT2OFF,reactiontimeset);			
								}
					}
					else if(unitchange_pressure_value<(Lo2_Value))
					{
								if(out2_state)
								{
									out2_state=0;	
									output_delay_set(OUT2OFF,reactiontimeset);			
								}
					}
				}break;
			}
		}break;
		case OFF2:{
		
		}break;
	}
	
}

void output_ctr_update(void)
{
	if(OUT1_state)
	{
		OUT1_on();	
	}
	else
	{
		OUT1_off();	
	}
	if(OUT2_state)
	{
		OUT2_on();
	}
	else
	{
		OUT2_off();
	}		
}

#endif
// 输出屏显示============================================
#if 1
void output_screen_disupdate(void)
{
	if(OUT1_state)
	{
		DIV_Disp_SetPoint(Out1Screen,S1);
			
	}
	else
	{
		DIV_Disp_UnSetPoint(Out1Screen,S1);
			
	}
	if(OUT2_state)
	{
		DIV_Disp_SetPoint(Out2Screen,S2);
	}
	else
	{
		DIV_Disp_UnSetPoint(Out2Screen,S2);
	}
}
#endif
// 主屏显示================================================
#if 1
#define MAINSCREEN_DISPAFTERTIME 0x0001
#define MAINSCREEN_DISPPRESSURE  0x0002

uint32_t main_status = 0; // 副屏显示状态

char main_screen_now[8]={0}; // 副屏当前显示
char main_screen_save[8]={0}; // 保存副屏显示历史状态

uint8_t Disp_S1Point_now=0;	// 副屏小数点显示
uint8_t Disp_S1Point_save=0;	// 副屏小数点显示存储

uint16_t dispspeed_set=250;//默认250ms

// 选择显示气压延迟
void main_screen_choicedispeed(void)
{	
	switch(dispeed_status) //选择延迟
	{
		case dispeed_500ms:{dispspeed_set=500;}break;
		case dispeed_1000ms:{dispspeed_set=1000;}break;
		case dispeed_250ms:{dispspeed_set=250;}break;
		default:{}break;
	}
}
// 发送主屏显示事件
void main_screen_tranfromevt(uint32 evt)
{
	main_status |= evt;
}

// 停止主屏显示事件
void main_screen_stopevt(uint32 evt)
{
	main_status &= ~evt;
	
	if(evt == MAINSCREEN_DISPPRESSURE)
	{
		DIV_Disp_ClearAllPoint(MainScreen);
	}
}
	
// 按单位显示气压
void main_screen_disppressure(void)
{
	switch(unitconver_status)
	{
		case bAr:{
							main_screen_dispfloat("%0.3f",unitchange_pressure_value);
								
		}break;
		case inHG:{
							main_screen_dispfloat("%0.1f",unitchange_pressure_value);
							
		}break;
		case KgF:{
							main_screen_dispfloat("%0.3f",unitchange_pressure_value);
									
		}break;
		case KPR:{
							main_screen_dispfloat("%0.1f",unitchange_pressure_value);
			
		}break;
		case MMHG:{
							main_screen_dispfloat("%0.f",unitchange_pressure_value);
					
		}break;
		case MPR:{
							main_screen_dispfloat("%0.3f",unitchange_pressure_value);
						
		}break;
		case PSI:{
							main_screen_dispfloat("%0.2f",unitchange_pressure_value);
		}break;
	}		
	
}
// 保存显示
void main_screen_savestatus(void)
{
	strcpy(main_screen_save,main_screen_now);
	
	Disp_S1Point_save = Disp_S1Point_now;
}
// 回退显示
void main_screen_returnstatus(void)
{
	DIV_Disp_ByString(MainScreen,main_screen_save);
	
	DIV_Disp_SetPoint(MainScreen,Disp_S1Point_save);
		
	strcpy(main_screen_now,main_screen_save);
	
	Disp_S1Point_now = Disp_S1Point_save;
}
// 插入显示一段时间后回到上一个显示状态
void main_screen_dispaftertime(uint16_t time,const char * data1,...)
{
	DIV_Disp_ClearAllPoint(MainScreen);
	
	main_screen_savestatus();
	
	char data[9]={0};
	int size = sizeof(data);
	va_list ps;
	va_start(ps,data1);
	vsnprintf(data,size,data1,ps);	
	va_end(ps);   

	DIV_Disp_ByString(MainScreen,data);
	
	main_status |= MAINSCREEN_DISPAFTERTIME;
}

// 主屏显示内容
void main_screen_disp(const char * data1,...)
{
	char data[10]={0};
	int size = sizeof(data);
	va_list ps;
	va_start(ps,data1);
	vsnprintf(data,size,data1,ps);	
	va_end(ps);   

	DIV_Disp_ByString(MainScreen,data);
	
	strcpy(main_screen_now,data);
}

// 主屏显示浮点数最多三位小数
void main_screen_dispfloat(const char * data1,...)
{
	// 获取字符串参数
	char data[20]={0};
	int size = sizeof(data);
	va_list ps;
	va_start(ps,data1);
	vsnprintf(data,size,data1,ps);
	va_end(ps);   
	
	char * string = data;
	
	// 判断是否是负数
	uint8 isNegative = 0;
	if (data[0] == '-') {
		string++;
		isNegative = 1;
    } 
	
	// 获取长度
	int len = strlen(data);
	if (len >= 20) return; // 或报错处理
	
	// 获取小数点
	int ader =0;	// 小数点位置
	int i;			
	for(i=0;i<len;i++)
	{
		if(string[i]=='.')
		{
			ader = i;
		}
	}
	if(!ader){return;}// 没有小数就返回
	
	// 获取小数点前后的数字
	char frontdate[10]={0};// 获取小数点前的字符串
	char afterdate[10]={0};// 获取小数点后的字符串
	
	int numfront = 0;
	int countfront = 0;
	
	int numafter = 0;
	int countafter = 0;
		
	uint8_t head2=0;     //不要逗号初始化然后第一个不写初始值
	uint8_t head3=0;
	
	if(ader<len)
	{
		for(i=0;i<ader;i++)
		{
			frontdate[i]=string[i];
			head2++;
		}  
		frontdate[head2]='\0';

		numfront = atoi(frontdate);
		countfront = strlen(frontdate);
		
		for(i=ader+1;string[i]!='\0';i++)
		{  
			afterdate[head3]=string[i];
			head3++;
		}
		afterdate[head3]='\0';
		
		numafter = atoi(afterdate);
		countafter = strlen(afterdate);
	}
	
	// 防止警告
	if(numafter){}
	if(countafter){}
	
	// 显示
	char disp[20] = {0};
	
	char Disp_Point_save = 0;
	
	if(countafter==1){
		if(countfront==1){
			sprintf(disp,"  %s%s",frontdate,afterdate);
		}
		else if(countfront==2){
			sprintf(disp," %s%s",frontdate,afterdate);
		}
		else if(countfront==3){
			sprintf(disp,"%s%s",frontdate,afterdate);
		}
		if(isNegative)
		{
			if(countfront==1){
				sprintf(disp,"- %s%s",frontdate,afterdate);
			}
			else if(countfront==2){
				sprintf(disp,"-%s%s",frontdate,afterdate);
			}
			else if(countfront==3){
				sprintf(disp,"%s%s",frontdate,afterdate);
				disp[0]='*';
			}
		}
		DIV_Disp_SetPoint(MainScreen,P3);
		Disp_Point_save |= P3;
	}
	if(countafter==2){
		if(countfront==1){
			sprintf(disp," %s%s",frontdate,afterdate);
		}
		else if(countfront==2){
			sprintf(disp,"%s%s",frontdate,afterdate);
		}
		if(isNegative)
		{
			if(countfront==1){
				sprintf(disp,"-%s%s",frontdate,afterdate);
			}
			else if(countfront==2){
				sprintf(disp,"%s%s",frontdate,afterdate);
				disp[0]='*';
			}
		}
		DIV_Disp_SetPoint(MainScreen,P2);
		Disp_Point_save |= P2;
	}
	if(countafter==3){
		if(countfront==1){
			sprintf(disp,"%s%s",frontdate,afterdate);
		}
		if(isNegative)
		{
			if(countfront==1){
				sprintf(disp,"%s%s",frontdate,afterdate);
				disp[0]='-';
				if(numfront>=1)
				{
					sprintf(disp,"%s%s",frontdate,afterdate);
					disp[0]='*';
				}
			}
		}
		
		DIV_Disp_SetPoint(MainScreen,P1);
		Disp_Point_save |= P1;
	}
	
	DIV_Disp_ByString(MainScreen,disp);
	strcpy(main_screen_now,disp); 
	Disp_S1Point_now = Disp_Point_save ;
}

// 事件更新主屏
uint8 main_screen_dispupdate(void)
{
	if(main_status & MAINSCREEN_DISPAFTERTIME)
	{
		main_screen_returnstatus();
		
		return (main_status ^ MAINSCREEN_DISPAFTERTIME);
	}
	
	if(main_status & MAINSCREEN_DISPPRESSURE)
	{
		static uint16 count =0;
		
		main_screen_choicedispeed();
		
		if(count < (dispspeed_set/3))
		{
			count++;
		}
		else
		{
			count=0;
			main_screen_disppressure();
		}
		
		
		return (main_status ^ MAINSCREEN_DISPPRESSURE);
	}
	
	return 0;
}

#endif
// 副屏显示================================================
#if 1
#define SECONDSCREEN_DISPAFTERTIME 0x0001
#define SECONDSCREEN_DISPSETVALUE  0x0002

uint32_t second_status = 0; // 副屏显示状态

char second_screen_now[8]={0}; // 副屏当前显示
char second_screen_save[8]={0}; // 保存副屏显示历史状态

uint8_t Disp_S2Point_now=0;	// 副屏小数点显示
uint8_t Disp_S2Point_save=0;	// 副屏小数点显示存储

uint8_t dispset_returntime = 0;

// 根据当前单位显示设定值
void second_screen_dispsetvalue(float value)
{
	switch(unitconver_status)
	{
		case bAr:{
							second_screen_dispfloat("%0.3f",value);
								
		}break;
		case inHG:{
							second_screen_dispfloat("%0.1f",value);
							
		}break;
		case KgF:{
							second_screen_dispfloat("%0.3f",value);
									
		}break;
		case KPR:{
							second_screen_dispfloat("%0.1f",value);
			
		}break;
		case MMHG:{
							second_screen_dispfloat("%0.f",value);
					
		}break;
		case MPR:{
							second_screen_dispfloat("%0.3f",value);
						
		}break;
		case PSI:{
							second_screen_dispfloat("%0.2f",value);
		}break;
	}		
}
// 刷新显示当前选中设定值
void second_screen_dispnowsetvalue(void)
{
	float testvalue =0;
	
	switch(nowsetwhichyc)       
	{
		case set_P1:{
							testvalue=P1_Value;
									
		}break;
		case set_P2:{	
							testvalue=P2_Value;
		}break;
		case set_Hi1:{
							testvalue=Hi1_Value;

		}break;
		case set_Lo1:{
							testvalue=Lo1_Value;

		}break;
		case set_Hi2:{
							testvalue=Hi2_Value;

		}break;
		case set_Lo2:{
							testvalue=Lo1_Value;

		}break;
	}
	
	switch(unitconver_status)
	{
		case bAr:{
							second_screen_dispfloat("%0.3f",testvalue);
								
		}break;
		case inHG:{
							second_screen_dispfloat("%0.1f",testvalue);
							
		}break;
		case KgF:{
							second_screen_dispfloat("%0.3f",testvalue);
									
		}break;
		case KPR:{
							second_screen_dispfloat("%0.1f",testvalue);
			
		}break;
		case MMHG:{
							second_screen_dispfloat("%0.f",testvalue);
					
		}break;
		case MPR:{
							second_screen_dispfloat("%0.3f",testvalue);
						
		}break;
		case PSI:{
							second_screen_dispfloat("%0.2f",testvalue);
		}break;
	}		
}



// 发送屏显示事件
void second_screen_tranfromevt(uint32 evt)
{
	second_status |= evt;
}

// 停止屏显示事件
void second_screen_stopevt(uint32 evt)
{
	second_status &= ~evt;
	
	if(evt == SECONDSCREEN_DISPSETVALUE)
	{
		DIV_Disp_ClearAllPoint(SecondScreen);
	}
}
// 保存显示
void second_screen_savestatus(void)
{
	strcpy(second_screen_save,second_screen_now);
	
	Disp_S2Point_save = Disp_S2Point_now;
}
// 回退显示
void second_screen_returnstatus(void)
{
	DIV_Disp_ByString(SecondScreen,second_screen_save);
	
	DIV_Disp_SetPoint(SecondScreen,Disp_S2Point_save);
		
	strcpy(second_screen_now,second_screen_save);
	
	Disp_S2Point_now = Disp_S2Point_save;
}
// 插入显示一段时间后回到上一个显示状态
void second_screen_dispaftertime(uint16_t time,const char * data1,...)
{
	DIV_Disp_ClearAllPoint(SecondScreen);
	
	second_screen_savestatus();
	
	char data[9]={0};
	int size = sizeof(data);
	va_list ps;
	va_start(ps,data1);
	vsnprintf(data,size,data1,ps);	
	va_end(ps);   

	DIV_Disp_ByString(SecondScreen,data);
	
	second_status |= SECONDSCREEN_DISPAFTERTIME;
	
	dispset_returntime = time;
}

// 副屏显示内容
void second_screen_disp(const char * data1,...)
{
	char data[10]={0};
	int size = sizeof(data);
	va_list ps;
	va_start(ps,data1);
	vsnprintf(data,size,data1,ps);	
	va_end(ps);   

	DIV_Disp_ByString(SecondScreen,data);
	
	strcpy(second_screen_now,data);
}

//副屏显示浮点数最多三位小数
void second_screen_dispfloat(const char * data1,...)
{
	// 获取字符串参数
	char data[20]={0};
	int size = sizeof(data);
	va_list ps;
	va_start(ps,data1);
	vsnprintf(data,size,data1,ps);
	va_end(ps);   
	
	char * string = data;
	
	// 判断是否是负数
	uint8 isNegative = 0;
	if (data[0] == '-') {
		string++;
		isNegative = 1;
    } 
	
	// 获取长度
	int len = strlen(data);
	if (len >= 20) return; // 或报错处理
	
	// 获取小数点
	int ader =0;	// 小数点位置
	int i;			
	for(i=0;i<len;i++)
	{
		if(string[i]=='.')
		{
			ader = i;
		}
	}
	if(!ader){return;}// 没有小数就返回
	
	// 获取小数点前后的数字
	char frontdate[10]={0};// 获取小数点前的字符串
	char afterdate[10]={0};// 获取小数点后的字符串
	
	int numfront = 0;
	int countfront = 0;
	
	int numafter = 0;
	int countafter = 0;
		
	uint8_t head2=0;     //不要逗号初始化然后第一个不写初始值
	uint8_t head3=0;
	
	if(ader<len)
	{
		for(i=0;i<ader;i++)
		{
			frontdate[i]=string[i];
			head2++;
		}  
		frontdate[head2]='\0';

		numfront = atoi(frontdate);
		countfront = strlen(frontdate);
		
		for(i=ader+1;string[i]!='\0';i++)
		{  
			afterdate[head3]=string[i];
			head3++;
		}
		afterdate[head3]='\0';
		
		numafter = atoi(afterdate);
		countafter = strlen(afterdate);
	}
	
	// 防止警告
	if(numfront){}
	if(numafter){}
	
	// 显示
	char disp[20] = {0};
	
	char Disp_Point_save = 0;
	if(countafter==1){
		if(countfront==1){
			sprintf(disp,"  %s%s",frontdate,afterdate);
		}
		else if(countfront==2){
			sprintf(disp," %s%s",frontdate,afterdate);
		}
		else if(countfront==3){
			sprintf(disp,"%s%s",frontdate,afterdate);
		}
		if(isNegative)
		{
			if(countfront==1){
				sprintf(disp,"- %s%s",frontdate,afterdate);
			}
			else if(countfront==2){
				sprintf(disp,"-%s%s",frontdate,afterdate);
			}
			else if(countfront==3){
				sprintf(disp,"%s%s",frontdate,afterdate);
				disp[0]='*';
			}
		}
		DIV_Disp_SetPoint(SecondScreen,P6);
		Disp_Point_save |= P6;
	}
	if(countafter==2){
		if(countfront==1){
			sprintf(disp," %s%s",frontdate,afterdate);
		}
		else if(countfront==2){
			sprintf(disp,"%s%s",frontdate,afterdate);
		}
		if(isNegative)
		{
			if(countfront==1){
				sprintf(disp,"-%s%s",frontdate,afterdate);
			}
			else if(countfront==2){
				sprintf(disp,"%s%s",frontdate,afterdate);
				disp[0]='*';
			}
		}
		DIV_Disp_SetPoint(SecondScreen,P7);
		Disp_Point_save |= P7;
	}
	if(countafter==3){
		if(countfront==1){
			sprintf(disp,"%s%s",frontdate,afterdate);
		}
		if(isNegative)
		{
			if(countfront==1){
				sprintf(disp,"%s%s",frontdate,afterdate);
				disp[0]='-';
				if(numfront>=1)
				{
					sprintf(disp,"%s%s",frontdate,afterdate);
					disp[0]='*';
				}
			}
		}
		
		DIV_Disp_SetPoint(SecondScreen,P8);
		Disp_Point_save |= P8;
	}
	DIV_Disp_ByString(SecondScreen,disp);
	strcpy(second_screen_now,disp); 
	Disp_S2Point_now = Disp_Point_save ;
}

// 事件更新副屏
uint8 second_screen_dispupdate(void)
{
	if(second_status & SECONDSCREEN_DISPAFTERTIME)
	{
		
		if(dispset_returntime)
		{
			dispset_returntime-=2;
			return 0;
		}
		
		second_screen_returnstatus();
		
		return (second_status ^ SECONDSCREEN_DISPAFTERTIME);
	}
	if(second_status & SECONDSCREEN_DISPSETVALUE)
	{
		second_screen_dispnowsetvalue();
		
		return (second_status ^ SECONDSCREEN_DISPSETVALUE);
	}
	
	return 0;
}


#endif
// 设定值数据处理==================================================
#if 1
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

Output1Mode current_out1_mode = OT1_EASY;      //当前模式
Output2Mode current_out2_mode = OT2_OFF;

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




#define PRESS_ADD 1
#define PRESS_DOWN 0

uint8_t nowsetwhichyc = set_no;  //正在设置哪个
uint8_t nowcansetwhich = set_no; //可以设置那些
uint8_t NOWChoice[6]={0};  //可设置选项
uint8_t NOWChoicelength=0;

#define DISPDELAYOFF  100 // 按下mode显示后关闭的延时

// 循环选择副屏的可修改值
void get_nowycset(void) 					
{
	//选择模式
	if(out1compare_status==compare1_EASY)	{current_out1_mode=OT1_EASY;} 
	if(out1compare_status==compare1_WCMP)	{current_out1_mode=OT1_WCMP;}	
	if(out1compare_status==compare1_HSY)	{current_out1_mode=OT1_HSY;}			

	if(out2compare_status==compare2_EASY)	{current_out2_mode=OT2_EASY;} 
	if(out2compare_status==compare2_WCMP)	{current_out2_mode=OT2_WCMP;}	
	if(out2compare_status==compare2_HSY)	{current_out2_mode=OT2_HSY;}
	if(out2compare_status==compare2_off)	{current_out2_mode=OT2_OFF;}

	nowsetwhichyc=0;
	NOWChoicelength = 0;

	int i=0; 
	const uint8_t param_masks[6] = {set_P1, set_P2, set_Hi1, set_Lo1, set_Hi2, set_Lo2};
	for(i=0;i<6;i++)
	{
		if(param_editable[current_out1_mode][current_out2_mode][i])
		{
			nowsetwhichyc |= param_masks[i];          // 更新位掩码
			NOWChoice[NOWChoicelength++] = param_masks[i]; // 存储宏定义值
		}	
	}

	if(!nowsetwhichyc)
	{
		nowsetwhichyc=NOWChoice[0];
	}
}

// 运行态下按mode键演示
void modeset_choiceanddisplay(void)
{
	get_nowycset();
	static uint8_t choice;
	choice++;
	choice%=NOWChoicelength;
	nowsetwhichyc=NOWChoice[choice];
	switch(nowsetwhichyc)      //显示当前设定值
	{
		case set_P1:{	second_screen_dispaftertime(DISPDELAYOFF," P-1");}break;
		case set_P2:{	second_screen_dispaftertime(DISPDELAYOFF," P-2");}break;
		case set_Hi1:{	second_screen_dispaftertime(DISPDELAYOFF,"Hi-1");}break;
		case set_Lo1:{	second_screen_dispaftertime(DISPDELAYOFF,"Lo-1");}break;
		case set_Hi2:{	second_screen_dispaftertime(DISPDELAYOFF,"Hi-2");}break;
		case set_Lo2:{	second_screen_dispaftertime(DISPDELAYOFF,"Lo-2");}break;
	}  
}

#define EPSILON 1e-6f   //误差容许
#define MIN_DECIMAL_PRECISION 0.0001f   // 最小小数精度（根据实际需求调整）

typedef struct { // 参数配置结构体
    float *value;        // 参数地址（如 &P1_Value）
    float max_value;     // 最大值（按单位）默认1.05
    float *peer_value;   // 关联参数（如 Hi1 对应 Lo1，NULL 表示无）
    uint8_t peer_check;  // 限制规则：0=无限制, 1=必须小于关联参数, 2=必须大于关联参数
} ParamConfig;

// 参数配置表（顺序与 set_P1/set_Hi1 等宏定义一致）
const ParamConfig param_table[] = {
    {&P1_Value, 	1.05, NULL, 		0},       // set_P1: 无限制
    {&P2_Value, 	1.05, NULL, 		0},       // set_P2: 无限制
    {&Hi1_Value, 	1.05, &Lo1_Value, 	1}, // set_Hi1: 必须 > Lo1
    {&Lo1_Value, 	1.05, &Hi1_Value, 	2}, // set_Lo1: 必须 < Hi1
    {&Hi2_Value, 	1.05, &Lo2_Value, 	1}, // set_Hi2: 必须 > Lo2
    {&Lo2_Value, 	1.05, &Hi2_Value, 	2}, // set_Lo2: 必须 < Hi2
};
// 小数是否清除
uint8_t IsLastDecimalZero(float value,float current_increment)     
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
// 短按设置应差参数
void short_setycvalue(uint8_t addordown)  
{
	if (nowsetwhichyc == set_no) return; // 未选择参数，直接返回
	
	float max_val = 0; // 参数设置上限 
	float increment = 0;// 增量
	
    // 获取当前参数配置
    const ParamConfig *cfg = 0; // 假设 set_P1=0x01 对应索引 0
    switch(nowsetwhichyc)       // 选择要设定的值
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
	// 获取设置参数
	float *current_value = cfg->value;	
	
	// 选择不同单位的上限和增量  
	switch(unitconver_status)     
	{
		case bAr:{
				max_val=1.05;
				increment=0.001f;
		}break;
		case inHG:{
				max_val=31.0;
				increment=0.01f;
		}break;
		case KgF:{
				max_val=105.0;
				increment=0.1f;
		}break;
		case KPR:{
				max_val=105.0;
				increment=0.1f;
		}break;
		case MMHG:{
				max_val=787.5;
				increment=1.0f;
		}break;
		case MPR:{
				max_val=0.105;
				increment=0.001f;
		}break;
		case PSI:{
				max_val=15.22;
				increment=0.01f;
		}break;
	}
	
	// 暂存
	float temp = *current_value;    
				
  // 处理增减逻辑
	if (addordown) {  //如果增加
		if (temp < max_val) {
			temp += increment; // 增加
		} else {
			temp = max_val;//强制设为上限
			// 显示上
		}
	} else {
		if (temp > -max_val) {
			temp -=	increment; // 减少
		} else {
			temp = -max_val; // 强制设为下限
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
	char data1[30]={0};
	
	snprintf(data1,sizeof(data1),"%0.3f",temp);
		
	temp = atof(data1);	

    *current_value = temp; 
	
	second_screen_dispsetvalue( *current_value);
}


static uint32_t key_press_start_time = 0;  // 按键按下开始时间
static uint8_t fast_mode = 0;              // 加速模式标志
static uint8_t super_fast_mode = 0;        // 超级加速模式标志
static uint8_t keypressaddorsub = 0; 		//	长按加或减少
void long_setclearstates(void)
{
	key_press_start_time=0;
	fast_mode = 0; 
	super_fast_mode = 0; 
}
// 长按设置应差参数
void long_setycvalue(uint8_t addordown)          
{
    if (nowsetwhichyc == set_no) return; // 未选择参数，直接返回
    
    float max_val = 0; // 参数设置上限 
    float increment = 0; // 增量
    float base_increment = 0; // 基础增量（用于恢复）
 
    // 获取当前参数配置
    const ParamConfig *cfg = 0;
    switch(nowsetwhichyc)       // 选择要设定的值
    {
        case set_P1: cfg = &param_table[0]; break;
        case set_P2: cfg = &param_table[1]; break;
        case set_Hi1: cfg = &param_table[2]; break;
        case set_Lo1: cfg = &param_table[3]; break;
        case set_Hi2: cfg = &param_table[4]; break;
        case set_Lo2: cfg = &param_table[5]; break;
    }
    float *current_value = cfg->value;    // 获取设置参数
    
    // 选择不同单位的上限和增量
    switch(unitconver_status)
    {
        case bAr: max_val = 1.05; increment = 0.001f; break;
        case inHG: max_val = 31.0; increment = 0.01f; break;
        case KgF: max_val = 105.0; increment = 0.1f; break;
        case KPR: max_val = 105.0; increment = 0.1f; break;
        case MMHG: max_val = 787.5; increment = 1.0f; break;
        case MPR: max_val = 0.105; increment = 0.001f; break;
        case PSI: max_val = 15.22; increment = 0.01f; break;
    }
    
    base_increment = increment; // 保存基础增量
    
	// 记录首次按下时间
	if (key_press_start_time == 0)
	{
		key_press_start_time = osal_GetSystemClock(); // OSAL获取系统时间(ms)
	}
	
	// 计算按键持续时间
	uint32_t press_duration = osal_GetSystemClock() - key_press_start_time;
	
	// 1秒后进入快速模式（10倍速）
	if (press_duration >= 1000 && !fast_mode) { // 1000ms = 1秒
		fast_mode = 1;
		increment *= 10; // 加速10倍
	}

	// 5秒后进入超级快速模式（100倍速）
	if (press_duration >= 5000 && !super_fast_mode && IsLastDecimalZero(*current_value, base_increment)) { // 5000ms = 5秒
		super_fast_mode = 1;
		increment = base_increment * 100; // 加速100倍
	}
	
	
	if (press_duration >= 4000) { // 1000ms = 2秒
		osal_start_reload_timer(iot_app_task_id,IOT_APP_LONGKEYSET_YCVALUE_EVT,50);
	}
	
	if (press_duration >= 8000) { // 1000ms = 2秒
		osal_start_reload_timer(iot_app_task_id,IOT_APP_LONGKEYSET_YCVALUE_EVT,5);
	}
	
	// 超级加速模式下处理
	if (super_fast_mode) {
		int steps = (int)(*current_value / base_increment);
		
		if (addordown == 1) { // 增加
			steps += 10;
			*current_value = steps * base_increment;
			if (*current_value > max_val) {
				*current_value = max_val;
			}
		} else { // 减少
			steps -= 10;
			*current_value = steps * base_increment;
			if (*current_value < -max_val) {
				*current_value = -max_val;
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
		
		char data1[30]={0};
	
		snprintf(data1,sizeof(data1),"%0.3f",*current_value);
			
		*current_value = atof(data1);	
		
		second_screen_dispsetvalue( *current_value);
		
		return; // 直接返回
	}
	
	// 快速模式下处理
	if(fast_mode) {
		float temp = *current_value;
		// 处理增减逻辑
		if (addordown) {
			if (temp < max_val) {
				temp += increment;
			} else {
				temp = max_val;
			}
		} else {
			if (temp > -max_val) {
				temp -= increment;
			} else {
				temp = -max_val;
			}
		}
		
		// 处理 Hi/Lo 相互限制
		if (cfg->peer_value != NULL) {
			float peer_val = *cfg->peer_value;
			if (cfg->peer_check == 1 && temp < peer_val) {
				temp = peer_val;
			} else if (cfg->peer_check == 2 && temp > peer_val) {
				temp = peer_val;
			}
		}
		
		char data1[30]={0};
	
		snprintf(data1,sizeof(data1),"%0.3f",temp);
			
		temp = atof(data1);	
		
		*current_value = temp;
		
		second_screen_dispsetvalue( *current_value);
	}
}


#endif
// 单位转换=================================================
#if 1
static uint8_t unit_save = 0;
static uint8_t unit_now =0;

// 数值单位换算
float unitconversion(float value, uint8_t unit)
{
	char data1[30]={0};
	
	switch(unitconver_status)
	{
		case bAr:{
							value*=0.01f;
							snprintf(data1,sizeof(data1),"%0.3f",value);	
		}break;
		case inHG:{
							value*=0.2953;
							snprintf(data1,sizeof(data1),"%0.1f",value);
		}break;
		case KgF:{
							value*=0.02089;
							snprintf(data1,sizeof(data1),"%0.3f",value);		
		}break;
		case KPR:{
							snprintf(data1,sizeof(data1),"%0.1f",value);
		}break;
		case MMHG:{
							value*=7.5;
							snprintf(data1,sizeof(data1),"%0.0f",value);
		}break;
		case MPR:{
							value*=0.001;
							snprintf(data1,sizeof(data1),"%0.3f",value);
		}break;
		case PSI:{
							value*=0.145;
							snprintf(data1,sizeof(data1),"%0.2f",value);
		}break;
	}		
		
	value = atof(data1);	
	
	return value;
}

// 单位切换后修改数值保留3位小数
void systemunit_change(void)
{
	static float testvalue=0;
	
	char data1[30]={0};
	
	if(unit_save!=unit_now)
	{
		switch(nowsetwhichyc)       
		{
			case set_P1:{
								testvalue=P1_Value;
										
			}break;
			case set_P2:{	
								testvalue=P2_Value;
			}break;
			case set_Hi1:{
								testvalue=Hi1_Value;

			}break;
			case set_Lo1:{
								testvalue=Lo1_Value;

			}break;
			case set_Hi2:{
								testvalue=Hi2_Value;

			}break;
			case set_Lo2:{
								testvalue=Lo1_Value;

			}break;
		}
		snprintf(data1,sizeof(data1),"%0.3f",testvalue);
			
		testvalue = atof(data1);	

							
		switch(unit_save)
		{
			case bAr:{
							testvalue/=0.01f;
								
			}break;
			case inHG:{
							testvalue/=0.2953;
							
			}break;
			case KgF:{
							testvalue/=0.02089;
									
			}break;
			case KPR:{
					
			}break;
			case MMHG:{
							testvalue/=7.5;
						
			}break;
			case MPR:{
							testvalue/=0.001;
						
			}break;
			case PSI:{
							testvalue/=0.145;
			}break;
		}
		
		snprintf(data1,sizeof(data1),"%0.3f",testvalue);
			
		testvalue = atof(data1);						
			
		switch(unit_now)
		{
			case bAr:{
							testvalue*=0.01f;
							
			}break;
			case inHG:{
							testvalue*=0.2953;
						
			}break;
			case KgF:{
							testvalue*=0.02089;
								
			}break;
			case KPR:{
					
			}break;
			case MMHG:{
							testvalue*=7.5;
						
			}break;
			case MPR:{
							testvalue*=0.001;
						
			}break;
			case PSI:{
							testvalue*=0.145;
			}break;
		}
		snprintf(data1,sizeof(data1),"%0.3f",testvalue);
			
		testvalue = atof(data1);					
						
		switch(nowsetwhichyc)       //选择要设定的值
		{
			case set_P1:{
								P1_Value	=	testvalue;
														   
			}break;                                        
			case set_P2:{	                               
								P2_Value	=	testvalue;
			}break;                                        
			case set_Hi1:{                                 
								Hi1_Value	=	testvalue;
														   
			}break;                                        
			case set_Lo1:{                                 
								Lo1_Value	=	testvalue;
														   
			}break;                                        
			case set_Hi2:{                                 
								Hi2_Value	=	testvalue;
														   
			}break;                                        
			case set_Lo2:{                                 
								Lo1_Value	=	testvalue;

			}break;
		}				
	}							
	unit_save=unit_now;					
}
	
#endif 
// 背光调整=====================================
#if 1
void iot_allbacklight_set(uint8 en)
{
	if(en)
	{
		HalLedSet(HAL_LED_ALL,HAL_LED_MODE_ON);
	}
	else
	{
		HalLedSet(HAL_LED_ALL,HAL_LED_MODE_OFF);
	}
}
void iot_mainbacklight_set(uint8 color)
{
	HalLedSet(HAL_THREED_RED|HAL_DOUBLE_GREED,HAL_LED_MODE_OFF);
	
	switch(color)
	{
		case BACKLIGHT_OFF:{
			HalLedSet(HAL_THREED_RED|HAL_DOUBLE_GREED,HAL_LED_MODE_OFF);
		}break;
		case BACKLIGHT_GREEN:{
			HalLedSet(HAL_DOUBLE_GREED,HAL_LED_MODE_ON);
		}break;
		case BACKLIGHT_RED:{
			HalLedSet(HAL_THREED_RED,HAL_LED_MODE_ON);
		}break;
		case BACKLIGHT_YELLOW:{
			HalLedSet(HAL_THREED_RED|HAL_DOUBLE_GREED,HAL_LED_MODE_ON);
		}break;
	}
}

void iot_secondbacklight_set(uint8 en)
{
	if(en)
	{
		HalLedSet(HAL_ONE_GREED,HAL_LED_MODE_ON);
	}
	else
	{
		HalLedSet(HAL_ONE_GREED,HAL_LED_MODE_OFF);
	}
}

void iot_outputbacklight_set(uint8 en)
{
	if(en)
	{
		HalLedSet(HAL_ONE_RED,HAL_LED_MODE_ON);
	}
	else
	{
		HalLedSet(HAL_ONE_RED,HAL_LED_MODE_OFF);
	}
}

void iot_backlight_levelset(uint8_t level)
{
	HalLedBlink(HAL_LED_ALL,0,level,10);
}

#endif
// 任务区=====================================
uint8 iot_app_key_callback(uint8 cur_keys, uint8 pre_keys, uint32 poll_time_milliseconds)
{
	static uint8 islongorshortpress = 0; // 长按或短按标志
	
    uint8  k;
    uint8  key_mask = HAL_KEY_1;
    uint8  scan_flag = 1;
    uint8  press_keys = 0;      // 按下的按键
    uint8  hold_keys = 0;       // 按住的按键
	uint8  release_keys = 0;    // 释放的按键
	uint8_t longpress_morethan_3s_keys = 0; // 长按超过3秒的按键
	uint8_t longpress_morethan_4s_keys = 0;
	uint8_t longpresssetycvalue = 0;
	
    // 只处理有效的按键
    cur_keys &= IOT_APP_KEY_MASK;
    pre_keys &= IOT_APP_KEY_MASK;

    // 遍历所有按键
    for (k = 0; k < 8; k++,key_mask<<=1)
    {
        IOT_WATCHDOG_RESET(); // 喂狗

        // 跳过无效按键
        if (!(key_mask & IOT_APP_KEY_MASK))
        {
            continue;
        }
        
        // 按键按下处理
        if (cur_keys & key_mask)
        {
            // 短按检测
            if (hal_key_press_time_count[k] == 2)
            {
               islongorshortpress = 1;
            }
			// 持续按下
			if (hal_key_press_time_count[k] > 15)
            {
                hold_keys |= key_mask;
            }
            // 超长按检测（>3s）
			if (hal_key_press_time_count[k] == 30)
            {
				islongorshortpress = 2;
                longpress_morethan_3s_keys |= key_mask;
            }
			if (hal_key_press_time_count[k] == 50)
            {
				islongorshortpress = 2;
                longpress_morethan_4s_keys |= key_mask;
            }
			// 长按应差
			if (hal_key_press_time_count[k] == 15)
            {
                longpresssetycvalue |= key_mask;
            }
        }
        // 按键释放处理
        else
        {
			if((pre_keys & key_mask) && !(cur_keys & key_mask))
			{
				// 短按处理
				if(islongorshortpress == 1)
				{
					press_keys |= key_mask;
				}
				
				islongorshortpress = 0;
				
				release_keys |= key_mask;
			}
        }
    }
	
	// 长按设置应差==========================================
	if((release_keys & HAL_KEY_LEFT_ADD)||(release_keys & HAL_KEY_RIGHT_SUB))// 按键释放
	{
		if(system_state == RUN_STATE)
		{
			osal_stop_timerEx(iot_app_task_id,IOT_APP_LONGKEYSET_YCVALUE_EVT);
			long_setclearstates();
		}
	}
	
	if((longpresssetycvalue & HAL_KEY_LEFT_ADD)&&!(longpresssetycvalue & HAL_KEY_RIGHT_SUB))
	{
		if(system_state == RUN_STATE)
		{
			osal_start_reload_timer(iot_app_task_id,IOT_APP_LONGKEYSET_YCVALUE_EVT,100);
			keypressaddorsub = PRESS_ADD; 
		}
	}
	if(!(longpresssetycvalue & HAL_KEY_LEFT_ADD)&&(longpresssetycvalue & HAL_KEY_RIGHT_SUB))
	{
		if(system_state == RUN_STATE)
		{
			osal_start_reload_timer(iot_app_task_id,IOT_APP_LONGKEYSET_YCVALUE_EVT,100);
			keypressaddorsub = PRESS_DOWN; 
		}
	}
	// 普通按键功能==========================================
	
	if(press_keys & HAL_KEY_MODE)
	{
		if(system_state == MENU_STATE)
		{
			Menu_Execute(MENU_CBK_MODE);
		}
		if(system_state == RUN_STATE)
		{
			modeset_choiceanddisplay();	// 按键选择切换应差设定
		}
		
	}
	
	if(press_keys & HAL_KEY_LEFT_ADD)
	{
		if(system_state == MENU_STATE)
		{
			Menu_Execute(MENU_CBK_ADD);
		}
		else if(system_state == RUN_STATE)
		{
			short_setycvalue(PRESS_ADD);
			second_status &= ~SECONDSCREEN_DISPAFTERTIME;// 防止按下mode后恢复旧现场改变当前副屏的显示
		}
	}
	
	if(press_keys & HAL_KEY_RIGHT_SUB)
	{
		if(system_state == MENU_STATE)
		{
			Menu_Execute(MENU_CBK_SUB);
		}
		else if(system_state == RUN_STATE)
		{
			short_setycvalue(PRESS_DOWN);
			second_status &= ~SECONDSCREEN_DISPAFTERTIME;// 防止按下mode后恢复旧现场改变当前副屏的显示
		}
	}

	if(longpress_morethan_3s_keys & HAL_KEY_MODE)
	{
		if(system_state == RUN_STATE)
		{
			MenuSystem_Start();		// 启动菜单
			Menu_Enter(); // 进入根下第一个菜单
			Menu_Execute(INCALLBACK);
			
			main_screen_stopevt(MAINSCREEN_DISPPRESSURE);// 关闭主屏刷新气压
			second_screen_stopevt(SECONDSCREEN_DISPSETVALUE);// 关闭副屏刷新设定值
			
			system_state = MENU_STATE; 
		}
		else if(system_state == MENU_STATE)
		{
			MenuSystem_Stop();		// 关闭菜单
			Menu_Execute(OUTCALLBACK);
			
			main_screen_tranfromevt(MAINSCREEN_DISPPRESSURE);// 主屏刷新气压
			second_screen_tranfromevt(SECONDSCREEN_DISPSETVALUE);// 副屏刷新设定值
			
			system_state = RUN_STATE; 
		}
		
	}
	
	if(longpress_morethan_4s_keys & HAL_KEY_MODE)
	{
		if(system_state == MENU_STATE)
		{
			if((strcmp(menuSystem.current->text,"NORMSET") == 0))
			{
				Menu_Next();
				Menu_Execute(INCALLBACK);
			}
		}
	}
	
	return scan_flag;
}

void iot_app_init(uint8 task_id)
{
    iot_app_task_id = task_id; // 保存任务ID
	
	DIV_Disp_Snprintf(MainScreen,"RL01");  //显示款型
	second_screen_disp(" NPN");//显示npn款
	
	iot_allbacklight_set(BACKLIGHT_ON);		// 打开背光
	
	main_screen_tranfromevt(MAINSCREEN_DISPPRESSURE);// 主屏刷新气压
	second_screen_tranfromevt(SECONDSCREEN_DISPSETVALUE);// 副屏刷新设定值
	osal_start_reload_timer(iot_app_task_id,IOTAPP_DISPSECOND_EVT,50);
	osal_start_reload_timer(iot_app_task_id,IOT_APP_TIMER_EVT,1);
	
	MenuItem* root = CreateTestMenu(); // 动态创建菜单，所有菜单都在这个函数里编辑好
    MenuSystem_Init(root);	// 初始化系统菜单

    // 注册按键回调函数
    HalKeyCallbackRegister(iot_app_key_callback);
}

uint16 iot_app_process_event(uint8 task_id, uint16 events)
{
    (void)task_id; // 显式忽略未引用参数（避免编译器警告）
     
    // 处理系统消息事件
    if ( events & SYS_EVENT_MSG )
    {
        // 接收消息队列中的消息
        uint8* msg_packet = (uint8 *)osal_msg_receive(iot_app_task_id);
        while ( msg_packet )
        {
            // 根据消息事件类型分发处理
            //switch ( msg_packet->hdr.event )
            //{
            //case KEY_CHANGE://按键事件处理
            // //UserApp_HandleKeys( ((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
            // break;

            //default:
            // break;
            //}

            // 释放消息内存
            osal_msg_deallocate((uint8 *)msg_packet);

            // 接收下一条消息
            msg_packet = (uint8 *)osal_msg_receive(iot_app_task_id);
        }

        // 返回未处理的事件（清除已处理的SYS_EVENT_MSG）
        return (events ^ SYS_EVENT_MSG);
    }
	

	if(events & IOT_APP_DELAYOUT1_EVT)
	{
		if(OUT1_CMD == OUT1ON)
		{
			uint32_t temp = outnonc_status & out1;
			if(temp)
			{
				OUT1_state=1;
			}
			else
			{
				OUT1_state=0;
			}
			
		}
		if(OUT1_CMD == OUT1OFF)
		{
			uint32_t temp = outnonc_status & out1;
			if(temp)
			{
				OUT1_state=0;
			}
			else
			{
				OUT1_state=1;
			}
		}
		
		OUT1_CMD =0;
		
		return (events ^ IOT_APP_DELAYOUT1_EVT);
	}
	
	if(events & IOT_APP_DELAYOUT2_EVT)
	{
			
		if(OUT2_CMD == OUT2ON)
		{
			uint32_t temp = outnonc_status & out2;
			if(temp)
			{
				OUT2_state=1;
			}
			else
			{
				OUT2_state=0;
			}
			
		}
		if(OUT2_CMD == OUT2OFF)
		{
			uint32_t temp = outnonc_status & out2;
			if(temp)
			{
				OUT2_state=0;
			}
			else
			{
				OUT2_state=1;
			}
		}
		
		OUT2_CMD =0;
		
		return (events ^ IOT_APP_DELAYOUT2_EVT);
	}

	if(events & IOT_APP_LONGKEYSET_YCVALUE_EVT)
	{
		long_setycvalue(keypressaddorsub);
		
		return (events ^ IOT_APP_LONGKEYSET_YCVALUE_EVT);
	}
	

	if(events & IOTAPP_DISPSECOND_EVT)
	{

		second_screen_dispupdate();
		
		events &= ~IOTAPP_DISPSECOND_EVT;
	}
	
	if(events & IOT_APP_TIMER_EVT)
	{
		
		main_screen_dispupdate();
		
		return (events ^IOT_APP_TIMER_EVT);
	}
	
	// 丢弃未知事件
    return 0;
}

/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif
