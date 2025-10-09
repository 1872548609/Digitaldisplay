#include "User_Flash_Hal.h"

/**
* @brief 从指定地址读取一个半字 (16 位数据)
* @param faddr : 读取地址 (此地址必须为 2 的倍数!!)
* @retval 读取到的数据 (16 位)
*/
uint16_t stmflash_read_halfword(uint32_t faddr)
{
 return *(volatile uint16_t *)faddr;
}
/**
* @brief 从指定地址开始读出指定长度的数据
* @param raddr : 起始地址
* @param pbuf : 数据指针
* @param length: 要读取的半字(16 位)数,即 2 个字节的整数倍
* @retval 无
*/
void stmflash_read(uint32_t raddr, uint16_t *pbuf, uint16_t length)
{
uint16_t i;
 for (i = 0; i < length; i++)
 {
 pbuf[i] = stmflash_read_halfword(raddr); /* 读取 2 个字节 */
 raddr += 2; /* 偏移 2 个字节 */
 }
}

/**
* @brief 不检查的写入
 这个函数的假设已经把原来的扇区擦除过再写入
* @param waddr : 起始地址 (此地址必须为 2 的倍数!!,否则写入出错!)
* @param pbuf : 数据指针
* @param length : 要写入的 半字(16 位)数
* @retval 无
*/
void stmflash_write_nocheck(uint32_t waddr, uint16_t *pbuf, uint16_t length)
{
uint16_t i;
 for (i = 0; i < length; i++)
 {
			FMC_ProgramHalfWord(waddr, pbuf[i]); //写入一个半字
 waddr += 2; /* 指向下一个半字 */
 }
}



/**
* @brief 在 FLASH 指定位置, 写入指定长度的数据(自动擦除)
* @note 该函数往 STM32 内部 FLASH 指定位置写入指定长度的数据
* 该函数会先检测要写入的扇区是否是空(全 0XFFFF)的?, 如果
* 不是, 则先擦除, 如果是, 则直接往扇区里面写入数据.
* 数据长度不足扇区时，自动被回擦除前的数据
* @param waddr : 起始地址 (此地址必须为 2 的倍数!!,否则写入出错!)
* @param pbuf : 数据指针
* @param length : 要写入的 半字(16 位)数
* @retval 无
*/
uint16_t g_flashbuf[STM32_SECTOR_SIZE]; /* 最多是 1K 字节 */
void stmflash_write(uint32_t waddr, uint16_t *pbuf, uint16_t length)
{
 uint32_t secpos; /* 扇区地址 */
 uint16_t secoff; /* 扇区内偏移地址(16 位字计算) */
 uint16_t secremain; /* 扇区内剩余地址(16 位字计算) */
 uint16_t i;
 uint32_t offaddr; /* 去掉 0X08000000 后的地址 */

//uint32_t erase_addr; /* 擦除错误，这个值为发生错误的扇区地址 */
 if(waddr<STM32_FLASH_BASE||(waddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))
 {

 return; /* 非法地址 */
}
FMC_Unlock(); /* FLASH 解锁 */
 offaddr = waddr - STM32_FLASH_BASE; /* 实际偏移地址 */
 secpos = offaddr / STM32_SECTOR_SIZE; /* 得到页区编号 */
 secoff = (offaddr % STM32_SECTOR_SIZE) / 2; /* 在页区内的偏移(2B 为基本单位) */
secremain = STM32_SECTOR_SIZE / 2 - secoff; /* 页区剩余半字数量 页区大小除以半字大小-页区内半字偏移=剩余半字数量 */
 if (length <= secremain)
 {
 secremain = length; /* 不大于该扇区范围 */
}
 while (1)
 {
 stmflash_read(secpos * STM32_SECTOR_SIZE + STM32_FLASH_BASE,
g_flashbuf, STM32_SECTOR_SIZE / 2); /* 读出整个扇区的内容 页区数量*扇区大小+基地址偏移=当前写入页区地址，读的数据，半字数量 */
 for (i = 0; i < secremain; i++) /* 校验数据 读写入的剩余半字数量的扇区每个半字里面是不是都是0xffff */
 {
 if (g_flashbuf[secoff + i] != 0XFFFF)
 {
 break; /* 需要擦除 */
 }
 }
 if (i < secremain) /* 需要擦除 */
 {
		
 
 FMC_ErasePage(secpos * STM32_SECTOR_SIZE +
	STM32_FLASH_BASE); /* 要擦除的起始地址 */
		
 for (i = 0; i < secremain; i++) /* 复制 */
 {
g_flashbuf[i + secoff] = pbuf[i];
 }
	
 stmflash_write_nocheck(secpos * STM32_SECTOR_SIZE + STM32_FLASH_BASE,
 g_flashbuf, STM32_SECTOR_SIZE / 2); /* 写入整个扇区 */
 }
 else
 { /* 写已经擦除了的,直接写入扇区剩余区间 */
 stmflash_write_nocheck(waddr, pbuf, secremain);
 }
	
 if (length == secremain)
 {
 break; /* 写入结束了 */
 }
 else /* 写入未结束 */
 {
 secpos++; /* 扇区地址增 1 */
 secoff = 0; /* 偏移位置为 0 */
 pbuf += secremain; /* 指针偏移 */
 waddr += secremain * 2; /* 写地址偏移(16 位数据地址,需要*2) */
 length -= secremain; /* 字节(16 位)数递减 */
 if (length > (STM32_SECTOR_SIZE / 2))
 {
 secremain = STM32_SECTOR_SIZE / 2; /* 下一个扇区还是写不完 */
 }
 else
 {
 secremain = length; /* 下一个扇区可以写完了 */
 }
 }
}
 FMC_Lock(); /* 上锁 */
}


//页管理

