/*********************************************************************
 * INCLUDES
 */

#include "ZComDef.h"
//#include "ZGlobals.h"
#include "OnBoard.h"
#include "OSAL.h"
//#include "MT.h"
//#include "MT_SYS.h"
//#include "DebugTrace.h"

/* Hal */
//#include "hal_lcd.h"
//#include "hal_mcu.h"
//#include "hal_timer.h"
//#include "hal_key.h"
//#include "hal_led.h"

/* Allow access macRandomByte() */
//#include "mac_radio_defs.h"

#include "main.h"

/*********************************************************************
 * CONSTANTS
 */

// Task ID not initialized
#define NO_TASK_ID 0xFF

// Minimum length RAM "pattern" for Stack check
#define MIN_RAM_INIT 12

/*********************************************************************
 * GLOBAL VARIABLES
 */

//#if defined MAKE_CRC_SHDW
//#pragma location="CRC_SHDW"
//const CODE uint16 _crcShdw = 0xFFFF;
//#pragma required=_crcShdw

//#else  // if !defined MAKE_CRC_SHDW

//#pragma location="LOCK_BITS_ADDRESS_SPACE"
//__no_init uint8 _lockBits[16];
//#pragma required=_lockBits

//#if defined ZCL_KEY_ESTABLISH
//#include "zcl_cert_data.c"
//#else
//#pragma location="IEEE_ADDRESS_SPACE"
//__no_init uint8 _nvIEEE[Z_EXTADDR_LEN];
//#pragma required=_nvIEEE
//#endif

//#pragma location="RESERVED_ADDRESS_SPACE"
//__no_init uint8 _reserved[1932];
//#pragma required=_reserved
//#endif

// 64-bit Extended Address of this device
uint8 aExtendedAddress[8];

/*********************************************************************
 * LOCAL VARIABLES
 */

// Registered keys task ID, initialized to NOT USED.
//static uint8 registeredKeysTaskID = NO_TASK_ID;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void ChkReset( void );

/*********************************************************************
 * @fn      InitBoard()
 * @brief   Initialize the CC2420DB Board Peripherals
 * @param   level: COLD,WARM,READY
 * @return  None
 */
void InitBoard( uint8 level )
{
  if ( level == OB_COLD )
  {
    // IAR does not zero-out this byte below the XSTACK.
   // *(uint8 *)0x0 = 0;
    // Interrupts off
    osal_int_disable( INTS_ALL );
    // Check for Brown-Out reset
    ChkReset();
  }
  else  // !OB_COLD
  {
    /* Initialize Key stuff */
    
  }
}

/*********************************************************************
 * @fn      ChkReset()
 * @brief   Check reset bits - if reset cause is unknown, assume a
 *          brown-out (low power), assume batteries are not reliable,
 *          hang in a loop and sequence through the LEDs.
 * @param   None
 * @return  None
 *********************************************************************/
void ChkReset(void)
{
    uint8 rib;  // 定义临时变量，用于存储复位标志位
 
    // 从SLEEPSTA寄存器中提取复位指示位（通过LRESET掩码）
    // SLEEPSTA：睡眠/复位状态寄存器
    // LRESET：复位标志位的掩码（可能包含多个复位类型的标志位）
    //rib = SLEEPSTA & LRESET;
 
    // 判断是否为Power-On复位（上电复位）
    if (rib == RESETPO)
    {
        // 此处应添加Power-On复位处理逻辑
        // 例如：初始化硬件外设、加载默认配置等
        // RESETPO：表示上电复位的标志位值
    }
    // 判断是否为External复位（外部复位）
    else if (rib == RESETEX)
    {
        // 此处应添加外部复位处理逻辑
        // 例如：检查复位引脚状态、处理外部设备触发的复位等
        // RESETEX：表示外部复位的标志位值
    }
    // 判断是否为Watchdog复位（看门狗复位）
    else if (rib == RESETWD)
    {
        // 此处应添加看门狗复位处理逻辑
        // 例如：记录看门狗超时原因、恢复关键外设状态等
        // RESETWD：表示看门狗复位的标志位值
    }
    // 处理未知复位原因（理论上不应进入此分支）
    else
    {
        // 触发断言失败，表示出现未预期的复位原因
        // HAL_ASSERT(0)：通常在调试阶段使程序中断，便于定位问题
        //HAL_ASSERT(0);
    }
}

/*********************************************************************
 *                        "Keyboard" Support
 *********************************************************************/

/*********************************************************************
 * Keyboard Register function
 *
 * The keyboard handler is setup to send all keyboard changes to
 * one task (if a task is registered).
 *
 * If a task registers, it will get all the keys. You can change this
 * to register for individual keys.
 *********************************************************************/
//uint8 RegisterForKeys( uint8 task_id )
//{
//  // Allow only the first task
//  if ( registeredKeysTaskID == NO_TASK_ID )
//  {
//    registeredKeysTaskID = task_id;
//    return ( true );
//  }
//  else
//    return ( false );
//}

/*********************************************************************
 * @fn      OnBoard_SendKeys
 *
 * @brief   Send "Key Pressed" message to application.
 *
 * @param   keys  - keys that were pressed
 *          state - shifted
 *
 * @return  status
 *********************************************************************/
//uint8 OnBoard_SendKeys( uint8 keys, uint8 state )
//{
//  keyChange_t *msgPtr;

//  if ( registeredKeysTaskID != NO_TASK_ID )
//  {
//    // Send the address to the task
//    msgPtr = (keyChange_t *)osal_msg_allocate( sizeof(keyChange_t) );
//    if ( msgPtr )
//    {
//      msgPtr->hdr.event = KEY_CHANGE;
//      msgPtr->state = state;
//      msgPtr->keys = keys;

//      osal_msg_send( registeredKeysTaskID, (uint8 *)msgPtr );
//    }
//    return ( ZSuccess );
//  }
//  else
//    return ( ZFailure );
//}

/*********************************************************************
 * @fn      OnBoard_KeyCallback
 *
 * @brief   Callback service for keys
 *
 * @param   keys  - keys that were pressed
 *          state - shifted
 *
 * @return  void
 *********************************************************************/
//void OnBoard_KeyCallback ( uint8 keys, uint8 state )
//{
//  uint8 shift;
//  (void)state;

//  shift = (keys & HAL_KEY_SW_6) ? true : false;

//  if ( OnBoard_SendKeys( keys, shift ) != ZSuccess )
//  {
//    // Process SW1 here
//    if ( keys & HAL_KEY_SW_1 )  // Switch 1
//    {
//    }
//    // Process SW2 here
//    if ( keys & HAL_KEY_SW_2 )  // Switch 2
//    {
//    }
//    // Process SW3 here
//    if ( keys & HAL_KEY_SW_3 )  // Switch 3
//    {
//    }
//    // Process SW4 here
//    if ( keys & HAL_KEY_SW_4 )  // Switch 4
//    {
//    }
//    // Process SW5 here
//    if ( keys & HAL_KEY_SW_5 )  // Switch 5
//    {
//    }
//    // Process SW6 here
//    if ( keys & HAL_KEY_SW_6 )  // Switch 6
//    {
//    }
//  }
//}

/*********************************************************************
 * @fn      OnBoard_stack_used
 *
 * @brief   Runs through the stack looking for touched memory.
 *
 * @param   none
 *
 * @return  Maximum number of bytes used by the stack.
 *********************************************************************/
//uint16 OnBoard_stack_used(void)
//{
//  uint8 const *ptr;
//  uint8 cnt = 0;

//  for (ptr = CSTACK_END; ptr > CSTACK_BEG; ptr--)
//  {
//    if (STACK_INIT_VALUE == *ptr)
//    {
//      if (++cnt >= MIN_RAM_INIT)
//      {
//        ptr += MIN_RAM_INIT;
//        break;
//      }
//    }
//    else
//    {
//      cnt = 0;
//    }
//  }

//  return (uint16)(CSTACK_END - ptr + 1);
//}

/*********************************************************************
 * @fn      _itoa
 *
 * @brief   convert a 16bit number to ASCII
 *
 * @param   num -
 *          buf -
 *          radix -
 *
 * @return  void
 *
 *********************************************************************/
void _itoa(uint16 num, uint8 *buf, uint8 radix)
{
  char c,i;
  uint8 *p, rst[5];

  p = rst;
  for ( i=0; i<5; i++,p++ )
  {
    c = num % radix;  // Isolate a digit
    *p = c + (( c < 10 ) ? '0' : '7');  // Convert to Ascii
    num /= radix;
    if ( !num )
      break;
  }

  for ( c=0 ; c<=i; c++ )
    *buf++ = *p--;  // Reverse character order

  *buf = '\0';
}

/*********************************************************************
 * @fn        Onboard_rand
 *
 * @brief    Random number generator
 *
 * @param   none
 *
 * @return  uint16 - new random number
 *
 *********************************************************************/
uint16 Onboard_rand( void )
{
  return ( rand() );
}

/*********************************************************************
 * @fn        Onboard_wait
 *
 * @brief    Delay wait
 *
 * @param   uint16 - time to wait
 *
 * @return  none
 *
 *********************************************************************/
//void Onboard_wait( uint16 timeout )
//{
//  while (timeout--)
//  {
//    asm("NOP");
//    asm("NOP");
//    asm("NOP");
//  }
//}

/*********************************************************************
 * @fn      Onboard_soft_reset
 *
 * @brief   Effect a soft reset.
 *
 * @param   none
 *
 * @return  none
 *
 *********************************************************************/
//__near_func void Onboard_soft_reset( void )
//{
//  HAL_DISABLE_INTERRUPTS();
//  // Abort all DMA channels to insure that ongoing operations do not
//  // interfere with re-configuration.
//  DMAARM = 0x80 | 0x1F;
//  asm("LJMP 0x0");
//}

/*********************************************************************
 *                    EXTERNAL I/O FUNCTIONS
 *
 * User defined functions to control external devices. Add your code
 * to the following functions to control devices wired to DB outputs.
 *
 *********************************************************************/

//void BigLight_On( void )
//{
//  // Put code here to turn on an external light
//}

//void BigLight_Off( void )
//{
//  // Put code here to turn off an external light
//}

//void BuzzerControl( uint8 on )
//{
//  // Put code here to turn a buzzer on/off
//  (void)on;
//}

//void Dimmer( uint8 lvl )
//{
//  // Put code here to control a dimmer
//  (void)lvl;
//}

//// No dip switches on this board
//uint8 GetUserDipSw( void )
//{
//  return 0;
//}



/*********************************************************************
*********************************************************************/
