/*********************************************************************
 * INCLUDES
 */

#include <string.h>

#include "comdef.h"
#include "OSAL.h"
#include "OSAL_Tasks.h"
#include "OSAL_Memory.h"
#include "OSAL_PwrMgr.h"
#include "OSAL_Clock.h"

#include "OnBoard.h"

/* HAL */
#include "hal_drivers.h"

#ifdef IAR_ARMCM3_LM
  #include "FreeRTOSConfig.h"
  #include "osal_task.h"
#endif

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// Message Pool Definitions
osal_msg_q_t osal_qHead;// OSAL消息队列头指针

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// Index of active task
static uint8 activeTaskID = TASK_NO_TASK;// 当前活跃任务ID（默认无任务）

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

/*********************************************************************
 * HELPER FUNCTIONS - 辅助函数
 */
/* Keil编译器兼容性处理 */
#ifdef __KEIL__
// 提供itoa的空实现（Keil缺少此函数）
char *  itoa ( int value, char * buffer, int radix )
{
  return(buffer);
}
#endif

/*********************************************************************
 * @fn      osal_strlen
 *
 * @brief
 *
 *   Calculates the length of a string.  The string must be null
 *   terminated.
 *
 * @param   char *pString - pointer to text string
 *
 * @return  int - number of characters
 */
/* 字符串长度计算 */
int osal_strlen( char *pString )
{
  return (int)( strlen( pString ) );
}

/*********************************************************************
 * @fn      osal_memcpy
 *
 * @brief
 *
 *   Generic memory copy.
 *
 *   Note: This function differs from the standard memcpy(), since
 *         it returns the pointer to the next destination uint8. The
 *         standard memcpy() returns the original destination address.
 *
 * @param   dst - destination address
 * @param   src - source address
 * @param   len - number of bytes to copy
 *
 * @return  pointer to end of destination buffer
 */
/* 内存拷贝（返回目标缓冲区末尾指针） */
void *osal_memcpy( void *dst, const void GENERIC *src, unsigned int len )
{
  uint8 *pDst;
  const uint8 GENERIC *pSrc;

  pSrc = src;
  pDst = dst;

  while ( len-- )
    *pDst++ = *pSrc++;

  return ( pDst );
}

/*********************************************************************
 * @fn      osal_revmemcpy
 *
 * @brief   Generic reverse memory copy.  Starts at the end of the
 *   source buffer, by taking the source address pointer and moving
 *   pointer ahead "len" bytes, then decrementing the pointer.
 *
 *   Note: This function differs from the standard memcpy(), since
 *         it returns the pointer to the next destination uint8. The
 *         standard memcpy() returns the original destination address.
 *
 * @param   dst - destination address
 * @param   src - source address
 * @param   len - number of bytes to copy
 *
 * @return  pointer to end of destination buffer
 */
/* 反向内存拷贝（从源缓冲区末尾开始拷贝） */
void *osal_revmemcpy( void *dst, const void GENERIC *src, unsigned int len )
{
  uint8 *pDst;
  const uint8 GENERIC *pSrc;

  pSrc = src;
  pSrc += (len-1);
  pDst = dst;

  while ( len-- )
    *pDst++ = *pSrc--;

  return ( pDst );
}

/*********************************************************************
 * @fn      osal_memdup
 *
 * @brief   Allocates a buffer [with osal_mem_alloc()] and copies
 *          the src buffer into the newly allocated space.
 *
 * @param   src - source address
 * @param   len - number of bytes to copy
 *
 * @return  pointer to the new allocated buffer, or NULL if
 *          allocation problem.
 */
/* 内存分配并拷贝 */
void *osal_memdup( const void GENERIC *src, unsigned int len )
{
  // 定义目标缓冲区指针
  uint8 *pDst;

  // 1. 调用内存分配函数，申请 len 字节的空间
  pDst = osal_mem_alloc( len );
  // 2. 检查内存是否分配成功
  if ( pDst )
  {
     // 3. 如果成功，将 src 指向的数据拷贝到新分配的缓冲区
    VOID osal_memcpy( pDst, src, len );
  }
  // 4. 返回新缓冲区的指针（如果分配失败，返回）
  return ( (void *)pDst );
}

/*********************************************************************
 * @fn      osal_memcmp
 *
 * @brief
 *
 *   Generic memory compare.
 *
 * @param   src1 - source 1 addrexx
 * @param   src2 - source 2 address
 * @param   len - number of bytes to compare
 *
 * @return  TRUE - same, FALSE - different
 */
/* 内存比较 */
uint8 osal_memcmp( const void GENERIC *src1, const void GENERIC *src2, unsigned int len )
{
  // 将泛型指针转换为 const uint8* 类型，便于逐字节比较
  const uint8 GENERIC *pSrc1;
  const uint8 GENERIC *pSrc2;

  // 初始化指针，指向源数据的起始地址
  pSrc1 = src1;
  pSrc2 = src2;

  // 循环比较每个字节
  while ( len-- )
  {
    if( *pSrc1++ != *pSrc2++ )
      return FALSE;
  }
  return TRUE;
}


