/**************************************************************************************************
Filename:       iot_app_data_processing_and_output_control.c
Revised:        $Date: 2019-8-20 15:35
Revision:       $Revision: V1.0.0

Description:

Copyright 2021 uwaycon. All rights reserved.
                                                
**************************************************-*****************^******************************/
#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H
/*************************************************-*************************************************
*                                               NOTE
**************************************************-*****************^******************************/
//-- source insight use UTF-8 to encode, File->Reload As Encoding...->UTF-8
//-- source insight set default encode, Option->Preferences->Files->Default encoding->UTF-8

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                               MACRO
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          CONSTANT DEFINED
**************************************************-*****************^******************************/

#define TESTFUNCTION  TRUE

#define SECOND_SCREEN_DISP_RETURN_TIME 600

#define PRESS_ADD 1
#define PRESS_DOWN 0

#define OUT1_MODE_COUNT 3
#define OUT2_MODE_COUNT 4
#define set_no  0x00 //开机无设置
#define set_P1 	0x01
#define set_P2 	0x02
#define set_Hi1 0x04
#define set_Lo1 0x08
#define set_Hi2 0x10
#define set_Lo2 0x20

#define IOT_APP_DATA_SET_TOLERANCE 0

/*************************************************-*************************************************
*                                           STRUCT DEFINED
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          GLOBAL VARIABLES
**************************************************-*****************^******************************/
management_of_output m_output = {0};

management_of_secondscreen m_secondscreen = {0};

/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/
uint8_t iot_app_data_processing_and_output_control_task_id;                   

/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/
uint32_t iot_app_data_processing_and_output_control_2ms_timer_events = 0;
int8_t iot_app_data_processing_and_output_control_keynum = 0;                      
uint8_t iot_app_data_processing_and_output_control_first_key = 0;      

uint8 iot_app_data_processing_and_output_control_event_set_value_flag = IOT_APP_DATA_SET_TOLERANCE;
uint8_t iot_app_hold_key_set_value_stage_index = 0;
const uint32_t iot_app_hold_key_set_value_stage_speed[] = {500, 50, 20, 1};  // 各阶段速度增量，每多少毫秒变化一次

uint8_t output_timer_set_out1_which_out = 0; 
uint8_t output_timer_set_out2_which_out = 0;
uint8_t output1_timer_finish_flag = 1;
uint8_t output2_timer_finish_flag = 1;


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
Output1Mode current_out1_mode = OT1_EASY;     
Output2Mode current_out2_mode = OT2_OFF;
bool param_editable[OUT1_MODE_COUNT][OUT2_MODE_COUNT][6] = {
   
    [OT1_EASY] = {    //p1    p2    hi1    lo1   hi2    lo2
        [OT2_EASY] = {true, true, 	false, false, false, false},  	
        [OT2_HSY] = 	{true, false, false, false, true, 	true},  
        [OT2_WCMP] = {true, false, false, false, true, 	true},   
        [OT2_OFF] = 	{true, false, false, false, false, false},  	
    },
   
    [OT1_HSY] = {
        [OT2_EASY] = {false, true, true, true, false, false}, 	
        [OT2_HSY] = 	{false, false, true, true, true, true},   
        [OT2_WCMP] = {false, false, true, true, true, true},  	
        [OT2_OFF] = 	{false, false, true, true, false,false}, 
    },
    
    [OT1_WCMP] = {
        [OT2_EASY] = {false, true, true, true, false, false},  		
        [OT2_HSY] = 	{false, false, true, true, true, true},   	
        [OT2_WCMP] = {false, false, true, true, true, true},    
        [OT2_OFF] = 	{false, false, true, true, false,false},    
    },
};
uint8_t nowsetwhichyc = set_no;  //= 正在设置哪个
uint8_t nowcansetwhich = set_no; //= 可以设置那些
uint8_t NOWChoice[6]={0}; 
uint8_t NOWChoicelength=0;
static uint8_t choice;


typedef struct { //= 参数配置结构体
    float *value;        
    float max_value;     
    float *peer_value;   
    uint8_t peer_check;  //= 0=无限制, 1=必须小于, 2=必须大于
} paramconfig;
//= 参数配置表（顺序与 set_P1/set_Hi1 等宏定义一致）
const paramconfig param_table[] = {
    {&iot_app_common_data_tolerance_P1, 	1.05, NULL, 		0},       
    {&iot_app_common_data_tolerance_P2, 	1.05, NULL, 		0},      
    {&iot_app_common_data_tolerance_Hi1, 	1.05, &iot_app_common_data_tolerance_Lo1, 	1}, 
    {&iot_app_common_data_tolerance_Lo1, 	1.05, &iot_app_common_data_tolerance_Hi1, 	2}, 
    {&iot_app_common_data_tolerance_Hi2, 	1.05, &iot_app_common_data_tolerance_Lo2, 	1}, 
    {&iot_app_common_data_tolerance_Lo2, 	1.05, &iot_app_common_data_tolerance_Hi2, 	2}, 
};                     
                      /*KPA  KGF    BAR    PSI   MMHG  INHG  MPA*/
float tolerance_max[]={105.0,1.071 ,1.05 ,15.22,787.5,31.0  ,0.105};
float increment_set[]={0.1  ,0.001 ,0.001,0.01 ,1.0  ,0.1   ,0.001};
float unit_conversion[]={1  ,0.0102,0.01 ,0.145,7.500,0.2953,0.001};


#define MAXDATA 64
#define MAX_PRESSURE_STR 50
#define NUM_CALIBRATION_POINTS 7   //校准点数
//float x[] = {2190.0,2355.0,2519.0,2689.0,2850.0,3019.0,3186.0,3342.0,3507.0};
//float y[] = {20.01,30.01,40.01,50.33,60.13,70.42,80.52,90.03,100.05};
//float k=0.0608, b=-113.08; 

//= 0
float x[] = {3347.0,2693.0,2199.0,1871.0,1536.0,1045.0,402.0};

//= 1
//float x[] = {3331.0,2678.0,2188.0,1856.0,1531.0,1034.0,383.0};

//= 2
//float x[] = {3314.0,2664.0,2177.0,1844.0,1514.0,1032.0,372.0};

float y[] = {90 ,50 ,20 ,0 ,-20.0 ,-50.0 , -90.0 };

float k=0, b=0; 

float k1=0, b1=0;
float k2=0, b2=0;
float k3=0, b3=0;
float k4=0, b4=0;
float k5=0, b5=0;
float k6=0, b6=0;

int n = sizeof(x) / sizeof(x[0]);					

uint8_t testdate[MAXDATA]={0};
uint16_t testlen=0;
uint8_t received_count = 0;  // 已接收数据数量

float zero_calibration_value = 0;
uint8 zero_calibration_flag = 0;

uint16 out1_timer_count = 0;
uint16 out2_timer_count = 0;
/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/
float float_keep_decimals(float value,uint8 decimal_places);
float unitconversion(float value, uint8_t oldunit, uint8 dstunit);
float pressure_read_once(void);

void out_put_react_time_set_output(uint8 out1orout2 ,uint8 output_set_state, management_of_output* temp);
void out_put_react_time_choice(uint8 reactime ,management_of_output* temp);
void out_put_diflevel_choice(uint8 level,uint8 units ,management_of_output* temp);
void output_comp_control(management_of_output* temp);

void main_screen_disp_pressure_start(void);
void main_screen_disp_pressure_stop(void);
uint32 main_screen_disp_speed_choice(uint8 speed);
void main_screen_dispfloat(const char * data1,...); //= 主屏显示浮点数最多三位小数
void main_screen_disp_float_by_unit(uint8 unit,float value);

void tolerance_set_change_unit(uint8 nowunit);
float unitconversiontolerance(float value, uint8_t oldunit, uint8 dstunit);

void second_screen_disp_clear_point(void);
void second_screen_disp_start(void);
void second_screen_disp_stop(void);
void second_screen_disp_choice(uint8 choice);
void second_screen_disp_2ms(void);
void second_screen_disp_float_by_unit(uint8 unit,float value);
void second_screen_dispfloat(const char * data1,...);
void second_screen_disp_now_set_value(void);
void second_screen_dispaftertime(uint16_t time,const char * data1,...);
void second_screen_disp_float_aftertime(uint16 time , float value);
void second_screen_disp(void);
void get_now_yc_set(void);
void flash_now_set(void);

void out_put_nonc_set_output_flashonce(void);
void out_put_nonc_set_output(uint8 output_set_which,uint8 output_en);
void out_put_react_time_set_output(uint8 out1orout2 ,uint8 output_set_state, management_of_output* temp);


/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/
#ifdef APM32F051

