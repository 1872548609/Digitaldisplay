/*!
 * @file        main.c
 *
 * @brief       Main program body
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
#include "main.h"
#include <stdio.h>
#include <string.h>
#include "bsp_i2c_ee.h"
#include "bsp_lcd.h"
#include "bsp_key.h"

/** @addtogroup Examples
  * @brief I2C_EEPROM examples
  @{
  */

/** @addtogroup I2C_EEPROM
  @{
  */

/** @defgroup I2C_EEPROM_Macros Macros
  @{
*/

/* printf using USART1  */
#define DEBUG_USART           USART1

/* Buffer Size */
#define DATA_BUF_SIZE         (255)
/**@} end of group I2C_EEPROM_Macros*/

/** @defgroup I2C_EEPROM_Variables Variables
  @{
  */

uint8_t txDataBufI2C[DATA_BUF_SIZE] = {0};
uint8_t rxDataBufI2C[DATA_BUF_SIZE] = {0};

/* Timer tick */
uint16_t cntTick = 0;

/* Menu title string*/
const char *menuTitleStr[] = {
    "    I2C EEPROM Example   ",
};

/* Menu item string*/
const char *menuItemStr[] = {
    "Press KEY1 to start test",
    "       Test OK          ",
    "       Test Error       ",
    "       Testing          ",
};

/* Menu bottom column string*/
const char *menuBottomColStr[] = {
    "   GEEHY SEMICONDUCTOR   ",
};

/**@} end of group I2C_EEPROM_Variables*/

/** @defgroup I2C_EEPROM_Functions Functions
  @{
  */

void APM_EVAL_Init(void);
/* Test EEPROM */
void I2C_EE_Test(void);
void Menu_DisplayInit(void);
void APM_EVAL_TMR14_Init(uint16_t period, uint16_t div);
/* compare Buffer */
uint8_t Buffer_Compare(uint8_t* buf1, uint8_t* buf2, uint16_t size);
void Buffer_Print(uint8_t* pBuffer, uint16_t BufferLength);

/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
int main(void)
{
    APM_EVAL_Init();
    printf("This is an example of AT24C32 read and write operation \r\n");
    
    printf("Press KEY1 to Read and Write EEPROM AT24C32 by I2C.\r\n");
    
    while (1)
    {
        if (APM_EVAL_PBGetState(BUTTON_KEY1) == 0)
        {
            /* I2C EEPROM test */
            I2C_EE_Test();
            
            while(APM_EVAL_PBGetState(BUTTON_KEY1) == 0);
        }
    }
}

/*!
 * @brief       Init EVAL board
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void APM_EVAL_Init(void)
{
    /* Init delay function */
    APM_EVAL_DelayInit();
    
    APM_EVAL_LEDInit(LED1);
    APM_EVAL_LEDInit(LED2);
    APM_EVAL_LEDInit(LED3);
    APM_EVAL_PBInit(BUTTON_KEY1,BUTTON_MODE_GPIO);
    APM_EVAL_COMInit(COM1);
    
    APM_EVAL_TMR14_Init(1000,48);
    APM_LCDInit();
    LCD_Clear(0,0,LCD_WIDTH,LCD_HEIGHT,LCD_BCOLOR);
    
    APM_I2C_EE_Init();
    
    Menu_DisplayInit();
}

/*!
 * @brief       Write and read EEPROM
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void I2C_EE_Test(void)
{
    I2C_EE_ERROR_T errSta = I2C_EE_OK;
    
    uint16_t i = 0;

    LCD_DisplayString(10,LCD_LINE_2,menuItemStr[3],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
    APM_EVAL_LEDOff(LED2);
    APM_EVAL_LEDOff(LED3);
    
    /* initialization  Buffer*/
    for (i = 0; i < DATA_BUF_SIZE; i++)
    {
        txDataBufI2C[i] = i;
        rxDataBufI2C[i] = 0;
    }
    
    printf("\r\nI2C write Data to EEPROM \r\n");

    /* Write data to EEPROM */
    errSta = I2C_EE_BufferWrite(txDataBufI2C, EEPROM_WRITE_ADDR, DATA_BUF_SIZE);
    
    if(errSta == I2C_EE_OK)
    {
        printf("Write data: \r\n");
        Buffer_Print(txDataBufI2C, DATA_BUF_SIZE);
        
        /* Read data from EEPROM */
        errSta = I2C_EE_BufferRead(rxDataBufI2C, EEPROM_READ_ADDR, DATA_BUF_SIZE);
        
        if(errSta == I2C_EE_OK)
        {
            printf("\r\nRead data: \r\n");
            Buffer_Print(rxDataBufI2C, DATA_BUF_SIZE);
        }
        else
        {
            printf("I2C EE Error Code:%d\r\n",errSta);
        }
    }
    else
    {
        printf("I2C EE Error Code:%d\r\n",errSta);
    }

    /* Compare receive Buffer */
    if (Buffer_Compare(txDataBufI2C, rxDataBufI2C, DATA_BUF_SIZE) == TRUE)
    {
        /* Data is ok then turn on LED2 */
        APM_EVAL_LEDOn(LED2);
        APM_EVAL_LEDOff(LED3);
        LCD_DisplayString(10,LCD_LINE_2,menuItemStr[1],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
        printf("\r\nEEPROM AT24C32 test OK! LED2 on\n\r");
    }
    else
    {
        APM_EVAL_LEDOff(LED2);
        APM_EVAL_LEDOn(LED3);
        LCD_DisplayString(10,LCD_LINE_2,menuItemStr[2],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
        printf("\r\nEEPROM AT24C32 test fail! LED3 on\n\r");
    }
}

/*!
 * @brief       Initialize the Menu UI
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void Menu_DisplayInit(void)
{
    /* Menu title*/
    LCD_Clear(0,0,LCD_WIDTH,40,LCD_FCOLOR);
    LCD_DisplayString(10,LCD_LINE_TITLE,menuTitleStr[0],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,24,0);
    
    /* Menu item*/
    LCD_DisplayString(10,LCD_LINE_2,menuItemStr[0],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
    
    /* Menu bottom*/
    LCD_Clear(0,LCD_LINE_BOTTOM,LCD_WIDTH,LCD_HEIGHT,LCD_FCOLOR);
    LCD_DisplayString(60,LCD_LINE_BOTTOM,menuBottomColStr[0],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,16,0);
}

/*!
 * @brief       TMR14 init
 *
 * @param       None
 *
 * @retval      None
 *
 */
void APM_EVAL_TMR14_Init(uint16_t period, uint16_t div)
{
    TMR_TimeBase_T  timeBaseConfig;

    /* Enable Clock*/
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR14);

    /* Set clockDivision = 1 */
    timeBaseConfig.clockDivision =  TMR_CKD_DIV1;
    /* Up-counter */
    timeBaseConfig.counterMode =  TMR_COUNTER_MODE_UP;
    /* Set divider */
    timeBaseConfig.div = div - 1 ;
    /* Set counter = 0xffff */
    timeBaseConfig.period = period - 1;
    /* Repetition counter = 0x0 */
    timeBaseConfig.repetitionCounter =  0;

    TMR_ConfigTimeBase(TMR14,&timeBaseConfig);

    /* Enable update interrupt*/
    TMR_EnableInterrupt(TMR14,TMR_INT_UPDATE);
    NVIC_EnableIRQRequest(TMR14_IRQn,2);

    /*  Enable TMR14  */
    TMR_Enable(TMR14);
}

/*!
 * @brief       TMR4 interrupt service runtine
 *
 * @param       None
 *
 * @retval      None
 *
 * @note        This function need to put into TMR14_IRQHandler() in apm32f0xx_int.c
 */
void TMR14_Isr(void)
{
    if(TMR_ReadIntFlag(TMR14,TMR_INT_FLAG_UPDATE) == SET)
    {
        TMR_ClearIntFlag(TMR14,TMR_INT_FLAG_UPDATE);
        cntTick++;
        
        if(cntTick >= 500)
        {
            cntTick = 0;
            APM_EVAL_LEDToggle(LED1);
        }
    }
}

/*!
 * @brief       Compares two buffers
 *
 * @param       buf1:    First buffer to be compared
 *
 * @param       buf1:    Second buffer to be compared
 *
 * @param       size:    Buffer size
 *
 * @retval      Return TRUE if buf1 = buf2. If not then return FALSE
 */
uint8_t Buffer_Compare(uint8_t* buf1, uint8_t* buf2, uint16_t size)
{
    uint16_t i;

    for (i = 0; i < size; i++)
    {
        if (buf1[i] != buf2[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*!
 * @brief       Print Buffer Data
 *
 * @param       pBuffer:buffer
 *
 * @param       length : length of the Buffer
 *
 * @retval      None
 */
void Buffer_Print(uint8_t* pBuffer, uint16_t BufferLength)
{
    uint16_t i;

    for (i=0; i  < BufferLength; i++)
    {
        printf("0x%02X  ", pBuffer[i]);

        if (( i + 1 ) % 8 == 0)
        {
            printf("\r\n");
        }
    }
}

/*!
 * @brief       Redirect C Library function printf to serial port.
 *              After Redirection, you can use printf function.
 *
 * @param       ch:  The characters that need to be send.
 *
 * @param       *f:  pointer to a FILE that can recording all information
 *              needed to control a stream
 *
 * @retval      The characters that need to be send.
 */
int fputc(int ch, FILE* f)
{
    /** send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /** wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return (ch);
}

/**@} end of group I2C_EEPROM_Functions */
/**@} end of group I2C_EEPROM */
/**@} end of group Examples */
