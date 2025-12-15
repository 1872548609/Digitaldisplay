#include "hal_drivers.h"

#ifdef HAL_NIXIETTUBE_H

//****************************************正向段码表****************************************//
// 数字段码表（共阴数码管，无小数点）
const uint8_t hal_nixietube_seg_codes[10] = 
{
    0x3F,  // "0"  
    0x06,  // "1"  
    0x5B,  // "2"  
    0x4F,  // "3"  
    0x66,  // "4"  
    0x6D,  // "5"  
    0x7D,  // "6"  
    0x07,  // "7"  
    0x7F,  // "8"  
    0x6F   // "9"  
};

// 字母段码表（共阴数码管，无小数点）
// 共阳极数码管字母段码表（A-Z）
// 顺序：A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
const unsigned char hal_nixietube_seg_table_letters_common_anode[26] = 
{
    0x77, // A 
    0x7C, // B 
    0x39, // C 
    0x5E, // D 
    0x79, // E 
    0x71, // F 
    0x3D, // G 
    0x76, // H 
    0x04, // I 
    0x1E, // J 
    0x7A, // K 
    0x38, // L 
    0x55, // M 
    0x54, // N 
    0x5C, // O 
    0x73, // P 
    0x67, // Q 
    0x50, // R 
    0x6D, // S 
    0x78, // T 
    0x3E, // U 
    0x3E, // V 
    0x3E, // W 
    0x76, // X 
    0x6E, // Y 
    0x5B  // Z 
};

// 顺序：a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z
const unsigned char hal_nixietube_seg_table_lowercase_common_anode[26] = 
{
	0x77, // a
	0x7C, // b
	0x58, // c
	0x5E, // d
	0x79, // e
	0x71, // f
	0x3D, // g
	0x74, // h
	0x04, // i
	0x1E, // j
	0x7A, // k
	0x38, // l
	0x55, // m
	0x54, // n
	0x5C, // o
	0x73, // p
	0x67, // q
	0x50, // r
	0x6D, // s
	0x78, // t
	0x1C, // u
	0x1C, // v
	0x1C, // w
	0x76, // x
	0x6E, // y
	0x5B  // z
};

//特殊字符
const unsigned char hal_nixietube_seg_special_sign[4] = 
{
	0x00, // 
	0x40, // -
	0x04, // ,
	0x06  // |
};

//****************************************反向段码表****************************************//
// 数字段码表（共阴数码管，无小数点）
const uint8_t hal_nixietube_seg_codes_reverse[10] = 
{
    0x3F,  // "0"  
    0x30,  // "1"  
    0x5B,  // "2"  
    0x79,  // "3"  
    0x74,  // "4"  
    0x6D,  // "5"  
    0x6F,  // "6"  
    0x38,  // "7"  
    0x7F,  // "8"  
    0x7D   // "9"  
};

// 字母段码表（共阴数码管，无小数点）
// 共阳极数码管字母段码表（A-Z）
// 顺序：A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
const unsigned char hal_nixietube_seg_table_letters_common_anode_reverse[26] = 
{
    0x7E, // A 
    0x67, // B 
    0x0F, // C 
    0x73, // D 
    0x4F, // E 
    0x4E, // F 
    0x2F, // G 
    0x76, // H 
    0x20, // I 
    0x33, // J 
    0x57, // K 
    0x07, // L 
    0x6A, // M 
    0x62, // N 
    0x63, // O 
    0x5E, // P 
    0x7C, // Q 
    0x42, // R 
    0x6D, // S 
    0x47, // T 
    0x37, // U 
    0x37, // V 
    0x37, // W 
    0x76, // X 
    0x75, // Y 
    0x5B  // Z 
};

// 顺序：a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z
const unsigned char hal_nixietube_seg_table_lowercase_common_anode_reverse[26] = 
{
	0x7E, // a
	0x67, // b
	0x43, // c
	0x73, // d
	0x4F, // e
	0x4E, // f
	0x2F, // g
	0x66, // h
	0x20, // i
	0x33, // j
	0x57, // k
	0x07, // l
	0x6A, // m
	0x62, // n
	0x63, // o
	0x5E, // p
	0x7C, // q
	0x42, // r
	0x6D, // s
	0x47, // t
	0x23, // u
	0x23, // v
	0x23, // w
	0x76, // x
	0x6E, // y
	0x75  // z
};

