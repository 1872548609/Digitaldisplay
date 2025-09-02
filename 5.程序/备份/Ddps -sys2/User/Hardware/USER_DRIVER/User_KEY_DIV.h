#ifndef _USER_KEY_DIV_H
#define _USER_KEY_DIV_H

#include "superhead.h"


// 按键消抖时间定义（单位：ms）
#define DEBOUNCE_TIME 20    // 20ms消抖阈值
#define LONG_PRESS_2S 1500  // 2秒长按时间阈值（单位：ms）
#define LONG_PRESS_4S 4000  // 4秒长按时间阈值（单位：ms）

#define KEY1 0x01
#define KEY2 0x02
#define KEY3 0x04

// 按键状态枚举定义
typedef enum {
    KEY_STATE_RELEASED,    // 按键释放状态
    KEY_STATE_PRESSED,     // 按键按下状态
    KEY_STATE_DEBOUNCING,   // 消抖中状态
				KEY_STATE_LONGPRESS_2S_MODE,
				KEY_STATE_LONGPRESS_4S_MODE,
} KeyState;

typedef struct 
{
				uint32_t Key_tick;
    uint8_t Key_State;
				uint16_t debounceCount;
				uint32_t Key_StartPressTime;
				uint8_t WichKeyDown;
				 union {
				      uint8_t memory;
						struct{				
										uint8_t key_pressEV:1;
						  	uint8_t Key_2sLongPressEV :1;
									uint8_t Key_4sLongPressEV :1;						
						}Key_Event_Bits;			
				}Key_Event_Ctr;
}Key_Management;

extern Key_Management key_management;

void DIV_Key_Init(void);

void Div_Key_Scan(void);
#endif
