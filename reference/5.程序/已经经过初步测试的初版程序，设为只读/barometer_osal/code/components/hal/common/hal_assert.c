/**************************************************************************************************
  Filename:       hal_assert.c
  Revised:        $Date: 2010-11-22 08:13:43 -0800 (Mon, 22 Nov 2010) $
  Revision:       $Revision: 24480 $

  Description:    Describe the purpose and contents of the file.


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


/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "hal_assert.h"
#include "hal_types.h"
#include "hal_board.h"
#include "hal_defs.h"
#include "hal_mcu.h"



/* ------------------------------------------------------------------------------------------------
 *                                       Local Prototypes
 * ------------------------------------------------------------------------------------------------
 */
void halAssertHazardLights(void);


/**************************************************************************************************
 * @fn          halAssertHandler
 *
 * @brief       Logic to handle an assert.
 *
 * @param       none
 *
 * @return      none
 **************************************************************************************************
 */
void halAssertHandler(void)
{
    /* execute code that handles asserts */
    #ifdef ASSERT_RESET
    HAL_SYSTEM_RESET();
    #elif !defined ASSERT_WHILE
    halAssertHazardLights();
    #else
    while(1);
    #endif
}

#if !defined ASSERT_WHILE
/**************************************************************************************************
 * @fn          halAssertHazardLights
 *
 * @brief       Blink LEDs to indicate an error.
 *
 * @param       none
 *
 * @return      none
 **************************************************************************************************
 */
