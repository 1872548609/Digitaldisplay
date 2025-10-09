/**************************************************************************************************
  Filename:       comdef.h
  Revised:        $Date: 2010-07-28 08:42:48 -0700 (Wed, 28 Jul 2010) $
  Revision:       $Revision: 23160 $

  Description:    Type definitions and macros.


  Copyright 2004-2008 Texas Instruments Incorporated. All rights reserved.
		
		ÐÞ¸Ä
**************************************************************************************************/

#ifndef COMDEF_H
#define COMDEF_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * INCLUDES
 */

/* HAL */
#include "hal_types.h"
#include "hal_defs.h"

/*********************************************************************
 * Lint Keywords      ¹Ø¼ü´Ê±í
 */
#define VOID (void)

#define NULL_OK
#define INP
#define OUTP
#define UNUSED
#define ONLY
#define READONLY
#define SHARED
#define KEEP
#define RELAX

/*********************************************************************
 * CONSTANTS            ³£Á¿
 */

#ifndef false
  #define false 0
#endif

#ifndef true
  #define true 1
#endif

#ifndef CONST
  #define CONST const
#endif

#ifndef GENERIC
  #define GENERIC
#endif

/*** Generic Status Return Values Í¨ÓÃ×´Ì¬·µ»ØÖµ***/
#define SUCCESS                   0x00
#define FAILURE                   0x01
#define INVALIDPARAMETER          0x02
#define INVALID_TASK              0x03
#define MSG_BUFFER_NOT_AVAIL      0x04
#define INVALID_MSG_POINTER       0x05
#define INVALID_EVENT_ID          0x06
#define INVALID_INTERRUPT_ID      0x07
#define NO_TIMER_AVAIL            0x08
#define NV_ITEM_UNINIT            0x09
#define NV_OPER_FAILED            0x0A
#define INVALID_MEM_SIZE          0x0B
#define NV_BAD_ITEM_LEN           0x0C
#define NV_INVALID_DATA           0x0D

/*** NV Error Mask ***/
#define NV_NIB_INIT_FAILURE       0x01
#define NV_ADDR_MGR_INIT_FAILURE  0x02 
#define NV_ASSOC_INIT_FAILURE     0x04  
#define NV_BIND_TBL_INIT_FAILURE  0x08
#define NV_GRPS_INIT_FAILURE      0x10
#define NV_SEC_MGR_FAILURE        0x20
  
/*********************************************************************
 * TYPEDEFS    ÀàÐÍ¶¨Òå
 */

// Generic Status return
typedef uint8 Status_t;

// Data types
typedef int32   int24;
typedef uint32  uint24;

/*********************************************************************
 * Global System Events       ÏµÍ³È«¾ÖÊÂ¼þ
 */

#define SYS_EVENT_MSG               0x8000  // A message is waiting event

/*********************************************************************
 * Global Generic System Messages        È«¾ÖÍ¨ÓÃÏµÍ³ÏûÏ¢
 */

#define KEY_CHANGE                0xC0    // Key Events

// OSAL System Message IDs/Events Reserved for applications (user applications)
// 0xE0 – 0xFC

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* COMDEF_H */
