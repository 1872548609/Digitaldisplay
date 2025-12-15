/**************************************************************************************************
  Filename:       hal_key.c
  Revised:        $Date: 2010-09-15 19:02:45 -0700 (Wed, 15 Sep 2010) $
  Revision:       $Revision: 23815 $

  Description:    This file contains the interface to the HAL KEY Service.


  Copyright 2006-2010 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/
/*********************************************************************
 NOTE: If polling is used, the hal_driver task schedules the KeyRead()
       to occur every 100ms.  This should be long enough to naturally
       debounce the keys.  The KeyRead() function remembers the key
       state of the previous poll and will only return a non-zero
       value if the key state changes.

 NOTE: If interrupts are used, the KeyRead() function is scheduled
       25ms after the interrupt occurs by the ISR.  This delay is used
       for key debouncing.  The ISR disables any further Key interrupt
       until KeyRead() is executed.  KeyRead() will re-enable Key
       interrupts after executing.  Unlike polling, when interrupts
       are enabled, the previous key state is not remembered.  This
       means that KeyRead() will return the current state of the keys
       (not a change in state of the keys).

 NOTE: If interrupts are used, the KeyRead() fucntion is scheduled by
       the ISR.  Therefore, the joystick movements will only be detected
       during a pushbutton interrupt caused by S1 or the center joystick
       pushbutton.

 NOTE: When a switch like S1 is pushed, the S1 signal goes from a normally
       high state to a low state.  This transition is typically clean.  The
       duration of the low state is around 200ms.  When the signal returns
       to the high state, there is a high likelihood of signal bounce, which
       causes a unwanted interrupts.  Normally, we would set the interrupt
       edge to falling edge to generate an interrupt when S1 is pushed, but
       because of the signal bounce, it is better to set the edge to rising
       edge to generate an interrupt when S1 is released.  The debounce logic
       can then filter out the signal bounce.  The result is that we typically
       get only 1 interrupt per button push.  This mechanism is not totally
       foolproof because occasionally, signal bound occurs during the falling
       edge as well.  A similar mechanism is used to handle the joystick
       pushbutton on the DB.  For the EB, we do not have independent control
       of the interrupt edge for the S1 and center joystick pushbutton.  As
       a result, only one or the other pushbuttons work reasonably well with
       interrupts.  The default is the make the S1 switch on the EB work more
       reliably.

*********************************************************************/

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
// °üº¬±ØÒªµÄÍ·ÎÄ¼þ
#include "hal_mcu.h"       // Î¢¿ØÖÆÆ÷Ó²¼þ³éÏó²ã
#include "hal_defs.h"      // Ó²¼þ³éÏó²ã¶¨Òå
#include "hal_types.h"     // Ó²¼þ³éÏó²ãÊý¾ÝÀàÐÍ
#include "hal_board.h"     // °å¼¶Ö§³Ö°ü
#include "hal_drivers.h"   // Ó²¼þÇý¶¯
#include "hal_adc.h"       // ADCÇý¶¯
#include "hal_key.h"       // ¼üÅÌÇý¶¯
#include "osal.h"          // ²Ù×÷ÏµÍ³³éÏó²ã

// ½öÔÚHAL_KEYÊ¹ÄÜÊ±±àÒëÒÔÏÂ´úÂë£¬Õâ¾äºê¾ö¶¨°´¼üÊÇ·ñ´ò¿ª
#if (defined HAL_KEY) && (HAL_KEY == TRUE)
/**************************************************************************************************
 *                                              MACROS
 **************************************************************************************************/

/**************************************************************************************************
 *                                            CONSTANTS
 **************************************************************************************************/
// ¶¨Òå°´¼ü±ßÑØ´¥·¢ÀàÐÍ
#define HAL_KEY_RISING_EDGE   0    // ÉÏÉýÑØ´¥·¢
#define HAL_KEY_FALLING_EDGE  1    // ÏÂ½µÑØ´¥·¢
 
#define HAL_KEY_DEBOUNCE_VALUE  25 // °´¼üÏû¶¶ÑÓ³ÙÖµ(ms)
 
/* CPU¶Ë¿ÚÖÐ¶Ï¶¨Òå */
#define HAL_KEY_CPU_PORT_0_IF P0IF // Port0ÖÐ¶Ï±êÖ¾
#define HAL_KEY_CPU_PORT_2_IF P2IF // Port2ÖÐ¶Ï±êÖ¾
 
/* SW_6°´¼ü¶¨Òå (Î»ÓÚP0.1Òý½Å) */
#define HAL_KEY_SW_6_PORT   P0     // ¶Ë¿Ú¼Ä´æÆ÷
#define HAL_KEY_SW_6_BIT    BV(1)  // Î»ÑÚÂë(µÚ1Î»)
#define HAL_KEY_SW_6_SEL    P0SEL  // ¹¦ÄÜÑ¡Ôñ¼Ä´æÆ÷
#define HAL_KEY_SW_6_DIR    P0DIR  // ·½Ïò¼Ä´æÆ÷
 
/* ±ßÑØÖÐ¶ÏÉèÖÃ */
#define HAL_KEY_SW_6_EDGEBIT  BV(0) // ±ßÑØ¿ØÖÆÎ»
#define HAL_KEY_SW_6_EDGE     HAL_KEY_FALLING_EDGE // Ê¹ÓÃÏÂ½µÑØ´¥·¢
 
/* SW_6ÖÐ¶Ï¿ØÖÆ¶¨Òå */
#define HAL_KEY_SW_6_IEN      IEN1   // ÖÐ¶ÏÊ¹ÄÜ¼Ä´æÆ÷
#define HAL_KEY_SW_6_IENBIT   BV(5)  // Port0ÖÐ¶ÏÊ¹ÄÜÎ»
#define HAL_KEY_SW_6_ICTL     P0IEN  // ¶Ë¿ÚÖÐ¶Ï¿ØÖÆ¼Ä´æÆ÷
#define HAL_KEY_SW_6_ICTLBIT  BV(1)  // P0.1ÖÐ¶ÏÊ¹ÄÜÎ»
#define HAL_KEY_SW_6_PXIFG    P0IFG  // ¶Ë¿ÚÖÐ¶Ï±êÖ¾¼Ä´æÆ÷
 
/* Ò¡¸ËÒÆ¶¯¶¨Òå (Î»ÓÚP2.0Òý½Å) */
#define HAL_KEY_JOY_MOVE_PORT   P2     // ¶Ë¿Ú¼Ä´æÆ÷
#define HAL_KEY_JOY_MOVE_BIT    BV(0)  // Î»ÑÚÂë(µÚ0Î»)
#define HAL_KEY_JOY_MOVE_SEL    P2SEL  // ¹¦ÄÜÑ¡Ôñ¼Ä´æÆ÷
#define HAL_KEY_JOY_MOVE_DIR    P2DIR  // ·½Ïò¼Ä´æÆ÷
 
/* Ò¡¸Ë±ßÑØÖÐ¶ÏÉèÖÃ */
#define HAL_KEY_JOY_MOVE_EDGEBIT  BV(3) // ±ßÑØ¿ØÖÆÎ»
#define HAL_KEY_JOY_MOVE_EDGE     HAL_KEY_FALLING_EDGE // Ê¹ÓÃÏÂ½µÑØ´¥·¢
 
/* Ò¡¸ËÖÐ¶Ï¿ØÖÆ¶¨Òå */
#define HAL_KEY_JOY_MOVE_IEN      IEN2   // ÖÐ¶ÏÊ¹ÄÜ¼Ä´æÆ÷
#define HAL_KEY_JOY_MOVE_IENBIT   BV(1)  // Port2ÖÐ¶ÏÊ¹ÄÜÎ»
#define HAL_KEY_JOY_MOVE_ICTL     P2IEN  // ¶Ë¿ÚÖÐ¶Ï¿ØÖÆ¼Ä´æÆ÷
#define HAL_KEY_JOY_MOVE_ICTLBIT  BV(0)  // P2.0ÖÐ¶ÏÊ¹ÄÜÎ»
#define HAL_KEY_JOY_MOVE_PXIFG    P2IFG  // ¶Ë¿ÚÖÐ¶Ï±êÖ¾¼Ä´æÆ÷
 
