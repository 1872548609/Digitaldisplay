 /* Includes */             
#include "superhead.h"

//断言失败执行  8051 架构需要 code 或 const 修饰符指定 ROM 存储。
// 假设 Q_ROM 是 const，Q_ROM_VAR 是空 file = (char_t const *)l_this_file;
/* 
// 嵌入式系统（如8051）可能这样定义：
#define Q_ROM const
#define Q_ROM_VAR __attribute__((section(".rodata")))
 
// 普通平台可能定义为空：
#define Q_ROM
#define Q_ROM_VAR
这些宏通常用于嵌入式系统，普通平台可能不需要。如果它们被定义为 const 或空，通常不会影响赋值。
*/
//其实就是传入了文件名和断言出问题的行
void Q_onAssert(char_t const Q_ROM * const Q_ROM_VAR file, int_t line) {
    while (1) {
        // 硬件调试信号（如点亮 LED）
    }
}



int main(void)
{
	 HAL_System_Init();
  APM_EVAL_DelayInit();

	
	while (1)             
	{		 
	
	}
}


