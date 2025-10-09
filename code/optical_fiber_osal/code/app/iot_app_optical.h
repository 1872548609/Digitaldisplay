
#ifndef _IOT_APP_OPTICAL_H
#define _IOT_APP_OPTICAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                              DEFINES
**************************************************-*****************^******************************/
typedef struct _iot_app_optical_adc_to_lux
{
    uint16 adc_min;
    uint16 adc_max;
    uint16 lux_min;
    uint16 lux_max;
}iot_app_optical_adc_to_lux_t;

/*************************************************-*************************************************
*                                              MACRO
**************************************************-*****************^******************************/
#define IOT_APP_OPTICAL_ATT_PORT                                    GPIOA
#define IOT_APP_OPTICAL_ATT_PIN                                     GPIO_PIN_5
#define IOT_APP_OPTICAL_ATT_INIT()                                  {iot_driver_gpio_init_output(IOT_APP_OPTICAL_ATT_PORT, IOT_APP_OPTICAL_ATT_PIN);}
#define IOT_APP_OPTICAL_ATT_HI()                                    {IOT_APP_OPTICAL_ATT_PORT->BSC = (uint32_t)IOT_APP_OPTICAL_ATT_PIN;}
#define IOT_APP_OPTICAL_ATT_LOW()                                   {IOT_APP_OPTICAL_ATT_PORT->BR = (uint32_t)IOT_APP_OPTICAL_ATT_PIN;}
//#define IOT_APP_OPTICAL_ATT_HI()                                  {GPIO_SetBit(IOT_APP_OPTICAL_ATT_PORT, IOT_APP_OPTICAL_ATT_PIN);}
//#define IOT_APP_OPTICAL_ATT_LOW()                                 {GPIO_ClearBit(IOT_APP_OPTICAL_ATT_PORT, IOT_APP_OPTICAL_ATT_PIN);}

#define IOT_APP_OPTICAL_TRANMIT_PORT                                GPIOB
#define IOT_APP_OPTICAL_TRANMIT_PIN                                 GPIO_PIN_0
#define IOT_APP_OPTICAL_TRANMIT_INIT()                              {iot_driver_gpio_init_output(IOT_APP_OPTICAL_TRANMIT_PORT, IOT_APP_OPTICAL_TRANMIT_PIN);}
#define IOT_APP_OPTICAL_TRANMIT_HI_OFF()                                {IOT_APP_OPTICAL_TRANMIT_PORT->BSC = (uint32_t)IOT_APP_OPTICAL_TRANMIT_PIN;}
#define IOT_APP_OPTICAL_TRANMIT_LOW_ON()                               {IOT_APP_OPTICAL_TRANMIT_PORT->BR = (uint32_t)IOT_APP_OPTICAL_TRANMIT_PIN;}
//#define IOT_APP_OPTICAL_TRANMIT_HI_OFF()                                {GPIO_ClearBit(IOT_APP_OPTICAL_TRANMIT_PORT, IOT_APP_OPTICAL_TRANMIT_PIN);}
//#define IOT_APP_OPTICAL_TRANMIT_LOW_ON()                               {GPIO_SetBit(IOT_APP_OPTICAL_TRANMIT_PORT, IOT_APP_OPTICAL_TRANMIT_PIN);}

#define IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PORT                    GPIOA
#define IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PIN                     GPIO_PIN_7
#define IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_INIT()                  {iot_driver_gpio_init_output(IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PORT, IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PIN);}
#define IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_ON()                    {IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PORT->BSC = (uint32_t)IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PIN;}
#define IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_OFF()                   {IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PORT->BR = (uint32_t)IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PIN;}
//#define IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_ON()                    {GPIO_SetBit(IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PORT, IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PIN);}
//#define IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_OFF()                   {GPIO_ClearBit(IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PORT, IOT_APP_OPTICAL_INPUT_SWITCH_TO_VDD_PIN);}

//#define IOT_APP_OPTICAL_TEST_IO_PORT                                GPIOB
//#define IOT_APP_OPTICAL_TEST_IO_PIN                                 GPIO_PIN_2
//#define IOT_APP_OPTICAL_TEST_IO_INIT()                              {iot_driver_gpio_init_output(IOT_APP_OPTICAL_TEST_IO_PORT, IOT_APP_OPTICAL_TEST_IO_PIN);}
//#define IOT_APP_OPTICAL_TEST_IO_ON()                                {IOT_APP_OPTICAL_TEST_IO_PORT->BSC = (uint32_t)IOT_APP_OPTICAL_TEST_IO_PIN;}
//#define IOT_APP_OPTICAL_TEST_IO_OFF()                               {IOT_APP_OPTICAL_TEST_IO_PORT->BR = (uint32_t)IOT_APP_OPTICAL_TEST_IO_PIN;}
//#define IOT_APP_OPTICAL_TEST_IO_OUTPUT_STATE()                      (IOT_APP_OPTICAL_TEST_IO_PORT->ODATA & IOT_APP_OPTICAL_TEST_IO_PIN)

/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
#define IOT_APP_OPTICAL_EVENT_1                                     0x0001

/*************************************************-*************************************************
*                                             FUNCTION
**************************************************-*****************^******************************/
extern void   iot_app_optical_init(uint8 task_id);
extern void   iot_app_optical_poll(void);
extern uint16 iot_app_optical_process_event(uint8 task_id, uint16 events);
extern void   iot_app_optical_set_att(uint8 mode, int8 value);
extern void   iot_app_optical_set_mode(uint8 auto_next, uint8 mode);


/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif

