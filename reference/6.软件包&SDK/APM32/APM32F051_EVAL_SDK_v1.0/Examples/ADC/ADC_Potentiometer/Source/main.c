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
  * @brief ADC potentiometer examples
  @{
  */

/** @addtogroup ADC_POTENTIOMETER
  @{
  */

/** @defgroup ADC_POTENTIOMETER_Macros Macros
  @{
*/

/* printf using USART1  */
#define DEBUG_USART           USART1

/**@} end of group ADC_POTENTIOMETER_Macros*/

/** @defgroup ADC_POTENTIOMETER_Variables Variables
  @{
  */

uint32_t voltage = 0;

/* Timer tick */
uint16_t cntTick = 0;

/* Menu title string*/
const char *menuTitleStr[] = {
    " Potentiometer Example  ",
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

/**@} end of group ADC_POTENTIOMETER_Variables*/

/** @defgroup ADC_POTENTIOMETER_Functions Functions
  @{
  */

void APM_EVAL_Init(void);
void Potentiometer_Test(void);
void ADC1_Init(void);
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
    printf("This is an example of potentiometer detection \r\n");
    
    printf("Press KEY1 to start test.\r\n");

    while (1)
    {
        if(APM_EVAL_PBGetState(BUTTON_KEY1) == 0)
        {
            Potentiometer_Test();
            
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
    
    Menu_DisplayInit();
    
    ADC1_Init();
    
    /* Wait until ADC is ready */
    while (!ADC_ReadStatusFlag(ADC_FLAG_ADRDY));

    ADC_StartConversion();
}

/*!
 * @brief       Detect potentiometer
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void Potentiometer_Test(void)
{
    APM_EVAL_LEDOff(LED2);
    APM_EVAL_LEDOff(LED3);
    LCD_Clear(0,LCD_LINE_2,LCD_WIDTH,134,LCD_BCOLOR);
    
    printf("\r\nTest start \r\n");
    
    while(1)
    {
        LCD_DisplayIntNum(128,LCD_LINE_2,voltage,4,LCD_TEXT_FC,LCD_TEXT_BC,24);
        LCD_DisplayString(180,LCD_LINE_2,"mV",LCD_TEXT_FC,LCD_TEXT_BC,24,0);
        APM_EVAL_DelayMs(50);
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
 * @brief       ADC1 Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void ADC1_Init(void)
{
    GPIO_Config_T gpioConfig;
    ADC_Config_T  adcConfig;

    /* RCM Enable*/
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_GPIOC);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC1);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);
    /* GPIO Configuration */
    gpioConfig.pin = GPIO_PIN_0;
    gpioConfig.mode = GPIO_MODE_AN;
    gpioConfig.pupd = GPIO_PUPD_PU;
    GPIO_Config(GPIOC, &gpioConfig);
    /* ADC Configuration */
    ADC_Reset();
    ADC_ConfigStructInit(&adcConfig);
    /* Set resolution*/
    adcConfig.resolution = ADC_RESOLUTION_12B;
    /* Set dataAlign*/
    adcConfig.dataAlign  = ADC_DATA_ALIGN_RIGHT;
    /* Set scanDir*/
    adcConfig.scanDir    = ADC_SCAN_DIR_UPWARD;
    /* Set convMode continous*/
    adcConfig.convMode   = ADC_CONVERSION_CONTINUOUS;
    /* Set extTrigConv*/
    adcConfig.extTrigConv  = ADC_EXT_TRIG_CONV_TRG0;
    /* Set TrigEdge*/
    adcConfig.extTrigEdge    = ADC_EXT_TRIG_EDGE_NONE;

    ADC_Config(&adcConfig);
    ADC_ConfigChannel(ADC_CHANNEL_10, ADC_SAMPLE_TIME_239_5);
    /* Enable Interrupt*/
    ADC_EnableInterrupt(ADC_INT_CS);

    NVIC_EnableIRQRequest(ADC1_COMP_IRQn, 2);

    /* Calibration*/
    ADC_ReadCalibrationFactor();
    /* Enable ADC*/
    ADC_Enable();
}

/*!
 * @brief       ADC1 Isr
 *
 * @param       None
 *
 * @retval      None
 *
 * @note        This function need to put into ADC1_IRQHandler in apm32f0xx_int.c
 */
void ADC1_Isr(void)
{
    uint32_t adcData = 0;

    if (ADC_ReadIntFlag(ADC_INT_FLAG_CS) == SET)
    {
        ADC_ClearIntFlag(ADC_INT_FLAG_CS);
        /* Read ADC Conversion value*/
        adcData = ADC_ReadConversionValue();
        /*  voltage(mV) =  adcData * (3300mV / 4095) */
        voltage = (adcData * 3300) / 4095;
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
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return (ch);
}

/**@} end of group ADC_POTENTIOMETER_Functions */
/**@} end of group ADC_POTENTIOMETER */
/**@} end of group Examples */
