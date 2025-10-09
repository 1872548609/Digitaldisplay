/**************************************************************************************************
Filename:       iot_app_optical.c
Revised:        $Date: 2019-8-20 15:35
Revision:       $Revision: V1.0.0

Description:

Copyright 2021 uwaycon. All rights reserved.
                                                
**************************************************-*****************^******************************/
#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _IOT_APP_OPTICAL_H
/*************************************************-*************************************************
*                                               NOTE
**************************************************-*****************^******************************/
//-- source insight use UTF-8 to encode, File->Reload As Encoding...->UTF-8
//-- source insight set default encode, Option->Preferences->Files->Default encoding->UTF-8

/*************************************************-*************************************************
*                                          CONSTANT DEFINED
**************************************************-*****************^******************************/
/*
//-- 实测友商的模式与PWM的关系，PB0输出的PWM时间周期如下
MEGA       : 200us + 100us + 100us + 100us, pulse 5us low, 240-9999, adc value range : 40mv - 216mv, 自己程序在友商相同距离下的adc的值的范围 : 0mv - 136mv
SUPER      : 100us + 50us  + 50us  + 50us,  pulse 5us low, 15-8000,  adc value range : 40mv - 2.00v, 自己程序在友商相同距离下的adc的值的范围 : 0mv - 1.28v
FINE       : 100us + 50us  + 50us  + 50us,  pulse 5us low, 3-4000,   adc value range : 40mv - 1.68v, 自己程序在友商相同距离下的adc的值的范围 : 0mv - 1.04v
HSP        : 50us  + 25us  + 25us  + 25us,  pulse 5us low, 3-4000,   adc value range : 40mv - 1.60v, 自己程序在友商相同距离下的adc的值的范围 : 0mv - 1.00v, 0 - 1070

//-- 实测友商的模式的ATT与PWM的关系，PA5输出的PWM时间周期如下
MEGA/SUPER/FINE : ATT
PWM PERIOD : 10khz 100us, ATT 0   : 60us low + 40us hi
PWM PERIOD : 10khz 100us, ATT 50  : 40us low + 60us hi
PWM PERIOD : 10khz 100us, ATT 100 : 20us low + 80us hi

PWM PERIOD : 10khz 100us, ATT 0   : 59us
PWM PERIOD : 10khz 100us, ATT 5   : 58us
PWM PERIOD : 10khz 100us, ATT 7   : 57us
PWM PERIOD : 10khz 100us, ATT 10  : 56us
PWM PERIOD : 10khz 100us, ATT 12  : 55us
PWM PERIOD : 10khz 100us, ATT 15  : 54us
PWM PERIOD : 10khz 100us, ATT 17  : 53us
PWM PERIOD : 10khz 100us, ATT 20  : 52us
PWM PERIOD : 10khz 100us, ATT 22  : 51us
PWM PERIOD : 10khz 100us, ATT 25  : 50us
PWM PERIOD : 10khz 100us, ATT 27  : 49us
PWM PERIOD : 10khz 100us, ATT 30  : 48us
PWM PERIOD : 10khz 100us, ATT 32  : 47us
PWM PERIOD : 10khz 100us, ATT 35  : 46us
PWM PERIOD : 10khz 100us, ATT 37  : 45us
PWM PERIOD : 10khz 100us, ATT 40  : 44us
PWM PERIOD : 10khz 100us, ATT 42  : 43us
PWM PERIOD : 10khz 100us, ATT 45  : 42us
PWM PERIOD : 10khz 100us, ATT 47  : 41us
PWM PERIOD : 10khz 100us, ATT 50  : 40us
PWM PERIOD : 10khz 100us, ATT 100 : 20us



HSP : ATT
PWM PERIOD : 10khz 100us, ATT 0   : 60us low + 40us hi
PWM PERIOD : 10khz 100us, ATT 100 : 40us low + 60us hi

PWM PERIOD : 10khz 100us, ATT 0   : 59us
PWM PERIOD : 10khz 100us, ATT 7   : 58us
PWM PERIOD : 10khz 100us, ATT 12  : 57us
PWM PERIOD : 10khz 100us, ATT 17  : 56us
PWM PERIOD : 10khz 100us, ATT 22  : 55us
PWM PERIOD : 10khz 100us, ATT 27  : 54us
PWM PERIOD : 10khz 100us, ATT 32  : 53us
PWM PERIOD : 10khz 100us, ATT 37  : 52us
PWM PERIOD : 10khz 100us, ATT 42  : 51us
PWM PERIOD : 10khz 100us, ATT 47  : 50us
PWM PERIOD : 10khz 100us, ATT 52  : 49us
PWM PERIOD : 10khz 100us, ATT 57  : 48us
PWM PERIOD : 10khz 100us, ATT 62  : 47us
PWM PERIOD : 10khz 100us, ATT 67  : 46us
PWM PERIOD : 10khz 100us, ATT 72  : 45us
PWM PERIOD : 10khz 100us, ATT 77  : 44us
PWM PERIOD : 10khz 100us, ATT 82  : 43us
PWM PERIOD : 10khz 100us, ATT 87  : 42us
PWM PERIOD : 10khz 100us, ATT 92  : 41us
PWM PERIOD : 10khz 100us, ATT 97  : 40us
PWM PERIOD : 10khz 100us, ATT 100 : 39us

*/
#define IOT_APP_OPTICAL_MEGA_ATT_ADC_VALUE_MIN                      3
#define IOT_APP_OPTICAL_MEGA_ATT_ADC_VALUE_MAX                      100
#define IOT_APP_OPTICAL_MEGA_ATT_LUX_VALUE_MIN                      0
#define IOT_APP_OPTICAL_MEGA_ATT_LUX_VALUE_MAX                      9999

#define IOT_APP_OPTICAL_SUPER_ATT_ADC_VALUE_MIN                     3
#define IOT_APP_OPTICAL_SUPER_ATT_ADC_VALUE_MAX                     1100
#define IOT_APP_OPTICAL_SUPER_ATT_LUX_VALUE_MIN                     0
#define IOT_APP_OPTICAL_SUPER_ATT_LUX_VALUE_MAX                     8000

#define IOT_APP_OPTICAL_FINE_ATT_ADC_VALUE_MIN                      3
#define IOT_APP_OPTICAL_FINE_ATT_ADC_VALUE_MAX                      800
#define IOT_APP_OPTICAL_FINE_ATT_LUX_VALUE_MIN                      0
#define IOT_APP_OPTICAL_FINE_ATT_LUX_VALUE_MAX                      4000

