/*********************************************************************
    Filename:       OSAL_Tasks.c
    Revised:        $Date: 2008/09/20  $
    Revision:       $Revision: 0.10 $
    
    Description:    
    
       This file contains the OSAL Task definition and manipulation
       functions.
            
*********************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include "OSALDef.h"
#include "OSAL.h"
#include "OSAL_Tasks.h"
#include "OSAL_Custom.h"


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

// Task Control
osalTaskRec_t *tasksHead;

osalTaskRec_t *activeTask;

byte taskIDs;

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
 * FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osalTaskInit
 *
 * @brief   Initialization for the Tasking System.
 *
 * @param   none
 *
 * @return  
 */
void osalTaskInit( void )
{
  tasksHead = (osalTaskRec_t *)NULL;
  activeTask = (osalTaskRec_t *)NULL;
  taskIDs = 0;
}

/*********************************************************************
 * @fn      osalTaskAdd
 *
 * @brief   Add a task to the task list
 *
 * @param   none
 *
 * @return  
 */
void osalTaskAdd( const pTaskInitFn pfnInit, 
                  const pTaskEventHandlerFn pfnEventProcessor )
{
  osalTaskRec_t *newTask;
  osalTaskRec_t *srchTask;

  newTask = osal_mem_alloc( sizeof( osalTaskRec_t ) );
  if ( newTask ) {
    // Fill in new task
    newTask->pfnInit = pfnInit;
    newTask->pfnEventProcessor = pfnEventProcessor;
    newTask->taskID = taskIDs++;
    newTask->events = 0;
    newTask->next = (osalTaskRec_t *)NULL;

    // Does the task list already exist
    if ( tasksHead == NULL ) {
      // Start task list
      tasksHead = newTask;
    }
    else {
      // Add it to the end of the task list
      srchTask = tasksHead;

      // Stop at the end
      while ( srchTask->next )
        srchTask = srchTask->next;

      // Add
      srchTask->next = newTask;
    }
  }
}
 
/*********************************************************************
 * @fn      osalInitTasks
 *
 * @brief   Call each of the tasks initailization functions.
 *
 * @param   none
 *
 * @return  none
 */
void osalInitTasks( void )
{
  // Start at the beginning
  activeTask = tasksHead;

  // Stop at the end
  while ( activeTask ) {
    if (  activeTask->pfnInit  )
      activeTask->pfnInit( activeTask->taskID );    

    activeTask = activeTask->next;
  }

  activeTask = (osalTaskRec_t *)NULL;
}

/*********************************************************************
 * @fn      osalNextActiveTask
 *
 * @brief   This function will return the next active task.
 * 
 * NOTE:    We could add priority here later. For now it just starts
 *          at the beginning and stops when activity is found.
 *
 * @param   none
 *
 * @return  pointer to the found task, NULL if not found
 */
osalTaskRec_t *osalNextActiveTask( void )
{
  osalTaskRec_t *srchTask;

  // Start at the beginning
  srchTask = tasksHead;

  // When found or not
  while ( srchTask ) {
    // Look for any activity
    if (  srchTask->events  )
      return ( srchTask );    

    srchTask = srchTask->next;
  }

  return ( (osalTaskRec_t *)NULL );
}

/*********************************************************************
 * @fn      osalFindTask
 *
 * @brief   This function will return a task pointer to the task
 *          found with the passed in task ID.
 * 
 * @param   taskID - task ID to look for
 *
 * @return  pointer to the found task, NULL if not found
 */
osalTaskRec_t *osalFindTask( byte taskID )
{
  osalTaskRec_t *srchTask;

  // Start at the beginning
  srchTask = tasksHead;

  // When found or not
  while ( srchTask ) {
    // Look for any activity
    if (  srchTask->taskID == taskID  )
      return ( srchTask );    

    srchTask = srchTask->next;
  }

  return ( (osalTaskRec_t *)NULL );
}

/*********************************************************************
*********************************************************************/