//特殊字符
const unsigned char hal_nixietube_seg_special_sign_reverse[4] = 
{
	0x00, // 
	0x40, // -
	0x20, // ,
	0x30  // |
};


// 定义显示缓冲区（8位，每位存储要显示的数字或字母索引）
hal_nixietube_data_t hal_nixietube_data;

// 闪烁控制变量
volatile uint8_t hal_nixietube_enable = true;          	// 全局显示使能
volatile uint8_t hal_nixietube_mode = HAL_NIXIETUBE_DISPLAY_FLASH_TYPE_NONE;   // 显示模式  0正常运行 1左闪烁 2右闪烁 3单个数码管闪烁
volatile uint8_t hal_nixietube_left_area_blink_enable = false;      	// 左四位闪烁使能
volatile uint8_t hal_nixietube_right_area_blink_enable = false;     	// 右四位闪烁使能
volatile uint8_t hal_nixietube_single_blink_position = 0xff;     	 	// 单个数码管闪烁位置（0xFF表示关闭）

uint8_t hal_nixietube_blue_led_enable = 0;
uint8_t hal_nixietube_red_led_enable = 0;
uint8_t hal_nixietube_green_led_enable = 0;


/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/

// 清除所有数码管位显示
//void display_clearalldig(void)
//{
//	GPIO_SetBit(HAL_NIXIETUBE_DIG_PORT1, HAL_NIXIETUBE_DIG_1_PIN | HAL_NIXIETUBE_DIG_2_PIN | HAL_NIXIETUBE_DIG_3_PIN | HAL_NIXIETUBE_DIG_4_PIN | HAL_NIXIETUBE_DIG_5_PIN | HAL_NIXIETUBE_DIG_6_PIN);
//    GPIO_SetBit(HAL_NIXIETUBE_DIG_PORT2, HAL_NIXIETUBE_DIG_7_PIN | HAL_NIXIETUBE_DIG_8_PIN);
//}

// 清除所有数码管段显示
//void display_clearallseg(void)
//{
//	GPIO_SetBit(HAL_NIXIETUBE_SEGMENT_PORT,HAL_NIXIETUBE_SEGMENT_A_PIN|HAL_NIXIETUBE_SEGMENT_B_PIN|HAL_NIXIETUBE_SEGMENT_C_PIN|HAL_NIXIETUBE_SEGMENT_D_PIN|HAL_NIXIETUBE_SEGMENT_E_PIN|HAL_NIXIETUBE_SEGMENT_F_PIN|HAL_NIXIETUBE_SEGMENT_G_PIN);
//}


// 显示函数：在指定位置显示数字 
//void display_nixie(uint8_t num, uint8_t digit) 
//{
//    // 1. 关闭所有位选（防止鬼影）
//	HAL_NIXIETUBE_DIG_ALL_PIN_OFF();
//	HAL_NIXIETUBE_SEGMENT_ALL_PIN_OFF();
// 
//    // 2. 设置段选
//    uint16_t seg_value = hal_nixietube_seg_codes[num % 10]; 
//	
//	uint16_t pin = 0;
//	
//	(seg_value & 0x01) ? pin|=HAL_NIXIETUBE_SEGMENT_A_PIN : 0;                          
//	(seg_value & 0x02) ? pin|=HAL_NIXIETUBE_SEGMENT_B_PIN : 0;                         
//	(seg_value & 0x04) ? pin|=HAL_NIXIETUBE_SEGMENT_C_PIN : 0;                          
//	(seg_value & 0x08) ? pin|=HAL_NIXIETUBE_SEGMENT_D_PIN : 0;                          
//	(seg_value & 0x10) ? pin|=HAL_NIXIETUBE_SEGMENT_E_PIN : 0;                         
//	(seg_value & 0x20) ? pin|=HAL_NIXIETUBE_SEGMENT_F_PIN : 0;                           
//	(seg_value & 0x40) ? pin|=HAL_NIXIETUBE_SEGMENT_G_PIN : 0;                           
//	
//    GPIO_ClearBit(HAL_NIXIETUBE_SEGMENT_PORT,pin);    

//		
//    // 3. 打开目标位选
//    switch (digit) {
//        case 0: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT1, HAL_NIXIETUBE_DIG_1_PIN); break;
//        case 1: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT1, HAL_NIXIETUBE_DIG_2_PIN); break;
//        case 2: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT1, HAL_NIXIETUBE_DIG_3_PIN); break;
//        case 3: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT1, HAL_NIXIETUBE_DIG_4_PIN); break;
//        case 4: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT1, HAL_NIXIETUBE_DIG_5_PIN); break;
//        case 5: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT1, HAL_NIXIETUBE_DIG_6_PIN); break;
//        case 6: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT2, HAL_NIXIETUBE_DIG_7_PIN); break;
//        case 7: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT2, HAL_NIXIETUBE_DIG_8_PIN); break;
//        default: break;
//    }
//}

// 显示函数：在指定位置显示字母
//void display_alphabet(uint8_t alphabet, uint8_t digit) 
//{
//    // 1. 关闭所有位选（防止鬼影）
//	HAL_NIXIETUBE_DIG_ALL_PIN_OFF();
//	HAL_NIXIETUBE_SEGMENT_ALL_PIN_OFF();
// 
//    // 2. 设置段选
//    uint16_t seg_value = hal_nixietube_seg_table_letters_common_anode[alphabet % 26]; 
//	
//	uint16_t pin = 0;
//	
//	(seg_value & 0x01) ? pin|=HAL_NIXIETUBE_SEGMENT_A_PIN : 0;                          
//	(seg_value & 0x02) ? pin|=HAL_NIXIETUBE_SEGMENT_B_PIN : 0;                         
//	(seg_value & 0x04) ? pin|=HAL_NIXIETUBE_SEGMENT_C_PIN : 0;                          
//	(seg_value & 0x08) ? pin|=HAL_NIXIETUBE_SEGMENT_D_PIN : 0;                          
//	(seg_value & 0x10) ? pin|=HAL_NIXIETUBE_SEGMENT_E_PIN : 0;                         
//	(seg_value & 0x20) ? pin|=HAL_NIXIETUBE_SEGMENT_F_PIN : 0;                           
//	(seg_value & 0x40) ? pin|=HAL_NIXIETUBE_SEGMENT_G_PIN : 0;                           
//	
//    GPIO_ClearBit(HAL_NIXIETUBE_SEGMENT_PORT,pin);    

//		
//    // 3. 打开目标位选
//    switch (digit) {
//        case 0: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT1, HAL_NIXIETUBE_DIG_1_PIN); break;
//        case 1: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT1, HAL_NIXIETUBE_DIG_2_PIN); break;
//        case 2: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT1, HAL_NIXIETUBE_DIG_3_PIN); break;
//        case 3: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT1, HAL_NIXIETUBE_DIG_4_PIN); break;
//        case 4: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT1, HAL_NIXIETUBE_DIG_5_PIN); break;
//        case 5: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT1, HAL_NIXIETUBE_DIG_6_PIN); break;
//        case 6: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT2, HAL_NIXIETUBE_DIG_7_PIN); break;
//        case 7: GPIO_ClearBit(HAL_NIXIETUBE_DIG_PORT2, HAL_NIXIETUBE_DIG_8_PIN); break;
//        default: break;
//    }
//}


// 显示模式设置
void hal_nixietube_mode_set(uint8_t mode)
{
	hal_nixietube_mode = mode;
}

// 全局显示开关
void hal_nixietube_set_global_enable(uint8_t enable) 
{
    hal_nixietube_enable = enable;
    if (!enable) 
    {
        HAL_NIXIETUBE_DIG_ALL_PIN_OFF();
        HAL_NIXIETUBE_SEGMENT_ALL_PIN_OFF();
    }
}

// 左四位闪烁控制
void hal_nixietube_set_left_area_flash_enable(uint8_t enable) 
{
    hal_nixietube_left_area_blink_enable = enable;
}

// 右四位闪烁控制
void hal_nixietube_set_right_area_flash_enable(uint8_t enable) 
{
    hal_nixietube_right_area_blink_enable = enable;
}

