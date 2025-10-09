
#ifndef _IOT_APP_H
#define _IOT_APP_H

#ifdef __cplusplus
extern "C"
{
#endif

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                             DEFINES
**************************************************-*****************^******************************/
#define IOT_APP_KEY_MASK                                            (HAL_KEY_SET|HAL_KEY_MODE|HAL_KEY_MEGA|HAL_KEY_SW6|HAL_KEY_RIGHT_SUB|HAL_KEY_LEFT_ADD)

// 目标扇区（SECTOR_1，地址 0x0800f800 ~ 0x0800Fc00）
#define TARGET_SECTOR_ADDR  0x0800f800
#define TARGET_SECTOR_NUM   ((TARGET_SECTOR_ADDR - STM32_FLASH_BASE) / STM32_SECTOR_SIZE)	
/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
#define IOT_APP_TIMER_EVT                                           0x0001 // 定义测试事件
#define IOT_APP_TIMER_INTERVAL                                      5 // 定时器间隔

#define IOT_APP_NIXIETUBE_FLASH_TIMER_INTERVAL						500 
#define IOT_APP_NIXIETUBE_FLASH_EVENT 								0x0002 // 两点设置数码管左半闪烁
	
#define IOT_APP_NIXIETUBE_DISPSETMS_TIMER_INTERVAL					100 
#define IOT_APP_NIXIETUBE_DISPSETMS_EVENT 							0x0004 // 数码管左半闪烁


#define IOT_APP_THRESHOLDVALUE_AUTOSETBLINK_EVENT					0x0008 // 自动设置显示闪烁


#define IOT_APP_FLASH_THRESHOLD_EVENT 								0x0010 // 刷新阈值显示

#define IOT_APP_SET_OUTMODECHANGE_EVENT 							0x0020 // 输出模式设置


/*************************************************-*************************************************
*                                             FUNCTION
**************************************************-*****************^******************************/
extern void   iot_app_init(uint8 task_id);
extern uint16 iot_app_process_event(uint8 task_id, uint16 events);

extern void Flash_Write_SetValue(void);

extern void Flash_Read_SetValue(void);

extern void AutoSetThresoldValue(void);
/*************************************************-*************************************************
*                                             GLOBEL VARIBLE
**************************************************-*****************^******************************/
extern uint8_t addkeymodechoice ; // 1 初始化 0 锁
extern uint8_t subkeymodechoice  ; // 1 初始化 0 锁  
extern uint8_t modekeymodechoice  ; // 1 初始化0 锁 
extern uint8_t setkeymodechoice ; // 1 初始化 0 锁 
extern uint8_t Leftandright_key_setting_mode ;// 0 设置阈值 1 设置输出模式
/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif

