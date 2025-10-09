/*!
 * @file        bsp_i2c_ee.c
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

/* Includes */
#include "bsp_i2c_ee.h"
#include <stdio.h>
#include <string.h>

/** @addtogroup Board
  * @brief Board support package
  @{
  */

/** @addtogroup Board_APM32F051_EVAL
  * @brief APM32F051 EVAL board support package
  @{
  */

/** @defgroup APM32F051_EVAL_Variables Variables
  @{
  */

I2C_EE_INFO_T eeI2CInfo;
/**@} end of group APM32F051_EVAL_Variables*/

/** @defgroup APM32F051_EVAL_Functions Functions
  @{
  */
static I2C_EE_ERROR_T I2C_EE_Callback(uint8_t errorCode);

/*!
 * @brief       EEPROM I2C Initialization
 *
 * @param       None
 *
 * @retval      None
 */
void APM_I2C_EE_Init(void)
{
    GPIO_Config_T gpioConfigStruct;
    I2C_Config_T i2cConfigStruct;
    
    /* Enable I2C related Clock */
    RCM_EnableAHBPeriphClock(EEPROM_I2C_SCL_GPIO_CLK | EEPROM_I2C_SDA_GPIO_CLK);
    RCM_EnableAPB1PeriphClock(EEPROM_I2C_CLK);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);

    /* Connect I2C to SCL */
    GPIO_ConfigPinAF(EEPROM_I2C_SCL_GPIO_PORT, EEPROM_I2C_SCL_SOURCE, EEPROM_I2C_SCL_AF);
    /* Connect I2C to SDA */
    GPIO_ConfigPinAF(EEPROM_I2C_SDA_GPIO_PORT, EEPROM_I2C_SDA_SOURCE, EEPROM_I2C_SDA_AF);
    /* Config I2C GPIO */
    gpioConfigStruct.mode = GPIO_MODE_AF;
    gpioConfigStruct.speed = GPIO_SPEED_50MHz;
    gpioConfigStruct.outtype = GPIO_OUT_TYPE_OD;
    gpioConfigStruct.pupd = GPIO_PUPD_NO;
    gpioConfigStruct.pin = EEPROM_I2C_SCL_PIN;
    GPIO_Config(EEPROM_I2C_SCL_GPIO_PORT, &gpioConfigStruct);

    gpioConfigStruct.pin = EEPROM_I2C_SDA_PIN;
    GPIO_Config(EEPROM_I2C_SDA_GPIO_PORT, &gpioConfigStruct);

    /*  Config I2C2 */
    I2C_Reset(EEPROM_I2C);

    i2cConfigStruct.ack = I2C_ACK_ENABLE;
    i2cConfigStruct.ackaddress = I2C_ACK_ADDRESS_7BIT;
    i2cConfigStruct.address1 = 0x00;
    i2cConfigStruct.analogfilter = I2C_ANALOG_FILTER_ENABLE;
    i2cConfigStruct.digitalfilter = I2C_DIGITAL_FILTER_0;
    i2cConfigStruct.mode = I2C_MODE_I2C;
    i2cConfigStruct.timing = 0x00210507;
    I2C_Config(EEPROM_I2C,&i2cConfigStruct);

    /* Enable I2Cx */
    I2C_Enable(EEPROM_I2C);
    
    eeI2CInfo.address = EEPROM_ADDRESS;
    
#ifdef  EEPROM_TYPE_24C32
    eeI2CInfo.addrNum = 2;
#else
    eeI2CInfo.addrNum = 1;
#endif
}

/*!
 * @brief       I2C write page data to eeprom
 *
 * @param       buffer: pointer to the read buffer 
 *
 * @param       writeAddr: write the flash address
 *
 * @param       number: the number of byte to write
 *
 * @retval      I2C status code
 */
I2C_EE_ERROR_T I2C_EE_PageWrite(uint8_t* buffer, uint16_t writeAddr, uint8_t* number)
{
    I2C_EE_ERROR_T errSta = I2C_EE_OK;
    uint32_t temp = 0;
    
    /* I2Cx communication start transfer */
    I2C_HandlingTransfer(EEPROM_I2C, eeI2CInfo.address, eeI2CInfo.addrNum, I2C_RELOAD_MODE_RELOAD, I2C_GENERATE_START_WRITE);
    
    eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
    while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_TXINT) == RESET)
    {
        if((eeI2CInfo.timeout--) == 0)
        {
            errSta = I2C_EE_TIMEOUT;
            return I2C_EE_Callback(1);
        }
    }

#ifdef EEPROM_TYPE_24C32
    /* I2Cx send MSB of WriteAddr to eeprom */
    I2C_TxData(EEPROM_I2C, (uint8_t)((writeAddr & 0xFF00) >> 8));
    
    eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
    while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_TXINT) == RESET)
    {
        if((eeI2CInfo.timeout--) == 0)
        {
            errSta = I2C_EE_TIMEOUT;
            return I2C_EE_Callback(2);
        }
    }
#endif

    /* I2Cx send LSB of WriteAddr to eeprom */
    I2C_TxData(EEPROM_I2C, (uint8_t)(writeAddr & 0x00FF));
    
    eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
    while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_TCRF) == RESET)
    {
        if((eeI2CInfo.timeout--) == 0)
        {
            errSta = I2C_EE_TIMEOUT;
            return I2C_EE_Callback(3);
        }
    }

    /* I2Cx send the number of byte to write to eeprom */
    I2C_HandlingTransfer(EEPROM_I2C, eeI2CInfo.address, (uint8_t)(*number), I2C_RELOAD_MODE_AUTOEND, I2C_GENERATE_NO_STARTSTOP);
    
    while(temp != (*number))
    {
        eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
        while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_TXINT) == RESET)
        {
            if((eeI2CInfo.timeout--) == 0)
            {
                errSta = I2C_EE_TIMEOUT;
                return I2C_EE_Callback(4);
            }
        }
        
        /* I2Cx send byte to write eeprom */
        I2C_TxData(EEPROM_I2C, (uint8_t)(buffer[temp]));
        temp++;
    }
    
    eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
    while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_STOP) == RESET)
    {
        if((eeI2CInfo.timeout--) == 0)
        {
            errSta = I2C_EE_TIMEOUT;
            return I2C_EE_Callback(5);
        }
    } 
    
    I2C_ClearStatusFlag(EEPROM_I2C, I2C_FLAG_STOP);
    
    return errSta;
}

/*!
 * @brief       Wait for EEPROM standby
 *
 * @param       None
 *
 * @retval      I2C status code
 */
I2C_EE_ERROR_T I2C_EE_WaitEEStandby(void)
{
    uint32_t retryTemp = 0;
    
    I2C_EE_ERROR_T errSta = I2C_EE_OK;
    
    I2C_HandlingTransfer(EEPROM_I2C, eeI2CInfo.address, 0, I2C_RELOAD_MODE_AUTOEND, I2C_GENERATE_NO_STARTSTOP);
    
    while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_NACK) != RESET)
    {
        eeI2CInfo.timeout = I2C_EE_FLAG_TIMEOUT;
        
        /* Clear NACK and STOP*/
        I2C_ClearStatusFlag(EEPROM_I2C, I2C_FLAG_NACK | I2C_FLAG_STOP);
        
        /* Generate start */
        I2C_EnableGenerateStart(EEPROM_I2C);
        
        while(eeI2CInfo.timeout-- != 0);
        
        if(retryTemp++ == I2C_EE_RETRY_NUMBER_MAX)
        {
            errSta = I2C_EE_ERR;
            return errSta;
        }
    }
    
    I2C_ClearStatusFlag(EEPROM_I2C, I2C_FLAG_STOP);
    
    return errSta;
}

/*!
 * @brief       EEPROM Write Buffer
 *
 * @param       buffer: pointer to the Write buffer
 *
 * @param       writeAddr: Write the flash Address
 *
 * @param       number: the number of byte to write
 *
 * @retval      I2C status code
 */