/* Ò¡¸ËADCÍ¨µÀ¶¨Òå */
#define HAL_KEY_JOY_CHN   HAL_ADC_CHANNEL_6 // Ê¹ÓÃADCÍ¨µÀ6
 


/**************************************************************************************************
 *                                            TYPEDEFS
 **************************************************************************************************/
// ÀàÐÍ¶¨ÒåÇøÓò£¨µ±Ç°Îª¿Õ£©

/**************************************************************************************************
 *                                        GLOBAL VARIABLES
 **************************************************************************************************/
// È«¾Ö±äÁ¿
static uint8 halKeySavedKeys;     // ±£´æÉÏÒ»´Î°´¼ü×´Ì¬£¨ÂÖÑ¯Ä£Ê½Ê¹ÓÃ£©
static halKeyCBack_t pHalKeyProcessFunction; // °´¼ü»Øµ÷º¯ÊýÖ¸Õë
static uint8 HalKeyConfigured;    // °´¼üÅäÖÃ±êÖ¾
bool Hal_KeyIntEnable;            // ÖÐ¶ÏÊ¹ÄÜ±êÖ¾

/**************************************************************************************************
 *                                        FUNCTIONS - Local
 **************************************************************************************************/
// ¾Ö²¿º¯ÊýÉùÃ÷
void halProcessKeyInterrupt(void); // ´¦Àí°´¼üÖÐ¶Ï
uint8 halGetJoyKeyInput(void);     // »ñÈ¡Ò¡¸Ë°´¼üÊäÈë


/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/


/**************************************************************************************************
 * @fn      HalKeyInit
 * @brief   ³õÊ¼»¯°´¼ü·þÎñ
 * @param   none
 * @return  None
 **************************************************************************************************/
void HalKeyInit( void )
{
  /* ³õÊ¼»¯°´¼ü×´Ì¬Îª0 */
  halKeySavedKeys = 0;
 
  /* ÅäÖÃSW_6°´¼üÒý½ÅÎªGPIOÊäÈë */
  HAL_KEY_SW_6_SEL &= ~(HAL_KEY_SW_6_BIT);    /* ÉèÖÃÒý½Å¹¦ÄÜÎªGPIO */
  HAL_KEY_SW_6_DIR &= ~(HAL_KEY_SW_6_BIT);    /* ÉèÖÃÒý½Å·½ÏòÎªÊäÈë */
 
  /* ÅäÖÃÒ¡¸ËÒý½ÅÎªGPIOÊäÈë */
  HAL_KEY_JOY_MOVE_SEL &= ~(HAL_KEY_JOY_MOVE_BIT); /* ÉèÖÃÒý½Å¹¦ÄÜÎªGPIO */
  HAL_KEY_JOY_MOVE_DIR &= ~(HAL_KEY_JOY_MOVE_BIT); /* ÉèÖÃÒý½Å·½ÏòÎªÊäÈë */
 
  /* ³õÊ¼»¯»Øµ÷º¯ÊýÎª */
  pHalKeyProcessFunction  =NULL;
 
  /* ±ê¼Ç°´¼üÎ´ÅäÖÃ */
  HalKeyConfigured = FALSE;
}


/**************************************************************************************************
 * @fn      HalKeyConfig
 * @brief   ÅäÖÃ°´¼ü·þÎñ
 * @param   interruptEnable - TRUE/FALSE, Ê¹ÄÜ/½ûÓÃÖÐ¶Ï
 *          cback - »Øµ÷º¯ÊýÖ¸Õë
 * @return  None
 **************************************************************************************************/