// 单个数码管闪烁控制（pos: 0-7, 0xFF表示关闭）
void hal_nixietube_set_single_flash_enable(uint8_t pos) 
{
    hal_nixietube_single_blink_position = (pos < 8) ? pos : 0xFF;
}

void hal_nixietube_set_flash_enable(uint8_t enable)
{
    switch (hal_nixietube_mode) 
    {
    case HAL_NIXIETUBE_DISPLAY_FLASH_TYPE_LEFT_AREA:
        hal_nixietube_set_left_area_flash_enable(enable);
        break;
        
    case HAL_NIXIETUBE_DISPLAY_FLASH_TYPE_RIGHT_AREA:
        hal_nixietube_set_right_area_flash_enable(enable);
        break;
        
    case HAL_NIXIETUBE_DISPLAY_FLASH_TYPE_SINGLE:
        hal_nixietube_set_global_enable(enable);
        break;
        
    default:
        break;
    }
}

// 设置指定位置的显示内容（数字）
void hal_nixietube_set_digit(uint8_t pos, uint8_t num) 
{
    if (pos >= 8) 
    {
        return;
    }

    hal_nixietube_data.data[pos] = num;
    hal_nixietube_data.type[pos] = HAL_NIXIETUBE_DISPLAY_DATA_TYPE_NUM;
}

// 设置指定位置的显示内容（字母）
void hal_nixietube_set_alphabet(uint8_t pos, uint8_t alphabet) 
{
    if (pos >= 8) 
    {
        return;
    }

    hal_nixietube_data.data[pos] = alphabet;
    hal_nixietube_data.type[pos] = HAL_NIXIETUBE_DISPLAY_DATA_TYPE_ALPHABET;
}

// 设置指定位置的显示内容（字符）
void hal_nixietube_set_sign(uint8_t pos, uint8_t sign) 
{
    if (pos >= 8) 
    {
        return;
    }
    
    hal_nixietube_data.data[pos] = sign;
    hal_nixietube_data.type[pos] = HAL_NIXIETUBE_DISPLAY_DATA_TYPE_SIGN;
}



//在全区域显示数字内容
void hal_nixietube_digit(uint32 num)
{
    //HAL_NIXIETUBE_DIG_ALL_PIN_OFF();
    //HAL_NIXIETUBE_SEGMENT_ALL_PIN_OFF();
    
	if (num>99999999) 
	{
	    num %= 100000000;
	}
	
	uint8 temp;
	temp = num % 10;                            //提取末位
	hal_nixietube_set_digit(7,num % 10);
	
	for (int i = 7; i > 0; i--) 
	{
		num /= 10;                              //每次滤去末位
		//判断数字的最高位是否为0
		if(num) 
		{
			temp = num % 10;
			hal_nixietube_set_digit(i-1,temp);
		}
		//如果数字的最高位为0，则把其余位赋空
		else 
		{
			hal_nixietube_set_sign(i-1, HAL_NIXIETUBE_SIGN_1);
		}
	}
}

//在左区域显示数字
void hal_nixietube_digit_left(uint16 num)
{
    //HAL_NIXIETUBE_DIG_ALL_PIN_OFF();
    //HAL_NIXIETUBE_SEGMENT_ALL_PIN_OFF();
	if(num > 9999) 
	{
	    num %= 10000;                           //当超过四位数时保留后四位
    }
    
	uint8 temp;
	temp = num % 10;                            //提取末位
	hal_nixietube_set_digit(3, num % 10);
	for(int i=3;i>0;i--) 
	{
		num /= 10;                              //每次滤去末位
		//判断数字的最高位是否为0
		if(num) 
		{
			temp = num % 10;
			hal_nixietube_set_digit(i-1, temp);
		}
		//如果数字的最高位为0，则把其余位赋空
		else 
		{
			hal_nixietube_set_sign(i-1, HAL_NIXIETUBE_SIGN_1);
		}
	}
}