//= 轮询任务
void iot_app_data_processing_and_output_control_poll(void)
{
    if(
        ((iot_app_data_open_system_init_flag == IOT_APP_SYSTEM_INIT_OK)
        &&(!iot_app_menu_is_active()))
        ||(iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_KEY_LOCK)
        ||(iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_PEAK_BOTTOM_HOLD)
    )
    {
        static uint8 i =1;
        while(i)
        {
            i--;
            
            #if (defined TESTFUNCTION) && (TESTFUNCTION == TRUE)
   
            //iot_app_common_data_pressure_unit_flag = UNIT_KPA;
            
            //iot_app_common_data_hysteresis_level = IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_8;
            
            //second_screen_disp_choice(SECOND_SCREEN_DISP_TOLERANCE);
            
            #endif
            
             osal_start_reload_timer(iot_app_data_processing_and_output_control_task_id,
                IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_2MS_TIMER_EVT,
                IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_2MS_TIMER_INTERVAL);
            
            get_now_yc_set();

            main_screen_disp_pressure_start();
            
            second_screen_disp();
             
            
            //= 初始化后读取如果有校零值就打开校零
            if(fabs(zero_calibration_value  - 0.0)>FLOAT_MIN_DIFFERENCE) 
            {
                zero_calibration_flag = 1;
            }
        }
    } 

    
    if(iot_app_data_open_system_init_flag != IOT_APP_SYSTEM_INIT_OK)
    {
        return ;
    }
        
    
    iot_app_common_data_now_pressure = pressure_read_once();
    
    //= 校零
    if(zero_calibration_flag)
    {
        iot_app_common_data_now_pressure -= zero_calibration_value;
    }

    iot_app_common_data_unit_conversion_pressure = iot_app_common_data_now_pressure;
    
    iot_app_common_data_unit_conversion_pressure = unitconversion(iot_app_common_data_unit_conversion_pressure,UNIT_KPA,iot_app_common_data_pressure_unit_flag);

    
    tolerance_set_change_unit(iot_app_common_data_pressure_unit_flag);
    
    output_comp_control(&m_output);
    
    if(!iot_app_menu_is_active())
    {
        iot_app_common_data_screen_set_brightness(100);
    }
    
}
  
 
//= 2ms任务添加
void iot_app_2ms_task_add(uint32 taskmask)
{
    switch(taskmask)
    {
         case IOT_APP_2MS_TASK_OUTPUT1_CONTROL:{
        iot_app_data_processing_and_output_control_2ms_timer_events |= IOT_APP_2MS_TASK_OUTPUT1_CONTROL;
        }break;
         case IOT_APP_2MS_TASK_OUTPUT2_CONTROL:{
        iot_app_data_processing_and_output_control_2ms_timer_events |= IOT_APP_2MS_TASK_OUTPUT2_CONTROL;
        }break;
        case IOT_APP_2MS_TASK_DISP_PRESSURE:{
        iot_app_data_processing_and_output_control_2ms_timer_events |= IOT_APP_2MS_TASK_DISP_PRESSURE;
        }break;
        case IOT_APP_2MS_TASK_DISP_SECOND_SCREEN:
        {
        iot_app_data_processing_and_output_control_2ms_timer_events |= IOT_APP_2MS_TASK_DISP_SECOND_SCREEN;
        }break;
    }
}
void iot_app_2ms_task_delete(uint32 taskmask)
{
    switch(taskmask)
    {
         case IOT_APP_2MS_TASK_OUTPUT1_CONTROL:{
        iot_app_data_processing_and_output_control_2ms_timer_events &= ~ IOT_APP_2MS_TASK_OUTPUT1_CONTROL;
        }break;
         case IOT_APP_2MS_TASK_OUTPUT2_CONTROL:{
        iot_app_data_processing_and_output_control_2ms_timer_events &= ~ IOT_APP_2MS_TASK_OUTPUT2_CONTROL;
        }break;
        case IOT_APP_2MS_TASK_DISP_PRESSURE:{
        iot_app_data_processing_and_output_control_2ms_timer_events &= ~IOT_APP_2MS_TASK_DISP_PRESSURE;
        }break;
        case IOT_APP_2MS_TASK_DISP_SECOND_SCREEN:
        {
        iot_app_data_processing_and_output_control_2ms_timer_events &= ~IOT_APP_2MS_TASK_DISP_SECOND_SCREEN;
        }break;
    }
}
//= 校准
#if 1
void calculate_linear_segments(float x[], float y[], int n, 
                              float *k1, float *b1,
                              float *k2, float *b2,
                              float *k3, float *b3,
                              float *k4, float *b4,
                              float *k5, float *b5,
                              float *k6, float *b6) {
    //= 验证数据点数量
    if (n < 2) {
        //printf("错误：需要至少2个点进行线性拟合\n");
        return;
    }
    
    //= 计算每两个相邻点之间的线性参数
    for (int i = 0; i < n - 1; i++) {
        float dx = x[i+1] - x[i];
        float dy = y[i+1] - y[i];
        
        //= 避免除以零的情况
        if (dx == 0) {
            //printf("警告：x坐标相同，无法计算斜率（段 %d）\n", i+1);
            continue;
        }
        
        //= 计算斜率和截距
        float ki = dy / dx;
        float bi = y[i] - ki * x[i];
        
        //= 将结果存储到对应的变量
        switch (i) {
            case 0: *k1 = ki; *b1 = bi; break;
            case 1: *k2 = ki; *b2 = bi; break;
            case 2: *k3 = ki; *b3 = bi; break;
            case 3: *k4 = ki; *b4 = bi; break;
            case 4: *k5 = ki; *b5 = bi; break;
            case 5: *k6 = ki; *b6 = bi; break;
            default: break;
        }
    }
}

void linearLeastSquares(float x[], float y[], int n, float *k, float *b) 
{
    float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
 
    for (int i = 0; i < n; i++) {
        sumX += x[i];
        sumY += y[i];
        sumXY += x[i] * y[i];
        sumX2 += x[i] * x[i];
    }
 
    //= 计算斜率 k
    *k = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
 
    //= 计算截距 b
    float meanX = sumX / n;
    float meanY = sumY / n;
    *b = meanY - (*k) * meanX;
}

void reset_calibration() 
{
    received_count = 0;
    memset(y, 0, sizeof(y));
}

uint8 usart_process(void)
{
     if(uartrxbuffer.uartfinish)
    {  
       uartrxbuffer.uartfinish=0; 
        
       testlen=usart_readavailable(testdate,sizeof(testdate));    
        
       printf("%s\r\n",(char *)testdate); //回传接收到的数据

       if (strstr((char *)testdate, "kPaG") == NULL)  
       { 
            return 0;
       }

        char *kpa_pos = strstr((char *)testdate, "kPaG");
        if (kpa_pos == NULL) {
            return 0;
        }
 
        int len = kpa_pos - (char *)testdate;
        if (len <= 0 || len >= MAX_PRESSURE_STR) 
        {
            return 0;
        }
 
        char pressure_str[MAXDATA];
        strncpy(pressure_str, (char *)testdate, len);
        pressure_str[len] = '\0';
 
        float pressure = atof(pressure_str);
 
        if (received_count < NUM_CALIBRATION_POINTS) 
        {
            y[received_count] = pressure;
			x[received_count] = adc_read();
			received_count++;
        }
 
        if (received_count >= NUM_CALIBRATION_POINTS) 
        {
            return 2;
        }			
    }
    
    return 1;
}


void barometric(void)
{
    uint8 returnflag = 0;
    
    while(!iot_app_common_data_calibration_ok_or_no)  
    {
        returnflag = usart_process(); //接收校准气压数据
    
        if (!returnflag)  	
        {
            printf("格式错误！请重新发送数据。\r\n");
            reset_calibration(); 	
            
            div_disp_by_string(MAIN_SCREEN," E00");
            div_disp_by_string(SECOND_SCREEN," ERR");
        }
        if (returnflag==2) 
        {

            linearLeastSquares(x,y,n,&k,&b);
    
            calculate_linear_segments(  x, y, n, 
                                        &k1, &b1, 
                                        &k2, &b2, 
                                        &k3, &b3, 
                                        &k4, &b4, 
                                        &k5, &b5, 
                                        &k6, &b6    );
            
            
            printf("校准成功！接收到的气压值：\n");
            for (int i = 0; i < NUM_CALIBRATION_POINTS; i++) 
            {
                printf("%d: %.5f kPa %0.f Adc\n", i + 1, y[i],x[i]);
            }
            printf("校准曲线=%fX%f",k,b);  //打印曲线

            iot_app_common_data_calibration_ok_or_no=1;  //校准完成
            
            div_disp_by_string(MAIN_SCREEN,"  OK");
            div_disp_by_string(SECOND_SCREEN,"  OK");
        }
    }
    
    linearLeastSquares(x,y,n,&k,&b);
    
    calculate_linear_segments(  x, y, n, 
                                &k1, &b1, 
                                &k2, &b2, 
                                &k3, &b3, 
                                &k4, &b4, 
                                &k5, &b5, 
                                &k6, &b6    );
    
}
#endif
//= 按键设置应差值
#if 1
//= 加速按键
void iot_app_display_hold_key_set_value_calculate_stage(uint8_t keys, uint16_t *hold_count, uint32_t poll_time)
{
    uint16_t key_hold_count = 0;
    if ((keys & HAL_KEY_LEFT_UP) && hold_count[HAL_KEY_LEFT_UP_INDEX] >= (1000/poll_time))
    {
        //= 长按超过1秒
        key_hold_count = hold_count[HAL_KEY_LEFT_UP_INDEX];
    }

    else if ((keys & HAL_KEY_RIGHT_DOWN) && hold_count[HAL_KEY_RIGHT_DOWN_INDEX] >= (1000/poll_time))
    {
        //= 长按超过1秒
        key_hold_count = hold_count[HAL_KEY_RIGHT_DOWN_INDEX];
    }

    if (!key_hold_count)
    {
        return;
    }

    if (key_hold_count >= (6000/poll_time))
    {
        iot_app_hold_key_set_value_stage_index = 3;
    }
    else if (key_hold_count >= (4000/poll_time))
    {
        iot_app_hold_key_set_value_stage_index = 2;
    }
    else if (key_hold_count >= (2000/poll_time))
    {
        iot_app_hold_key_set_value_stage_index = 1;
    }
    else
    {
        iot_app_hold_key_set_value_stage_index = 0;
    }
}

