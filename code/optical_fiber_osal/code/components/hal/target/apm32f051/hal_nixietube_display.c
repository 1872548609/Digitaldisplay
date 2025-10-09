#include "hal_nixietube_display.h"


// 数字段码表（共阴数码管，无小数点）
const uint8_t seg_codes[10] = 
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
const unsigned char SEG_TABLE_LETTERS_COMMON_ANODE[26] = 
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
const unsigned char SEG_TABLE_LOWERCASE_COMMON_ANODE[26] = 
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
const unsigned char SEG_SPECIAL_SIGN[4] = 
{
	0x00, // 
	0x40, // -
	0x04, // i
	0x06  // 1
};

// 清除所有数码管位显示
//void display_clearalldig(void)
//{
//	GPIO_SetBit(DIG_PORT1, DIG_1 | DIG_2 | DIG_3 | DIG_4 | DIG_5 | DIG_6);
//    GPIO_SetBit(DIG_PORT2, DIG_7 | DIG_8);
//}

// 清除所有数码管段显示
//void display_clearallseg(void)
//{
//	GPIO_SetBit(SEG_PORT,SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G);
//}


// 显示函数：在指定位置显示数字 
void display_nixie(uint8_t num, uint8_t digit) {
    // 1. 关闭所有位选（防止鬼影）
	DISPLAY_CLEAR_ALL_DIG();
	DISPLAY_CLEAR_ALL_SEG();
 
    // 2. 设置段选
    uint16_t seg_value = seg_codes[num % 10]; 
	
	uint16_t pin = 0;
	
	(seg_value & 0x01) ? pin|=SEG_A : 0;                          
	(seg_value & 0x02) ? pin|=SEG_B : 0;                         
	(seg_value & 0x04) ? pin|=SEG_C : 0;                          
	(seg_value & 0x08) ? pin|=SEG_D : 0;                          
	(seg_value & 0x10) ? pin|=SEG_E : 0;                         
	(seg_value & 0x20) ? pin|=SEG_F : 0;                           
	(seg_value & 0x40) ? pin|=SEG_G : 0;                           
	
    GPIO_ClearBit(SEG_PORT,pin);    

		
    // 3. 打开目标位选
    switch (digit) {
        case 0: GPIO_ClearBit(DIG_PORT1, DIG_1); break;
        case 1: GPIO_ClearBit(DIG_PORT1, DIG_2); break;
        case 2: GPIO_ClearBit(DIG_PORT1, DIG_3); break;
        case 3: GPIO_ClearBit(DIG_PORT1, DIG_4); break;
        case 4: GPIO_ClearBit(DIG_PORT1, DIG_5); break;
        case 5: GPIO_ClearBit(DIG_PORT1, DIG_6); break;
        case 6: GPIO_ClearBit(DIG_PORT2, DIG_7); break;
        case 7: GPIO_ClearBit(DIG_PORT2, DIG_8); break;
        default: break;
    }
}

// 显示函数：在指定位置显示字母
void display_alphabet(uint8_t alphabet, uint8_t digit) 
{
    // 1. 关闭所有位选（防止鬼影）
	DISPLAY_CLEAR_ALL_DIG();
	DISPLAY_CLEAR_ALL_SEG();
 
    // 2. 设置段选
    uint16_t seg_value = SEG_TABLE_LETTERS_COMMON_ANODE[alphabet % 26]; 
	
	uint16_t pin = 0;
	
	(seg_value & 0x01) ? pin|=SEG_A : 0;                          
	(seg_value & 0x02) ? pin|=SEG_B : 0;                         
	(seg_value & 0x04) ? pin|=SEG_C : 0;                          
	(seg_value & 0x08) ? pin|=SEG_D : 0;                          
	(seg_value & 0x10) ? pin|=SEG_E : 0;                         
	(seg_value & 0x20) ? pin|=SEG_F : 0;                           
	(seg_value & 0x40) ? pin|=SEG_G : 0;                           
	
    GPIO_ClearBit(SEG_PORT,pin);    

		
    // 3. 打开目标位选
    switch (digit) {
        case 0: GPIO_ClearBit(DIG_PORT1, DIG_1); break;
        case 1: GPIO_ClearBit(DIG_PORT1, DIG_2); break;
        case 2: GPIO_ClearBit(DIG_PORT1, DIG_3); break;
        case 3: GPIO_ClearBit(DIG_PORT1, DIG_4); break;
        case 4: GPIO_ClearBit(DIG_PORT1, DIG_5); break;
        case 5: GPIO_ClearBit(DIG_PORT1, DIG_6); break;
        case 6: GPIO_ClearBit(DIG_PORT2, DIG_7); break;
        case 7: GPIO_ClearBit(DIG_PORT2, DIG_8); break;
        default: break;
    }
}


// 定义显示缓冲区（8位，每位存储要显示的数字或字母索引）
uint8_t display_buffer[8] = {0}; // 默认全0
uint8_t display_type[8] = {0};   // 0=数字, 1=字母
uint8_t current_digit = 0;       // 当前扫描的位数（0-7）

// 闪烁控制变量
volatile bool display_enabled = true;          	// 全局显示使能
volatile bool display_mode = 1;				   	// 显示模式  1正常运行 2 左闪烁 3 右闪烁 4 单闪烁
volatile bool left_blink_enabled = false;      	// 左四位闪烁使能
volatile bool right_blink_enabled = false;     	// 右四位闪烁使能
volatile uint8_t single_blink_pos = 0xff;     	 	// 单个数码管闪烁位置（0xFF表示关闭）
volatile bool left_blink_en = false;      	// 左四位闪烁使能
volatile bool right_blink_en = false;     	// 右四位闪烁使能
volatile uint8_t single_blink_posen = false;     	 	// 单个数码管闪烁位置（0xFF表示关闭）
uint8 LED1_EN = 0;
uint8 LED2_EN = 0;
uint8 LED3_EN = 0;

// 显示模式设置
void display_modeset(uint8_t mode)
{
	display_mode = mode;
}

// 全局显示开关
void display_set_global_enable(bool ifenabled) {
    display_enabled = ifenabled;
    if (!ifenabled) {
        DISPLAY_CLEAR_ALL_DIG();
        DISPLAY_CLEAR_ALL_SEG();
    }
}

// 左四位闪烁控制
void display_set_left_en(bool ifenabled) {

    left_blink_enabled = ifenabled;
}

// 右四位闪烁控制
void display_set_right_en(bool ifenabled) {
    right_blink_enabled = ifenabled;
}

// 单个数码管闪烁控制（pos: 0-7, 0xFF表示关闭）
void display_set_single_en(uint8_t pos) {
    single_blink_pos = (pos < 8) ? pos : 0xFF;
}