/*********************************************************************
 * @fn      osal_memset
 *
 * @brief
 *
 *   Set memory buffer to value.
 *
 * @param   dest - pointer to buffer
 * @param   value - what to set each uint8 of the message
 * @param   size - how big
 *
 * @return  pointer to destination buffer
 */
/* 内存填充 */
void *osal_memset( void *dest, uint8 value, int len )
{
  //标准库自带填充函数
  return memset( dest, value, len );
}

/*********************************************************************
 * @fn      osal_build_uint16
 *
 * @brief
 *
 *   Build a uint16 out of 2 bytes (0 then 1).
 *
 * @param   swapped - 0 then 1
 *
 * @return  uint16
 */
/* 构建16位整数（小端模式） */
uint16 osal_build_uint16( uint8 *swapped )
{
  //返回 uint16 类型的值，由 swapped[0] 和 swapped[1] 组合而成。
  //输入 swapped = {0x12, 0x34} → 返回 0x3412（小端模式）
  return ( BUILD_UINT16( swapped[0], swapped[1] ) );
}

/*********************************************************************
 * @fn      osal_build_uint32
 *
 * @brief
 *
 *   Build a uint32 out of sequential bytes.
 *
 * @param   swapped - sequential bytes
 * @param   len - number of bytes in the uint8 array
 *
 * @return  uint32
 */
/* 构建32位整数（支持2/3/4字节输入） */
uint32 osal_build_uint32(uint8 *swapped, uint8 len)
{
  // 处理2字节输入：将 swapped[0] 和 swapped[1] 组合，高16位补0
  if (len == 2)
    return (BUILD_UINT32(swapped[0], swapped[1], 0L, 0L));
  // 处理3字节输入：将 swapped[0]、swapped[1]、swapped[2] 组合，最高8位补0
  else if (len == 3)
    return (BUILD_UINT32(swapped[0], swapped[1], swapped[2], 0L));
  // 处理4字节输入：将所有4个字节完整组合
  else if (len == 4)
    return (BUILD_UINT32(swapped[0], swapped[1], swapped[2], swapped[3]));
  // 默认情况：仅使用 swapped[0]，返回8位值（强制转换为 uint32）
  else
    return ((uint32)swapped[0]);
}

#if !defined ( ZBIT ) && !defined ( ZBIT2 ) && !defined (UBIT)
/*********************************************************************
 * @fn      _ltoa
 *
 * @brief
 *
 *   convert a long unsigned int to a string.
 *
 * @param  l - long to convert
 * @param  buf - buffer to convert to
 * @param  radix - 10 dec, 16 hex
 *
 * @return  pointer to buffer
 */
/* 长整型转字符串（非GNU编译器实现） */
unsigned char * _ltoa(unsigned long l, unsigned char *buf, unsigned char radix)
{
#if defined( __GNUC__ )
  return ( (char*)ltoa( l, buf, radix ) );
#else
  unsigned char tmp1[10] = "", tmp2[10] = "", tmp3[10] = "";
  unsigned short num1, num2, num3;
  unsigned char i;

  buf[0] = '\0';

  if ( radix == 10 )
  {
    num1 = l % 10000;
    num2 = (l / 10000) % 10000;
    num3 = (unsigned short)(l / 100000000);

    if (num3) _itoa(num3, tmp3, 10);
    if (num2) _itoa(num2, tmp2, 10);
    if (num1) _itoa(num1, tmp1, 10);

    if (num3)
    {
      strcpy((char*)buf, (char const*)tmp3);
      for (i = 0; i < 4 - strlen((char const*)tmp2); i++)
        strcat((char*)buf, "0");
    }
    strcat((char*)buf, (char const*)tmp2);
    if (num3 || num2)
    {
      for (i = 0; i < 4 - strlen((char const*)tmp1); i++)
        strcat((char*)buf, "0");
    }
    strcat((char*)buf, (char const*)tmp1);
    if (!num3 && !num2 && !num1)
      strcpy((char*)buf, "0");
  }
  else if ( radix == 16 )
  {
    num1 = l & 0x0000FFFF;
    num2 = l >> 16;

    if (num2) _itoa(num2, tmp2, 16);
    if (num1) _itoa(num1, tmp1, 16);

    if (num2)
    {
      strcpy((char*)buf,(char const*)tmp2);
      for (i = 0; i < 4 - strlen((char const*)tmp1); i++)
        strcat((char*)buf, "0");
    }
    strcat((char*)buf, (char const*)tmp1);
    if (!num2 && !num1)
      strcpy((char*)buf, "0");
  }
  else
    return NULL;

  return buf;
#endif
}
#endif // !defined(ZBIT) && !defined(ZBIT2)

/*********************************************************************
 * @fn        osal_rand
 *
 * @brief    Random number generator
 *
 * @param   none
 *
 * @return  uint16 - new random number
 */