#define IOT_APP_OPTICAL_HSP_ATT_ADC_VALUE_MIN                       3
#define IOT_APP_OPTICAL_HSP_ATT_ADC_VALUE_MAX                       950
#define IOT_APP_OPTICAL_HSP_ATT_LUX_VALUE_MIN                       0
#define IOT_APP_OPTICAL_HSP_ATT_LUX_VALUE_MAX                       4000



/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                               MACRO
**************************************************-*****************^******************************/
#define IOT_APP_OPTICAL_ADC_START()                                 {if (iot_app_optical_adc_init_flag)ADC->CTRL_B.STARTCEN = BIT_SET;}
#define IOT_APP_OPTICAL_ADC_STOP()                                  {ADC->CTRL_B.STOPCEN = BIT_SET;}

#define IOT_APP_DATA_MIDDLE(a, b, c)                                ((a >  b) ? (b > c ? b : (a > c ? c : a)) : (a > c ? a : (b > c ? c : b)))
#define IOT_APP_DATA_MIN(a, b, c)                                   ((a >= b) ? (b >= c ? c : b) : (a >= c ? c : a))
#define IOT_APP_DATA_ABS(a, b)                                      ((a >= b) ? (a - b) : (b - a))

/*************************************************-*************************************************
*                                          CONSTANT DEFINED
**************************************************-*****************^******************************/
//-- ATT : CC value 20us - 60us, 0->max, 100->min
#define IOT_APP_OPTICAL_TIMER14_PERIOD_VALUE                        4800

#define IOT_APP_OPTICAL_TIMER14_PWM_CC_VALUE_MAX                    4704

#define IOT_APP_OPTICAL_ATT_TIMER14_PWM_CC1_VALUE_MAX               2960    //-- 实际2880就可以，但是方便加减整数计算，step为20，即将最大值设置为2960， 20 * 100 = 2000
#define IOT_APP_OPTICAL_ATT_TIMER14_PWM_CC1_VALUE_MIN               960
#define IOT_APP_OPTICAL_ATT_TIMER14_PWM_CC1_VALUE_STEP              20

//-- HSP MODE ATT : CC value 40us - 60us, 0->max, 100->min
#define IOT_APP_OPTICAL_HSP_ATT_TIMER14_PWM_CC1_VALUE_MAX           2920    //-- 实际2880就可以，但是方便加减整数计算，step为10，即将最大值设置为2920， 10 * 100 = 1000
#define IOT_APP_OPTICAL_HSP_ATT_TIMER14_PWM_CC1_VALUE_MIN           1920
#define IOT_APP_OPTICAL_HSP_ATT_TIMER14_PWM_CC1_VALUE_STEP          10

#define IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_25US                1200
#define IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_50US                2400
#define IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_100US               4800
#define IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_200US               9600

#define IOT_APP_OPTICAL_KEY_MASK                                    (HAL_KEY_SET|HAL_KEY_RIGHT_SUB|HAL_KEY_LEFT_ADD)

#define IOT_APP_OPTICAL_ADC_DATA_BUF_SIZE                           10
#define IOT_APP_OPTICAL_ADC_DELTA_MIN_DATA_BUF_SIZE                 100

#define IOT_APP_OPTICAL_LONG_PERIOD_START_COUNT                     2
#define IOT_APP_OPTICAL_SHORT_PERIOD_START_COUNT                    3

