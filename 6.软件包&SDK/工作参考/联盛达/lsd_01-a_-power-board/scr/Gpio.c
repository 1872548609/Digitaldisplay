#include "Gpio.h"

void Gpio_init(void)
{
    //输出
    GPIO_OUT_P3(0);
    GPIO_OUT_P3(1);
    GPIO_OUT_P3(2);
	GPIO_OUT_P3(3);
    GPIO_OUT_P3(4);
    GPIO_OUT_P1(2);
    GPIO_OUT_P1(3);
    GPIO_OUT_P1(6);
    GPIO_OUT_P1(7);
    GPIO_OUT_P0(7);

    //传感器
    GPIO_IN_P0(3);
    GPIO_IN_P0(5);
    GPIO_IN_P0(6);
    GPIO_IN_P1(0);
    GPIO_IN_P3(5);

    //串口
    GPIO_RESET_P2(5);
    GPIO_RESET_P2(6);

    //通讯
    GPIO_RESET_P1(5);
    GPIO_RESET_P1(4);

    SCR1 = High_OFF; 
    SCR2 = High_OFF; 
    SCR3 = High_OFF; 
    RL1  = Low_OFF;
    RL2  = Low_OFF;
    RL3  = Low_OFF;
    RL4  = Low_OFF;
    RL5  = Low_OFF;
    RL6  = Low_OFF;

    RXD  =   1;
    TXD  =   1;
    RXD0 =   1;
    TXD0 =   1;
}
