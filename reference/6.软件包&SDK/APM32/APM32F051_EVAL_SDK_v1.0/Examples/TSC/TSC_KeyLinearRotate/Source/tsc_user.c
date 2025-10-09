/*!
 * @file        tsc_user.c
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

/* Includes */
#include "tsc_user.h"

/** @addtogroup Examples
  * @brief TSC touch examples
  @{
  */

/** @addtogroup TSC_KeyLinearRotate
  @{
  */

/** @defgroup TSC_KeyLinearRotate_Variables Variables
  @{
  */

/* Source and Configuration (ROM) */
CONST TSC_Channel_Src_T MyChannels_Src[TOUCH_TOTAL_CHANNELS] =
{
    /* Block 0 */
    { CHANNEL_0_SRC, CHANNEL_0_IO_MSK, CHANNEL_0_GRP_MSK },
    { CHANNEL_2_SRC, CHANNEL_2_IO_MSK, CHANNEL_2_GRP_MSK },

    /* Block 1 */
    { CHANNEL_1_SRC, CHANNEL_1_IO_MSK, CHANNEL_1_GRP_MSK },
    { CHANNEL_3_SRC, CHANNEL_3_IO_MSK, CHANNEL_3_GRP_MSK },
};

/* Destination (ROM) */
CONST TSC_Channel_Dest_T MyChannels_Dest[TOUCH_TOTAL_CHANNELS] =
{
    /* Block 0 */
    { CHANNEL_0_DEST },
    { CHANNEL_2_DEST },

    /* Block 1 */
    { CHANNEL_1_DEST },
    { CHANNEL_3_DEST },
};

/* Data (RAM) */
TSC_Channel_Data_T MyChannels_Data[TOUCH_TOTAL_CHANNELS];

/**@} Channels_Config */

/** @addtogroup Blocks_Config
  @{
*/
/* List (ROM) */
CONST TSC_Block_T MyBlocks[TOUCH_TOTAL_BLOCKS] = {
    {&MyChannels_Src[0], &MyChannels_Dest[0], MyChannels_Data, BLOCK_0_NUMCHANNELS, BLOCK_0_MSK_CHANNELS, BLOCK_0_MSK_GROUPS},
    {&MyChannels_Src[2], &MyChannels_Dest[2], MyChannels_Data, BLOCK_1_NUMCHANNELS, BLOCK_1_MSK_CHANNELS, BLOCK_1_MSK_GROUPS},
};

/**@} Blocks_Config */

/** @addtogroup TouchKeys_Config
  @{
*/
/* Data (RAM) */
TSC_TouchKeyData_T MyKeys_Data[TOUCH_TOTAL_KEYS];

/* Parameters (RAM) */
TSC_TouchKeyParam_T MyKeys_Param[TOUCH_TOTAL_KEYS];

/**@} end of group TSC_KeyLinearRotate_Variables*/

/** @defgroup TSC_KeyLinearRotate_Functions Functions
  @{
  */

/* State Machine (ROM) */
void MyKeys_ProcessErrorState(void);
void MyKeys_ProcessOffState(void);
/**@} end of group TSC_KeyLinearRotate_Functions */

/** @defgroup TSC_KeyLinearRotate_Variables Variables
  @{
  */

CONST TSC_State_T MyKeys_StateMachine[] =
{
    /* Calibration states */
    { TSC_STATEMASK_CALIB,              TSC_TouchKey_ProcessCalibrationState },    /*!< 0 */
    { TSC_STATEMASK_DEB_CALIB,          TSC_TouchKey_ProcessDebCalibrationState }, /*!< 1 */
    /* Release states */
    { TSC_STATEMASK_RELEASE,            TSC_TouchKey_ProcessReleaseState },        /*!< 2 */
#if TOUCH_USE_PROX > 0
    { TSC_STATEMASK_DEB_RELEASE_PROX,   TSC_TouchKey_ProcessDebReleaseProxState }, /*!< 3 */
#else
    { TSC_STATEMASK_DEB_RELEASE_PROX,   0 }, //!< 3
#endif
    { TSC_STATEMASK_DEB_RELEASE_DETECT, TSC_TouchKey_ProcessDebReleaseDetectState }, /*!< 4 */
    { TSC_STATEMASK_DEB_RELEASE_TOUCH,  TSC_TouchKey_ProcessDebReleaseTouchState },  /*!< 5 */
#if TOUCH_USE_PROX > 0
    /* Proximity states */
    { TSC_STATEMASK_PROX,               TSC_TouchKey_ProcessProxState },          /*!< 6 */
    { TSC_STATEMASK_DEB_PROX,           TSC_TouchKey_ProcessDebProxState },       /*!< 7 */
    { TSC_STATEMASK_DEB_PROX_DETECT,    TSC_TouchKey_ProcessDebProxDetectState }, /*!< 8 */
    { TSC_STATEMASK_DEB_PROX_TOUCH,     TSC_TouchKey_ProcessDebProxTouchState },  /*!< 9 */
#else
    { TSC_STATEMASK_PROX,               0 }, /*!< 6 */
    { TSC_STATEMASK_DEB_PROX,           0 }, /*!< 7 */
    { TSC_STATEMASK_DEB_PROX_DETECT,    0 }, /*!< 8 */
    { TSC_STATEMASK_DEB_PROX_TOUCH,     0 }, /*!< 9 */
#endif
    /* Detect states */
    { TSC_STATEMASK_DETECT,             TSC_TouchKey_ProcessDetectState },    /*!< 10 */
    { TSC_STATEMASK_DEB_DETECT,         TSC_TouchKey_ProcessDebDetectState }, /*!< 11 */
    /* Touch states */
    { TSC_STATEMASK_TOUCH,              TSC_TouchKey_ProcessTouchState },     /*!< 12 */
    /* Error states */
    { TSC_STATEMASK_ERROR,              MyKeys_ProcessErrorState },          /*!< 13 */
    { TSC_STATEMASK_DEB_ERROR_CALIB,    TSC_TouchKey_ProcessDebErrorState }, /*!< 14 */
    { TSC_STATEMASK_DEB_ERROR_RELEASE,  TSC_TouchKey_ProcessDebErrorState }, /*!< 15 */
    { TSC_STATEMASK_DEB_ERROR_PROX,     TSC_TouchKey_ProcessDebErrorState }, /*!< 16 */
    { TSC_STATEMASK_DEB_ERROR_DETECT,   TSC_TouchKey_ProcessDebErrorState }, /*!< 17 */
    { TSC_STATEMASK_DEB_ERROR_TOUCH,    TSC_TouchKey_ProcessDebErrorState }, /*!< 18 */
    /* Other states */
    { TSC_STATEMASK_OFF,                MyKeys_ProcessOffState } /*!< 19 */
};

/* Methods for "extended" type (ROM) */
CONST TSC_TouchKeyMethods_T MyKeys_Methods =
{
    TSC_TouchKey_Config,
    TSC_TouchKey_Process
};

/* TouchKeys list (ROM) */
CONST TSC_TouchKey_T MyTouchKeys[TOUCH_TOTAL_KEYS] =
{
  { &MyKeys_Data[0], &MyKeys_Param[0], &MyChannels_Data[CHANNEL_0_DEST], MyKeys_StateMachine, &MyKeys_Methods },
  { &MyKeys_Data[1], &MyKeys_Param[1], &MyChannels_Data[CHANNEL_1_DEST], MyKeys_StateMachine, &MyKeys_Methods },
  { &MyKeys_Data[2], &MyKeys_Param[2], &MyChannels_Data[CHANNEL_2_DEST], MyKeys_StateMachine, &MyKeys_Methods },
  { &MyKeys_Data[3], &MyKeys_Param[3], &MyChannels_Data[CHANNEL_3_DEST], MyKeys_StateMachine, &MyKeys_Methods },
};

/* List (ROM) */
CONST TSC_Object_T MyObjects[TOUCH_TOTAL_OBJECTS] =
{
    { TSC_OBJ_TOUCHKEY, (TSC_TouchKey_T *)&MyTouchKeys[0] },
    { TSC_OBJ_TOUCHKEY, (TSC_TouchKey_T *)&MyTouchKeys[1] },
    { TSC_OBJ_TOUCHKEY, (TSC_TouchKey_T *)&MyTouchKeys[2] },
    { TSC_OBJ_TOUCHKEY, (TSC_TouchKey_T *)&MyTouchKeys[3] },
};

