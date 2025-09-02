/*********************************************************************
    Filename:       OSAL_Timers.c
    Revised:        $Date: 2008/09/20$
    Revision:       $Revision: 0.10 $
    
    Description:    OSAL Timer definition and manipulation functions.
   
*********************************************************************/

/*********************************************************************
 * INCLUDES
 */

#include "OSALDef.h"
#include "OnBoard.h"
#include "OSAL.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define DEFAULT_DECR_TIME     0x0001  // 1ms - has to be matched with Modulo
#define TIMEOUT_THRESHOLD     0x0032  // Threshold for power saving algorithm
#define DEFAULT_TMOD_ROLLOVER TC_OCC  // Rollover counter value Modulo

/*********************************************************************
 * TYPEDEFS
 */
/* 
typedef struct
{
  byte task_id;
  UINT16 event_flag;
  UINT16 timeout;
  void *next;
} osalTimerRec_t; 
*/

/*********************************************************************
 * GLOBAL VARIABLES
 */

osalTimerRec_t *timerHead;
uint16 tmr_tmod_rollover;  // Rollover for system timer
uint16 tmr_decr_time;      // Decr_Time for system timer
byte timerActive;          // Flag if hw timer active

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
// Milliseconds since last reboot
static uint32 osal_systemClock;

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */
osalTimerRec_t  *osalAddTimer( byte task_id, UINT16 event_flag, UINT16 timeout );
osalTimerRec_t *osalFindTimer( byte task_id, uint16 event_flag );
void osalDeleteTimer( osalTimerRec_t *rmTimer );

void osal_timer_activate( byte turn_on );
void osal_timer_hw_setup( byte turn_on );
byte osal_timer_num_active( void );
void osal_set_timer_interrupt( byte turn_on );

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osalTimerInit       liu0920 callby osal_init_system
 *
 * @brief   Initialization for the OSAL Timer System.
 *
 * @param   none
 *
 * @return  
 */
void osalTimerInit( void )
{
  // Initialize the rollover modulo
  tmr_tmod_rollover = DEFAULT_TMOD_ROLLOVER;
  tmr_decr_time = DEFAULT_DECR_TIME;

  // Initialize the system timer
  osal_timer_activate( false );
  timerActive = false;

  osal_systemClock = 0;
}

/*********************************************************************
 * @fn      osalAddTimer           
 *
 * @brief   Add a timer to the timer list.
 *
 * @param   task_id
 * @param   event_flag
 * @param   timeout
 *
 * @return  osalTimerRec_t * - pointer to newly created timer
 */
osalTimerRec_t * osalAddTimer( byte task_id, UINT16 event_flag, UINT16 timeout )
{
  osalTimerRec_t *newTimer;
  osalTimerRec_t *srchTimer;

  // Look for an existing timer first  
  newTimer = osalFindTimer( task_id, event_flag );
  if ( newTimer ) {
    // Timer is found - update it.
    newTimer->timeout = timeout;
    
    return ( newTimer );
  }
  else {
    // New Timer
    newTimer = osal_mem_alloc( sizeof( osalTimerRec_t ) );

    if ( newTimer ) {
      // Fill in new timer
      newTimer->task_id = task_id;
      newTimer->event_flag = event_flag;
      newTimer->timeout = timeout;
      newTimer->next = (void *)NULL;

      // Does the timer list already exist
      if ( timerHead == NULL ) {
        // Start task list
        timerHead = newTimer;
      }
      else {
        // Add it to the end of the timer list
        srchTimer = timerHead;

        // Stop at the last record
        while ( srchTimer->next )
          srchTimer = srchTimer->next;

        // Add to the list
        srchTimer->next = newTimer;
      }

      return ( newTimer );
    }
    else
      return ( (osalTimerRec_t *)NULL );
  }
}

/*********************************************************************
 * @fn      osalFindTimer
 *
 * @brief   Find a timer in a timer list.
 *
 * @param   task_id
 * @param   event_flag
 *
 * @return  osalTimerRec_t *
 */
