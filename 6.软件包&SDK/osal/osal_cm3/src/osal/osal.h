#ifndef OSAL_H
#define OSAL_H

/*********************************************************************
    Filename:       OSAL.h
    Revised:        $Date: 2008/09/20 $
    Revision:       $Revision: 0.10 $
    
    Description:    
    
       This API allows the software components in the [] to be 
       written independently of the specifics of the operating system,
       kernel or tasking environment (including control loops or 
       connect-to-interrupt systems). 
            
*********************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif      

//liu0920 add this define for use osal standard ,string lib ,(CCAVR lib has't memset,memcpy) 
#define OSAL_WRAPPER_STLIB

/*********************************************************************
 * INCLUDES
 */
#include "OSALDef.h"
#include "OSAL_Timers.h"
 
/*********************************************************************
 * MACROS
 */
 
#define osal_offsetof(type, member) ((uint16) &(((type *) 0)->member))


/*********************************************************************
 * CONSTANTS
 */
 
/*** System Events ***/
#define SYS_EVENT_MSG             0x8000     //liu0920 for osal_msg_send ,communicate between task and task!!!


/*** Interrupts ***/
#define INTS_ALL    0xFF          


/*********************************************************************
 * TYPEDEFS
 */

typedef struct
#ifdef __GNUC__
__attribute__ ((packed))
#endif
{
  byte task_id;             // Task id of allocating task, then
                            //   changed when sent
  byte dest_task_id;        // filled in when sent, cleared when
                            //   received.
  byte send_len;            // filled in when sent
}osal_msg_rec_header_t;

typedef struct
#ifdef __GNUC__
__attribute__ ((packed))
#endif
{
  osal_msg_rec_header_t hdr;
  byte *msg_ptr;
} osal_msg_received_t;



/*********************************************************************
 * GLOBAL VARIABLES
 */
 
extern osal_msg_received_t osal_rcvd;

/*********************************************************************
 * FUNCTIONS
 */

/*** Message Management ***/

  /*
   * Task Message Allocation
   */
  extern byte osal_msg_allocate( byte **msg_ptr, byte len );

  /*
   * Task Message Deallocation
   */
  extern byte osal_msg_deallocate( byte *msg_ptr );
  
  /*
   * Task Messages Count
   */
  extern UINT16 osal_num_msgs( void );
  
  /*
   * Send a Task Message
   */
  extern byte osal_msg_send( byte destination_task,
                             byte *msg_ptr, byte len );
  
  /*
   * Receive a Task Message
   */
  extern osal_msg_received_t *osal_msg_receive( byte task_id );
  
  
/*** Task Synchronization  ***/

  /*
   * Set a Task Event
   */
  extern byte osal_set_event( byte task_id, UINT16 event_flag );
  
  
/*** Interrupt Management  ***/

  /*
   * Register Interrupt Service Routine (ISR)
   */
 // extern byte osal_isr_register( byte interrupt_id, void (*isr_ptr)( byte* ) ); //liu0920

  /*
   * Enable Interrupt
   */
  extern byte osal_int_enable( byte interrupt_id );

  /*
   * Disable Interrupt
   */
  extern byte osal_int_disable( byte interrupt_id );


/*** Task Management  ***/

  /*
   * Initialize the Task System
   */
  extern byte osal_init_system( void );
  
  /*
   * System Processing Loop
   */
  extern void osal_start_system( void );

  /*
   * Get the active task ID
   */
  extern byte osal_self( void );

/*** Memory Allocation Functions ***/

  /*
   * Allocate a block of memory
   */
  extern void *osal_mem_alloc( short size );

  /*
   * Free a block of memory
   */
  extern byte osal_mem_free( void *ptr );

  /*
   * Calculate unused allocation space
   */
  short osal_calc_free_space( void );


/*** Helper Functions ***/

#if defined(OSAL_WRAPPER_STLIB)
  /*
   * String Length
   */
  extern int osal_strlen( const char *pString );

  /*
   * Memory copy
   */
  extern void* osal_memcpy( void*, const void *, unsigned int );
  
  /*
   * Memory set
   */
  extern void *osal_memset( void *dest, byte value, int len );
#else
  #define osal_strlen(a)      strlen(a)
  #define osal_memcpy(a,b,c)  memcpy(a,b,c)
  #define osal_memset(a,b,c)  memset(a,b,c)
#endif   
             
  /*
   * Build a uint16 out of 2 bytes (0 then 1).
   */
  extern uint16 osal_build_uint16( byte *swapped );
  
  /*
   * Build a uint32 out of sequential bytes.
   */
  extern uint32 osal_build_uint32( byte *swapped, byte len );
  
  /*
   * Convert long to ascii string
   */
  //extern byte *_ltoa( uint32 l, byte * buf, byte radix );  //liu0920

  /*
   * Compares two address_t structures. true if equal
   */
  // extern byte osal_AddrEqual( zAddrType_t *a1, zAddrType_t *a2 ); //liu0920 will modi for 2401 addr

  /*
   * Copy Extended Addresses
   */
  extern void* osal_cpyExtAddr( void *dest, const void *src );

  /*
   * Verify that the extended addresses are equal. true if equal
   */
  extern byte osal_ExtAddrEqual( byte *extAddr1, byte *extAddr2 );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_H */