void halAssertHazardLights(void)
{
    enum
    {
        DEBUG_DATA_RSTACK_HIGH_OFS,
        DEBUG_DATA_RSTACK_LOW_OFS,
        DEBUG_DATA_TX_ACTIVE_OFS,
        DEBUG_DATA_RX_ACTIVE_OFS,

        #if (defined HAL_MCU_AVR) || (defined HAL_MCU_CC2430)
        DEBUG_DATA_INT_MASK_OFS,
        #elif (defined HAL_MCU_CC2530) || (defined HAL_MCU_CC2533)
        DEBUG_DATA_INT_MASK0_OFS,
        DEBUG_DATA_INT_MASK1_OFS,
        #endif

        DEBUG_DATA_SIZE
    };

    uint8 buttonHeld;
    uint8 debugData[DEBUG_DATA_SIZE];

    /* disable all interrupts before anything else */
    #ifdef HAL_DISABLE_INTERRUPTS
    HAL_DISABLE_INTERRUPTS();
    #endif

    /*-------------------------------------------------------------------------------
    *  Initialize LEDs and turn them off.
    */
    #ifdef HAL_BOARD_INIT
    HAL_BOARD_INIT();
    #endif

    #ifdef HAL_LED1_TURN_OFF
    HAL_LED1_TURN_OFF();
    #endif

    #ifdef HAL_LED2_TURN_OFF
    HAL_LED2_TURN_OFF();
    #endif

    #ifdef HAL_LED3_TURN_OFF
    HAL_LED3_TURN_OFF();
    #endif

    #ifdef HAL_LED4_TURN_OFF
    HAL_LED4_TURN_OFF();
    #endif
    
    /*-------------------------------------------------------------------------------
    *  Master infinite loop.
    */
    for (;;)
    {
        buttonHeld = 0;

        /*-------------------------------------------------------------------------------
        *  "Hazard lights" loop.  A held keypress will exit this loop.
        */
        do
        {
            #ifdef HAL_LED_BLINK_DELAY
            HAL_LED_BLINK_DELAY();
            #endif

            /* toggle LEDS, the #ifdefs are in case HAL has logically remapped non-existent LEDs */
            #if (HAL_LED_NUM >= 1)
            HAL_LED1_TOGGLE();
            #endif
            
            #if (HAL_LED_NUM >= 2)
            HAL_LED2_TOGGLE();
            #endif
            
            #if (HAL_LED_NUM >= 3)
            HAL_LED3_TOGGLE();
            #endif
            
            #if (HAL_LED_NUM >= 4)
            HAL_LED4_TOGGLE();
            #endif

            /* escape hatch to continue execution, set escape to '1' to continue execution */
            {
                static uint8 escape = 0;
                if (escape)
                {
                    escape = 0;
                    return;
                }
            }

            /* break out of loop if button is held long enough */
            #ifdef HAL_PUSH_BUTTON1
            if (HAL_PUSH_BUTTON1())
            {
                buttonHeld++;
            }
            else
            {
                buttonHeld = 0;
            }
            #endif
        }
        while (buttonHeld != 10);  /* loop until button is held specified number of loops */

        /*-------------------------------------------------------------------------------
        *  Just exited from "hazard lights" loop.
        */

        /* turn off all LEDs */
        #ifdef HAL_LED1_TURN_OFF
        HAL_LED1_TURN_OFF();
        #endif

        #ifdef HAL_LED2_TURN_OFF
        HAL_LED2_TURN_OFF();
        #endif

        #ifdef HAL_LED3_TURN_OFF
        HAL_LED3_TURN_OFF();
        #endif

        #ifdef HAL_LED4_TURN_OFF
        HAL_LED4_TURN_OFF();
        #endif
        
        /* wait for button release */
        #if defined(HAL_DEBOUNCE) && defined(HAL_PUSH_BUTTON1)
        HAL_DEBOUNCE(!HAL_PUSH_BUTTON1());
        #endif

        /*-------------------------------------------------------------------------------
        *  Load debug data into memory.
        */
        #ifdef HAL_MCU_AVR
        {
            uint8 * pStack;
            pStack = (uint8 *) SP;
            pStack++; /* point to return address on stack */
            debugData[DEBUG_DATA_RSTACK_HIGH_OFS] = *pStack;
            pStack++;
            debugData[DEBUG_DATA_RSTACK_LOW_OFS] = *pStack;
        }
        debugData[DEBUG_DATA_INT_MASK_OFS] = EIMSK;
        #endif

        #if (defined HAL_MCU_CC2430)
        debugData[DEBUG_DATA_INT_MASK_OFS] = RFIM;
        #elif (defined HAL_MCU_CC2530) || (defined HAL_MCU_CC2533)
        debugData[DEBUG_DATA_INT_MASK0_OFS] = RFIRQM0;
        debugData[DEBUG_DATA_INT_MASK1_OFS] = RFIRQM1;
        #endif


        #if (defined HAL_MCU_AVR) || (defined HAL_MCU_CC2430) || (defined HAL_MCU_CC2530) || (defined HAL_MCU_CC2533) || (defined HAL_MCU_MSP430)
        debugData[DEBUG_DATA_TX_ACTIVE_OFS] = macTxActive;
        debugData[DEBUG_DATA_RX_ACTIVE_OFS] = macRxActive;
        #endif

        /* initialize for data dump loop */
        {
            uint8 iBit;
            uint8 iByte;

            iBit  = 0;
            iByte = 0;

            /*-------------------------------------------------------------------------------
            *  Data dump loop.  A button press cycles data bits to an LED.
            */
            while (iByte < DEBUG_DATA_SIZE)
            {
                /* wait for key press */
                #ifdef HAL_PUSH_BUTTON1
                while(!HAL_PUSH_BUTTON1());
                #endif

                /* turn on all LEDs for first bit of byte, turn on three LEDs if not first bit */
                #ifdef HAL_LED1_TURN_ON
                HAL_LED1_TURN_ON();
                #endif

                #ifdef HAL_LED2_TURN_ON
                HAL_LED2_TURN_ON();
                #endif

                #ifdef HAL_LED3_TURN_ON
                HAL_LED3_TURN_ON();
                #endif
                
                if (iBit == 0)
                {
                    #ifdef HAL_LED4_TURN_ON
                    HAL_LED4_TURN_ON();
                    #endif
                }
                else
                {
                    #ifdef HAL_LED4_TURN_OFF
                    HAL_LED4_TURN_OFF();
                    #endif
                }

                /* wait for debounced key release */
                #if defined(HAL_DEBOUNCE) && defined(HAL_PUSH_BUTTON1)
                HAL_DEBOUNCE(!HAL_PUSH_BUTTON1());
                #endif

                /* turn off all LEDs */
                #ifdef HAL_LED1_TURN_OFF
                HAL_LED1_TURN_OFF();
                #endif

                #ifdef HAL_LED2_TURN_OFF
                HAL_LED2_TURN_OFF();
                #endif

                #ifdef HAL_LED3_TURN_OFF
                HAL_LED3_TURN_OFF();
                #endif

                #ifdef HAL_LED4_TURN_OFF
                HAL_LED4_TURN_OFF();
                #endif
                        
                /* output value of data bit to LED1 */
                if (debugData[iByte] & (1 << (7 - iBit)))
                {
                    #ifdef HAL_LED1_TURN_ON
                    HAL_LED1_TURN_ON();
                    #endif
                }
                else
                {
                    #ifdef HAL_LED1_TURN_OFF
                    HAL_LED1_TURN_OFF();
                    #endif
                }

                /* advance to next bit */
                iBit++;
                if (iBit == 8)
                {
                    iBit = 0;
                    iByte++;
                }
            }
        }

        /*
        *  About to enter "hazard lights" loop again.  Turn off LED1 in case the last bit
        *  displayed happened to be one.  This guarantees all LEDs are off at the start of
        *  the flashing loop which uses a toggle operation to change LED states.
        */
        #ifdef HAL_LED1_TURN_OFF
        HAL_LED1_TURN_OFF();
        #endif
    }
}
#endif

/* ------------------------------------------------------------------------------------------------
 *                                    Compile Time Assertions
 * ------------------------------------------------------------------------------------------------
 */

/* integrity check of type sizes */
HAL_ASSERT_SIZE(  int8, 1);
HAL_ASSERT_SIZE( uint8, 1);
HAL_ASSERT_SIZE( int16, 2);
HAL_ASSERT_SIZE(uint16, 2);
HAL_ASSERT_SIZE( int32, 4);
HAL_ASSERT_SIZE(uint32, 4);


/**************************************************************************************************
*/