void tolerance_set_by_short_press(uint8 addordown)
{
    float max_val;
    float increment;
    
    uint8 ifmax = 0;
    
    const paramconfig * cfg;
     
    switch(nowsetwhichyc)
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
    
    if(cfg==NULL){return ;}
    
    float *current_value = cfg->value;
    
    float temp = *current_value;
    
    increment = increment_set[iot_app_common_data_pressure_unit_flag];
    
    float peer_val = 0;
    
     if (cfg->peer_value != NULL) {
        peer_val = *cfg->peer_value;
     }

    max_val = tolerance_max[iot_app_common_data_pressure_unit_flag];
    
    if (addordown) {  

         if (temp < max_val)
        {
            temp += increment; 
            
             if(temp >max_val){temp = max_val;}
        } 
        
        if ((cfg->peer_check == 2) && (cfg->peer_value != NULL)) {// 必须小于
            if(temp >= peer_val)
            {
                temp = peer_val-increment_set[iot_app_common_data_pressure_unit_flag];
                ifmax=1;
            }
        }
        else if (temp >= max_val) 
        {
			temp = max_val;
            ifmax=1;
		}
	} else {
        
        temp -=	increment; 
        
        if ((cfg->peer_check == 1) && (cfg->peer_value != NULL))
        { 
            if(temp <= peer_val)
            {
                temp = peer_val+increment_set[iot_app_common_data_pressure_unit_flag];
                ifmax=2;
            }
        }
		else if (temp < -max_val) 
        {
			temp = -max_val;
            ifmax=2;
		}
	}

    if(ifmax)
    {
        if(ifmax==1)
        {
            float_keep_decimals(temp,3);

            *current_value = temp; 
            
            second_screen_dispaftertime(SECOND_SCREEN_DISP_RETURN_TIME,"  UP");
            
            return ;
        }
        else if(ifmax==2)
        {
            float_keep_decimals(temp,3);

            *current_value = temp; 
            
            second_screen_dispaftertime(SECOND_SCREEN_DISP_RETURN_TIME,"DOWN");
            
            return ;
        }
            
    }
    else
    {
       float_keep_decimals(temp,3);

       *current_value = temp; 
        
       second_screen_disp_float_aftertime(SECOND_SCREEN_DISP_RETURN_TIME,temp);
    }
}

void tolerance_set_by_long_press(uint8 addordown, uint8 coefficient)
{
    float max_val;
    float increment;
     
    uint8 ifmax =0;
    
    const paramconfig * cfg;
     
    switch(nowsetwhichyc)
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
    
    float *current_value = cfg->value;
    
    float temp = *current_value;
    
    increment = increment_set[iot_app_common_data_pressure_unit_flag];
    
    float peer_val = 0;
    
     if (cfg->peer_value != NULL) {
        peer_val = *cfg->peer_value;
       
    }
       
    max_val = tolerance_max[iot_app_common_data_pressure_unit_flag];
    
    if(coefficient)
    {
        increment *= coefficient;
    }
    
    if (addordown) {  
        
        if (temp < max_val)
        {
            temp += increment; 
            
             if(temp >max_val){temp = max_val;}
        }
        
        if ((cfg->peer_check == 2) && (cfg->peer_value != NULL)) {// 必须小于
            if(temp >= peer_val)
            {
                temp = peer_val-increment_set[iot_app_common_data_pressure_unit_flag];
                ifmax=1;
            }
        }
        else if (temp >= max_val) 
        {
			temp = max_val;
            ifmax=1;
		}
        
        
//		if (temp < max_val) {
//			temp += increment; 
//            if(temp >max_val){temp = max_val;}
//		} else {
//			temp = max_val;
//            ifmax=1;
//		}
        
        
	} else {
        
        if (temp > -max_val)
        {
            temp -=	increment; 
            
            if(temp < -max_val){temp = -max_val;}
        }
        
        if ((cfg->peer_check == 1) && (cfg->peer_value != NULL))
        { 
            if(temp <= peer_val)
            {
                temp = peer_val+increment_set[iot_app_common_data_pressure_unit_flag];
                ifmax=2;
            }
        }
		else if (temp < -max_val) 
        {
			temp = -max_val;
            ifmax=2;
		}
        
        
//		if (temp > -max_val) {
//			temp -=	increment; 
//            if(temp < -max_val){temp = -max_val;}
//		} else {
//			temp = -max_val;
//            ifmax=2;
//		}
	}

    if(ifmax)
    {
        if(ifmax==1)
        {
            float_keep_decimals(temp,3);

            *current_value = temp; 
            
            second_screen_dispaftertime(SECOND_SCREEN_DISP_RETURN_TIME,"  UP");
            
            return ;
        }
        else if(ifmax==2)
        {
            float_keep_decimals(temp,3);

            *current_value = temp; 
            
            second_screen_dispaftertime(SECOND_SCREEN_DISP_RETURN_TIME,"DOWN");
            
            return ;
        }
            
    }
    else
    {
       float_keep_decimals(temp,3);

       *current_value = temp; 
        
       second_screen_disp_float_aftertime(SECOND_SCREEN_DISP_RETURN_TIME,temp);
    }
}
void tolerance_set_change_unit(uint8 nowunit)
{
    static uint8 save_unit =0;
    
    static uint8 first_save = 0;
    
    if(!(first_save++))
    {
         save_unit = nowunit;
        
         return ;
    }

    
    if(save_unit == nowunit)
    {
        return ;
    }
    
    if(save_unit != nowunit)
    { 

        iot_app_common_data_tolerance_P1 = unitconversiontolerance(iot_app_common_data_tolerance_P1,save_unit,nowunit);
        iot_app_common_data_tolerance_P2 = unitconversiontolerance(iot_app_common_data_tolerance_P2,save_unit,nowunit);
        iot_app_common_data_tolerance_Hi1 = unitconversiontolerance(iot_app_common_data_tolerance_Hi1,save_unit,nowunit);
        iot_app_common_data_tolerance_Lo1 = unitconversiontolerance(iot_app_common_data_tolerance_Lo1,save_unit,nowunit);
        iot_app_common_data_tolerance_Hi2 = unitconversiontolerance(iot_app_common_data_tolerance_Hi2,save_unit,nowunit);
        iot_app_common_data_tolerance_Lo2 = unitconversiontolerance(iot_app_common_data_tolerance_Lo2,save_unit,nowunit);
             
        save_unit = nowunit;
    }
}
#endif
//= 副屏显示
#if 1
//= 保存插入前的显示
void second_screen_savestatus(management_of_secondscreen* temp,float value,char* data)
{
    if(value != NULL)
    {
        temp->second_screen_disp_float_save = value;
    }
    
    if(data != NULL)
    {
        temp->second_screen_disp_save = data;
    }
}
//= 回退插入前的显示
void second_screen_returnstatus(management_of_secondscreen* temp)
{
    switch(iot_app_common_data_subsidiary_display_flag)
    {
        case SECOND_SCREEN_DISP_TOLERANCE:{
            
            second_screen_disp_float_by_unit(iot_app_common_data_pressure_unit_flag,
                    temp->second_screen_disp_float_save);
            
        }break;
         case SECOND_SCREEN_DISP_NOW_UNIT:{
           
            second_screen_disp_clear_point();
             
             div_disp_by_string(SECOND_SCREEN,temp->second_screen_disp_save);
             
        }break;
        case SECOND_SCREEN_DISP_OFF:{
            
            second_screen_disp_clear_point();
            
            div_disp_by_string(SECOND_SCREEN,"    ");
            
        }break;
        case SECOND_SCREEN_DISP_NO:{
            
            second_screen_disp_clear_point();
            
            div_disp_out2andpoint(P7, 1);

            div_disp_by_string(SECOND_SCREEN,temp->second_screen_disp_save);
            
        }break;
        
    }
   
}


//= 插入显示字符
void second_screen_dispaftertime(uint16_t time,const char * data1,...)
{
    static char pre_data[9]={0};
    
	char data[9]={0};
	int size = sizeof(data);
	va_list ps;
	va_start(ps,data1);
	vsnprintf(data,size,data1,ps);	
	va_end(ps);   

    if(strcmp(data,pre_data)!=0)
    {
        strcpy(pre_data,data);
        
        div_disp_by_string(SECOND_SCREEN,"    ");
    }
    
    second_screen_disp_clear_point();
	div_disp_by_string(SECOND_SCREEN,data);
    
    osal_start_timerEx(iot_app_data_processing_and_output_control_task_id,
    IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_RECOVER_SECONDSCREEN_DISP_TIMER_EVT,
    time);
    
}
//= 插入显示浮点
void second_screen_disp_float_aftertime(uint16 time , float value)
{
    second_screen_disp_float_by_unit(iot_app_common_data_pressure_unit_flag,value);
    
    osal_start_timerEx(iot_app_data_processing_and_output_control_task_id,
    IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_RECOVER_SECONDSCREEN_DISP_TIMER_EVT,
    time);
}

