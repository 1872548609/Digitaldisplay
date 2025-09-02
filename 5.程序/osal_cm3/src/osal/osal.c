/*********************************************************************
    Filename:       OSAL.c
    Revised:        $Date: 2008/09/20 $
    Revision:       $Revision: 0.10 $

    Description:    

       This API allows the software components in the []  to be 
       written independently of the specifics of the operating system,
       kernel or tasking environment (including control loops or 
       connect-to-interrupt systems). 

*********************************************************************/


/*********************************************************************
 * INCLUDES
 */
#ifdef WIN32
  #include <stdio.h>
#endif

#include <stdlib.h>
#include <string.h>

#include "OSALDef.h"
#include "OSAL.h"
#include "OSAL_Tasks.h"
#include "OSAL_Custom.h"
#include "OSAL_Memory.h"
#include "OSAL_PwrMgr.h"


#ifndef WIN32
  #include "OnBoard.h"
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

// Message Header
typedef struct
#ifdef __GNUC__
__attribute__ ((packed))
#endif
{
  byte dest_id;
  byte len;
  void *next;
  void *prev;
}osal_msg_hdr_t;


/*********************************************************************
 * GLOBAL VARIABLES
 */

// Message Pool Definitions
osal_msg_hdr_t *osal_qHead;

osal_msg_received_t osal_rcvd;

#if defined( OSAL_TOTAL_MEM )
  UINT16 osal_msg_cnt;
#endif

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

/*********************************************************************
 * HELPER FUNCTIONS
 */

/*********************************************************************
 * @fn      osal_strlen 
 *
 * @brief   
 *
 *   Calculates the length of a string.  The string must be null
 *   terminated.
 *
 * @param   const char *pString - pointer to text string
 *
 * @return  int - number of characters
 */
int osal_strlen( const char *pString )
{
  return strlen( pString );  
}

/*********************************************************************
 * @fn      osal_memcpy
 *
 * @brief   
 *
 *   Generic memory copy. 
 *
 *  Note: This function is differnt from the standard memcpy in the 
 *        return value.  The standard memcpy returns the original
 *        destination address.
 *
 * @param   dst - destination address
 * @param   src - source address
 * @param   len - number of bytes to copy
 *
 * @return  pointer to end of destination buffer
 */
void *osal_memcpy( void *dst, const void *src, unsigned int len )
{
#if defined( GCC )
  return ( memcpy( dst, src, len ) + len );
#else
  while ( len-- )
    *((unsigned char*)dst)++ = *((unsigned char*)src)++;

  return ( dst );
#endif
} 

/*********************************************************************
 * @fn      osal_memset
 *
 * @brief   
 *
 *   Set memory buffer to value.
 *
 * @param   dest - pointer to buffer
 * @param   value - what to set each byte of the message
 * @param   size - how big
 *
 * @return  value of next widget, 0 if no widget found
 */
void *osal_memset( void *dest, byte value, int len )
{
  return memset( dest, value, len );
}

/*********************************************************************
 * @fn      osal_cpyExtAddr
 *
 * @brief   
 *
 *   Copy extended addresses.  This function will copy 8 bytes. 
 *
 * @param   dest  destination address pointer
 * @param   src   source address pointer
 *
 * @return  dest + Z_EXTADDR_LEN
 */
void *osal_cpyExtAddr( void *dest, const void *src )
{
  return osal_memcpy( dest, src, Z_EXTADDR_LEN );
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
uint16 osal_build_uint16( byte *swapped )
{
  return ( BUILD_UINT16( (uint16)(swapped[0]), (uint16)(swapped[1]) ) );
}

/*********************************************************************
 * @fn      osal_build_uint32
 *
 * @brief   
 *
 *   Build a uint32 out of sequential bytes.
 *
 * @param   swapped - sequential bytes
 * @param   len - number of bytes in the byte array
 *
 * @return  uint32 
 */
uint32 osal_build_uint32( byte *swapped, byte len )
{
  if ( len == 2 )
    return ( BUILD_UINT32( swapped[0], swapped[1], 0L, 0L ) );
  else if ( len == 3 )
    return ( BUILD_UINT32( swapped[0], swapped[1], swapped[2], 0L ) );
  else if ( len == 4 )
    return ( BUILD_UINT32( swapped[0], swapped[1], swapped[2], swapped[3] ) );
  else
    return ( (uint32)swapped[0] );
}

#if 0  //avr standard lib has ltoa function   liu0920
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
unsigned char * _ltoa(unsigned long l, unsigned char *buf, unsigned char radix)
{
#if defined( GCC )
  return ( (char*)ltoa( l, buf, radix ) );
#else
  unsigned char tmp1[10] = "", tmp2[10] = "", tmp3[10] = "";
  unsigned short num1, num2, num3;
  unsigned char i;

  buf[0] = '\0';

  if ( radix == 10 ) {
    num1 = l % 10000;
    num2 = (l / 10000) % 10000;
    num3 = (unsigned short)(l / 100000000);

    if (num3) itoa(num3, tmp3/*, 10*/);   /*liu0920 _itoa->itoa for avr*/
    if (num2) itoa(num2, tmp2/*, 10*/);
    if (num1) itoa(num1, tmp1/*, 10*/);

    if (num3) {
      strcpy(buf, tmp3);
      for (i = 0; i < 4 - strlen(tmp2); i++)
        strcatf(buf, "0");   /*liu0920 strcatf*/
    }
    strcat(buf, tmp2);
    if (num3 || num2) {
      for (i = 0; i < 4 - strlen(tmp1); i++)
        strcatf(buf, "0");   /*liu0920 strcatf*/
    }
    strcat(buf, tmp1);
    if (!num3 && !num2 && !num1)
      strcpy(buf, (char *)("0"));    
  }
  else if ( radix == 16 ) {
    num1 = l & 0x0000FFFF;
    num2 = l >> 16;

    if (num2) itoa(num2, tmp2/*, 16*/);   /*liu0920*/
    if (num1) itoa(num1, tmp1/*, 16*/);

    if (num2){
      strcpy(buf, tmp2);
      for (i = 0; i < 4 - strlen(tmp1); i++)
        strcatf(buf, "0");        /*liu0920 strcatf*/
    }
    strcat(buf, tmp1);
    if (!num2 && !num1)
      strcpy(buf, (char *)("0"));          
  }
  else
    return NULL;
    
  return buf;
#endif
}
#endif 

/*********************************************************************
 * @fn          osal_AddrEqual
 *
 * @brief       Compares two address_t structures.
 *
 * @param       a1 - first Address
 * @param       a2 - second endpoint
 *
 * @return      true - addresses are equal, false - not equal
 */
#if 0
byte osal_AddrEqual( zAddrType_t *a1, zAddrType_t *a2 )
{
  if ( a1->addrMode != a2->addrMode )
    return ( false );
  else                             /*liu0920 not support else if! fool!*/
     if (a1->addrMode == AddrNotPresent )
        return ( true );
     else 
        if (a1->addrMode == Addr16Bit )
           return ( a1->addr.shortAddr == a2->addr.shortAddr );
        else 
           if ( a1->addrMode == Addr64Bit )
             return ( osal_ExtAddrEqual( a1->addr.extAddr, a2->addr.extAddr ) );
           else 
             return ( false );
  
}    
#endif 

/*********************************************************************
 * @fn      osal_ExtAddrEqual()
 *
 * @brief   Verify that the extended addresses are equal.
 *
 * @param   extAddr1 - extended address
 * @param   extAddr2 - extended address
 *
 * @return  TRUE - If addresses are same
 *          FALSE  - If not same
 */
byte osal_ExtAddrEqual( byte *extAddr1, byte *extAddr2 )
{
  byte i;

  for ( i = Z_EXTADDR_LEN; i ; i-- ) {
    if ( *extAddr1++ != *extAddr2++ )
      return ( false );
  }

  return ( true );
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
 *    When the call is made, msg_ptr should be NULL or set to an unused 
 *    value as the result of the call will result in the return value 
 *    being NULL (if no message was allocated as indicated by Status) or 
 *    the pointer to the newly allocated buffer).
 *
 *
 * @param   byte **msg_ptr - pointer to new message buffer pointer
 * @param   byte len  - wanted buffer length
 *
 *
 * @return  ZSUCCESS, INVALID_TASK, MSG_BUFFER_NOT_AVAIL, INVALID_LEN
 */
byte osal_msg_allocate( byte **msg_ptr, byte len )
{
  byte *memPtr;
  osal_msg_hdr_t *hdr;
  
  if ( len == 0 )
    return ( INVALID_LEN );

  memPtr = osal_mem_alloc( (uint16)(len) + sizeof( osal_msg_hdr_t ) );
  if ( memPtr ) {
    hdr = (osal_msg_hdr_t *)memPtr;
    memPtr += sizeof(osal_msg_hdr_t);
    hdr->dest_id = TASK_NO_TASK;
    hdr->len = len;
    hdr->next = (void *)NULL;
    hdr->prev = (void *)NULL;
    *msg_ptr = memPtr;

#if defined( OSAL_TOTAL_MEM )
    osal_msg_cnt++;
#endif    
    return ( ZSUCCESS );
  }
  else
    return ( MSG_BUFFER_NOT_AVAIL );
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
 * @param   byte *msg_ptr - pointer to new message buffer
 *
 * @return  ZSUCCESS, INVALID_MSG_POINTER
 */
byte osal_msg_deallocate( byte *msg_ptr )
{
  byte*x;

  if ( msg_ptr == NULL )
    return ( INVALID_MSG_POINTER );

  x = (byte *)((byte *)msg_ptr - sizeof( osal_msg_hdr_t ));

  osal_mem_free( (void *)x );

#if defined( OSAL_TOTAL_MEM )
  if ( osal_msg_cnt ) 
    osal_msg_cnt--;
#endif    

  return ( ZSUCCESS );
}

#if defined( OSAL_TOTAL_MEM )
/*********************************************************************
 * @fn      osal_num_msgs
 *
 * @brief   
 *
 *    This function returns the number of allocated messages
 *
 * @param   void
 *
 * @return  UINT16 - number of msgs out
 */
UINT16 osal_num_msgs( void )
{
  return ( osal_msg_cnt );
}
#endif

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
 * @param   byte destination task - Send msg to?  Task ID
 * @param   byte *msg_ptr - pointer to new message buffer
 * @param   byte len - length of data in message
 *
 * @return  ZSUCCESS, INVALID_SENDING_TASK, INVALID_DESTINATION_TASK, 
 *          INVALID_MSG_POINTER, INVALID_LEN
 */
byte osal_msg_send( byte destination_task, byte *msg_ptr, byte len )
{
  byte *x;
  osal_msg_hdr_t *hdr;
  osal_msg_hdr_t *listHdr;
  IntsStorage;

  if ( len == 0 ) {
    osal_msg_deallocate( msg_ptr );
    return ( INVALID_LEN );
  }
    
  if ( msg_ptr == NULL )
    return ( INVALID_MSG_POINTER );  

  if ( osalFindTask( destination_task ) == NULL ) {
    osal_msg_deallocate( msg_ptr );
    return ( INVALID_TASK );
  }
    
  // Get the message's header
  x = (byte *)((byte*)msg_ptr - sizeof( osal_msg_hdr_t ));
  hdr = (osal_msg_hdr_t *)x;
  
  // Check the message header
  if ( hdr->next != NULL || hdr->prev != NULL
    || hdr->dest_id != TASK_NO_TASK || hdr->len != len ) {
    osal_msg_deallocate( msg_ptr );
    return ( INVALID_MSG_POINTER );
  }

  // Hold off interrupts
  StoreDisableInts;

  if ( osal_qHead ) {
    // Point to the top of the list
    listHdr = osal_qHead;

    // Find the end of the list
    while ( listHdr && listHdr->next )
        listHdr = listHdr->next;

    // End is found, put new message into the list
    hdr->prev = listHdr;
    listHdr->next = hdr;
  }
  else {
    // First message
    osal_qHead = hdr;
  }

  hdr->dest_id = destination_task;

  // Signal the task that a message is waiting
  osal_set_event( destination_task, SYS_EVENT_MSG );

  // Re-enable interrupts
  RestoreInts;
    
  return ( ZSUCCESS );
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
 * @param   byte task_id - receiving tasks ID
 *
 * @return  *osal_msg_received_t - message information or NULL if no message
 */
osal_msg_received_t *osal_msg_receive( byte task_id )
{
  osal_msg_hdr_t *listHdr;
  IntsStorage;

  // Hold off interrupts
  StoreDisableInts;
  
  // Point to the top of the queue
  listHdr = osal_qHead;

  // Look through the queue for a message that belongs to the asking task
  while ( listHdr && listHdr->dest_id != task_id )
    listHdr = listHdr->next;

  // Did we find a message?
  if ( listHdr == NULL || listHdr->dest_id != task_id ) {
    // Release interrupts
    RestoreInts;
    return NULL;
  }

  // Take out of the link list
  if ( listHdr == osal_qHead )
    osal_qHead = listHdr->next;
  if ( listHdr->prev )
    ((osal_msg_hdr_t *)(listHdr->prev))->next = listHdr->next;
  if ( listHdr->next )
    ((osal_msg_hdr_t *)(listHdr->next))->prev = listHdr->prev;
  listHdr->dest_id = TASK_NO_TASK;
  listHdr->next = NULL;
  listHdr->prev = NULL;

  // Setup the return structure
  osal_rcvd.hdr.task_id = task_id;
  osal_rcvd.hdr.dest_task_id = task_id;
  osal_rcvd.hdr.send_len = listHdr->len;
  osal_rcvd.msg_ptr = (byte*)((byte*)listHdr + sizeof( osal_msg_hdr_t ));

  // Release interrupts
  RestoreInts;
  
  return ( &osal_rcvd );
}

/*********************************************************************
 * @fn      osal_set_event 
 *
 * @brief   
 *
 *    This function is called to set the event flags for a task.  The
 *    event passed in is OR'd into the task's event variable.
 *
 * @param   byte task_id - receiving tasks ID
 * @param   byte event_flag - what event to set
 *
 * @return  ZSUCCESS, INVALID_TASK
 */
byte osal_set_event( byte task_id, UINT16 event_flag )
{
  osalTaskRec_t *srchTask;
  IntsStorage;

  srchTask = osalFindTask( task_id );
  if ( srchTask ) {
    // Hold off interrupts
    StoreDisableInts;
    // Stuff the event bit(s)
    srchTask->events |= event_flag;
    // Release interrupts
    RestoreInts;
  }
   else
    return ( INVALID_TASK );

  return ( ZSUCCESS );
}

#if 0   //liu0920 remove
/*********************************************************************
 * @fn      osal_isr_register 
 *
 * @brief   
 *
 *   This function is called to register a service routine with an 
 *   interrupt. When the interrupt occurs, this service routine is called.
 *
 * @param   byte interrupt_id - Interrupt number
 * @param   void (*isr_ptr)( byte* ) - function pointer to ISR
 *
 * @return  ZSUCCESS, INVALID_INTERRUPT_ID, or INVALID_ISR_PTR
 */
byte osal_isr_register( byte interrupt_id, void (*isr_ptr)( byte* ) )
{
  return ( ZSUCCESS );
}    
#endif 

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
 * @param   byte interrupt_id - Interrupt number
 *
 * @return  ZSUCCESS or INVALID_INTERRUPT_ID
 */
byte osal_int_enable( byte interrupt_id )
{

  if ( interrupt_id == INTS_ALL ) {
    EnableInts;
  }
  else
    return ( INVALID_INTERRUPT_ID );

  return ( ZSUCCESS );
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
 * @param   byte interrupt_id - Interrupt number
 *
 * @return  ZSUCCESS or INVALID_INTERRUPT_ID
 */
byte osal_int_disable( byte interrupt_id )
{

  if ( interrupt_id == INTS_ALL ) {
    DisableInts;
  }
  else
    return ( INVALID_INTERRUPT_ID );

  return ( ZSUCCESS );
}

/*********************************************************************
 * @fn      osal_init_system 
 *
 * @brief   
 *
 *   This function initializes the "task" system by creating the 
 *   tasks defined in the task table (OSAL_Tasks.h).
 *
 * @param   void
 *
 * @return  ZSUCCESS
 */
byte osal_init_system( void )
{
  // Initialize the Memory Allocation System
  osal_mem_init();

  // Initialize the message queue
  osal_qHead = NULL;
  
#if defined( OSAL_TOTAL_MEM )
  osal_msg_cnt = 0;
#endif  

  // Initialize the timers
  osalTimerInit();

  // Initialize the Power Management System
  osal_pwrmgr_init();
  
  // Initialize the tasking system
  osalTaskInit();
  osalAddTasks();
  osalInitTasks();

  return ( ZSUCCESS );
}

/*********************************************************************
 * @fn      osal_start_system 
 *
 * @brief   
 *
 *   This function is the main loop function of the task system.  It
 *   will look through all task events and call the task_event_processor()
 *   function for the task with the event.  If there are no events (for 
 *   all tasks), this function puts the processor into Sleep.
 *   This Function doesn't return.
 *
 * @param   void
 *
 * @return  none
 */
void osal_start_system( void )
{
  uint16 events;
  byte activity;
  IntsStorage;

  // Forever Loop
#ifndef WIN32
  for(;;)
#endif
  {

#if !defined( TIMER_INT )
  // Check the Timer for activity
  osal_check_timer();
#endif    

//#if defined( MT_TASK ) && (!defined( SERIAL_RX_INT ) || !defined( SERIAL_TX_INT ))
  // Check for serial activity
//  MT_SerialPoll();
//#endif    
  
    activity = false;

    activeTask = osalNextActiveTask();
    if ( activeTask ) {
      StoreDisableInts;
      events = activeTask->events;
      // Clear the Events for this task
      activeTask->events = 0; 
      RestoreInts;

      if ( events != 0 ) {
        // Call the task to process the event(s)
        if ( activeTask->pfnEventProcessor )
          (activeTask->pfnEventProcessor)( activeTask->taskID, events );

        activity = true;
      }
    }

    // Give the RF some time.
    //-activity |= Z_RFProcess();  //liu0920 remove

    // Complete pass through all task events with no activity?
    if ( activity == false ) {
#if defined( POWER_SAVING )
      // Put the processor/system into sleep
      osal_pwrmgr_powerconserve();
#endif
	//__WFI();
    }
  }
}

/*********************************************************************
 * @fn      osal_self 
 *
 * @brief   
 *
 *   This function returns the task ID of the calling (current) task.
 *
 * @param   void
 *
 * @return  byte task ID, 0xFF bad task ID
 */
byte osal_self( void )
{
  if ( activeTask )
    return ( activeTask->taskID );
  else
    return ( TASK_NO_TASK );
}

/*********************************************************************
*********************************************************************/