I2C_EE_ERROR_T I2C_EE_BufferWrite(uint8_t* buffer, uint16_t writeAddr, uint16_t number)
{
    I2C_EE_ERROR_T errSta = I2C_EE_OK;
    
    uint16_t pageNumber = 0;
    uint16_t remainNumber = 0;
    uint16_t addr = 0;
    uint16_t pageCnt = 0;
    
    /* complete Page need to write */
    pageNumber =  number / EEPROM_PAGE_SIZE;

    /* Remain byte */
    remainNumber = number % EEPROM_PAGE_SIZE;
    addr = writeAddr % EEPROM_PAGE_SIZE;
    pageCnt = EEPROM_PAGE_SIZE - addr;
    
    if(addr == 0)
    {
        if(pageNumber == 0)
        {
            eeI2CInfo.dataNum = remainNumber;
            errSta = I2C_EE_PageWrite(buffer, writeAddr, (uint8_t*)(&eeI2CInfo.dataNum));
            I2C_EE_WaitEEStandby();
            
            if(errSta != I2C_EE_OK)
            {
                return I2C_EE_ERR;
            }
            
        }
        else
        {
            while(pageNumber--)
            {
                eeI2CInfo.dataNum = EEPROM_PAGE_SIZE;
                errSta = I2C_EE_PageWrite(buffer, writeAddr, (uint8_t*)(&eeI2CInfo.dataNum));
                I2C_EE_WaitEEStandby();
                
                /* Need add some delay in here*/
                APM_EVAL_DelayMs(3);
                
                if(errSta != I2C_EE_OK)
                {
                    return I2C_EE_ERR;
                }
                
                writeAddr += EEPROM_PAGE_SIZE;
                buffer += EEPROM_PAGE_SIZE;
            }
            
            if(remainNumber != 0)
            {
                eeI2CInfo.dataNum = remainNumber;
                errSta = I2C_EE_PageWrite(buffer, writeAddr, (uint8_t*)(&eeI2CInfo.dataNum));
                I2C_EE_WaitEEStandby();
                
                if(errSta != I2C_EE_OK)
                {
                    return I2C_EE_ERR;
                }
            }
        }
    }
    else
    {
        if(pageNumber == 0)
        {
            if(number > pageCnt)
            {
                eeI2CInfo.dataNum = pageCnt;
                errSta = I2C_EE_PageWrite(buffer, writeAddr, (uint8_t*)(&eeI2CInfo.dataNum));
                I2C_EE_WaitEEStandby();
                
                if(errSta != I2C_EE_OK)
                {
                    return I2C_EE_ERR;
                }
                
                eeI2CInfo.dataNum = (number - pageCnt);
                errSta = I2C_EE_PageWrite(buffer, writeAddr, (uint8_t*)(&eeI2CInfo.dataNum));
                I2C_EE_WaitEEStandby();
                
                if(errSta != I2C_EE_OK)
                {
                    return I2C_EE_ERR;
                }
            }
            else
            {
                eeI2CInfo.dataNum = remainNumber;
                errSta = I2C_EE_PageWrite(buffer, writeAddr, (uint8_t*)(&eeI2CInfo.dataNum));
                I2C_EE_WaitEEStandby();
                
                if(errSta != I2C_EE_OK)
                {
                    return I2C_EE_ERR;
                }
            }
        }
        else
        {
            number -= pageCnt;
            /* complete Page need to write */
            pageNumber =  number / EEPROM_PAGE_SIZE;

            /* Remain byte */
            remainNumber = number % EEPROM_PAGE_SIZE;
            
            if(pageCnt != 0)
            {
                eeI2CInfo.dataNum = pageCnt;
                errSta = I2C_EE_PageWrite(buffer, writeAddr, (uint8_t*)(&eeI2CInfo.dataNum));
                I2C_EE_WaitEEStandby();
                
                if(errSta != I2C_EE_OK)
                {
                    return I2C_EE_ERR;
                }
                
                writeAddr += pageCnt;
                buffer += pageCnt;
            }
            
            while(remainNumber--)
            {
                eeI2CInfo.dataNum = EEPROM_PAGE_SIZE;
                errSta = I2C_EE_PageWrite(buffer, writeAddr, (uint8_t*)(&eeI2CInfo.dataNum));
                
                eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
                I2C_EE_WaitEEStandby();
                
                if(errSta != I2C_EE_OK)
                {
                    return I2C_EE_ERR;
                }
                
                writeAddr += EEPROM_PAGE_SIZE;
                buffer += EEPROM_PAGE_SIZE;
            }
            
            if(remainNumber != 0)
            {
                eeI2CInfo.dataNum = remainNumber;
                errSta = I2C_EE_PageWrite(buffer, writeAddr, (uint8_t*)(&eeI2CInfo.dataNum));
                I2C_EE_WaitEEStandby();
                
                if(errSta != I2C_EE_OK)
                {
                    return I2C_EE_ERR;
                }
            }
        }
    }
    
    return errSta;
}

