
#ifndef _MENU_APP_H
#define _MENU_APP_H

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
#define MENU_APP_KEY_MASK                                            (HAL_KEY_SET|HAL_KEY_MODE|HAL_KEY_MEGA|HAL_KEY_SW6|HAL_KEY_RIGHT_SUB|HAL_KEY_LEFT_ADD)

/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
#define MENU_APP_TIMER_EVT                                           0x0001 // 定义测试事件
#define MENU_APP_TIMER_INTERVAL                                      1000 // 定时器间隔(1秒)
        
#define MENU_APP_DISP_BOOT_ANIMATION 								0x0002 // 显示开机动画
#define MENU_APP_ANIMATION_TIMER_INTERVAL                            250 // 定时器间隔(250ms)
       
#define MENU_APP_NIXIETUBE_FLASH_TIMER_INTERVAL						500 
#define MENU_APP_NIXIETUBE_FLASH_EVENT 								0x0004 // 数码管左半闪烁

#define MENU_APP_INTOEND_END_TIMER_INTERVAL							250
#define MENU_APP_INTOEND_END 										0x0008 // 退出菜单关闭事件

#define MENU_APP_ATTSET_FLASH_TIMER_INTERVAL						500
#define MENU_APP_ATTSET_FLASH 										0x0010 // att设置闪烁

#define MENU_APP_FLASHDISP_AFTEROPEN_EVENT							0x0020 // 显示动画后刷新显示

#define MENU_APP_FLASHDISP_COMMON_TIMER_EVT   						1
#define MENU_APP_FLASHDISP_COMMON_EVENT 							0x0040 // 公共闪烁，避免太多事件定义

#define MENU_APP_RESET_SETTING 										0x0080 // 恢复出厂设置

/*************************************************-*************************************************
*                                             FUNCTION
**************************************************-*****************^******************************/
extern void   menu_app_init(uint8 task_id);
extern uint16 menu_app_process_event(uint8 task_id, uint16 events);


/*************************************************-*************************************************
*                                             GLOBEL VARIBLE
**************************************************-*****************^******************************/


/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif

