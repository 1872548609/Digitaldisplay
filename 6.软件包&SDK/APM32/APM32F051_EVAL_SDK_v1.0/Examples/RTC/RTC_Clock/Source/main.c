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
  * @brief RTC Clock examples
  @{
  */

/** @addtogroup RTC_Clock
  @{
  */

/** @defgroup RTC_Clock_Macros Macros
  @{
*/

/* printf using USART1  */
#define DEBUG_USART           USART1

#define RTC_BKP_VALUE         0xAA55
/**@} end of group RTC_Clock_Macros*/

/** @defgroup RTC_Clock_Variables Variables
  @{
  */

/* Timer tick */
uint16_t cntTick = 0;

/* Menu title string*/
const char *menuTitleStr[] = {
    "   RTC Clock Example    ",
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

/**@} end of group RTC_Clock_Variables*/

/** @defgroup RTC_Clock_Functions Functions
  @{
  */

void APM_EVAL_Init(void);
void RTC_ClockTest(void);
void Menu_DisplayInit(void);
void APM_EVAL_TMR14_Init(uint16_t period, uint16_t div);
/** RTC Init */
void RTC_Init(void);

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
    printf("This is an example of RTC clock.\r\n");
    
    while (1)
    {
        RTC_ClockTest();
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
    
    Menu_DisplayInit();
    
}

/*!
 * @brief       RTC clock
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void RTC_ClockTest(void)
{
    uint8_t prevTime;
    uint32_t date;
    uint32_t time;
    char dateStr[11];
    char timeStr[9];
    RTC_DATE_T DateStruct;
    RTC_TIME_T TimeStruct;
    
    APM_EVAL_LEDOff(LED2);
    APM_EVAL_LEDOff(LED3);
    LCD_Clear(0,LCD_LINE_2,LCD_WIDTH,134,LCD_BCOLOR);
    
    /* RTC Reset */
    RTC_Init();
    RTC_Reset();
    RTC_Init();

    /* RTC Enable Init*/
    RTC_EnableInit();

    RTC_ConfigDateStructInit(&DateStruct);
    
    printf("\r\nTest start \r\n");
    
    /* First time Init*/
    if(RTC_ReadBackup(RTC_BAKP_DATA4) != RTC_BKP_VALUE)
    {
        /* Init Date 22/8/22/5*/
        DateStruct.year =  22;
        DateStruct.month = 8;
        DateStruct.date =  26;
        DateStruct.weekday = 5;
        RTC_ConfigDate(RTC_FORMAT_BIN,&DateStruct);
        APM_EVAL_DelayMs(10);

        TimeStruct.H12 = 12;
        TimeStruct.hours = 23;
        TimeStruct.minutes = 59;
        TimeStruct.seconds = 50;
        RTC_ConfigTime(RTC_FORMAT_BIN,&TimeStruct);
        
        RTC_WriteBackup(RTC_BAKP_DATA1,RTC->DATE);
        RTC_WriteBackup(RTC_BAKP_DATA0,RTC->TIME);

        RTC_WriteBackup(RTC_BAKP_DATA4,RTC_BKP_VALUE);
    }
    /** Read Date information from Backup*/
    else
    {
        time = RTC_ReadBackup(RTC_BAKP_DATA0);
        TimeStruct.hours = time >> 16;
        TimeStruct.minutes = (time >> 8) & 0xff;
        TimeStruct.seconds = (time) & 0xff;
        RTC_ConfigTime(RTC_FORMAT_BCD,&TimeStruct);
        APM_EVAL_DelayMs(10);
        
        date = RTC_ReadBackup(RTC_BAKP_DATA1);
        DateStruct.year =  date >> 16;
        DateStruct.month = (date >> 8) & 0x0f;
        DateStruct.date =  date & 0xff;
        DateStruct.weekday = (date >> 13) & 0x07;
        RTC_ConfigDate(RTC_FORMAT_BCD,&DateStruct);
    }
    
    /* Exit RTC init mode */
    RTC_DisableInit();
    
    while(1)
    {
        /* Read last Time*/
        prevTime = TimeStruct.seconds;
        
        /* Read time*/
        RTC_ReadTime(RTC_FORMAT_BIN,&TimeStruct);
        
        /** Read Date */
        RTC_ReadDate(RTC_FORMAT_BIN,&DateStruct);
        
        /** If pass 1 second */
        if(prevTime != TimeStruct.seconds)
        {
            sprintf(dateStr,"20%02d-%02d-%02d",DateStruct.year,DateStruct.month,DateStruct.date);
            sprintf(timeStr,"%02d:%02d:%02d",TimeStruct.hours,TimeStruct.minutes,TimeStruct.seconds);

            printf("%s\r\n",dateStr);
            printf("%s\r\n",timeStr);
            
            LCD_DisplayString(102,LCD_LINE_1,dateStr,LCD_TEXT_FC,LCD_TEXT_BC,24,0);
            LCD_DisplayString(112,LCD_LINE_2,timeStr,LCD_TEXT_FC,LCD_TEXT_BC,24,0);
            
            RTC_WriteBackup(RTC_BAKP_DATA0,RTC->TIME);
            
            if((TimeStruct.hours == 0) && (TimeStruct.minutes == 0) && (TimeStruct.seconds == 0))
            RTC_WriteBackup(RTC_BAKP_DATA1,RTC->DATE);
        }
    }
}

/*!
 * @brief       RTC Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void RTC_Init(void)
{
    RTC_Config_T Struct;

    /* Enable PMU Periph Clock*/
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_PMU);

    /* Backup Access*/
    PMU_EnableBackupAccess();

    /* Enable LSI */
    RCM_EnableLSI();

    /* Wait until LSI is ready*/
    while(RCM_ReadStatusFlag(RCM_FLAG_LSIRDY) == RESET)
    {
    }
    /* Select LICT as RCM clock*/
    RCM_ConfigRTCCLK(RCM_RTCCLK_LSI);

    /* Enable RTC clock*/
    RCM_EnableRTCCLK();

    /* Disable RTC write Protection*/
    RTC_DisableWriteProtection();

    /* Wait Synchro*/
    RTC_WaitForSynchro();

    /* set LSI Prediv */
    RTC_ConfigStructInit(&Struct);
    Struct.AsynchPrediv = 0X63;
    Struct.SynchPrediv  = 0X18F;

    RTC_Config(&Struct);
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

/**@} end of group RTC_Clock_Functions */
/**@} end of group RTC_Clock */
/**@} end of group Examples */
