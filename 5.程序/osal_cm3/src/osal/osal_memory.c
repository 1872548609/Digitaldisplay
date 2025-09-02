/*********************************************************************
    Filename:       OSAL_Memory.c
    Revised:        $Date: 2008/09/20 $
    
    Description:    

       This module contains the OSAL memory allocation functions. 

*********************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSALDef.h"
#include "OnBoard.h"
#include "OSAL.h"
#include "OSAL_Memory.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

// Memory Allocation Heap
#if defined( EXTERNAL_RAM )
  #define MAXMEMHEAP  EXT_RAM_LEN  // 32K
#elif defined( MCU_RAM_END )
  #define MAXMEMHEAP  (MCU_RAM_END-(uint32)((unsigned char *)&_ebss))
#else
  #define MAXMEMHEAP   1536  //  1.5K
#endif

// Memory Allocation Flags
#define OSALMEM_INUSE  0xFF
#define OSALMEM_FREE   0x0F

// Memory Allocation Minimum block size
#define OSALMEM_MINBLOCKSIZE (2 * sizeof( osalMemHdr_t ))

#if defined( OSAL_TOTAL_MEM )
  #define OSAL_HEAP_INIT_VALUE   0xA5
#else
  #define OSAL_HEAP_INIT_VALUE   0x00
#endif

/*********************************************************************
 * TYPEDEFS
 */

// Memory Block Allocation Header (MBAH)
typedef struct
#ifdef __GNUC__
__attribute__ ((packed))
#endif
{
  byte inUse;   // OSALMEM_INUSE or OSALMEM_FREE
  short size;   // Available buffer size for this block
  void *next;   // Pointer to the next MBAH, NULL if last block
  void *prev;   // Pointer to the previous MBAH, NULL if first block
} osalMemHdr_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */

#if defined( OSAL_TOTAL_MEM )
  short osal_mem_high;
  short osal_mem_used;
  short osal_chunk_count;
  unsigned long osal_chunk_inc;
  unsigned long osal_chunk_dec;
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

// Memory Allocation Heap
#if defined( EXTERNAL_RAM )
  byte *osalMemHeap = (byte*)EXT_RAM_BEG;
#elif defined( MCU_RAM_END )
  byte *osalMemHeap; // = (byte*)_ebss;
#else
  byte osalMemHeap[MAXMEMHEAP];  // Internal RAM !!!
#endif

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

/*********************************************************************
 * API FUNCTIONS
 *********************************************************************/
 
/*********************************************************************
 * @fn      osal_mem_init 
 *
 * @brief   
 *
 *   This function initializes the memory allocation system.
 *
 * @param   void
 *
 * @return  void
 */
void osal_mem_init( void )
{
  osalMemHdr_t *hdr;

#if defined( MCU_RAM_END )
  osalMemHeap = (unsigned char *)&_ebss;
#endif

  osal_memset( osalMemHeap, OSAL_HEAP_INIT_VALUE, MAXMEMHEAP );
  
  hdr = (osalMemHdr_t *)osalMemHeap;

  // Initialize the first header
  hdr->inUse = OSALMEM_FREE;
  hdr->size = MAXMEMHEAP - sizeof( osalMemHdr_t );
  hdr->next = (void *)NULL;
  hdr->prev = (void *)NULL;
  
#if defined( OSAL_TOTAL_MEM )
  osal_mem_high = 0;
  osal_mem_used = 0;
  osal_chunk_inc = 0L;
  osal_chunk_dec = 0L;
  osal_chunk_count = 0;
#endif  
}

/*********************************************************************
 * @fn      osal_mem_alloc 
 *
 * @brief   
 *
 *   This function is the allocation function.
 *
 * @param   size - number of bytes wanted
 *
 * @return  void * to the newly allocated memory.  NULL if not allocated.
 */
void *osal_mem_alloc( short int size )
{
  byte *pTmp;  // return value
  short leftOver;
  osalMemHdr_t *hdr;
  osalMemHdr_t *newHdr;
  uint32 tmp;
#if defined( OSAL_TOTAL_MEM )
  short chunkSize;
#endif  
  IntsStorage;

  // Hold off interrupts
  StoreDisableInts;

  // Point to the start of heap
  hdr = (osalMemHdr_t *)osalMemHeap;

  // Find a free space that's big enough to use
  while ( hdr 
    && ((hdr->inUse == OSALMEM_INUSE) || (hdr->size < size)) ) {
    hdr = (osalMemHdr_t *)hdr->next;
  }
  
  if ( hdr != NULL ) {
    // How much is left over in this block? 
    leftOver = hdr->size - size;

    // if big enough, make a new block
    if ( leftOver > OSALMEM_MINBLOCKSIZE ) {
      // Make a new block
      pTmp = (byte*)(((byte*)hdr) + (sizeof( osalMemHdr_t ) + size));
      newHdr = (osalMemHdr_t *)pTmp;
      newHdr->inUse = OSALMEM_FREE;
      newHdr->size = leftOver - sizeof( osalMemHdr_t );

tmp= (void *)(hdr->next);
tmp = newHdr->next ;
      newHdr->next = (void *)(hdr->next);
      newHdr->prev = (void *)hdr;

      // adjust current block
      hdr->next = (void *)newHdr;
      hdr->size = size;

      // adjust the old next block
      if ( newHdr->next )
        ((osalMemHdr_t *)(newHdr->next))->prev = newHdr;
    }

    hdr->inUse = OSALMEM_INUSE;

#if defined( OSAL_TOTAL_MEM )
    osal_chunk_count++;
    osal_chunk_inc++;
    chunkSize = (hdr->size + sizeof( osalMemHdr_t ));
    osal_mem_used += chunkSize;
    if ( osal_mem_used > osal_mem_high ) osal_mem_high = osal_mem_used;
#endif    

    pTmp = (byte*)(((byte*)hdr) + (sizeof( osalMemHdr_t )));
  }
  else {
    pTmp = (byte *)NULL; // no memory
  }

  // Re-enable interrupts
  RestoreInts;
  
  return (void *)pTmp;
}

