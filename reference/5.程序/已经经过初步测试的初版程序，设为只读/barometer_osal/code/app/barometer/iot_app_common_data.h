
#ifndef _IOT_APP_COMMON_DATA_H
#define _IOT_APP_COMMON_DATA_H

#ifdef __cplusplus
extern "C"
{
#endif

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                               MACRO
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
//=系统初始化完成定义
#define IOT_APP_SYSTEM_INIT_OK                                      1
#define IOT_APP_SYSTEM_INIT_NO                                      0
    
// 系统状态定义
#define IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL                     0       // 主界面
#define IOT_APP_COMMON_DATA_SYSTEM_STATE_BOOT_ANIMATION             1       // 开机界面
#define IOT_APP_COMMON_DATA_SYSTEM_STATE_MENU                       2       // 菜单界面
#define IOT_APP_COMMON_DATA_SYSTEM_STATE_KEY_LOCK                   3       // 锁屏界面
#define IOT_APP_COMMON_DATA_SYSTEM_STATE_PEAK_BOTTOM_HOLD           4       // 峰谷值界面
#define IOT_APP_COMMON_DATA_SYSTEM_STATE_ZERO_CALIBRATION           5       // 校零功能界面

// 输出1比较项
#define COMPARE1_EASY                                               1
#define COMPARE1_HSY                                                2
#define COMPARE1_WCMP                                               3

// OUT1输出模式变量
#define IOT_APP_COMMON_DATA_OUTPUT1_EASY                            0
#define IOT_APP_COMMON_DATA_OUTPUT1_HYSTERESIS                      1
#define IOT_APP_COMMON_DATA_OUTPUT1_WINDOW_COMPARE                  2

// OUT2输出模式变量
#define IOT_APP_COMMON_DATA_OUTPUT2_OFF                             0
#define IOT_APP_COMMON_DATA_OUTPUT2_EASY                            1
#define IOT_APP_COMMON_DATA_OUTPUT2_HYSTERESIS                      2
#define IOT_APP_COMMON_DATA_OUTPUT2_WINDOW_COMPARE                  3

// 常开常闭输出模式变量
#define IOT_APP_COMMON_DATA_NONC_O2ON_NORMAL_OPEN                   0
#define IOT_APP_COMMON_DATA_NONC_O2ON_NORMAL_CLOSE                  1
#define IOT_APP_COMMON_DATA_NONC_O2ON_OPEN_CLOSE                    2
#define IOT_APP_COMMON_DATA_NONC_O2ON_CLOSE_OPEN                    3
#define IOT_APP_COMMON_DATA_NONC_O2OFF_NORMAL_OPEN                  0
#define IOT_APP_COMMON_DATA_NONC_O2OFF_NORMAL_CLOSE                 1

// 反应时间设定变量
#define IOT_APP_COMMON_DATA_RESPONSE_TIME_2500US                    0
#define IOT_APP_COMMON_DATA_RESPONSE_TIME_5MS                       1
#define IOT_APP_COMMON_DATA_RESPONSE_TIME_10MS                      2
#define IOT_APP_COMMON_DATA_RESPONSE_TIME_25MS                      3
#define IOT_APP_COMMON_DATA_RESPONSE_TIME_50MS                      4
#define IOT_APP_COMMON_DATA_RESPONSE_TIME_100MS                     5
#define IOT_APP_COMMON_DATA_RESPONSE_TIME_250MS                     6
#define IOT_APP_COMMON_DATA_RESPONSE_TIME_500MS                     7
#define IOT_APP_COMMON_DATA_RESPONSE_TIME_1S                        8
#define IOT_APP_COMMON_DATA_RESPONSE_TIME_5S                        9

// 屏幕文字颜色变量
#define IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_RON                   0
#define IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_GON                   1
#define IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_RED                   2
#define IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_GREEN                 3

// 单位变量
#define IOT_APP_COMMON_DATA_PRESSURE_UNIT_KPA                       0
#define IOT_APP_COMMON_DATA_PRESSURE_UNIT_KGF                       1
#define IOT_APP_COMMON_DATA_PRESSURE_UNIT_BAR                       2
#define IOT_APP_COMMON_DATA_PRESSURE_UNIT_PSI                       3
#define IOT_APP_COMMON_DATA_PRESSURE_UNIT_MMHG                      4
#define IOT_APP_COMMON_DATA_PRESSURE_UNIT_INHG                      5
#define IOT_APP_COMMON_DATA_PRESSURE_UNIT_MPA                       6

// 显示速度变量
#define IOT_APP_COMMON_DATA_DISPLAY_SPEED_250MS                     0
#define IOT_APP_COMMON_DATA_DISPLAY_SPEED_500MS                     1
#define IOT_APP_COMMON_DATA_DISPLAY_SPEED_1S                        2

// 应差级别变量
#define IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_0                      0
#define IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_1                      1
#define IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_2                      2
#define IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_3                      3
#define IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_4                      4
#define IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_5                      5
#define IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_6                      6
#define IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_7                      7
#define IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_8                      8

// ECO模式变量
#define IOT_APP_COMMON_DATA_ECO_OFF                                 0
#define IOT_APP_COMMON_DATA_ECO_STD                                 1
#define IOT_APP_COMMON_DATA_ECO_FULL                                2

// 复制功能变量
#define IOT_APP_COMMON_DATA_COPY_OFF                                0
#define IOT_APP_COMMON_DATA_COPY_ON                                 1
#define IOT_APP_COMMON_DATA_COPY_ON_LOCK                            2
#define IOT_APP_COMMON_DATA_COPY_READY                              3

//= 副屏显示
#define IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_STD                  0
#define IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_OFF                  1
#define IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_UNIT                 2
#define IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_NO                   3
#define IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_CUSTOM               4

// 显示颜色联动转换变量
#define IOT_APP_COMMON_DATA_DISPLAY_COLOR_LINKAGE_OUT1              0
#define IOT_APP_COMMON_DATA_DISPLAY_COLOR_LINKAGE_OUT2              1

#define FLOAT_MIN_DIFFERENCE 1e-9
/*************************************************-*************************************************
*                                           FLASH DEFAULT VALUE
**************************************************-*****************^******************************/

#define IOT_APP_COMMON_DATA_TOLERANCE_DEFAULT    0


#define IOT_APP_DATA_NV_ID                                          0x0001
#define IOT_APP_DATA_NV_LEN                                         (37 * 4)


/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
#define HAL_KEY_EVENT_TIMEOUT                                       50

#define IOT_APP_COMMON_DATA_EVENT_SCREEN_SET_BRIGHTNESS_EVT                         0x0001
#define IOT_APP_COMMON_DATA_EVENT_SCREEN_SET_BRIGHTNESS_TIMEOUT                     1
#define IOT_APP_COMMON_DATA_EVENT_SCREEN_SET_BRIGHTNESS_PERIOD_START()              osal_start_reload_timer(iot_app_common_data_task_id, IOT_APP_COMMON_DATA_EVENT_SCREEN_SET_BRIGHTNESS_EVT, IOT_APP_COMMON_DATA_EVENT_SCREEN_SET_BRIGHTNESS_TIMEOUT)
#define IOT_APP_COMMON_DATA_EVENT_SCREEN_SET_BRIGHTNESS_STOP()                      osal_stop_timerEx(iot_app_common_data_task_id, IOT_APP_COMMON_DATA_EVENT_SCREEN_SET_BRIGHTNESS_EVT)

// 目标扇区（SECTOR_1，地址 0x0800f400 ~ 0x0800F7FF）
//#define TARGET_SECTOR_ADDR  0x0800f400
//#define TARGET_SECTOR_NUM   ((TARGET_SECTOR_ADDR - STM32_FLASH_BASE) / STM32_SECTOR_SIZE)	

/*************************************************-*************************************************
*                                          GLOBAL VARIABLES
**************************************************-*****************^******************************/
extern uint8_t iot_app_common_data_task_id;
extern uint8_t  iot_app_data_system_state;
extern uint8_t iot_app_data_open_system_init_flag;

extern uint32_t iot_app_out1compare_set_item;	

extern uint8_t iot_app_common_data_output1_flag;
extern uint8_t iot_app_common_data_output2_flag;
extern uint8_t iot_app_common_data_nonc_output_flag;
extern uint8_t iot_app_common_data_response_time_flag;
extern uint8_t iot_app_common_data_screen_text_color_flag;
extern uint8_t iot_app_common_data_pressure_unit_flag;
extern uint8_t iot_app_common_data_display_speed_flag;
extern uint8_t iot_app_common_data_hysteresis_level;
extern uint8_t iot_app_common_data_eco_flag;
extern uint8_t iot_app_common_data_copy_flag;
extern uint8_t iot_app_common_data_subsidiary_display_flag;
extern uint8_t iot_app_common_data_display_color_linkage_flag;
extern uint8_t iot_app_common_data_reset_flag;

extern float iot_app_common_data_pressure_max;                              // 开启峰值/谷值保持功能的最大气压值
extern float iot_app_common_data_pressure_min;                              // 开启峰值/谷值保持功能的最小气压值

extern float * iot_app_common_data_now_set_which_tolerance; 				// 当前正在设置的应差
extern float iot_app_common_data_now_pressure;                              // 当前气压，单位kpa
extern float iot_app_common_data_unit_conversion_pressure;                  // 单位转换后的气压

extern float iot_app_common_data_tolerance_P1;                              // 各应差模式下的设定值
extern float iot_app_common_data_tolerance_P2;
extern float iot_app_common_data_tolerance_Hi1; 
extern float iot_app_common_data_tolerance_Lo1;
extern float iot_app_common_data_tolerance_Hi2;
extern float iot_app_common_data_tolerance_Lo2;


extern uint8_t iot_app_common_data_screen_pwm_counter ;                     // PWM计数器
extern uint8_t iot_app_common_data_screen_pwm_duty ;                        // PWM占空比
extern uint8_t iot_app_common_data_screen_pwm_enabled ;                     // PWM使能标志
extern uint8_t iot_app_common_data_screen_pwm_state ;                       // PWM状态
extern uint16_t iot_app_common_data_eco_count ;                             // 息屏时间计算值
extern uint8_t iot_app_common_data_data_subsidiary_display_no_right;        // 次屏显示NO档右编号
extern uint8_t iot_app_common_data_data_subsidiary_display_no_left;         // 次屏显示NO档左编号

extern uint8_t iot_app_common_data_calibration_ok_or_no;

/*************************************************-*************************************************
*                                             FUNCTION
**************************************************-*****************^******************************/
extern void iot_app_common_data_init(uint8_t task_id);
extern uint16_t iot_app_common_data_process_event(uint8_t task_id, uint16_t events);

extern void iot_app_common_data_screen_set_brightness(uint8_t duty_percent);
extern void iot_app_data_menu_mode_enter_cb(void);
extern void iot_app_common_data_system_state_set(uint8_t new_state);

extern void iot_app_data_flash_erase(void);
extern void iot_app_data_flash_store(void);
extern void iot_app_data_flash_restore(void);
/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif

