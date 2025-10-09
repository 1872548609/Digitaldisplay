
/* 防止头文件重复包含 */
#ifndef ONBOARD_H
#define ONBOARD_H

/* C++兼容性支持 */
#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/* 包含必要的硬件抽象层和操作系统抽象层头文件 */
#include "hal_types.h"
#include "hal_mcu.h"
#include "hal_uart.h"
#include "hal_sleep.h"
#include "hal_drivers.h"
#include "osal.h"

/*********************************************************************
 * GLOBAL VARIABLES
 */

/* 定义设备的64位扩展地址 */
extern uint8 aExtendedAddress[8];

/*********************************************************************
 * CONSTANTS
 */

/* 定时器时钟和省电定义 */
#define TIMER_DECR_TIME    1  /* 1ms - 必须与TC_OCC匹配 */

/* OSAL定时器定义 */
#define TICK_TIME   1000   /* 每个时钟滴答的时间(微秒) */
/*
  Timer4使用1/128预分频器在1.0毫秒时中断
  TICK_COUNT = (CPUMHZ / 128) / 1000
*/
#define TICK_COUNT  1  /* 32 Mhz输出比较计数 */

/* CC2430定义 */

/* MEMCTR位定义 */
#define ALWAYS1    0x01  /* 保留位：始终=1 */
#define CACHDIS    0x02  /* Flash缓存：禁用=1 */
#define FMAP0      0x10  /* Flash银行映射，位0 */
#define FMAP1      0x20  /* Flash银行映射，位1 */
#define FMAP       0x30  /* Flash银行映射，掩码 */
#define MUNIF      0x40  /* 内存映射：统一=1 */

/* PCON位定义 */
#define PMODESET   0x01  /* 电源模式控制：1=设置PMx */

/* 复位位定义 */
#define LRESET     0x18  /* 最后复位位掩码 */
#define RESETPO    0x00  /* 上电复位 */
#define RESETEX    0x08  /* 外部复位 */
#define RESETWD    0x10  /* 看门狗复位 */

/* GPIO端口定义 */
/* GPIO位定义 */
#define GPIO_0     0x01  /* Px_0: GPIO=0, PIO=1 */
#define GPIO_1     0x02  /* Px_1: GPIO=0, PIO=1 */
#define GPIO_2     0x04  /* Px_2: GPIO=0, PIO=1 */
#define GPIO_3     0x08  /* Px_3: GPIO=0, PIO=1 */
#define GPIO_4     0x10  /* Px_4: GPIO=0, PIO=1 */
#define GPIO_5     0x20  /* Px_5: GPIO=0, PIO=1 */
#define GPIO_6     0x40  /* Px_6: GPIO=0, PIO=1 */
#define GPIO_7     0x80  /* Px_7: GPIO=0, PIO=1 */

/* 看门狗定时器定义 */
/* WDCTL位定义 */

#define WDINT0     0x01  /* 间隔，位0 */
#define WDINT1     0x02  /* 间隔，位1 */
#define WDINT      0x03  /* 间隔，掩码 */
#define WDMODE     0x04  /* 模式：看门狗=0，定时器=1 */
#define WDEN       0x08  /* 定时器：禁用=0，启用=1 */
#define WDCLR0     0x10  /* 清除定时器，位0 */
#define WDCLR1     0x20  /* 清除定时器，位1 */
#define WDCLR2     0x40  /* 清除定时器，位2 */
#define WDCLR3     0x80  /* 清除定时器，位3 */
#define WDCLR      0xF0  /* 清除定时器，掩码 */

/* WD定时器间隔 */
#define WDTISH     0x03  /* 短：clk * 64 */
#define WDTIMD     0x02  /* 中：clk * 512 */
#define WDTILG     0x01  /* 长：clk * 8192 */
#define WDTIMX     0x00  /* 最大：clk * 32768 */

/* WD清除定时器模式 */
#define WDCLP1     0xA0  /* 清除模式1 */
#define WDCLP2     0x50  /* 清除模式2 */

/*********************************************************************
 * MACROS
 */

/* 这些按键定义是此开发系统独有的，用于启动设备时绕过某些功能 */
#define SW_BYPASS_NV    HAL_KEY_SW_5  /* 绕过网络层NV恢复 */
#define SW_BYPASS_START HAL_KEY_SW_1  /* 绕过网络初始化 */

/* LCD支持定义 */
#ifdef LCD_SUPPORTED
  #if !defined DEBUG
    #define DEBUG  0  /* 如果未定义DEBUG，则默认为0 */
  #endif
  #if LCD_SUPPORTED==DEBUG
    #define SERIAL_DEBUG_SUPPORTED  /* 串行调试支持 */
  #endif
#else /* 无LCD支持 */
  #undef SERIAL_DEBUG_SUPPORTED  /* 无串行调试支持 */
#endif

/* 串口定义 */
#if defined (ZAPP_P1)
  #define ZAPP_PORT HAL_UART_PORT_0  /* ZAPP使用端口0 */
#elif defined (ZAPP_P2)
  #define ZAPP_PORT HAL_UART_PORT_1  /* ZAPP使用端口1 */
#else
  #undef ZAPP_PORT  /* 未定义ZAPP端口 */
#endif
#if defined (ZTOOL_P1)
  #define ZTOOL_PORT HAL_UART_PORT_0  /* ZTOOL使用端口0 */
#elif defined (ZTOOL_P2)
  #define ZTOOL_PORT HAL_UART_PORT_1  /* ZTOOL使用端口1 */
