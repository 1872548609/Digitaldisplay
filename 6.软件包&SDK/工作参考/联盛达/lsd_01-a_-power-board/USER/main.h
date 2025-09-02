#ifndef MAINT_H_
#define MAINT_H_

#include "DataType.h"
#include "OB38R16T1.h"
#include "Timer.h"
#include "Gpio.h"
// #include "Display.h"
#include "Logic.h"
// #include "Key.h"
#include "ADC.h"
#include "Flow.h"
#include "uart.h"
#include "Sensor.h"


#include "math.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "intrins.h"

#define Low_ON      0x00 //低开
#define High_OFF    0x01 //高关

#define Low_OFF     0x00 //低关
#define High_ON     0x01 //高开

#define PERIOD_20MS 8 
#define All_Down    0 //全关
#define FULL_POWER  1 //全功率
#define REDUCTION   2 //降功率
#define UART_TIMEOUT_COUNT  3000 // 超时时间30S
extern volatile LoopUartType   LoopUart;
extern u16 volatile UartTimeOutCount ; // 超时计数器

void Lamp_Ctrl();

#endif // !MAINT_H_