/* 随机数生成 */
uint16 osal_rand( void )
{
  return ( Onboard_rand() );
}

/*********************************************************************
 * API FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osal_msg_allocate
 *
 * @brief
 *
 *    This function is called by a task to allocate a message buffer
 *    into which the task will encode the particular message it wishes
 *    to send.  This common buffer scheme is used to strictly limit the
 *    creation of message buffers within the system due to RAM size
 *    limitations on the microprocessor.   Note that all message buffers
 *    are a fixed size (at least initially).  The parameter len is kept
 *    in case a message pool with varying fixed message sizes is later
 *    created (for example, a pool of message buffers of size LARGE,
 *    MEDIUM and SMALL could be maintained and allocated based on request
 *    from the tasks).
 *
 *
 * @param   uint8 len  - wanted buffer length
 *
 *
 * @return  pointer to allocated buffer or NULL if allocation failed.
 */
/* 消息缓冲区分配 */
uint8 * osal_msg_allocate( uint16 len )
{
  osal_msg_hdr_t *hdr;

  // 检查请求长度是否为0
  if ( len == 0 )
    return ( NULL );

  // 分配内存：消息头（osal_msg_hdr_t） + 用户数据区（len字节）
  hdr = (osal_msg_hdr_t *) osal_mem_alloc( (short)(len + sizeof( osal_msg_hdr_t )) );
  // 如果分配成功
  if ( hdr )
  {
    // 初始化消息头
    hdr->next = NULL;// 下一条消息指针（用于队列）
    hdr->len = len;// 用户数据长度
    hdr->dest_id = TASK_NO_TASK;// 目标任务ID（默认无效值）
    return ( (uint8 *) (hdr + 1) );
  }
  else
    return ( NULL );
}

/*********************************************************************
 * @fn      osal_msg_deallocate
 *
 * @brief
 *
 *    This function is used to deallocate a message buffer. This function
 *    is called by a task (or processing element) after it has finished
 *    processing a received message.
 *
 *
 * @param   uint8 *msg_ptr - pointer to new message buffer
 *
 * @return  SUCCESS, INVALID_MSG_POINTER
 */
/* 消息缓冲区释放 */
uint8 osal_msg_deallocate( uint8 *msg_ptr )
{
  uint8 *x;

  // 检查指针是否为空
  if ( msg_ptr == NULL )
    return ( INVALID_MSG_POINTER );

  // 检查消息是否仍在队列中（通过消息头中的 dest_id 字段）
  if ( OSAL_MSG_ID( msg_ptr ) != TASK_NO_TASK )
    return ( MSG_BUFFER_NOT_AVAIL );

  // 计算消息头起始地址：用户数据区指针 - 消息头大小
  x = (uint8 *)((uint8 *)msg_ptr - sizeof( osal_msg_hdr_t ));

  // 释放内存，这里应该是释放这个消息的内存，由osal自定内存管理释放
  osal_mem_free( (void *)x );

  return ( SUCCESS );
}

/*********************************************************************
 * @fn      osal_msg_send
 *
 * @brief
 *
 *    This function is called by a task to send a command message to
 *    another task or processing element.  The sending_task field must
 *    refer to a valid task, since the task ID will be used
 *    for the response message.  This function will also set a message
 *    ready event in the destination tasks event list.
 *
 *
 * @param   uint8 destination task - Send msg to?  Task ID
 * @param   uint8 *msg_ptr - pointer to new message buffer
 * @param   uint8 len - length of data in message
 *
 * @return  SUCCESS, INVALID_TASK, INVALID_MSG_POINTER
 */
/* 发送消息到指定任务 */
uint8 osal_msg_send( uint8 destination_task, uint8 *msg_ptr )
{
  // 检查空指针
  if ( msg_ptr == NULL )
    return ( INVALID_MSG_POINTER );

  // 检查目标任务ID是否合法
  if ( destination_task >= tasksCnt )
  {
    osal_msg_deallocate( msg_ptr );
    return ( INVALID_TASK );
  }

  // 检查消息头状态（是否已入队或链表指针异常）
  if ( OSAL_MSG_NEXT( msg_ptr ) != NULL ||
       OSAL_MSG_ID( msg_ptr ) != TASK_NO_TASK )
  {
    osal_msg_deallocate( msg_ptr );
    return ( INVALID_MSG_POINTER );
  }

  // 设置目标任务ID（标记消息为“已入队”）
  OSAL_MSG_ID( msg_ptr ) = destination_task;

  // 将消息加入队列（假设 osal_qHead 是全局队列头）
  osal_msg_enqueue( &osal_qHead, msg_ptr );

  // 通知目标任务有新消息（通过事件标志）
  osal_set_event( destination_task, SYS_EVENT_MSG );

  return ( SUCCESS );
}

/*********************************************************************
 * @fn      osal_msg_receive
 *
 * @brief
 *
 *    This function is called by a task to retrieve a received command
 *    message. The calling task must deallocate the message buffer after
 *    processing the message using the osal_msg_deallocate() call.
 *
 * @param   uint8 task_id - receiving tasks ID
 *
 * @return  *uint8 - message information or NULL if no message
 */
/* 接收发送给当前任务的消息 */
uint8 *osal_msg_receive( uint8 task_id )
{
  osal_msg_hdr_t *listHdr;// 当前遍历的消息头指针
  osal_msg_hdr_t *prevHdr = NULL; // 前驱消息头指针（用于链表操作）
  osal_msg_hdr_t *foundHdr = NULL;// 找到的目标消息头指针
  halIntState_t   intState;// 保存中断状态（用于临界区保护）

  // 进入临界区（禁用中断）
  HAL_ENTER_CRITICAL_SECTION(intState);

  // 从队列头开始遍历
  listHdr = osal_qHead;

  // 查找属于 task_id 的消息
  while ( listHdr != NULL )
  {
    // 检查消息目标任务ID是否匹配
    if ( (listHdr - 1)->dest_id == task_id )
    {
      if ( foundHdr == NULL )
      {
        // 记录第一个匹配的消息
        foundHdr = listHdr;
      }
      else
      {
        // 找到第二个匹配消息时停止遍历（保留剩余消息在队列中）
        break;
      }
    }
    // 更新前驱指针（仅当未找到消息时）
    if ( foundHdr == NULL )
    {
      prevHdr = listHdr;
    }
    // 移动到下一个消息
    listHdr = OSAL_MSG_NEXT( listHdr );
  }

  // 根据是否还有剩余消息设置/清除事件标志
  if ( listHdr != NULL )
  {
    // 仍有消息未处理，触发事件通知
    osal_set_event( task_id, SYS_EVENT_MSG );
  }
  else
  {
    // 无剩余消息，清除事件标志
    osal_clear_event( task_id, SYS_EVENT_MSG );
  }

  // 如果找到消息
  if ( foundHdr != NULL )
  {
    // 将其从队列中移除
    osal_msg_extract( &osal_qHead, foundHdr, prevHdr );
  }

   // 退出临界区（恢复中断状态）
  HAL_EXIT_CRITICAL_SECTION(intState);

  return ( (uint8*) foundHdr );
}

/**************************************************************************************************
 * @fn          osal_msg_find
 *
 * @brief       This function finds in place an OSAL message matching the task_id and event
 *              parameters.
 *
 * input parameters
 *
 * @param       task_id - The OSAL task id that the enqueued OSAL message must match.
 * @param       event - The OSAL event id that the enqueued OSAL message must match.
 *
 * output parameters
 *
 * None.
 *
 * @return      NULL if no match, otherwise an in place pointer to the matching OSAL message.
 **************************************************************************************************
 */
osal_event_hdr_t *osal_msg_find(uint8 task_id, uint8 event)
{
  osal_msg_hdr_t *pHdr;       // 当前遍历的消息头指针
  halIntState_t intState;     // 保存中断状态（用于临界区保护）
 
  // 进入临界区（禁用中断）
  HAL_ENTER_CRITICAL_SECTION(intState);
 
  // 从队列头开始遍历
  pHdr = osal_qHead;
 
  // 查找匹配 task_id 和 event 的消息
  while (pHdr !=NULL)
  {
    // 检查消息目标任务ID和事件类型是否匹配
    if (((pHdr - 1)->dest_id == task_id) && (((osal_event_hdr_t *)pHdr)->event == event))
    {
      break; // 找到匹配项，退出循环
    }
 
    // 移动到下一个消息
    pHdr = OSAL_MSG_NEXT(pHdr);
  }
 
  // 退出临界区（恢复中断状态）
  HAL_EXIT_CRITICAL_SECTION(intState);
 
  // 返回匹配的消息头指针（可能为）
  return (osal_event_hdr_t *)pHdr;
}

/*********************************************************************
 * @fn      osal_msg_enqueue
 *
 * @brief
 *
 *    This function enqueues an OSAL message into an OSAL queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message
 *
 * @return  none
 */
void osal_msg_enqueue(osal_msg_q_t *q_ptr, void *msg_ptr)
{
  void *list;           // 临时指针，用于遍历队列
  halIntState_t intState; // 保存中断状态（用于临界区保护）
 
  // 进入临界区（禁用中断）
  HAL_ENTER_CRITICAL_SECTION(intState);
 
  // 将消息的下一指针置空（标记为队列末尾）
  OSAL_MSG_NEXT(msg_ptr) =NULL;
 
  // 如果队列为空（*q_ptr ==NULL）
  if (*q_ptr ==NULL)
  {
    *q_ptr = msg_ptr; // 直接将消息设为队列头
  }
  else
  {
    // 遍历队列找到末尾
    for (list = *q_ptr; OSAL_MSG_NEXT(list) !=NULL; list = OSAL_MSG_NEXT(list));
 
    // 将消息添加到队列末尾
    OSAL_MSG_NEXT(list) = msg_ptr;
  }
 
  // 退出临界区（恢复中断状态）
  HAL_EXIT_CRITICAL_SECTION(intState);
}

