#include "hal_segment.h"

#ifdef _HAL_SEGMENT_H

//延时
void hal_disp_delay_us(uint32_t us)
{
    uint32_t i;
    for (i = 0; i < us * 10; i++) {  // 10是经验值，需根据实际主频调整
        __NOP();                     // 空操作，防止被优化（可选）
    }
}

//清屏
void hal_disp_clear_all(void)
{
    uint16_t i;
    for (i = 0; i < MAX_ADDR; i++) {
        hal_disp_w_data(DATA_ID, i, 0x00);
    }
}

//填充
void hal_disp_fill(uint8_t data)
{
    uint16_t i;
    for (i = 0; i < MAX_ADDR; i++) {
        hal_disp_w_data(DATA_ID, i, data);
    }
}

//启动配置
void hal_disp_config(void)
{
    // 设置LCD偏置4com
    hal_disp_w_cmd(CMD_ID, BIAS_1_3_4_COMS);
    // 打开LCD显示
    hal_disp_w_cmd(CMD_ID, LCD_ON);
    // 打开系统时钟，使用内部RC
    hal_disp_w_cmd(CMD_ID, SYS_EN);
    // 清屏
    hal_disp_clear_all();

}

//初始化断码屏引脚
void hal_disp_init(void)
{
    GPIO_Config_T gpio_config;
    RCM_EnableAHBPeriphClock(DISP_PERIPH);

    gpio_config.pin = CS_PIN | WR_PIN | DATA_PIN;
    gpio_config.mode = GPIO_MODE_OUT;
    gpio_config.outtype = GPIO_OUT_TYPE_PP;
    gpio_config.speed = GPIO_SPEED_50MHz;
    gpio_config.pupd = GPIO_PUPD_NO;
    GPIO_Config(ALL_PORT, &gpio_config);

    hal_disp_config();
//    div_disp_out1andpoint(S1);
//    div_disp_alphabet(1,ALPHABET_B);
}


//连续写数据
void hal_disp_wr_series_data(uint8_t id, uint16_t start_address, uint8_t *data, uint16_t length)
{
    DISP_W_CS(0);
    hal_disp_delay_us(1);
    // 写ID
    for (int i = 0; i < ID_LENGTH; i++) {
        DISP_W_WR(0);
		DISP_W_DATA(id & (0x04 >> i));
        hal_disp_delay_us(1);
        DISP_W_WR(1);
        hal_disp_delay_us(1);
    }
    // 写地址
    for (int i = 0; i < ADDRESS_LENGTH; i++) {
        DISP_W_WR(0);
        DISP_W_DATA(start_address & (0x20 >> i));
        hal_disp_delay_us(1);
        DISP_W_WR(1);
        hal_disp_delay_us(1);
    }
    // 写数据
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < DATA_LENGTH; j++) {
            DISP_W_DATA(data[i] & (0x01 << j));
            hal_disp_delay_us(WR_DELAY_US);
            DISP_W_WR(0);
            hal_disp_delay_us(WR_DELAY_US);
            DISP_W_WR(1);
            hal_disp_delay_us(1);
        }
    }
    DISP_W_CS(1);
    hal_disp_delay_us(1);
}

//写命令
void hal_disp_w_cmd(uint8_t id, uint16_t cmd)
{
    DISP_W_CS(0);
    hal_disp_delay_us(1);
    // 写ID
    for (int i = 0; i < ID_LENGTH; i++) {
        DISP_W_WR(0);
        DISP_W_DATA(id & (0x04 >> i));
        hal_disp_delay_us(1);
        DISP_W_WR(1);
        hal_disp_delay_us(1);
    }
    // 写命令代码
    for (int i = 0; i < CMD_LENGTH; i++) {
        DISP_W_WR(0);
        DISP_W_DATA(cmd & (0x80 >> i));
        hal_disp_delay_us(1);
        DISP_W_WR(1);
        hal_disp_delay_us(1);
    }
    DISP_W_CS(1);
    hal_disp_delay_us(1);
}


