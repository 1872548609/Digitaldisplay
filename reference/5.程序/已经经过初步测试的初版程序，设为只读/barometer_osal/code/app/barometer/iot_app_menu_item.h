#ifndef _IOT_APP_MENU_ITEM_H
#define _IOT_APP_MENU_ITEM_H

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/
#include "main.h"

/*************************************************-*************************************************
*                                             CONSTANTS
**************************************************-*****************^******************************/
#define IOT_APP_MENU_ITEM_TEXT_STRING_ROOT                          "root"
#define IOT_APP_MENU_ITEM_TEXT_STRING_NORMAL                        "NROM"
#define IOT_APP_MENU_ITEM_TEXT_STRING_PRO                           " PRO"

#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_OUT1MODE              "OUT1MODE"
#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_OUT2MODE              "OUT2MODE"
#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_NONC_OUTPUT           "NONCMODE"
#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_RESPONSE_TIME         " RESTIME"
#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_SCREEN_TEXT_COLOR     "TEXTCLOR"
#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_PRESSURE_UNIT         "PRESUNIT"
#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_SUBSIDIARY_DISPLAY    " SUBDISP"
#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_DISPLAY_SPEED         "DISPSPED"
#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_HYSTERESIS_LEVEL      " HYSLEVE"
#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_DISPLAY_COLOR_LINKAGE "DISPCLOR"
#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_ECO                   " ECO    "
#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_COPY                  "COPY    "
#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_RESET                 "RSET    "

//#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_BASE                  "basicset"
//#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_POWER                 "powerset"
//#define IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_THREE                 "threeset"

//#define IOT_APP_MENU_ITEM_TEXT_STRING_MODE_FINE                     "FinE1234"
//#define IOT_APP_MENU_ITEM_TEXT_STRING_MODE_HIGH_SPEED               " hsp1234"
//#define IOT_APP_MENU_ITEM_TEXT_STRING_MODE_MEGA                     "mega1234"
//#define IOT_APP_MENU_ITEM_TEXT_STRING_MODE_SUPER                    "supr1234"
//#define IOT_APP_MENU_ITEM_TEXT_STRING_MODE_MEGA_LOC                 "loc mega"

//#define IOT_APP_MENU_ITEM_TEXT_STRING_SET_ATT                       " Att set"
//#define IOT_APP_MENU_ITEM_TEXT_STRING_SET_STA                       " STA set"

//#define IOT_APP_MENU_ITEM_TEXT_STRING_END                           "     End"
//#define IOT_APP_MENU_ITEM_TEXT_STRING_FUN                           "STG- Fun"
//#define IOT_APP_MENU_ITEM_TEXT_STRING_FUN_DTEC                      "Dtce    "
//#define IOT_APP_MENU_ITEM_TEXT_STRING_DISP                          "STG-DISP"
//#define IOT_APP_MENU_ITEM_TEXT_STRING_DISP_REV                      "Rev     "
//#define IOT_APP_MENU_ITEM_TEXT_STRING_DISP_ACCT                     "ACCt    "
//#define IOT_APP_MENU_ITEM_TEXT_STRING_SYS                           "STG- SYS"

//#define IOT_APP_MENU_ITEM_TEXT_STRING_END_END                       "   End  "

//#define IOT_APP_DISPLAY_LOCK                                        "     Loc"
//#define IOT_APP_DISPLAY_UNLOCK                                      " unL    "

/*************************************************-*************************************************
*                                           STRUCT DEFINED
**************************************************-*****************^******************************/
// 菜单项结构体
typedef struct tag_iot_app_menu_item_t 
{
    char *text;                                             // 菜单项显示的文本
    bool  enabled;                                          // 标记菜单项是否可用
    int   level;                                            // 菜单项的等级
    //char  which_callback;                                 // 1 进入回调 2 退出回调
    uint8 mode;

    void (*cb_enter)(void *);                               //-- 切换进入菜单时立即执行的回调
    void (*cb_exit)(void *);                                //-- 切换退出菜单时立即执行的回调
    uint8_t (*cb_click_keys_1)(void *, uint8);                 //-- 短按，可作为公共的按键回调执行通用功能
    uint8_t (*cb_click_keys_2)(void *, uint8);                 //-- 短按，可作为item私属的按键功能
    void (*cb_hold_keys)(void *, uint8, uint16 *, uint32);  //-- 长按
    void (*cb_release_keys)(void *, uint8);                 //-- 松开

    struct tag_iot_app_menu_item_t *parent;                 // 指向父菜单项的指针
    
    struct tag_iot_app_menu_item_t **children;              // 指向子菜单项指针数组的指针（动态数组）
    struct tag_iot_app_menu_item_t *child_menu;             // 指向子菜单项的记录指针
    int children_count;                                     // 子菜单项的数量

}iot_app_menu_item_t;