/*********************************************************************
 * @fn      osal_msg_dequeue
 *
 * @brief
 *
 *    This function dequeues an OSAL message from an OSAL queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 *
 * @return  void * - pointer to OSAL message or NULL of queue is empty.
 */
void *osal_msg_dequeue(osal_msg_q_t *q_ptr)
{
  void *msg_ptr =NULL;      // 存储出队消息的指针
  halIntState_t intState;    // 保存中断状态（用于临界区保护）
 
  // 进入临界区（禁用中断）
  HAL_ENTER_CRITICAL_SECTION(intState);
 
  // 如果队列非空
  if (*q_ptr !=NULL)
  {
    // 出队操作：获取队列头消息
    msg_ptr = *q_ptr;
    // 更新队列头为下一消息
    *q_ptr = OSAL_MSG_NEXT(msg_ptr);
    // 清空出队消息的下一指针（避免野指针）
    OSAL_MSG_NEXT(msg_ptr) =NULL;
    // 标记消息ID为“无任务”（可能用于调试或内存回收）
    OSAL_MSG_ID(msg_ptr) = TASK_NO_TASK;
  }
 
  // 退出临界区（恢复中断状态）
  HAL_EXIT_CRITICAL_SECTION(intState);
 
  return msg_ptr; // 返回出队消息（可能为）
}

/*********************************************************************
 * @fn      osal_msg_push
 *
 * @brief
 *
 *    This function pushes an OSAL message to the head of an OSAL
 *    queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message
 *
 * @return  none
 */
void osal_msg_push(osal_msg_q_t *q_ptr, void *msg_ptr)
{
  halIntState_t intState;
 
  // 进入临界区（禁用中断）
  HAL_ENTER_CRITICAL_SECTION(intState);
 
  // 将消息插入队列头部（LIFO）
  OSAL_MSG_NEXT(msg_ptr) = *q_ptr; // 新消息的下一指针指向原队列头
  *q_ptr = msg_ptr;                // 更新队列头为新消息
 
  // 退出临界区（恢复中断状态）
  HAL_EXIT_CRITICAL_SECTION(intState);
}

/*********************************************************************
 * @fn      osal_msg_extract
 *
 * @brief
 *
 *    This function extracts and removes an OSAL message from the
 *    middle of an OSAL queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message to be extracted
 * @param   void *prev_ptr  - OSAL message before msg_ptr in queue
 *
 * @return  none
 */
void osal_msg_extract(osal_msg_q_t *q_ptr, void *msg_ptr, void *prev_ptr)
{
  halIntState_t intState;
 
  // 进入临界区
  HAL_ENTER_CRITICAL_SECTION(intState);
 
  if (msg_ptr == *q_ptr)
  {
    // 情况1：消息在队列头部
    *q_ptr = OSAL_MSG_NEXT(msg_ptr); // 更新队列头为下一消息
  }
  else
  {
    // 情况2：消息在队列中部或尾部
    OSAL_MSG_NEXT(prev_ptr) = OSAL_MSG_NEXT(msg_ptr); // 跳过待提取消息
  }
 
  // 清理提取的消息
  OSAL_MSG_NEXT(msg_ptr) =NULL;
  OSAL_MSG_ID(msg_ptr) = TASK_NO_TASK;
 
  // 退出临界区
  HAL_EXIT_CRITICAL_SECTION(intState);
}
/*********************************************************************
 * @fn      osal_msg_enqueue_max
 *
 * @brief
 *
 *    This function enqueues an OSAL message into an OSAL queue if
 *    the length of the queue is less than max.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message
 * @param   uint8 max - maximum length of queue
 *
 * @return  TRUE if message was enqueued, FALSE otherwise
 */
uint8 osal_msg_enqueue_max(osal_msg_q_t *q_ptr, void *msg_ptr, uint8 max)
{
  void *list;          // 用于遍历队列的临时指针
  uint8 ret = FALSE;   // 返回值，默认失败
  halIntState_t intState;
 
  // 进入临界区（禁用中断）
  HAL_ENTER_CRITICAL_SECTION(intState);
 
  // 情况1：队列为空，直接插入头部
  if (*q_ptr ==NULL)
  {
    *q_ptr = msg_ptr;  // 队列头指向新消息
    ret = TRUE;        // 成功
  }
  else
  {
    // 情况2：队列非空，遍历到尾部或达到 max 限制
    list = *q_ptr;     // 从队列头开始遍历
    max--;             // 已占用1个位置（队列头）
    while ((OSAL_MSG_NEXT(list) !=NULL) && (max > 0))
    {
      list = OSAL_MSG_NEXT(list); // 移动到下一个消息
      max--;                     // 递减剩余可插入位置
    }
 
    // 如果未达到 max 限制，插入到尾部
    if (max != 0)
    {
      OSAL_MSG_NEXT(list) = msg_ptr; // 尾部消息指向新消息
      ret = TRUE;                   // 成功
    }
    // 否则队列已满，丢弃消息（ret保持FALSE）
  }
 
  // 退出临界区（恢复中断状态）
  HAL_EXIT_CRITICAL_SECTION(intState);
 
  return ret;
}

