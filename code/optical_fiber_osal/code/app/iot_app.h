
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
#define IOT_APP_TIMER_INTERVAL                                      1 // 定时器间隔

#define IOTAPP_DISPSECOND_EVT										0x0002
	
#define IOT_APP_LONGKEYSET_YCVALUE_EVT 								0x0004 // 长按设置应差事件	

#define IOT_APP_DELAYOUT1_EVT    									0x0008
#define IOT_APP_DELAYOUT2_EVT										0x0010	
	
/*************************************************-*************************************************
*                                             FUNCTION
**************************************************-*****************^******************************/
#define EPSILON 1e-6f   //误差容许
#define MIN_DECIMAL_PRECISION 0.0001f   // 最小小数精度（根据实际需求调整）

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


#define set_no  0x00 //开机无设置
#define set_P1 	0x01
#define set_P2 	0x02
#define set_Hi1 0x04
#define set_Lo1 0x08
#define set_Hi2 0x10
#define set_Lo2 0x20

extern uint8_t nowsetwhichyc;  //正在设置哪个

void iot_app_Poll(void);

// 气压读取
void pressure_readalways(void);


// 单位转换
float unitconversion(float value, uint8_t unit);

// 输出控制
void output_ctr_update(void);
void Yingcha_Comp(void);


// 输出显示
void output_screen_disupdate(void);

// 显示颜色联动
void ColorLinkTrun(void);

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

