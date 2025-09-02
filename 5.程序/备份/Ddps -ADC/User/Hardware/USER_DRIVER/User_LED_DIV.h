#ifndef _USER_LED_DIV_H
#define _USER_LED_DIV_H
                              
#include "superhead.h"


//LEDHAL层驱动
#define THRERED_ON 				HAL_LED_On(THREREDLED_PORT,THREREDLED_PIN);
#define THRERED_OFF 			HAL_LED_Off(THREREDLED_PORT,THREREDLED_PIN);

#define THREERED_FLASH HAL_LED_Toggle(THREREDLED_PORT,THREREDLED_PIN);

#define ONERED_ON						HAL_LED_On(ONEREDLED_PORT,ONEREDLED_PIN);
#define ONERED_OFF 			 HAL_LED_Off(ONEREDLED_PORT,ONEREDLED_PIN);

#define DOUBGED_ON   		HAL_LED_On(DOUBGEDLED_PORT,DOUBGEDLED_PIN);
#define DOUBGED_OFF    HAL_LED_Off(DOUBGEDLED_PORT,DOUBGEDLED_PIN);

#define ONEGED_ON      HAL_LED_On(ONEGEDLED_PORT,ONEGEDLED_PIN);
#define ONEGED_OFF     HAL_LED_Off(ONEGEDLED_PORT,ONEGEDLED_PIN);

#define ALLLED_ON      HAL_LED_On(ONEGEDLED_PORT,THREREDLED_PIN|   \
																																																	ONEREDLED_PIN|    \
																																																	DOUBGEDLED_PIN|   \
																																																	ONEGEDLED_PIN);   
#define ALLLED_OFF	    HAL_LED_Off(ONEGEDLED_PORT,THREREDLED_PIN|  \
																																																	ONEREDLED_PIN|    \
																																																	DOUBGEDLED_PIN|   \
																																																	ONEGEDLED_PIN);   

typedef enum {
    THREERED  =  0,
	   ONERED 			=  1,
				DOUBGED 		=  2,
				ONEGED 			= 	3,
				ALLLED    =  4,
} Led_id;

typedef struct {  
	
				union{
						uint8_t unionspace;	
						struct
						{
							uint8_t state :1;
							uint8_t blink :1;
						}Crt_Bit;
				}LED_Crt;
	   
				Led_id LED_ID;
				
} LED_Management;

//使用控制块控制LED显示
//使用控制块操作
//ALLLED_M.LED_Crt.Crt_Bit.state=1;    //点亮
//ALLLED_M.LED_Crt.Crt_Bit.blink=1;    //翻转
extern LED_Management  THREERE_M;
extern LED_Management  ONERED_M;
extern LED_Management  DOUBGED_M;
extern LED_Management  ONEGED_M;
extern LED_Management  ALLLED_M;
//初始化
void DIV_LED_Init(void);
//将控制函数丢进更新函数里
void DIV_LED_Control(LED_Management MA);
void DIV_LED_TURN(LED_Management MA);

#endif