osalTimerRec_t *osalFindTimer( byte task_id, uint16 event_flag )
{
  osalTimerRec_t *srchTimer;

  // Head of the timer list
  srchTimer = timerHead;

  // Stop when found or at the end
  while ( srchTimer ) {
    if ( srchTimer->event_flag == event_flag && 
         srchTimer->task_id == task_id )
      break;
      
    // Not this one, check another
    srchTimer = srchTimer->next;
  }
  
  return ( srchTimer );
}

/*********************************************************************
 * @fn      osalDeleteTimer
 *
 * @brief   Delete a timer from a timer list.
 *
 * @param   table
 * @param   rmTimer
 *
 * @return  none
 */
void osalDeleteTimer( osalTimerRec_t *rmTimer )
{
  osalTimerRec_t *srchTimer;

  // Does the timer list really exist
  if ( (timerHead != NULL) && rmTimer ) {
    // Add it to the end of the timer list
    srchTimer = timerHead;

    // First element?
    if ( srchTimer == rmTimer ) {
      timerHead = rmTimer->next;
      osal_mem_free( rmTimer );
    }
    else {
      // Stop when found or at the end
      while ( srchTimer->next && srchTimer->next != rmTimer )
        srchTimer = srchTimer->next;

      // Found?
      if ( srchTimer->next == rmTimer ) {
        // Fix pointers
        srchTimer->next = rmTimer->next;

        // Deallocate the timer struct memory
        osal_mem_free( rmTimer );
      }
    }
  }
}

/*********************************************************************
 * @fn      osal_start_timer 
 *
 * @brief   
 *
 *   This function is called to start a timer to expire in n mSecs.   
 *   When the timer expires, the calling task will get the specified event.
 *
 * @param   UINT16 event_id - event to be notified with
 * @param   UNINT16 timeout_value - in milliseconds.
 *
 * @return  ZSUCCESS, NO_TIMER_AVAIL, INVALID_EVENT_ID or INVALID_TIMEOUT_VALUE
 */
byte osal_start_timer( UINT16 event_id, UINT16 timeout_value )
{
  return osal_start_timerEx( osal_self(), event_id, timeout_value );
}

/*********************************************************************
 * @fn      osal_start_timerEx 
 *
 * @brief   
 *
 *   This function is called to start a timer to expire in n mSecs.   
 *   When the timer expires, the calling task will get the specified event.
 *
 * @param   byte taskID - task id to set timer for
 * @param   UINT16 event_id - event to be notified with
 * @param   UNINT16 timeout_value - in milliseconds.
 *
 * @return  ZSUCCESS, NO_TIMER_AVAIL, INVALID_EVENT_ID or INVALID_TIMEOUT_VALUE
 */
byte osal_start_timerEx( byte taskID, UINT16 event_id, UINT16 timeout_value )
{
  osalTimerRec_t *newTimer;
  
#ifdef WIN32
  int newTimeOut;

  // The windows timer has a resolution of 54.5 mSecs
  newTimeOut = (timeout_value / 55);
  if ( newTimeOut <= 0 )
    newTimeOut = 1;
  timeout_value = (UINT16)newTimeOut;
#endif

  // Add timer
  newTimer = osalAddTimer( taskID, event_id, timeout_value );
  if ( newTimer ) {
    // Does the timer need to be started?
    if ( timerActive == false ) {
      osal_timer_activate( true );
    }

    return ( ZSUCCESS );
  }

  return ( NO_TIMER_AVAIL );
}

/*********************************************************************
 * @fn      osal_stop_timer 
 *
 * @brief   
 *
 *   This function is called to stop a timer that has already been started.
 *   If ZSUCCESS, the function will cancel the timer and prevent the event
 *   associated with the timer from being set for the calling task.
 *
 * @param   UINT16 event_id - identifier of the timer that is to be stopped
 *
 * @return  ZSUCCESS or INVALID_EVENT_ID
 */
byte osal_stop_timer( UINT16 event_id )
{
  return osal_stop_timerEx( osal_self(), event_id );
}

