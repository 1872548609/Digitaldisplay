#include "User_Flash_DIV.h"

Datahander datahander={0};


Flash_Status Write_HalfWord(uint32_t addr, uint16_t data) {
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

Flash_Status Erase_Page(uint32_t page_addr) {
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

Flash_Status Page_Write(Flash_Page_Manager *manager, const void *data, uint32_t size)
{
   //获取页地址
    uint32_t page_addr = STM32_FLASH_BASE + (manager->current_page * STM32_SECTOR_SIZE);

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
        Flash_Status status =Write_HalfWord(addr + i, half_word);
        if (status != FLASH_OK) {
            return status;
        }
    }
    return FLASH_OK;
}


void rank_write_data(Datahander* handler)
{  

 handler->rank++;//写入下一个序列号 
	
	if(handler->rank >=STM32_SECTOR_SIZE/sizeof(Datahander))
	{
		 Erase_Page(STM32_FLASH_BASE + (manager.current_page * STM32_SECTOR_SIZE)); 
		
		 handler->rank %=STM32_SECTOR_SIZE/sizeof(Datahander) ;
		
		 manager.write_offset=(handler->rank)*sizeof(Datahander);	
		
			Page_Write(&manager, handler, sizeof(Datahander));  
			
			return ;
	}
	
	handler->rank %=STM32_SECTOR_SIZE/sizeof(Datahander) ;
	
	manager.write_offset=(handler->rank)*sizeof(Datahander);	 
	
	Page_Write(&manager, handler, sizeof(Datahander));

}

void rank_read_data(Datahander* handler)
{
  read_date(handler);  
}

                                                                          					
//读写数据,自动寻找最新的章节                                                                    				 
void write_date(Datahander* handler)
{
			Flash_Status status = Flash_Page_SafeWrite(&manager, handler, sizeof(*handler));
	   if (status == FLASH_ERROR_PAGE_FULL) {
     
    } else if (status != FLASH_OK) {
        while(1);
    }	 
}

void read_date(Datahander * handler)
{
		const uint32_t page_index = STM32_START_PAGE; // 从第 63 页读取
		const uint32_t packet_size = sizeof(Datahander);     // 从页开头读取
  const uint32_t max_packet = STM32_SECTOR_SIZE/packet_size;//每页最大数据包数
	
		Datahander temphandler;
	 int16_t lastseqence =0;   //最新序列
	uint32_t lastoffset = 0;    //最新偏移
	
	for(uint32_t i =0;i<max_packet;i++)
	{
		  Flash_Status status = Flash_Page_Read(page_index, &temphandler, i*packet_size , sizeof(Datahander));
				if(status != FLASH_OK) {
				continue;
				}	
				// 假设第一个float存储序列号（需确保写入时也遵循此规则）
				if(temphandler.rank > lastseqence&&temphandler.rank!=0xffff) { 
								lastseqence = temphandler.rank;
								lastoffset = i * packet_size;
				}
	}
	
	if(lastseqence >= 0) { // 找到有效数据
		Flash_Status status = Flash_Page_Read(page_index, &temphandler, lastoffset, sizeof(Datahander));
		if(status == FLASH_OK) {
					*handler = temphandler;
		}
	}								
}