void HalKeyConfig (bool interruptEnable, halKeyCBack_t cback)
{
  /* ÉèÖÃÖÐ¶ÏÊ¹ÄÜ±êÖ¾ */
  Hal_KeyIntEnable = interruptEnable;
 
  /* ×¢²á»Øµ÷º¯Êý */
  pHalKeyProcessFunction = cback;
 
  /* ¼ì²éÊÇ·ñÆôÓÃÖÐ¶Ï */
  if (Hal_KeyIntEnable)
  {
    /* ÅäÖÃSW_6°´¼ü±ßÑØ´¥·¢ */
    PICTL &= ~(HAL_KEY_SW_6_EDGEBIT);    /* Çå³ý±ßÑØ¿ØÖÆÎ» */
    /* Èç¹ûÊÇÏÂ½µÑØ´¥·¢£¬ÔòÉèÖÃ¸ÃÎ» */
  #if (HAL_KEY_SW_6_EDGE == HAL_KEY_FALLING_EDGE)
    PICTL |= HAL_KEY_SW_6_EDGEBIT;
  #endif
 
    /* ÖÐ¶ÏÅäÖÃ£º
     * - Ê¹ÄÜ¶Ë¿ÚÖÐ¶ÏÉú³É
     * - Ê¹ÄÜCPUÖÐ¶Ï
     * - Çå³ý´ý´¦ÀíÖÐ¶Ï±êÖ¾
     */
    HAL_KEY_SW_6_ICTL |= HAL_KEY_SW_6_ICTLBIT;
    HAL_KEY_SW_6_IEN |= HAL_KEY_SW_6_IENBIT;
    HAL_KEY_SW_6_PXIFG = ~(HAL_KEY_SW_6_BIT);
    
 
    /* ÅäÖÃÒ¡¸Ë±ßÑØ´¥·¢ */
    HAL_KEY_JOY_MOVE_ICTL &= ~(HAL_KEY_JOY_MOVE_EDGEBIT);    /* Çå³ý±ßÑØ¿ØÖÆÎ» */
    /* Èç¹ûÊÇÏÂ½µÑØ´¥·¢£¬ÔòÉèÖÃ¸ÃÎ» */
  #if (HAL_KEY_JOY_MOVE_EDGE == HAL_KEY_FALLING_EDGE)
    HAL_KEY_JOY_MOVE_ICTL |= HAL_KEY_JOY_MOVE_EDGEBIT;
  #endif
 
    /* ÖÐ¶ÏÅäÖÃ£º
     * - Ê¹ÄÜ¶Ë¿ÚÖÐ¶ÏÉú³É
     * - Ê¹ÄÜCPUÖÐ¶Ï
     * - Çå³ý´ý´¦ÀíÖÐ¶Ï±êÖ¾
     */
    HAL_KEY_JOY_MOVE_ICTL |= HAL_KEY_JOY_MOVE_ICTLBIT;
    HAL_KEY_JOY_MOVE_IEN |= HAL_KEY_JOY_MOVE_IENBIT;
    HAL_KEY_JOY_MOVE_PXIFG = ~(HAL_KEY_JOY_MOVE_BIT);
 
    /* Èç¹û°´¼üÒÑÅäÖÃÖÐ¶Ï£¬Í£Ö¹ÂÖÑ¯¶¨Ê±Æ÷ */
    if (HalKeyConfigured == TRUE)
    {
      osal_stop_timerEx(Hal_TaskID, HAL_KEY_EVENT);  /* È¡Ïû»î¶¯ÂÖÑ¯ */
    }
  }
  else    /* ÖÐ¶ÏÎ´Ê¹ÄÜ */
  {
    /* ½ûÓÃÖÐ¶ÏÉú³É */
    HAL_KEY_SW_6_ICTL &= ~(HAL_KEY_SW_6_ICTLBIT);
    HAL_KEY_SW_6_IEN &= ~(HAL_KEY_SW_6_IENBIT);   /* Çå³ýÖÐ¶ÏÊ¹ÄÜÎ» */
 
    /* Æô¶¯ÂÖÑ¯ÊÂ¼þ */
    osal_set_event(Hal_TaskID, HAL_KEY_EVENT);
  }
 
  /* ±ê¼Ç°´¼üÒÑÅäÖÃ */
  HalKeyConfigured = TRUE;
}


/**************************************************************************************************
 * @fn      HalKeyRead
 * @brief   ¶ÁÈ¡µ±Ç°°´¼ü×´Ì¬
 * @param   None
 * @return  keys - µ±Ç°°´¼ü×´Ì¬
 **************************************************************************************************/
uint8 HalKeyRead ( void )
{
  uint8 keys = 0;
 
  /* ¼ì²éSW_6°´¼üÊÇ·ñ°´ÏÂ£¨µÍµçÆ½ÓÐÐ§£© */
  if (HAL_PUSH_BUTTON1())
  {
    keys |= HAL_KEY_SW_6;
  }
 
  /* ¼ì²éÒ¡¸ËÒÆ¶¯£¨¸ßµçÆ½ÓÐÐ§£© */
  if ((HAL_KEY_JOY_MOVE_PORT & HAL_KEY_JOY_MOVE_BIT))
  {
    keys |= halGetJoyKeyInput();
  }
 
  return keys;
}


/**************************************************************************************************
 * @fn      HalKeyPoll
 * @brief   ÓÉhal_driverµ÷ÓÃÒÔÂÖÑ¯°´¼ü
 * @param   None
 * @return  None
 **************************************************************************************************/
void HalKeyPoll (void)
{
  uint8 keys = 0;
 
  /* ¼ì²éÒ¡¸ËÒÆ¶¯£¨¸ßµçÆ½ÓÐÐ§£© */
  if ((HAL_KEY_JOY_MOVE_PORT & HAL_KEY_JOY_MOVE_BIT))
  {
    //keys = halGetJoyKeyInput();//Î´Ê¹ÓÃµ½
  }
 
  /* Èç¹ûÖÐ¶ÏÎ´Ê¹ÄÜ£¬±È½Ïµ±Ç°ºÍÉÏÒ»´Î°´¼ü×´Ì¬ */
  if (!Hal_KeyIntEnable)
  {
    if (keys == halKeySavedKeys)
    {
      /* ÍË³ö - Ã»ÓÐ°´¼ü×´Ì¬±ä»¯ */
      return;
    }
    /* ±£´æµ±Ç°°´¼ü×´Ì¬¹©ÏÂ´Î±È½Ï */
    //halKeySavedKeys = keys;
  }
  else
  {
    /* ÖÐ¶ÏÄ£Ê½ÔÚ´Ë´¦Àí */
  }
 
  /* ¼ì²éSW_6°´¼üÊÇ·ñ°´ÏÂ */
  if (HAL_PUSH_BUTTON1())
  {
    keys |= HAL_KEY_SW_6;
  }
 
  /* Èç¹ûÓÐÐÂ°´¼ü°´ÏÂÇÒ»Øµ÷º¯ÊýÒÑ×¢²á£¬Ôòµ÷ÓÃ»Øµ÷º¯Êý */
  if (keys && (pHalKeyProcessFunction))
  {
    (pHalKeyProcessFunction) (keys, HAL_KEY_STATE_NORMAL);
  }
}
 
/**************************************************************************************************
 * @fn      halGetJoyKeyInput
 * @brief   ½«ADCÖµÓ³Éäµ½¶ÔÓ¦µÄÒ¡¸Ë°´¼ü
 * @param   None
 * @return  keys - µ±Ç°Ò¡¸Ë°´¼ü×´Ì¬
 **************************************************************************************************/
uint8 halGetJoyKeyInput(void)
{
  /* Ò¡¸Ë¿ØÖÆ±àÂëÎªÄ£ÄâµçÑ¹Öµ
   * ¶ÁÈ¡JOY_LEVELÄ£ÄâÖµ²¢Ó³Éäµ½Ò¡¸ËÒÆ¶¯·½Ïò
   */
  uint8 adc;
  uint8 ksave0 = 0;
  uint8 ksave1;
 
  /* ³ÖÐø¶ÁÈ¡ADCÖµ£¬Ö±µ½Á½´ÎÁ¬Ðø¶ÁÈ¡½á¹ûÏàÍ¬£¨Ïû¶¶£© */
  do
  {
    ksave1 = ksave0;    /* ±£´æÉÏÒ»´Î¶ÁÈ¡½á¹û */
 
    /* ¶ÁÈ¡ADCÖµ£¨8Î»·Ö±æÂÊ£© */
    adc = HalAdcRead (HAL_KEY_JOY_CHN, HAL_ADC_RESOLUTION_8);
 
    /* ¸ù¾ÝADCÖµ·¶Î§ÅÐ¶Ï°´¼ü·½Ïò */
    if ((adc >= 2) && (adc <= 38))
    {
       ksave0 |= HAL_KEY_UP;      // ÉÏ
    }
    else if ((adc >= 74) && (adc <= 88))
    {
      ksave0 |= HAL_KEY_RIGHT;   // ÓÒ
    }
    else if ((adc >= 60) && (adc <= 73))
    {
      ksave0 |= HAL_KEY_LEFT;    // ×ó
    }
    else if ((adc >= 39) && (adc <= 59))
    {
      ksave0 |= HAL_KEY_DOWN;    // ÏÂ
    }
    else if ((adc >= 89) && (adc <= 100))
    {
      ksave0 |= HAL_KEY_CENTER;  // ÖÐ¼ä
    }
  } while (ksave0 != ksave1);    // Ö±µ½Á½´Î¶ÁÈ¡½á¹ûÏàÍ¬
 
  return ksave0;
}
 
/**************************************************************************************************
 * @fn      halProcessKeyInterrupt
 * @brief   ¼ì²éÓÐÐ§°´¼üÖÐ¶Ï£¬±£´æÖÐ¶ÏÇý¶¯µÄ°´¼ü×´Ì¬£¬²¢Í¨¹ýÏû¶¶ÑÓ³Ù´¦Àí
 * @param   None
 * @return  None
 **************************************************************************************************/
void halProcessKeyInterrupt (void)
{
  bool valid=FALSE;
 
  /* ¼ì²éSW_6°´¼üÖÐ¶Ï±êÖ¾ */
  if (HAL_KEY_SW_6_PXIFG & HAL_KEY_SW_6_BIT)  /* ÖÐ¶Ï±êÖ¾ÒÑÉèÖÃ */
  {
    HAL_KEY_SW_6_PXIFG = ~(HAL_KEY_SW_6_BIT); /* Çå³ýÖÐ¶Ï±êÖ¾ */
    valid = TRUE;
  }
 
  /* ¼ì²éÒ¡¸ËÒÆ¶¯ÖÐ¶Ï±êÖ¾ */
  if (HAL_KEY_JOY_MOVE_PXIFG & HAL_KEY_JOY_MOVE_BIT)  /* ÖÐ¶Ï±êÖ¾ÒÑÉèÖÃ */
  {
    HAL_KEY_JOY_MOVE_PXIFG = ~(HAL_KEY_JOY_MOVE_BIT); /* Çå³ýÖÐ¶Ï±êÖ¾ */
    valid = TRUE;
  }
 
  /* Èç¹ûÓÐÓÐÐ§ÖÐ¶Ï£¬Æô¶¯Ïû¶¶¶¨Ê±Æ÷ */
  if (valid)
  {
    osal_start_timerEx (Hal_TaskID, HAL_KEY_EVENT, HAL_KEY_DEBOUNCE_VALUE);
  }
}
 
/**************************************************************************************************
 * @fn      HalKeyEnterSleep
 * @brief   ½øÈëË¯ÃßÄ£Ê½Ç°µ÷ÓÃ
 * @param   None
 * @return  None
 **************************************************************************************************/