/* Group (RAM) */
TSC_ObjectGroup_T MyObjGroup =
{
    &MyObjects[0],        /*!< First object */
    TOUCH_TOTAL_OBJECTS,  /*!< Number of objects */
    0x00,                 /*!< State mask reset value */
    TSC_STATE_NOT_CHANGED /*!< Current state */
};

TSC_Params_T TSC_Params =
{
    TOUCH_ACQ_MIN,
    TOUCH_ACQ_MAX,
    TOUCH_CALIB_SAMPLES,
    TOUCH_DTO,
#if TOUCH_TOTAL_KEYS > 0
    MyKeys_StateMachine,    /*!< Default state machine for TouchKeys */
    &MyKeys_Methods,        /*!< Default methods for TouchKeys */
#endif
#if TOUCH_TOTAL_LNRTS > 0
    MyLinRots_StateMachine, /*!< Default state machine for LinRots */
    &MyLinRots_Methods      /*!< Default methods for LinRots */
#endif
};

/* Hold the last time value for ECS */
__IO TSC_tTick_ms_T Global_ECS_last_tick;
/* Set by TSC interrupt routine to indicate the End Of Acquisition */
__IO uint32_t Global_EOA;
uint32_t Global_ProcessSensor;

/**@} end of group TSC_KeyLinearRotate_Variables*/

/** @defgroup TSC_KeyLinearRotate_Functions Functions
  @{
  */

/*!
 * @brief       Config the Touch Driver
 *
 * @param       None
 *
 * @retval      None
 */
void TSC_User_Config(void)
{
#if TOUCH_TSC_GPIO_CONFIG == 0
    /* This function must be created by the user to initialize the Touch Sensing GPIOs */
#endif
    TSC_Obj_ConfigGroup(&MyObjGroup);
    TSC_Config(MyBlocks);
    TSC_User_Thresholds();
}

/*!
 * @brief       Execute Touch Driver main State machine
 *
 * @param       None
 *
 * @retval      status Return TSC_STATUS_OK if the acquisition is done
 */
TSC_STATUS_T TSC_User_Action(void)
{
    static uint32_t idx_block = 0;
    static uint32_t config_done = 0;
    TSC_STATUS_T status;

    /* Configure block */
    if (!config_done)
    {
        TSC_Acq_ConfigBlock(idx_block);
        TSC_Acq_StartPerConfigBlock();
        config_done = 1;
        #if TOUCH_USE_ACQ_INTERRUPT > 0
        Global_EOA = 0; //!< Will be set by the TSC interrupt routine
        #endif
    }

    /* Check end of acquisition */
    #if TOUCH_USE_ACQ_INTERRUPT > 0
    if (Global_EOA) //!< Set by the TSC interrupt routine
    #else
    if (TSC_Acq_WaitBlockEOA() == TSC_STATUS_OK)
    #endif
    {
        TSC_Acq_ReadBlockResult(idx_block, 0, 0);
        idx_block++;
        config_done = 0;
    }

    /* Process objects, DxS and ECS, Check if all blocks have been acquired */
    if (idx_block > TOUCH_TOTAL_BLOCKS-1)
    {
        idx_block = 0;
        config_done = 0;

        TSC_Obj_ProcessGroup(&MyObjGroup);
        TSC_Dxs_FirstObj(&MyObjGroup);

        /* ECS every 100ms */
        if (TSC_Time_Delay_ms(100, &Global_ECS_last_tick) == TSC_STATUS_OK)
        {
            if (TSC_Ecs_Process(&MyObjGroup) == TSC_STATUS_OK)
            {
                Global_ProcessSensor = 0;
            }
            else
            {
                Global_ProcessSensor = 1;
            }
        }
        status = TSC_STATUS_OK;
    }
    else
    {
        status = TSC_STATUS_BUSY;
    }
    return status;
}

/*!
 * @brief       Set thresholds for each object (optional).
 *
 * @param       None
 *
 * @retval      None
 */
void TSC_User_Thresholds(void)
{

}

/**@} end of group TSC_KeyLinearRotate_Functions */
/**@} end of group TSC_KeyLinearRotate */
/**@} end of group Examples */