/*********************************************************************
 * @fn      osal_set_event
 *
 * @brief
 *
 *  把事件添加到事件列表  
 *   
 *
 * @param   uint8 task_id - receiving tasks ID
 * @param   uint8 event_flag - what event to set
 *
 * @return  SUCCESS, INVALID_TASK
 */
uint8 osal_set_event( uint8 task_id, uint16 event_flag )
{
  // 检查任务ID是否合法（必须小于系统注册的任务总数 tasksCnt）
  if ( task_id < tasksCnt )
  {
    halIntState_t   intState;  // 定义变量用于保存中断状态
    
    // 进入临界区（关闭中断），防止多线程/中断竞争访问共享资源
    HAL_ENTER_CRITICAL_SECTION(intState);    
    
    // 将事件标志位按位或操作写入对应任务的事件变量（保留原有事件）
    tasksEvents[task_id] |= event_flag;  
    
    // 退出临界区（恢复中断），允许其他中断/线程执行
    HAL_EXIT_CRITICAL_SECTION(intState);     
    
    return ( SUCCESS );  // 返回操作成功状态
  }
  else
  {
    return ( INVALID_TASK );  // 任务ID非法，返回错误码
  }
}

/*********************************************************************
 * @fn      osal_clear_event
 *
 * @brief
 *
 *    This function is called to clear the event flags for a task. The
 *    event passed in is masked out of the task's event variable.
 *
 * @param   uint8 task_id - receiving tasks ID
 * @param   uint8 event_flag - what event to clear
 *
 * @return  SUCCESS, INVALID_TASK
 */
uint8 osal_clear_event( uint8 task_id, uint16 event_flag )
{
  if ( task_id < tasksCnt )
  {
    halIntState_t   intState;
    HAL_ENTER_CRITICAL_SECTION(intState);    // Hold off interrupts
    tasksEvents[task_id] &= ~(event_flag);   // Clear the event bit(s)
    HAL_EXIT_CRITICAL_SECTION(intState);     // Release interrupts
    return ( SUCCESS );
  }
   else
  {
    return ( INVALID_TASK );
  }
}

/*********************************************************************
 * @fn      osal_isr_register
 *
 * @brief
 *
 *   This function is called to register a service routine with an
 *   interrupt. When the interrupt occurs, this service routine is called.
 *
 * @param   uint8 interrupt_id - Interrupt number
 * @param   void (*isr_ptr)( uint8* ) - function pointer to ISR
 *
 * @return  SUCCESS, INVALID_INTERRUPT_ID,
 */
uint8 osal_isr_register( uint8 interrupt_id, void (*isr_ptr)( uint8* ) )
{
  // Remove these statements when functionality is complete
  (void)interrupt_id;
  (void)isr_ptr;
  return ( SUCCESS );
}

/*********************************************************************
 * @fn      osal_int_enable
 *
 * @brief
 *
 *   This function is called to enable an interrupt. Once enabled,
 *   occurrence of the interrupt causes the service routine associated
 *   with that interrupt to be called.
 *
 *   If INTS_ALL is the interrupt_id, interrupts (in general) are enabled.
 *   If a single interrupt is passed in, then interrupts still have
 *   to be enabled with another call to INTS_ALL.
 *
 * @param   uint8 interrupt_id - Interrupt number
 *
 * @return  SUCCESS or INVALID_INTERRUPT_ID
 */
uint8 osal_int_enable( uint8 interrupt_id )
{

  if ( interrupt_id == INTS_ALL )
  {
    HAL_ENABLE_INTERRUPTS();
    return ( SUCCESS );
  }
  else
  {
    return ( INVALID_INTERRUPT_ID );
  }
}

/*********************************************************************
 * @fn      osal_int_disable
 *
 * @brief
 *
 *   This function is called to disable an interrupt. When a disabled
 *   interrupt occurs, the service routine associated with that
 *   interrupt is not called.
 *
 *   If INTS_ALL is the interrupt_id, interrupts (in general) are disabled.
 *   If a single interrupt is passed in, then just that interrupt is disabled.
 *
 * @param   uint8 interrupt_id - Interrupt number
 *
 * @return  SUCCESS or INVALID_INTERRUPT_ID
 */
uint8 osal_int_disable( uint8 interrupt_id )
{

  if ( interrupt_id == INTS_ALL )
  {
    HAL_DISABLE_INTERRUPTS();
    return ( SUCCESS );
  }
  else
  {
    return ( INVALID_INTERRUPT_ID );
  }
}

/*********************************************************************
 * @fn      osal_init_system
 *
 * @brief   Initialize the OSAL (Operating System Abstraction Layer) system.
 *          This function initializes all core components of OSAL, including
 *          memory management, message queues, timers, power management, and tasks.
 *
 * @param   void
 *
 * @return  SUCCESS (always returns SUCCESS in current implementation)
 */
uint8 osal_init_system( void )
{
  // 1. 初始化内存管理系统
  //    - 分配和初始化堆内存（Heap），用于动态内存分配（如osal_mem_alloc/osal_mem_free）。
  //    - 内部可能调用类似 `malloc` 的底层函数，但OSAL会封装成自己的内存管理机制。
  osal_mem_init();

  // 2. 初始化消息队列头指针
  //    - `osal_qHead` 是全局变量，指向任务间消息队列的头部。
  //    - 初始化为表示队列为空，后续通过 `osal_msg_enqueue()`/`osal_msg_dequeue()` 操作队列。
  osal_qHead =NULL;

  // 3. 初始化定时器系统
  //    - 初始化硬件定时器（如CC2530的Timer1）和软件定时器列表。
  //    - 为 `osal_start_timerEx()`/`osal_stop_timerEx()` 等定时器API提供支持。
  osalTimerInit();

  // 4. 初始化电源管理系统
  //    - 设置低功耗模式（如PM1/PM2/PM3）的默认参数。
  //    - 配置唤醒源（如定时器中断、外部中断）。
  //    - 后续可通过 `osal_pwrmgr_task_state()` 控制任务的功耗状态。
  //osal_pwrmgr_init();

  // 5. 初始化系统任务表
  //    - 遍历 `OSAL_Tasks.h` 中定义的任务表（`tasksArr[]` 和 `tasksEvents[]`）。
  //    - 为每个任务分配事件控制块（Event Control Block, ECB），但任务本身未激活。
  //    - 典型任务包括：MAC层任务、NWK层任务、AF层任务、用户自定义任务等。
  osalInitTasks();

  // 6. 优化堆内存的首次空闲块搜索
  //    - 可能通过维护空闲内存链表或位图（Bitmap）加速内存分配。
  //    - 避免动态分配时的线性搜索，提升 `osal_mem_alloc()` 效率。
  osal_mem_kick();

  // 7. 返回成功状态
  //    - 当前实现固定返回 SUCCESS（0），无错误处理逻辑。
  return ( SUCCESS );
}

/*********************************************************************
 * @fn      osal_start_system
 *
 * @brief   OSAL系统的主循环函数（当未定义ZBIT和UBIT时）。
 *          该函数是一个无限循环，持续调用osal_run_system()执行任务调度。
 *          注意：此函数通常不会返回。
 *
 * @param   void
 *
 * @return  none
 */
void osal_start_system( void )
{
#if !defined ( ZBIT ) && !defined ( UBIT )  // 如果未定义ZBIT和UBIT宏
  for(;;)  // 无限循环（主事件循环）
#endif
  {
    osal_run_system();  // 调用OSAL调度器核心函数，处理任务事件
  }
}

/*********************************************************************
 * @fn      osal_run_system
 *
 * @brief   OSAL调度器核心函数，轮询任务事件表并处理最高优先级任务的事件。
 *          若无事件待处理，则进入低功耗模式（可选）。
 *
 * @param   void
 *
 * @return  none
 */
void osal_run_system( void )
{
  uint8 idx = 0;  // 任务索引，用于遍历任务表

  // 1. 更新系统时间（维护OSAL的时钟tick）
  //    - 可能递增全局时间变量（如 `osal_systemClock`），用于定时器超时判断。
  osalTimeUpdate();

  // 2. 处理硬件轮询（如按键、ADC采样等）
  //    - 调用HAL层的 `Hal_ProcessPoll()`，检查硬件事件并触发对应任务。
  Hal_ProcessPoll();

  // 轮询任务
  osalPoll();

  // 3. 查找第一个有待处理事件的任务（按优先级顺序）
  do {
    if (tasksEvents[idx])  // 检查任务idx是否有事件待处理
    {
      break;  // 找到最高优先级的待处理任务，退出循环
    }
  } while (++idx < tasksCnt);  // 遍历所有任务（tasksCnt为任务总数）

  // 4. 处理找到的任务事件
  if (idx < tasksCnt)  // 确保索引有效（未越界）
  {
    uint16 events;          // 临时存储事件标志
    halIntState_t intState; // 保存中断状态（用于临界区保护）

    // 4.1 进入临界区，防止事件标志被中断修改
    HAL_ENTER_CRITICAL_SECTION(intState);
    events = tasksEvents[idx];       // 读取当前任务的事件标志
    tasksEvents[idx] = 0;            // 清除事件标志（表示开始处理）
    HAL_EXIT_CRITICAL_SECTION(intState); // 退出临界区

    // 4.2 设置当前活动任务ID，并调用任务处理函数
    activeTaskID = idx;                     // 标记当前任务为活跃状态
    events = (tasksArr[idx])( idx, events ); // 调用任务的事件处理函数（如 `macEventLoop`）
    activeTaskID = TASK_NO_TASK;            // 清除活跃任务标记

    // 4.3 将未处理的事件重新加入队列（若任务返回剩余事件）
    HAL_ENTER_CRITICAL_SECTION(intState);
    tasksEvents[idx] |= events;  // 合并未处理事件（|= 确保不丢失新事件）
    HAL_EXIT_CRITICAL_SECTION(intState);
  }
#if defined( POWER_SAVING )  // 如果启用了低功耗模式
  else  // 所有任务均无事件（完成一次完整轮询）
  {
    // 5. 进入低功耗模式（如睡眠或待机）
    //    - 调用电源管理模块，根据配置选择休眠级别（如PM2）。
    //    - 可被中断（如定时器、外部事件）唤醒。
    osal_pwrmgr_powerconserve();
  }
#endif

  /* 6. 协作式调度让出CPU（仅当使用非抢占式调度时） */
#if defined (configUSE_PREEMPTION) && (configUSE_PREEMPTION == 0)
  {
    // 若配置为协作式调度（非抢占式），主动调用 `osal_task_yield()` 让出CPU
    // 允许其他任务或中断运行（即使当前任务未执行完）。
    osal_task_yield();
  }
#endif
}

