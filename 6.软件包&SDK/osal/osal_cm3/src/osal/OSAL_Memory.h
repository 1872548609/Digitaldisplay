#ifndef OSAL_MEMORY_H
#define OSAL_MEMORY_H

/*********************************************************************
    Filename:       OSAL_Memory.h
    Revised:        $Date: 2008/09/20 $
    Revision:       $Revision: 1.10 $
    
    Description:    
    
       This module defines the OSAL memory control functions. 
            
*********************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "OSALDef.h"
 
/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
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
   * Initialize memory manager
   */
  extern void osal_mem_init( void );

  /*
   * Allocate a block of memory
   */
  extern void *osal_mem_alloc( short size );

  /*
   * Free a block of memory
   */
  extern byte osal_mem_free( void *ptr );

  /*
   * Validate a memory address
   */
  extern byte osal_mem_valid( uint16 addr );
  
#if defined( OSAL_TOTAL_MEM )
  /*
   * Calculate unused allocation space
   */
  extern short osal_mem_unused( void );
  
  /*
   * Calculate the number of total bytes used in the stack (High Water Mark)
   */
  extern uint16 osal_stack_used( void );
  
  /*
   * Calculate the high water mark in the heap.
   */
  extern uint16 osal_heap_high_water( void );
#endif   

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_MEMORY.H */
