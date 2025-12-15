
#ifndef _IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H
#define _IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                             DEFINES
**************************************************-*****************^******************************/
#define IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H_KEY_MASK            				    (HAL_KEY_MODE|HAL_KEY_RIGHT_DOWN|HAL_KEY_LEFT_UP)	

#define IOT_APP_2MS_TASK_DISP_PRESSURE      0x0001 
#define IOT_APP_2MS_TASK_OUTPUT1_CONTROL    0x0002   
#define IOT_APP_2MS_TASK_OUTPUT2_CONTROL    0x0004   
#define IOT_APP_2MS_TASK_DISP_SECOND_SCREEN 0x0008

#define UNIT_KPA    IOT_APP_COMMON_DATA_PRESSURE_UNIT_KPA
#define UNIT_KGF    IOT_APP_COMMON_DATA_PRESSURE_UNIT_KGF 
#define UNIT_BAR    IOT_APP_COMMON_DATA_PRESSURE_UNIT_BAR 
#define UNIT_PSI    IOT_APP_COMMON_DATA_PRESSURE_UNIT_PSI 
#define UNIT_MMHG   IOT_APP_COMMON_DATA_PRESSURE_UNIT_MMHG
#define UNIT_INHG   IOT_APP_COMMON_DATA_PRESSURE_UNIT_INHG
#define UNIT_MPA    IOT_APP_COMMON_DATA_PRESSURE_UNIT_MPA

#define TOLERANCE_LEVEL1  IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_1                      
#define TOLERANCE_LEVEL2  IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_2                      
#define TOLERANCE_LEVEL3  IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_3                      
#define TOLERANCE_LEVEL4  IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_4                      
#define TOLERANCE_LEVEL5  IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_5                      
#define TOLERANCE_LEVEL6  IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_6                      
#define TOLERANCE_LEVEL7  IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_7                      
#define TOLERANCE_LEVEL8  IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_8                      

 
#define REACT_TIME_2MS	        IOT_APP_COMMON_DATA_RESPONSE_TIME_2500US
#define REACT_TIME_5MS	        IOT_APP_COMMON_DATA_RESPONSE_TIME_5MS
#define REACT_TIME_10MS         IOT_APP_COMMON_DATA_RESPONSE_TIME_10MS
#define REACT_TIME_25MS         IOT_APP_COMMON_DATA_RESPONSE_TIME_25MS
#define REACT_TIME_50MS         IOT_APP_COMMON_DATA_RESPONSE_TIME_50MS
#define REACT_TIME_100MS        IOT_APP_COMMON_DATA_RESPONSE_TIME_100MS
#define REACT_TIME_250MS	    IOT_APP_COMMON_DATA_RESPONSE_TIME_250MS
#define REACT_TIME_500MS	    IOT_APP_COMMON_DATA_RESPONSE_TIME_500MS  
#define REACT_TIME_1000MS       IOT_APP_COMMON_DATA_RESPONSE_TIME_1S
#define REACT_TIME_5000MS       IOT_APP_COMMON_DATA_RESPONSE_TIME_5S
    
#define OUTPUT_SET_OUT1         1
#define OUTPUT_SET_OUT2         2
#define OUTPUT_STATE_ON         1
#define OUTPUT_STATE_OFF        0
#define OUTPUT_NONCSTATE_1NO2NC         IOT_APP_COMMON_DATA_NONC_O2ON_OPEN_CLOSE
#define OUTPUT_NONCSTATE_1NO2NO         IOT_APP_COMMON_DATA_NONC_O2ON_NORMAL_OPEN
#define OUTPUT_NONCSTATE_1NC2NC         IOT_APP_COMMON_DATA_NONC_O2ON_NORMAL_CLOSE
#define OUTPUT_NONCSTATE_1NC2NO         IOT_APP_COMMON_DATA_NONC_O2ON_CLOSE_OPEN
#define OUTPUT_NONCSTATE_1NO            IOT_APP_COMMON_DATA_NONC_O2OFF_NORMAL_OPEN
#define OUTPUT_NONCSTATE_1NC            IOT_APP_COMMON_DATA_NONC_O2OFF_NORMAL_CLOSE
#define NO 1
#define NC 0
#define OUT1 1
#define OUT2 2

#define SECOND_SCREEN_DISP_TOLERANCE    IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_STD
#define SECOND_SCREEN_DISP_NOW_UNIT     IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_UNIT
#define SECOND_SCREEN_DISP_OFF          IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_OFF
#define SECOND_SCREEN_DISP_NO           IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_NO

/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
#define IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_2MS_TIMER_EVT                                        0x0001 
#define IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_2MS_TIMER_INTERVAL                                   1 

#define IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_RECOVER_SECONDSCREEN_DISP_TIMER_EVT                  0x0002


#define IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_TIMER_EVT                    0x0004
#define IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_TIMER_INTERVAL               1
#define IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_START()                        osal_start_reload_timer(iot_app_data_processing_and_output_control_task_id, \
                                                                                                        IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_TIMER_EVT,\
                                                                                                        IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_TIMER_INTERVAL)
#define IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_STOP()                        osal_stop_timerEx(iot_app_data_processing_and_output_control_task_id, \
                                                                                                        IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_TIMER_EVT)                                                                                                        
#define IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_IS_ACTIVE()                    osal_get_timeoutEx(iot_app_data_processing_and_output_control_task_id, \
                                                                                                        IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_LONGPRESS_SET_TOLERANCE_TIMER_EVT)
                                                                                                        
                                                                                                        

/*************************************************-*************************************************
*                                             FUNCTION
**************************************************-*****************^******************************/
extern void     iot_app_data_processing_and_output_control_init(uint8 task_id);
extern uint16_t iot_app_data_processing_and_output_control_process_event(uint8 task_id, uint16 events);

extern void iot_app_data_processing_and_output_control_poll(void);

extern uint8_t iot_app_data_processing_and_output_control_common_key_callback(
    uint8_t cur_keys,
    uint8_t click_keys, uint8_t hold_keys, 
    uint8_t release_keys, uint32_t poll_time_milliseconds);

//= 可使用的公共变量
extern void main_screen_disp_clear_point(void);
extern void main_screen_disp_pressure_start(void);
extern void main_screen_disp_pressure_stop(void);

extern void main_screen_disp_float_by_unit(uint8 unit, float value);
extern void second_screen_disp_float_by_unit(uint8 unit, float value);

extern void second_screen_disp_clear_point(void);
extern void second_screen_disp_start(void);
extern void second_screen_disp_stop(void);
//= 私有公共变量
extern void second_screen_disp(void);
extern void second_screen_disp_now_set_value(void);
extern void flash_now_set(void);
    
extern void out_put_nonc_set_output_flashonce(void);

extern float pressure_read_once(void);

/*************************************************-*************************************************
*                                             GLOBEL VARIBLE
**************************************************-*****************^******************************/
typedef struct
{
    float output_tolerance;          // 应差值
    uint32_t output_react_time;      // 输出延迟
    
    uint8_t output_out1_state;       // 输出1控制状态
    uint8_t output_out2_state;       // 输出2控制状态
}management_of_output;

extern management_of_output m_output;

typedef struct
{
	char* second_screen_disp_save;
	float second_screen_disp_float_save;
}management_of_secondscreen;

extern management_of_secondscreen m_secondscreen;


extern float zero_calibration_value;

extern uint8 zero_calibration_flag;

extern float x[];
extern float y[];

/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif

