#include "Gpio.h"


void Gpio_init(void)
{
    GPIO_IN_P2(4);
    GPIO_IN_P0(4);
    GPIO_IN_P0(5);
    GPIO_IN_P0(6);
    GPIO_IN_P0(7);
    GPIO_IN_P1(0);
    GPIO_IN_P1(7);

    GPIO_IN_P3(2);
 
    GPIO_IN_P1(6);

    GPIO_RESET_P1(5);
    GPIO_RESET_P1(4);

    GPIO_RESET_P2(6);
    GPIO_RESET_P2(5);

    GPIO_RESET_P3(0);
    GPIO_RESET_P3(1);

    GPIO_IN_P3(3);
    GPIO_IN_P3(4);
}
