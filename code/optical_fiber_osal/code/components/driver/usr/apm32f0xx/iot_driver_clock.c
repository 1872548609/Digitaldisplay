/**************************************************************************************************
Filename:       iot_driver_clock.c
Revised:        $Date: 2019-8-20 15:35
Revision:       $Revision: V1.0.0

Description:

Copyright 2021 uwaycon. All rights reserved.
                                                
**************************************************-*****************^******************************/

#ifdef __cplusplus
extern "C"
{
#endif

/*************************************************-*************************************************
*                                               NOTE
**************************************************-*****************^******************************/
//-- source insight use UTF-8 to encode, File->Reload As Encoding...->UTF-8
//-- source insight set default encode, Option->Preferences->Files->Default encoding->UTF-8

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/
#include "main.h"

/*************************************************-*************************************************
*                                               MACRO
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          CONSTANT DEFINED
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                           STRUCT DEFINED
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          GLOBAL VARIABLES
**************************************************-*****************^******************************/
uint32_t iot_systick = 0;               // 中断计数器（每次中断递增 1）
uint32_t iot_systick_count = 0;         // 系统运行的总秒数
uint32_t iot_systick_load = 0;          // SysTick 的 LOAD 值
uint32_t iot_systick_load_1ms = 0;      // 1 毫秒对应的计数值（用于计算毫秒部分）
uint32_t iot_system_seconds = 0;        // 系统时间（秒）
uint32_t iot_system_milliseconds = 0;   // 系统时间（毫秒）

/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/
void init_system_clk(void)
{
    //默认初始化，使用内部时钟初始化为48mhz
    /** Set HSIEN bit */   
    RCM->CTRL1_B.HSIEN = BIT_SET;
    /** Reset SCLKSEL, AHBPSC, APB1PSC, APB2PSC, ADCPSC and COC bits */
    RCM->CFG1 &= (uint32_t)0x08FFB80CU;
    /** Reset HSEEN, CSSEN and PLLEN bits */
    RCM->CTRL1 &= (uint32_t)0xFEF6FFFFU;
    /** Reset HSEBCFG bit */
    RCM->CTRL1_B.HSEBCFG = BIT_RESET;
    /** Reset PLLSRCSEL, PLLHSEPSC, PLLMULCFG bits */
    RCM->CFG1 &= (uint32_t)0xFFC0FFFFU;
    /** Reset PREDIV[3:0] bits */
    RCM->CFG1 &= (uint32_t)0xFFFFFFF0U;
    /** Reset USARTSW[1:0], I2CSW, CECSW and ADCSW bits */
    RCM->CFG3 &= (uint32_t)0xFFFFFEAC;
    /** Reset  HSI14 bit */
    RCM->CTRL2_B.HSI14EN = BIT_RESET;
    /** Disable all interrupts */
    RCM->INT = 0x00000000U;
    
    //设置时钟源为HSI内部RC输出48MHZ到HCLK
    uint32_t i;

    RCM->CTRL1_B.HSEEN= BIT_RESET;

    //等待HSI时钟稳定
    for (i = 0; i < HSI_STARTUP_TIMEOUT; i++)
    {
        if (RCM->CTRL1_B.HSIRDYFLG)
        {
            break;
        }
    }
    
    //如果时钟稳定了才开始设置
    if (RCM->CTRL1_B.HSIRDYFLG)
    {
        /** Enable Prefetch Buffer */
        FMC->CTRL1_B.PBEN = BIT_SET;
        /** Flash 1 wait state */
        FMC->CTRL1_B.WS = 1;

        /** HCLK = SYSCLK */
        RCM->CFG1_B.AHBPSC= 0X00;

        /** PCLK = HCLK */
        RCM->CFG1_B.APB1PSC = 0X00;

        /** PLL: HSI /2 * 12 */
        RCM->CFG1_B.PLLSRCSEL = 0;
        RCM->CFG1_B.PLLMULCFG = 0xA;

        /** Enable PLL */
        RCM->CTRL1_B.PLLEN = 1;

        /** Wait PLL Ready */
        while (RCM->CTRL1_B.PLLRDYFLG == BIT_RESET);

        /** Select PLL as system clock source */
        RCM->CFG1_B.SCLKSEL = 2;

        /** Wait till PLL is used as system clock source */
        while (RCM->CFG1_B.SCLKSWSTS!= 0x02);
    }
}

void init_systick(void)
{
    //-- 48M
    // Configure the SysTick to generate a time base equal to 1    ms -> SystemCoreClock / 1000
    // Configure the SysTick to generate a time base equal to 10   ms -> SystemCoreClock / 100
    // Configure the SysTick to generate a time base equal to 100  ms -> SystemCoreClock / 10

    //-- 6M
    // Configure the SysTick to generate a time base equal to 1    ms -> SystemCoreClock / 8 / 1000
    // Configure the SysTick to generate a time base equal to 10   ms -> SystemCoreClock / 8 / 100
    // Configure the SysTick to generate a time base equal to 100  ms -> SystemCoreClock / 8 / 10
    // Configure the SysTick to generate a time base equal to 1000 ms -> SystemCoreClock / 8

    //-- SysTick_ConfigCLKSource(SysTick_CLKSource_HCLK);

    //-- 修改里面函数使得时钟源8分频，实现1秒的systick中断，之所以要以秒为单位，主要中断不能太频繁
    if (SysTick_Config(SystemCoreClock / 8))
    {
        /*
        SystemCoreClock / 8：
        假设 SystemCoreClock = 48 MHz，则 48 MHz / 8 = 6 MHz（即 SysTick 时钟为 6 MHz）。
        这样 LOAD = 6,000,000 - 1 时，SysTick 每 1 秒 触发一次中断（因为 6 MHz / 6,000,000 = 1 Hz）。
        */
        while (1);
    }

    //-- 实现1秒的systick后，那么SysTick->LOAD的技术即为1秒，1毫秒为SysTick->LOAD / 1000，后面通过此值计算毫秒
    iot_systick_load = SysTick->LOAD;
    iot_systick_load_1ms = iot_systick_load / 1000;
}

void iot_driver_clock_init(void)
{
    init_system_clk();
    init_systick();
}

void iot_driver_clock_update_system_time(void)
{
    uint32_t tick;
    uint32_t systick_value;
    
    HAL_DISABLE_INTERRUPTS();       // 关闭中断，防止竞争
    tick = iot_systick;             // 读取中断计数（每次中断 +1）
    iot_systick = 0;                // 清零中断计数
    systick_value = SysTick->VAL;   // 读取当前 SysTick 计数器值
    HAL_ENABLE_INTERRUPTS();        // 重新开启中断

    //-- 1000ms一个tick
    iot_systick_count += tick;                  // 总秒数增加
    iot_system_seconds = iot_systick_count;     // 更新秒
    iot_system_milliseconds = (iot_systick_load - systick_value) / iot_systick_load_1ms;   // 计算毫秒
}

/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
 