//= 刷新当前设定
void flash_now_set(void)
{
    get_now_yc_set();

    choice=0;
    
    choice%=NOWChoicelength;
    
    nowsetwhichyc=NOWChoice[choice];
}
//= 循环选择副屏的可修改值
void get_now_yc_set(void) 					
{
	//选择模式
	if(iot_app_common_data_output1_flag==IOT_APP_COMMON_DATA_OUTPUT1_EASY)	            {current_out1_mode=OT1_EASY;} 
	if(iot_app_common_data_output1_flag==IOT_APP_COMMON_DATA_OUTPUT1_WINDOW_COMPARE)	{current_out1_mode=OT1_WCMP;}	
	if(iot_app_common_data_output1_flag==IOT_APP_COMMON_DATA_OUTPUT1_HYSTERESIS)	    {current_out1_mode=OT1_HSY;}			

	if(iot_app_common_data_output2_flag==IOT_APP_COMMON_DATA_OUTPUT2_EASY)	            {current_out2_mode=OT2_EASY;} 
	if(iot_app_common_data_output2_flag==IOT_APP_COMMON_DATA_OUTPUT2_WINDOW_COMPARE)	{current_out2_mode=OT2_WCMP;}	
	if(iot_app_common_data_output2_flag==IOT_APP_COMMON_DATA_OUTPUT2_HYSTERESIS)	    {current_out2_mode=OT2_HSY;}
	if(iot_app_common_data_output2_flag==IOT_APP_COMMON_DATA_OUTPUT2_OFF)	            {current_out2_mode=OT2_OFF;}

	nowsetwhichyc=0;
	NOWChoicelength = 0;

	int i=0; 
	const uint8_t param_masks[6] = {set_P1, set_P2, set_Hi1, set_Lo1, set_Hi2, set_Lo2};
	for(i=0;i<6;i++)
	{
		if(param_editable[current_out1_mode][current_out2_mode][i])
		{
			nowcansetwhich |= param_masks[i];          // 更新位掩码
			NOWChoice[NOWChoicelength++] = param_masks[i]; // 存储宏定义值
		}	
	}

	if(!nowsetwhichyc)
	{
		nowsetwhichyc=NOWChoice[0];
	}
}

//= 各按键按下插入显示后关闭
void mode_press_set_choice_and_display(void)
{
    get_now_yc_set();
    
    choice++;
    choice%=NOWChoicelength;
    nowsetwhichyc=NOWChoice[choice];
    
     switch(nowsetwhichyc)      //显示当前设定值
    {
        case set_P1: {second_screen_dispaftertime(SECOND_SCREEN_DISP_RETURN_TIME," P-1");	}break;
        case set_P2: {second_screen_dispaftertime(SECOND_SCREEN_DISP_RETURN_TIME," P-2");	}break;
        case set_Hi1:{second_screen_dispaftertime(SECOND_SCREEN_DISP_RETURN_TIME,"Hi-1");	}break;
        case set_Lo1:{second_screen_dispaftertime(SECOND_SCREEN_DISP_RETURN_TIME,"Lo-1");	}break;
        case set_Hi2:{second_screen_dispaftertime(SECOND_SCREEN_DISP_RETURN_TIME,"Hi-2");	}break;
        case set_Lo2:{second_screen_dispaftertime(SECOND_SCREEN_DISP_RETURN_TIME,"Lo-2");	}break;
    } 
}
void upordown_press_set_choice_and_display(void)
{
     switch(nowsetwhichyc)      //显示当前设定值
    {
        case set_P1: {second_screen_disp_float_aftertime(SECOND_SCREEN_DISP_RETURN_TIME,iot_app_common_data_tolerance_P1);}break;
        case set_P2: {second_screen_disp_float_aftertime(SECOND_SCREEN_DISP_RETURN_TIME,iot_app_common_data_tolerance_P2);}break;
        case set_Hi1:{second_screen_disp_float_aftertime(SECOND_SCREEN_DISP_RETURN_TIME,iot_app_common_data_tolerance_Hi1);}break;
        case set_Lo1:{second_screen_disp_float_aftertime(SECOND_SCREEN_DISP_RETURN_TIME,iot_app_common_data_tolerance_Lo1);}break;
        case set_Hi2:{second_screen_disp_float_aftertime(SECOND_SCREEN_DISP_RETURN_TIME,iot_app_common_data_tolerance_Hi2);}break;
        case set_Lo2:{second_screen_disp_float_aftertime(SECOND_SCREEN_DISP_RETURN_TIME,iot_app_common_data_tolerance_Lo2);}break;
    } 
}
//=========================================
void second_screen_disp_start(void)
{
   iot_app_2ms_task_add(IOT_APP_2MS_TASK_DISP_SECOND_SCREEN);

}

void second_screen_disp_stop(void)
{
   second_screen_disp_clear_point();
   
   iot_app_2ms_task_delete(IOT_APP_2MS_TASK_DISP_SECOND_SCREEN);
}


void second_screen_disp_now_set_value(void)
{
     float tempvalue = 0; 
    
    switch(nowsetwhichyc)       
    {
        case set_P1:{tempvalue=iot_app_common_data_tolerance_P1; }break;
        case set_P2:{tempvalue=iot_app_common_data_tolerance_P2; }break;
        case set_Hi1:{tempvalue=iot_app_common_data_tolerance_Hi1;}break;
        case set_Lo1:{tempvalue=iot_app_common_data_tolerance_Lo1;}break;
        case set_Hi2:{tempvalue=iot_app_common_data_tolerance_Hi2;}break;
        case set_Lo2:{tempvalue=iot_app_common_data_tolerance_Lo2;}break;
    }
    

    second_screen_savestatus(&m_secondscreen,tempvalue,NULL);
    
    second_screen_disp_float_by_unit(iot_app_common_data_pressure_unit_flag,
            tempvalue);
}
void second_screen_disp_off(void)
{
    second_screen_disp_clear_point();
    
    second_screen_savestatus(&m_secondscreen,NULL,"    "); 
    
    div_disp_by_string(SECOND_SCREEN,"    ");    
}
void second_screen_disp_now_set_unit(void)
{
    second_screen_disp_clear_point();
    
    char * string = NULL;
    
    switch(iot_app_common_data_pressure_unit_flag)
    {
        case UNIT_KPA:
        {
           string = " KPA";
        }break;
        case UNIT_KGF:
        {
            string = " KGF";
        }break;
        case UNIT_BAR:
        {
            string = " bAR";
        }break;
        case UNIT_PSI:
        {
            string = " PSI";
        }break;
        case UNIT_MMHG:
        {
            string = "MMHG";
        }break;
        case UNIT_INHG:
        {
            string = "1NHG";
        }break;
        case UNIT_MPA:
        {
            string = " MPA";
        }break;
    }	
    
    if(string != NULL)
    {
       second_screen_savestatus(&m_secondscreen,NULL,string); 
    }
    
    div_disp_by_string(SECOND_SCREEN,string);
}
void second_screen_disp_now_set_no(void)
{
    char * string = NULL;
    
    static char data [10];
    
    second_screen_disp_clear_point();
    
    snprintf(data,sizeof(data),"No%u%u",iot_app_common_data_data_subsidiary_display_no_left,iot_app_common_data_data_subsidiary_display_no_right);
    
    div_disp_out2andpoint(P7, 1);

    string = data;
    
    div_disp_by_string(SECOND_SCREEN,string);
    
    if(string != NULL)
    {
        second_screen_savestatus(&m_secondscreen,NULL,string); 
    }
}
void second_screen_disp_choice(uint8 choice)
{
    switch(choice)
    {
        case SECOND_SCREEN_DISP_TOLERANCE:{
            iot_app_common_data_subsidiary_display_flag = SECOND_SCREEN_DISP_TOLERANCE;
        }break;
        case SECOND_SCREEN_DISP_NOW_UNIT:{
             iot_app_common_data_subsidiary_display_flag = SECOND_SCREEN_DISP_NOW_UNIT;
        }break;
        case SECOND_SCREEN_DISP_OFF:{
             iot_app_common_data_subsidiary_display_flag = SECOND_SCREEN_DISP_OFF;
        }break;
        case SECOND_SCREEN_DISP_NO:{
             iot_app_common_data_subsidiary_display_flag = SECOND_SCREEN_DISP_NO;
        }break;
    }
}

void second_screen_disp(void)
{
    switch(iot_app_common_data_subsidiary_display_flag)
    {
        case SECOND_SCREEN_DISP_TOLERANCE:{
            second_screen_disp_now_set_value();
        }break;
         case SECOND_SCREEN_DISP_NOW_UNIT:{
           	second_screen_disp_now_set_unit();
        }break;
         case SECOND_SCREEN_DISP_OFF:{
            second_screen_disp_off();
         }break;
        case SECOND_SCREEN_DISP_NO:{
            second_screen_disp_now_set_no();
        }break;
    }
}

//=========================================
void second_screen_disp_clear_point(void)
{
    div_disp_out2andpoint(P8,0);
    div_disp_out2andpoint(P7,0);
    div_disp_out2andpoint(P6,0);
}
void second_screen_disp_float_by_unit(uint8 unit,float value)
{	
	uint8 decimal_places = 0;

    switch(iot_app_common_data_pressure_unit_flag)
	{
		case UNIT_BAR:{
			decimal_places = 3;
		}break;
		case UNIT_INHG:{
			decimal_places = 1;
		}break;
		case UNIT_KGF:{
			decimal_places = 3;
		}break;
		case UNIT_KPA:{
			decimal_places = 1;
		}break;
		case UNIT_MMHG:{
			decimal_places = 0;
		}break;
		case UNIT_MPA:{
			decimal_places = 3;
		}break;
		case UNIT_PSI:{
			decimal_places = 2;
		}break;
	}		
    
    second_screen_dispfloat("%.*f",decimal_places,value);
}

void second_screen_dispfloat(const char * data1,...)
{
    second_screen_disp_clear_point();
    
	char data[20]={0};
	int size = sizeof(data);
	va_list ps;
	va_start(ps,data1);
	vsnprintf(data,size,data1,ps);
	va_end(ps);   
	
	char * string = data;
	
	uint8 isNegative = 0;
	if (data[0] == '-') {
		string++;
		isNegative = 1;
    } 
	
	int len = strlen(data);
	if (len >= 20) return;
	
	int ader =0;	
	int i;			
	for(i=0;i<len;i++)
	{
		if(string[i]=='.')
		{
			ader = i;
		}
	}
    
	if(!ader){
	
		char disp[20] = {0};

		int num = atoi(string);
		
		if(num>=0 && num<10)
		{
			sprintf(disp,"   %s",string);
		}
		else if(num>=10 && num<100)
		{
			sprintf(disp,"  %s",string);
		}
		else if(num>=100 && num<1000)
		{
			sprintf(disp," %s",string);
		}
		if(isNegative)
		{
			disp[0]='-';
		}
		
		if(num>=1000)
		{
			sprintf(disp,"%s",string);
			if(isNegative)
			{
				disp[0]='*';
			}
		}
				
		div_disp_by_string(SECOND_SCREEN,disp);
	
		return;
	}

	char frontdate[10]={0};
	char afterdate[10]={0};
	
	int numfront = 0;
	int countfront = 0;
	
	int numafter = 0;
	int countafter = 0;
		
	uint8_t head2=0;    
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
	
    if(frontdate[0]=='0')
    {
        frontdate[0]=' ';
    }
    
	char disp[20] = {0};

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
				disp[0]='^';
			}
		}
        
		div_disp_out2andpoint(P6,1);
        
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
				disp[0]='^';
			}
		}
        
		div_disp_out2andpoint(P7,1);
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
					disp[0]='^';
				}
			}
		}
		
		div_disp_out2andpoint(P8,1);
	}

	div_disp_by_string(SECOND_SCREEN,disp);
}   

#endif
//= 显示气压
#if 1
void main_screen_disp_clear_point()
{
    div_disp_out1andpoint(P1,0);
    div_disp_out1andpoint(P2,0);
    div_disp_out1andpoint(P3,0);
}
void main_screen_disp_pressure_start(void)
{
    main_screen_disp_float_by_unit(iot_app_common_data_pressure_unit_flag,
        iot_app_common_data_unit_conversion_pressure);
    
   iot_app_2ms_task_add(IOT_APP_2MS_TASK_DISP_PRESSURE);
}

void main_screen_disp_pressure_stop(void)
{    
    iot_app_2ms_task_delete(IOT_APP_2MS_TASK_DISP_PRESSURE);
    
    main_screen_disp_clear_point();
}
    
//= 选择显示速率
uint32 main_screen_disp_speed_choice(uint8 speed)
{
    uint32_t dispspeed = 0;
    
    switch(speed)
    {
        case IOT_APP_COMMON_DATA_DISPLAY_SPEED_250MS:{dispspeed=250;}break;
        case IOT_APP_COMMON_DATA_DISPLAY_SPEED_500MS:{dispspeed=500;}break;
        case IOT_APP_COMMON_DATA_DISPLAY_SPEED_1S   :{dispspeed=1000;}break;
    }
    
    return dispspeed;
}
//= 按单位在主屏显示浮点
void main_screen_disp_float_by_unit(uint8 unit,float value)
{	
	uint8 decimal_places = 0;

    switch(iot_app_common_data_pressure_unit_flag)
	{
		case UNIT_BAR:{
			decimal_places = 3;
		}break;
		case UNIT_INHG:{
			decimal_places = 1;
		}break;
		case UNIT_KGF:{
			decimal_places = 3;
		}break;
		case UNIT_KPA:{
			decimal_places = 1;
		}break;
		case UNIT_MMHG:{
			decimal_places = 0;
		}break;
		case UNIT_MPA:{
			decimal_places = 3;
		}break;
		case UNIT_PSI:{
			decimal_places = 2;
		}break;
	}		
    
    main_screen_dispfloat("%.*f",decimal_places,value);
}

//= 主屏显示浮点数最多三位小数
void main_screen_dispfloat(const char * data1,...)
{
    main_screen_disp_clear_point();
    
	char data[20]={0};
	int size = sizeof(data);
	va_list ps;
	va_start(ps,data1);
	vsnprintf(data,size,data1,ps);
	va_end(ps);   
	
	char * string = data;
	
	uint8 isNegative = 0;
	if (data[0] == '-') {
		string++;
		isNegative = 1;
    } 
	
	int len = strlen(data);
	if (len >= 20) return;
	
	int ader =0;	
	int i;			
	for(i=0;i<len;i++)
	{
		if(string[i]=='.')
		{
			ader = i;
		}
	}
    
	if(!ader){
	
		char disp[20] = {0};

		int num = atoi(string);
		
		if(num>=0 && num<10)
		{
			sprintf(disp,"   %s",string);
		}
		else if(num>=10 && num<100)
		{
			sprintf(disp,"  %s",string);
		}
		else if(num>=100 && num<1000)
		{
			sprintf(disp," %s",string);
		}
		if(isNegative)
		{
			disp[0]='-';
		}
		
		if(num>=1000)
		{
			sprintf(disp,"%s",string);
			if(isNegative)
			{
				disp[0]='*';
			}
		}
				
		div_disp_by_string(MAIN_SCREEN,disp);
	
		return;
	}

	char frontdate[10]={0};
	char afterdate[10]={0};
	
	int numfront = 0;
	int countfront = 0;
	
	int numafter = 0;
	int countafter = 0;
		
	uint8_t head2=0;    
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
	
    if(frontdate[0]=='0')
    {
        frontdate[0]=' ';
    }
    
	char disp[20] = {0};

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
				disp[0]='^';
			}
		}
        
		div_disp_out1andpoint(P3,1);
        
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
				disp[0]='^';
			}
		}
        
		div_disp_out1andpoint(P2,1);
        
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
					disp[0]='^';
				}
			}
		}
		
		div_disp_out1andpoint(P1,1);
	}
	
	div_disp_by_string(MAIN_SCREEN,disp);
}   
    



#endif
//= 输出控制
#if 1
void out_put_nonc_set_output_flashonce(void)
{
    if(m_output.output_out1_state==1)
    {
        out_put_nonc_set_output(OUT1,1);
    }
    else
    {
        out_put_nonc_set_output(OUT1,0);
    }
        
        
    if(m_output.output_out2_state==1)
    {
        out_put_nonc_set_output(OUT2,1);
    }
    else
    {
        out_put_nonc_set_output(OUT2,0);
    }
}
void out_put_nonc_set_output(uint8 output_set_which,uint8 output_en)
{
    uint8 output1_noncstate = 0;
    uint8 output2_noncstate = 0;
    
    if (iot_app_common_data_output2_flag)
    {
        switch (iot_app_common_data_nonc_output_flag)
        {
            case OUTPUT_NONCSTATE_1NO2NO:
            {
                output1_noncstate = NO;
                output2_noncstate = NO;
            }break;
            case OUTPUT_NONCSTATE_1NC2NC:
            {
                output1_noncstate = NC;
                output2_noncstate = NC;
            }break;
            case OUTPUT_NONCSTATE_1NO2NC:
            {
                output1_noncstate = NO;
                output2_noncstate = NC;
            }break;
            case OUTPUT_NONCSTATE_1NC2NO:
            {
                output1_noncstate = NC;
                output2_noncstate = NO;
            }break;
            default:break;
        }
        
        if(output_set_which==OUT1)
        {
             if(output1_noncstate)
            {
                if(output_en)
                {
                    HAL_OUT1_ON();
                    
                    div_disp_out1andpoint(S1,1);
                }
                else
                {
                    HAL_OUT1_OFF();
                    
                    div_disp_out1andpoint(S1,0);
                }
            }
            else
            {
                if(output_en)
                {
                    HAL_OUT1_OFF();
                    
                    div_disp_out1andpoint(S1,0);
                }
                else
                {
                    HAL_OUT1_ON();
                    
                    div_disp_out1andpoint(S1,1);
                }
            }
        }
        
        
        if(output_set_which==OUT2)
        {
             if(output2_noncstate)
            {
                if(output_en)
                {
                    HAL_OUT2_ON();
                    
                    div_disp_out2andpoint(S2,1);
                }
                else
                {
                    HAL_OUT2_OFF();
                    
                    div_disp_out2andpoint(S2,0);
                }
            }
            else
            {
                if(output_en)
                {
                    HAL_OUT2_OFF();
                    
                    div_disp_out2andpoint(S2,0);
                }
                else
                {
                    HAL_OUT2_ON();
                    
                    div_disp_out2andpoint(S2,1);
                }
            }
        }
    }
    else
    {
        switch (iot_app_common_data_nonc_output_flag)
        {
            case OUTPUT_NONCSTATE_1NO:
            {
                output1_noncstate = NO;
            }break;
            case OUTPUT_NONCSTATE_1NC:
            {
                output1_noncstate = NC;
            }break;
            default:break;
        }
        
       if(output_set_which==OUT1)
       {
             if(output1_noncstate)
            {
                if(output_en)
                {
                    HAL_OUT1_ON();
                    
                    
                    div_disp_out1andpoint(S1,1);
                }
                else
                {
                    HAL_OUT1_OFF();
                    
                    
                    div_disp_out1andpoint(S1,0);
                }
            }
            else
            {
                if(output_en)
                {
                    HAL_OUT1_OFF();
                    
                    
                    div_disp_out1andpoint(S1,0);
                }
                else
                {
                    HAL_OUT1_ON();
                    
                    
                    div_disp_out1andpoint(S1,1);
                }
            }
        }
    }
}
void out_put_react_time_set_output(uint8 out1orout2 ,uint8 output_set_state, management_of_output* temp)
{

    if(out1orout2 == OUTPUT_SET_OUT1)
    {
         if(temp->output_out1_state != output_set_state)
         {
             output_timer_set_out1_which_out = output_set_state;
             
             if(output1_timer_finish_flag)
            {
                output1_timer_finish_flag = 0;
                iot_app_2ms_task_add(IOT_APP_2MS_TASK_OUTPUT1_CONTROL);
            }   
         }
    }
    if(out1orout2 == OUTPUT_SET_OUT2)
    {  
         if(temp->output_out2_state != output_set_state)
         {
             output_timer_set_out2_which_out = output_set_state;
             
             if(output2_timer_finish_flag)
            {
                output2_timer_finish_flag = 0;
                iot_app_2ms_task_add(IOT_APP_2MS_TASK_OUTPUT2_CONTROL);
            }    
         } 
    }
  
    
}
void out_put_react_time_choice(uint8 reactime ,management_of_output* temp)
{
    uint32_t reactiontimeset = 0;
    
	switch(reactime)
	{
		case REACT_TIME_2MS	  	:{reactiontimeset=2;	}break;
        case REACT_TIME_5MS	    :{reactiontimeset=5;	}break;
		case REACT_TIME_10MS   	:{reactiontimeset=10;	}break;
        case REACT_TIME_50MS  	:{reactiontimeset=50;	}break;
		case REACT_TIME_100MS   :{reactiontimeset=100;	}break;
		case REACT_TIME_250MS	:{reactiontimeset=250;	}break;
		case REACT_TIME_500MS	:{reactiontimeset=500;	}break;
		case REACT_TIME_1000MS  :{reactiontimeset=1000;	}break;   
        case REACT_TIME_5000MS  :{reactiontimeset=5000;	}break;
	}
    
    temp->output_react_time = reactiontimeset;
}

