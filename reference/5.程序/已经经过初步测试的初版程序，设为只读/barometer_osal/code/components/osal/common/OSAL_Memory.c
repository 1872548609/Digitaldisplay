/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */

// 基础定义头文件（包含数据类型、常量等）
#include "comdef.h"
// 操作系统抽象层（任务调度、事件管理等）
#include "OSAL.h"
// OSAL内存管理模块
#include "OSAL_Memory.h"
// 硬件抽象层（板级支持包）
#include "OnBoard.h"
// 硬件断言（用于调试）
#include "hal_assert.h"

/* ------------------------------------------------------------------------------------------------
 *                                           Constants
 * ------------------------------------------------------------------------------------------------
 */

// 内存块"使用中"标志位（最高位）
#define OSALMEM_IN_USE             0x8000
// 检查MAXMEMHEAP是否超出管理范围（避免与标志位冲突）
#if (MAXMEMHEAP & OSALMEM_IN_USE)
#error MAXMEMHEAP is too big to manage!
#endif

// 内存块头大小（单位：字节）
#define OSALMEM_HDRSZ              sizeof(osalMemHdr_t)

// 内存对齐宏：将值X向上取整到OSALMEM_HDRSZ的整数倍
#define OSALMEM_ROUND(X)       ((((X) + OSALMEM_HDRSZ - 1) / OSALMEM_HDRSZ) * OSALMEM_HDRSZ)

/* 最小可接受的内部分裂块大小（避免频繁分裂导致碎片）
 * 需确保是OSALMEM_HDRSZ的整数倍
 */
#if !defined OSALMEM_MIN_BLKSZ
#define OSALMEM_MIN_BLKSZ         (OSALMEM_ROUND((OSALMEM_HDRSZ * 2)))
#endif

/* 长生命周期内存块大小（用于系统关键对象）
 * 根据不同配置（安全模式、密钥建立等）动态调整
 */
#if !defined OSALMEM_LL_BLKSZ
#if defined NONWK  // 非网络设备简化配置
#define OSALMEM_LL_BLKSZ          (OSALMEM_ROUND(6) + (1 * OSALMEM_HDRSZ))
#else  // 标准网络设备配置
/*
 * 默认配置下长期存活对象分析：
 * 包括ZCL密钥建立、链接密钥加入等场景的最大内存需求
 * 调整此值以适应应用特定的长生命周期对象
 */
#if defined ZCL_KEY_ESTABLISH     // 安全应用极端情况
#define OSALMEM_LL_BLKSZ          (OSALMEM_ROUND(526) + (32 * OSALMEM_HDRSZ))
#elif defined TC_LINKKEY_JOIN    // 信任中心链接密钥加入
#define OSALMEM_LL_BLKSZ          (OSALMEM_ROUND(454) + (21 * OSALMEM_HDRSZ))
#elif ((defined SECURE) && (SECURE != 0))  // 普通安全模式
#define OSALMEM_LL_BLKSZ          (OSALMEM_ROUND(418) + (19 * OSALMEM_HDRSZ))
#else  // 默认配置
#define OSALMEM_LL_BLKSZ          (OSALMEM_ROUND(417) + (19 * OSALMEM_HDRSZ))
#endif
#endif
#endif

/* 小内存块配置（用于高频分配/释放场景）
 * 默认16字节（可容纳osalTimerRec_t等高频对象）
 * 需是OSALMEM_MIN_BLKSZ的整数倍
 */
#if !defined OSALMEM_SMALL_BLKSZ
#define OSALMEM_SMALL_BLKSZ       (OSALMEM_ROUND(16))
#endif
// 小内存块池数量
#if !defined OSALMEM_SMALL_BLKCNT
#define OSALMEM_SMALL_BLKCNT       8
#endif

/* 小内存块池配置（位于堆起始区域）
 * 计算公式：(小内存块大小×数量) + 长生命周期块大小
 */
