#ifndef _USER_KEY_DIV_H
#define _USER_KEY_DIV_H

#include "superhead.h"


// 按键消抖时间定义（单位：ms）
#define DEBOUNCE_TIME 20    // 20ms消抖阈值
#define LONG_PRESS_2S 2000  // 2秒长按时间阈值（单位：ms）
#define LONG_PRESS_4S 4000  // 4秒长按时间阈值（单位：ms）

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
    uint8_t Key1_State;
				uint16_t debounceCount;
				uint32_t Key1_StartPressTime;
}Key_Management;

extern Key_Management key_management;

void Div_Key_Scan(void);
#endif
