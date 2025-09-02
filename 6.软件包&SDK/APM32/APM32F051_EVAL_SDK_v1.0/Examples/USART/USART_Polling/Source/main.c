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
  * @brief USART Polling examples
  @{
  */

/** @addtogroup USART_Polling
  @{
  */

/** @defgroup USART_Polling_Macros Macros
  @{
*/

/* printf using USART1  */
#define DEBUG_USART             USART1

#define DATA_BUF_SIZE           (6)

/**@} end of group USART_Polling_Macros*/

/** @defgroup USART_Polling_Variables Variables
  @{
  */

/* Timer tick */
uint16_t cntTick = 0;

/* Menu title string*/
const char *menuTitleStr[] = {
    "  USART Polling Example ",
};

/* Menu item string*/
const char *menuItemStr[] = {
    "Press KEY1 to start test",
    "       Data Match       ",
    "       Data Mismatch    ",
    "       Test Error       ",
    "       Testing          ",
};

/* Menu bottom column string*/
const char *menuBottomColStr[] = {
    "   GEEHY SEMICONDUCTOR   ",
};

/* Receive buffer*/
uint8_t rxDataBuf[DATA_BUF_SIZE] = {0};
/* Transmit buffer*/
uint8_t txDataBuf[DATA_BUF_SIZE] = {0};

/**@} end of group USART_Polling_Variables*/

/** @defgroup USART_Polling_Functions Functions
  @{
  */

void APM_EVAL_Init(void);
void USART1_Polling_Test(void);
void USART1_Init(uint32_t baud);
void USART_Write(USART_T* usart,uint8_t *dat);
void Menu_DisplayInit(void);
void APM_EVAL_TMR14_Init(uint16_t period, uint16_t div);
BOOL BufferCompare(uint8_t *buf1, uint8_t *buf2, uint8_t size);

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
    printf("This is an example of USART1 polling operation \r\n");
    
    printf("Press KEY1 to start test.\r\n");
    
    while (1)
    {
        if (APM_EVAL_PBGetState(BUTTON_KEY1) == 0)
        {
            /* Test */
            USART1_Polling_Test();
            
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
    
    USART1_Init(115200);
    
    APM_EVAL_TMR14_Init(1000,48);
    APM_LCDInit();
    LCD_Clear(0,0,LCD_WIDTH,LCD_HEIGHT,LCD_BCOLOR);
    
    Menu_DisplayInit();
}

/*!
 * @brief       Init USART1
 *
 * @param       baud:baudrate
 *
 * @retval      None
 *
 */
void USART1_Init(uint32_t baud)
{
    USART_Config_T usartConfigStruct;
    GPIO_Config_T gpioConfigStruct;
    
    /* Enable GPIO clock */
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_GPIOA);
    
    /* Enable USART1 clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_USART1);
    
    /* Connect PXx to USARTx_Tx */
    GPIO_ConfigPinAF(GPIOA,GPIO_PIN_SOURCE_9,GPIO_AF_PIN1);

    /* Connect PXx to USARRX_Rx */
    GPIO_ConfigPinAF(GPIOA,GPIO_PIN_SOURCE_10,GPIO_AF_PIN1);
    
    /* Configure USART Tx as alternate function push-pull */
    gpioConfigStruct.mode = GPIO_MODE_AF;
    gpioConfigStruct.pin = GPIO_PIN_9;
    gpioConfigStruct.speed = GPIO_SPEED_50MHz;
    gpioConfigStruct.outtype = GPIO_OUT_TYPE_PP;
    gpioConfigStruct.pupd = GPIO_PUPD_PU;
    GPIO_Config(GPIOA, &gpioConfigStruct);
    
    /* Configure USART Rx as input floating */
    gpioConfigStruct.pin  = GPIO_PIN_10;
    GPIO_Config(GPIOA, &gpioConfigStruct);
    
    /* USARTs configured as follow: */
    /* BaudRate */
    usartConfigStruct.baudRate = baud;
    /* Receive and transmit enabled  */
    usartConfigStruct.mode     = USART_MODE_TX_RX;
    /* Hardware flow control disabled (RTS and CTS signals) */
    usartConfigStruct.hardwareFlowCtrl = USART_FLOW_CTRL_NONE;
    /* No parity */
    usartConfigStruct.parity   = USART_PARITY_NONE;
    /* One Stop Bit */
    usartConfigStruct.stopBits =  USART_STOP_BIT_1;
    /* Word Length = 8 Bits  */
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;
    /* USART_Config */
    USART_Config(USART1, &usartConfigStruct);

    /* Enable USART */
    USART_Enable(USART1);
}

/*!
 * @brief       USART Write and receive data
 *
 * @param       None
 *
 * @retval      None
 *
 */
void USART1_Polling_Test(void)
{
    uint16_t i;
    
    LCD_DisplayString(10,LCD_LINE_2,menuItemStr[3],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
    APM_EVAL_LEDOff(LED2);
    APM_EVAL_LEDOff(LED3);
    
    /* Init txDataBuf */
    for(i = 0; i < DATA_BUF_SIZE; i++)
    {
        txDataBuf[i] = 0x31 + i;
    }
    
    printf("Device send data:\r\n");
    for(i = 0; i < DATA_BUF_SIZE; i++)
    {
        /* Wait until end of transmission */
        while(USART_ReadStatusFlag(USART1, USART_FLAG_TXBE) == RESET);
        USART_TxData(USART1, txDataBuf[i]);
    }
    printf("\r\n\r\n");
    printf("Please send the some data \"123456\" to device\r\n");
    LCD_DisplayString(10,LCD_LINE_2,"Please send data to DEV ",LCD_TEXT_FC,LCD_TEXT_BC,24,0);
    
    for(i = 0; i < DATA_BUF_SIZE; i++)
    {
        /* Wait until end of reception */
        while(USART_ReadStatusFlag(USART1, USART_FLAG_RXBNE) == RESET);
        rxDataBuf[i] = USART_RxData(USART1);
    }
    
    printf("Checking data\r\n");
    
    /* Compare receive Buffer */
    if(BufferCompare(rxDataBuf, txDataBuf, DATA_BUF_SIZE) == TRUE)
    {
        APM_EVAL_LEDOn(LED2);
        APM_EVAL_LEDOff(LED3);
        LCD_DisplayString(10,LCD_LINE_2,menuItemStr[1],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
        printf("Data is match! LED2 on\n\r");
    }
    else
    {
        APM_EVAL_LEDOff(LED2);
        APM_EVAL_LEDOn(LED3);
        LCD_DisplayString(10,LCD_LINE_2,menuItemStr[2],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
        printf("Data mismatch! LED3 on\n\r");
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
 * @brief       Compares two buffers
 *
 * @param       buf1:    First buffer to be compared
 *
 * @param       buf1:    Second buffer to be compared
 *
 * @param       size:    Buffer size
 *
 * @retval      Return TRUE if buf1 = buf2. If not then return FALSE
 *
 * @note
 */
BOOL BufferCompare(uint8_t *buf1, uint8_t *buf2, uint8_t size)
{
    uint8_t i;

    for(i = 0; i < size; i++)
    {
        if(buf1[i] != buf2[i])
        {
            return FALSE;
        }
    }

    return TRUE;
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

/**@} end of group USART_Polling_Functions */
/**@} end of group USART_Polling */
/**@} end of group Examples */
