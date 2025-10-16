
#ifndef _IOT_APP_H
#define _IOT_APP_H

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
#define IOT_APP_KEY_MASK                                            (HAL_KEY_MODE|HAL_KEY_RIGHT_SUB|HAL_KEY_LEFT_ADD)

/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
#define IOT_APP_TIMER_EVT                                           0x0001 // 定义测试事件
#define IOT_APP_TIMER_INTERVAL                                      5 // 定时器间隔

#define IOT_APP_LONGKEYSET_YCVALUE_EVT 								0x0002 // 长按设置应差事件	
#define IOT_APP_SECONDSCREEN_DISP_EVT								0x0004 // 副屏显示事件	
#define IOT_APP_MAINSCREEN_DISP_EVT									0x0008	// 主屏显示

/*************************************************-*************************************************
*                                             FUNCTION
**************************************************-*****************^******************************/

float unitconversion(float value, uint8_t unit);

// 主屏显示
void main_screen_dispaftertime(uint16_t time,const char * data1,...);
void main_screen_disp(const char * data1,...);
void main_screen_dispfloat(const char * data1,...);

// 副屏显示
void second_screen_dispaftertime(uint16_t time,const char * data1,...);
void second_screen_disp(const char * data1,...);
void second_screen_dispfloat(const char * data1,...);
	
extern void   iot_app_init(uint8 task_id);
extern uint16 iot_app_process_event(uint8 task_id, uint16 events);

/*************************************************-*************************************************
*                                             GLOBEL VARIBLE
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif

