#include "User_Flash_Hal.h"

Flash_Page_Manager manager={0};


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


 if (length <= secremain)    /* 不大于该扇区范围 */
 {
 secremain = length;
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
 else     //不需要擦除
 { /* 写已经擦除了的,直接写入扇区剩余区间 */
			stmflash_write_nocheck(waddr, pbuf, secremain);
 }
	
 if (length == secremain)  //长度相同写完
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
		
 if (length > (STM32_SECTOR_SIZE / 2))    //长度大于整个扇区
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






//===================================分割线================================================

// Flash 写入
static Flash_Status Flash_Write_HalfWord(uint32_t addr, uint16_t data) {
    // 检查地址是否对齐（2字节）
    if (addr % 2 != 0) {
        return FLASH_ERROR_ADDR;
    }
    // 检查地址是否在 Flash 范围内
    if (addr < STM32_FLASH_BASE || addr >= STM32_FLASH_BASE + (STM32_SECTOR_SIZE * STM32_FLASH_TOTAL_PAGES)) {
        return FLASH_ERROR_ADDR;
    }
    // 实际写入   
				FMC_Unlock(); /* FLASH 解锁 */
			
    FMC_ProgramHalfWord(addr,data); 

  		FMC_Lock(); /* 上锁 */
    return FLASH_OK;
}
// Flash 页擦除
Flash_Status Flash_Erase_Page(uint32_t page_addr) {
    // 检查地址是否对齐（页大小）
    if (page_addr % STM32_SECTOR_SIZE != 0) {
        return FLASH_ERROR_ADDR;
    }
    // 检查地址是否在 Flash 范围内
    if (page_addr < STM32_FLASH_BASE || page_addr >= STM32_FLASH_BASE + (STM32_SECTOR_SIZE * STM32_FLASH_TOTAL_PAGES)) {
        return FLASH_ERROR_ADDR;
    }
    // 实际擦除（这里用模拟方式）
    FMC_Unlock(); /* FLASH 解锁 */
				
				FMC_ErasePage(page_addr); /* 要擦除的起始地址 */
				
				FMC_Lock(); /* 上锁 */
    return FLASH_OK;
}


// 初始化 Flash 页管理
void Flash_Page_Init(Flash_Page_Manager *manager) {
    manager->current_page = STM32_START_PAGE;      //当前页
    manager->write_offset = 0;      //页偏移
    manager->is_page_full = 0;      //满页
}
//设置 Flash 页管理
void Flash_Page_Set(Flash_Page_Manager *manager,uint32_t currentpage,uint32_t offset){
    manager->current_page = currentpage;      //当前页
    manager->write_offset = offset;      //页偏移
}
// 擦除指定页并重置写入位置
Flash_Status Flash_Page_EraseAndReset(Flash_Page_Manager *manager, uint32_t page_index) {
    if (page_index >= STM32_FLASH_TOTAL_PAGES) {  //如果索引大于等于总页数返回
        return FLASH_ERROR_ADDR;
    }
    Flash_Status status = Flash_Erase_Page(STM32_FLASH_BASE + (page_index * STM32_SECTOR_SIZE));
    if (status != FLASH_OK) {
        return status;
    }
    // 如果是当前页，重置写入位置
    if (page_index == manager->current_page) {
        manager->write_offset = 0;   //清除偏移
        manager->is_page_full = 0;
    }
    return FLASH_OK;
}

// 写入数据到当前页（满页时返回 FLASH_ERROR_PAGE_FULL）
Flash_Status Flash_Page_Write(Flash_Page_Manager *manager, const void *data, uint32_t size) {
    // 如果页已满，返回错误（由上层决定是否擦除）
    if (manager->is_page_full) {
        return FLASH_ERROR_PAGE_FULL;
    }
   //获取页地址
    uint32_t page_addr = STM32_FLASH_BASE + (manager->current_page * STM32_SECTOR_SIZE);
   //获取该页空间剩余 
				uint32_t remaining_space = STM32_SECTOR_SIZE - manager->write_offset;
 
    // 检查是否有足够空间，如果写入长度大于页剩余空间
    if (size > remaining_space) {
        manager->is_page_full = 1; // 标记页已满
        return FLASH_ERROR_PAGE_FULL;
    }
 
    // 16 位对齐写入
    uint32_t addr = page_addr + manager->write_offset;
				//单字节读取数据
    const uint8_t *data_ptr = (const uint8_t *)data;
     //两个字节两个字节写入
    for (uint32_t i = 0; i < size; i += 2) {
        uint16_t half_word; //半字
        if (i + 1 < size) {  //满足两个字节
            // 正常读取 2 字节
            half_word = (data_ptr[i + 1] << 8) | data_ptr[i];
        } else {
            // 不足 2 字节时补 0xFF
            half_word = 0xFF | (data_ptr[i] << 8);
        }
        Flash_Status status =Flash_Write_HalfWord(addr + i, half_word);
        if (status != FLASH_OK) {
            return status;
        }
    }
 
    manager->write_offset += size;
    return FLASH_OK;
}
 
// 安全写入数据（自动处理页满情况）
Flash_Status Flash_Page_SafeWrite(Flash_Page_Manager *manager, const void *data, uint32_t size) {
    Flash_Status status;
    while (1) {
        status = Flash_Page_Write(manager, data, size);
        if (status == FLASH_OK) {
            return FLASH_OK; // 写入成功
        }
								else if (status == FLASH_ERROR_PAGE_FULL) {   // 页已满，擦除当前页并重试
            
            status = Flash_Page_EraseAndReset(manager, manager->current_page);
            if (status != FLASH_OK) {
                return status; // 擦除失败
            }
            // 切换到下一页（可选，用于磨损均衡）
           // manager->current_page = (manager->current_page + 1) % STM32_FLASH_TOTAL_PAGES;
												
        } 
								else {
            return status; // 其他错误
        }
    }
}
 
// 从指定页读取数据
Flash_Status Flash_Page_Read(uint32_t page_index, void *buffer, uint32_t offset, uint32_t size) {
    if (page_index >= STM32_FLASH_TOTAL_PAGES) {
        return FLASH_ERROR_ADDR;
    }
    uint32_t page_addr = STM32_FLASH_BASE + (page_index * STM32_SECTOR_SIZE);
    uint8_t *buffer_ptr = (uint8_t *)buffer;
 
    for (uint32_t i = 0; i < size; i += 2) {
        uint32_t addr = page_addr + offset + i;
        uint16_t half_word = *(volatile uint16_t *)addr;
        buffer_ptr[i] = half_word & 0xFF;
        if (i + 1 < size) {
            buffer_ptr[i + 1] = (half_word >> 8) & 0xFF;
        }
    }
    return FLASH_OK;
}