/*************************************************-*************************************************
*                                           STRUCT DEFINED
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          GLOBAL VARIABLES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/
uint8  iot_app_optical_task_id;   // 任务ID，由OSAL分配，用于内部任务/事件处理
uint16 iot_app_optical_timer14_pwm_cc_value = IOT_APP_OPTICAL_ATT_TIMER14_PWM_CC1_VALUE_MIN;//-- 0-100 : max-min, step

uint8  iot_app_optical_timer3_period_count = 0;
uint32 iot_app_optical_timer3_period_short = IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_25US;
uint32 iot_app_optical_timer3_period_long  = IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_50US;

uint8  iot_app_optical_adc_init_flag = 0;
uint8  iot_app_optical_adc_data_index = 0;
uint16 iot_app_optical_adc_data[IOT_APP_OPTICAL_ADC_DATA_BUF_SIZE] = {0};
uint16 iot_app_optical_adc_read_count = 0;
uint16 iot_app_optical_adc_read_vdd_count = 0;
uint16 iot_app_optical_adc_interrupt_count = 0;
int16  iot_app_optical_adc_delta_min_data[IOT_APP_OPTICAL_ADC_DELTA_MIN_DATA_BUF_SIZE] = {0};
uint8  iot_app_optical_adc_delta_min_data_index = 0;

iot_app_optical_adc_to_lux_t iot_app_optical_adc_to_lux_table[IOT_APP_OPTICAL_MODE_MAX] = 
{
    {IOT_APP_OPTICAL_MEGA_ATT_ADC_VALUE_MIN, IOT_APP_OPTICAL_MEGA_ATT_ADC_VALUE_MAX, IOT_APP_OPTICAL_MEGA_ATT_LUX_VALUE_MIN, IOT_APP_OPTICAL_MEGA_ATT_LUX_VALUE_MAX},
    {IOT_APP_OPTICAL_SUPER_ATT_ADC_VALUE_MIN, IOT_APP_OPTICAL_SUPER_ATT_ADC_VALUE_MAX, IOT_APP_OPTICAL_SUPER_ATT_LUX_VALUE_MIN, IOT_APP_OPTICAL_SUPER_ATT_LUX_VALUE_MAX},
    {IOT_APP_OPTICAL_FINE_ATT_ADC_VALUE_MIN, IOT_APP_OPTICAL_FINE_ATT_ADC_VALUE_MAX, IOT_APP_OPTICAL_FINE_ATT_LUX_VALUE_MIN, IOT_APP_OPTICAL_FINE_ATT_LUX_VALUE_MAX},
    {IOT_APP_OPTICAL_HSP_ATT_ADC_VALUE_MIN, IOT_APP_OPTICAL_HSP_ATT_ADC_VALUE_MAX, IOT_APP_OPTICAL_HSP_ATT_LUX_VALUE_MIN, IOT_APP_OPTICAL_HSP_ATT_LUX_VALUE_MAX},
};

/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/
//#define IOT_APP_TIMER3_INTERRUPT_CHANNEL_COUNT
//uint16 iot_app_timer3_interrupt_channel_1_count = 0;
//uint16 iot_app_timer3_interrupt_channel_2_count = 0;
//uint16 iot_app_timer3_interrupt_channel_3_count = 0;
//uint16 iot_app_timer3_interrupt_channel_4_count = 0;
//uint16 iot_app_timer3_interrupt_update_count = 0;


int16 delta_data = 0;
int16 delta_min;
int16 delta_temp[3];
uint8 delta_positive_threshold_count = 0;
uint8 delta_negative_threshold_count = 0;

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/
void TMR2_IRQHandler(void)
{
    display_scan();
    TMR2->STS = 0;
}

void TMR3_IRQHandler(void)
{
    uint8 channel_1_flag = 0;
    
    //-- channel 1设置的时间为channel 3前面一点，就是LED发光后的一定的时间去读取ADC的值，仅作为触发ADC，没实际控制IO
    if ((TMR3->STS & TMR_INT_FLAG_CH1) != 0)
    {
        TMR3->STS = ~TMR_INT_FLAG_CH1;
        channel_1_flag = 1;
        //IOT_APP_OPTICAL_TEST_IO_ON();
        IOT_APP_OPTICAL_ADC_START();

        #ifdef IOT_APP_TIMER3_INTERRUPT_CHANNEL_COUNT
        iot_app_timer3_interrupt_channel_1_count++;
        #endif
    }

    if ((TMR3->STS & TMR_INT_FLAG_UPDATE) != 0)
    {
        //-- channel 2和channel 3比较点输出高，周期点输出低，也就是一个周期里面前半段为低电平
        //IOT_APP_OPTICAL_TEST_IO_OFF();
        TMR3->STS = ~TMR_INT_FLAG_UPDATE;
        
        #ifdef IOT_APP_TIMER3_INTERRUPT_CHANNEL_COUNT
        iot_app_timer3_interrupt_update_count++;
        #endif
        
        iot_app_optical_timer3_period_count++;
        if (iot_app_optical_timer3_period_count == IOT_APP_OPTICAL_SHORT_PERIOD_START_COUNT)
        {
            //-- 这个判断必须要最先
            if (iot_app_optical_adc_data_index == 4 &&
                iot_app_optical_adc_interrupt_count == 4 &&
                iot_app_optical_adc_read_vdd_count == 1/* &&
                iot_app_timer3_interrupt_channel_1_count == 3 &&
                iot_app_timer3_interrupt_channel_2_count == 3 &&
                iot_app_timer3_interrupt_channel_3_count == 3 &&
                iot_app_timer3_interrupt_channel_4_count == 3*/)
            {
                uint32 current_data = iot_app_optical_current_adc_value;

                {
                    //-- 找一个合适的当前值
                    
                    #if 1
                    current_data = IOT_APP_DATA_MIDDLE(iot_app_optical_adc_data[0]&0xffff, iot_app_optical_adc_data[1]&0xffff, iot_app_optical_adc_data[2]&0xffff);
                    #endif

                    #if 0
                    current_data = (iot_app_optical_adc_data[0]&0xffff) + (iot_app_optical_adc_data[1]&0xffff) + (iot_app_optical_adc_data[2]&0xffff);
                    current_data /= 3;
                    #endif

                    #if 0
                    delta_temp[0] = (iot_app_optical_adc_data[0]&0xffff) - iot_app_optical_current_adc_value;
                    delta_temp[1] = (iot_app_optical_adc_data[1]&0xffff) - iot_app_optical_current_adc_value;
                    delta_temp[2] = (iot_app_optical_adc_data[2]&0xffff) - iot_app_optical_current_adc_value;
                    delta_min = IOT_APP_DATA_MIN(delta_temp[0], delta_temp[1], delta_temp[2]);

                    iot_app_optical_adc_delta_min_data[iot_app_optical_adc_delta_min_data_index++] = delta_min;
                    iot_app_optical_adc_delta_min_data_index %= IOT_APP_OPTICAL_ADC_DELTA_MIN_DATA_BUF_SIZE;

                    delta_data += delta_min;
                    delta_data -= iot_app_optical_adc_delta_min_data[(iot_app_optical_adc_delta_min_data_index+1)%IOT_APP_OPTICAL_ADC_DELTA_MIN_DATA_BUF_SIZE];

                    delta_data /= IOT_APP_OPTICAL_ADC_DELTA_MIN_DATA_BUF_SIZE;
                    
                    current_data = iot_app_optical_current_adc_value + delta_data;
                    #endif

                    #if 0
                    delta_temp[0] = 4095 - (iot_app_optical_adc_data[0]&0xffff);
                    delta_temp[1] = 4095 - (iot_app_optical_adc_data[1]&0xffff);
                    delta_temp[2] = 4095 - (iot_app_optical_adc_data[2]&0xffff);
                    delta_min = IOT_APP_DATA_MIN(delta_temp[0], delta_temp[1], delta_temp[2]);

                    iot_app_optical_adc_delta_min_data[iot_app_optical_adc_delta_min_data_index++] = delta_min;
                    iot_app_optical_adc_delta_min_data_index %= IOT_APP_OPTICAL_ADC_DELTA_MIN_DATA_BUF_SIZE;

                    if (delta_min > 50)
                    {
                        delta_positive_threshold_count++;
                        delta_negative_threshold_count = 0;
                    }
                    else if (delta_min < 50)
                    {
                        delta_negative_threshold_count++;
                        delta_positive_threshold_count = 0;
                    }
                    else
                    {
                        delta_positive_threshold_count = 0;
                        delta_negative_threshold_count = 0;
                    }

                    if (delta_positive_threshold_count > 3 || delta_negative_threshold_count > 3)
                    {
                        current_data += delta_min;
                    }
                    #endif
                }

                {
                    //-- 滤波过滤
                    #if 0
                    //iot_app_optical_current_adc_value = current_data;
                    iot_app_optical_current_adc_value += current_data;
                    //iot_app_optical_current_adc_value += iot_app_optical_adc_data[1]&0xffff;
                    iot_app_optical_current_adc_value >>= 1;
                    #else
                    #define ALPHA 5
                    uint32 output = (ALPHA * current_data + (10 - ALPHA) * iot_app_optical_current_adc_value)/10;
                    iot_app_optical_current_adc_value = output;
                    #endif

                    iot_app_vdd_current_adc_value += iot_app_optical_adc_data[3]&0xffff;
                    iot_app_vdd_current_adc_value >>= 1;
                }

                {
                    //-- 数据转换
                    if (iot_app_vdd_current_adc_value >= iot_app_optical_current_adc_value)
                    {
                        iot_app_optical_delta_adc_value = iot_app_vdd_current_adc_value - iot_app_optical_current_adc_value;

                        iot_app_optical_delta_voltage_mv = iot_app_optical_delta_adc_value;
                        iot_app_optical_delta_voltage_mv *= 3300;
                        iot_app_optical_delta_voltage_mv /= 4095;
                        
                        if (iot_app_optical_delta_adc_value > iot_app_optical_adc_to_lux_table[iot_app_optical_current_mode].adc_max)
                        {
                            iot_app_optical_current_lux = iot_app_optical_adc_to_lux_table[iot_app_optical_current_mode].lux_max;
                        }
                        else if (iot_app_optical_delta_adc_value < iot_app_optical_adc_to_lux_table[iot_app_optical_current_mode].adc_min)
                        {
                            iot_app_optical_current_lux = iot_app_optical_adc_to_lux_table[iot_app_optical_current_mode].lux_min;
                        }
                        else
                        {
                            //-- lux / (lux_max - lux_min) = adc_delta / (adc_max - adc_min)
                            //-- lux = adc_delta * (lux_max - lux_min) / (adc_max - adc_min)
                            iot_app_optical_current_lux = iot_app_optical_delta_adc_value;
                            iot_app_optical_current_lux *= (iot_app_optical_adc_to_lux_table[iot_app_optical_current_mode].lux_max - iot_app_optical_adc_to_lux_table[iot_app_optical_current_mode].lux_min);
                            iot_app_optical_current_lux /= (iot_app_optical_adc_to_lux_table[iot_app_optical_current_mode].adc_max - iot_app_optical_adc_to_lux_table[iot_app_optical_current_mode].adc_min);
                            if (iot_app_optical_current_lux > iot_app_optical_adc_to_lux_table[iot_app_optical_current_mode].lux_max)
                            {
                                iot_app_optical_current_lux = iot_app_optical_adc_to_lux_table[iot_app_optical_current_mode].lux_max;
                            }
                            else if (iot_app_optical_current_lux < iot_app_optical_adc_to_lux_table[iot_app_optical_current_mode].lux_min)
                            {
                                iot_app_optical_current_lux = iot_app_optical_adc_to_lux_table[iot_app_optical_current_mode].lux_min;
                            }
                        }
                    }
                }

                {
                	//-- 逻辑判断，刷新显示
                	if (iot_app_optical_current_lux > thresholdValue)
                	{
                		if (!outmodeset)
                		{
                			HAL_NPN_OUT_ON();
                			HAL_RLED_SET(1);
                		}
                		else	
                		{
                			HAL_NPN_OUT_OFF();
                			HAL_RLED_SET(0);
                		}
                	}
                	else
                	{
                		if (!outmodeset)
                		{
                			HAL_NPN_OUT_OFF();
                			HAL_RLED_SET(0);
                		}
                		else	
                		{
                			HAL_NPN_OUT_ON();
                			HAL_RLED_SET(1);
                		}
                	}
            	}

                //iot_app_optical_adc_data[0] = 0;
                //iot_app_optical_adc_data[1] = 0;
                //iot_app_optical_adc_data[2] = 0;
                //iot_app_optical_adc_data[3] = 0;
            }
            else
            {
                //-- data error, output test
                //if (IOT_APP_OPTICAL_TEST_IO_OUTPUT_STATE())
                //{
                //    IOT_APP_OPTICAL_TEST_IO_OFF();
                //}
                //else
                //{
                //    IOT_APP_OPTICAL_TEST_IO_ON();
                //}
            }

            TMR3->AUTORLD = iot_app_optical_timer3_period_short;
            iot_app_optical_timer3_period_count = 0;
            
            iot_app_optical_adc_data_index = 0;
            iot_app_optical_adc_read_vdd_count = 0;
            iot_app_optical_adc_interrupt_count = 0;

            if (!channel_1_flag)
            {
                iot_app_optical_adc_read_count = 0;
            }

            #ifdef IOT_APP_TIMER3_INTERRUPT_CHANNEL_COUNT
            iot_app_timer3_interrupt_channel_1_count = 0;
            iot_app_timer3_interrupt_channel_2_count = 0;
            iot_app_timer3_interrupt_channel_3_count = 0;
            iot_app_timer3_interrupt_channel_4_count = 0;
            iot_app_timer3_interrupt_update_count = 0;
            #endif
        }
        
        else if (iot_app_optical_timer3_period_count == IOT_APP_OPTICAL_LONG_PERIOD_START_COUNT)
        {
            TMR3->AUTORLD = iot_app_optical_timer3_period_long;
        }
    }

    if (channel_1_flag)
    {
        //if (iot_app_optical_timer3_period_count == (IOT_APP_OPTICAL_SHORT_PERIOD_START_COUNT-1))
        {
            iot_app_optical_adc_read_count = iot_app_optical_timer3_period_count;
        }
        //else
        //{
        //    iot_app_optical_adc_read_count++;
        //}
    }

    //-- PA7 channel 2 for switch ADC to connect VDD, compare to output HI
    if ((TMR3->STS & TMR_INT_FLAG_CH2) != 0)
    {
        TMR3->STS = ~TMR_INT_FLAG_CH2;
        #ifdef IOT_APP_TIMER3_INTERRUPT_CHANNEL_COUNT
        iot_app_timer3_interrupt_channel_2_count++;
        #endif
    }

    //-- PB0 channel 3 for open LED transmit, compare to output HI
    if ((TMR3->STS & TMR_INT_FLAG_CH3) != 0)
    {
        TMR3->STS = ~TMR_INT_FLAG_CH3;
        #ifdef IOT_APP_TIMER3_INTERRUPT_CHANNEL_COUNT
        iot_app_timer3_interrupt_channel_3_count++;
        #endif
    }

    //-- channel 4设置的时间为channel 3后面一点，就是当PA7控制ADC连接到VDD后一定的时间去读取ADC的值，仅作为触发ADC，没实际控制IO
    if ((TMR3->STS & TMR_INT_FLAG_CH4) != 0)
    {
        TMR3->STS = ~TMR_INT_FLAG_CH4;
        #ifdef IOT_APP_TIMER3_INTERRUPT_CHANNEL_COUNT
        iot_app_timer3_interrupt_channel_4_count++;
        #endif

        if (iot_app_optical_timer3_period_count == IOT_APP_OPTICAL_LONG_PERIOD_START_COUNT)
        {
            iot_app_optical_adc_read_vdd_count++;
            iot_app_optical_adc_read_count++;
            //IOT_APP_OPTICAL_TEST_IO_ON();
            IOT_APP_OPTICAL_ADC_START();
        }
    }
}

void TMR14_IRQHandler(void)
{
    if ((TMR14->STS & TMR_INT_FLAG_CH1) != 0)
    {
        TMR14->STS = ~TMR_INT_FLAG_CH1;
        IOT_APP_OPTICAL_ATT_HI();
    }

    if ((TMR14->STS & TMR_INT_FLAG_UPDATE) != 0)
    {
        TMR14->STS = ~TMR_INT_FLAG_UPDATE;
        IOT_APP_OPTICAL_ATT_LOW();
    }
}

void ADC1_COMP_IRQHandler(void)
{
    iot_app_optical_adc_interrupt_count++;
    if (ADC->STS & (uint32_t)(ADC_INT_FLAG_CC & 0xff))
    {
        //IOT_APP_OPTICAL_TEST_IO_OFF();
        iot_app_optical_adc_data[iot_app_optical_adc_data_index] = (uint16_t)ADC->DATA;
        //iot_app_optical_adc_data[iot_app_optical_adc_data_index] |= iot_app_optical_adc_read_count << 16;
        iot_app_optical_adc_data_index++;
    }

    ADC->STS = 0xff;
}

void DMA1_CH1_IRQHandler(void)
{
    //IOT_APP_OPTICAL_TEST_IO_OFF();
    DMA1->INTFCLR |= (uint32_t)(DMA1_INT_FLAG_AL1 & 0x0FFFFFFF);
}