//在右区域显示数字
void hal_nixietube_digit_right(uint16 num)
{
    //HAL_NIXIETUBE_DIG_ALL_PIN_OFF();
    //HAL_NIXIETUBE_SEGMENT_ALL_PIN_OFF();
	if(num > 9999) 
	{
	    num %= 10000;                           //当超过四位数时保留后四位
    }
    
	uint8 temp;
	temp = num % 10;                            //提取末位
	hal_nixietube_set_digit(7, num % 10);
	for(int i = 3; i > 0; i--) 
	{
		num /= 10;                              //每次滤去末位
		//判断数字的最高位是否为0
		if(num) 
		{
			temp = num % 10;
			hal_nixietube_set_digit(i+3, temp);
		}
		//如果数字的最高位为0，则把其余位赋空
		else 
		{
			hal_nixietube_set_sign(i+3, HAL_NIXIETUBE_SIGN_1);
		}
	}
}


//在左区域显示字符串
void hal_nixietube_alphabet_left(char *string)
{
    //HAL_NIXIETUBE_DIG_ALL_PIN_OFF();
    //HAL_NIXIETUBE_SEGMENT_ALL_PIN_OFF();
	for(uint8 i=0;(i<4) && (string[i]!='\0');i++) 
	{
		if((string[i]>=65) && (string[i]<=90)) 
		{
		    //大写字母A-Z的ASCII值为65-90
			hal_nixietube_set_alphabet(i, string[i]-65);
		}
		else if((string[i]>=97) && (string[i]<=122)) 
		{
		    //小写字母a-z的ASCII值为97-122
			hal_nixietube_set_alphabet(i, string[i]-71);
		}
		else if((string[i]>=48) && (string[i]<=57)) 
		{
		    //数字0-9的ASCII值为48-57
			hal_nixietube_set_digit(i, string[i]-48);
		}
		else if(string[i] == ' ') 
		{
			hal_nixietube_set_sign(i, HAL_NIXIETUBE_SIGN_1);
		}
		else if(string[i] == '-') 
		{
			hal_nixietube_set_sign(i, HAL_NIXIETUBE_SIGN_2);
		}
		else 
		{
		    //其余部分用空白补充
			hal_nixietube_set_sign(i, HAL_NIXIETUBE_SIGN_1);
		}
	}
}

//在右区域显示字符串
void hal_nixietube_alphabet_right(char *string)
{
    //HAL_NIXIETUBE_DIG_ALL_PIN_OFF();
    //HAL_NIXIETUBE_SEGMENT_ALL_PIN_OFF();
	for(uint8 i=0;(i<4) && (string[i]!='\0');i++) 
	{
		if((string[i]>=65) && (string[i]<=90)) 
		{
		    //大写字母A-Z的ASCII值为65-90
			hal_nixietube_set_alphabet(i+4, string[i]-65);
		}
		else if((string[i]>=97) && (string[i]<=122)) 
		{
		    //小写字母a-z的ASCII值为97-122
			hal_nixietube_set_alphabet(i+4, string[i]-71);
		}
		else if((string[i]>=48) && (string[i]<=57)) 
		{
		    //数字0-9的ASCII值为48-57
			hal_nixietube_set_digit(i+4, string[i]-48);
		}
		else if(string[i] == ' ') 
		{
			hal_nixietube_set_sign(i+4, HAL_NIXIETUBE_SIGN_1);
		}
		else if(string[i] == '-') 
		{
			hal_nixietube_set_sign(i+4, HAL_NIXIETUBE_SIGN_2);
		}
		else 
		{
		    //其余部分用空白补充
			hal_nixietube_set_sign(i+4, HAL_NIXIETUBE_SIGN_1);
		}
	}
}

//在全区域显示字符串
void hal_nixietube_string(char *string)
{
    //HAL_NIXIETUBE_DIG_ALL_PIN_OFF();
    //HAL_NIXIETUBE_SEGMENT_ALL_PIN_OFF();
	
	for(uint8 i=0;(i<8) && (string[i]!='\0');i++) 
	{
		if((string[i]>=65) && (string[i]<=90)) 
		{
		    //大写字母A-Z的ASCII值为65-90
			hal_nixietube_set_alphabet(i, string[i]-65);
		}
		else if((string[i]>=97) && (string[i]<=122)) 
		{
		    //小写字母a-z的ASCII值为97-122
			hal_nixietube_set_alphabet(i, string[i]-71);
		}
		else if((string[i]>=48) && (string[i]<=57)) 
		{
		    //数字0-9的ASCII值为48-57
			hal_nixietube_set_digit(i, string[i]-48);
		}
		else if(string[i] == ' ') 
		{
			hal_nixietube_set_sign(i, HAL_NIXIETUBE_SIGN_1);
		}
		else if(string[i] == '-') 
		{
			hal_nixietube_set_sign(i, HAL_NIXIETUBE_SIGN_2);
		}
        else if (string[i] == ',')
        {
            hal_nixietube_set_sign(i, HAL_NIXIETUBE_SIGN_3);
        }
        else if (string[i] == '|')
        {
            hal_nixietube_set_sign(i, HAL_NIXIETUBE_SIGN_4);
        }
		else 
		{												//其余部分用空白补充
			hal_nixietube_set_sign(i, HAL_NIXIETUBE_SIGN_1);
		}
	}
}