/*********************************************************************
 * @fn      osal_mem_free
 *
 * @brief   
 *
 *   This function frees allocated memory.
 *
 * @param   ptr - pointer to the memory to free
 *
 * @return  true if freed, false if not found
 */
byte osal_mem_free( void *ptr )
{
  osalMemHdr_t *hdr;
  osalMemHdr_t *npHdr;
  byte *pTmp;
#if defined( OSAL_TOTAL_MEM )
  short chunkSize;
#endif  
  IntsStorage;

  // Hold off interrupts
  StoreDisableInts;

  // Point to the start of heap
  hdr = (osalMemHdr_t *)osalMemHeap;

  // Find the block
  while ( hdr ) {
    // Is this the block?
    pTmp = (byte *)((byte *)hdr + (sizeof( osalMemHdr_t )));
    if ( (void *)pTmp == ptr ) {
#if defined( OSAL_TOTAL_MEM )
      if ( hdr->inUse == OSALMEM_INUSE ) {
        chunkSize = (hdr->size + sizeof( osalMemHdr_t ));
        osal_mem_used -= chunkSize;
        osal_chunk_count--;
        osal_chunk_dec++;
      }
#endif      

      hdr->inUse = OSALMEM_FREE;  // mark this block as free

      // Is the next block free?
      npHdr = (osalMemHdr_t *)(hdr->next);
      if ( npHdr && (npHdr->inUse == OSALMEM_FREE) ) {
        // Collapse next block into this block
        hdr->size = hdr->size + sizeof( osalMemHdr_t ) + npHdr->size;
        hdr->next = npHdr->next;

        // Adjust the previous pointer of the next block
        if ( npHdr->next ) {
          npHdr = npHdr->next;
          npHdr->prev = hdr;
        }
      }

      // Is the previous block free?
      npHdr = (osalMemHdr_t *)(hdr->prev);
      if ( npHdr && (npHdr->inUse == OSALMEM_FREE) ) {
        // Collapse this block into the previous block
        npHdr->size = npHdr->size + sizeof( osalMemHdr_t ) + hdr->size;
        npHdr->next = hdr->next;

        // Adjust the previous pointer of the next block
        if ( hdr->next ) {
          hdr = hdr->next;
          hdr->prev = npHdr;
        }
      }

      // Re-enable interrupts
      RestoreInts;

      // We are done
      return true;
    }
    else
      hdr = hdr->next;
  }

  // Re-enable interrupts
  RestoreInts;

  return false;
}

/*********************************************************************
 * @fn      osal_mem_valid
 *
 * @brief   
 *
 *   This function checks for address within valid range.
 *
 * @param   addr - address of memory location
 *
 * @return  true if valid RAM, false if invalid RAM
 */
byte osal_mem_valid( UINT16 addr )
{
  // Check for valid internal RAM address
  if ( ( addr < MCU_RAM_BEG ) || ( addr > MCU_RAM_END ) ) {
    return ( FALSE );
  }
  else {
#if defined( EXTERNAL_RAM )
    // Check for valid external RAM address
    if ( ( addr < EXT_RAM_BEG ) || ( addr > EXT_RAM_END ) )
    {
       return ( FALSE );
    }
#endif    
    return ( TRUE );
  }
}

#if defined( OSAL_TOTAL_MEM )
/*********************************************************************
 * @fn      osal_mem_unused
 *
 * @brief   
 *
 *   Runs through the memory allocation tables and adds up the 
 *   unused memory.
 *
 * @param   none
 *
 * @return  number of bytes free (usable)
 */
short osal_mem_unused( void )
{
  osalMemHdr_t *hdr;
  short size = 0;

  // Point to the start of heap
  hdr = (osalMemHdr_t *)osalMemHeap;

  // Loop through memory records
  while ( hdr ) {
    if ( hdr->inUse == OSALMEM_FREE )
      size += hdr->size;
    hdr = (osalMemHdr_t *)hdr->next;
  }
  return ( size );
}
#endif

#if defined( OSAL_TOTAL_MEM )
/*********************************************************************
 * @fn      osal_stack_used()
 *
 * @brief   
 *
 *   Runs through the stack looking for touched memory.
 *
 * @param   none
 *
 * @return  number of bytes used by the stack
 */
uint16 osal_stack_used( void )
{
  return OnBoard_stack_used();
}
#endif

#if defined( OSAL_TOTAL_MEM )
/*********************************************************************
 * @fn      osal_heap_high_water()
 *
 * @brief   
 *
 *   Calculate the high water mark in the heap
 *
 * @param   none
 *
 * @return  returns the total (high water) heap used
 */
uint16 osal_heap_high_water( void )
{
  byte *pHeap = &osalMemHeap[MAXMEMHEAP-1];
  
  while ( (*pHeap == OSAL_HEAP_INIT_VALUE) && (pHeap > osalMemHeap) )
  {
    pHeap--;
  }
  
  return ( (uint16)(pHeap - osalMemHeap) );
}
#endif

/*********************************************************************
*********************************************************************/
