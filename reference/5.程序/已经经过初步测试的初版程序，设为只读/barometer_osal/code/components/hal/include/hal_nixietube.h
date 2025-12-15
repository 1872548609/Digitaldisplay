
#ifndef HAL_NIXIETTUBE_H
#define HAL_NIXIETTUBE_H

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                        大小写字母选项定义
**************************************************-*****************^******************************/
#define HAL_NIXIETUBE_ALPHABET_A                                    0
#define HAL_NIXIETUBE_ALPHABET_B                                    1
#define HAL_NIXIETUBE_ALPHABET_C                                    2
#define HAL_NIXIETUBE_ALPHABET_D                                    3
#define HAL_NIXIETUBE_ALPHABET_E                                    4
#define HAL_NIXIETUBE_ALPHABET_F                                    5
#define HAL_NIXIETUBE_ALPHABET_G                                    6
#define HAL_NIXIETUBE_ALPHABET_H                                    7
#define HAL_NIXIETUBE_ALPHABET_I                                    8
#define HAL_NIXIETUBE_ALPHABET_J                                    9
#define HAL_NIXIETUBE_ALPHABET_K                                    10
#define HAL_NIXIETUBE_ALPHABET_L                                    11
#define HAL_NIXIETUBE_ALPHABET_M                                    12
#define HAL_NIXIETUBE_ALPHABET_N                                    13
#define HAL_NIXIETUBE_ALPHABET_O                                    14
#define HAL_NIXIETUBE_ALPHABET_P                                    15
#define HAL_NIXIETUBE_ALPHABET_Q                                    16
#define HAL_NIXIETUBE_ALPHABET_R                                    17
#define HAL_NIXIETUBE_ALPHABET_S                                    18
#define HAL_NIXIETUBE_ALPHABET_T                                    19
#define HAL_NIXIETUBE_ALPHABET_U                                    20
#define HAL_NIXIETUBE_ALPHABET_V                                    21
#define HAL_NIXIETUBE_ALPHABET_W                                    22
#define HAL_NIXIETUBE_ALPHABET_X                                    23
#define HAL_NIXIETUBE_ALPHABET_Y                                    24
#define HAL_NIXIETUBE_ALPHABET_Z                                    25

#define HAL_NIXIETUBE_ALPHABET_a                                    26
#define HAL_NIXIETUBE_ALPHABET_b                                    27
#define HAL_NIXIETUBE_ALPHABET_c                                    28
#define HAL_NIXIETUBE_ALPHABET_d                                    29
#define HAL_NIXIETUBE_ALPHABET_e                                    30
#define HAL_NIXIETUBE_ALPHABET_f                                    31
#define HAL_NIXIETUBE_ALPHABET_g                                    32
#define HAL_NIXIETUBE_ALPHABET_h                                    33
#define HAL_NIXIETUBE_ALPHABET_i                                    34
#define HAL_NIXIETUBE_ALPHABET_j                                    35
#define HAL_NIXIETUBE_ALPHABET_k                                    36
#define HAL_NIXIETUBE_ALPHABET_l                                    37
#define HAL_NIXIETUBE_ALPHABET_m                                    38
#define HAL_NIXIETUBE_ALPHABET_n                                    39
#define HAL_NIXIETUBE_ALPHABET_o                                    40
#define HAL_NIXIETUBE_ALPHABET_p                                    41
#define HAL_NIXIETUBE_ALPHABET_q                                    42
#define HAL_NIXIETUBE_ALPHABET_r                                    43
#define HAL_NIXIETUBE_ALPHABET_s                                    44
#define HAL_NIXIETUBE_ALPHABET_t                                    45
#define HAL_NIXIETUBE_ALPHABET_u                                    46
#define HAL_NIXIETUBE_ALPHABET_v                                    47
#define HAL_NIXIETUBE_ALPHABET_w                                    48
#define HAL_NIXIETUBE_ALPHABET_x                                    49
#define HAL_NIXIETUBE_ALPHABET_y                                    50
#define HAL_NIXIETUBE_ALPHABET_z                                    51

