#include "hal_adc.h"

uint16_t adc_buffer[WINDOW_SIZE] = {0};
uint8_t ad_index = 0;
uint32_t adc_sum = 0; 			// adc累加
uint32_t adcData = 0;           // ad值
uint32_t vol_value = 0;         // 电压值，参考3.3v
uint32_t adc_temp=0;
uint8_t adc_finish = 0;


void hal_adc_init(void)
{
    //初始化ADC引脚 PA6
    GPIO_Config_T gpioConfig;
    ADC_Config_T  adcConfig;
    /* RCM Enable*/	
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_GPIOA);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC1);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);
    /* GPIO Configuration */
    gpioConfig.pin = GPIO_PIN_6;
    gpioConfig.mode = GPIO_MODE_AN;
    gpioConfig.pupd = GPIO_PUPD_NO;
    GPIO_Config(GPIOA, &gpioConfig);
    /* ADC Configuration */
    
    ADC_Reset();                                
    
    ADC_ConfigStructInit(&adcConfig);
    adcConfig.resolution = ADC_RESOLUTION_12B;        //分辨率
    adcConfig.dataAlign  = ADC_DATA_ALIGN_RIGHT;      //数据对齐
    adcConfig.scanDir    = ADC_SCAN_DIR_UPWARD;       //扫描方向
    adcConfig.convMode   = ADC_CONVERSION_CONTINUOUS; //转换模式
    adcConfig.extTrigConv  = ADC_EXT_TRIG_CONV_TRG0;  	//外部触发
    adcConfig.extTrigEdge    = ADC_EXT_TRIG_EDGE_NONE; //禁用外部触发
    ADC_Config(&adcConfig);
    			
    ADC_ConfigChannel(ADC_CHANNEL_6, ADC_SAMPLE_TIME_239_5);
    
    NVIC_EnableIRQRequest(ADC1_COMP_IRQn,2);
    	
    ADC_EnableInterrupt(ADC_INT_CS);
    			
    ADC_ReadCalibrationFactor();
    			
    ADC_Enable();
    			
    ADC_StartConversion();
}


void ADC1_COMP_IRQHandler(void)
{
    adc_isr(); 
}

uint8 adc_if_ready(void)
{
    return adc_finish;
}

uint32_t adc_read(void)
{
    adc_finish = 0;
    
    return adcData;
}

//com 为采样的原始数值
//iData 经过一阶滤波后的采样值
unsigned int adc_lowv( unsigned int com )
{
    static unsigned int iLastData;    //上一次值
    unsigned int iData;               //本次计算值
    float dPower = 0.1;               //滤波系数
    iData = ( com * dPower ) + ( 1 - dPower ) * iLastData; //计算
    iLastData = iData;                                     //存贮本次数据
    return iData;                                         //返回数据
}

// 气压传感器adc读取中断
void adc_isr(void)
{
    if (ADC_ReadIntFlag(ADC_INT_FLAG_CS) == SET)
    {
        ADC_ClearIntFlag(ADC_INT_FLAG_CS);

        adc_temp =ADC_ReadConversionValue();

        adcData = adc_lowv(adc_temp);
        
        vol_value=(adcData * 3300)/4095;
        
        adc_finish = 1;
    }
}
