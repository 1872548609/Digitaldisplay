#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif
	
#ifdef _IOT_APP_COMMON_DATA_H
/*************************************************-*************************************************
*                                               NOTE
**************************************************-*****************^******************************/
//-- source insight使用UTF-8编码，File->Reload As Encoding...->UTF-8
//-- source insight设置默认编码，Option->Preferences->Files->Default encoding->UTF-8

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/
// 头文件包含区域（实际使用时需要包含必要的头文件）

/*************************************************-*************************************************
*                                               MACRO
**************************************************-*****************^******************************/
// 宏定义区域

/*************************************************-*************************************************
*                                          CONSTANT DEFINED
**************************************************-*****************^******************************/
// 常量定义区域

/*************************************************-*************************************************
*                                           STRUCT DEFINED
**************************************************-*****************^******************************/
// 结构体定义区域

/*************************************************-*************************************************
*                                          GLOBAL VARIABLES
**************************************************-*****************^******************************/
// 全局变量定义区域
uint8_t system_state = RUN_STATE;

/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/
// 局部变量定义区域	
	
// 菜单回调
void normset_click(MenuItem* item) 
{
	if(item->whichcallback == INCALLBACK)
	{
		DIV_Disp_ByString(MainScreen,"NoRM");         
		DIV_Disp_ByString(SecondScreen,"Mode");	
	}
	else if (item->whichcallback == MENU_CBK_MODE)
	{
		DIV_Disp_ByString(MainScreen,"    ");         
		DIV_Disp_ByString(SecondScreen,"    ");	
		
		Menu_Enter();
		
		Menu_Execute(INCALLBACK);
	}
}

void out1mode_click(MenuItem* item) 
{
	if(item->whichcallback == INCALLBACK)
	{
		Menu_Enter();
		
		Menu_Execute(INCALLBACK);
	}
	else if (item->whichcallback == OUTCALLBACK)
	{
		
	}

}	

void out1easy_click(MenuItem* item) 
{
	if(item->whichcallback == INCALLBACK)
	{
		DIV_Disp_ByString(MainScreen,"EASY");         
		DIV_Disp_ByString(SecondScreen,"Out1");	
	}
	else if (item->whichcallback == MENU_CBK_ADD)
	{
		Menu_Prev();
		
		Menu_Execute(INCALLBACK);
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		Menu_Next();
		
		Menu_Execute(INCALLBACK);
	}
}	

void out1hsy_click(MenuItem* item) 
{
	if(item->whichcallback == INCALLBACK)
	{
		DIV_Disp_ByString(MainScreen," HSY");         
		DIV_Disp_ByString(SecondScreen,"Out1");	
	}
	else if (item->whichcallback == MENU_CBK_ADD)
	{
		Menu_Prev();
		
		Menu_Execute(INCALLBACK);
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		Menu_Next();
		
		Menu_Execute(INCALLBACK);
	}
}	

void out1wcmp_click(MenuItem* item)
{

	if(item->whichcallback == INCALLBACK)
	{
		DIV_Disp_ByString(MainScreen,"WCMP");         
		DIV_Disp_ByString(SecondScreen,"Out1");
	}
	else if (item->whichcallback == MENU_CBK_ADD)
	{
		Menu_Prev();
		
		Menu_Execute(INCALLBACK);
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		Menu_Next();
		
		Menu_Execute(INCALLBACK);
	}

}		


void out2mode_click(MenuItem* item) {}	

void out2no_click(MenuItem* item) {}

void out2easy_click(MenuItem* item) {}	

void out2hsy_click(MenuItem* item) {}	

void out2wcmp_click(MenuItem* item) {}		
	
#endif
