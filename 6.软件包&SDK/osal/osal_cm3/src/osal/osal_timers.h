#ifndef OSAL_TIMERS_H
#define OSAL_TIMERS_H

/*********************************************************************
    Filename:       OSAL_Timers.h
    Revised:        $Date: 2008/09/20 $
    Revision:       $Revision: 0.10 $
    
    Description:    
    
       This file contains the OSAL Timer definition and manipulation
       functions.
            
*********************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
 
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
 
/*********************************************************************
 * FUNCTIONS
 */

  /*
   * Initialization for the OSAL Timer System.
   */
  extern void osalTimerInit( void );

  /*
   * Set a Timer
   */
  extern byte osal_start_timer( UINT16 event_id, UINT16 timeout_value );
  extern byte osal_start_timerEx( byte task_id, UINT16 event_id, UINT16 timeout_value );

  /*
   * Stop a Timer
   */
  extern byte osal_stop_timer( UINT16 event_id );
  extern byte osal_stop_timerEx( byte task_id, UINT16 event_id );

  /*
   * Simulated Timer Interrupt Service Routine
   */
  //extern void osal_timer_ISR( void );  //liu0920 remove


  /*
   * Check for timer tick.
   */
  extern void osal_check_timer( void );

  /*
   * Update timer tables
   */
  extern void osal_update_timers( void );
  
  /*
   * Retune the active timers
   */
  extern  void osal_retune_timers( void );
 
  /*
   * Set the hardware timer interrupts for sleep mode.
   * These functions should only be called in OSAL_PwrMgr.c
   */
  extern void osal_sleep_timers( void );
  extern void osal_unsleep_timers( void );

 /*
  * Read the system clock - returns milliseconds
  */
  extern uint32 osal_GetSystemClock( void );
  
  
  //liu0920 add some delay func
  extern void osal_delay_nus(unsigned int n);       //N us延时函数
  
  extern void osal_delay_1ms(void);                 //1ms延时函数
  
  extern void osal_delay_nms(unsigned int n);       //N ms延时函数 ,general use for less then 10ms ,liunote
  
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_TIMERS_H */
