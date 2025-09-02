#ifndef GPIO_H_
#define GPIO_H_

#include "main.h"


//PxM1.y 	PxM0.y
//	0		0			准双向口
//	0		1			推完输出
//	1		0			只输入
//	1		1			开漏

//推挽输出设置
#define GPIO_OUT_P0(x)  do{ P0M1 &=~(1<<(x)); P0M0 |=(1<<(x)); }while(0);
#define GPIO_OUT_P1(x)  do{ P1M1 &=~(1<<(x)); P1M0 |=(1<<(x)); }while(0);
#define GPIO_OUT_P2(x)  do{ P2M1 &=~(1<<(x)); P2M0 |=(1<<(x)); }while(0);
#define GPIO_OUT_P3(x)  do{ P3M1 &=~(1<<(x)); P3M0 |=(1<<(x)); }while(0);
//只输入设置
#define GPIO_IN_P0(x)   do{ P0M1 |=(1<<(x)); P0M0 &=~(1<<(x)); }while(0);
#define GPIO_IN_P1(x)   do{ P1M1 |=(1<<(x)); P1M0 &=~(1<<(x)); }while(0);
#define GPIO_IN_P2(x)   do{ P2M1 |=(1<<(x)); P2M0 &=~(1<<(x)); }while(0);
#define GPIO_IN_P3(x)   do{ P3M1 |=(1<<(x)); P3M0 &=~(1<<(x)); }while(0);
//准双向口设置
#define GPIO_RESET_P0(x)   do{ P0M1 &=~(1<<(x)); P0M0 &=~(1<<(x)); }while(0);
#define GPIO_RESET_P1(x)   do{ P1M1 &=~(1<<(x)); P1M0 &=~(1<<(x)); }while(0);
#define GPIO_RESET_P2(x)   do{ P2M1 &=~(1<<(x)); P2M0 &=~(1<<(x)); }while(0);
#define GPIO_RESET_P3(x)   do{ P3M1 &=~(1<<(x)); P3M0 &=~(1<<(x)); }while(0);

#define PowerKey    P2_4 //电源按键
#define GrindKey    P0_4 //磨豆按键
#define TempKey     P0_5 //温度调节按键
#define CleanKey    P0_6 //清洗按键
#define SteamKey    P0_7 //蒸汽按键
#define HotWaterKey P1_0 //热水按键
#define ExtractKey  P1_7 //萃取按键

//蒸汽旋钮
#define KS          P3_2   

//蜂鸣器
#define BUZ         P1_6 

//通讯口
#define RX          P1_5
#define TX          P1_4

//调试口
#define RX_1        P2_6 
#define TX_1        P2_5

//AIP通讯
#define CLK         P3_0
#define DIN         P3_1

//编码器
#define EnCoder_A    P3_3
#define EnCoder_B    P3_4

void Gpio_init(void);

#endif // !GPIO_H_