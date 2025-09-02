#ifndef OSAL_CUSTOM_H
#define OSAL_CUSTOM_H

/*********************************************************************
    Filename:       $RCSfile: OSAL_Custom.h,v $
    Revised:        $Date: 2008/09/20 $
    Revision:       $Revision: 0.10 $
    
    Description:    
    
    This file contains all the settings and other functions that
    the user should set and change.

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
 * This function adds all the tasks to the task list.
 *  This is where to add new tasks.
 */
extern void osalAddTasks( void );


#ifdef __cplusplus
}
#endif

#endif /* OSAL_CUSTOM_H */
