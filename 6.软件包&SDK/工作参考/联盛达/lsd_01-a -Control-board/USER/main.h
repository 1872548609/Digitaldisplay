#ifndef MAINT_H_
#define MAINT_H_

#include "DataType.h"
#include "OB38R16T1.h"
#include "Timer.h"
#include "Gpio.h"
#include "Display.h"
#include "Key.h"
#include "PWM.h"
#include "uart.h"
#include "Logic.h"
#include "Sensor.h"
#include "SelCheck.h"
#include "EEPROM.h"
#include "LVI_LVR.h"


#include "math.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "intrins.h"

#define NoWork      0x01    //无工作
#define NoLoad      0x02    //空转
#define Grinding    0x04    //磨豆
#define Stall       0x08    //堵转

#define DirOutPut_OFF 0x00
#define DirOutPut_ON  0x01

#define OFF 0x00
#define ON  0x01

#define Flow_Scan       0x01
#define Flow_End        0x02

#define Delay_30s       3000 
#define Delay_10s       1000 
#define Delay_1S        100 
#define Delay_500ms     50 
#define Delay_250ms     25 

#define CleanFlow       35000 //清洁流量
#define PulsePerMl      2    //每升多少脉冲

#define MaxCoffeTemp    98//
#define MinCoffeTemp    90//

#define DefCoffeTemp    93

#define All_Down    0
#define FULL_POWER  1 //全功率
#define REDUCTION   2 //降功率

#define MaxGrindTime    60  //最大研磨时间
#define MinGrindTime    2   //最小研磨时间

#define MaxSteamTime    180//最大蒸汽时间
#define MaxWaterTime    120//最大热水时间

#define SHORTERR  0xFF //短路
#define BROKENERR 0xFE //断路

#define PreSingleEspTime 3    // 单杯浓缩预注水时间
#define PreDoubleEspTime 5    // 双杯浓缩预注水时间
#define PreCoffeeTime    4    // 咖啡预注水时间
#define PreAmericanoTime 3    // 美式咖啡预注水时间
#define perEspCoolTime   3    // 浓缩预注水时间
// #define PreColdBrewTime  0 // 冷沁预注水时间

#define FlowSingleEspe  60    // 单杯浓缩流量
#define FlowDoubleEsp   120   // 双杯浓缩流量
#define FlowCoffee      90    // 咖啡流量
#define FlowAmericano   60    // 美式咖啡流量
#define FlowColdBrew    125   // 冷沁流量
#define FlowEspCool     60    // 浓缩流量

#define SleepTime       720000//休眠时间30分钟


#define DEBUG_TEMP      0 //调试显示实时温度

extern DutyCycleOutputType  xdata   PumpCtrl            ;  
extern DutyCycleOutputType  xdata   PotHeatCtrl         ;  
extern DirectOutputType     xdata   DirectOutput        ;
extern FlowCtrlTypedef      xdata   FlowCtrl            ;
extern DeviceStatusTypedef  xdata   DeviceStatus        ;
extern AdParaTypedef        xdata   AdPara              ;
extern LampCtrlTypedef		xdata   LampCtrl		  	;
extern volatile LoopUartType xdata	LoopUart			;

extern u8 xdata curTempUnit       ;//当前温度单位
extern u8 xdata UartFlag          ;
extern u8 xdata CurPotAimTemp     ;
extern u8 xdata CurFuntion        ;
extern u8 xdata MakeFlow          ;
extern u8 xdata GrindTime         ;
extern u8 xdata PreInjectionTime  ;
extern u8 xdata ErrCode           ; 
extern u8 xdata TempSingleEsp     ;
extern u8 xdata TempDoubleEsp     ;
extern u8 xdata TempCoffee        ;
extern u8 xdata TempAmericano     ;
extern u8 xdata TempColdDrink     ;
extern u8 xdata EndDis            ; //结束显示
extern u8 * CurSettingTemp   	  ;
extern u16 xdata CurFlow          ;
extern u16 xdata TotalFlow        ;
extern u32 xdata SleepCut         ; //休眠计数   
#endif // !MAINT_H_