/*********************************************************************
 * @fn      osal_stop_timerEx 
 *
 * @brief   
 *
 *   This function is called to stop a timer that has already been started.
 *   If ZSUCCESS, the function will cancel the timer and prevent the event
 *   associated with the timer from being set for the calling task.
 *
 * @param   byte task_id - task id of timer to stop
 * @param   UINT16 event_id - identifier of the timer that is to be stopped
 *
 * @return  ZSUCCESS or INVALID_EVENT_ID
 */
byte osal_stop_timerEx( byte task_id, UINT16 event_id )
{
  osalTimerRec_t *foundTimer;
  
  // Find the timer to stop
  foundTimer = osalFindTimer( task_id, event_id );
  if ( foundTimer ) {
    osalDeleteTimer( foundTimer );
    
    // Does the main timer need to be turned off?
    if ( osal_timer_num_active() == 0 )
      osal_timer_activate( false );
      
    return ( ZSUCCESS );
  }
  
  return ( INVALID_EVENT_ID );
}

/*********************************************************************
 * @fn      osal_timer_activate
 *
 * @brief   
 *
 *   Turns the hardware timer on or off
 *
 * @param  byte turn_on - false - turn off, true - turn on
 *
 * @return  none
 */
void osal_timer_activate( byte turn_on )
{
  osal_timer_hw_setup( turn_on );
  timerActive = turn_on;
}

/*********************************************************************
 * @fn      osal_timer_num_active 
 *
 * @brief   
 *
 *   This function counts the number of active timers.
 *
 * @return  byte - number of timers
 */
byte osal_timer_num_active( void )
{
  byte num_timers;
  osalTimerRec_t *srchTimer;

  num_timers = 0;

  // Head of the timer list
  srchTimer = timerHead;

  // Count timers in the list
  while ( srchTimer != NULL ) {
    num_timers++;
    srchTimer = srchTimer->next;
  }

  return ( num_timers );
}

/*********************************************************************
 * @fn      osal_timer_hw_setup
 *
 * @brief
 *
 *   Setup the timer hardware.
 *
 * @param  byte turn_on
 *
 * @return  void
 */
void osal_timer_hw_setup( byte turn_on )
{
  TimerSetControls( turn_on, tmr_tmod_rollover );
}

/*********************************************************************
 * @fn      osal_sleep_timers
 *
 * @brief   
 *
 *   This function will enable interrupts if timers are running.
 *
 * @param  none
 *
 * @return  none
 */
void osal_sleep_timers( void )   //liu0920 call by powersave mode ,leave suspend
{
#ifndef TIMER_INT
  if ( osal_timer_num_active() )
    osal_set_timer_interrupt( TRUE );
#endif
}

/*********************************************************************
 * @fn      osal_unsleep_timers
 *
 * @brief   
 *
 *   This function will disable interrupts if timers are running.
 *
 * @param  none
 *
 * @return  none
 */
void osal_unsleep_timers( void )
{
#ifndef TIMER_INT
  osal_set_timer_interrupt( FALSE );
#endif    
}

/*********************************************************************
 * @fn      osal_set_timer_interrupt
 *
 * @brief
 *
 *   Setup the timer hardware interrupt.
 *
 * @param  byte turn_on
 *
 * @return  void
 */
void osal_set_timer_interrupt( byte turn_on )
{
  // Enable or disable timer interrupts
  TimerSetInterrupt( turn_on );
}

#if 0
/*********************************************************************
 * @fn      osal_timer_ISR
 *
 * @brief
 *
 *   Interrupt Service Routine for the Hardware Timer.
 *
 * @param   void
 *
 * @return  void
 */
void osal_timer_ISR( void )
{
  osal_update_timers( );
}    
#endif 

/*********************************************************************
 * @fn      osal_check_timer
 *
 * @brief
 *
 *   Check for a timer tick.
 *
 * @note
 *
 * @param   void
 *
 * @return  void
 */
void osal_check_timer( void )
{
  // Check for a timer tick
  //if ( TimerTickCheck() != 0 )   //liu0920 remove now use timer0 overflow interrupt
    osal_update_timers();  // Update if there is one
}

