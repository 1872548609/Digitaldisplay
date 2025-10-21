#ifndef _LIU_APP_H
#define _LIU_APP_H

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
#define LIU_APP_KEY_MASK            				(HAL_KEY_MODE|HAL_KEY_RIGHT_SUB|HAL_KEY_LEFT_ADD)	
/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
#define LIU_APP_TIMER_EVT                                           0x0001 // 定义测试事件
#define LIU_APP_TIMER_INTERVAL                                      5 // 定时器间隔


/*************************************************-*************************************************
*                                             FUNCTION
**************************************************-*****************^******************************/
#define BACKLIGHT_OFF       	0		// 关闭
#define BACKLIGHT_ON        	1		// 开启
#define BACKLIGHT_RED 			2		// 红色
#define BACKLIGHT_YELLOW		3		// 黄色
#define BACKLIGHT_GREEN			4		// 绿色

#define BACKLIGHT_LEVEL_HALF		10	// 半亮
#define BACKLIGHT_LEVEL_MAX			100	// 全亮

extern void iot_mainbacklight_set(uint8 color);		// 主屏背光设置
extern void iot_secondbacklight_set(uint8 en);		// 副屏背光设置
extern void iot_outputbacklight_set(uint8 en);		// 输出屏背光设置
extern void iot_backlight_levelset(uint8_t level);	// 背光亮度设置



typedef struct
{
	uint8 Zero_Calibration_bit;
	uint16 Zero_Calibration_500msTimeSwitch;
	float Zero_calibration_pressure;
	
}Zero_Calibration_typedef;

extern Zero_Calibration_typedef Zero_Calibration_struct;


void   liu_app_init(uint8 task_id);
uint16 liu_app_process_event(uint8 task_id, uint16 events);
void Zero_Calibration_Task(Zero_Calibration_typedef *pHandle);
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

