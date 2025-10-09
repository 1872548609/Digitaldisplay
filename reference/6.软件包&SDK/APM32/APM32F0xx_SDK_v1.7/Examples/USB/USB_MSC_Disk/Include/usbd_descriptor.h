/*!
 * @file        ubsd_descriptor.h
 *
 * @brief       MSC descriptor head file
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
#ifndef __USBD_DESCRIPTOR_H
#define __USBD_DESCRIPTOR_H

/* Includes */
#include "usbd_core.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup USB_MSC_Disk
  @{
  */

/** @defgroup USB_MSC_Disk_Macros Macros
  @{
*/

/** @defgroup USBD_Descriptor_Macros
  @{
*/

#define USB_DEVICE_DESCRIPTOR_SIZE          (18)
#define USB_CONFIG_DESCRIPTOR_SIZE          (32)
#define USB_LANGID_STRING_SIZE              (4)
#define USB_VENDOR_STRING_SIZE              (12)
#define USB_PRODUCT_STRING_SIZE             (26)
#define USB_SERIAL_STRING_SIZE              (26)
#define USB_USER_STRING_1_SIZE              (94)
#define USB_USER_STRING_2_SIZE              (72)
#define SRTING_DESC_NUM                     (4)

/**@} end of group USBD_Descriptor_Macros */
/**@} end of group USB_MSC_Disk_Macros */

/** @defgroup USB_MSC_Disk_Enumerations Enumerations
  @{
*/

/**@} end of group USB_MSC_Disk_Enumerations */

/** @defgroup USB_MSC_Disk_Structures Structures
  @{
*/

/**@} end of group USB_MSC_Disk_Structures */

/** @defgroup USB_MSC_Disk_Variables Variables
  @{
*/

/** @defgroup USBD_Descriptor_Variables
  @{
*/

extern USBD_Descriptor_T g_deviceDescriptor;
extern USBD_Descriptor_T g_configDescriptor;
extern USBD_Descriptor_T g_stringDescriptor[SRTING_DESC_NUM];

/**@} end of group USBD_Descriptor_Variables */
/**@} end of group USB_MSC_Disk_Variables */

/** @defgroup USB_MSC_Disk_Functions Functions
  @{
  */

#endif /* __USBD_DESCRIPTOR_H */

/**@} end of group USB_MSC_Disk_Functions */
/**@} end of group USB_MSC_Disk */
/**@} end of group Examples */