/*********************************************************************
 * @fn      osal_update_timers
 *
 * @brief   Update the timer structures for a timer tick.
 *
 * @param   none
 *
 * @return  none
 */
void osal_update_timers( void )
{
  osalTimerRec_t *srchTimer;
  osalTimerRec_t *prevTimer;
  osalTimerRec_t *saveTimer;

  // Update the system time
  osal_systemClock += tmr_decr_time;

  // Look for open timer slot
  if ( timerHead != NULL ) {
    // Add it to the end of the timer list
    srchTimer = timerHead;
    prevTimer = (void *)NULL;
  
    // Look for open timer slot
    while ( srchTimer ) {
      /* Decrease the correct amount of time */
      if (srchTimer->timeout >= tmr_decr_time)
        srchTimer->timeout = srchTimer->timeout - tmr_decr_time;
        
      /* When timeout, execute the task */
      if ( srchTimer->timeout == 0 ) {
        osal_set_event( srchTimer->task_id, srchTimer->event_flag );
        
        // Take out of list
        if ( prevTimer == NULL )
          timerHead = srchTimer->next;
        else
          prevTimer->next = srchTimer->next;

        // next 
        saveTimer = srchTimer->next;

        // Free memory
        osal_mem_free( srchTimer );

        srchTimer = saveTimer;
      }
      else {
        // Get next
        prevTimer = srchTimer;
        srchTimer = srchTimer->next;
      }
    }
  
    if ( osal_timer_num_active() == 0 ) {
      osal_timer_hw_setup( false );  // Turn timer hardware off
      timerActive = false;
    }
    else {
#ifdef POWER_SAVING
      osal_retune_timers();
#endif
    }
  }
}

#ifdef POWER_SAVING
/*********************************************************************
 * @fn      osal_retune_timers
 *
 * @brief   
 *
 *   Adjust CPU sleep time to the lowest timeout value. If the timeout
 *   value is more then TIMEOUT_THRESHOLD, then the sleep time will be
 *   TIMEOUT_THRESHOLD.
 *
 * @param   none
 *
 * @return  none
 */
void osal_retune_timers( void )
{
  osalTimerRec_t *srchTimer;
  uint16 nextTimeout = TIMEOUT_THRESHOLD;
  
  // Head of the timer list
  srchTimer = timerHead;
          
  // Look for the next timeout timer
  while ( srchTimer )
  {
    if (srchTimer->timeout < nextTimeout)
    {
      nextTimeout = srchTimer->timeout;
    } 

    // Check next timer 
    srchTimer = srchTimer->next;
  }

  // Stop the clock
  osal_timer_activate( FALSE );

  // Alter the rolling time
  tmr_decr_time = nextTimeout;
  tmr_tmod_rollover = nextTimeout * DEFAULT_TMOD_ROLLOVER;

  // Restart the clock
  osal_timer_activate( TRUE );
}
#endif // POWER_SAVING

/*********************************************************************
 * @fn      osal_GetSystemClock()
 *
 * @brief   Read the local system clock.
 *
 * @param   none
 *
 * @return  local clock in milliseconds
 */
uint32 osal_GetSystemClock( void )
{
  return ( osal_systemClock );
} 

extern volatile uint32_t msTicks;                            /* counts 1ms timeTicks */

//liu0920 for some delay function ,only use delay 1us to 10ms ,other use osal_timer func
void osal_delay_nus(unsigned int n)       //N us��ʱ����
{
   unsigned int i;
   for (i=0;i<n;i++)
	   __ASM("nop");
}
  
#if 0  
void osal_delay_nms(unsigned int n)       //N ms��ʱ����
{
   unsigned int i,j;
   for (i=0;i<n;i++)
     for (j=0;j<1140;j++);                //liu0920 ,1140 must adjust for xstal!!!
}
#else
void osal_delay_nms (unsigned int dlyMS)
{
  uint32 curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyMS);
}
#endif


/*********************************************************************
*********************************************************************/