//写数据
void hal_disp_w_data(uint8_t id, uint16_t address, uint16_t data)
{
    DISP_W_CS(0);
    hal_disp_delay_us(1);
    // 写ID
    for (int i = 0; i < ID_LENGTH; i++) {
        DISP_W_DATA(id & (0x04 >> i));
        hal_disp_delay_us(WR_DELAY_US);
        DISP_W_WR(0);
        hal_disp_delay_us(WR_DELAY_US);
        DISP_W_WR(1);
        hal_disp_delay_us(1);
    }
    // 写地址
    for (int i = 0; i < ADDRESS_LENGTH; i++) {
        DISP_W_DATA(address & (0x20 >> i));
        hal_disp_delay_us(WR_DELAY_US);
        DISP_W_WR(0);
        hal_disp_delay_us(WR_DELAY_US);
        DISP_W_WR(1);
        hal_disp_delay_us(1);
    }
    // 写数据
    for (int i = 0; i < DATA_LENGTH; i++) {
        DISP_W_DATA(data & (0x01 << i));
        hal_disp_delay_us(WR_DELAY_US);
        DISP_W_WR(0);
        hal_disp_delay_us(WR_DELAY_US);
        DISP_W_WR(1);
        hal_disp_delay_us(1);
    }
    DISP_W_CS(1);
    hal_disp_delay_us(1);
}

// 引脚写操作
void hal_disp_write_pin(uint8_t cmd, uint8_t level)
{
    switch (cmd) {
        case W_CS: {
            level ? CS_1 : CS_0;
        } break;
        case W_WR: {
            level ? WR_1 : WR_0;
        } break;
        case W_DATA: {
            level ? DATA_1 : DATA_0;
        } break;
    }
}

