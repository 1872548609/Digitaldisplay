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
#include "bsp_lcd.h"
#include "bsp_key.h"

/** @addtogroup Examples
  * @brief USART RS485 examples
  @{
  */

/** @addtogroup USART_RS485
  @{
  */

/** @defgroup USART_RS485_Macros Macros
  @{
*/

/* printf using USART1  */
#define DEBUG_USART             USART1

/* max number of rx buffer */
#define RS485_RXBUF_MAX_NUM   512

/* Time out for data receive */
#define RS485_RX_TIME_OUT     1000

/**@} end of group USART_RS485_Macros*/

/** @defgroup USART_RS485_Variables Variables
  @{
  */

/* Timer tick */
uint16_t cntTick = 0;

uint8_t rxData[RS485_RXBUF_MAX_NUM];

/* Menu title string*/
const char *menuTitleStr[] = {
    "  USART RS485 Example   ",
};

/* Menu item string*/
const char *menuItemStr[] = {
    "Please send data to DEV ",
    "       Data Match       ",
    "       Data Mismatch    ",
    "       Test Error       ",
    "       Testing          ",
};

/* Menu bottom column string*/
const char *menuBottomColStr[] = {
    "   GEEHY SEMICONDUCTOR   ",
};


/**@} end of group USART_RS485_Variables*/

/** @defgroup USART_RS485_Functions Functions
  @{
  */

void APM_EVAL_Init(void);
void RS485_Test(void);
void RS485_ConfigTxRx(RS485_STATUS_T status);
void RS485_TxString(char* pStr);
void RS485_Init(uint32_t baud);
void Menu_DisplayInit(void);
void APM_EVAL_TMR14_Init(uint16_t period, uint16_t div);

/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 */
int main(void)
{
    APM_EVAL_Init();
    printf("This is an example of RS485 \r\n");
    
    /* Test */
    RS485_Test();
}

/*!
 * @brief       Init EVAL board
 *
 * @param       None
 *
 * @retval      None
 *
 */
void APM_EVAL_Init(void)
{
    /* Init delay function */
    APM_EVAL_DelayInit();
    
    APM_EVAL_LEDInit(LED1);
    APM_EVAL_LEDInit(LED2);
    APM_EVAL_LEDInit(LED3);
    APM_EVAL_LEDInit(LED4);
    APM_EVAL_PBInit(BUTTON_KEY1,BUTTON_MODE_GPIO);
    APM_EVAL_COMInit(COM1);
    
    APM_EVAL_TMR14_Init(1000,48);
    APM_LCDInit();
    LCD_Clear(0,0,LCD_WIDTH,LCD_HEIGHT,LCD_BCOLOR);
    
    Menu_DisplayInit();
    
    RS485_Init(115200);
}

/*!
 * @brief       RS485 Test
 *
 * @param       None
 *
 * @retval      None
 *
 */
void RS485_Test(void)
{
    /* RS485 Send data */
    RS485_ConfigTxRx(RS485_STATUS_TX);
    RS485_TxString("[USART2] This is a RS485 Demo.\r\n");
    RS485_TxString("[USART2] You can send data to this com, and MCU will send back.\r\n");
    RS485_ConfigTxRx(RS485_STATUS_RX);
}

/*!
 * @brief       Init RS485
 *
 * @param       baud : the baudrate for usart3
 *
 * @retval      None
 */
void RS485_Init(uint32_t baud)
{
    GPIO_Config_T gpioConfig;

    /* RS485_EN Config Rx or Tx */
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_GPIOD);
    gpioConfig.mode = GPIO_MODE_OUT;
    gpioConfig.outtype = GPIO_OUT_TYPE_PP;
    gpioConfig.pin = GPIO_PIN_2;
    gpioConfig.pupd = GPIO_PUPD_NO;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOD, &gpioConfig);
    GPIO_ClearBit(GPIOD, GPIO_PIN_2);

    /* RS485 init, use COM2*/
    APM_EVAL_COMInit(COM2);

    USART_EnableInterrupt(USART2, USART_INT_RXBNEIE);
    NVIC_EnableIRQ(USART2_IRQn);
}

/*!
 * @brief       Config RS485 to Rx or Tx.
 *
 * @param       status : Choose Send or Receive.
 *                       This parameter can be one of the following values:
 *                       @arg RS485_STATUS_TX : Send data
 *                       @arg RS485_STATUS_RX : Receive data
 *
 * @retval      None
 */
void RS485_ConfigTxRx(RS485_STATUS_T status)
{
    /* Prepare to thransfer data though RS485 */
    APM_EVAL_DelayMs(1);

    if (status == RS485_STATUS_TX)
    {
        GPIO_SetBit(GPIOD, GPIO_PIN_2);
    }
    else
    {
        GPIO_ClearBit(GPIOD, GPIO_PIN_2);
    }
}

/*!
 * @brief       RS485 Send a byte data.
 *
 * @param       data : data to send.
 *
 * @retval      None
 */
void RS485_TxByte(uint8_t data)
{
    USART_TxData(USART2, data);

    while (!USART_ReadStatusFlag(USART2, USART_FLAG_TXBE));
}

/*!
 * @brief       RS485 Send String.
 *
 * @param       pStr : pointer to the string to send.
 *
 * @retval      None
 */
void RS485_TxString(char* pStr)
{
    while (*pStr != 0)
    {
        USART_TxData(USART2, *pStr++);

        while (!USART_ReadStatusFlag(USART2, USART_FLAG_TXBE));
    }
}

/*!
 * @brief       RS485 Reveive Interrupt handler.
 *
 * @param       None
 *
 * @retval      None
 */
void RS485_RxIsr(void)
{
    uint32_t timeCnt = 0, bufIndex = 0;

    while (timeCnt < RS485_RX_TIME_OUT)
    {
        /* Keep Receive the a serie data */
        if (USART_ReadStatusFlag(USART2, USART_FLAG_RXBNE))
        {
            rxData[bufIndex++] = USART_RxData(USART2);
            timeCnt = 0;
        }
        else
        {
            timeCnt++;
        }

        /* If data more than buffer then break out */
        if (bufIndex == RS485_RXBUF_MAX_NUM)
        {
            printf("RS485 Receive buffer is full!\r\n");
            break;
        }
    }

    /* Send data */
    RS485_ConfigTxRx(RS485_STATUS_TX);

    for (uint32_t i = 0; i < bufIndex; i++)
    {
        RS485_TxByte(rxData[i]);
    }

    RS485_ConfigTxRx(RS485_STATUS_RX);
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
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return (ch);
}

/**@} end of group USART_RS485_Functions */
/**@} end of group USART_RS485 */
/**@} end of group Examples */
