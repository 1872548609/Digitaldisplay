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
#define IOT_SYSTICK_UPDATE_WITH_POLL
#define IOT_SYSTICK_MAX_MILLISECONDS                                2796

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
uint32_t iot_system_milliseconds_systick_backup = 0;


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

void init_systick_with_no_interrupt(void)
{
    //-- 此初始化不配置systick中断，当使用osal时，通过轮询方式查询SysTick->LOAD计算出时间
    //-- 这方式好处不用频繁中断，缺点就是查询时间间隔必须要小于SysTick->LOAD，代码就得要严格控制不能阻塞
    
    //-- 48M
    // Configure the SysTick to generate a time base equal to 1    ms -> SystemCoreClock / 1000
    // Configure the SysTick to generate a time base equal to 10   ms -> SystemCoreClock / 100
    // Configure the SysTick to generate a time base equal to 100  ms -> SystemCoreClock / 10

    //-- 6M
    // Configure the SysTick to generate a time base equal to 1    ms -> SystemCoreClock / 8 / 1000
    // Configure the SysTick to generate a time base equal to 10   ms -> SystemCoreClock / 8 / 100
    // Configure the SysTick to generate a time base equal to 100  ms -> SystemCoreClock / 8 / 10
    // Configure the SysTick to generate a time base equal to 1000 ms -> SystemCoreClock / 8
    
    // 6M频率，即每计数1为1/6000000s = 1/6000ms = 1/6us，计数6000为1ms，SysTick->LOAD最大为0xffffff，即最大计时为2,796.2025毫秒
    // 配置SysTick->LOAD为计时2796毫秒，2796 * 6000 = 16776000

    //-- SysTick_ConfigCLKSource(SysTick_CLKSource_HCLK);

    uint32_t ticks = (IOT_SYSTICK_MAX_MILLISECONDS * 6000);

    //-- 参照SysTick_Config
    if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)  
    {
        return;                                                     /* Reload value impossible */
    }
    
    SysTick->LOAD  = ticks - 1;                                     /* set reload register */
    SysTick->VAL   = 0UL;                                           /* Load the SysTick Counter Value */
    SysTick->CTRL  = /*SysTick_CTRL_CLKSOURCE_Msk |*/               //-- 去掉这一项，时钟源8分频
                     /*SysTick_CTRL_TICKINT_Msk   |*/               //-- 去掉这一项，时钟不产生中断
                     SysTick_CTRL_ENABLE_Msk;                       /* Enable SysTick IRQ and SysTick Timer */

    //-- 实现1秒的systick后，那么SysTick->LOAD的计数即为1秒，1毫秒为SysTick->LOAD / 1000，后面通过此值计算毫秒
    iot_systick_load = SysTick->LOAD;
    iot_systick_load_1ms = SystemCoreClock / 8 / 1000;
}

void init_systick_with_1s_interrupt(void)
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

    uint32_t ticks = SystemCoreClock / 8;

    //-- 修改里面函数使得时钟源8分频，实现1秒的systick中断，之所以要以秒为单位，主要中断不能太频繁
    #if 0
    {
        if (SysTick_Config(ticks))
        {
            /*
            SystemCoreClock / 8：
            假设 SystemCoreClock = 48 MHz，则 48 MHz / 8 = 6 MHz（即 SysTick 时钟为 6 MHz）。
            这样 LOAD = 6,000,000 - 1 时，SysTick 每 1 秒 触发一次中断（因为 6 MHz / 6,000,000 = 1 Hz）。
            */
            while (1);
        }
    }
    #else
    {
        //-- 参照SysTick_Config
        if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)  
        {
            return;                                                     /* Reload value impossible */
        }
        
        SysTick->LOAD  = ticks - 1;                                     /* set reload register */
        NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1UL);   /* set Priority for Systick Interrupt */
        SysTick->VAL   = 0UL;                                           /* Load the SysTick Counter Value */
        SysTick->CTRL  = /*SysTick_CTRL_CLKSOURCE_Msk |*/               //-- 去掉这一项，时钟源8分频
                         SysTick_CTRL_TICKINT_Msk   |
                         SysTick_CTRL_ENABLE_Msk;                       /* Enable SysTick IRQ and SysTick Timer */
    }
    #endif

    //-- 实现1秒的systick后，那么SysTick->LOAD的计数即为1秒，1毫秒为SysTick->LOAD / 1000，后面通过此值计算毫秒
    iot_systick_load = SysTick->LOAD;
    iot_systick_load_1ms = SysTick->LOAD / 1000;
}

