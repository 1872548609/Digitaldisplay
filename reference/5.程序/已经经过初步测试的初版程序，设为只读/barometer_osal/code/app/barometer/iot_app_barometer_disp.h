
#ifndef _IOT_APP_BAROMETER_DISP_H
#define _IOT_APP_BAROMETER_DISP_H

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
#define IOT_APP_BAROMETER_DISP_KEY_MASK            				    (HAL_KEY_MODE|HAL_KEY_RIGHT_DOWN|HAL_KEY_LEFT_UP)	

// 显示动画事件变量
#define IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_NORMAL           0
#define IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_BOOT_ANIMATION   1
#define IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_KEY_LOCK         2
#define IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_PEAK_BOTTOM_HOLD 3
#define IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_ZERO_CALIBRATION 4

// 闪烁显示事件变量
#define IOT_APP_BAROMETER_DISP_EVENT_FLASH_STATE_NORMAL             0
#define IOT_APP_BAROMETER_DISP_EVENT_FLASH_STATE_PEAK_BOTTOM_HOLD   1
#define IOT_APP_BAROMETER_DISP_EVENT_FLASH_STATE_NO_LEFT            2
#define IOT_APP_BAROMETER_DISP_EVENT_FLASH_STATE_NO_RIGHT           3

// 按键状态变量
#define IOT_APP_BAROMETER_DISP_KEY_STATE_UNLOCK                     0
#define IOT_APP_BAROMETER_DISP_KEY_STATE_LOCK                       1
#define IOT_APP_BAROMETER_DISP_KEY_STATE_LOCKING                    2

/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
#define IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_EVT                    0x0001
#define IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_TIMEOUT                50
#define IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_PERIOD_START()         osal_start_reload_timer(iot_app_barometer_disp_task_id, IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_EVT, IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_TIMEOUT)
#define IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STOP()                 osal_stop_timerEx(iot_app_barometer_disp_task_id, IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_EVT)
#define IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_IS_ACTIVE()            osal_get_timeoutEx(iot_app_barometer_disp_task_id, IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_EVT)

#define IOT_APP_BAROMETER_DISP_EVENT_FLASH_EVT                      0x0002
#define IOT_APP_BAROMETER_DISP_EVENT_FLASH_TIMEOUT                  500
#define IOT_APP_BAROMETER_DISP_EVENT_FLASH_PERIOD_START()           osal_start_reload_timer(iot_app_barometer_disp_task_id, IOT_APP_BAROMETER_DISP_EVENT_FLASH_EVT, IOT_APP_BAROMETER_DISP_EVENT_FLASH_TIMEOUT)
#define IOT_APP_BAROMETER_DISP_EVENT_FLASH_STOP()                   osal_stop_timerEx(iot_app_barometer_disp_task_id, IOT_APP_BAROMETER_DISP_EVENT_FLASH_EVT)
#define IOT_APP_BAROMETER_DISP_EVENT_FLASH_IS_ACTIVE()              osal_get_timeoutEx(iot_app_barometer_disp_task_id, IOT_APP_BAROMETER_DISP_EVENT_FLASH_EVT)

#define IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_EVT                   0x0004
#define IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_TIMEOUT               1
#define IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_PERIOD_START()        osal_start_reload_timer(iot_app_barometer_disp_task_id, IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_EVT, IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_TIMEOUT)
#define IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_STOP()                osal_stop_timerEx(iot_app_barometer_disp_task_id, IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_EVT)
#define IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_IS_ACTIVE()           osal_get_timeoutEx(iot_app_barometer_disp_task_id, IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_EVT)

/*************************************************-*************************************************
*                                             FUNCTION
**************************************************-*****************^******************************/
extern void     iot_app_barometer_disp_init(uint8 task_id);
extern uint16_t iot_app_barometer_disp_process_event(uint8 task_id, uint16 events);

extern uint8_t  iot_app_menu_item_level_1_cb_click_key(void *item, uint8_t keys);
extern uint8_t  iot_app_menu_item_level_2_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_pro_cb_enter(void *item);
extern void     iot_app_menu_item_setting_pro_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_pro_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_subsidiary_display_cb_enter(void *item);
extern void     iot_app_menu_item_setting_subsidiary_display_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_subsidiary_display_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_display_speed_cb_enter(void *item);
extern void     iot_app_menu_item_setting_display_speed_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_display_speed_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_hysteresis_level_cb_enter(void *item);
extern void     iot_app_menu_item_setting_hysteresis_level_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_hysteresis_level_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_display_color_linkage_cb_enter(void *item);
extern void     iot_app_menu_item_setting_display_color_linkage_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_display_color_linkage_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_eco_cb_enter(void *item);
extern void     iot_app_menu_item_setting_eco_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_eco_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_copy_cb_enter(void *item);
extern void     iot_app_menu_item_setting_copy_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_copy_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_reset_cb_enter(void *item);
extern void     iot_app_menu_item_setting_reset_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_reset_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_normal_cb_enter(void *item);
extern void     iot_app_menu_item_setting_normal_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_normal_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_out1mode_cb_enter(void *item);
extern void     iot_app_menu_item_setting_out1mode_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_out1mode_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_out2mode_cb_enter(void *item);
extern void     iot_app_menu_item_setting_out2mode_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_out2mode_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_nonc_output_cb_enter(void *item);
extern void     iot_app_menu_item_setting_nonc_output_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_nonc_output_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_response_time_cb_enter(void *item);
extern void     iot_app_menu_item_setting_response_time_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_response_time_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_screen_text_color_cb_enter(void *item);
extern void     iot_app_menu_item_setting_screen_text_color_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_screen_text_color_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_menu_item_setting_pressure_unit_cb_enter(void *item);
extern void     iot_app_menu_item_setting_pressure_unit_cb_exit(void *item);
extern uint8_t  iot_app_menu_item_setting_pressure_unit_cb_click_key(void *item, uint8_t keys);
extern void     iot_app_barometer_disp_screen_text_color(uint8_t enable);

/*************************************************-*************************************************
*                                             GLOBEL VARIBLE
**************************************************-*****************^******************************/
extern uint8_t iot_app_barometer_disp_task_id;


/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif

