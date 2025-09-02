/*!
 * @file        usbd_storage_disk.h
 *
 * @brief       usb device storage disk operations functions
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
#ifndef __USBD_STORAGE_H
#define __USBD_STORAGE_H

/* Includes */
#include "usbd_msc.h"
#include "usbd_init.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup USB_MSC_Disk
  @{
  */

/** @defgroup USB_MSC_Disk_Macros Macros
  @{
*/

/** @defgroup USBD_Storage_Disk_Macros
  @{
*/

#define USBD_STD_INQUIRY_LENGTH          36

/* define Storage disk block num and size */
#define STORAGE_BLK_SIZ                  MSC_MEDIA_PACKET

#define STORAGE_BLK_NBR                  (100 * 1024)

/**@} end of group USBD_Storage_Disk_Macros */
/**@} end of group USB_MSC_Disk_Macros */

/** @defgroup USB_MSC_Disk_Enumerations Enumerations
  @{
*/

/**@} end of group USB_MSC_Disk_Enumerations */

/** @defgroup USB_MSC_Disk_Variables Variables
  @{
*/

/** @defgroup USBD_Storage_Disk_Variables
  @{
*/

typedef uint8_t (*STORAGE_Init_T)(uint8_t lun);
typedef uint8_t (*STORAGE_ReadCapacity_T)(uint8_t lun, uint32_t* blockNum, uint32_t* blockSize);
typedef uint8_t (*STORAGE_CheckReady_T)(uint8_t lun);
typedef uint8_t (*STORAGE_CheckWPR_T)(uint8_t lun);
typedef uint8_t (*STORAGE_Read_T)(uint8_t lun, uint8_t* buf, uint32_t blkAddr, uint16_t blkLen);
typedef uint8_t (*STORAGE_Write_T)(uint8_t lun, uint8_t* buf, uint32_t blkAddr, uint16_t blkLen);

/**@} end of group USBD_Storage_Disk_Variables */
/**@} end of group USB_MSC_Disk_Variables */

/** @defgroup USB_MSC_Disk_Structures Structures
  @{
*/

/** @defgroup USBD_Storage_Disk_Structures
  @{
*/

typedef struct
{
    STORAGE_Init_T         Init;
    STORAGE_ReadCapacity_T ReadCapacity;
    STORAGE_CheckReady_T   CheckReady;
    STORAGE_CheckWPR_T     CheckWPR;
    STORAGE_Read_T         ReadData;
    STORAGE_Write_T        WriteData;
    uint8_t* pInquiryData;
} STORAGE_Callback_T;

/**@} end of group USBD_Storage_Disk_Structures */
/**@} end of group USB_MSC_Disk_Structures */

extern STORAGE_Callback_T g_storageCallBack;

/** @defgroup USB_MSC_Disk_Functions Functions
  @{
  */

#endif /*__USBD_STORAGE_H */

/**@} end of group USB_MSC_Disk_Functions */
/**@} end of group USB_MSC_Disk */
/**@} end of group Examples */
