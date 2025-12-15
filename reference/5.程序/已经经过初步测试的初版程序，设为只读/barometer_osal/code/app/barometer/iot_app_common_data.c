/**************************************************************************************************
Filename:       iot_app_common_data.c
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

#ifdef _IOT_APP_COMMON_DATA_H
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

/*************************************************-*************************************************
*                                           STRUCT DEFINED
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          GLOBAL VARIABLES
**************************************************-*****************^******************************/
uint8_t iot_app_common_data_task_id;                                                                // 任务ID，由OSAL分配，用于内部任务/事件处理
uint8_t  iot_app_data_system_state = IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL;                       // 系统状态

uint8_t iot_app_data_open_system_init_flag = IOT_APP_SYSTEM_INIT_NO ; // 开机初始化是否完成，完成flash读写，开机动画之后

uint32_t iot_app_out1compare_set_item = COMPARE1_EASY;

uint8_t iot_app_common_data_output1_flag            = IOT_APP_COMMON_DATA_OUTPUT1_EASY          ;                           // out1输出变量
uint8_t iot_app_common_data_output2_flag            = IOT_APP_COMMON_DATA_OUTPUT2_OFF           ;                           // out2输出变量
uint8_t iot_app_common_data_nonc_output_flag        = IOT_APP_COMMON_DATA_NONC_O2ON_NORMAL_OPEN ;                           // 开闭输出变量
uint8_t iot_app_common_data_response_time_flag      = IOT_APP_COMMON_DATA_RESPONSE_TIME_2500US  ;                           // 反应时间变量
uint8_t iot_app_common_data_screen_text_color_flag  = IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_RON ;                           // 屏幕文字颜色变量
uint8_t iot_app_common_data_pressure_unit_flag      = IOT_APP_COMMON_DATA_PRESSURE_UNIT_KPA     ;                           // 单位变量
uint8_t iot_app_common_data_display_speed_flag      = IOT_APP_COMMON_DATA_DISPLAY_SPEED_250MS   ;                           // 显示速度变量
uint8_t iot_app_common_data_hysteresis_level        = IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_1    ;                           // 应差级别变量
uint8_t iot_app_common_data_eco_flag                = IOT_APP_COMMON_DATA_ECO_OFF               ;                           // ECO模式变量
uint8_t iot_app_common_data_copy_flag               = IOT_APP_COMMON_DATA_COPY_OFF              ;                           // 复制功能变量
uint8_t iot_app_common_data_subsidiary_display_flag = IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_STD;                           // 副屏显示内容设置变量
uint8_t iot_app_common_data_display_color_linkage_flag = IOT_APP_COMMON_DATA_DISPLAY_COLOR_LINKAGE_OUT1;                    // 颜色联动转换变量
uint8_t iot_app_common_data_reset_flag = 0;                                                                                 // 恢复出厂设置变量

float iot_app_common_data_pressure_max = 0;                              // 开启峰值/谷值保持功能的最大气压值
float iot_app_common_data_pressure_min = 0;                              // 开启峰值/谷值保持功能的最小气压值

float iot_app_common_data_now_pressure = 0;                              // 当前气压，单位kpa
float iot_app_common_data_unit_conversion_pressure = 0 ;                 // 单位转换后的气压

float iot_app_common_data_tolerance_P1 = 0.0;                            // 各应差模式下的设定值
float iot_app_common_data_tolerance_P2 = 0.0;
float iot_app_common_data_tolerance_Hi1 = 0.0; 
float iot_app_common_data_tolerance_Lo1 = 0.0;
float iot_app_common_data_tolerance_Hi2 = 0.0;
float iot_app_common_data_tolerance_Lo2 = 0.0;
float iot_app_common_data_default = -80.0;

uint8_t iot_app_common_data_screen_pwm_counter = 0;                     // PWM计数器
uint8_t iot_app_common_data_screen_pwm_duty = 0;                        // PWM占空比
uint8_t iot_app_common_data_screen_pwm_enabled = 0;                     // PWM使能标志
uint8_t iot_app_common_data_screen_pwm_state = 0;                       // PWM状态
uint16_t iot_app_common_data_eco_count = 0;                             // 息屏时间计算值