void iot_app_optical_timer3_pwm_init(void)
{
    GPIO_Config_T  GPIO_ConfigStruct;
    TMR_TimeBase_T TMR_TimeBaseStruct;
    TMR_OCConfig_T OCcongigStruct;
    
    /* Enable Clock */
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_GPIOA);
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_GPIOB);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR3);

    /* Config PA7 for timer3 channel2 output PWM for connect adc to VDD*/
    GPIO_ConfigPinAF(GPIOA, GPIO_PIN_SOURCE_7, GPIO_AF_PIN1);
    GPIO_ConfigStruct.mode    = GPIO_MODE_AF;
    GPIO_ConfigStruct.outtype = GPIO_OUT_TYPE_PP;
    GPIO_ConfigStruct.pin     = GPIO_PIN_7;
    GPIO_ConfigStruct.pupd    = GPIO_PUPD_NO;
    GPIO_ConfigStruct.speed   = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);

    /* Config PB0 for timer3 channel3 output PWM for open LED to transmit*/
    GPIO_ConfigPinAF(GPIOB, GPIO_PIN_SOURCE_0, GPIO_AF_PIN1);
    GPIO_ConfigStruct.mode    = GPIO_MODE_AF;
    GPIO_ConfigStruct.outtype = GPIO_OUT_TYPE_PP;
    GPIO_ConfigStruct.pin     = GPIO_PIN_0;
    GPIO_ConfigStruct.pupd    = GPIO_PUPD_NO;
    GPIO_ConfigStruct.speed   = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOB, &GPIO_ConfigStruct);

    

    /* config TMR3 */
    TMR_TimeBaseStruct.clockDivision = TMR_CKD_DIV1;                            /* Set clockDivision = 1 */
    TMR_TimeBaseStruct.counterMode = TMR_COUNTER_MODE_UP;                       /* Up-counter */
    TMR_TimeBaseStruct.div = 0;                                                 /* Set divider = 0, So TMR1 clock freq = 48MHZ, count 1 means 0.02us */
    TMR_TimeBaseStruct.period = IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_25US;   /* Set period 100us, 100us / (1/48) = 4800 */
    TMR_TimeBaseStruct.repetitionCounter = 0;                                   /* Repetition counter = 0x0 */
    TMR_ConfigTimeBase(TMR3, &TMR_TimeBaseStruct);

    /* Configure channel1 for read signal adc value*/
    OCcongigStruct.OC_Mode =  TMR_OC_MODE_PWM1;                     /* PWM1 mode */
    OCcongigStruct.OC_OutputState = TMR_OUTPUT_STATE_ENABLE;        /* Enable CH3 ouput */
    OCcongigStruct.OC_Polarity  = TMR_OC_POLARITY_LOW;              /* CH1  polarity is high */
    OCcongigStruct.Pulse = 20;                                     /* Set compare value, 120 -> 2.5us */
    TMR_OC1Config(TMR3, &OCcongigStruct);
    TMR_OC1PreloadConfig(TMR3, TMR_OC_PRELOAD_DISABLE);

    /* Config PA7 for timer3 channel2 output PWM for connect adc to VDD*/
    OCcongigStruct.OC_Mode =  TMR_OC_MODE_PWM1;                     /* PWM1 mode */
    OCcongigStruct.OC_OutputState = TMR_OUTPUT_STATE_ENABLE;        /* Enable CH3 ouput */
    OCcongigStruct.OC_Polarity  = TMR_OC_POLARITY_LOW;              /* CH2  polarity is high */
    OCcongigStruct.Pulse = 240;                                     /* Set compare value, 240 -> 5us */
    TMR_OC2Config(TMR3, &OCcongigStruct);
    TMR_OC2PreloadConfig(TMR3, TMR_OC_PRELOAD_DISABLE);

    /* Config PB0 for timer3 channel3 output PWM for open LED to transmit*/
    OCcongigStruct.OC_Mode =  TMR_OC_MODE_PWM1;                     /* PWM1 mode */
    //OCcongigStruct.OC_Idlestate  = TMR_OCIDLESTATE_RESET;         /* Idle State is reset */
    //OCcongigStruct.OC_NIdlestate = TMR_OCNIDLESTATE_RESET;        /* NIdle State is reset */
    //OCcongigStruct.OC_OutputNState = TMR_OUTPUT_NSTATE_DISABLE;   /* Enable CH3N ouput */
    OCcongigStruct.OC_OutputState = TMR_OUTPUT_STATE_ENABLE;        /* Enable CH3 ouput */
    OCcongigStruct.OC_Polarity  = TMR_OC_POLARITY_LOW;              /* CH3  polarity is low */
    //OCcongigStruct.OC_NPolarity = TMR_OC_NPOLARITY_HIGH;          /* CH3N polarity is high */
    OCcongigStruct.Pulse = 240;                                     /* Set compare value, 240 -> 5us */
    TMR_OC3Config(TMR3, &OCcongigStruct);
    TMR_OC3PreloadConfig(TMR3, TMR_OC_PRELOAD_DISABLE);

    /* Configure channel4 for read VDD adc value*/
    OCcongigStruct.OC_Mode =  TMR_OC_MODE_PWM1;                     /* PWM1 mode */
    OCcongigStruct.OC_OutputState = TMR_OUTPUT_STATE_ENABLE;        /* Enable CH3 ouput */
    OCcongigStruct.OC_Polarity  = TMR_OC_POLARITY_LOW;              /* CH4  polarity is high */
    OCcongigStruct.Pulse = 480;                                     /* Set compare value, 480 -> 10us */
    TMR_OC4Config(TMR3, &OCcongigStruct);
    TMR_OC4PreloadConfig(TMR3, TMR_OC_PRELOAD_DISABLE);

    /* Enable PWM output */
    TMR_EnablePWMOutputs(TMR3);

    /* Timer3 Commutation Interrupt */
    TMR_ClearStatusFlag(TMR3, TMR_INT_UPDATE|TMR_INT_CH1|TMR_INT_CH2|TMR_INT_CH3|TMR_INT_CH4);   //清除产生更新事件的标志位
    TMR_EnableInterrupt(TMR3, TMR_INT_UPDATE|TMR_INT_CH1|TMR_INT_CH2|TMR_INT_CH3|TMR_INT_CH4);
    NVIC_EnableIRQ(TMR3_IRQn);
    NVIC_EnableIRQRequest(TMR3_IRQn, 1);                    //-- 设置中断优先级，数字越大优先级越低
    
    /* Enable TMR3 */
    TMR_Enable(TMR3);
}

