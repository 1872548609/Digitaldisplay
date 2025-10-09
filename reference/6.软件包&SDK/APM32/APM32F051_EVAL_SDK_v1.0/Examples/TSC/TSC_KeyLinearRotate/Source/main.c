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
#include "tsc_user.h"
#include "bsp_lcd.h"
#include <stdio.h>

/** @addtogroup Examples
  * @brief TSC touch examples
  @{
  */

/** @addtogroup TSC_KeyLinearRotate
  @{
  */

/** @defgroup TSC_KeyLinearRotate_Macros Macros
  @{
*/

/* USART Macros*/
#define DEBUG_USART         USART1

#define TOUCHKEY_PRESS(Num) ((MyTouchKeys[(Num)].p_Data->StateId == TSC_STATEID_DETECT))
#define TOUCHKEY_RELEASE(Num) ((MyTouchKeys[(Num)].p_Data->StateId == TSC_STATEID_RELEASE))

/**@} end of group TSC_KeyLinearRotate_Macros*/

/** @defgroup TSC_KeyLinearRotate_Variables Variables
  @{
  */


/* Timer tick */
uint16_t cntTick = 0;
uint16_t cnt50ms = 0;
uint8_t taskFlag = 0;
uint8_t tscPressK1Status = 0;
uint8_t tscPressK2Status = 0;
uint8_t tscPressK3Status = 0;
uint8_t tscPressK4Status = 0;

/* Menu title string*/
const char *menuTitleStr[] = {
    "    TSC Touch Example   ",
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

/**@} end of group TSC_KeyLinearRotate_Variables*/

/** @defgroup TSC_KeyLinearRotate_Functions Functions
  @{
  */

void TSC_DetectHandler(void);
void TSC_ReleaseHandler(void);
void APM_EVAL_Init(void);
void APM_EVAL_TMR14_Init(uint16_t period, uint16_t div);
void Menu_TSCHandler(void);

/*!
 * @brief       Main
 *
 * @param       None
 *
 * @retval      None
 */
int main(void)
{
    APM_EVAL_Init();
    TSC_User_Config();

    printf("This is an example of TSC touch detection \r\n");
    while(1)
    {
        /* Execute Touch Driver */
        if (TSC_User_Action() == TSC_STATUS_OK)
        {
            if(taskFlag)
            {
                TSC_DetectHandler();
                Menu_TSCHandler();
            }
        }
        else
        {
            /* Execute other tasks... */
        }
    }
}

/*!
 * @brief       TSC detect handler
 *
 * @param       None
 *
 * @retval      None
 */
void TSC_DetectHandler(void)
{
    uint8_t idx_key;

    for(idx_key = 0; idx_key < TOUCH_TOTAL_CHANNELS; idx_key++)
    {
        if(TOUCHKEY_PRESS(idx_key))
        {
            switch(idx_key)
            {
                case 0:
                    tscPressK1Status ^= 1;
                break;
                
                case 1:
                    tscPressK2Status ^= 1;
                break;
                
                case 2:
                    tscPressK3Status ^= 1;
                break;
                
                case 3:
                    tscPressK4Status ^= 1;
                break;
            }
        }
    }
}

/*!
 * @brief       TSC release handler
 *
 * @param       None
 *
 * @retval      None
 */
void TSC_ReleaseHandler(void)
{
    uint8_t idx_key;

    for(idx_key = 0; idx_key < TOUCH_TOTAL_CHANNELS; idx_key++)
    {
        if(TOUCHKEY_RELEASE(idx_key))
        {
            switch(idx_key)
            {
                case 0:
                    tscPressK1Status = 0;
                break;
                
                case 1:
                    tscPressK2Status = 0;
                break;
                
                case 2:
                    tscPressK3Status = 0;
                break;
                
                case 3:
                    tscPressK4Status = 0;
                break;
            }
        }
    }
}

/*!
 * @brief       TSC touch UI menu
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void Menu_TSCHandler(void)
{
    if(tscPressK1Status)
    {
        LCD_DrawCorss(156,70,LCD_TEXT_FC);
    }
    else
    {
        LCD_DrawCorss(156,70,LCD_TEXT_BC);
    }
    
    if(tscPressK2Status)
    {
        LCD_DrawCorss(96,130,LCD_TEXT_FC);
    }
    else
    {
        LCD_DrawCorss(96,130,LCD_TEXT_BC);
    }
    
    if(tscPressK3Status)
    {
        LCD_DrawCorss(156,190,LCD_TEXT_FC);
    }
    else
    {
        LCD_DrawCorss(156,190,LCD_TEXT_BC);
    }
    
    if(tscPressK4Status)
    {
        LCD_DrawCorss(216,130,LCD_TEXT_FC);
    }
    else
    {
        LCD_DrawCorss(216,130,LCD_TEXT_BC);
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
    
    /* Menu content*/
    LCD_DrawCircle(100,130,20,LCD_TEXT_FC);
    LCD_DrawCircle(220,130,20,LCD_TEXT_FC);
    LCD_DrawCircle(160,70,20,LCD_TEXT_FC);
    LCD_DrawCircle(160,190,20,LCD_TEXT_FC);
    
    /* Menu bottom*/
    LCD_Clear(0,LCD_LINE_BOTTOM,LCD_WIDTH,LCD_HEIGHT,LCD_FCOLOR);
    LCD_DisplayString(60,LCD_LINE_BOTTOM,menuBottomColStr[0],LCD_TEXT_SELECT_FC,LCD_TEXT_SELECT_BC,16,0);
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
    APM_EVAL_LEDInit(LED4);
    APM_EVAL_PBInit(BUTTON_KEY1,BUTTON_MODE_GPIO);
    APM_EVAL_COMInit(COM1);
    
    APM_EVAL_TMR14_Init(1000,48);
    APM_LCDInit();
    LCD_Clear(0,0,LCD_WIDTH,LCD_HEIGHT,LCD_BCOLOR);
    
    Menu_DisplayInit();
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
        cnt50ms++;
        
        if(cnt50ms >= 500)
        {
            cnt50ms = 0;
            
            taskFlag ^= 1;
        }
        
        if(cntTick >= 500)
        {
            cntTick = 0;
            APM_EVAL_LEDToggle(LED1);
        }
    }
}

/*!
 * @brief       Executed when a sensor is in Error state
 *
 * @param       None
 *
 * @retval      None
 */
void MyKeys_ProcessErrorState(void)
{
    TSC_TouchKey_ConfigOffState();
    
    while(1)
    {
        APM_EVAL_LEDToggle(LED1);
        APM_EVAL_LEDToggle(LED2);
        APM_EVAL_DelayMs(100);
    }
}

/*!
 * @brief       Executed when a sensor is in Off state
 *
 * @param       None
 *
 * @retval      None
 */
void MyKeys_ProcessOffState(void)
{
    /* Add here your own processing*/
}

/*!
 * @brief       Executed when a sensor is in Off state
 *
 * @param       None
 *
 * @retval      None
 */
void MyLinRots_ProcessOffState(void)
{
    /* Add here your own processing*/
}

/*!
 * @brief       Executed at each timer interrupt
 *
 * @param       None
 *
 * @retval      None
 */
void TSC_CallBack_TimerTick(void)
{
    /* Add here your own processing*/
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
 *
 * @note
 */
int fputc(int ch, FILE *f)
{
    /** send a byte of data to the serial port */
    USART_TxData(DEBUG_USART,(uint8_t)ch);

    /** wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return (ch);
}

/**@} end of group TSC_KeyLinearRotate_Functions */
/**@} end of group TSC_KeyLinearRotate */
/**@} end of group Examples */