// 显示字符串到指定区域（0-3 或 4-7）
// area = 0	: 左半部分（0-3）
// area = 1	: 右半部分（4-7）
// colum	: 从哪个位开始显示
void hal_nixietube_string_by_area(uint8 area, uint8 column, char *string) 
{
    //HAL_NIXIETUBE_DIG_ALL_PIN_OFF();
    //HAL_NIXIETUBE_SEGMENT_ALL_PIN_OFF();

    // 计算起始位置（左半部分或右半部分）
    uint8 start_pos = (area == 0) ? column : column + 4;

    // 限制显示范围（0-3 或 4-7）
    uint8 max_pos = (area == 0) ? 3 : 7;

    for (uint8 i = 0; (i < 4) && (string[i] != '\0'); i++) 
    {
        uint8 current_pos = start_pos + i;

        // 检查是否超出目标区域
        if (current_pos > max_pos) 
        {
            break; // 超出范围，停止显示
        }

        // 判断字符类型并显示
        if ((string[i] >= 'A') && (string[i] <= 'Z')) 
        {
            // 大写字母 A-Z
            hal_nixietube_set_alphabet(current_pos, string[i] - 'A');
        }
        else if ((string[i] >= 'a') && (string[i] <= 'z')) 
        {
            // 小写字母 a-z
            hal_nixietube_set_alphabet(current_pos, string[i] - 'a' + 26); // 小写字母从 26 开始
        }
        else if ((string[i] >= '0') && (string[i] <= '9')) 
        {
            // 数字 0-9
            hal_nixietube_set_digit(current_pos, string[i] - '0');
        }
        else if (string[i] == ' ') 
        {
            // 空格
            hal_nixietube_set_sign(current_pos, HAL_NIXIETUBE_SIGN_1);
        }
        else if (string[i] == '-') 
        {
            // 横线
            hal_nixietube_set_sign(current_pos, HAL_NIXIETUBE_SIGN_2);
        }
        // 其他字符默认不显示
    }
}