#define OSALMEM_SMALLBLK_BUCKET  ((OSALMEM_SMALL_BLKSZ * OSALMEM_SMALL_BLKCNT) + OSALMEM_LL_BLKSZ)
// 小内存块池结束后的第一个可用块索引，小池尾部用于和大池划分
#define OSALMEM_SMALLBLK_HDRCNT   (OSALMEM_SMALLBLK_BUCKET / OSALMEM_HDRSZ)
// 大内存池起始索引（小内存池之后）
#define OSALMEM_BIGBLK_IDX        (OSALMEM_SMALLBLK_HDRCNT + 1)
// 大内存池大小（总堆大小减去小内存池和两个保留头）
#define OSALMEM_BIGBLK_SZ         (MAXMEMHEAP - OSALMEM_SMALLBLK_BUCKET - OSALMEM_HDRSZ*2)
// 堆结束标记块索引（最后一个块置零）
#define OSALMEM_LASTBLK_IDX      ((MAXMEMHEAP / OSALMEM_HDRSZ) - 1)

// 内存分析工具开关（参考SWRA204文档1.5节）
#if !defined OSALMEM_PROFILER
#define OSALMEM_PROFILER           FALSE  // 是否启用内存使用分析
#endif
#if !defined OSALMEM_PROFILER_LL
#define OSALMEM_PROFILER_LL        FALSE  // 是否单独分析长生命周期块
#endif

#if OSALMEM_PROFILER
// 分析标记字符
#define OSALMEM_INIT              'X'    // 初始化标记
#define OSALMEM_ALOC              'A'    // 分配标记
#define OSALMEM_REIN              'F'    // 释放标记
#endif

/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */

// 内存块头结构（位域实现）
typedef struct {
  // 低15位：总大小（含头部），单位字节
  unsigned len : 15;
  // 最高位：使用标志（1=使用中，0=空闲）
  unsigned inUse : 1;
} osalMemHdrHdr_t;

// 内存块头联合体（确保对齐）
typedef union {
  /* 对齐填充变量
   * 强制结构体按最大元素对齐，避免在目标平台浪费空间
   * （当halDataAlign_t小于uint16时）  halDataAlign_t 通常是目标平台的基本对齐类型（如 uint32_t 或 uint64_t）
   * 如果 halDataAlign_t 比 uint16 更大（例如在32位系统中为4字节），则联合体会按4字节对齐，避免未对齐访问。   
   * 若 halDataAlign_t 是 uint32_t，则 osalMemHdr_t 的起始地址一定是4的倍数。
   */
  halDataAlign_t alignDummy;
  // 16位无符号整数值（可直接操作）可能用于快速存储/读取内存块的大小或状态（如空闲/已分配标志）。
  uint16 val;
  // 位域视图
  osalMemHdrHdr_t hdr;
} osalMemHdr_t;

/* ------------------------------------------------------------------------------------------------
 *                                           Local Variables
 * ------------------------------------------------------------------------------------------------
 */

// 堆内存数组（__no_init表示不初始化以节省启动时间）
static __no_init osalMemHdr_t theHeap[MAXMEMHEAP / OSALMEM_HDRSZ];
// 小内存池中的第一个空闲块指针
static __no_init osalMemHdr_t *ff1;  

// 内存管理状态标志（0x01表示已初始化）
static uint8 osalMemStat;            

#if OSALMEM_METRICS
// 内存分析统计变量
static uint16 blkMax;  // 历史最大块数
static uint16 blkCnt;  // 当前块总数
static uint16 blkFree; // 当前空闲块数
static uint16 memAlo;  // 当前已分配内存
static uint16 memMax;  // 历史最大分配量
#endif

#if OSALMEM_PROFILER
#define OSALMEM_PROMAX  8
/* 分析桶配置（需满足最小差异OSALMEM_MIN_BLKSZ）
 * 最后一个桶必须等于最大分配尺寸
 */