uint8_t iot_app_common_data_data_subsidiary_display_no_right = 0;       // 次屏显示NO档右编号
uint8_t iot_app_common_data_data_subsidiary_display_no_left = 0;        // 次屏显示NO档左编号

uint8_t iot_app_common_data_calibration_ok_or_no = 0;

/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
// 设置LED亮度并启动pwm
void iot_app_common_data_screen_set_brightness(uint8_t duty_percent)
{
    IOT_APP_COMMON_DATA_EVENT_SCREEN_SET_BRIGHTNESS_STOP();
    if (!duty_percent)
    {
        iot_app_common_data_screen_pwm_enabled = 0;
        iot_app_common_data_screen_pwm_state = 0;
        iot_app_barometer_disp_screen_text_color(0);
        return;
    }
    if (duty_percent > 100)
    {
        duty_percent = 100;
    }

    // 将百分比转换为对应的计数阈值
    // 例如：50% -> 5/10，75% -> 7.5/10，四舍五入
    iot_app_common_data_screen_pwm_duty = (duty_percent * 10 + 50) / 100;

    // 确保占空比在有效范围内
    if (iot_app_common_data_screen_pwm_duty > 10)
    {
        iot_app_common_data_screen_pwm_duty = 10;
    }

    // 启动pwm
    iot_app_common_data_screen_pwm_enabled = 1;
    iot_app_common_data_screen_pwm_state = 1;
    iot_app_common_data_screen_pwm_counter = 0;

    // 立即更新LED状态，根据新counter和duty
    if (iot_app_common_data_screen_pwm_counter < iot_app_common_data_screen_pwm_duty)
    {
        iot_app_barometer_disp_screen_text_color(1);
    }
    else
    {
        iot_app_barometer_disp_screen_text_color(0);
    }

    IOT_APP_COMMON_DATA_EVENT_SCREEN_SET_BRIGHTNESS_PERIOD_START();
}

void iot_app_common_data_system_state_set(uint8_t new_state)
{
    // 系统状态没变时直接退出
    if (iot_app_data_system_state == new_state)
    {
        return ;
    }

    uint8_t old_state = iot_app_data_system_state;

    // 处理退出旧状态的操作
    switch (old_state)
    {
        case IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL:
        {
            #ifdef _IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H

            second_screen_disp_clear_point();
            main_screen_disp_clear_point();
            
            //= 关闭气压显示
            main_screen_disp_pressure_stop();
            second_screen_disp_stop();
            #endif
            
        }break;
        case IOT_APP_COMMON_DATA_SYSTEM_STATE_BOOT_ANIMATION:
        {
            iot_app_common_data_screen_set_brightness(100);
        }break;
        case IOT_APP_COMMON_DATA_SYSTEM_STATE_MENU:
        {
            
        }break;
        case IOT_APP_COMMON_DATA_SYSTEM_STATE_KEY_LOCK:
        {
        }break;
        case IOT_APP_COMMON_DATA_SYSTEM_STATE_PEAK_BOTTOM_HOLD:
        {
        }break;
        case IOT_APP_COMMON_DATA_SYSTEM_STATE_ZERO_CALIBRATION:
        {
        }break;
        default:break;
    }

    iot_app_data_system_state = new_state;

    // 处理进入新状态的操作
    switch (new_state)
    {
        case IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL:
        {
            iot_app_barometer_disp_screen_text_color(1);
            #ifdef _IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H
            
            if(old_state != IOT_APP_COMMON_DATA_SYSTEM_STATE_BOOT_ANIMATION)
            {
                div_disp_by_string(SECOND_SCREEN,"    ");
                div_disp_by_string(MAIN_SCREEN,"    ");
                
                second_screen_disp_clear_point();
                main_screen_disp_clear_point();
                
                //= 开启气压显示
                main_screen_disp_pressure_start();
                second_screen_disp_stop();
                second_screen_disp();
                
                //= 刷新一下设定值
                flash_now_set();
                second_screen_disp();
                iot_app_common_data_screen_set_brightness(100);
            }
            #endif
        }break;
        case IOT_APP_COMMON_DATA_SYSTEM_STATE_BOOT_ANIMATION:
        {
        }break;
        case IOT_APP_COMMON_DATA_SYSTEM_STATE_MENU:
        {
            #ifdef HAL_BOARD_CFG_H
            HAL_LED1_TURN_ON();
            HAL_LED3_TURN_ON();
            #endif

        }break;
        case IOT_APP_COMMON_DATA_SYSTEM_STATE_KEY_LOCK:
        {
            
            #ifdef _IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H
            //= 清除残留
            div_disp_by_string(SECOND_SCREEN,"    ");
            div_disp_by_string(MAIN_SCREEN,"    ");
            
            second_screen_disp_clear_point();
            main_screen_disp_clear_point();
            
            //= 开启气压显示
            main_screen_disp_pressure_start();
            second_screen_disp_start();
            #endif
            
        }break;
        case IOT_APP_COMMON_DATA_SYSTEM_STATE_PEAK_BOTTOM_HOLD:
        {
        }break;
        case IOT_APP_COMMON_DATA_SYSTEM_STATE_ZERO_CALIBRATION:
        {
        }break;
        default:break;
    }
}




