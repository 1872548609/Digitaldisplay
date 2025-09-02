#ifndef OSAL_PWRMGR_H
#define OSAL_PWRMGR_H

/*********************************************************************
    Filename:       OSAL_PwrMgr.h
    Revised:        $Date: 2008/09/20 $
    Revision:       $Revision: 0.10 $
    
    Description:    
    
       This file contains the OSAL Power Management API.
            
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
typedef enum
{
  PWRMGR_MAINS,
  PWRMGR_BATTERY_DEVICE
} pwrmgr_mode_t;

typedef enum
{
  PWRMGR_CONSERVE,
  PWRMGR_HOLD
} pwrmgr_state_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */
 
/*********************************************************************
 * FUNCTIONS
 */
 
  /*
   * Initialize the power management system.
   *   This function is called from OSAL.
   *
   */
  extern void osal_pwrmgr_init( void );

  /*
   * Sets the power saving mode for the device.
   */
  extern void osal_pwrmgr_mode( pwrmgr_mode_t mode );

  /*
   * Sets the power saving mode for the device.
   *
   */
  extern byte osal_pwrmgr_task_state( byte task_id, pwrmgr_state_t state );

  /*
   * This function is called from the main OSAL loop and
   *          shouldn't be call from anywhere else.
   */
  extern void osal_pwrmgr_powerconserve( void );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_PWRMGR_H */
