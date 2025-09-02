/*!
 * @file        usbd_hid.h
 *
 * @brief       HID head file
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
#ifndef __USBD_HID_H
#define __USBD_HID_H

/* Includes */
#include "usbd_init.h"

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

/** @defgroup USBD_HID_Enumerations
  @{
*/

/**
 * @brief   Key enumeration
 */
enum
{
    HID_MOUSE_KEY_NULL,
    HID_MOUSE_KEY_LEFT,
    HID_MOUSE_KEY_RIGHT,
    HID_MOUSE_KEY_UP,
    HID_MOUSE_KEY_DOWN,
};

/**@} end of group USBD_HID_Enumerations */
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

/** @defgroup USBD_HID_Functions
  @{
  */

void HidMouse_Init(void);
void HidMouse_Proc(void);
void HidMouse_SetConfigCallBack(void);

#endif /* __USBD_HID_H */

/**@} end of group USBD_HID_Functions */
/**@} end of group USB_HID_Mouse_Functions */
/**@} end of group USB_HID_Mouse */
/**@} end of group Examples */
