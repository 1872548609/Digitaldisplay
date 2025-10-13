#ifndef _HAL_MENU_H
#define _HAL_MENU_H

#include "main.h"

// 执行进入还是退出回调
#define INCALLBACK 	1
#define OUTCALLBACK 2

// 菜单项结构体
typedef struct MenuItem MenuItem; // 结构体内部需要引用自身类型
struct MenuItem {
    char text[16];          	// 菜单项显示的文本
    void (*func)(MenuItem*); 	// 回调函数，点击时触发
    bool enabled;           	// 标记菜单项是否可用
    MenuItem *parent;       	// 指向父菜单项的指针
    MenuItem **children;    	// 指向子菜单项指针数组的指针（动态数组）
    int childCount;         	// 子菜单项的数量
	int level;              	// 菜单项的等级
	MenuItem *childrenmenu;     // 指向子菜单项的记录指针
	char whichcallback;			// 1 进入回调 2 退出回调
};

// 菜单系统结构体
typedef struct {
    MenuItem *current;      	// 当前所在的菜单项
    MenuItem *root;         	// 根菜单项（顶级菜单）
    bool running;           	// 菜单系统是否运行中
} MenuSystem;

// 统一创建菜单树，返回树根
MenuItem* CreateTestMenu(void);

void MenuSystem_Init(MenuItem* root);

// 创建并初始化一个菜单项
MenuItem* CreateMenuItem(const char* text, void (*func)(MenuItem*), bool enabled , int level) ;

// 为父菜单项添加子菜单项
void MenuItem_AddChild(MenuItem* parent, MenuItem* child);

// 菜单是否启动
bool MenuSystem_IsActive(void);

// 启动菜单系统
void MenuSystem_Start(void);

// 停止菜单系统
void MenuSystem_Stop(void);

// 相对或绝对查找进入指定菜单
bool Menu_NavigateTo(const char* targetText) ;

// 获取当前菜单项的等级
int Menu_GetCurrentLevel(void);

// 进入当前菜单的子菜单（下一级）
void Menu_Enter(void);

// 返回父菜单（上一级）
void Menu_Back(bool record);

// 切换到下一个同级菜单
void Menu_Next(void);

// 切换到上一个同级菜单
void Menu_Prev(void);

// 获取当前菜单项
MenuItem* Menu_GetCurrent(void);

// 执行当前菜单项的回调函数
void Menu_Execute(char wichcallback);



extern MenuSystem menuSystem;

extern MenuItem* root;

extern MenuItem* basic_setting;

extern MenuItem* power_setting;

extern MenuItem* FinE_mode ;

extern MenuItem* highspeed_mode;

extern MenuItem* mega_mode;

extern MenuItem* super_mode;

extern MenuItem* att_setting;

extern MenuItem* sta_setting;

extern MenuItem* end_setting;

extern MenuItem* end_end;


// 回调函数
extern void onItem1Click(MenuItem* item); 

void onItem2Click(MenuItem* item);







#endif 
