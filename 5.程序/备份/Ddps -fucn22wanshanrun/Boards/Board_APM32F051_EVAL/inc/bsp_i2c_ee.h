/*!
 * @file        bsp_i2c_ee.h
 *
 * @brief       This file provides i2c read and write EEPROM(AT24C32) functions
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
#ifndef __BSP_I2C_EE_H
#define __BSP_I2C_EE_H

/* Includes */
#include "main.h"

/** @addtogroup Board
  * @brief Board support package
  @{
  */

/** @addtogroup Board_APM32F051_EVAL
  @{
  */

/** @defgroup APM32F051_EVAL_Macros Macros
  @{
*/
/* EEPROM PIN */
#define EEPROM_I2C                          I2C2
#define EEPROM_I2C_CLK                      RCM_APB1_PERIPH_I2C2

#define EEPROM_I2C_SCL_PIN                  GPIO_PIN_10
#define EEPROM_I2C_SCL_GPIO_PORT            GPIOB
#define EEPROM_I2C_SCL_GPIO_CLK             RCM_AHB_PERIPH_GPIOB
#define EEPROM_I2C_SCL_SOURCE               GPIO_PIN_SOURCE_10
#define EEPROM_I2C_SCL_AF                   GPIO_AF_PIN1

#define EEPROM_I2C_SDA_PIN                  GPIO_PIN_11
#define EEPROM_I2C_SDA_GPIO_PORT            GPIOB
#define EEPROM_I2C_SDA_GPIO_CLK             RCM_AHB_PERIPH_GPIOB
#define EEPROM_I2C_SDA_SOURCE               GPIO_PIN_SOURCE_11
#define EEPROM_I2C_SDA_AF                   GPIO_AF_PIN1

#define I2C_EE_RETRY_NUMBER_MAX             300
/* define I2C timeout */
#define I2C_EE_FLAG_TIMEOUT                 ((uint32_t)0x1000)
#define I2C_EE_LONG_TIMEOUT                 ((uint32_t)(10 * I2C_EE_FLAG_TIMEOUT))

/*
 * AT24Cxx
 */
/* #define EEPROM_TYPE_24C32 */
#define EEPROM_TYPE_24C08
#define EEPROM_ADDRESS                      0xA0
#define EEPROM_PAGE_SIZE                    8

#define EEPROM_WRITE_ADDR                   0x00
#define EEPROM_READ_ADDR                    EEPROM_WRITE_ADDR

/**@} end of group APM32F051_EVAL_Macros*/

/** @defgroup APM32F051_EVAL_Enumerates Enumerates
  @{
  */

/**
 * @brief I2C EEPROM error type
 */
typedef enum
{
    I2C_EE_OK,
    I2C_EE_ERR,
    I2C_EE_TIMEOUT,
} I2C_EE_ERROR_T;

/**@} end of group APM32F051_EVAL_Enumerates*/

/** @defgroup APM32F051_EVAL_Structures Structures
  @{
  */

/**
 * @brief I2C EEPROM information
 */
typedef struct {
    uint16_t address;
    uint8_t addrNum;
    uint16_t dataNum;
    uint32_t timeout;
    uint8_t startFlag;
} I2C_EE_INFO_T;

/**@} end of group APM32F051_EVAL_Structures*/

/** @defgroup APM32F051_EVAL_Functions Functions
  @{
  */
void APM_I2C_EE_Init(void);
I2C_EE_ERROR_T I2C_EE_BufferWrite(uint8_t* buffer, uint16_t writeAddr, uint16_t number);
I2C_EE_ERROR_T I2C_EE_BufferRead(uint8_t* buffer, uint16_t readAddr, uint16_t number);

#endif

/**@} end of group APM32F051_EVAL_Functions */
/**@} end of group Board_APM32F051_EVAL */
/**@} end of group Board */