void iot_driver_clock_update_system_time_with_1s_systick_interrupt(void)
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

uint16_t iot_driver_clock_update_osal_time(void)
{
    uint16_t elapsed_milliseconds = 0;
    uint32_t systick_value;

    
    #ifdef IOT_SYSTICK_UPDATE_WITH_POLL
    {
        uint8_t  systick_count_flag = 0;
        uint32_t milliseconds_temp = 0;
        
        // 进入临界区（禁用中断）
        //IOT_DISABLE_INTERRUPTS();
        
        // 获取当前 SysTick 值（倒计时，从 LOAD 递减到 0）
        systick_value = SysTick->VAL;
        systick_count_flag = (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)?1:0;
        if (systick_count_flag)
        {
            SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;
        }
        
        // 退出临界区（恢复中断）
        //IOT_ENABLE_INTERRUPTS();

        milliseconds_temp = (iot_systick_load - systick_value) / iot_systick_load_1ms;   //-- 计算当前周期的毫秒

        if (systick_count_flag)
        {
            elapsed_milliseconds = milliseconds_temp + (IOT_SYSTICK_MAX_MILLISECONDS - iot_system_milliseconds_systick_backup);
        }
        else
        {
            elapsed_milliseconds = milliseconds_temp - iot_system_milliseconds_systick_backup;
        }

        iot_system_milliseconds += elapsed_milliseconds;
        if (iot_system_milliseconds >= 1000)
        {
            iot_system_seconds += iot_system_milliseconds/1000;
            iot_system_milliseconds %= 1000;
        }

        iot_system_milliseconds_systick_backup = milliseconds_temp;
    }
    #else
    {
        uint32_t tick;
        uint32_t seconds_backup;
        uint32_t milliseconds_backup;
        
        seconds_backup = iot_system_seconds;
        milliseconds_backup = iot_system_milliseconds;

        // 进入临界区（禁用中断）
        IOT_DISABLE_INTERRUPTS();
        
        // 获取当前 SysTick 值（倒计时，从 LOAD 递减到 0）
        systick_value = SysTick->VAL;
        tick = iot_systick;                         //-- 读取中断计数（每次中断 +1）
        iot_systick = 0;                            //-- 清零中断计数
        
        // 退出临界区（恢复中断）
        IOT_ENABLE_INTERRUPTS();

        iot_systick_count += tick;                  //-- 总秒数增加
        iot_system_seconds = iot_systick_count;     //-- 更新秒
        iot_system_milliseconds = (iot_systick_load - systick_value) / iot_systick_load_1ms;   //-- 计算毫秒

        elapsed_milliseconds = (iot_system_seconds - seconds_backup) * 1000 + iot_system_milliseconds - milliseconds_backup;
    }
    #endif

    #if 0
    // 获取 SysTick 重装载值（1 秒对应的 ticks）
    uint32 loadValue = iot_systick_load; // 即 SystemCoreClock / 8（6 MHz 时为 6,000,000 - 1）
    uint32 tmp;
    
    // 计算自上次更新以来的 ticks 变化（处理倒计时和回绕）
    if (tick)
    {
        // 回绕情况：SysTick 从 0 重新加载 LOAD
        tmp = (loadValue - currentSysTick) + previousSysTick;
        
        // 加上中断触发的ticks（每次中断=1秒=iot_systick_load ticks）
        tmp += (tick-1) * iot_systick_load;
    }
    else
    {
        // 正常情况：SysTick 未回绕
        if (currentSysTick <= previousSysTick)
        {
            tmp = previousSysTick - currentSysTick;
        }
        else // 第一次进入这个函数
        {
            tmp = currentSysTick;
        }
    }
    
    // 更新 previousSysTick 为当前值（供下次计算）
    previousSysTick = currentSysTick;
        
    // 将 ticks 转换为毫秒（1 秒 = loadValue ticks，因此 1 ms = system_time_update）
    tmp += remTicks;
    
    elapsedMSec = (uint32)(tmp / iot_systick_load_1ms);
    
    remTicks = (uint32)(tmp % iot_systick_load_1ms);
    #endif

    return elapsed_milliseconds;
}

void iot_driver_clock_init(void)
{
    init_system_clk();
    #ifdef IOT_SYSTICK_UPDATE_WITH_POLL
    init_systick_with_no_interrupt();
    #else
    init_systick_with_1s_interrupt();
    #endif
}

/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
 
