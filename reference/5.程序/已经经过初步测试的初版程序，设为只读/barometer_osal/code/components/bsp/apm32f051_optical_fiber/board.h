/*!
 * @file        Board.h
 *
 * @brief       Header file for Board
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
#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/
#ifdef BOARD_APM32F051_EVAL
#include "Board_APM32F051_EVAL/inc/board_apm32f051_eval.h"
#endif

#ifdef BOARD_APM32F072_EVAL
#include "Board_APM32F072_EVAL/inc/board_apm32f072_eval.h"
#endif

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/
//#include "bsp_delay.h"
#include "apm32f0xx.h"
#include "apm32f0xx_i2c.h"
#include "apm32f0xx_eint.h"
#include "apm32f0xx_rcm.h"
#include "apm32f0xx_gpio.h"
#include "apm32f0xx_usart.h"
#include "apm32f0xx_misc.h"
#include "apm32f0xx_tmr.h"
#include "apm32f0xx_adc.h"
#include "apm32f0xx_wwdt.h"
#include "apm32f0xx_dma.h"
#include "apm32f0xx_fmc.h"
#include "apm32f0xx_pmu.h"

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/
#include "iot_driver_clock.h"
#include "iot_driver_gpio.h"
#include "iot_driver_flash.h"



/*************************************************-*************************************************
*                                               MACRO
**************************************************-*****************^******************************/
#define IOT_ENABLE_INTERRUPTS()                                     __set_PRIMASK(0)
#define IOT_DISABLE_INTERRUPTS()                                    __set_PRIMASK(1)

#define IOT_WATCHDOG_RESET()                                        {WWDT_Reset();}

#define __no_init                                                   __attribute__((section("no_init")))


/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif

/**@} end of group Board_APM32F051_EVAL */
/**@} end of group Board */
