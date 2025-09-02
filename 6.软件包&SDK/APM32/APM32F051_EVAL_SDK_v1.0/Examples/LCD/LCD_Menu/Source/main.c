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
  * @brief LCD Menu examples
  @{
  */

/** @addtogroup LCD_Menu
  @{
  */

/** @defgroup LCD_Menu_Macros Macros
  @{
*/

/* printf using USART1  */
#define DEBUG_USART           USART1

/**@} end of group LCD_Menu_Macros*/

/** @defgroup LCD_Menu_Variables Variables
  @{
  */

/* Timer tick */
uint16_t cntTick = 0;

MENU_INFO_T MenuTable[] = {
    /* current menu, enter, prev, next, return, function*/
    {0,0,3,1,0,(*LCD_HomePageHandle),0},
    {1,4,3,2,0,(*LCD_HomePageHandle),1},
    {2,5,1,3,0,(*LCD_HomePageHandle),2},
    {3,6,2,1,0,(*LCD_HomePageHandle),3},
    
    {4,4,4,4,1,(*LCD_MenuLevel1Handle),0},
    {5,5,5,5,2,(*LCD_MenuLevel2Handle),0},
    {6,6,6,6,3,(*LCD_MenuLevel3Handle),0},
};

/* Menu title string*/
const char *menuTitleStr[] = {
    "    SPI LCD Example     ",
    "    Menu 0 - 1 Title    ",
    "    Menu 0 - 2 Title    ",
    "    Menu 0 - 3 Title    ",
};

/* Menu item string*/
const char *menuItemStr[] = {
    " 1.Menu 0 - 1           ",
    " 2.Menu 0 - 2           ",
    " 3.Menu 0 - 3           ",
};

/* Menu bottom column string*/
const char *menuBottomColStr[] = {
    "   GEEHY SEMICONDUCTOR   ",
};

uint8_t curFuncIndex = 0;
uint8_t menuChange = 0;

/**@} end of group LCD_Menu_Variables*/

/** @defgroup LCD_Menu_Functions Functions
  @{
  */

void (*curFunction)(uint8_t param);
void APM_EVAL_Init(void);
void ADC1_Init(void);
void Menu_DisplayInit(void);
void LCD_MenuHandle(void);
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
    printf("This is an example of multi-level menu by SPI LCD \r\n");
    
    while(1)
    {
        LCD_MenuHandle();
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
    
    APM_EVAL_DelayMs(100);
    
    Menu_DisplayInit();
}

/*!
 * @brief       Initialize the LCD menu level 1 UI
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void LCD_MenuLevel1Handle(uint8_t menuState)
{
    /* Clear content*/
    LCD_Clear(0,40,LCD_WIDTH,150,LCD_BCOLOR);
    LCD_Clear(0,0,LCD_WIDTH,40,LCD_FCOLOR);
    /* Menu title*/
    LCD_Clear(0,0,LCD_WIDTH,40,LCD_FCOLOR);
    LCD_DisplayString(10,LCD_LINE_TITLE,menuTitleStr[1],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,24,0);
    
    /* Menu item*/
    LCD_DisplayString(10,LCD_LINE_1,"    Menu 0 - 1 Content   ",LCD_TEXT_FC,LCD_TEXT_BC,24,0);

    /* Menu bottom*/
    LCD_Clear(0,LCD_LINE_BOTTOM,LCD_WIDTH,LCD_HEIGHT,LCD_FCOLOR);
    LCD_DisplayString(60,LCD_LINE_BOTTOM,menuBottomColStr[0],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,16,0);
}

/*!
 * @brief       Initialize the LCD menu level 2 UI
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void LCD_MenuLevel2Handle(uint8_t menuState)
{
    /* Clear content*/
    LCD_Clear(0,40,LCD_WIDTH,150,LCD_BCOLOR);
    LCD_Clear(0,0,LCD_WIDTH,40,LCD_FCOLOR);
    /* Menu title*/
    LCD_Clear(0,0,LCD_WIDTH,40,LCD_FCOLOR);
    LCD_DisplayString(10,LCD_LINE_TITLE,menuTitleStr[2],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,24,0);
    
    /* Menu item*/
    LCD_DisplayString(10,LCD_LINE_1,"    Menu 0 - 2 Content   ",LCD_TEXT_FC,LCD_TEXT_BC,24,0);
    
    /* Menu bottom*/
    LCD_Clear(0,LCD_LINE_BOTTOM,LCD_WIDTH,LCD_HEIGHT,LCD_FCOLOR);
    LCD_DisplayString(60,LCD_LINE_BOTTOM,menuBottomColStr[0],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,16,0);
}

/*!
 * @brief       Initialize the LCD menu level 3 UI
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void LCD_MenuLevel3Handle(uint8_t menuState)
{
    /* Clear content*/
    LCD_Clear(0,40,LCD_WIDTH,150,LCD_BCOLOR);
    LCD_Clear(0,0,LCD_WIDTH,40,LCD_FCOLOR);
    /* Menu title*/
    LCD_Clear(0,0,LCD_WIDTH,40,LCD_FCOLOR);
    LCD_DisplayString(10,LCD_LINE_TITLE,menuTitleStr[3],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,24,0);
    
    /* Menu item*/
    LCD_DisplayString(10,LCD_LINE_1,"    Menu 0 - 3 Content   ",LCD_TEXT_FC,LCD_TEXT_BC,24,0);
    
    /* Menu bottom*/
    LCD_Clear(0,LCD_LINE_BOTTOM,LCD_WIDTH,LCD_HEIGHT,LCD_FCOLOR);
    LCD_DisplayString(60,LCD_LINE_BOTTOM,menuBottomColStr[0],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,16,0);
}


