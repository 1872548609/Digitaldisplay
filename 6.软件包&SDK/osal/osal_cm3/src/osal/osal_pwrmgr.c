/*********************************************************************
    Filename:       $RCSfile: OSAL_PwrMgr.c,v $
    Revised:        $Date: 2008/09/20 01:36:22 $
    Revision:       $Revision: 0.10 $
    
    Description:    
    
      This file contains the OSAL Power Management API.

            
*********************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include "OSALDef.h"
#include "OnBoard.h"
#include "OSAL.h"
#include "OSAL_Tasks.h"
#include "OSAL_Timers.h"
#include "OSAL_PwrMgr.h"

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
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

 /*********************************************************************
 * LOCAL VARIABLES
 */
pwrmgr_mode_t pwrmgr_mode;

uint32 pwrmgr_task_state;       // bit masked field, each bit
                                // represents each task. (32 max)
                                
/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osal_pwrmgr_init
 *
 * @brief   Initialize the power management system.
 *
 * @param   none.
 *
 * @return  none.
 */
void osal_pwrmgr_init( void )
{
  pwrmgr_mode = PWRMGR_MAINS;   // Default to no power conservation.
  pwrmgr_task_state = 0;        // Cleared.  All set to conserve
}

/*********************************************************************
 * @fn      osal_pwrmgr_mode
 *
 * @brief   Sets the power saving mode for the device.
 *
 * @param   mode - type of power savings to perform.
 *
 * @return  none
 */
void osal_pwrmgr_mode( pwrmgr_mode_t mode )
{
  if ( mode == PWRMGR_BATTERY_DEVICE )
    pwrmgr_mode = PWRMGR_BATTERY_DEVICE;
  else
    pwrmgr_mode = PWRMGR_MAINS;   // Default to mains
}

/*********************************************************************
 * @fn      osal_pwrmgr_task_state
 *
 * @brief   Sets the power saving mode for the task.
 *
 * @param   mode - type of power savings to perform.
 *
 * @return  none
 */
byte osal_pwrmgr_task_state( byte task_id, pwrmgr_state_t state )
{
  if ( osalFindTask( task_id ) == NULL )
    return ( INVALID_TASK );
    
  if ( state == PWRMGR_CONSERVE ) {
    // Clear the task state flag
    pwrmgr_task_state &= ~(1 << task_id );
  }
  else {
    // Set the task state flag
    pwrmgr_task_state |= (1 << task_id);
  }
  
  return ( ZSUCCESS );
}

/*********************************************************************
 * @fn      osal_pwrmgr_powerconserve
 *
 * @brief   This function is called from the main OSAL loop and
 *          shouldn't be call from anywhere else.
 *
 * @param   none.
 *
 * @return  none.
 */
void osal_pwrmgr_powerconserve( void )
{ 
  // look to see if we should even look into power conservation
  if ( pwrmgr_mode == PWRMGR_BATTERY_DEVICE ) {
    // Are all tasks in agreement to conserve
    if ( pwrmgr_task_state == 0 ) {
      // Go to sleep
      
      // Turn the radio off while sleeping
      
      // Setup timers for sleep
      osal_sleep_timers();

      // Put the processor into sleep mode
      OSAL_SET_CPU_INTO_SLEEP;
      
      //*** Must have been awaken by interrupt
      
      // Out of sleep now
      osal_unsleep_timers();
    }
  }
}

/*********************************************************************
*********************************************************************/
