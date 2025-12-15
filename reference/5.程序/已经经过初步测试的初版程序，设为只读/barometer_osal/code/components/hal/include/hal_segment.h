#ifndef _HAL_SEGMENT_H
#define _HAL_SEGMENT_H

#include "main.h"

// 宏定义区域
#define CS_PIN         GPIO_PIN_7
#define CS_PORT        GPIOB
#define WR_PIN         GPIO_PIN_6
#define WR_PORT        GPIOB
#define DATA_PIN       GPIO_PIN_5
#define DATA_PORT      GPIOB
#define ALL_PORT       GPIOB
#define DISP_PERIPH    RCM_AHB_PERIPH_GPIOB

#define ID_LENGTH      3
#define CMD_LENGTH     9
#define ADDRESS_LENGTH 6
#define DATA_LENGTH    4

#define CMD_ID         0x4
#define DATA_ID        0x5

#define SYS_DIS        0x00    // 上电默认关闭
#define SYS_EN         0x01
#define LCD_OFF        0x02
#define LCD_ON         0x03
#define TIMERS_DIS     0x04
#define TIMER_EN       0x06
#define RC_256K        0x18   // 内部RC

#define BIAS_1_2_2_COMS  0x20
#define BIAS_1_2_3_COMS  0x24
#define BIAS_1_2_4_COMS  0x28
#define BIAS_1_3_2_COMS  0x21
#define BIAS_1_3_3_COMS  0x25
#define BIAS_1_3_4_COMS  0x29

#define TEST           0xE0
#define NORMAL         0xE3
#define MAX_ADDR       0x31    // RAM最大地址
#define WR_DELAY_US    5

// 引脚操作宏
#define CS_0           GPIO_ClearBit(CS_PORT, CS_PIN)
#define CS_1           GPIO_SetBit(CS_PORT, CS_PIN)
#define WR_0           GPIO_ClearBit(WR_PORT, WR_PIN)
#define WR_1           GPIO_SetBit(WR_PORT, WR_PIN)
#define DATA_0         GPIO_ClearBit(DATA_PORT, DATA_PIN)
#define DATA_1         GPIO_SetBit(DATA_PORT, DATA_PIN)

// 引脚操作封装
#define DISP_W_CS(x)   hal_disp_write_pin(W_CS, x)
#define DISP_W_WR(x)   hal_disp_write_pin(W_WR, x)
#define DISP_W_DATA(x) hal_disp_write_pin(W_DATA, x)

// 命令类型
#define W_CS           0
#define W_WR           1
#define W_DATA         2

// 屏幕选择
#define MAIN_SCREEN    1
#define SECOND_SCREEN  0
#define OUT1_SCREEN    2
#define OUT2_SCREEN    3

// 数字定义
#define NUM_0          0
#define NUM_1          1
#define NUM_2          2
#define NUM_3          3
#define NUM_4          4
#define NUM_5          5
#define NUM_6          6
#define NUM_7          7
#define NUM_8          8
#define NUM_9          9
#define NUM_NULL       10

// 符号定义
#define SYMBOL_SLASH     0     // /
#define SYMBOL_MINUS     1     // -
#define SYMBOL_SAW       2     // |
#define SYMBOL_POINT     3     // .
#define SYMBOL_NEG_ONE   4     // -1
#define SYMBOL_X         5     // *

// 数码管位定义
#define NIXIE_1         0
#define NIXIE_2         1
#define NIXIE_3         2
#define NIXIE_4         3
#define NIXIE_5         4
#define NIXIE_6         5
#define NIXIE_7         6
#define NIXIE_8         7

// 点位定义
#define S1              0x01
#define P3              0x02
#define P2              0x04
#define P1              0x08
#define S2              0x01
#define P6              0x02
#define P7              0x04
#define P8              0x08
#define NULL_POINT      0x00

// 函数声明区域
void hal_disp_write_pin(uint8_t cmd, uint8_t level);
void hal_disp_init(void);
void hal_disp_w_cmd(uint8_t id, uint16_t cmd);
void hal_disp_w_data(uint8_t id, uint16_t address, uint16_t data);
void hal_disp_wr_series_data(uint8_t id, uint16_t start_address, uint8_t *data, uint16_t length);
void hal_disp_fill(uint8_t data);
void hal_disp_clear_all(void);

// 显示控制函数
void div_disp_by_string(int main_screen, char *string);
void div_disp_alphabet(uint8_t nixie, uint8_t alphabet);
void div_disp_num(uint8_t nixie, uint8_t num);
void div_disp_symbol(uint8_t nixie, uint8_t symbol);
void div_disp_clearpoint(void);
void div_disp_out1andpoint(uint8_t data,uint8_t enable);
void div_disp_out2andpoint(uint8_t data,uint8_t enable);

// 字母枚举类型
typedef enum {
    ALPHABET_A, ALPHABET_B, ALPHABET_C, ALPHABET_D, ALPHABET_E,
    ALPHABET_F, ALPHABET_G, ALPHABET_H, ALPHABET_I, ALPHABET_J,
    ALPHABET_K, ALPHABET_L, ALPHABET_M, ALPHABET_N, ALPHABET_O,
    ALPHABET_P, ALPHABET_Q, ALPHABET_R, ALPHABET_S, ALPHABET_T,
    ALPHABET_U, ALPHABET_V, ALPHABET_W, ALPHABET_X, ALPHABET_Y,
    ALPHABET_Z, ALPHABET_a, ALPHABET_b, ALPHABET_c, ALPHABET_d,
    ALPHABET_e, ALPHABET_f, ALPHABET_g, ALPHABET_h, ALPHABET_i,
    ALPHABET_j, ALPHABET_k, ALPHABET_l, ALPHABET_m, ALPHABET_n,
    ALPHABET_o, ALPHABET_p, ALPHABET_q, ALPHABET_r, ALPHABET_s,
    ALPHABET_t, ALPHABET_u, ALPHABET_v, ALPHABET_w, ALPHABET_x,
    ALPHABET_y, ALPHABET_z, ALPHABET_NULL
} Alphabet;

#endif