/*!
 * @brief       EEPROM read buffer
 *
 * @param       buffer: pointer to the Write buffer
 *
 * @param       readAddr: read the flash Address
 *
 * @param       number: the number of byte to read
 *
 * @retval      I2C status code
 */
I2C_EE_ERROR_T I2C_EE_BufferRead(uint8_t* buffer, uint16_t readAddr, uint16_t number)
{
    I2C_EE_ERROR_T errSta = I2C_EE_OK;
    
    uint32_t remainNumber = 0;
    uint32_t pageCnt = 0;
    uint32_t temp = 0;
    
    pageCnt = number / 255;
    remainNumber = number % 255;
    
    /* I2Cx communication start transfer */
    I2C_HandlingTransfer(EEPROM_I2C, eeI2CInfo.address, eeI2CInfo.addrNum, I2C_RELOAD_MODE_SOFTEND, I2C_GENERATE_START_WRITE);
    
    eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
    while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_TXINT) == RESET)
    {
        if((eeI2CInfo.timeout--) == 0)
        {
            errSta = I2C_EE_TIMEOUT;
            return I2C_EE_Callback(11);
        }
    }
    
#ifdef  EEPROM_TYPE_24C32
    /* I2Cx send MSB of WriteAddr to eeprom */
    I2C_TxData(EEPROM_I2C, (uint8_t)((readAddr & 0xFF00) >> 8));
    
    eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
    while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_TXINT) == RESET)
    {
        if((eeI2CInfo.timeout--) == 0)
        {
            errSta = I2C_EE_TIMEOUT;
            return I2C_EE_Callback(12);
        }
    }
#endif
    /* I2Cx send LSB of WriteAddr to eeprom */
    I2C_TxData(EEPROM_I2C, (uint8_t)(readAddr & 0x00FF));
    
    eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
    while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_TXCF) == RESET)
    {
        if((eeI2CInfo.timeout--) == 0)
        {
            errSta = I2C_EE_TIMEOUT;
            return I2C_EE_Callback(13);
        }
    }
    
    if(pageCnt != 0)
    {
        eeI2CInfo.startFlag = 1;
        
        while(pageCnt != 0)
        {
            if(eeI2CInfo.startFlag == 0)
            {
                eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
                while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_TCRF) == RESET)
                {
                    if((eeI2CInfo.timeout--) == 0)
                    {
                        errSta = I2C_EE_TIMEOUT;
                        return I2C_EE_Callback(14);
                    }
                }
            }
        
            if((pageCnt == 1) && (remainNumber == 0))
            {
                if(eeI2CInfo.startFlag != 0)
                {
                    /* I2Cx communication start transfer */
                    I2C_HandlingTransfer(EEPROM_I2C, eeI2CInfo.address, 255, I2C_RELOAD_MODE_AUTOEND, I2C_GENERATE_START_READ);
                }
                else
                {
                    /* I2Cx communication start transfer */
                    I2C_HandlingTransfer(EEPROM_I2C, eeI2CInfo.address, 255, I2C_RELOAD_MODE_AUTOEND, I2C_GENERATE_NO_STARTSTOP);
                }
            }
            else
            {
                if(eeI2CInfo.startFlag != 0)
                {
                    I2C_HandlingTransfer(EEPROM_I2C, eeI2CInfo.address, 255, I2C_RELOAD_MODE_RELOAD, I2C_GENERATE_START_READ);
                }
                else
                {
                    I2C_HandlingTransfer(EEPROM_I2C, eeI2CInfo.address, 255, I2C_RELOAD_MODE_RELOAD, I2C_GENERATE_NO_STARTSTOP);
                }
            }
        
            eeI2CInfo.startFlag = 0;
            temp = 0;
            
            while(temp != 255)
            {
                eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
                while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_RXBNE) == RESET)
                {
                    if((eeI2CInfo.timeout--) == 0)
                    {
                        errSta = I2C_EE_TIMEOUT;
                        return I2C_EE_Callback(15);
                    }
                }
                
                buffer[temp] = I2C_RxData(EEPROM_I2C);
                
                temp++;
                number--;
            }
            
            buffer += temp;
            
            pageCnt--;
        }
        
        if(remainNumber != 0)
        {
            eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
            while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_TCRF) == RESET)
            {
                if((eeI2CInfo.timeout--) == 0)
                {
                    errSta = I2C_EE_TIMEOUT;
                    return I2C_EE_Callback(16);
                }
            }
            
            I2C_HandlingTransfer(EEPROM_I2C, eeI2CInfo.address, (uint8_t)number, I2C_RELOAD_MODE_AUTOEND, I2C_GENERATE_NO_STARTSTOP);
            
            temp = 0;
            
            while(temp != remainNumber)
            {
                eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
                while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_RXBNE) == RESET)
                {
                    if((eeI2CInfo.timeout--) == 0)
                    {
                        errSta = I2C_EE_TIMEOUT;
                        return I2C_EE_Callback(17);
                    }
                }
                
                buffer[temp] = I2C_RxData(EEPROM_I2C);
                
                temp++;
                number--;
            }
        }
    }
    else
    {
        I2C_HandlingTransfer(EEPROM_I2C, eeI2CInfo.address, (uint32_t)remainNumber, I2C_RELOAD_MODE_AUTOEND, I2C_GENERATE_START_READ);
        
        temp = 0;
        
        while(temp != remainNumber)
        {
            eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
            while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_RXBNE) == RESET)
            {
                if((eeI2CInfo.timeout--) == 0)
                {
                    errSta = I2C_EE_TIMEOUT;
                    return I2C_EE_Callback(18);
                }
            }
            
            buffer[temp] = I2C_RxData(EEPROM_I2C);
            
            temp++;
            number--;
        }
    }
    
    eeI2CInfo.timeout = I2C_EE_LONG_TIMEOUT;
    while(I2C_ReadStatusFlag(EEPROM_I2C, I2C_FLAG_STOP) == RESET)
    {
        if((eeI2CInfo.timeout--) == 0)
        {
            errSta = I2C_EE_TIMEOUT;
            return I2C_EE_Callback(19);
        }
    }
    
    I2C_ClearStatusFlag(EEPROM_I2C, I2C_FLAG_STOP);
    
    return errSta;
}

/*!
 * @brief       I2C eeprom error callback
 *
 * @param       errorCode: print this error code
 *
 * @retval      I2C status code
 */
static I2C_EE_ERROR_T I2C_EE_Callback(uint8_t errorCode)
{
    I2C_EE_ERROR_T errSta = I2C_EE_ERR;
    
    /* Block communication and all processes */
    printf("I2C wait timeout! errorCode = %d\r\n",errorCode);
    
    return errSta;
}

/**@} end of group APM32F051_EVAL_Functions */
/**@} end of group Board_APM32F051_EVAL */
/**@} end of group Board */