//= flash浮点存储要特殊处理
void iot_app_data_flash_erase(void)
{
    for (uint16_t pg = HAL_NV_PAGE_BEG; pg <= HAL_NV_PAGE_END; pg++)
    {
        HalFlashErase(pg);
    }
}

void iot_app_data_flash_store(void)
{
    uint8 ifchange = 0;
    
    uint32_t read_buf[IOT_APP_DATA_NV_LEN/4];
    
    memset((uint8_t *)read_buf, 0, sizeof(read_buf));
    
    if (SUCCESS != osal_nv_read(IOT_APP_DATA_NV_ID, 0, IOT_APP_DATA_NV_LEN, (uint8_t *)read_buf)) 
    {
        return ;
    }
    
    if(fabs(iot_app_common_data_tolerance_P1  - *(float*)(&read_buf[0]))>FLOAT_MIN_DIFFERENCE){ifchange = 1;}
    if(fabs(iot_app_common_data_tolerance_P2  - *(float*)(&read_buf[1]))>FLOAT_MIN_DIFFERENCE){ifchange = 1;}
    if(fabs(iot_app_common_data_tolerance_Hi1 - *(float*)(&read_buf[2]))>FLOAT_MIN_DIFFERENCE){ifchange = 1;}
    if(fabs(iot_app_common_data_tolerance_Lo1 - *(float*)(&read_buf[3]))>FLOAT_MIN_DIFFERENCE){ifchange = 1;}
    if(fabs(iot_app_common_data_tolerance_Hi2 - *(float*)(&read_buf[4]))>FLOAT_MIN_DIFFERENCE){ifchange = 1;}
    if(fabs(iot_app_common_data_tolerance_Lo2 - *(float*)(&read_buf[5]))>FLOAT_MIN_DIFFERENCE){ifchange = 1;}
    if (iot_app_common_data_screen_text_color_flag  != (uint8_t)(read_buf[6] ))           {ifchange = 1;}
    if (iot_app_common_data_output1_flag            != (uint8_t)(read_buf[7] ))           {ifchange = 1;}
    if (iot_app_common_data_output2_flag            != (uint8_t)(read_buf[8] ))           {ifchange = 1;}
    if (iot_app_common_data_nonc_output_flag        != (uint8_t)(read_buf[9] ))           {ifchange = 1;}
    if (iot_app_common_data_response_time_flag      != (uint8_t)(read_buf[10]))           {ifchange = 1;}
    if (iot_app_common_data_pressure_unit_flag      != (uint8_t)(read_buf[11]))           {ifchange = 1;}
    if (iot_app_common_data_display_speed_flag      != (uint8_t)(read_buf[12]))           {ifchange = 1;}
    if (iot_app_common_data_hysteresis_level        != (uint8_t)(read_buf[13]))           {ifchange = 1;}
    if (iot_app_common_data_eco_flag                != (uint8_t)(read_buf[14]))           {ifchange = 1;}
    if (iot_app_common_data_copy_flag               != (uint8_t)(read_buf[15]))           {ifchange = 1;}
    if (iot_app_common_data_subsidiary_display_flag != (uint8_t)(read_buf[16]))           {ifchange = 1;}
    if (iot_app_common_data_data_subsidiary_display_no_right    != (uint8_t)(read_buf[17]))   {ifchange = 1;}
    if (iot_app_common_data_data_subsidiary_display_no_left     != (uint8_t)(read_buf[18]))   {ifchange = 1;}
    if (iot_app_common_data_calibration_ok_or_no                != (uint8_t)(read_buf[19]))   {ifchange = 1;}
    if (iot_app_common_data_display_color_linkage_flag          != (uint8_t)(read_buf[20]))   {ifchange = 1;}
    if((x[0]  - *(float*)(&read_buf[21]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}
    if((x[1]  - *(float*)(&read_buf[22]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}
    if((x[2]  - *(float*)(&read_buf[23]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}
    if((x[3]  - *(float*)(&read_buf[24]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}
    if((x[4]  - *(float*)(&read_buf[25]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}
    if((x[5]  - *(float*)(&read_buf[26]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}
    if((x[6]  - *(float*)(&read_buf[27]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}
    if((y[0]  - *(float*)(&read_buf[28]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}
    if((y[1]  - *(float*)(&read_buf[29]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}
    if((y[2]  - *(float*)(&read_buf[30]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}
    if((y[3]  - *(float*)(&read_buf[31]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}
    if((y[4]  - *(float*)(&read_buf[32]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}
    if((y[5]  - *(float*)(&read_buf[33]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}
    if((y[6]  - *(float*)(&read_buf[34]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}
    if(fabs(zero_calibration_value  - *(float*)(&read_buf[35]))>FLOAT_MIN_DIFFERENCE)  {ifchange = 1;}

    if(ifchange)
    {
         uint32_t data[IOT_APP_DATA_NV_LEN/4] = 
        {
            //= 应差比较值
            *(uint32_t*)&iot_app_common_data_tolerance_P1 ,
            *(uint32_t*)&iot_app_common_data_tolerance_P2 ,
            *(uint32_t*)&iot_app_common_data_tolerance_Hi1,
            *(uint32_t*)&iot_app_common_data_tolerance_Lo1,
            *(uint32_t*)&iot_app_common_data_tolerance_Hi2,
            *(uint32_t*)&iot_app_common_data_tolerance_Lo2,
            (uint32_t)iot_app_common_data_screen_text_color_flag ,           // 屏幕文字颜色变量
            (uint32_t)iot_app_common_data_output1_flag           ,           // out1输出变量    
            (uint32_t)iot_app_common_data_output2_flag           ,           // out2输出变量
            (uint32_t)iot_app_common_data_nonc_output_flag       ,           // 开闭输出变量
            (uint32_t)iot_app_common_data_response_time_flag     ,           // 反应时间变量
            (uint32_t)iot_app_common_data_pressure_unit_flag     ,           // 单位变量
            (uint32_t)iot_app_common_data_display_speed_flag     ,           // 显示速度变量
            (uint32_t)iot_app_common_data_hysteresis_level       ,           // 应差级别变量
            (uint32_t)iot_app_common_data_eco_flag               ,           // ECO模式变量
            (uint32_t)iot_app_common_data_copy_flag              ,           // 复制功能变量
            (uint32_t)iot_app_common_data_subsidiary_display_flag,           // 副屏显示功能变量
            (uint32_t)iot_app_common_data_data_subsidiary_display_no_right,  // 副屏显示NO档右编号
            (uint32_t)iot_app_common_data_data_subsidiary_display_no_left,   // 副屏显示NO档左编号
            (uint32_t)iot_app_common_data_calibration_ok_or_no   ,           
            (uint32_t)iot_app_common_data_display_color_linkage_flag,        // 显色颜色联动变量
            *(uint32_t*)&x[0] ,
            *(uint32_t*)&x[1] ,
            *(uint32_t*)&x[2] ,
            *(uint32_t*)&x[3] ,
            *(uint32_t*)&x[4] ,
            *(uint32_t*)&x[5] ,
            *(uint32_t*)&x[6] ,
            *(uint32_t*)&y[0] ,
            *(uint32_t*)&y[1] ,
            *(uint32_t*)&y[2] ,
            *(uint32_t*)&y[3] ,
            *(uint32_t*)&y[4] ,
            *(uint32_t*)&y[5] ,
            *(uint32_t*)&y[6] ,
            *(uint32_t*)&zero_calibration_value,
        };

        osal_nv_write(IOT_APP_DATA_NV_ID, 0, IOT_APP_DATA_NV_LEN, (uint8_t *)data);
    }
} 

void iot_app_data_flash_restore(void)
{
    uint32_t read_buf[IOT_APP_DATA_NV_LEN/4];

    memset((uint8_t *)read_buf, 0, sizeof(read_buf));
    
    if (SUCCESS != osal_nv_read(IOT_APP_DATA_NV_ID, 0, IOT_APP_DATA_NV_LEN, (uint8_t *)read_buf))
    {
        uint32_t data[IOT_APP_DATA_NV_LEN/4] = 
        {
          //= 应差默认比较值
            *(uint32_t*)&iot_app_common_data_default,
            IOT_APP_COMMON_DATA_TOLERANCE_DEFAULT,
            IOT_APP_COMMON_DATA_TOLERANCE_DEFAULT,
            IOT_APP_COMMON_DATA_TOLERANCE_DEFAULT,
            IOT_APP_COMMON_DATA_TOLERANCE_DEFAULT,
            IOT_APP_COMMON_DATA_TOLERANCE_DEFAULT,
            IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_RON,          // 屏幕文字颜色变量默认值
            IOT_APP_COMMON_DATA_OUTPUT1_EASY         ,
            IOT_APP_COMMON_DATA_OUTPUT2_OFF          ,
            IOT_APP_COMMON_DATA_NONC_O2OFF_NORMAL_OPEN,
            IOT_APP_COMMON_DATA_RESPONSE_TIME_2500US ,
            IOT_APP_COMMON_DATA_PRESSURE_UNIT_KPA    ,
            IOT_APP_COMMON_DATA_DISPLAY_SPEED_250MS  ,
            IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_1   ,
            IOT_APP_COMMON_DATA_ECO_OFF              ,
            IOT_APP_COMMON_DATA_COPY_OFF             ,
            IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_STD,
            0,                                                  // 副屏显示NO档右编号
            0,                                                  // 副屏显示NO档左编号
            1,                                                  //= 初始化校准
            IOT_APP_COMMON_DATA_DISPLAY_COLOR_LINKAGE_OUT1,
            *(uint32_t*)&x[0] ,
            *(uint32_t*)&x[1] ,
            *(uint32_t*)&x[2] ,
            *(uint32_t*)&x[3] ,
            *(uint32_t*)&x[4] ,
            *(uint32_t*)&x[5] ,
            *(uint32_t*)&x[6] ,
            *(uint32_t*)&y[0] ,
            *(uint32_t*)&y[1] ,
            *(uint32_t*)&y[2] ,
            *(uint32_t*)&y[3] ,
            *(uint32_t*)&y[4] ,
            *(uint32_t*)&y[5] ,
            *(uint32_t*)&y[6] ,
            *(uint32_t*)&zero_calibration_value,
        };

        osal_nv_item_init(IOT_APP_DATA_NV_ID, IOT_APP_DATA_NV_LEN, (uint8_t *)data);
       
        osal_nv_read(IOT_APP_DATA_NV_ID, 0, IOT_APP_DATA_NV_LEN, (uint8_t *)read_buf);
    }
    
    
    iot_app_common_data_tolerance_P1  = *(float*)(&read_buf[0]);
    iot_app_common_data_tolerance_P2  = *(float*)(&read_buf[1]);
    iot_app_common_data_tolerance_Hi1 = *(float*)(&read_buf[2]);
    iot_app_common_data_tolerance_Lo1 = *(float*)(&read_buf[3]);
    iot_app_common_data_tolerance_Hi2 = *(float*)(&read_buf[4]);
    iot_app_common_data_tolerance_Lo2 = *(float*)(&read_buf[5]);
    iot_app_common_data_screen_text_color_flag = (uint8_t)(read_buf[6]);
    iot_app_common_data_output1_flag            = (uint8_t)(read_buf[7]) ;
    iot_app_common_data_output2_flag            = (uint8_t)(read_buf[8]) ;
    iot_app_common_data_nonc_output_flag        = (uint8_t)(read_buf[9]) ;
    iot_app_common_data_response_time_flag      = (uint8_t)(read_buf[10]);
    iot_app_common_data_pressure_unit_flag      = (uint8_t)(read_buf[11]);
    iot_app_common_data_display_speed_flag      = (uint8_t)(read_buf[12]);
    iot_app_common_data_hysteresis_level        = (uint8_t)(read_buf[13]);
    iot_app_common_data_eco_flag                = (uint8_t)(read_buf[14]);
    iot_app_common_data_copy_flag               = (uint8_t)(read_buf[15]);
    iot_app_common_data_subsidiary_display_flag = (uint8_t)(read_buf[16]);
    iot_app_common_data_data_subsidiary_display_no_right = (uint8_t)(read_buf[17]);
    iot_app_common_data_data_subsidiary_display_no_left  = (uint8_t)(read_buf[18]);
    iot_app_common_data_calibration_ok_or_no =  (uint8_t)(read_buf[19]);
    iot_app_common_data_display_color_linkage_flag= (uint8_t)(read_buf[20]);
    x[0] =  *(float*)(&read_buf[21]);
    x[1] =  *(float*)(&read_buf[22]);
    x[2] =  *(float*)(&read_buf[23]);
    x[3] =  *(float*)(&read_buf[24]);
    x[4] =  *(float*)(&read_buf[25]);
    x[5] =  *(float*)(&read_buf[26]);
    x[6] =  *(float*)(&read_buf[27]);
    y[0] =  *(float*)(&read_buf[28]);
    y[1] =  *(float*)(&read_buf[29]);
    y[2] =  *(float*)(&read_buf[30]);
    y[3] =  *(float*)(&read_buf[31]);
    y[4] =  *(float*)(&read_buf[32]);
    y[5] =  *(float*)(&read_buf[33]);
    y[6] =  *(float*)(&read_buf[34]);
    zero_calibration_value = *(float*)(&read_buf[35]);
}
uint16_t iot_app_common_data_process_event(uint8_t task_id, uint16_t events)
{
    (void)task_id; // 显式忽略未引用参数（避免编译器警告）

    if (events & IOT_APP_COMMON_DATA_EVENT_SCREEN_SET_BRIGHTNESS_EVT)
    {
        // 如果PWM未使能，直接返回
        if (!iot_app_common_data_screen_pwm_enabled)
        {
            return (events ^ IOT_APP_COMMON_DATA_EVENT_SCREEN_SET_BRIGHTNESS_EVT);
        }

        // 增加计数器
        iot_app_common_data_screen_pwm_counter++;

        // 如果达到周期，重置计数器
        if (iot_app_common_data_screen_pwm_counter >= 10)
        {
            iot_app_common_data_screen_pwm_counter = 0;
        }

        // 根据占空比控制LED
        if (iot_app_common_data_screen_pwm_counter < iot_app_common_data_screen_pwm_duty)
        {
            // 在占空比时间内，LED亮
            iot_app_barometer_disp_screen_text_color(1);
        }
        else
        {
            // 其他时间，LED灭
            iot_app_barometer_disp_screen_text_color(0);
        }
        return (events ^ IOT_APP_COMMON_DATA_EVENT_SCREEN_SET_BRIGHTNESS_EVT);
    }

    // 丢弃未知事件
    return 0;
}

void iot_app_common_data_init(uint8_t task_id)
{
    iot_app_common_data_task_id = task_id;
}

#ifdef __cplusplus
}
#endif
#endif 

