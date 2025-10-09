/*********************************************************************
 * INCLUDES
 */

// 包含通用定义头文件
#include "comdef.h"
// 包含硬件抽象层头文件
#include "OnBoard.h"
// 包含操作系统抽象层头文件
#include "OSAL.h"
// 包含时钟管理模块头文件
#include "OSAL_Clock.h"
#include "main.h"

/*********************************************************************
 * MACROS
 */

// 根据是否为闰年返回一年天数（365或366）
#define YearLength(yr) ((uint16)(IsLeapYear(yr) ? 366 : 365)) 

/*********************************************************************
 * CONSTANTS
 */

// UTC起始年份定义为2000年
#define BEGYEAR  2000      // UTC起始年份
// 定义一天的秒数（24*60*60=86400）
#define DAY      86400UL   // 一天的秒数（24*60*60）

/*********************************************************************
 * TYPEDEFS
 */
// 类型定义部分（当前为空）

/*********************************************************************
 * GLOBAL VARIABLES
 */
// 全局变量部分（当前为空）

/*********************************************************************
 * EXTERNAL VARIABLES
 */
// 外部变量部分（当前为空）

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
// 声明外部函数，用于获取MAC层高精度定时器的当前计数值（320μs间隔）
extern uint32 macMcuPrecisionCount(void); 

// 针对特定MCU（如CC2530）的硬件优化除法函数
#if (defined HAL_MCU_CC2430) || (defined HAL_MCU_CC2530) || (defined HAL_MCU_CC2533)
  /* 
   * 该函数用于将31位被除数除以16位除数，返回16位商和16位余数的打包结果
   * 参数：
   *   dividend - 31位被除数
   *   divisor - 16位除数
   * 返回：
   *   高16位是商，低16位是余数
   */
  //extern __near_func uint32 osalMcuDivide31By16To16( uint32 dividend, uint16 divisor );

  // 定义宏：将320μs ticks转换为毫秒并处理余数（使用硬件优化除法）
  #define CONVERT_320US_TO_MS_ELAPSED_REMAINDER( x, y, z ) st( \
    /* 调用硬件优化除法函数，x存储商(MSW)和余数(LSW) */ \
    x = osalMcuDivide31By16To16( x, 25 );                      \
    /* 将商(高16位)加到y（毫秒计数） */ \
    y += (x >> 16);                                            \
    /* 将余数(低16位)存入z */ \
    z = (uint16)(x & 0x0FFFF);                                 \
  )

#else /* 其他MCU使用标准除法 */
  // 定义宏：将320μs ticks转换为毫秒并处理余数（使用标准除法）
  #define CONVERT_320US_TO_MS_ELAPSED_REMAINDER( x, y, z ) st( \
    /* 标准除法计算毫秒数 */ \
    y += x / 25;                                               \
    /* 标准取模计算余数 */ \
    z = x % 25;                                                \
  )
#endif

/*********************************************************************
 * LOCAL VARIABLES
 */
/*
 * 静态变量定义：
 * previousMacTimerTick：记录上一次的定时器计数值
 * remUsTicks：存储320μs ticks转换后的余数（用于精确毫秒计算）
 * timeMSec：当前毫秒计数（未满1秒时暂存）
 * OSAL_timeSeconds：全局UTC时间（秒级）
 */
//static uint32 previousMacTimerTick = 0;
//static uint16 remUsTicks = 0;
static uint16 timeMSec = 0;
// 2000年1月1日0时0分0秒以来的UTC时间（秒）
UTCTime OSAL_timeSeconds = 0;

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */
// 声明静态函数：计算指定年份和月份的天数
static uint8 monthLength( uint8 lpyr, uint8 mon );
// 声明静态函数：更新OSAL时钟
static void osalClockUpdate( uint16 elapsedMSec );

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osalTimeUpdate
 *
 * @brief   使用MAC退避定时器的自由运行计数（320μs间隔），将tick计数转换为毫秒，
 *          并更新OSAL时钟和定时器。应在后台调用，而非中断中。
 *
 * @param   无
 *
 * @return  无
 */
//void osalTimeUpdate( void )
//{
//	// 保存中断状态
//	halIntState_t intState;
//	// 临时变量
//	uint32 tmp;
//	// 存储320μs ticks数
//	uint32 ticks320us;


//	// 存储转换后的毫秒数
//	uint16 elapsedMSec = 0;

//	// 进入临界区（禁用中断）
//	HAL_ENTER_CRITICAL_SECTION(intState);

//	// 获取MAC高精度定时器的当前计数值硬件+软件累计值
//	 tmp = macMcuPrecisionCount();

//	// 退出临界区（恢复中断）
//	HAL_EXIT_CRITICAL_SECTION(intState);