//位显示====================================================
//显示字符串
// 显示字符串
void div_disp_by_string(int main_screen, char *string)
{
    uint16_t sz = 0;
    uint16_t length = 4;
    for (int i = sz; i < length; i++) {
        int temp = i;
        // 副屏显示位置调整
        if (!main_screen) {
            if (i == 0) temp = 7;
            if (i == 1) temp = 6;
            if (i == 2) temp = 5;
            if (i == 3) temp = 4;
        }

        if ((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z')) {
            switch (string[i]) {
                // 完整字母映射（小写）
                case 'a': div_disp_alphabet(temp, ALPHABET_a); break;
                case 'b': div_disp_alphabet(temp, ALPHABET_b); break;
                case 'c': div_disp_alphabet(temp, ALPHABET_c); break;
                case 'd': div_disp_alphabet(temp, ALPHABET_d); break;
                case 'e': div_disp_alphabet(temp, ALPHABET_e); break;
                case 'f': div_disp_alphabet(temp, ALPHABET_f); break;
                case 'g': div_disp_alphabet(temp, ALPHABET_g); break;
                case 'h': div_disp_alphabet(temp, ALPHABET_h); break;
                case 'i': div_disp_alphabet(temp, ALPHABET_i); break;
                case 'j': div_disp_alphabet(temp, ALPHABET_j); break;
                case 'k': div_disp_alphabet(temp, ALPHABET_k); break;
                case 'l': div_disp_alphabet(temp, ALPHABET_l); break;
                case 'm': div_disp_alphabet(temp, ALPHABET_m); break;
                case 'n': div_disp_alphabet(temp, ALPHABET_n); break;
                case 'o': div_disp_alphabet(temp, ALPHABET_o); break;
                case 'p': div_disp_alphabet(temp, ALPHABET_p); break;
                case 'q': div_disp_alphabet(temp, ALPHABET_q); break;
                case 'r': div_disp_alphabet(temp, ALPHABET_r); break;
                case 's': div_disp_alphabet(temp, ALPHABET_s); break;
                case 't': div_disp_alphabet(temp, ALPHABET_t); break;
                case 'u': div_disp_alphabet(temp, ALPHABET_u); break;
                case 'v': div_disp_alphabet(temp, ALPHABET_v); break;
                case 'w': div_disp_alphabet(temp, ALPHABET_w); break;
                case 'x': div_disp_alphabet(temp, ALPHABET_x); break;
                case 'y': div_disp_alphabet(temp, ALPHABET_y); break;
                case 'z': div_disp_alphabet(temp, ALPHABET_z); break;
                
                // 完整字母映射（大写）
                case 'A': div_disp_alphabet(temp, ALPHABET_A); break;
                case 'B': div_disp_alphabet(temp, ALPHABET_B); break;
                case 'C': div_disp_alphabet(temp, ALPHABET_C); break;
                case 'D': div_disp_alphabet(temp, ALPHABET_D); break;
                case 'E': div_disp_alphabet(temp, ALPHABET_E); break;
                case 'F': div_disp_alphabet(temp, ALPHABET_F); break;
                case 'G': div_disp_alphabet(temp, ALPHABET_G); break;
                case 'H': div_disp_alphabet(temp, ALPHABET_H); break;
                case 'I': div_disp_alphabet(temp, ALPHABET_I); break;
                case 'J': div_disp_alphabet(temp, ALPHABET_J); break;
                case 'K': div_disp_alphabet(temp, ALPHABET_K); break;
                case 'L': div_disp_alphabet(temp, ALPHABET_L); break;
                case 'M': div_disp_alphabet(temp, ALPHABET_M); break;
                case 'N': div_disp_alphabet(temp, ALPHABET_N); break;
                case 'O': div_disp_alphabet(temp, ALPHABET_O); break;
                case 'P': div_disp_alphabet(temp, ALPHABET_P); break;
                case 'Q': div_disp_alphabet(temp, ALPHABET_Q); break;
                case 'R': div_disp_alphabet(temp, ALPHABET_R); break;
                case 'S': div_disp_alphabet(temp, ALPHABET_S); break;
                case 'T': div_disp_alphabet(temp, ALPHABET_T); break;
                case 'U': div_disp_alphabet(temp, ALPHABET_U); break;
                case 'V': div_disp_alphabet(temp, ALPHABET_V); break;
                case 'W': div_disp_alphabet(temp, ALPHABET_W); break;
                case 'X': div_disp_alphabet(temp, ALPHABET_X); break;
                case 'Y': div_disp_alphabet(temp, ALPHABET_Y); break;
                case 'Z': div_disp_alphabet(temp, ALPHABET_Z); break;
            }
        } else if (string[i] >= '0' && string[i] <= '9') {
            // 完整数字映射
            switch (string[i]) {
                case '0': div_disp_num(temp, NUM_0); break;
                case '1': div_disp_num(temp, NUM_1); break;
                case '2': div_disp_num(temp, NUM_2); break;
                case '3': div_disp_num(temp, NUM_3); break;
                case '4': div_disp_num(temp, NUM_4); break;
                case '5': div_disp_num(temp, NUM_5); break;
                case '6': div_disp_num(temp, NUM_6); break;
                case '7': div_disp_num(temp, NUM_7); break;
                case '8': div_disp_num(temp, NUM_8); break;
                case '9': div_disp_num(temp, NUM_9); break;
            }
        } else if (string[i] == '-' || string[i] == '|' || string[i] == '/' || string[i] == '*' || string[i] == '^') {
            // 完整符号映射
            switch (string[i]) {
                case '-': div_disp_symbol(temp, SYMBOL_MINUS); break;
                case '|': div_disp_symbol(temp, SYMBOL_SAW); break;
                case '/': div_disp_symbol(temp, SYMBOL_SLASH); break;
                case '*': div_disp_symbol(temp, SYMBOL_X); break;
                case '^': div_disp_symbol(temp, SYMBOL_NEG_ONE); break;
            }
        } else {
            div_disp_num(temp, NUM_NULL);
        }
    }
}

// 显示符号
void div_disp_symbol(uint8_t nixie, uint8_t symbol)
{
    // 符号数据映射（保留原始数据）
    uint8_t data1, data2, data3;
    switch (symbol) {
        case SYMBOL_SLASH:    { data1 = 0x4; data2 = 0x0; data3 = 0x2; } break;  // /
        case SYMBOL_MINUS:    { data1 = 0x0; data2 = 0x6; data3 = 0x0; } break;  // -
        case SYMBOL_SAW:      { data1 = 0x3; data2 = 0x0; data3 = 0x0; } break;  // |
        case SYMBOL_POINT:    { data1 = 0x0; data2 = 0x0; data3 = 0x0; } break;  // .（点位控制）
        case SYMBOL_NEG_ONE:  { data1 = 0x0; data2 = 0x6; data3 = 0xc; } break;  // -1
        case SYMBOL_X:        { data1 = 0x4; data2 = 0x9; data3 = 0x2; } break;  // *
    }

    // 数码管位置映射
    uint8_t address1, address2, address3;
    switch (nixie) {
        case NIXIE_1: { address1 = 0x14; address2 = 0x13; address3 = 0x12; } break;
        case NIXIE_2: { address1 = 0x11; address2 = 0x10; address3 = 0x0f; } break;
        case NIXIE_3: { address1 = 0x0e; address2 = 0x0d; address3 = 0x0c; } break;
        case NIXIE_4: { address1 = 0x0b; address2 = 0x0a; address3 = 0x09; } break;
        case NIXIE_5: { address1 = 0x06; address2 = 0x07; address3 = 0x08; } break;
        case NIXIE_6: { address1 = 0x03; address2 = 0x04; address3 = 0x05; } break;
        case NIXIE_7: { address1 = 0x00; address2 = 0x01; address3 = 0x02; } break;
        case NIXIE_8: { address1 = 0x17; address2 = 0x18; address3 = 0x19; } break;
    }

    // 写入显示数据
    hal_disp_w_data(DATA_ID, address1, data1);
    hal_disp_w_data(DATA_ID, address2, data2);
    hal_disp_w_data(DATA_ID, address3, data3);
    
    // 特殊处理点符号（单独控制点位）
    if (symbol == SYMBOL_POINT) {
        if (nixie < NIXIE_4) {
            hal_disp_w_data(DATA_ID, 0x15, 0x08);  // 前4位点位控制地址
        } else {
            hal_disp_w_data(DATA_ID, 0x16, 0x08);  // 后4位点位控制地址
        }
    }
}





// 显示字母
void div_disp_alphabet(uint8_t nixie, uint8_t alphabet)
{
    // 字母数据映射（保留原始数据）
    uint8_t data1, data2, data3;
    switch (alphabet) {
        case ALPHABET_A: { data1 = 0x3; data2 = 0x6; data3 = 0xd; } break;  // A
        case ALPHABET_B: { data1 = 0xb; data2 = 0x6; data3 = 0xb; } break;  // B
        case ALPHABET_C: { data1 = 0xb; data2 = 0x0; data3 = 0x1; } break;  // C
        case ALPHABET_D: { data1 = 0xc; data2 = 0x3; data3 = 0xd; } break;  // D
        case ALPHABET_E: { data1 = 0xb; data2 = 0x6; data3 = 0x1; } break;  // E
        case ALPHABET_F: { data1 = 0x3; data2 = 0x6; data3 = 0x1; } break;  // F
        case ALPHABET_G: { data1 = 0xb; data2 = 0x4; data3 = 0x9; } break;  // G
        case ALPHABET_H: { data1 = 0x3; data2 = 0x6; data3 = 0xc; } break;  // H
        case ALPHABET_I: { data1 = 0x3; data2 = 0x0; data3 = 0x0; } break;  // I
        case ALPHABET_J: { data1 = 0xa; data2 = 0x0; data3 = 0xc; } break;  // J
        case ALPHABET_K: { data1 = 0x3; data2 = 0xa; data3 = 0x2; } break;  // K
        case ALPHABET_L: { data1 = 0xb; data2 = 0x0; data3 = 0x0; } break;  // L
        case ALPHABET_M: { data1 = 0x3; data2 = 0x1; data3 = 0xe; } break;  // M
        case ALPHABET_N: { data1 = 0x3; data2 = 0x9; data3 = 0xc; } break;  // N
        case ALPHABET_O: { data1 = 0xb; data2 = 0x0; data3 = 0xd; } break;  // O
        case ALPHABET_P: { data1 = 0x3; data2 = 0x6; data3 = 0x5; } break;  // P
        case ALPHABET_Q: { data1 = 0xb; data2 = 0x0; data3 = 0xd; } break;  // Q
        case ALPHABET_R: { data1 = 0x3; data2 = 0xe; data3 = 0x5; } break;  // R
        case ALPHABET_S: { data1 = 0x9; data2 = 0x6; data3 = 0x9; } break;  // S
        case ALPHABET_T: { data1 = 0xb; data2 = 0x6; data3 = 0x0; } break;  // T
        case ALPHABET_U: { data1 = 0xb; data2 = 0x0; data3 = 0xc; } break;  // U
        case ALPHABET_V: { data1 = 0xb; data2 = 0x0; data3 = 0xc; } break;  // V
        case ALPHABET_W: { data1 = 0x7; data2 = 0x8; data3 = 0xc; } break;  // W
        case ALPHABET_X: { data1 = 0x4; data2 = 0x9; data3 = 0x2; } break;  // X
        case ALPHABET_Y: { data1 = 0x9; data2 = 0x6; data3 = 0xc; } break;  // Y
        case ALPHABET_Z: { data1 = 0xc; data2 = 0x0; data3 = 0x3; } break;  // Z
        case ALPHABET_a: { data1 = 0x3; data2 = 0x6; data3 = 0xd; } break;  // a
        case ALPHABET_b: { data1 = 0xb; data2 = 0x6; data3 = 0x8; } break;  // b
        case ALPHABET_c: { data1 = 0xa; data2 = 0x6; data3 = 0x0; } break;  // c
        case ALPHABET_d: { data1 = 0xa; data2 = 0x6; data3 = 0xc; } break;  // d
        case ALPHABET_e: { data1 = 0xb; data2 = 0x6; data3 = 0x1; } break;  // e
        case ALPHABET_f: { data1 = 0x3; data2 = 0x6; data3 = 0x1; } break;  // f
        case ALPHABET_g: { data1 = 0x9; data2 = 0x6; data3 = 0xd; } break;  // g
        case ALPHABET_h: { data1 = 0x3; data2 = 0x6; data3 = 0x8; } break;  // h
        case ALPHABET_i: { data1 = 0x0; data2 = 0x0; data3 = 0x8; } break;  // i
        case ALPHABET_j: { data1 = 0xa; data2 = 0x0; data3 = 0xc; } break;  // j
        case ALPHABET_k: { data1 = 0x3; data2 = 0x6; data3 = 0x0; } break;  // k
        case ALPHABET_l: { data1 = 0xb; data2 = 0x0; data3 = 0x0; } break;  // l
        case ALPHABET_m: { data1 = 0x3; data2 = 0x1; data3 = 0xe; } break;  // m
        case ALPHABET_n: { data1 = 0x2; data2 = 0x6; data3 = 0x8; } break;  // n
        case ALPHABET_o: { data1 = 0xa; data2 = 0x6; data3 = 0x8; } break;  // o
        case ALPHABET_p: { data1 = 0x3; data2 = 0x6; data3 = 0x5; } break;  // p
        case ALPHABET_q: { data1 = 0x1; data2 = 0x6; data3 = 0xd; } break;  // q
        case ALPHABET_r: { data1 = 0x2; data2 = 0x6; data3 = 0x0; } break;  // r
        case ALPHABET_s: { data1 = 0x9; data2 = 0x6; data3 = 0x9; } break;  // s
        case ALPHABET_t: { data1 = 0xb; data2 = 0x6; data3 = 0x0; } break;  // t
        case ALPHABET_u: { data1 = 0xa; data2 = 0x0; data3 = 0x8; } break;  // u
        case ALPHABET_v: { data1 = 0xb; data2 = 0x0; data3 = 0xc; } break;  // v
        case ALPHABET_w: { data1 = 0x7; data2 = 0x8; data3 = 0xc; } break;  // w
        case ALPHABET_x: { data1 = 0x4; data2 = 0x9; data3 = 0x2; } break;  // x
        case ALPHABET_y: { data1 = 0x9; data2 = 0x6; data3 = 0xc; } break;  // y
        case ALPHABET_z: { data1 = 0xc; data2 = 0x0; data3 = 0x3; } break;  // z
        case ALPHABET_NULL: { data1 = 0x0; data2 = 0x0; data3 = 0x0; } break;  // NULL
    }

    // 数码管位置映射
    uint8_t address1, address2, address3;
    switch (nixie) {
        case NIXIE_1: { address1 = 0x14; address2 = 0x13; address3 = 0x12; } break;
        case NIXIE_2: { address1 = 0x11; address2 = 0x10; address3 = 0x0f; } break;
        case NIXIE_3: { address1 = 0x0e; address2 = 0x0d; address3 = 0x0c; } break;
        case NIXIE_4: { address1 = 0x0b; address2 = 0x0a; address3 = 0x09; } break;
        case NIXIE_5: { address1 = 0x06; address2 = 0x07; address3 = 0x08; } break;
        case NIXIE_6: { address1 = 0x03; address2 = 0x04; address3 = 0x05; } break;
        case NIXIE_7: { address1 = 0x00; address2 = 0x01; address3 = 0x02; } break;
        case NIXIE_8: { address1 = 0x17; address2 = 0x18; address3 = 0x19; } break;
    }

    // 写入显示数据
    hal_disp_w_data(DATA_ID, address1, data1);
    hal_disp_w_data(DATA_ID, address2, data2);
    hal_disp_w_data(DATA_ID, address3, data3);
}

// 显示数字
void div_disp_num(uint8_t nixie, uint8_t num)
{
    // 数字数据映射（保留原始数据）
    uint8_t data1, data2, data3;
    switch (num) {
        case NUM_NULL: { data1 = 0x0; data2 = 0x0; data3 = 0x0; } break;  // 空数字
        case NUM_0: { data1 = 0xb; data2 = 0x0; data3 = 0xd; } break;  // 0
        case NUM_1: { data1 = 0x0; data2 = 0x0; data3 = 0xc; } break;  // 1
        case NUM_2: { data1 = 0xa; data2 = 0x6; data3 = 0x5; } break;  // 2
        case NUM_3: { data1 = 0x8; data2 = 0x6; data3 = 0xd; } break;  // 3
        case NUM_4: { data1 = 0x1; data2 = 0x6; data3 = 0xc; } break;  // 4
        case NUM_5: { data1 = 0x9; data2 = 0x6; data3 = 0x9; } break;  // 5
        case NUM_6: { data1 = 0xb; data2 = 0x6; data3 = 0x9; } break;  // 6
        case NUM_7: { data1 = 0x0; data2 = 0x0; data3 = 0xd; } break;  // 7
        case NUM_8: { data1 = 0xb; data2 = 0x6; data3 = 0xd; } break;  // 8
        case NUM_9: { data1 = 0x9; data2 = 0x6; data3 = 0xd; } break;  // 9
    }

    // 数码管位置映射
    uint8_t address1, address2, address3;
    switch (nixie) {
        case NIXIE_1: { address1 = 0x14; address2 = 0x13; address3 = 0x12; } break;
        case NIXIE_2: { address1 = 0x11; address2 = 0x10; address3 = 0x0f; } break;
        case NIXIE_3: { address1 = 0x0e; address2 = 0x0d; address3 = 0x0c; } break;
        case NIXIE_4: { address1 = 0x0b; address2 = 0x0a; address3 = 0x09; } break;
        case NIXIE_5: { address1 = 0x06; address2 = 0x07; address3 = 0x08; } break;
        case NIXIE_6: { address1 = 0x03; address2 = 0x04; address3 = 0x05; } break;
        case NIXIE_7: { address1 = 0x00; address2 = 0x01; address3 = 0x02; } break;
        case NIXIE_8: { address1 = 0x17; address2 = 0x18; address3 = 0x19; } break;
    }

    // 写入显示数据
    hal_disp_w_data(DATA_ID, address1, data1);
    hal_disp_w_data(DATA_ID, address2, data2);
    hal_disp_w_data(DATA_ID, address3, data3);
}

void div_disp_clearpoint(void)
{
    hal_disp_w_data(DATA_ID,0x15,0x0);
    hal_disp_w_data(DATA_ID,0x16,0x0);
}
          
void div_disp_out1andpoint(uint8_t data, uint8_t enable)
{
    static uint8_t state = 0;
    switch (data)
    {
        case P1:
        {
            if (enable)
            {
                state |= P1;
            }
            else
            {
                state &= ~P1;
            }
        }break;
        case P2:
        {
            if (enable)
            {
                state |= P2;
            }
            else
            {
                state &= ~P2;
            }
        }break;
        case P3:
        {
            if (enable)
            {
                state |= P3;
            }
            else
            {
                state &= ~P3;
            }
        }break;
        case S1:
        {
            if (enable)
            {
                state |= S1;
            }
            else
            {
                state &= ~S1;
            }
        }break;
        default:break;
    }
    hal_disp_w_data(DATA_ID,0x15,state);
}

void div_disp_out2andpoint(uint8_t data, uint8_t enable)
{
    static uint8_t state = 0;
    switch (data)
    {
        case P6:
        {
            if (enable)
            {
                state |= P6;
            }
            else
            {
                state &= ~P6;
            }
        }break;
        case P7:
        {
            if (enable)
            {
                state |= P7;
            }
            else
            {
                state &= ~P7;
            }
        }break;
        case P8:
        {
            if (enable)
            {
                state |= P8;
            }
            else
            {
                state &= ~P8;
            }
        }break;
        case S2:
        {
            if (enable)
            {
                state |= S2;
            }
            else
            {
                state &= ~S2;
            }
        }break;
        default:break;
    }
    hal_disp_w_data(DATA_ID,0x16,state);
}


#endif