void HalKeyEnterSleep ( void )
{
  /* µ±Ç°Îª¿ÕÊµÏÖ */
}
 
/**************************************************************************************************
 * @fn      HalKeyExitSleep
 * @brief   Ë¯Ãß½áÊøºóµ÷ÓÃ
 * @param   None
 * @return  - ·µ»Ø±£´æµÄ°´¼ü×´Ì¬
 **************************************************************************************************/
uint8 HalKeyExitSleep ( void )
{
  /* »½ÐÑ²¢¶ÁÈ¡°´¼ü×´Ì¬ */
  return ( HalKeyRead () );
}
 
/***************************************************************************************************
 *                                    INTERRUPT SERVICE ROUTINE
 ***************************************************************************************************/
 
/**************************************************************************************************
 * @fn      halKeyPort0Isr
 * @brief   Port0ÖÐ¶Ï·þÎñ³ÌÐò
 * @param   None
 * @return  None
 **************************************************************************************************/
HAL_ISR_FUNCTION( halKeyPort0Isr, P0INT_VECTOR )
{
  HAL_ENTER_ISR();  // ½øÈëÖÐ¶Ï
 
  /* ¼ì²éSW_6°´¼üÖÐ¶Ï */
  if (HAL_KEY_SW_6_PXIFG & HAL_KEY_SW_6_BIT)
  {
    halProcessKeyInterrupt(); // ´¦Àí°´¼üÖÐ¶Ï
  }
 
  /* Çå³ýPort0µÄCPUÖÐ¶Ï±êÖ¾
   * ×¢Òâ£º±ØÐëÏÈÇå³ýPxIFGÔÙÇå³ýPxIF
   */
  HAL_KEY_SW_6_PXIFG = 0;
  HAL_KEY_CPU_PORT_0_IF = 0;
  
  CLEAR_SLEEP_MODE(); // Çå³ýË¯ÃßÄ£Ê½
  HAL_EXIT_ISR();     // ÍË³öÖÐ¶Ï
}
 
/**************************************************************************************************
 * @fn      halKeyPort2Isr
 * @brief   Port2ÖÐ¶Ï·þÎñ³ÌÐò
 * @param   None
 * @return  None
 **************************************************************************************************/
HAL_ISR_FUNCTION( halKeyPort2Isr, P2INT_VECTOR )
{
  HAL_ENTER_ISR();  // ½øÈëÖÐ¶Ï
  
  /* ¼ì²éÒ¡¸ËÒÆ¶¯ÖÐ¶Ï */
  if (HAL_KEY_JOY_MOVE_PXIFG & HAL_KEY_JOY_MOVE_BIT)
  {
    halProcessKeyInterrupt(); // ´¦Àí°´¼üÖÐ¶Ï
  }
 
  /* Çå³ýPort2µÄCPUÖÐ¶Ï±êÖ¾
   * ×¢Òâ£º±ØÐëÏÈÇå³ýPxIFGÔÙÇå³ýPxIF
   * ±¸×¢£ºP2_1ºÍP2_2ÊÇµ÷ÊÔÏß
   */
  HAL_KEY_JOY_MOVE_PXIFG = 0;
  HAL_KEY_CPU_PORT_2_IF = 0;
 
  CLEAR_SLEEP_MODE(); // Çå³ýË¯ÃßÄ£Ê½
  HAL_EXIT_ISR();     // ÍË³öÖÐ¶Ï
}
 
#else /* HAL_KEYÎ´Ê¹ÄÜÊ±µÄ¿ÕÊµÏÖ */
 
void HalKeyInit(void){}
void HalKeyConfig(bool interruptEnable, halKeyCBack_t cback){}
uint8 HalKeyRead(void){ return 0;}
void HalKeyPoll(void){}
 
#endif /* HAL_KEY */





/**************************************************************************************************
**************************************************************************************************/



