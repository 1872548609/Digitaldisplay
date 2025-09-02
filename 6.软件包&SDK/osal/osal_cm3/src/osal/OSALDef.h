#ifndef OSALDEF_H
#define OSALDEF_H

/*********************************************************************
    Filename:       ZComDef.h
    Revised:        $Date: 2008/09/20 $
    Revision:       $Revision: 0.10 $
    
    Description:    Type definitions and macros.
    
*********************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
 
/*********************************************************************
 * CONSTANTS
 */
 
#ifndef NULL
  #define NULL (0)
#endif

#ifndef false
	#define false 0
#endif

#ifndef true
	#define true 1
#endif

#ifndef TRUE
  #define TRUE 1
#endif

#ifndef FALSE 
  #define FALSE 0
#endif

/*** Return Values ***/
#define ZSUCCESS                  0
#define INVALID_TASK              1
#define MSG_BUFFER_NOT_AVAIL      2
#define INVALID_MSG_POINTER       3
#define INVALID_LEN               4
#define INVALID_SENDING_TASK      5
#define INVALID_DESTINATION_TASK  6
#define INVALID_EVENT_ID          7
#define INVALID_TIMEOUT_VALUE     8
#define INVALID_INTERRUPT_ID      9
#define INVALID_ISR_PTR           10
#define INVALID_TASK_TABLE        11
#define NO_TIMER_AVAIL            12
#define INVALID_MSG_LEN           13
#define NOT_READY                 14
#define NV_ITEM_UNINIT            15
#define NV_OPER_FAILED            16
#define INVALID_MEM_SIZE          17

/*** Component IDs ***/
#define COMPID_OSAL               0
#define COMPID_MTEL               1
#define COMPID_MTSPCI             2
#define COMPID_NWK                3
#define COMPID_NWKIF              4
#define COMPID_MACCB              5
#define COMPID_MAC                6
#define COMPID_APP                7
#define COMPID_TEST								8

#define COMPID_RTG								9
#define COMPID_DATA								11


/* Temp CompIDs for testing */
#define COMPID_TEST_NWK_STARTUP           20
#define COMPID_TEST_SCAN_CONFIRM          21
#define COMPID_TEST_ASSOC_CONFIRM         22
#define COMPID_TEST_REMOTE_DATA_CONFIRM   23

/*********************************************************************
 * TYPEDEFS
 */
 
/*** Data Types ***/
typedef unsigned char     byte;
typedef unsigned char     uint8;
typedef char              int8;
typedef unsigned short int   UINT16;
typedef unsigned short int   uint16;
typedef short int	  int16;
typedef short int	  INT16;
typedef unsigned long     uint32; //windows has typedef'd unsigned int as UINT32 
typedef long              int32;

#define Z_EXTADDR_LEN     8
enum
{
  AddrNotPresent = 0,
  Addr16Bit = 2,
  Addr64Bit = 3,
  AddrBroadcast = 15
};

typedef  struct
#ifdef __GNUC__
__attribute__ ((packed))
#endif
{
  byte task_id;
  UINT16 event_flag;
  UINT16 timeout;
  void *next;
} osalTimerRec_t; 


#if 0  //liu0920
typedef byte	ZLongAddr_t[8];
typedef struct
{
  union 
  {
    uint16      shortAddr;
    ZLongAddr_t extAddr;
  } addr;
  byte addrMode;
} zAddrType_t;  
#endif 


/*********************************************************************
 * MACROS
 */

// Takes a byte out of a uint32.  
//     var - uint32
//     ByteNum - byte to take out (0 - 3)
#define BREAK_UINT32( var, ByteNum ) \
        (byte)((uint32)((var >>(ByteNum * 8)) & 0x00FF))

#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
     ((uint32)((uint32)(Byte0 & 0x00FF) \
      + ((uint32)(Byte1 & 0x00FF) << 8) \
      + ((uint32)(Byte2 & 0x00FF) << 16) \
      + ((uint32)(Byte3 & 0x00FF) << 24)))

#define BUILD_UINT16(loByte, hiByte) \
        ((UINT16)((loByte & 0x00FF) + ((hiByte & 0x00FF) << 8)))

#define HI_UINT16(a) ((a >> 8) & 0xFF)
#define LO_UINT16(a) (a & 0xFF)

#define BUILD_UINT8(hiByte, loByte) \
        ((uint8)((loByte & 0x0F) + ((hiByte & 0x0F) << 4)))

#define HI_UINT8(a) ((a >> 4) & 0x0F)
#define LO_UINT8(a) (a & 0x0F)

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

#endif /* OSALDEF_H */
