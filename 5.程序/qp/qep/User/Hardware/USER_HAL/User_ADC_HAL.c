#include "User_ADC_Hal.h"

void HAL_ADC_Init(void)
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
    //ADC_Isr(); 
}