/*************************************************-*************************************************
*                                        特殊符号选项定义
**************************************************-*****************^******************************/
#define HAL_NIXIETUBE_SIGN_1                                        0
#define HAL_NIXIETUBE_SIGN_2                                        1
#define HAL_NIXIETUBE_SIGN_3                                        2
#define HAL_NIXIETUBE_SIGN_4                                        3

/*************************************************-*************************************************
*                                          CONSTANT DEFINED
**************************************************-*****************^******************************/
#define HAL_NIXIETUBE_DISPLAY_DATA_TYPE_NUM                         1
#define HAL_NIXIETUBE_DISPLAY_DATA_TYPE_ALPHABET                    2
#define HAL_NIXIETUBE_DISPLAY_DATA_TYPE_SIGN                        3

/*************************************************-*************************************************
*                                          CONSTANT DEFINED
**************************************************-*****************^******************************/
// 定义闪烁类型枚举
typedef enum 
{
    HAL_NIXIETUBE_DISPLAY_FLASH_TYPE_NONE,
    HAL_NIXIETUBE_DISPLAY_FLASH_TYPE_LEFT_AREA,
    HAL_NIXIETUBE_DISPLAY_FLASH_TYPE_RIGHT_AREA,
    HAL_NIXIETUBE_DISPLAY_FLASH_TYPE_SINGLE,
}hal_nixietube_flash_type_enum_t;

typedef struct
{
    uint8_t data[8];
    uint8_t type[8];        //-- 0=数字, 1=字母，2=符号
    uint8_t pos;            //-- 当前扫描的位数（0-7）
}hal_nixietube_data_t;

/*************************************************-*************************************************
*                                              MACRO
**************************************************-*****************^******************************/
#define HAL_NIXIETUBE_DISPLAY_SET_BLUE_LED(x)	                    {hal_nixietube_blue_led_enable = x;}
#define HAL_NIXIETUBE_DISPLAY_SET_RED_LED(x)	                    {hal_nixietube_red_led_enable = x;}
#define HAL_NIXIETUBE_DISPLAY_SET_GREEN_LED(x)	                    {hal_nixietube_green_led_enable = x;}

/*************************************************-*************************************************
*                                          GLOBAL VARIABLES
**************************************************-*****************^******************************/
extern uint8 hal_nixietube_blue_led_enable;
extern uint8 hal_nixietube_red_led_enable;
extern uint8 hal_nixietube_green_led_enable;


/*************************************************-*************************************************
*                                             FUNCTION
**************************************************-*****************^******************************/
extern void hal_nixietube_init(void);                   // 初始化数码管
extern void hal_nixietube_string_by_area(uint8 area, uint8 column, char *string);
extern void hal_nixietube_mode_set(uint8_t mode);
extern void hal_nixietube_set_single_flash_enable(uint8_t pos);
extern void hal_nixietube_set_right_area_flash_enable(uint8_t enable) ;
extern void hal_nixietube_set_left_area_flash_enable(uint8_t enable) ;
extern void hal_nixietube_set_flash_enable(uint8_t enable);
extern void hal_nixietube_set_global_enable(uint8_t enable);
extern void hal_nixietube_scan(uint8_t rev_flag);    // 动态扫描（建议调用频率 >100Hz）
extern void hal_nixietube_set_digit(uint8_t pos, uint8_t num) ;
extern void hal_nixietube_set_alphabet(uint8_t pos, uint8_t alphabet);
extern void hal_nixietube_digit(uint32 num);
extern void hal_nixietube_digit_left(uint16 num);
extern void hal_nixietube_digit_right(uint16 num);
extern void hal_nixietube_string(char *string);
extern void hal_nixietube_alphabet_left(char *string);
extern void hal_nixietube_alphabet_right(char *string);


//extern void display_clearalldig(void);
//extern void display_clearallseg(void);
//extern void display_nixie(uint8_t num, uint8_t digit);          // 某位显示数字
//extern void display_alphabet(uint8_t alphabet, uint8_t digit);

/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#endif