void iot_app_optical_timer14_pwm_init(void)
{
    //GPIO_Config_T  GPIO_ConfigStruct;
    TMR_TimeBase_T TMR_TimeBaseStruct;
    TMR_OCConfig_T OCcongigStruct;
    
    /* Enable Clock */
    //RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_GPIOB);
    //RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR14);

    /* Config TMR3 GPIO for output PWM */
    //GPIO_ConfigPinAF(GPIOB, GPIO_PIN_SOURCE_0, GPIO_AF_PIN0);
    //GPIO_ConfigStruct.mode    = GPIO_MODE_AF;
    //GPIO_ConfigStruct.outtype = GPIO_OUT_TYPE_PP;
    //GPIO_ConfigStruct.pin     = GPIO_PIN_0;
    //GPIO_ConfigStruct.pupd    = GPIO_PUPD_NO;
    //GPIO_ConfigStruct.speed   = GPIO_SPEED_50MHz;
    //GPIO_Config(GPIOB, &GPIO_ConfigStruct);

    /* config TMR3 */
    TMR_TimeBaseStruct.clockDivision = TMR_CKD_DIV1;                    /* Set clockDivision = 1 */
    TMR_TimeBaseStruct.counterMode = TMR_COUNTER_MODE_UP;               /* Up-counter */
    TMR_TimeBaseStruct.div = 0;                                         /* Set divider = 0, So TMR1 clock freq = 48MHZ, count 1 means 0.02us */
    TMR_TimeBaseStruct.period = IOT_APP_OPTICAL_TIMER14_PERIOD_VALUE;   /* Set period 100us, 100us / (1/48) = 4800 */
    TMR_TimeBaseStruct.repetitionCounter = 0;                           /* Repetition counter = 0x0 */
    TMR_ConfigTimeBase(TMR14, &TMR_TimeBaseStruct);

    /* Configure channel3 */
    OCcongigStruct.OC_Mode =  TMR_OC_MODE_PWM1;                 /* PWM1 mode */
    OCcongigStruct.OC_Idlestate  = TMR_OCIDLESTATE_RESET;       /* Idle State is reset */
    OCcongigStruct.OC_NIdlestate = TMR_OCNIDLESTATE_RESET;      /* NIdle State is reset */
    OCcongigStruct.OC_OutputNState = TMR_OUTPUT_NSTATE_DISABLE; /* Enable CH3N ouput */
    OCcongigStruct.OC_OutputState = TMR_OUTPUT_STATE_ENABLE;    /* Enable CH3 ouput */
    OCcongigStruct.OC_Polarity  = TMR_OC_POLARITY_HIGH;         /* CH3  polarity is high */
    OCcongigStruct.OC_NPolarity = TMR_OC_NPOLARITY_HIGH;        /* CH3N polarity is high */
    OCcongigStruct.Pulse = IOT_APP_OPTICAL_ATT_TIMER14_PWM_CC1_VALUE_MIN;/* Set compare value, 960 -> 1/5 20us */
    TMR_OC1Config(TMR14, &OCcongigStruct);
    //TMR_OC1PreloadConfig(TMR14, TMR_OC_PRELOAD_DISABLE);

    /* Enable PWM output */
    TMR_EnablePWMOutputs(TMR14);

    /* Timer14 Commutation Interrupt */
    TMR_ClearStatusFlag(TMR14,TMR_INT_UPDATE|TMR_INT_CH1);   //清除产生更新事件的标志位
    TMR_EnableInterrupt(TMR14, TMR_INT_UPDATE|TMR_INT_CH1);
    NVIC_EnableIRQ(TMR14_IRQn);
    NVIC_EnableIRQRequest(TMR14_IRQn, 0);                    //-- 设置中断优先级，数字越大优先级越低
    
    /* Enable TMR14 */
    TMR_Enable(TMR14);
}

void iot_app_optical_timer2_pwm_init(void)
{
    TMR_TimeBase_T TMR_TimeBaseStruct;
    
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2);

    TMR_TimeBaseStruct.clockDivision = TMR_CKD_DIV2;            /* Set clockDivision = 1 */
    TMR_TimeBaseStruct.counterMode = TMR_COUNTER_MODE_UP;       /* Up-counter */
    TMR_TimeBaseStruct.div = 0;                                 /* Set divider = 0, So TMR1 clock freq = 48MHZ, count 1 means 0.02us */
    TMR_TimeBaseStruct.period = 48000;                          /* Set period 2ms, 100us / (1/24) = 2400 */
    TMR_TimeBaseStruct.repetitionCounter = 0;                   /* Repetition counter = 0x0 */
    TMR_ConfigTimeBase(TMR2, &TMR_TimeBaseStruct);

    /* Timer14 Commutation Interrupt */
    TMR_ClearStatusFlag(TMR2,TMR_INT_UPDATE);                   //清除产生更新事件的标志位
    TMR_EnableInterrupt(TMR2, TMR_INT_UPDATE);
    NVIC_EnableIRQ(TMR2_IRQn);
    NVIC_EnableIRQRequest(TMR2_IRQn, 1);                        //-- 设置中断优先级，数字越大优先级越低
    
    TMR_Enable(TMR2);
}