// 动态扫描显示函数（需在定时器中断或主循环中频繁调用） 动态扫描（建议调用频率 >100Hz） 也就是扫描周期小于10ms
void display_scan(void) 
{
    //-- 放中断里运行，解决屏闪问题，所以此函数需要尽量压缩时间，目前实测为5.2us左右
    // 关闭所有位选（防止鬼影）
    DISPLAY_CLEAR_ALL_DIG();
	DISPLAY_CLEAR_ALL_SEG();

	// 检查全局显示使能
    if (!display_enabled) 
    {
        return;
    }	

    // 设置当前位的段选数据
    uint8_t  data = display_buffer[current_digit];
    uint16_t seg_value = 0;

    if (display_type[current_digit] == 1) 
    {
        // 显示数字
        seg_value = seg_codes[data % 10];
    }
	else if (display_type[current_digit] == 2) 
	{
        // 显示小写字母
        if (data >= 26)
        {
			seg_value = SEG_TABLE_LOWERCASE_COMMON_ANODE[(data-26) % 26];
		}
		//显示大写字母
		else 
		{
        	seg_value = SEG_TABLE_LETTERS_COMMON_ANODE[data % 26];
		}
    }
	else if(display_type[current_digit] == 3) 
	{
		// 显示字符
		seg_value = SEG_SPECIAL_SIGN[data % 4];
	}
	
	 // 检查显示条件
	bool should_display = 1;
	switch(display_mode)
	{
	case 1:
		{
			should_display = 1;
		}
		break;
		
	case 2:
	    {
			if (!left_blink_enabled && current_digit <= 3) 
			{
    			should_display = 0; // 左四位闪烁
			} 
		}
		break;
		
	case 3:
	    {
			if (!right_blink_enabled && current_digit >= 4) 
			{
				should_display = 0; // 右四位闪烁
			} 
		}
		break;	
		
	case 4:
    	{
    		if (single_blink_pos != 0xFF && current_digit == single_blink_pos) 
			{
				should_display = 0; // 单个数码管闪烁
			}
		}
		break;	
	} 

    // 转换段码到引脚输出
	uint16_t pin = 0;
	
	if(current_digit == 0 && LED1_EN)
	{
		pin |= SEG_LED;
	}
	else if(current_digit == 2 && LED2_EN)
	{
		pin |= SEG_LED;
	}
	else if(current_digit == 4 && LED3_EN)
	{
		pin |= SEG_LED;
	}
	
   if (should_display) 
   {
		(seg_value & 0x01) ? (pin |= SEG_A) : 0;
		(seg_value & 0x02) ? (pin |= SEG_B) : 0;
		(seg_value & 0x04) ? (pin |= SEG_C) : 0;
		(seg_value & 0x08) ? (pin |= SEG_D) : 0;
		(seg_value & 0x10) ? (pin |= SEG_E) : 0;
		(seg_value & 0x20) ? (pin |= SEG_F) : 0;
		(seg_value & 0x40) ? (pin |= SEG_G) : 0;
	   
		if(!(pin & SEG_LED))
		{
			//GPIO_SetBit(SEG_PORT, SEG_LED);
			SEG_PORT->BSC = (uint32_t)SEG_LED;
		}
		
		//GPIO_ClearBit(SEG_PORT, pin);
		SEG_PORT->BR = (uint32_t)pin;
	}

	
    // 打开当前位选
    switch (current_digit) 
    {
    /*case 0: GPIO_ClearBit(DIG_PORT1, DIG_1); break;//blueled
    case 1: GPIO_ClearBit(DIG_PORT1, DIG_2); break;
    case 2: GPIO_ClearBit(DIG_PORT1, DIG_3); break;//redled
    case 3: GPIO_ClearBit(DIG_PORT1, DIG_4); break;
    case 4: GPIO_ClearBit(DIG_PORT1, DIG_5); break;//greenled
    case 5: GPIO_ClearBit(DIG_PORT1, DIG_6); break;
    case 6: GPIO_ClearBit(DIG_PORT2, DIG_7); break;
    case 7: GPIO_ClearBit(DIG_PORT2, DIG_8); break;*/
    case 0: DIG_PORT1->BR = (uint32_t)DIG_1; break;//blueled
    case 1: DIG_PORT1->BR = (uint32_t)DIG_2; break;
    case 2: DIG_PORT1->BR = (uint32_t)DIG_3; break;//redled
    case 3: DIG_PORT1->BR = (uint32_t)DIG_4; break;
    case 4: DIG_PORT1->BR = (uint32_t)DIG_5; break;//greenled
    case 5: DIG_PORT1->BR = (uint32_t)DIG_6; break;
    case 6: DIG_PORT2->BR = (uint32_t)DIG_7; break;
    case 7: DIG_PORT2->BR = (uint32_t)DIG_8; break;
    default: break;
    }

    // 切换到下一位（0-7循环）
    current_digit = (current_digit + 1) % 8;
}
//{
//    // 关闭所有位选（防止鬼影）
//    DISPLAY_CLEAR_ALL_DIG();
//	DISPLAY_CLEAR_ALL_SEG();

//	// 检查全局显示使能
//    if (!display_enabled) 
//    {
//        return;
//    }

//	IOT_APP_OPTICAL_TEST_IO_ON();
//	

//    // 设置当前位的段选数据
//    uint8_t  data = display_buffer[current_digit];
//    uint16_t seg_value;

//    if (display_type[current_digit] == 1) 
//    {
//        // 显示数字
//        seg_value = seg_codes[data % 10];
//    }
//	else if (display_type[current_digit] == 2) 
//	{
//        // 显示小写字母
//        if (data >= 26)
//        {
//			seg_value = SEG_TABLE_LOWERCASE_COMMON_ANODE[(data-26) % 26];
//		}
//		//显示大写字母
//		else 
//		{
//        	seg_value = SEG_TABLE_LETTERS_COMMON_ANODE[data % 26];
//		}
//    }
//	else if(display_type[current_digit] == 3) 
//	{
//		// 显示字符
//		seg_value = SEG_SPECIAL_SIGN[data % 4];
//	}
//	
//	 // 检查显示条件
//	bool should_display = 1;
//	switch(display_mode)
//	{
//	case 1:
//		{
//			should_display = 1;
//		}
//		break;
//		
//	case 2:
//	    {
//			if (!left_blink_enabled && current_digit <= 3) 
//			{
//    			should_display = 0; // 左四位闪烁
//			} 
//		}
//		break;
//		
//	case 3:
//	    {
//			if (!right_blink_enabled && current_digit >= 4) 
//			{
//				should_display = 0; // 右四位闪烁
//			} 
//		}
//		break;	
//		
//	case 4:
//    	{
//    		if (single_blink_pos != 0xFF && current_digit == single_blink_pos) 
//			{
//				should_display = 0; // 单个数码管闪烁
//			}
//		}
//		break;	
//	} 

//    // 转换段码到引脚输出
//	uint16_t pin = 0;
//	
//	if(current_digit == 0 && LED1_EN)
//	{
//		pin |= SEG_LED;
//	}
//	else if(current_digit == 2 && LED2_EN)
//	{
//		pin |= SEG_LED;
//	}
//	else if(current_digit == 4 && LED3_EN)
//	{
//		pin |= SEG_LED;
//	}
//	
//   if (should_display) 
//   {
//		(seg_value & 0x01) ? (pin |= SEG_A) : 0;
//		(seg_value & 0x02) ? (pin |= SEG_B) : 0;
//		(seg_value & 0x04) ? (pin |= SEG_C) : 0;
//		(seg_value & 0x08) ? (pin |= SEG_D) : 0;
//		(seg_value & 0x10) ? (pin |= SEG_E) : 0;
//		(seg_value & 0x20) ? (pin |= SEG_F) : 0;
//		(seg_value & 0x40) ? (pin |= SEG_G) : 0;
//	   
//		if(!(pin & SEG_LED))
//		{
//			//GPIO_SetBit(SEG_PORT, SEG_LED);
//			SEG_PORT->BSC = (uint32_t)SEG_LED;
//		}
//		
//		//GPIO_ClearBit(SEG_PORT, pin);
//		SEG_PORT->BSC = (uint32_t)pin;
//	}

//	
//    // 打开当前位选
//    switch (current_digit) 
//    {
//    /*case 0: GPIO_ClearBit(DIG_PORT1, DIG_1); break;//blueled
//    case 1: GPIO_ClearBit(DIG_PORT1, DIG_2); break;
//    case 2: GPIO_ClearBit(DIG_PORT1, DIG_3); break;//redled
//    case 3: GPIO_ClearBit(DIG_PORT1, DIG_4); break;
//    case 4: GPIO_ClearBit(DIG_PORT1, DIG_5); break;//greenled
//    case 5: GPIO_ClearBit(DIG_PORT1, DIG_6); break;
//    case 6: GPIO_ClearBit(DIG_PORT2, DIG_7); break;
//    case 7: GPIO_ClearBit(DIG_PORT2, DIG_8); break;*/
//    case 0: DIG_PORT1->BR = (uint32_t)DIG_1; break;//blueled
//    case 1: DIG_PORT1->BR = (uint32_t)DIG_2; break;
//    case 2: DIG_PORT1->BR = (uint32_t)DIG_3; break;//redled
//    case 3: DIG_PORT1->BR = (uint32_t)DIG_4; break;
//    case 4: DIG_PORT1->BR = (uint32_t)DIG_5; break;//greenled
//    case 5: DIG_PORT1->BR = (uint32_t)DIG_6; break;
//    case 6: DIG_PORT2->BR = (uint32_t)DIG_7; break;
//    case 7: DIG_PORT2->BR = (uint32_t)DIG_8; break;
//    default: break;
//    }

//	
//	
//    // 切换到下一位（0-7循环）
//    current_digit = (current_digit + 1) % 8;


//    
//    IOT_APP_OPTICAL_TEST_IO_OFF();
//}

// 设置指定位置的显示内容（数字）
void display_set_digit(uint8_t pos, uint8_t num) {
    if (pos < 8) {
        display_buffer[pos] = num;
        display_type[pos] = 1; // 标记为数字
    }
}

// 设置指定位置的显示内容（字母）
void display_set_alphabet(uint8_t pos, uint8_t alphabet) {
    if (pos < 8) {
        display_buffer[pos] = alphabet;
        display_type[pos] = 2; // 标记为字母
    }
}

// 设置指定位置的显示内容（字符）
void display_set_sign(uint8_t pos, uint8_t sign) {
    if (pos < 8) {
        display_buffer[pos] = sign;
        display_type[pos] = 3; // 标记为字符
    }
}