// 动态扫描显示函数（需在定时器中断或主循环中频繁调用） 动态扫描（建议调用频率 >100Hz） 也就是扫描周期小于10ms
void hal_nixietube_scan(uint8_t rev_flag) 
{
    //-- 放中断里运行，解决屏闪问题，所以此函数需要尽量压缩时间，目前实测为5.2us左右
    // 关闭所有位选（防止鬼影）
    HAL_NIXIETUBE_DIG_ALL_PIN_OFF();
	HAL_NIXIETUBE_SEGMENT_ALL_PIN_OFF();

	// 检查全局显示使能
    if (!hal_nixietube_enable) 
    {
        return;
    }	

    // 设置当前位的段选数据
    uint8_t  cur_pos = hal_nixietube_data.pos;
    uint8_t  data = hal_nixietube_data.data[cur_pos];
    uint8_t  data_type = hal_nixietube_data.type[cur_pos];
    uint16_t seg_value = 0;

	if(rev_flag)						//开启反相显示
	{
	    switch (data_type)
	    {
	    case HAL_NIXIETUBE_DISPLAY_DATA_TYPE_NUM:
	        {
	            // 显示数字
	            seg_value = hal_nixietube_seg_codes_reverse[data % 10];
	        }
	        break;

	    case HAL_NIXIETUBE_DISPLAY_DATA_TYPE_ALPHABET:
	        {
	            // 显示小写字母
	            if (data >= 26)
	            {
	                seg_value = hal_nixietube_seg_table_lowercase_common_anode_reverse[(data-26) % 26];
	            }
	            //显示大写字母
	            else 
	            {
	                seg_value = hal_nixietube_seg_table_letters_common_anode_reverse[data % 26];
	            }
	        }
	        break;

	    case HAL_NIXIETUBE_DISPLAY_DATA_TYPE_SIGN:
	        {
	            // 显示字符
	            seg_value = hal_nixietube_seg_special_sign_reverse[data % 4];
	        }
	        break;
	    }
	}
	else
	 {
		 switch (data_type)
		 {
		 case HAL_NIXIETUBE_DISPLAY_DATA_TYPE_NUM:
			 {
				 // 显示数字
				 seg_value = hal_nixietube_seg_codes[data % 10];
			 }
			 break;
	 
		 case HAL_NIXIETUBE_DISPLAY_DATA_TYPE_ALPHABET:
			 {
				 // 显示小写字母
				 if (data >= 26)
				 {
					 seg_value = hal_nixietube_seg_table_lowercase_common_anode[(data-26) % 26];
				 }
				 //显示大写字母
				 else 
				 {
					 seg_value = hal_nixietube_seg_table_letters_common_anode[data % 26];
				 }
			 }
			 break;
	 
		 case HAL_NIXIETUBE_DISPLAY_DATA_TYPE_SIGN:
			 {
				 // 显示字符
				 seg_value = hal_nixietube_seg_special_sign[data % 4];
			 }
			 break;
		 }
	 }
	
	 // 检查显示条件
	bool should_display = 1;
	switch (hal_nixietube_mode)
	{
	case HAL_NIXIETUBE_DISPLAY_FLASH_TYPE_NONE:
		{
			should_display = 1;
		}
		break;
		
	case HAL_NIXIETUBE_DISPLAY_FLASH_TYPE_LEFT_AREA:
	    {
			if (!hal_nixietube_left_area_blink_enable && hal_nixietube_data.pos <= 3) 
			{
    			should_display = 0; // 左四位闪烁
			} 
		}
		break;
		
	case HAL_NIXIETUBE_DISPLAY_FLASH_TYPE_RIGHT_AREA:
	    {
			if (!hal_nixietube_right_area_blink_enable && hal_nixietube_data.pos >= 4) 
			{
				should_display = 0; // 右四位闪烁
			} 
		}
		break;	
		
	case HAL_NIXIETUBE_DISPLAY_FLASH_TYPE_SINGLE:
    	{
    		if (hal_nixietube_single_blink_position != 0xFF && hal_nixietube_data.pos == hal_nixietube_single_blink_position) 
			{
				should_display = 0; // 单个数码管闪烁
			}
		}
		break;	
	} 

    // 转换段码到引脚输出
	if (hal_nixietube_data.pos == 0 && hal_nixietube_blue_led_enable)
	{
        HAL_NIXIETUBE_SEGMENT_LED_ON();
	}
	else if (hal_nixietube_data.pos == 2 && hal_nixietube_red_led_enable)
	{
        HAL_NIXIETUBE_SEGMENT_LED_ON();
	}
	else if (hal_nixietube_data.pos == 4 && hal_nixietube_green_led_enable)
	{
        HAL_NIXIETUBE_SEGMENT_LED_ON();
	}
	
   if (should_display) 
   {
		if (seg_value & 0x01)
		{
		    HAL_NIXIETUBE_SEGMENT_A_ON();
		}

		if (seg_value & 0x02)
		{
		    HAL_NIXIETUBE_SEGMENT_B_ON();
		}
		
		if (seg_value & 0x04)
		{
		    HAL_NIXIETUBE_SEGMENT_C_ON();
		}

		if (seg_value & 0x08)
		{
		    HAL_NIXIETUBE_SEGMENT_D_ON();
		}

		if (seg_value & 0x10)
		{
		    HAL_NIXIETUBE_SEGMENT_E_ON();
		}

		if (seg_value & 0x20)
		{
		    HAL_NIXIETUBE_SEGMENT_F_ON();
		}
		
		if (seg_value & 0x40)
		{
		    HAL_NIXIETUBE_SEGMENT_G_ON();
		}
	}

	
    // 打开当前位选
    if(rev_flag)						//开启反向显示
    {
	    switch (hal_nixietube_data.pos) 
	    {
	    case 0: HAL_NIXIETUBE_DIG_8_ON(); break;//blueled
	    case 1: HAL_NIXIETUBE_DIG_7_ON(); break;
	    case 2: HAL_NIXIETUBE_DIG_6_ON(); break;//redled
	    case 3: HAL_NIXIETUBE_DIG_5_ON(); break;
	    case 4: HAL_NIXIETUBE_DIG_4_ON(); break;//greenled
	    case 5: HAL_NIXIETUBE_DIG_3_ON(); break;
	    case 6: HAL_NIXIETUBE_DIG_2_ON(); break;
	    case 7: HAL_NIXIETUBE_DIG_1_ON(); break;
	    default: break;
	    }
    }
	else												//正常显示
    {
	    switch (hal_nixietube_data.pos) 
	    {
	    case 0: HAL_NIXIETUBE_DIG_1_ON(); break;//blueled
	    case 1: HAL_NIXIETUBE_DIG_2_ON(); break;
	    case 2: HAL_NIXIETUBE_DIG_3_ON(); break;//redled
	    case 3: HAL_NIXIETUBE_DIG_4_ON(); break;
	    case 4: HAL_NIXIETUBE_DIG_5_ON(); break;//greenled
	    case 5: HAL_NIXIETUBE_DIG_6_ON(); break;
	    case 6: HAL_NIXIETUBE_DIG_7_ON(); break;
	    case 7: HAL_NIXIETUBE_DIG_8_ON(); break;
	    default: break;
	    }
    }

    // 切换到下一位（0-7循环）
    hal_nixietube_data.pos = (hal_nixietube_data.pos + 1) % 8;
}