// 菜单系统结构体
typedef struct 
{
    bool running;           	                    // 菜单系统是否运行中
    iot_app_menu_item_t *root;         	            // 根菜单项（顶级菜单）
    iot_app_menu_item_t *current;      	            // 当前所在的菜单项
} iot_app_menu_t;

/*************************************************-*************************************************
*                                             GLOBEL VARIBLE
**************************************************-*****************^******************************/
extern iot_app_menu_t iot_app_menu;

extern iot_app_menu_item_t* iot_app_menu_item_root;
extern iot_app_menu_item_t* iot_app_menu_item_setting_normal;
extern iot_app_menu_item_t* iot_app_menu_item_setting_pro;
extern iot_app_menu_item_t* iot_app_menu_item_setting_out1mode;
extern iot_app_menu_item_t* iot_app_menu_item_setting_out2mode;
extern iot_app_menu_item_t* iot_app_menu_item_setting_nonc_output;
extern iot_app_menu_item_t* iot_app_menu_item_setting_response_time;
extern iot_app_menu_item_t* iot_app_menu_item_setting_screen_text_color;
extern iot_app_menu_item_t* iot_app_menu_item_setting_pressure_unit;
extern iot_app_menu_item_t* iot_app_menu_item_setting_subsidiary_display;
extern iot_app_menu_item_t* iot_app_menu_item_setting_display_speed;
extern iot_app_menu_item_t* iot_app_menu_item_setting_hysteresis_level;
extern iot_app_menu_item_t* iot_app_menu_item_setting_display_color_linkage;
extern iot_app_menu_item_t* iot_app_menu_item_setting_eco;
extern iot_app_menu_item_t* iot_app_menu_item_setting_copy;
extern iot_app_menu_item_t* iot_app_menu_item_setting_reset;

//extern iot_app_menu_t iot_app_menu;
//extern iot_app_menu_item_t* iot_app_menu_item_root;
//extern iot_app_menu_item_t* iot_app_menu_item_setting_basic;
//extern iot_app_menu_item_t* iot_app_menu_item_setting_power;
//extern iot_app_menu_item_t* iot_app_menu_item_setting_three;
//extern iot_app_menu_item_t* iot_app_menu_item_mode_fine ;
//extern iot_app_menu_item_t* iot_app_menu_item_mode_high_speed;
//extern iot_app_menu_item_t* iot_app_menu_item_mode_mega;
//extern iot_app_menu_item_t* iot_app_menu_item_mode_super;
//extern iot_app_menu_item_t* iot_app_menu_item_setting_att;
//extern iot_app_menu_item_t* iot_app_menu_item_setting_sta;
//extern iot_app_menu_item_t* iot_app_menu_item_setting_end;
//extern iot_app_menu_item_t* iot_app_menu_item_setting_end_end;
//extern iot_app_menu_item_t* iot_app_menu_item_stg_fun;
//extern iot_app_menu_item_t* iot_app_menu_item_stg_fun_dtec;
//extern iot_app_menu_item_t* iot_app_menu_item_stg_disp;
//extern iot_app_menu_item_t* iot_app_menu_item_stg_disp_rev;
//extern iot_app_menu_item_t* iot_app_menu_item_stg_disp_acct;
//extern iot_app_menu_item_t* iot_app_menu_item_stg_sys;


/*************************************************-*************************************************
*                                             FUNCTION
**************************************************-*****************^******************************/
extern void iot_app_menu_item_init(void);
extern void iot_app_menu_create_all_item(void);                     // 统一创建菜单树
extern void iot_app_menu_start(void);                               // 启动菜单系统
extern void iot_app_menu_stop(void);                                // 停止菜单系统
extern bool iot_app_menu_is_active(void);                           // 菜单是否启动
extern void iot_app_menu_enter_child_menu(void);                    // 进入当前菜单的子菜单（下一级）
extern void iot_app_menu_return_parent_menu(bool record);           // 返回父菜单（上一级）
extern void iot_app_menu_designative_menu(iot_app_menu_item_t* item);	//切换到指定菜单
extern void iot_app_menu_switch_next(void);                         // 切换到下一个同级菜单
extern void iot_app_menu_switch_prev(void);                         // 切换到上一个同级菜单
/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#endif 