//在全区域显示数字内容
void display_digit(uint32 num)
{
//	DISPLAY_CLEAR_ALL_DIG();
//	DISPLAY_CLEAR_ALL_SEG();
	if(num>99999999) num %= 100000000;
	uint8 temp;
	temp = num % 10;									//提取末位
	display_set_digit(7,num % 10);
	for(int i=7;i>0;i--) {
		num /= 10;										//每次滤去末位
		//判断数字的最高位是否为0
		if(num) {
			temp = num % 10;
			display_set_digit(i-1,temp);
		}
		//如果数字的最高位为0，则把其余位赋空
		else {
			display_set_sign(i-1,sign_1);
		}
	}
}

//在左区域显示数字
void display_digit_left(uint16 num)
{
//	DISPLAY_CLEAR_ALL_DIG();
//	DISPLAY_CLEAR_ALL_SEG();
	if(num > 9999) num %= 10000;						//当超过四位数时保留后四位
	uint8 temp;
	temp = num % 10;									//提取末位
	display_set_digit(3,num % 10);
	for(int i=3;i>0;i--) {
		num /= 10;										//每次滤去末位
		//判断数字的最高位是否为0
		if(num) {
			temp = num % 10;
			display_set_digit(i-1,temp);
		}
		//如果数字的最高位为0，则把其余位赋空
		else {
			display_set_sign(i-1,sign_1);
		}
	}
}

//在右区域显示数字
void display_digit_right(uint16 num)
{
//	DISPLAY_CLEAR_ALL_DIG();
//	DISPLAY_CLEAR_ALL_SEG();
	if(num > 9999) num %= 10000;						//当超过四位数时保留后四位
	uint8 temp;
	temp = num % 10;									//提取末位
	display_set_digit(7,num % 10);
	for(int i=3;i>0;i--) {
		num /= 10;										//每次滤去末位
		//判断数字的最高位是否为0
		if(num) {
			temp = num % 10;
			display_set_digit(i+3,temp);
		}
		//如果数字的最高位为0，则把其余位赋空
		else {
			display_set_sign(i+3,sign_1);
		}
	}
}


//在左区域显示字符串
void display_alphabet_left(char *string)
{
//	DISPLAY_CLEAR_ALL_DIG();
//	DISPLAY_CLEAR_ALL_SEG();
	for(uint8 i=0;(i<4) && (string[i]!='\0');i++) {
		if((string[i]>=65) && (string[i]<=90)) {			//大写字母A-Z的ASCII值为65-90
			display_set_alphabet(i,string[i]-65);
		}
		else if((string[i]>=97) && (string[i]<=122)) {		//小写字母a-z的ASCII值为97-122
			display_set_alphabet(i,string[i]-71);
		}
		else if((string[i]>=48) && (string[i]<=57)) {		//数字0-9的ASCII值为48-57
			display_set_digit(i,string[i]-48);
		}
		else if(string[i] == ' ') {
			display_set_sign(i,sign_1);
		}
		else if(string[i] == '-') {
			display_set_sign(i,sign_2);
		}
		else {												//其余部分用空白补充
			display_set_sign(i,sign_1);
		}
	}
}

//在右区域显示字符串
void display_alphabet_right(char *string)
{
//	DISPLAY_CLEAR_ALL_DIG();
//	DISPLAY_CLEAR_ALL_SEG();
	for(uint8 i=0;(i<4) && (string[i]!='\0');i++) {
		if((string[i]>=65) && (string[i]<=90)) {			//大写字母A-Z的ASCII值为65-90
			display_set_alphabet(i+4,string[i]-65);
		}
		else if((string[i]>=97) && (string[i]<=122)) {		//小写字母a-z的ASCII值为97-122
			display_set_alphabet(i+4,string[i]-71);
		}
		else if((string[i]>=48) && (string[i]<=57)) {		//数字0-9的ASCII值为48-57
			display_set_digit(i+4,string[i]-48);
		}
		else if(string[i] == ' ') {
			display_set_sign(i+4,sign_1);
		}
		else if(string[i] == '-') {
			display_set_sign(i+4,sign_2);
		}
		else {												//其余部分用空白补充
			display_set_sign(i+4,sign_1);
		}
	}
}

//在全区域显示字符串
void display_string(char *string)
{
//	DISPLAY_CLEAR_ALL_DIG();
//	DISPLAY_CLEAR_ALL_SEG();
	
	for(uint8 i=0;(i<8) && (string[i]!='\0');i++) {
		if((string[i]>=65) && (string[i]<=90)) {			//大写字母A-Z的ASCII值为65-90
			display_set_alphabet(i,string[i]-65);
		}
		else if((string[i]>=97) && (string[i]<=122)) {		//小写字母a-z的ASCII值为97-122
			display_set_alphabet(i,string[i]-71);
		}
		else if((string[i]>=48) && (string[i]<=57)) {		//数字0-9的ASCII值为48-57
			display_set_digit(i,string[i]-48);
		}
		else if(string[i] == ' ') {
			display_set_sign(i,sign_1);
		}
		else if(string[i] == '-') {
			display_set_sign(i,sign_2);
		}
		else {												//其余部分用空白补充
			display_set_sign(i,sign_1);
		}
	}
}

// 显示字符串到指定区域（0-3 或 4-7）
// area = 0	: 左半部分（0-3）
// area = 1	: 右半部分（4-7）
// colum	: 从哪个位开始显示
void display_stringbyarea(uint8 colum, char *string, uint8 area) {
    	
//	DISPLAY_CLEAR_ALL_DIG();
//  DISPLAY_CLEAR_ALL_SEG();

    // 计算起始位置（左半部分或右半部分）
    uint8 start_pos = (area == 0) ? colum : colum + 4;

    // 限制显示范围（0-3 或 4-7）
    uint8 max_pos = (area == 0) ? 3 : 7;

    for (uint8 i = 0; (i < 4) && (string[i] != '\0'); i++) {
        uint8 current_pos = start_pos + i;

        // 检查是否超出目标区域
        if (current_pos > max_pos) {
            break; // 超出范围，停止显示
        }

        // 判断字符类型并显示
        if ((string[i] >= 'A') && (string[i] <= 'Z')) {          // 大写字母 A-Z
            display_set_alphabet(current_pos, string[i] - 'A');
        }
        else if ((string[i] >= 'a') && (string[i] <= 'z')) {     // 小写字母 a-z
            display_set_alphabet(current_pos, string[i] - 'a' + 26); // 小写字母从 26 开始
        }
        else if ((string[i] >= '0') && (string[i] <= '9')) {     // 数字 0-9
            display_set_digit(current_pos, string[i] - '0');
        }
        else if (string[i] == ' ') {                             // 空格
            display_set_sign(current_pos, sign_1);
        }
        else if (string[i] == '-') {                             // 横线
            display_set_sign(current_pos, sign_2);
        }
        // 其他字符默认不显示
    }
}

// 示例：显示 "HELLO123"
void display_example(void) {
//    display_set_alphabet(0, alphabet_H);
//    display_set_alphabet(1, alphabet_E);
//    display_set_alphabet(2, alphabet_L);
//    display_set_alphabet(3, alphabet_L);
//    display_set_alphabet(4, alphabet_O);
//    display_set_digit(5, 1);
//    display_set_digit(6, 2);
//    display_set_digit(7, 3);
//	uint8 i =123;
//	char buf[8];
//	sprintf(buf,"HELLO%d",i);
//	display_string(buf);

}

// 初始化数码管引脚
void iot_nixie_init(void) 
{
	
	GPIO_Config_T gpioConfig;

#ifdef SEPARATE_PORT
	
	RCM_EnableAHBPeriphClock(SEG_PORT_PERIPH_PORT);
	RCM_EnableAHBPeriphClock(DIG_PORT1_PERIPH_PORT);
	RCM_EnableAHBPeriphClock(DIG_PORT2_PERIPH_PORT);

	// 配置段选引脚（PA8-PA14）
	gpioConfig.pin = SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G |SEG_LED;
	gpioConfig.mode = GPIO_MODE_OUT;
	gpioConfig.outtype = GPIO_OUT_TYPE_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	gpioConfig.pupd = GPIO_PUPD_PD;
	GPIO_Config(SEG_PORT, &gpioConfig);
	
	// 配置位选引脚（PB3-PB8）
	gpioConfig.pin = DIG_1|DIG_2|DIG_3|DIG_4|DIG_5 |DIG_6;
	GPIO_Config(DIG_PORT1, &gpioConfig);
	

	// 配置位选引脚（PF0-PF1）
	gpioConfig.pin = DIG_7|DIG_8;
	GPIO_Config(DIG_PORT2, &gpioConfig);
	

	// 清除一次数码管显示
	DISPLAY_CLEAR_ALL_DIG();
	DISPLAY_CLEAR_ALL_SEG();
	
	// 关闭led
	GPIO_SetBit(SEG_PORT, SEG_LED);
#else

	// 位选端口现在是分开的以后要移植新芯片就都放在同一个PORTB端口初始化

#endif
}