/*!
 * @brief       Initialize the LCD home page UI
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void LCD_HomePageHandle(uint8_t menuState)
{
    switch(menuState)
    {
        case 0:
            /* Clear content*/
            LCD_Clear(0,40,LCD_WIDTH,150,LCD_BCOLOR);
            LCD_Clear(0,0,LCD_WIDTH,40,LCD_FCOLOR);
            /* Menu title*/
            LCD_Clear(0,0,LCD_WIDTH,40,LCD_FCOLOR);
            LCD_DisplayString(10,LCD_LINE_TITLE,menuTitleStr[0],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,24,0);
            
            /* Menu item*/
            LCD_DisplayString(10,LCD_LINE_0,menuItemStr[0],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
            LCD_DisplayString(10,LCD_LINE_1,menuItemStr[1],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
            LCD_DisplayString(10,LCD_LINE_2,menuItemStr[2],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
        
            LCD_DrawLine(10,160,310,160,LCD_FCOLOR);
            /* Menu button*/
            LCD_DisplayString(30,LCD_LINE_4,"RESET",LCD_TEXT_FC,LCD_TEXT_BC,16,0);
            LCD_DisplayString(80,LCD_LINE_4,"PREV",LCD_TEXT_FC,LCD_TEXT_BC,16,0);
            LCD_DisplayString(130,LCD_LINE_4,"NEXT",LCD_TEXT_FC,LCD_TEXT_BC,16,0);
            LCD_DisplayString(180,LCD_LINE_4,"ENTER",LCD_TEXT_FC,LCD_TEXT_BC,16,0);
            LCD_DisplayString(240,LCD_LINE_4,"RETURN",LCD_TEXT_FC,LCD_TEXT_BC,16,0);
        
            /* Menu bottom*/
            LCD_Clear(0,LCD_LINE_BOTTOM,LCD_WIDTH,LCD_HEIGHT,LCD_FCOLOR);
            LCD_DisplayString(60,LCD_LINE_BOTTOM,menuBottomColStr[0],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,16,0);
            break;

        case 1:
            /* Menu item*/
            LCD_DisplayString(10,LCD_LINE_2,menuItemStr[2],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
            LCD_DisplayString(10,LCD_LINE_1,menuItemStr[1],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
            LCD_DisplayString(10,LCD_LINE_0,menuItemStr[0],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,24,0);
            break;
        
        case 2:
            /* Menu item*/
            LCD_DisplayString(10,LCD_LINE_0,menuItemStr[0],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
            LCD_DisplayString(10,LCD_LINE_2,menuItemStr[2],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
            LCD_DisplayString(10,LCD_LINE_1,menuItemStr[1],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,24,0);
            break;
        
        case 3:
            /* Menu item*/
            LCD_DisplayString(10,LCD_LINE_1,menuItemStr[1],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
            LCD_DisplayString(10,LCD_LINE_2,menuItemStr[2],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,24,0);
            LCD_DisplayString(10,LCD_LINE_0,menuItemStr[0],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
            break;
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
    /* Menu item*/
    LCD_DisplayString(10,LCD_LINE_0,menuItemStr[0],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
    LCD_DisplayString(10,LCD_LINE_1,menuItemStr[1],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
    LCD_DisplayString(10,LCD_LINE_2,menuItemStr[2],LCD_TEXT_FC,LCD_TEXT_BC,24,0);
    
    LCD_DrawLine(10,160,310,160,LCD_FCOLOR);
    /* Menu button*/
    LCD_DisplayString(30,LCD_LINE_4,"RESET",LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    LCD_DisplayString(80,LCD_LINE_4,"PREV",LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    LCD_DisplayString(130,LCD_LINE_4,"NEXT",LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    LCD_DisplayString(180,LCD_LINE_4,"ENTER",LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    LCD_DisplayString(240,LCD_LINE_4,"RETURN",LCD_TEXT_FC,LCD_TEXT_BC,16,0);
    
    /* Menu bottom*/
    LCD_Clear(0,LCD_LINE_BOTTOM,LCD_WIDTH,LCD_HEIGHT,LCD_FCOLOR);
    LCD_DisplayString(60,LCD_LINE_BOTTOM,menuBottomColStr[0],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,16,0);
}

/*!
 * @brief       LCD menu handle
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void LCD_MenuHandle(void)
{
    uint8_t keyType;
    static uint8_t menuTemp = 0;
    
    keyType = Key_Scan(KEY_MODE_SINGLE);
    
    switch(keyType)
    {
        case MENU_KEY_PREV:
            curFuncIndex = MenuTable[curFuncIndex].prevIndex;
            break;
        
        case MENU_KEY_NEXT:
            curFuncIndex = MenuTable[curFuncIndex].nextIndex;
            break;
        
        case MENU_KEY_ENTER:
            curFuncIndex = MenuTable[curFuncIndex].enterIndex;
            break;
        
        case MENU_KEY_RETURN:
            curFuncIndex = MenuTable[curFuncIndex].retIndex;
            break;
    }
    
    if(menuTemp !=  MenuTable[curFuncIndex].curMenu)
    {
        menuTemp = MenuTable[curFuncIndex].curMenu;
        
        (*MenuTable[curFuncIndex].function)(MenuTable[curFuncIndex].stateMenu);
    }
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

/**@} end of group LCD_Menu_Functions */
/**@} end of group LCD_Menu */
/**@} end of group Examples */
