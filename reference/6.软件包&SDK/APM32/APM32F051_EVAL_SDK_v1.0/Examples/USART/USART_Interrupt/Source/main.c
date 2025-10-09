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
#include <stdlib.h>
#include "bsp_lcd.h"
#include "bsp_key.h"

/** @addtogroup Examples
  * @brief USART Interrupt examples
  @{
  */

/** @addtogroup USART_Interrupt
  @{
  */

/** @defgroup USART_Interrupt_Macros Macros
  @{
*/

/* printf using USART1  */
#define DEBUG_USART           USART1

/**@} end of group USART_Interrupt_Macros*/

/** @defgroup USART_Interrupt_Variables Variables
  @{
  */

/* Timer tick */
uint16_t cntTick = 0;

/* Menu title string*/
const char *menuTitleStr[] = {
    "    USART INT Example   ",
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

/**@} end of group USART_Interrupt_Variables*/

/** @defgroup USART_Interrupt_Functions Functions
  @{
  */
void APM_EVAL_Init(void);
void USART_INT_Test(void);
void USART1_Init(uint32_t baud);
void USART_Write(USART_T* usart,uint8_t *dat);
void Menu_DisplayInit(void);
void APM_EVAL_TMR14_Init(uint16_t period, uint16_t div);

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
    printf("This is an example of USART1 interrupt operation \r\n");
    
    printf("Press KEY1 to start test.\r\n");
    
    while(1)
    {
        if(APM_EVAL_PBGetState(BUTTON_KEY1) == 0)
        {
            /* Test */
            USART_INT_Test();
            
            while(APM_EVAL_PBGetState(BUTTON_KEY1) == 0);
        }
    }
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
    /* BaudRate = 115200 baud   */
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

    /* Enable USART_Interrupt_RXBNEIE */
    USART_EnableInterrupt(USART1,USART_INT_RXBNEIE);

    NVIC_EnableIRQRequest(USART1_IRQn,2);

    /* Enable USART */
    USART_Enable(USART1);
}

/*!
 * @brief       Write data to USART
 *
 * @param       None
 *
 * @retval      None
 *
 */
void USART_INT_Test(void)
{
    LCD_DisplayString(10,LCD_LINE_2,menuItemStr[3],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
    APM_EVAL_LEDOff(LED2);
    APM_EVAL_LEDOff(LED3);
    
    USART_Write(USART1,(uint8_t*)"Hello APM EVAL Board\r\n");
    
    LCD_DisplayString(10,LCD_LINE_2,menuItemStr[1],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
}

/*!
 * @brief        serial port tramsimt data
 *
 * @param        usart:pointer to USART peripheral
 *
 * @param        data: pointer to date that need to be sent
 *
 * @retval       None
 *
 * @note
 */
void USART_Write(USART_T* usart,uint8_t *dat)
{
     /* Enable USART_INT_TXCIE*/
    USART_EnableInterrupt(usart,USART_INT_TXCIE);

    while(*dat)
    {
        while(USART_ReadStatusFlag(usart, USART_FLAG_TXBE) == RESET);

        USART_TxData(usart, *dat++);
    }

     USART_DisableInterrupt(usart,USART_INT_TXCIE);
}

/*!
 * @brief        This function handles USART1 RX interrupt Handler
 *
 * @param        None
 *
 * @retval       None
 *
 * @note        This function need to put into  void USART1_IRQHandler(void)
 */
void USART_Receive_Isr(void)
{
    uint8_t dat;

    if(USART_ReadStatusFlag(USART1, USART_FLAG_RXBNE) == SET)
    {
        dat = (uint8_t)USART_RxData(USART1);

        printf("%c",dat);
    }
}


/*!
 * @brief        This function handles USART1 TXCIE interrupt Handler
 *
 * @param        None
 *
 * @retval       None
 *
 * @note        This function need to put into  void USART1_IRQHandler(void)
 */
void USART_Send_Isr(void)
{
    while(USART_ReadStatusFlag(USART1,USART_FLAG_TXBE)==RESET);

    if(USART_ReadStatusFlag(USART1,USART_FLAG_TXC))
    {
        /* USART1 output this information*/
        printf("Send Finish\r\n");

        USART_ClearStatusFlag(USART1,USART_FLAG_TXC);
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
    
    USART1_Init(115200);
    
    APM_EVAL_TMR14_Init(1000,48);
    APM_LCDInit();
    LCD_Clear(0,0,LCD_WIDTH,LCD_HEIGHT,LCD_BCOLOR);
    
    APM_EVAL_DelayMs(100);
    
    Menu_DisplayInit();
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
    /** send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /** wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return (ch);
}

/**@} end of group USART_Interrupt_Functions */
/**@} end of group USART_Interrupt */
/**@} end of group Examples */