void iot_app_optical_set_att(uint8 mode, int8 value)
{
    if (value >= 0)
    {
        if (mode == IOT_APP_OPTICAL_MODE_HIGH_SPEED)
        {
            iot_app_optical_timer14_pwm_cc_value = IOT_APP_OPTICAL_HSP_ATT_TIMER14_PWM_CC1_VALUE_MAX - value * IOT_APP_OPTICAL_HSP_ATT_TIMER14_PWM_CC1_VALUE_STEP;
        }
        else
        {
            iot_app_optical_timer14_pwm_cc_value = IOT_APP_OPTICAL_ATT_TIMER14_PWM_CC1_VALUE_MAX - value * IOT_APP_OPTICAL_ATT_TIMER14_PWM_CC1_VALUE_STEP;
        }
    }
    else
    {
        //-- 用于自动调节时可以调更低的pwm波形，实际在各个模式里手动调节不能进这里，手动调节限制于0-100

        value = 0 - value;

        if (mode == IOT_APP_OPTICAL_MODE_HIGH_SPEED)
        {
            iot_app_optical_timer14_pwm_cc_value = IOT_APP_OPTICAL_HSP_ATT_TIMER14_PWM_CC1_VALUE_MAX;
            iot_app_optical_timer14_pwm_cc_value += value * (IOT_APP_OPTICAL_TIMER14_PWM_CC_VALUE_MAX - IOT_APP_OPTICAL_HSP_ATT_TIMER14_PWM_CC1_VALUE_MAX)/100;
        }
        else
        {
            iot_app_optical_timer14_pwm_cc_value = IOT_APP_OPTICAL_ATT_TIMER14_PWM_CC1_VALUE_MAX;
            iot_app_optical_timer14_pwm_cc_value += value * (IOT_APP_OPTICAL_TIMER14_PWM_CC_VALUE_MAX - IOT_APP_OPTICAL_ATT_TIMER14_PWM_CC1_VALUE_MAX)/100;
        }
    }

    TMR_SetCompare1(TMR14, iot_app_optical_timer14_pwm_cc_value);
}

void iot_app_optical_set_mode(uint8 auto_next, uint8 mode)
{
    /*
    MEGA       : 200us + 100us + 100us + 100us, pulse 5us low
    SUPER      : 100us + 50us  + 50us  + 50us,  pulse 5us low
    FINE       : 100us + 50us  + 50us  + 50us,  pulse 5us low
    HSP        : 50us  + 25us  + 25us  + 25us,  pulse 5us low
    */

    int8 current_att = iot_app_optical_current_att;

    if (auto_next)
    {
        mode = ++iot_app_optical_current_mode % IOT_APP_OPTICAL_MODE_MAX;
    }
    
    switch (mode)
    {
    case IOT_APP_OPTICAL_MODE_HIGH_SPEED:
        {
            iot_app_optical_timer3_period_short = IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_25US;
            iot_app_optical_timer3_period_long  = IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_50US;
        }
        break;

    case IOT_APP_OPTICAL_MODE_FINE:
        {
            iot_app_optical_timer3_period_short = IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_50US;
            iot_app_optical_timer3_period_long  = IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_100US;
        }
        break;

    case IOT_APP_OPTICAL_MODE_SUPER:
        {
            iot_app_optical_timer3_period_short = IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_50US;
            iot_app_optical_timer3_period_long  = IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_100US;
        }
        break;

    case IOT_APP_OPTICAL_MODE_MEGA:
        {
            iot_app_optical_timer3_period_short = IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_100US;
            iot_app_optical_timer3_period_long  = IOT_APP_OPTICAL_TIMER3_PWM_PERIOD_VALUE_200US;
        }
        break;
    }

    if (iot_app_optical_auto_adjust_att < 0)
    {
        current_att = iot_app_optical_auto_adjust_att;
    }

    iot_app_optical_set_att(mode, current_att);
}

void iot_app_optical_adc_dma_init(void)
{
    /* DMA Configure */
    DMA_Config_T dmaConfig;

    /* Enable DMA clock */
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA1);

    /* Set Peripheral Address */
    dmaConfig.peripheralAddress = (uint32_t)&ADC->DATA;//-- (uint32_t)ADC_BASE + 0x40;
    /* Set memory Address */
    dmaConfig.memoryAddress = (uint32_t)&iot_app_optical_adc_data;
    /* read from peripheral */
    dmaConfig.direction = DMA_DIR_PERIPHERAL;
    /* size of buffer*/
    dmaConfig.bufferSize = IOT_APP_OPTICAL_ADC_DATA_BUF_SIZE;
    /* Disable Peripheral Address increase */
    dmaConfig.peripheralInc = DMA_PERIPHERAL_INC_DISABLE;
    /* Enable Memory Address increase */
    dmaConfig.memoryInc = DMA_MEMORY_INC_ENABLE;
    /* Set peripheral Data Size */
    dmaConfig.peripheralDataSize = DMA_PERIPHERAL_DATASIZE_HALFWORD;
    /* set memory Data Size */
    dmaConfig.memoryDataSize = DMA_MEMORY_DATASIZE_HALFWORD;
    /* Reset Circular Mode */
    dmaConfig.circular = DMA_CIRCULAR_ENABLE;
    /* Disable M2M */
    dmaConfig.memoryTomemory = DMA_M2M_DISABLE;
    /* set priority */
    dmaConfig.priority = DMA_PRIORITY_LEVEL_LOW;

    DMA_Config(DMA1_CHANNEL_1, &dmaConfig);

    DMA_EnableInterrupt(DMA1_CHANNEL_1, DMA_INT_HTIE);
    NVIC_EnableIRQRequest(DMA1_CH1_IRQn, 3);            //-- 设置中断优先级，数字越大优先级越低

    DMA_Enable(DMA1_CHANNEL_1);
}