static uint16 proCnt[OSALMEM_PROMAX] = {
OSALMEM_SMALL_BLKSZ, 48, 112, 176, 192, 224, 256, 65535 };
static uint16 proCur[OSALMEM_PROMAX] = { 0 };  // 当前计数
static uint16 proMax[OSALMEM_PROMAX] = { 0 };  // 历史最大值
static uint16 proTot[OSALMEM_PROMAX] = { 0 };  // 总分配次数
static uint16 proSmallBlkMiss;  // 小内存池未命中计数
#endif

/* ------------------------------------------------------------------------------------------------
 *                                           Global Variables
 * ------------------------------------------------------------------------------------------------
 */

// 堆内存跟踪调试接口（可选）
#ifdef DPRINTF_HEAPTRACE
extern int dprintf(const char *fmt, ...);
#endif /* DPRINTF_HEAPTRACE */

/**************************************************************************************************
 * @fn          osal_mem_init
 *
 * @brief       OSAL堆内存管理初始化
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 */
void osal_mem_init(void)//*
{
  // 验证关键配置参数的对齐要求,即它们的值必须是内存块头大小（OSALMEM_HDRSZ）的整数倍。
  HAL_ASSERT(((OSALMEM_MIN_BLKSZ % OSALMEM_HDRSZ) == 0));
  HAL_ASSERT(((OSALMEM_LL_BLKSZ % OSALMEM_HDRSZ) == 0));
  HAL_ASSERT(((OSALMEM_SMALL_BLKSZ % OSALMEM_HDRSZ) == 0));

#if OSALMEM_PROFILER
  // 如果启用了内存分析（OSALMEM_PROFILER），将堆内存（theHeap）初始化为标记值（OSALMEM_INIT）
  //用于调试时检测内存越界或未初始化访问（例如通过比较内存内容是否被意外修改）。
  (void)osal_memset(theHeap, OSALMEM_INIT, MAXMEMHEAP);
#endif

  // 设置堆结束标记块（val=0便于快速判空）,在堆的末尾设置一个值为 0 的标记块，表示内存结束。val=0 可以快速判断是否到达堆尾，避免越界。
  theHeap[OSALMEM_LASTBLK_IDX].val = 0;

  // 初始化小内存池,将全局指针 ff1 指向堆起始地址，表示小内存池的入口。
  ff1 = theHeap;
  // 设置小内存池总大小（清空使用标志）
  //设置小内存池的总大小（SMALLBLK_BUCKET）
  ff1->val = OSALMEM_SMALLBLK_BUCKET;
  // 设置保护块（防止合并到野区域）在小内存池末尾设置一个特殊块（HDRSZ | IN_USE），防止内存合并时越界到非法区域。
  theHeap[OSALMEM_SMALLBLK_HDRCNT].val = (OSALMEM_HDRSZ | OSALMEM_IN_USE);

  // 初始化大内存池（野区域）将堆的某个区域（BIGBLK_IDX）初始化为大内存池，大小为 OSALMEM_BIGBLK_SZ。
  theHeap[OSALMEM_BIGBLK_IDX].val = OSALMEM_BIGBLK_SZ;

#if ( OSALMEM_METRICS )
  /* 初始化统计计数
   * 包括：小内存池、大内存池
   * 不包括：结束标记块和保护块
   */
  blkCnt = blkFree = 2;// 小内存池和大内存池各算1块，共2块
#endif
}

/**************************************************************************************************
 * @fn          osal_mem_kick
 *
 * @brief       初始化后调整内存池指针
 *              在所有长生命周期块分配后调用（通常在osal_init_system末尾）
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 */
void osal_mem_kick(void)
{
  halIntState_t intState;
  // 临时分配1字节以定位第一个可用内存目的是让内存管理器返回当前第一个可用块的地址（tmp）。
  osalMemHdr_t *tmp = osal_mem_alloc(1);
  //若分配失败（tmp ==），触发断言（通常在调试阶段暂停系统）。
  HAL_ASSERT((tmp !=NULL));
  HAL_ENTER_CRITICAL_SECTION(intState);  // 关闭中断内存池操作需原子性，避免中断或其他任务干扰。

  /* 长生命周期块已填满LL预留区域
   * 设置osalMemStat使后续搜索仅针对符合OSALMEM_SMALL_BLKSZ的块
   */
  ff1 = tmp - 1;       // 调整ff1指向LL区域后的第一个可用块
  osal_mem_free(tmp);  // 释放临时块
  osalMemStat = 0x01;  // 启用内存分析（需在释放后设置）

  HAL_EXIT_CRITICAL_SECTION(intState);  // 恢复中断
}

/**************************************************************************************************
 * @fn          osal_mem_alloc
 *
 * @brief       动态内存分配实现
 *
 * input parameters
 *
 * @param size - 需要分配的字节数
 *
 * output parameters
 *
 * None.
 *
 * @return      分配的内存指针（失败返回）
 */
#ifdef DPRINTF_OSALHEAPTRACE    //提供了调试版本和普通版本，调试版本会记录调用位置信息
void *osal_mem_alloc_dbg( uint16 size, const char *fname, unsigned lnum )
#else
void *osal_mem_alloc( uint16 size )
#endif
{
  osalMemHdr_t *prev =NULL;  // 前驱块指针，用于记录前一个内存块，以便合并空闲块
  osalMemHdr_t *hdr;         // 当前块指针，当前处理的内存块头部指针
  halIntState_t intState;    // 中断状态保存，保存中断状态，用于临界区保护
  uint8 coal = 0;            // 合并标志，标记是否需要与前一个块合并

  // 增加头部空间需求OSALMEM_HDRSZ 是内存块头部的元数据大小（如块长度、使用标志）。
  size += OSALMEM_HDRSZ;

  // 对齐处理（按halDataAlign_t对齐）
  //若对齐单位是 2 字节（uint16），补齐到偶数地址。若对齐单位更大（如 4 字节），补齐到对齐边界。
  if ( sizeof( halDataAlign_t ) == 2 )
  {
    size += (size & 0x01);  // 2字节对齐补齐
  }
  else if ( sizeof( halDataAlign_t ) != 1 )
  {
    const uint8 mod = size % sizeof( halDataAlign_t );
    if ( mod != 0 )
    {
      size += (sizeof( halDataAlign_t ) - mod);  // 通用对齐补齐
    }
  }

  HAL_ENTER_CRITICAL_SECTION( intState );  // 关闭中断

  /* 分配策略：
   * - 初始化阶段或范围在小内存：搜索小内存池
   * - 其他情况：直接搜索大内存池
   */
  if ((osalMemStat == 0) || (size <= OSALMEM_SMALL_BLKSZ))
  {
    hdr = ff1;  // 从小内存池开始
  }
  else
  {
    hdr = (theHeap + OSALMEM_BIGBLK_IDX);  // 从大内存池开始
  }

  // 首次适应搜索算法
  do
  {
    if ( hdr->hdr.inUse )  // 已使用块跳过，对于已使用的块，跳过并重置合并标志
    {
      coal = 0;
    }
    else  // 空闲块处理
    {
      if ( coal != 0 )  // 尝试与前驱块合并，如果前一个块也是空闲的(coal != 0)，尝试合并
      {
#if ( OSALMEM_METRICS )
        blkCnt--;       // 块总数减少
        blkFree--;      // 空闲块减少
#endif
        // 合并到前驱块
        prev->hdr.len += hdr->hdr.len;

        // 检查合并后是否满足需求
        if ( prev->hdr.len >= size )
        {
          hdr = prev;   // 使用合并后的块
          break;
        }
      }
      else  // 首次遇到空闲块
      {
        if ( hdr->hdr.len >= size )  // 足够大
        {
          break;
        }
        // 记录前驱块以便后续合并
        coal = 1;
        prev = hdr;
      }
    }

    // 移动到下一个块
    hdr = (osalMemHdr_t *)((uint8 *)hdr + hdr->hdr.len);

    // 到达堆尾（val=0标记）
    if ( hdr->val == 0 )
    {
      hdr =NULL;
      break;
    }
  } while (1);

  // 找到合适块后的处理
  if ( hdr != NULL)
  {
    uint16 tmp = hdr->hdr.len - size;  // 计算剩余空间

    // 检查是否值得分裂
    if ( tmp >= OSALMEM_MIN_BLKSZ )//如果剩余空间足够大(>= OSALMEM_MIN_BLKSZ)，分裂成两个块
    {
      // 分裂块
      osalMemHdr_t *next = (osalMemHdr_t *)((uint8 *)hdr + size);//计算新块的起始地址
      next->val = tmp;                     // 新块大小
      hdr->val = (size | OSALMEM_IN_USE);  // 当前块标记为使用中

#if ( OSALMEM_METRICS )
      blkCnt++;        // 块总数增加
      if ( blkMax < blkCnt )
      {
        blkMax = blkCnt; // 更新历史最大值
      }
      memAlo += size;   // 已分配内存增加
#endif
    }
    else  // 不分裂
    {
#if ( OSALMEM_METRICS )
      memAlo += hdr->hdr.len;  // 分配整个块
      blkFree--;              // 空闲块减少
#endif
      hdr->hdr.inUse = TRUE;   // 标记为使用中
    }

#if ( OSALMEM_METRICS )
    // 更新峰值内存使用
    if ( memMax < memAlo )
    {
      memMax = memAlo;
    }
#endif

#if ( OSALMEM_PROFILER )//用特定模式填充分配的内存，帮助检测内存越界
#if !OSALMEM_PROFILER_LL
    if (osalMemStat != 0)  // 跳过初始化阶段分析
#endif
    {
      uint8 idx;
      // 确定所属分析桶
      for ( idx = 0; idx < OSALMEM_PROMAX; idx++ )
      {
        if ( hdr->hdr.len <= proCnt[idx] )
        {
          break;
        }
      }
      proCur[idx]++;  // 当前计数增加
      if ( proMax[idx] < proCur[idx] )
      {
        proMax[idx] = proCur[idx]; // 更新峰值
      }
      proTot[idx]++;  // 总分配次数增加

      /* 小内存池未命中统计
       * 当小内存块无法在小内存池分配时计数
       * 可通过proSmallBlkMiss/proTot[0]计算未命中率
       */
      if ((hdr->hdr.len <= OSALMEM_SMALL_BLKSZ) && (hdr >= (theHeap + OSALMEM_BIGBLK_IDX)))
      {
        proSmallBlkMiss++;
      }
    }

    // 标记分配区域（调试用）
    (void)osal_memset((uint8 *)(hdr+1), OSALMEM_ALOC, (hdr->hdr.len - OSALMEM_HDRSZ));
#endif

    // 调整首次适应指针（如果分配的是第一个块）更新ff1指针指向下一个块
    if ((osalMemStat != 0) && (ff1 == hdr))
    {
      ff1 = (osalMemHdr_t *)((uint8 *)hdr + hdr->hdr.len);
    }

    hdr++;  // 移动指针越过头部，返回用户可用的内存地址
  }

  HAL_EXIT_CRITICAL_SECTION( intState );  // 恢复中断
//#pragma diag_suppress=Pe767
//  // 对齐检查断言
//  HAL_ASSERT(((halDataAlign_t)hdr % sizeof(halDataAlign_t)) == 0);
//#pragma diag_default=Pe767

#ifdef DPRINTF_OSALHEAPTRACE
  // 调试信息输出
  dprintf("osal_mem_alloc(%u)->%lx:%s:%u\n", size, (unsigned) hdr, fname, lnum);
#endif
  return (void *)hdr;
}

