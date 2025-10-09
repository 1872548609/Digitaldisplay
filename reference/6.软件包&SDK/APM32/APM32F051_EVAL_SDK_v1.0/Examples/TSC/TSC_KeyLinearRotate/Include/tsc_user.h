/*!
 * @file        tsc_user.h
 *
 * @brief       Touch-Sensing user configuration and api file.
 *
 * @version     V1.0.0
 *
 * @date        2022-09-29
 *
 * @attention
 *
 *  Copyright (C) 2022 Geehy Semiconductor
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
#ifndef __TSC_USER_H
#define __TSC_USER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "tsc.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup TSC_KeyLinearRotate
  @{
  */

/** @defgroup TSC_KeyLinearRotate_Macros Macros
  @{
*/
#define CHANNEL_0_IO_MSK    (TSC_GROUP2_IO2)
#define CHANNEL_0_GRP_MSK   (TSC_GROUP2)
#define CHANNEL_0_SRC       (1) /*!< Index in source register (TSC->IOGXCR[]) */
#define CHANNEL_0_DEST      (0) /*!< Index in destination result array */

#define CHANNEL_1_IO_MSK    (TSC_GROUP2_IO3)
#define CHANNEL_1_GRP_MSK   (TSC_GROUP2)
#define CHANNEL_1_SRC       (1) /*!< Index in source register (TSC->IOGXCR[]) */
#define CHANNEL_1_DEST      (1) /*!< Index in destination result array */

#define CHANNEL_2_IO_MSK    (TSC_GROUP3_IO2)
#define CHANNEL_2_GRP_MSK   (TSC_GROUP3)
#define CHANNEL_2_SRC       (2) /*!< Index in source register (TSC->IOGXCR[]) */
#define CHANNEL_2_DEST      (2) /*!< Index in destination result array */

#define CHANNEL_3_IO_MSK    (TSC_GROUP3_IO3)
#define CHANNEL_3_GRP_MSK   (TSC_GROUP3)
#define CHANNEL_3_SRC       (2) /*!< Index in source register (TSC->IOGXCR[]) */
#define CHANNEL_3_DEST      (3) /*!< Index in destination result array */

#define SHIELD_IO_MSK       (0)

#define BLOCK_0_NUMCHANNELS      (2)
#define BLOCK_0_MSK_CHANNELS     (CHANNEL_0_IO_MSK | CHANNEL_2_IO_MSK)
#define BLOCK_0_MSK_GROUPS       (CHANNEL_0_GRP_MSK | CHANNEL_2_GRP_MSK)
                                 
#define BLOCK_1_NUMCHANNELS      (2)
#define BLOCK_1_MSK_CHANNELS     (CHANNEL_1_IO_MSK | CHANNEL_3_IO_MSK)
#define BLOCK_1_MSK_GROUPS       (CHANNEL_1_GRP_MSK |CHANNEL_3_GRP_MSK)
/**@} end of group TSC_KeyLinearRotate_Macros*/

/** @defgroup TSC_KeyLinearRotate_Variables Variables
  @{
  */

extern CONST TSC_Block_T MyBlocks[];
extern CONST TSC_TouchKey_T MyTouchKeys[];
extern CONST TSC_Object_T MyObjects[];
extern TSC_ObjectGroup_T MyObjGroup;
extern uint32_t Global_ProcessSensor;

/**@} end of group TSC_KeyLinearRotate_Variables*/

/** @defgroup TSC_KeyLinearRotate_Functions Functions
  @{
  */
void TSC_User_Config(void);
void TSC_User_Thresholds(void);
TSC_STATUS_T TSC_User_Action(void);


#ifdef __cplusplus
}
#endif

#endif /* __TSC_USER_H */

/**@} end of group TSC_KeyLinearRotate_Functions */
/**@} end of group TSC_KeyLinearRotate */
/**@} end of group Examples */