#else
  #undef ZTOOL_PORT  /* 未定义ZTOOL端口 */
#endif

/* MT UART缓冲区大小定义 */
#define MT_UART_TX_BUFF_MAX  128  /* 发送缓冲区最大大小 */
#define MT_UART_RX_BUFF_MAX  128  /* 接收缓冲区最大大小 */
#define MT_UART_THRESHOLD   (MT_UART_RX_BUFF_MAX / 2)  /* 阈值 */
#define MT_UART_IDLE_TIMEOUT 6  /* 空闲超时 */

/* 从绝对开始重新启动系统，禁用中断，强制看门狗复位 */
#define SystemReset()       \
{                           \
  HAL_DISABLE_INTERRUPTS(); \
  HAL_SYSTEM_RESET();       \
}

/* 软件复位 */
#define SystemResetSoft()  Onboard_soft_reset()

/* 获取复位原因用于复位指示 */
#define ResetReason() ((SLEEPSTA >> 3) & 0x03)

/* 看门狗定时器使能 */
#define WatchDogEnable(wdti)              \
{                                         \
  WDCTL = WDCLP1 | WDEN | (wdti & WDINT); \
  WDCTL = WDCLP2 | WDEN | (wdti & WDINT); \
}

/* 等待指定的微秒数 */
#define MicroWait(t) Onboard_wait(t)

/* 设置CPU进入睡眠状态 */
#define OSAL_SET_CPU_INTO_SLEEP(timeout) halSleep(timeout); /* 由OSAL_PwrMgr调用 */

///* IAR编译器特定定义 */
//#ifdef __IAR_SYSTEMS_ICC__
///* 内部(MCU)堆栈地址 */
//#define CSTACK_BEG ((uint8 const *)(_Pragma("segment=\"XSTACK\"") __segment_begin("XSTACK")))
//#define CSTACK_END ((uint8 const *)(_Pragma("segment=\"XSTACK\"") __segment_end("XSTACK"))-1)
///* 堆栈初始化值 */
//#define STACK_INIT_VALUE  0xCD
//#else
//#error 检查编译器兼容性。
//#endif

/* 以下堆大小是为典型的TI示例应用程序设置的，应根据系统要求进行调整 */
#if !defined INT_HEAP_LEN
#if defined RTR_NWK
  #define INT_HEAP_LEN  3072  /* 路由器网络堆大小 */
#else
  #define INT_HEAP_LEN  2048  /* 默认堆大小 */
#endif
#endif
#define MAXMEMHEAP INT_HEAP_LEN  /* 最大内存堆大小 */

/* 按键变化索引定义 */
#define KEY_CHANGE_SHIFT_IDX 1  /* shift键索引 */
#define KEY_CHANGE_KEYS_IDX  2  /* keys键索引 */

/* 初始化级别 */
#define OB_COLD  0  /* 冷启动 */
#define OB_WARM  1  /* 暖启动 */
#define OB_READY 2  /* 就绪状态 */

#ifdef LCD_SUPPORTED
  /* 蜂鸣器控制定义 */
  #define BUZZER_OFF  0  /* 关闭蜂鸣器 */
  #define BUZZER_ON   1  /* 打开蜂鸣器 */
  #define BUZZER_BLIP 2  /* 蜂鸣器短响 */
#endif

/* 按键变化事件结构体 */
typedef struct
{
  osal_event_hdr_t hdr;  /* 事件头 */
  uint8 state; // shift  /* shift状态 */
  uint8 keys;  // keys   /* 按键状态 */
} keyChange_t;

/*********************************************************************
 * FUNCTIONS
 */

  /*
   * 初始化外设
   *    level: 0=冷启动, 1=暖启动, 2=就绪
   */
  extern void InitBoard( uint8 level );

 /*
  * 获取已过去的定时器时钟计数
  */
  extern uint32 TimerElapsed( void );

  /*
   * 注册所有按键事件
   */
  extern uint8 RegisterForKeys( uint8 task_id );

/* 键盘控制函数 */

  /*
   * 向应用程序发送"按键按下"消息
   */
  extern uint8 OnBoard_SendKeys( uint8 keys, uint8 shift );

/* LCD模拟/控制函数 */
  /*
   * 将整数转换为ASCII字符串
   */
  extern void _itoa( uint16 num, uint8 *buf, uint8 radix );


  extern void Dimmer( uint8 lvl );  /* 调光函数 */

/* 外部I/O处理函数 */
  /*
   * 打开外部灯
   */
  extern void BigLight_On( void );

  /*
   * 关闭外部灯
   */
  extern void BigLight_Off( void );

  /*
   * 控制外部蜂鸣器开关
   *   on: BUZZER_ON 或 BUZZER_OFF
   */
  extern void BuzzerControl( uint8 on );

  /*
   * 获取外部DIP开关的设置
   */
  extern uint8 GetUserDipSw( void );

  /*
   * 计算已使用的堆栈大小
   */
  extern uint16 OnBoard_stack_used( void );

  /*
   * 按键回调函数
   */
  extern void OnBoard_KeyCallback ( uint8 keys, uint8 state );

  /*
   * 板级随机数生成器
   */
  extern uint16 Onboard_rand( void );

  /*
   * 板级微秒级等待
   */
  extern void Onboard_wait( uint16 timeout );

//  /*
//   * 板级软件复位
//   */
//  extern __near_func void Onboard_soft_reset( void );

/*********************************************************************
*********************************************************************/

/* C++兼容性支持结束 */
#ifdef __cplusplus
}
#endif

/* 头文件保护结束 */
#endif // ONBOARD_H