/*********************************************************************
 * @fn      osal_buffer_uint32
 *
 * @brief
 *
 *   将 uint32 类型的值按 LSB（最低有效字节）优先顺序存入缓冲区。
 *
 * @param   buf - 目标缓冲区指针
 * @param   val - 待存储的 uint32 值
 *
 * @return  指向缓冲区末尾的指针（即 buf + 4）
 */
uint8* osal_buffer_uint32( uint8 *buf, uint32 val )
{
  // 将 val 的第 0 字节（LSB）存入 buf，并后移指针
  *buf++ = BREAK_UINT32( val, 0 );
  // 将 val 的第 1 字节存入 buf，并后移指针
  *buf++ = BREAK_UINT32( val, 1 );
  // 将 val 的第 2 字节存入 buf，并后移指针
  *buf++ = BREAK_UINT32( val, 2 );
  // 将 val 的第 3 字节（MSB）存入 buf，并后移指针
  *buf++ = BREAK_UINT32( val, 3 );

  // 返回缓冲区末尾指针
  return buf;
}

/*********************************************************************
 * @fn      osal_buffer_uint24
 *
 * @brief
 *
 *   将 uint24 类型的值按 LSB 优先顺序存入缓冲区。
 *   注意：uint24 类型在 comdef.h 中被 typedef 为 uint32。
 *
 * @param   buf - 目标缓冲区指针
 * @param   val - 待存储的 uint24 值（实际为 uint32 的低 3 字节）
 *
 * @return  指向缓冲区末尾的指针（即 buf + 3）
 */
uint8* osal_buffer_uint24( uint8 *buf, uint24 val )
{
  // 将 val 的第 0 字节（LSB）存入 buf，并后移指针
  *buf++ = BREAK_UINT32( val, 0 );
  // 将 val 的第 1 字节存入 buf，并后移指针
  *buf++ = BREAK_UINT32( val, 1 );
  // 将 val 的第 2 字节存入 buf，并后移指针
  *buf++ = BREAK_UINT32( val, 2 );

  // 返回缓冲区末尾指针
  return buf;
}

/*********************************************************************
 * @fn      osal_isbufset
 *
 * @brief
 *
 *   检查缓冲区中所有元素是否等于指定值。
 *
 * @param   buf - 待检查的缓冲区指针
 * @param   val - 目标比较值
 * @param   len - 需检查的缓冲区长度
 *
 * @return  TRUE - 所有元素均等于 val
 *          FALSE - 存在不等于 val 的元素
 */
uint8 osal_isbufset( uint8 *buf, uint8 val, uint8 len )
{
  uint8 x; // 循环计数器

  // 检查缓冲区指针是否为空
  if ( buf == NULL)
  {
    return ( FALSE ); // 返回 FALSE 表示无效输入
  }

  // 遍历缓冲区的前 len 个字节
  for ( x = 0; x < len; x++ )
  {
    // 检查当前字节是否不等于 val
    if ( buf[x] != val )
    {
      return ( FALSE ); // 发现不匹配，立即返回 FALSE
    }
  }
  return ( TRUE ); // 所有字节均匹配，返回 TRUE
}

/*********************************************************************
 * @fn      osal_self
 *
 * @brief
 *
 *   返回当前活动任务的 ID。
 *
 * @param   void
 *
 * @return  当前活动任务 ID；若无活动任务，返回 TASK_NO_TASK
 */
uint8 osal_self( void )
{
  // 返回全局变量 activeTaskID（需确保其已正确定义）
  return ( activeTaskID );
}

/*********************************************************************
 */