// 初始化数码管引脚
void hal_nixietube_init(void) 
{
    memset((uint8*)&hal_nixietube_data, 0, sizeof(hal_nixietube_data));

    // 配置段选引脚
    HAL_NIXIETUBE_SEGMENT_A_INIT();
    HAL_NIXIETUBE_SEGMENT_B_INIT();
    HAL_NIXIETUBE_SEGMENT_C_INIT();
    HAL_NIXIETUBE_SEGMENT_D_INIT();
    HAL_NIXIETUBE_SEGMENT_E_INIT();
    HAL_NIXIETUBE_SEGMENT_F_INIT();
    HAL_NIXIETUBE_SEGMENT_G_INIT();
    HAL_NIXIETUBE_SEGMENT_LED_INIT();

    // 配置位选引脚
    HAL_NIXIETUBE_DIG_1_INIT();
    HAL_NIXIETUBE_DIG_2_INIT();
    HAL_NIXIETUBE_DIG_3_INIT();
    HAL_NIXIETUBE_DIG_4_INIT();
    HAL_NIXIETUBE_DIG_5_INIT();
    HAL_NIXIETUBE_DIG_6_INIT();
    HAL_NIXIETUBE_DIG_7_INIT();
    HAL_NIXIETUBE_DIG_8_INIT();

    // 清除一次数码管显示
    HAL_NIXIETUBE_DIG_ALL_PIN_OFF();
    HAL_NIXIETUBE_SEGMENT_ALL_PIN_OFF();

    // 关闭led
    HAL_NIXIETUBE_SEGMENT_LED_OFF();
}

// 示例：显示 "HELLO123"
void hal_nixietube_example(void) 
{
//    hal_nixietube_set_alphabet(0, HAL_NIXIETUBE_ALPHABET_H);
//    hal_nixietube_set_alphabet(1, HAL_NIXIETUBE_ALPHABET_E);
//    hal_nixietube_set_alphabet(2, HAL_NIXIETUBE_ALPHABET_L);
//    hal_nixietube_set_alphabet(3, HAL_NIXIETUBE_ALPHABET_L);
//    hal_nixietube_set_alphabet(4, HAL_NIXIETUBE_ALPHABET_O);
//    hal_nixietube_set_digit(5, 1);
//    hal_nixietube_set_digit(6, 2);
//    hal_nixietube_set_digit(7, 3);
//    uint8 i =123;
//    char buf[8];
//    sprintf(buf,"HELLO%d",i);
//    hal_nixietube_string(buf);
}

#endif