/**************************************************************************************************
 * @fn          osal_mem_free
 *
 * @brief       动态内存释放实现
 *
 * input parameters
 *
 * @param ptr - 需要释放的内存指针（必须由osal_mem_alloc返回）
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 */
#ifdef DPRINTF_OSALHEAPTRACE//根据是否定义 DPRINTF_OSALHEAPTRACE，选择调试版本（带文件名和行号）或普通版本。
void osal_mem_free_dbg(void *ptr, const char *fname, unsigned lnum)
#else
void osal_mem_free(void *ptr)
#endif
{
  osalMemHdr_t *hdr = (osalMemHdr_t *)ptr - 1;  // 获取块头部
  halIntState_t intState;                      // 中断状态保存

#ifdef DPRINTF_OSALHEAPTRACE
  // 调试信息输出
  dprintf("osal_mem_free(%lx):%s:%u\n", (unsigned) ptr, fname, lnum);
#endif

  // 参数有效性检查指针 ptr 必须在堆的合法范围内（theHeap 到 theHeap+MAXMEMHEAP）。
  HAL_ASSERT(((uint8 *)ptr >= (uint8 *)theHeap) && ((uint8 *)ptr < (uint8 *)theHeap+MAXMEMHEAP));
  HAL_ASSERT(hdr->hdr.inUse);  // 必须是在用块

  HAL_ENTER_CRITICAL_SECTION( intState );  // 关闭中断进入临界区，禁用中断以防止多线程/中断竞争。
  hdr->hdr.inUse = FALSE;      // 标记为空闲

  // 调整首次适应指针（如果释放的是第一个块）
  if (ff1 > hdr)
  {
    ff1 = hdr;
  }

#if OSALMEM_PROFILER//内存分析统计不同大小内存块的释放情况。
#if !OSALMEM_PROFILER_LL
  if (osalMemStat != 0)  // 跳过初始化阶段分析
#endif
  {
    uint8 idx;
    // 确定所属分析桶
    for (idx = 0; idx < OSALMEM_PROMAX; idx++)
    {
      if (hdr->hdr.len <= proCnt[idx])
      {
        break;
      }
    }
    proCur[idx]--;  // 当前计数减少
  }

  // 标记释放区域（调试用）
  (void)osal_memset((uint8 *)(hdr+1), OSALMEM_REIN, (hdr->hdr.len - OSALMEM_HDRSZ) );
#endif

#if OSALMEM_METRICS
  memAlo -= hdr->hdr.len;  // 已分配内存减少
  blkFree++;              // 空闲块增加
#endif

  HAL_EXIT_CRITICAL_SECTION( intState );  // 恢复中断
}