//	// 检查定时器计数是否变化
//	if ( tmp != previousMacTimerTick )
//	{
//	// 计算自上次以来的ticks数（处理32位回绕）
//	// 无符号运算规则：实际计算的是 (a - b) mod 2^32。
//	// 结果：0x00000002 + (0xFFFFFFFF - 0xFFFFFFFE + 1) = 0x00000004（即 4）。
//	// 虽然 uint32_t 的减法本身已经模 2^32处理了回绕，但在某些情况下（如 混合符号运算 或 编译器优化），
//	// 直接相减可能会被提升为 int（有符号整数），导致意外行为。
//	// 所以这里要限定范围
//	ticks320us = (tmp - previousMacTimerTick) & 0xffffffffu;
//	  
//	
//	// 保存当前ticks值供下次使用
//	previousMacTimerTick = tmp;

//	// 合并新ticks和剩余余数ticks（*8是因为320/1000=0.32=8/25，1ms=3.125*320μs）
//	 tmp = (ticks320us * 8) + remUsTicks;

//	// 将320μs ticks转换为毫秒和余数（使用前面定义的宏）
//	 CONVERT_320US_TO_MS_ELAPSED_REMAINDER( tmp, elapsedMSec, remUsTicks );
//	  
//	
//	// 如果有毫秒时间变化，更新OSAL时钟和定时器
//	if ( elapsedMSec )
//	{
//	  osalClockUpdate( elapsedMSec );
//	  osalTimerUpdate( elapsedMSec );
//	}
//	}
//}



// 记录上一次的 SysTick 值（用于计算时间差）
//static uint32 previousSysTick = 0;

// 记录上一次的余数
//static uint16 remTicks = 0;

// 自定义时间更新函数 要求这个函数的更新周期不要大于1s 的滴答计数周期
//void osalTimeUpdate(void)
//{
//    // 保存中断状态
//    halIntState_t intState;
//    // 临时变量
//    uint32 tmp;
//    // 存储转换后的毫秒数
//    uint16 elapsedMSec = 0;
//	
//    // 进入临界区（禁用中断）
//    HAL_ENTER_CRITICAL_SECTION(intState);

//    // 获取当前 SysTick 值（倒计时，从 LOAD 递减到 0）
//    uint32 currentSysTick = SysTick->VAL;
//    // 获取 SysTick 重装载值（1 秒对应的 ticks）
//    uint32 loadValue = iot_systick_load; // 即 SystemCoreClock / 8（6 MHz 时为 6,000,000 - 1）

//	// 退出临界区（恢复中断）
//	HAL_EXIT_CRITICAL_SECTION(intState);
//	
//	// 检查计数是否变化
//	if ( currentSysTick != previousSysTick )
//	{
//	
//		// 计算自上次更新以来的 ticks 变化（处理倒计时和回绕）
//		if (currentSysTick <= previousSysTick)
//		{
//			// 正常情况：SysTick 未回绕
//			tmp = previousSysTick - currentSysTick;
//		}
//		else
//		{
//			// 回绕情况：SysTick 从 0 重新加载 LOAD
//			tmp = (loadValue - currentSysTick) + previousSysTick;
//		}
//		
//		// 更新 previousSysTick 为当前值（供下次计算）
//		previousSysTick = currentSysTick;

//		// 将 ticks 转换为毫秒（1 秒 = loadValue ticks，因此 1 ms = system_time_update）
//		tmp += remUsTicks;
//		
//		elapsedMSec = (uint16)(tmp / iot_systick_load_1ms);
//		
//		remUsTicks = (uint16)(tmp % iot_systick_load_1ms);

//		// 如果有毫秒时间变化，更新 OSAL 时钟和定时器
//		if (elapsedMSec)
//		{
//			osalClockUpdate(elapsedMSec);
//			osalTimerUpdate(elapsedMSec);
//		}
//	}
//}

// 自定义时间更新函数
void osalTimeUpdate(void)
{
    // 保存中断状态
    halIntState_t intState;
    
    // 存储转换后的毫秒数
    uint16 elapsedMSec = 0;

    uint32 tick;
    uint32 currentSysTick;
    uint32 seconds_backup = iot_system_seconds;
    uint32 milliseconds_backup = iot_system_milliseconds;
	
    // 进入临界区（禁用中断）
    HAL_ENTER_CRITICAL_SECTION(intState);

    // 获取当前 SysTick 值（倒计时，从 LOAD 递减到 0）
    currentSysTick = SysTick->VAL;
    tick = iot_systick;                         //-- 读取中断计数（每次中断 +1）
    iot_systick = 0;                            //-- 清零中断计数
    
    // 退出临界区（恢复中断）
    HAL_EXIT_CRITICAL_SECTION(intState);

    iot_systick_count += tick;                  //-- 总秒数增加
    iot_system_seconds = iot_systick_count;     //-- 更新秒
    iot_system_milliseconds = (iot_systick_load - currentSysTick) / iot_systick_load_1ms;   //-- 计算毫秒

    elapsedMSec = (iot_system_seconds - seconds_backup) * 1000 + iot_system_milliseconds - milliseconds_backup;

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
	
	
	// 如果有毫秒时间变化，更新 OSAL 时钟和定时器
	if (elapsedMSec)
	{
		osalClockUpdate(elapsedMSec);
		osalTimerUpdate(elapsedMSec);
	}
}



