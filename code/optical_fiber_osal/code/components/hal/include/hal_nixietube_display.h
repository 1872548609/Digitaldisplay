#ifndef HAL_NIXIETTUBE_DISPLAY_H
#define HAL_NIXIETTUBE_DISPLAY_H

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/
#include "main.h"

/*************************************************-*************************************************
*                                         定义段选/位选引脚
**************************************************-*****************^******************************/
#define SEPARATE_PORT                                               1
    
#ifdef SEPARATE_PORT
    
    //定义段选/位选引脚
	#define SEG_PORT                                                GPIOA
	#define DIG_PORT1                                               GPIOB
	#define DIG_PORT2                                               GPIOF
	
    //外设端口
	#define SEG_PORT_PERIPH_PORT                                    RCM_AHB_PERIPH_GPIOA
	#define DIG_PORT1_PERIPH_PORT                                   RCM_AHB_PERIPH_GPIOB
	#define DIG_PORT2_PERIPH_PORT                                   RCM_AHB_PERIPH_GPIOF
        
#else
    
    //定义段选/位选引脚
	#define SEG_PORT                                                GPIOA
	#define DIG_PORT1                                               GPIOB
	
    //外设端口
	#define SEG_PORT_PERIPH_PORT                                    RCM_AHB_PERIPH_GPIOA
	#define DIG_PORT1_PERIPH_PORT                                   RCM_AHB_PERIPH_GPIOB
        
#endif
    
    
// 段选引脚（a~g）
#define SEG_A                                                       GPIO_PIN_8
#define SEG_B                                                       GPIO_PIN_9
#define SEG_C                                                       GPIO_PIN_10
#define SEG_D                                                       GPIO_PIN_11
#define SEG_E                                                       GPIO_PIN_12
#define SEG_LED                                                     GPIO_PIN_15
// 如果解除这个注释就会显示完整的数码管，因为这两个脚是用来下载程序的，我懒得等初始化就注释了
//#define SEG_F                                                       GPIO_PIN_13
//#define SEG_G                                                       GPIO_PIN_14
#define SEG_F                                                       0
#define SEG_G                                                       0

    
// 位选引脚（8个数码管）
#define DIG_1                                                       GPIO_PIN_3
#define DIG_2                                                       GPIO_PIN_4
#define DIG_3                                                       GPIO_PIN_5
#define DIG_4                                                       GPIO_PIN_6
#define DIG_5                                                       GPIO_PIN_7
#define DIG_6                                                       GPIO_PIN_8     
#define DIG_7                                                       GPIO_PIN_0
#define DIG_8                                                       GPIO_PIN_1

/*************************************************-*************************************************
*                                        大小写字母选项定义
**************************************************-*****************^******************************/
#define alphabet_A                                                  0
#define alphabet_B                                                  1
#define alphabet_C                                                  2
#define alphabet_D                                                  3
#define alphabet_E                                                  4
#define alphabet_F                                                  5
#define alphabet_G                                                  6
#define alphabet_H                                                  7
#define alphabet_I                                                  8
#define alphabet_J                                                  9
#define alphabet_K                                                  10
#define alphabet_L                                                  11
#define alphabet_M                                                  12
#define alphabet_N                                                  13
#define alphabet_O                                                  14
#define alphabet_P                                                  15
#define alphabet_Q                                                  16
#define alphabet_R                                                  17
#define alphabet_S                                                  18
#define alphabet_T                                                  19
#define alphabet_U                                                  20
#define alphabet_V                                                  21
#define alphabet_W                                                  22
#define alphabet_X                                                  23
#define alphabet_Y                                                  24
#define alphabet_Z                                                  25

#define alphabet_a                                                  26
#define alphabet_b                                                  27
#define alphabet_c                                                  28
#define alphabet_d                                                  29
#define alphabet_e                                                  30
#define alphabet_f                                                  31
#define alphabet_g                                                  32
#define alphabet_h                                                  33
#define alphabet_i                                                  34
#define alphabet_j                                                  35
#define alphabet_k                                                  36
#define alphabet_l                                                  37
#define alphabet_m                                                  38
#define alphabet_n                                                  39
#define alphabet_o                                                  40
#define alphabet_p                                                  41
#define alphabet_q                                                  42
#define alphabet_r                                                  43
#define alphabet_s                                                  44
#define alphabet_t                                                  45
#define alphabet_u                                                  46
#define alphabet_v                                                  47
#define alphabet_w                                                  48
#define alphabet_x                                                  49
#define alphabet_y                                                  50
#define alphabet_z                                                  51

/*************************************************-*************************************************
*                                        特殊符号选项定义
**************************************************-*****************^******************************/
#define sign_1                                                      0
#define sign_2                                                      1
#define sign_3                                                      2
#define sign_4                                                      3

/*************************************************-*************************************************
*                                              MACRO
**************************************************-*****************^******************************/
#define HAL_BLED_SET(x)	                                            {LED1_EN = x;}
#define HAL_RLED_SET(x)	                                            {LED2_EN = x;}
#define HAL_GLED_SET(x)	                                            {LED3_EN = x;}


#define DISPLAY_CLEAR_ALL_DIG()                                     {DIG_PORT1->BSC = (uint32_t)(DIG_1 | DIG_2 | DIG_3 | DIG_4 | DIG_5 | DIG_6);DIG_PORT2->BSC = (uint32_t)(DIG_7 | DIG_8);}
#define DISPLAY_CLEAR_ALL_SEG()                                     {SEG_PORT->BSC = (uint32_t)(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G);}

/*************************************************-*************************************************
*                                          GLOBAL VARIABLES
**************************************************-*****************^******************************/
extern uint8 LED1_EN;
extern uint8 LED2_EN;
extern uint8 LED3_EN;


/*************************************************-*************************************************
*                                             FUNCTION
**************************************************-*****************^******************************/
extern void iot_nixie_init(void);     // 初始化数码管

extern void display_nixie(uint8_t num, uint8_t digit);   // 某位显示数字

extern void display_alphabet(uint8_t alphabet, uint8_t digit);

extern void display_modeset(uint8_t mode);

extern void display_set_single_en(uint8_t pos);

extern void display_set_right_en(bool ifenabled) ;

extern void display_set_left_en(bool ifenabled) ;

extern void display_set_global_enable(bool ifenabled);

extern void display_scan(void);    // 动态扫描（建议调用频率 >100Hz）

extern void display_set_digit(uint8_t pos, uint8_t num) ;

extern void display_set_alphabet(uint8_t pos, uint8_t alphabet);

//extern void display_clearalldig(void);

//extern void display_clearallseg(void);

extern void display_digit(uint32 num);

extern void display_digit_left(uint16 num);

extern void display_digit_right(uint16 num);

extern void display_string(char *string);

extern void display_alphabet_left(char *string);

extern void display_alphabet_right(char *string);

extern void display_stringbyarea(uint8 colum, char *string, uint8 area);


/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#endif