void out_put_diflevel_choice(uint8 level,uint8 units ,management_of_output* temp)
{
    float resolution_ratio = 0;
    
    float coefficient = 0;
    
    switch(units)
    {
        case UNIT_BAR	:{resolution_ratio=0.001;}break;
        case UNIT_INHG	:{resolution_ratio=0.1;}break;
        case UNIT_KGF	:{resolution_ratio=0.001;}break;
        case UNIT_KPA	:{resolution_ratio=0.1;}break;
        case UNIT_MMHG	:{resolution_ratio=1.0;}break;
        case UNIT_MPA	:{resolution_ratio=0.001;}break;
        case UNIT_PSI	:{resolution_ratio=0.01;}break;
    }    
    
	 switch(level)     
	{
		case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_1:{coefficient = 1;}break;
		case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_2:{coefficient = 2;}break;
		case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_3:{coefficient = 3;}break;
		case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_4:{coefficient = 4;}break;
		case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_5:{coefficient = 5;}break;
		case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_6:{coefficient = 6;}break;
		case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_7:{coefficient = 7;}break;
		case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_8:{coefficient = 8;}break;
    }
    
    temp->output_tolerance = coefficient * resolution_ratio;
}


void output_comp_control(management_of_output* temp)  //应差比较
{
    // 应差值
    float tolerance_value=0.0f;		
    
	// 延迟时间选中		
	out_put_react_time_choice(iot_app_common_data_response_time_flag,temp);		
    
	// 应差值选中
	out_put_diflevel_choice(iot_app_common_data_hysteresis_level,
    iot_app_common_data_pressure_unit_flag,
    temp);			
    
    tolerance_value = temp->output_tolerance;
        
    // 输出1判断
	switch(iot_app_common_data_output1_flag)
	{
		case IOT_APP_COMMON_DATA_OUTPUT1_EASY:{
			switch(temp->output_out1_state)
			{
				case 0:{
					if(iot_app_common_data_unit_conversion_pressure>
                         (iot_app_common_data_tolerance_P1+tolerance_value))//大于应差才输出
                    {
                         out_put_react_time_set_output(OUTPUT_SET_OUT1,OUTPUT_STATE_ON,temp);
					}
                    else
                    {
                        output1_timer_finish_flag = 1;
             
                        out1_timer_count = 0;
                         
                        iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT1_CONTROL);
                    }
				}break;
				case 1:{   
					if(iot_app_common_data_unit_conversion_pressure<
                        (iot_app_common_data_tolerance_P1))
					{
						out_put_react_time_set_output(OUTPUT_SET_OUT1,OUTPUT_STATE_OFF,temp);
					}
                    else
                    {
                        output1_timer_finish_flag = 1;
             
                        out1_timer_count = 0;
                         
                        iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT1_CONTROL);
                    }
				}break;
			}
        }break;
        case IOT_APP_COMMON_DATA_OUTPUT1_HYSTERESIS:{
            switch(temp->output_out1_state)
            {
                case 0:{
                    if(iot_app_common_data_unit_conversion_pressure>(iot_app_common_data_tolerance_Hi1))
                    {
                        out_put_react_time_set_output(OUTPUT_SET_OUT1,OUTPUT_STATE_ON,temp);	
                    }
                    else
                    {
                        output1_timer_finish_flag = 1;
             
                        out1_timer_count = 0;
                         
                        iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT1_CONTROL);
                    }
                }break;
                case 1:{
                    if(iot_app_common_data_unit_conversion_pressure<(iot_app_common_data_tolerance_Lo1))
                    {
                        out_put_react_time_set_output(OUTPUT_SET_OUT1,OUTPUT_STATE_OFF,temp);
                    }
                    else
                    {
                        output1_timer_finish_flag = 1;
             
                        out1_timer_count = 0;
                         
                        iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT1_CONTROL);
                    }
                }break;
            } 
        }break;
        case IOT_APP_COMMON_DATA_OUTPUT1_WINDOW_COMPARE:{
             switch(temp->output_out1_state)
            {
                case 0:{
                   if(iot_app_common_data_unit_conversion_pressure<(iot_app_common_data_tolerance_Hi1+tolerance_value)
                       &&iot_app_common_data_unit_conversion_pressure>(iot_app_common_data_tolerance_Lo1+tolerance_value))
                    {
                            out_put_react_time_set_output(OUTPUT_SET_OUT1,OUTPUT_STATE_ON,temp);	
                    }
                    else
                    {
                        output1_timer_finish_flag = 1;
             
                        out1_timer_count = 0;
                         
                        iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT1_CONTROL);
                    }
                }break;
                case 1:{
                     if(iot_app_common_data_unit_conversion_pressure>(iot_app_common_data_tolerance_Hi1+tolerance_value))
                    {
                            out_put_react_time_set_output(OUTPUT_SET_OUT1,OUTPUT_STATE_OFF,temp);
                    }
                    else if(iot_app_common_data_unit_conversion_pressure<iot_app_common_data_tolerance_Lo1)
                    {
                            out_put_react_time_set_output(OUTPUT_SET_OUT1,OUTPUT_STATE_OFF,temp);
                    }
                    else
                    {
                        output1_timer_finish_flag = 1;
             
                        out1_timer_count = 0;
                         
                        iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT1_CONTROL);
                    }
                }break;
            }
		}break;
    }
        
    
    switch(iot_app_common_data_output2_flag)
	{
		case IOT_APP_COMMON_DATA_OUTPUT2_EASY:{
			switch(temp->output_out2_state)
			{
				case 0:{
					if(iot_app_common_data_unit_conversion_pressure>
                         (iot_app_common_data_tolerance_P2+tolerance_value))//大于应差才输出
					{
                         out_put_react_time_set_output(OUTPUT_SET_OUT2,OUTPUT_STATE_ON,temp);
					}
                    else
                    {
                        output2_timer_finish_flag = 1;
             
                        out2_timer_count = 0;
                         
                        iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT2_CONTROL);
                    }
				}break;
				case 1:{   
					if(iot_app_common_data_unit_conversion_pressure<
                        (iot_app_common_data_tolerance_P2))
					{
						out_put_react_time_set_output(OUTPUT_SET_OUT2,OUTPUT_STATE_OFF,temp);
					}
                    else
                    {
                        output2_timer_finish_flag = 1;
             
                        out2_timer_count = 0;
                         
                        iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT2_CONTROL);
                    }
				}break;
			}
		}break;
        case IOT_APP_COMMON_DATA_OUTPUT2_HYSTERESIS:{
            switch(temp->output_out2_state)
            {
                case 0:{
                    if(iot_app_common_data_unit_conversion_pressure>(iot_app_common_data_tolerance_Hi2))
                    {
                        out_put_react_time_set_output(OUTPUT_SET_OUT2,OUTPUT_STATE_ON,temp);	
                    }
                    else
                    {
                        output2_timer_finish_flag = 1;
             
                        out2_timer_count = 0;
                         
                        iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT2_CONTROL);
                    }
                }break;
                case 1:{
                    if(iot_app_common_data_unit_conversion_pressure<(iot_app_common_data_tolerance_Lo2))
                    {
                        out_put_react_time_set_output(OUTPUT_SET_OUT2,OUTPUT_STATE_OFF,temp);
                    }
                    else
                    {
                        output2_timer_finish_flag = 1;
             
                        out2_timer_count = 0;
                         
                        iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT2_CONTROL);
                    }
                }break;
            } 
        }break;
        case IOT_APP_COMMON_DATA_OUTPUT2_WINDOW_COMPARE:{
             switch(temp->output_out2_state)
            {
                case 0:{
                   if(iot_app_common_data_unit_conversion_pressure<(iot_app_common_data_tolerance_Hi2+tolerance_value)
                       &&iot_app_common_data_unit_conversion_pressure>(iot_app_common_data_tolerance_Lo2+tolerance_value))
                    {
                            out_put_react_time_set_output(OUTPUT_SET_OUT2,OUTPUT_STATE_ON,temp);	
                    }
                    else
                    {
                        output2_timer_finish_flag = 1;
             
                        out2_timer_count = 0;
                         
                        iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT2_CONTROL);
                    }
                }break;
                case 1:{
                     if(iot_app_common_data_unit_conversion_pressure>(iot_app_common_data_tolerance_Hi2+tolerance_value))
                    {
                            out_put_react_time_set_output(OUTPUT_SET_OUT2,OUTPUT_STATE_OFF,temp);
                    }
                    else if(iot_app_common_data_unit_conversion_pressure<iot_app_common_data_tolerance_Lo2)
                    {
                            out_put_react_time_set_output(OUTPUT_SET_OUT2,OUTPUT_STATE_OFF,temp);
                    }
                    else
                    {
                        output2_timer_finish_flag = 1;
             
                        out2_timer_count = 0;
                         
                        iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT2_CONTROL);
                    }
                }break;
            }
		}break;
        case IOT_APP_COMMON_DATA_OUTPUT2_OFF:{
            
           HAL_OUT2_OFF();//= 无视nonc强制关闭
            
           div_disp_out2andpoint(S2,0);
            
           m_output.output_out2_state = OUTPUT_STATE_OFF;
                    
           output2_timer_finish_flag = 1;
            
           iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT2_CONTROL);
            
        }break;
    }
}

#endif
//= 气压数据
#if 1
//= 返回当前气压值 kpa
float pressure_read_once(void)
{
	float tempvalue=0;
    
    uint32_t adcvalue = 0;
	
    if(adc_if_ready())
    {
       adcvalue = adc_read();
    }
   
    tempvalue=(float)adcvalue*k+b;
    
    #if 1
        
    if(adcvalue<x[6])
    {
        tempvalue=(float)adcvalue*k6+b6;
    }
    if(adcvalue>x[6]&&adcvalue<=x[5])
    {
        tempvalue=(float)adcvalue*k6+b6;
    }
    if(adcvalue>x[5]&&adcvalue<=x[4])
    {
        tempvalue=(float)adcvalue*k5+b5;
    }
    if(adcvalue>x[4]&&adcvalue<=x[3])
    {
        tempvalue=(float)adcvalue*k4+b4;
    }
    if(adcvalue>x[3]&&adcvalue<=x[2])
    {
        tempvalue=(float)adcvalue*k3+b3;
    }
    if(adcvalue>x[2]&&adcvalue<=x[1])
    {
        tempvalue=(float)adcvalue*k2+b2;
    }
    if(adcvalue>x[1]&&adcvalue<=x[0])
    {
        tempvalue=(float)adcvalue*k1+b1;
    }
    if(adcvalue>x[0])
    {
        tempvalue=(float)adcvalue*k1+b1;
    }
    #endif
    
	return float_keep_decimals(tempvalue,1);
}

//= 保留多少位小数
float float_keep_decimals(float value,uint8 decimal_places)
{
	char data[50]={0};
	
	snprintf(data,sizeof(data),"%0.*f", decimal_places, value);
	
	return atof(data);
}

//= 单位转换函数
float unitconversiontolerance(float value, uint8_t oldunit, uint8 dstunit)
{
	float conversion = 0;
	
	uint8 decimal_places = 0;
    
    const paramconfig * cfg;
    
    if(oldunit == dstunit) return value;
    
     switch(dstunit)
	{
		case UNIT_BAR:{
			decimal_places = 3;
		}break;
		case UNIT_INHG:{
			decimal_places = 1;
		}break;
		case UNIT_KGF:{
			decimal_places = 3;
		}break;
		case UNIT_KPA:{
			decimal_places = 1;
		}break;
		case UNIT_MMHG:{
			decimal_places = 0;
		}break;
		case UNIT_MPA:{
			decimal_places = 3;
		}break;
		case UNIT_PSI:{
			decimal_places = 2;
		}break;
	}		
     switch(nowsetwhichyc)
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
    
    if(cfg==NULL){return 0;}
    
    

    conversion  = unit_conversion[oldunit];    
    
    value /= conversion;
    
    value = float_keep_decimals(value,1);
    
    conversion  = unit_conversion[dstunit]; 
    
    value *= conversion;
    
    
    
    float max_val = 0;
    
    max_val = tolerance_max[iot_app_common_data_pressure_unit_flag];
    
    if (value > max_val) 
    {
        value = max_val;
    }
    else if (value < -max_val)
    {
        value = -max_val;
    }
	
	return float_keep_decimals(value,decimal_places);
}


float unitconversion(float value, uint8_t oldunit, uint8 dstunit)
{
	float conversion = 0;
	
	uint8 decimal_places = 0;
    
     const paramconfig * cfg;
    
    if(oldunit == dstunit) return value;
    
    switch(dstunit)
	{
		case UNIT_BAR:{
			decimal_places = 3;
		}break;
		case UNIT_INHG:{
			decimal_places = 1;
		}break;
		case UNIT_KGF:{
			decimal_places = 3;
		}break;
		case UNIT_KPA:{
			decimal_places = 1;
		}break;
		case UNIT_MMHG:{
			decimal_places = 0;
		}break;
		case UNIT_MPA:{
			decimal_places = 3;
		}break;
		case UNIT_PSI:{
			decimal_places = 2;
		}break;
	}		
    
   
    
     switch(nowsetwhichyc)
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
    
    if(cfg==NULL){return 0;}
    

    conversion  = unit_conversion[oldunit];    
    
    value /= conversion;
    
    value = float_keep_decimals(value,1);

    conversion  = unit_conversion[dstunit]; 
    
    value *= conversion;
    
    
    
	return float_keep_decimals(value,decimal_places);
}

#endif
//= 应差设置按键回调
uint8_t iot_app_data_processing_and_output_control_common_key_callback(uint8_t cur_keys, uint8_t click_keys, uint8_t hold_keys, uint8_t release_keys, uint32_t poll_time_milliseconds)
{ 
    if(iot_app_data_system_state != IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL)
    {
        return 0;
    }

    // 截断应差比较值的长按设置，避免与校准功能的触发起冲突
    if ((hold_keys & (HAL_KEY_LEFT_UP | HAL_KEY_RIGHT_DOWN)) && 
        (hal_key_press_time_count[HAL_KEY_LEFT_UP_INDEX] >= (500/poll_time_milliseconds)) &&
        (hal_key_press_time_count[HAL_KEY_RIGHT_DOWN_INDEX] >= (500/poll_time_milliseconds)) &&
        !(cur_keys & HAL_KEY_MODE))
    {
        return 0;
    }

    static uint8 hold_flags;
    
    static uint8 pre_keys;
    
    if (hold_keys)
    {
        if(!hold_flags)//= 存放需要自锁的长按
        {
            hold_flags=1;
        }
        
        if(
            (hold_keys & (HAL_KEY_LEFT_UP|HAL_KEY_RIGHT_DOWN))
            &&((hal_key_press_time_count[HAL_KEY_LEFT_UP_INDEX] > (1000/poll_time_milliseconds))||(hal_key_press_time_count[HAL_KEY_RIGHT_DOWN_INDEX] > (1000/poll_time_milliseconds)))
            &&(!(hold_keys & HAL_KEY_MODE))
        )
        {
            if(!IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_IS_ACTIVE())
            {
                IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_START();
            }
            
            iot_app_display_hold_key_set_value_calculate_stage(hold_keys, hal_key_press_time_count, poll_time_milliseconds);
            
            return 1;
        }
    }
    else
    {
        hold_flags =0;
    }
    if(click_keys)
    {
        if ((click_keys & HAL_KEY_MODE) &&  
         !(cur_keys & (HAL_KEY_RIGHT_DOWN|HAL_KEY_LEFT_UP)))
        {
            mode_press_set_choice_and_display();
            
             return 1;
        }   
        if(click_keys & (HAL_KEY_LEFT_UP|HAL_KEY_RIGHT_DOWN))
        {
             if (click_keys & HAL_KEY_LEFT_UP)
            {
               tolerance_set_by_short_press(PRESS_ADD);
                
                return 1;
            }
            else if (click_keys & HAL_KEY_RIGHT_DOWN)
            {
               tolerance_set_by_short_press(PRESS_DOWN);
                
                return 1;
            }
        }
    }
    if(release_keys)
    {
         if (release_keys & (HAL_KEY_LEFT_UP|HAL_KEY_RIGHT_DOWN))
        {
            iot_app_hold_key_set_value_stage_index=0;
            IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_STOP();
            return 1;
        }
    }
    
    if(pre_keys != cur_keys)
    {
        pre_keys = cur_keys;
    }
    
    
    return 0;
}

