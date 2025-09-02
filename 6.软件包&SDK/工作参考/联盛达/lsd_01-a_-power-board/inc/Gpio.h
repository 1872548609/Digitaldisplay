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

//输出
#define SCR1        P3_0        //加热1
#define SCR2        P3_1        //咖啡水泵
#define SCR3        P3_2        //蒸汽水泵 
#define RL1         P3_3        //磨豆电机
#define RL2         P3_4        //加热2
#define RL3         P1_2        //电磁阀1
#define RL4         P1_3        //电磁阀2
#define RL5         P1_6        //电磁阀3
#define RL6         P1_7        //等离子发生器
#define LAMP        P0_7        //照明

//传感器
#define ADC_NTC1    P0_3       //NTC1
#define ADC_NTC2    P0_5       //NTC2
#define ADC_WL      P0_6       //电机堵转检测
#define PRESS       P1_0       //压力
#define FLOWMETER   P3_5       //流量计

//调试口
#define RXD         P2_6 
#define TXD         P2_5 

//通讯口
#define RXD0        P1_5 
#define TXD0        P1_4 

void Gpio_init(void);

#endif // !GPIO_H_