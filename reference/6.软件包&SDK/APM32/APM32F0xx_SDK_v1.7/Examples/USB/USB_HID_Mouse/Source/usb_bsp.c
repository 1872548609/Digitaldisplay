/*!
 * @file        usb_bsp.c
 *
 * @brief       Borad source file
 *
 * @version     V1.0.1
 *
 * @date        2022-09-20
 *
 * @attention
 *
 *  Copyright (C) 2020-2022 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Includes */
#include "usb_bsp.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup USB_HID_Mouse
  @{
  */

/** @defgroup USB_HID_Mouse_Macros Macros
  @{
*/

/**@} end of group USB_HID_Mouse_Macros */

/** @defgroup USB_HID_Mouse_Enumerations Enumerations
  @{
*/

/**@} end of group USB_HID_Mouse_Enumerations */

/** @defgroup USB_HID_Mouse_Structures Structures
  @{
*/

/**@} end of group USB_HID_Mouse_Structures */

/** @defgroup USB_HID_Mouse_Variables Variables
  @{
*/

/**@} end of group USB_HID_Mouse_Variables */

/** @defgroup USB_HID_Mouse_Functions Functions
  @{
  */

/** @defgroup USB_BSP_Functions
  @{
  */

void USBD_HardWareInit(void)
{
    RCM_EnableHSI48();
    RCM_ConfigSYSCLK(RCM_SYSCLK_SEL_HSI48);
    RCM_ConfigUSBCLK(RCM_USBCLK_HSI48);
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_USB);

    NVIC_EnableIRQRequest(USBD_IRQn, 2);
}

/**@} end of group USB_BSP_Functions */
/**@} end of group USB_HID_Mouse_Functions */
/**@} end of group USB_HID_Mouse */
/**@} end of group Examples */