uint8_t iot_app_data_processing_and_output_control_key_callback(uint8_t cur_keys, uint8_t pre_keys, uint32_t poll_time_milliseconds)
{
    uint8_t  k;
    uint8_t  key_mask = HAL_KEY_1;
    uint8_t  scan_flag = 1;
    uint8_t  click_keys = 0;     
    uint8_t  hold_keys = 0;     
    uint8_t  release_keys = 0;

    cur_keys &= IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H_KEY_MASK;
    pre_keys &= IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H_KEY_MASK;

    for (k = 0; k < 8; k++,key_mask<<=1)
    {
        IOT_WATCHDOG_RESET();

        if (!(key_mask & IOT_APP_BAROMETER_DISP_KEY_MASK))
        {
            continue;
        }
        
        if (cur_keys & key_mask)
        {
            if (hal_key_press_time_count[k] == 2)
            {

            }
            else if (hal_key_press_time_count[k] >= (1000/poll_time_milliseconds)) 
            {
                hold_keys |= key_mask;
            }
        }
        else
        {
            if ((pre_keys & key_mask) && 
                (hal_key_press_time_count[k] > (100/poll_time_milliseconds)) &&
                (hal_key_press_time_count[k] < (1000/poll_time_milliseconds)))
            {
                click_keys |= key_mask;
            }

            if ((pre_keys & key_mask) && (hal_key_press_time_count[k] >= 2))
            {
                release_keys |= key_mask;
            }
        }
    }

    return scan_flag;
}

uint16_t iot_app_data_processing_and_output_control_process_event(uint8_t task_id, uint16_t events)
{
    (void)task_id; 


    if(events & IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_RECOVER_SECONDSCREEN_DISP_TIMER_EVT)
    {
        if (iot_app_menu_is_active()||iot_app_data_system_state != IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL)
        {
            osal_stop_timerEx(iot_app_data_processing_and_output_control_task_id,
            IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_RECOVER_SECONDSCREEN_DISP_TIMER_EVT);
            return (events ^ IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_RECOVER_SECONDSCREEN_DISP_TIMER_EVT);
        }
        
        switch(iot_app_common_data_subsidiary_display_flag)
        {
            case SECOND_SCREEN_DISP_TOLERANCE:{
                
                static uint8 set_save = set_no;
        
                if((set_save == nowsetwhichyc)||(set_save == set_no))
                {
                    //second_screen_returnstatus(&m_secondscreen);
                    
                    second_screen_disp_now_set_value();
                }
                
                
                if(set_save != nowsetwhichyc)
                {
                    set_save = nowsetwhichyc;
                    
                    second_screen_disp_now_set_value();
                }
                
            }break;
             case SECOND_SCREEN_DISP_NOW_UNIT:{
               
                second_screen_returnstatus(&m_secondscreen);
                 
            }break;
            case SECOND_SCREEN_DISP_OFF:{
                
                second_screen_returnstatus(&m_secondscreen);
                
            }break;
             case SECOND_SCREEN_DISP_NO:{
                second_screen_returnstatus(&m_secondscreen);
            }break;
        }
        
        
        
        
        return (events ^ IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_RECOVER_SECONDSCREEN_DISP_TIMER_EVT);
    }
    
    
   
    if(events & IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_TIMER_EVT)
    {
        static uint32_t count = 0;
       
        if(iot_app_data_processing_and_output_control_event_set_value_flag==IOT_APP_DATA_SET_TOLERANCE)
        {
            if (iot_app_menu_is_active()||iot_app_data_system_state != IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL)
            {
                iot_app_hold_key_set_value_stage_index=0;
                IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_STOP();
                return (events ^ IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_TIMER_EVT);
            }
            
            if ((hal_key_press_time_count[HAL_KEY_LEFT_UP_INDEX]  >= (1000/hal_key_scan_timeout)) ||
                (hal_key_press_time_count[HAL_KEY_RIGHT_DOWN_INDEX] >= (1000/hal_key_scan_timeout)))
            {
                if (++count > iot_app_hold_key_set_value_stage_speed[iot_app_hold_key_set_value_stage_index])
                {
                    count = 0;
                    
                    uint8 coefficient_temp = 0;
                    
                    switch(iot_app_hold_key_set_value_stage_index)
                    {
                        case 0:{
                            coefficient_temp = 1;
                        }break;
                        case 1:{
                            coefficient_temp = 1;
                        }break;
                        case 2:{
                            coefficient_temp = 2;
                        }break;
                        case 3:{
                            coefficient_temp = 4;
                        }break;
                    }
                    
                    if (hal_key_press_time_count[HAL_KEY_LEFT_UP_INDEX] >= (1000/hal_key_scan_timeout))//-- add按键长按
                    {
                        tolerance_set_by_long_press(PRESS_ADD,coefficient_temp);
                    }
                    else//-- sub按键长按
                    {
                        tolerance_set_by_long_press(PRESS_DOWN,coefficient_temp);
                    }
                }
            }
            else
            {
                count = 0;
            }
        }
         
       // return(events ^ IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_TIMER_EVT);
    }
   
    if(events & IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_2MS_TIMER_EVT)
    {
        if(iot_app_data_processing_and_output_control_2ms_timer_events & IOT_APP_2MS_TASK_DISP_PRESSURE)
        {     
            static uint16 timer_count = 0;
        
            uint32 dispspeed =  main_screen_disp_speed_choice(iot_app_common_data_display_speed_flag);
           
            
            if (timer_count < dispspeed/3) 
            {
                timer_count++;
            } 
            else 
            {
              if(!iot_app_menu_is_active())
              { 
              
                  float max = tolerance_max[iot_app_common_data_pressure_unit_flag];
                  
                  if((iot_app_common_data_unit_conversion_pressure>max)||(iot_app_common_data_unit_conversion_pressure<-max))
                  {
                        if(iot_app_common_data_unit_conversion_pressure>max)
                        {
                            div_disp_by_string(MAIN_SCREEN,"  UP");
                             main_screen_disp_clear_point();
                        }
                        else if(iot_app_common_data_unit_conversion_pressure>max)
                        {
                            div_disp_by_string(MAIN_SCREEN,"DOWN");
                            main_screen_disp_clear_point();
                        }
                  }
                  else
                  {
                        main_screen_disp_float_by_unit(iot_app_common_data_pressure_unit_flag,
                        iot_app_common_data_unit_conversion_pressure);
                  }                        
                    
              }
                
               timer_count = 0;
            }
        }

        if(iot_app_data_processing_and_output_control_2ms_timer_events & IOT_APP_2MS_TASK_DISP_SECOND_SCREEN)
        {
            static uint16 timer_count = 0;
        
            if (timer_count < 100/2) 
            {
                timer_count++;
            } 
            else 
            {
              
               second_screen_disp();
                
               timer_count = 0;
            }
            
        }
        if(iot_app_data_processing_and_output_control_2ms_timer_events & IOT_APP_2MS_TASK_OUTPUT1_CONTROL)
        {
           
            if (out1_timer_count < m_output.output_react_time/2) 
            {
                out1_timer_count++;
            } 
            else 
            {
                if(output_timer_set_out1_which_out == OUTPUT_STATE_ON)
                {
                    out_put_nonc_set_output(OUT1,1);
                    
                    m_output.output_out1_state = OUTPUT_STATE_ON;
                    
                    output1_timer_finish_flag = 1;
                    
                    iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT1_CONTROL);
                }
                if(output_timer_set_out1_which_out == OUTPUT_STATE_OFF)
                {
                 
                    out_put_nonc_set_output(OUT1,0);
                    
                    m_output.output_out1_state = OUTPUT_STATE_OFF;
                    
                    output1_timer_finish_flag = 1;
                    
                    iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT1_CONTROL);
                }
                
                out1_timer_count = 0;
            }
        }
        
        if(iot_app_data_processing_and_output_control_2ms_timer_events & IOT_APP_2MS_TASK_OUTPUT2_CONTROL)
        {
            
            
            if (out2_timer_count < m_output.output_react_time/2) 
            {
                out2_timer_count++;
            } 
            else 
            {
                if(output_timer_set_out2_which_out == OUTPUT_STATE_ON)
                {
                    //HAL_OUT2_ON();
                     out_put_nonc_set_output(OUT2,1);
                    
                    m_output.output_out2_state = OUTPUT_STATE_ON;
                    
                   output2_timer_finish_flag = 1;
                    
                    iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT2_CONTROL);
                }
                if(output_timer_set_out2_which_out == OUTPUT_STATE_OFF)
                {
                    //HAL_OUT2_OFF();
                    out_put_nonc_set_output(OUT2,0);
                    
                    m_output.output_out2_state = OUTPUT_STATE_OFF;
                    
                    output2_timer_finish_flag = 1;
                    
                   iot_app_2ms_task_delete(IOT_APP_2MS_TASK_OUTPUT2_CONTROL);
                }
                
                out2_timer_count = 0;
            }
        }
        
        iot_app_data_flash_store();
        
        return (events ^ IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_2MS_TIMER_EVT);
    }
    
    return 0;
}

void iot_app_data_processing_and_output_control_init(uint8_t task_id)
{
    iot_app_data_processing_and_output_control_task_id = task_id;
    
     /* flash数据读取 */
    #if (defined HAL_READFLASH) && (HAL_READFLASH == TRUE)
    iot_app_data_flash_restore();
    #endif
    
    barometric();
    
    HalKeyCallbackRegister(iot_app_data_processing_and_output_control_key_callback);

}

#endif 

/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif 