#if OSALMEM_METRICS
/*********************************************************************
 * @fn      osal_heap_block_max
 *
 * @brief   获取历史最大块数
 *
 * @param   none
 *
 * @return  历史最大块数
 */
uint16 osal_heap_block_max( void )
{
  return blkMax;
}

/*********************************************************************
 * @fn      osal_heap_block_cnt
 *
 * @brief   获取当前块总数
 *
 * @param   none
 *
 * @return  当前块总数
 */
uint16 osal_heap_block_cnt( void )
{
  return blkCnt;
}

/*********************************************************************
 * @fn      osal_heap_block_free
 *
 * @brief   获取当前空闲块数
 *
 * @param   none
 *
 * @return  当前空闲块数
 */
uint16 osal_heap_block_free( void )
{
  return blkFree;
}

/*********************************************************************
 * @fn      osal_heap_mem_used
 *
 * @brief   获取当前已分配内存
 *
 * @param   none
 *
 * @return  当前已分配字节数
 */
uint16 osal_heap_mem_used( void )
{
  return memAlo;
}
#endif

#if defined (ZTOOL_P1) || defined (ZTOOL_P2)
/*********************************************************************
 * @fn      osal_heap_high_water
 *
 * @brief   获取内存使用峰值
 *
 * @param   none
 *
 * @return  历史最大内存使用量
 */
uint16 osal_heap_high_water( void )
{
#if ( OSALMEM_METRICS )
  return memMax;
#else
  return MAXMEMHEAP;  // 未启用分析时返回总大小
#endif
}
#endif

/**************************************************************************************************
*/
