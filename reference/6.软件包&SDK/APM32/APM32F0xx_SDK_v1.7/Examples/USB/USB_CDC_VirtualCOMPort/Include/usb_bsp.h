/*!
 * @file        usb_bsp.h
 *
 * @brief       board head file
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

/* Define to prevent recursive inclusion */
#ifndef __USB_BSP_H
#define __USB_BSP_H

/* Includes */
#include "usbd_init.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup USB_CDC_VirtualCOMPort
  @{
  */

/** @defgroup USB_CDC_VirtualCOMPort_Macros Macros
  @{
*/

/**@} end of group USB_CDC_VirtualCOMPort_Macros */

/** @defgroup USB_CDC_VirtualCOMPort_Enumerations Enumerations
  @{
*/

/**@} end of group USB_CDC_VirtualCOMPort_Enumerations */

/** @defgroup USB_CDC_VirtualCOMPort_Structures Structures
  @{
*/

/**@} end of group USB_CDC_VirtualCOMPort_Structures */

/** @defgroup USB_CDC_VirtualCOMPort_Variables Variables
  @{
*/

/**@} end of group USB_CDC_VirtualCOMPort_Variables */

/** @defgroup USB_CDC_VirtualCOMPort_Functions Functions
  @{
*/

void USBD_HardWareInit(void);

#endif /*__USB_BSP_H */

/**@} end of group USB_CDC_VirtualCOMPort_Functions */
/**@} end of group USB_CDC_VirtualCOMPort */
/**@} end of group Examples */