/*********************************************************************
 * @fn      osalClockUpdate
 *
 * @brief   用经过的毫秒数更新OSAL时钟
 *
 * @param   elapsedMSec - 经过的毫秒数
 *
 * @return  无
 */
static void osalClockUpdate( uint16 elapsedMSec )
{
  // 将毫秒数加到毫秒计数器
  timeMSec += elapsedMSec;

  // 如果满1秒，更新秒级时钟并保留剩余毫秒
  if ( timeMSec >= 1000 )
  {
    OSAL_timeSeconds += timeMSec / 1000;  // 增加整秒数
    timeMSec = timeMSec % 1000;          // 保留剩余毫秒
  }
}

/*********************************************************************
 * @fn      osal_setClock
 *
 * @brief   设置新的时间（仅设置秒部分，不影响毫秒计数器）
 *
 * @param   newTime - 自2000年1月1日0时0分0秒以来的秒数
 *
 * @return  无
 */
void osal_setClock( UTCTime newTime )
{
  OSAL_timeSeconds = newTime;
}

/*********************************************************************
 * @fn      osal_getClock
 *
 * @brief   获取当前时间（仅返回秒部分）
 *
 * @param   无
 *
 * @return  自2000年1月1日0时0分0秒以来的秒数
 */
UTCTime osal_getClock( void )
{
  return ( OSAL_timeSeconds );
}

/*********************************************************************
 * @fn      osal_ConvertUTCTime
 *
 * @brief   将UTCTime转换为UTCTimeStruct（分解为年月日时分秒）
 *
 * @param   tm - 指向时间结构体的指针
 * @param   secTime - 自2000年1月1日0时0分0秒以来的秒数
 *
 * @return  无
 */
void osal_ConvertUTCTime( UTCTimeStruct *tm, UTCTime secTime )
{
  // 计算当天的时间（时、分、秒）
  {
    // 计算当天已过去的秒数
    uint32 day = secTime % DAY;
    tm->seconds = day % 60UL;        // 秒
    tm->minutes = (day % 3600UL) / 60UL; // 分
    tm->hour = day / 3600UL;         // 时
  }

  // 计算日期（年、月、日）
  {
    // 计算总天数（去掉当天）
    uint16 numDays = secTime / DAY;
    // 初始化年份为起始年
    tm->year = BEGYEAR;
    // 减去每年的天数，直到找到当前年份
    while ( numDays >= YearLength( tm->year ) )
    {
      numDays -= YearLength( tm->year );
      tm->year++;
    }

    // 初始化月份为0（1月）
    tm->month = 0;
    // 减去每月的天数，直到找到当前月份
    while ( numDays >= monthLength( IsLeapYear( tm->year ), tm->month ) )
    {
      numDays -= monthLength( IsLeapYear( tm->year ), tm->month );
      tm->month++;
    }

    // 剩余的天数就是当月日期
    tm->day = numDays;
  }
}

/*********************************************************************
 * @fn      monthLength
 *
 * @brief   计算指定年份和月份的天数
 *
 * @param   lpyr - 1表示闰年，0表示非闰年
 * @param   mon - 月份（0-11表示1-12月）
 *
 * @return  该月的天数
 */
static uint8 monthLength( uint8 lpyr, uint8 mon )
{
  // 默认设为31天
  uint8 days = 31;

  // 如果是2月（mon==1）
  if ( mon == 1 ) // feb
  {
    // 闰年2月有29天，否则28天
    days = ( 28 + lpyr );
  }
  else
  {
    // 7月及以后月份需要调整（原数组从0开始）
    if ( mon > 6 ) // aug-dec
    {
      mon--;
    }

    // 单数月（除8月外）有30天
    if ( mon & 1 )
    {
      days = 30;
    }
  }

  return ( days );
}

/*********************************************************************
 * @fn      osal_ConvertUTCSecs
 *
 * @brief   将UTCTimeStruct转换为UTCTime（自2000年以来的秒数）
 *
 * @param   tm - 指向时间结构体的指针
 *
 * @return  自2000年1月1日0时0分0秒以来的秒数
 */
UTCTime osal_ConvertUTCSecs( UTCTimeStruct *tm )
{
  uint32 seconds;

  /* 计算当天已过去的秒数 */
  seconds = (((tm->hour * 60UL) + tm->minutes) * 60UL) + tm->seconds;

  /* 计算完整天数对应的秒数 */
  {
    /* 从当前月的天数开始 */
    uint16 days = tm->day;

    /* 加上之前各月的天数 */
    {
      int8 month = tm->month;
      while ( --month >= 0 )
      {
        days += monthLength( IsLeapYear( tm->year ), month );
      }
    }

    /* 加上之前各年的天数 */
    {
      uint16 year = tm->year;
      while ( --year >= BEGYEAR )
      {
        days += YearLength( year );
      }
    }

    /* 将总天数转换为秒并加到之前计算的秒数上 */
    seconds += (days * DAY);
  }

  return ( seconds );
}