void iot_app_optical_adc_init(void)
{
    GPIO_Config_T gpioConfig;
    ADC_Config_T  adcConfig;

    /* RCM Enable*/
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_GPIOB);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC1);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);
    
    /* GPIO Configuration */
    gpioConfig.pin = GPIO_PIN_1;
    gpioConfig.mode = GPIO_MODE_AN;
    gpioConfig.pupd = GPIO_PUPD_NO;
    GPIO_Config(GPIOB, &gpioConfig);
    
    /* ADC Configuration */
    ADC_Reset();
    ADC_ConfigStructInit(&adcConfig);
    
    adcConfig.resolution  = ADC_RESOLUTION_12B;         /* Set resolution*/
    adcConfig.dataAlign   = ADC_DATA_ALIGN_RIGHT;       /* Set dataAlign*/
    adcConfig.scanDir     = ADC_SCAN_DIR_UPWARD;        /* Set scanDir*/
    adcConfig.convMode    = ADC_CONVERSION_SINGLE;      /* Set convMode*/
    adcConfig.extTrigConv = ADC_EXT_TRIG_CONV_TRG0;     /* Set extTrigConv*/
    adcConfig.extTrigEdge = ADC_EXT_TRIG_EDGE_NONE;     /* Set TrigEdge*/

    ADC_Config(&adcConfig);
    ADC_ConfigChannel(ADC_CHANNEL_9, ADC_SAMPLE_TIME_13_5);

    
    //iot_app_optical_adc_dma_init();
    //ADC_EnableDMA();
    //ADC_DMARequestMode(ADC_DMA_MODE_CIRCULAR);
    
    /* Enable Interrupt*/
    ADC_EnableInterrupt(ADC_INT_CC);
    NVIC_EnableIRQRequest(ADC1_COMP_IRQn, 2);           //-- 设置中断优先级，数字越大优先级越低

    /* Calibration*/
    ADC_ReadCalibrationFactor();
    
    /* Enable ADC*/
    ADC_Enable();

    while (!ADC_ReadStatusFlag(ADC_FLAG_ADRDY));

    //ADC_StartConversion();

    iot_app_optical_adc_init_flag = 1;
}

uint8 iot_app_optical_key_callback(uint8 cur_keys, uint8 pre_keys, uint32 poll_time_milliseconds)
{
    uint8  k;
    uint8  key_mask = HAL_KEY_1;
    uint8  scan_flag = 1;
    uint8  press_keys = 0;
    uint8  hold_keys = 0;
	
    cur_keys &= IOT_APP_OPTICAL_KEY_MASK;
    pre_keys &= IOT_APP_OPTICAL_KEY_MASK;

    for (k = 0; k < 8; k++,key_mask<<=1)
    {
        IOT_WATCHDOG_RESET();

        if (!(key_mask & IOT_APP_OPTICAL_KEY_MASK))
        {
            continue;
        }
        
        if (cur_keys & key_mask)//-- 按键按下
        {
            //if (!(pre_keys & key_mask))
            //{
            //    printf("KEY%d Press\r\n",k+1);
            //}
            //else
            //{
            //    printf("KEY%d Hold:%d\r\n", k+1, hal_key_press_time_count[k]);
            //}

            if (hal_key_press_time_count[k] == 1)
            {
                press_keys |= key_mask;
            }
            else if (hal_key_press_time_count[k] > 2)
            {
                hold_keys |= key_mask;
            }
        }
        else
        {
        }
    }

//    if (press_keys & HAL_KEY_RIGHT_SUB)
//    {
//        //-- TODO
//        iot_app_optical_att_sub();
//    }

//    if (hold_keys & HAL_KEY_RIGHT_SUB)
//    {
//        //-- TODO
//        iot_app_optical_att_sub();
//    }

//    if (press_keys & HAL_KEY_LEFT_ADD)
//    {
//        //-- TODO
//        iot_app_optical_att_add();
//    }

//    if (hold_keys & HAL_KEY_LEFT_ADD)
//    {
//        //-- TODO
//        iot_app_optical_att_add();
//    }
//	
//    if (press_keys & HAL_KEY_SET)
//    {
//        //-- TODO
//        ADC_StartConversion();
//    }
		
    
    return scan_flag;
}

uint16 iot_app_optical_process_event(uint8 task_id, uint16 events)
{
    (void)task_id; // 显式忽略未引用参数（避免编译器警告）
    
    // 处理系统消息事件
    if ( events & SYS_EVENT_MSG )
    {
        // 接收消息队列中的消息
        uint8* msg_packet = (uint8 *)osal_msg_receive(iot_app_optical_task_id);
        while ( msg_packet )
        {
            // 根据消息事件类型分发处理
            //switch ( msg_packet->hdr.event )
            //{
            //case KEY_CHANGE://按键事件处理
            // //UserApp_HandleKeys( ((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
            // break;

            //default:
            // break;
            //}

            // 释放消息内存
            osal_msg_deallocate((uint8 *)msg_packet);

            // 接收下一条消息
            msg_packet = (uint8 *)osal_msg_receive(iot_app_optical_task_id);
        }

        // 返回未处理的事件（清除已处理的SYS_EVENT_MSG）
        return (events ^ SYS_EVENT_MSG);
    }

    if (events & IOT_APP_OPTICAL_EVENT_1)
    {
        // 定时器事件处理，刷新adc显示
        if (adc_update_en)
        {
            display_digit_right(iot_app_optical_current_lux);   
        }

        //-- 70ms勉强能看，对比测试友商程序，大概为100ms-200ms刷新一次
        osal_start_timerEx(iot_app_optical_task_id, IOT_APP_OPTICAL_EVENT_1, 200);
        return events ^ IOT_APP_OPTICAL_EVENT_1;
    }

    // 丢弃未知事件
    return 0;
}

void iot_app_optical_poll(void)
{
}

void iot_app_optical_init(uint8 task_id)
{
    iot_app_optical_task_id = task_id;

    //IOT_APP_OPTICAL_TRANMIT_INIT();
    //IOT_APP_OPTICAL_TRANMIT_HI_OFF();

    //IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_INIT();
    //IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_OFF();
    
    IOT_APP_OPTICAL_ATT_INIT();
    IOT_APP_OPTICAL_ATT_HI();
    IOT_APP_OPTICAL_ATT_LOW();

    iot_app_optical_adc_init();
    iot_app_optical_timer2_pwm_init();
    iot_app_optical_timer3_pwm_init();
    iot_app_optical_timer14_pwm_init();

    iot_app_optical_set_mode(0, iot_app_optical_current_mode);

    //-- only for test
    #ifdef IOT_APP_OPTICAL_TEST_IO_PIN
    IOT_APP_OPTICAL_TEST_IO_INIT();
    IOT_APP_OPTICAL_TEST_IO_OFF();
    #endif
    
    osal_start_timerEx(iot_app_optical_task_id, IOT_APP_OPTICAL_EVENT_1, 1);
    HalKeyCallbackRegister(iot_app_optical_key_callback);
}


/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif 

