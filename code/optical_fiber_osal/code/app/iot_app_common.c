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
	

#define		compare1_EASY 		1
#define		compare1_HSY 		2
#define		compare1_WCMP 		3

#define   	compare2_off	 	1 
#define		compare2_EASY 		2
#define		compare2_HSY 		3
#define		compare2_WCMP 		4

uint32_t out1compare_status = compare1_EASY;	
	
uint32_t out2compare_status = compare2_off;	

#define  	noncstatus_1o2o     0
#define		noncstatus_1o2c     1
#define		noncstatus_1c2c     3
#define		noncstatus_1c2o     2

#define 	out1nc  1
#define     out1no  0

#define 	out1   2
#define 	out2   1

uint32_t outnonc_status = noncstatus_1o2o;

#define  		R_ON 	     1
#define			G_ON 	     2
#define			AL_RED 	     3
#define			AL_GREEN     4

uint32_t maincolor_status = R_ON;
	
// 菜单回调
// callback 1
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
// callback 2
void out1mode_click(MenuItem* item) 
{
	if(item->whichcallback == INCALLBACK)
	{
		
	}
	else if (item->whichcallback == OUTCALLBACK)
	{
		
	}
	else if (item->whichcallback == MENU_CBK_MODE)
	{
		 Menu_Next();
		
		 Menu_Execute(INCALLBACK);
		
		return;
	}
	else if (item->whichcallback == MENU_CBK_ADD)
	{
		if(out1compare_status>compare1_EASY)
		{
			out1compare_status--;
		}
		else
		{
			out1compare_status=compare1_WCMP;
		}	
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		if(out1compare_status<compare1_WCMP)
		{
			out1compare_status++;
		}
		else
		{
			out1compare_status=compare1_EASY;
		}	
	}
	
	switch(out1compare_status)
	{
		case compare1_EASY:{
			DIV_Disp_ByString(MainScreen,"EASY");         
			DIV_Disp_ByString(SecondScreen,"Out1");	
		}break;
		case compare1_HSY:{
			DIV_Disp_ByString(MainScreen," HSY");         
			DIV_Disp_ByString(SecondScreen,"Out1");	
		}break;
		case compare1_WCMP:{
			DIV_Disp_ByString(MainScreen,"WCMP");         
			DIV_Disp_ByString(SecondScreen,"Out1");
		}break;	
	}
}	

void out2mode_click(MenuItem* item) 
{
	if(item->whichcallback == INCALLBACK)
	{
		
	}
	else if (item->whichcallback == OUTCALLBACK)
	{
		
	}
	else if (item->whichcallback == MENU_CBK_MODE)
	{
		 Menu_Next();
		
		Menu_Execute(INCALLBACK);
		
		return;
	}
	else if (item->whichcallback == MENU_CBK_ADD)
	{
		if(out2compare_status>compare2_off)
		{
			out2compare_status--;
		}
		else
		{
			out2compare_status=compare2_WCMP;
		}	
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		if(out2compare_status<compare2_WCMP)
		{
			out2compare_status++;
		}
		else
		{
			out2compare_status=compare2_off;
		}	
	}
	
	switch(out2compare_status)
	{
		case compare2_off:{
			DIV_Disp_ByString(MainScreen," OFF");         
			DIV_Disp_ByString(SecondScreen,"Out2");
		}break;
		case compare2_EASY:{
			DIV_Disp_ByString(MainScreen,"EASY");         
			DIV_Disp_ByString(SecondScreen,"Out2");	
		}break;
		case compare2_HSY:{
			DIV_Disp_ByString(MainScreen," HSY");         
			DIV_Disp_ByString(SecondScreen,"Out2");	
		}break;
		case compare2_WCMP:{
			DIV_Disp_ByString(MainScreen,"WCMP");         
			DIV_Disp_ByString(SecondScreen,"Out2");
		}break;	
	}

}	

void noncmode_click(MenuItem* item)
{
	if(item->whichcallback == INCALLBACK)
	{
		
	}
	else if (item->whichcallback == OUTCALLBACK)
	{
		
	}
	else if (item->whichcallback == MENU_CBK_MODE)
	{
		Menu_Next();
		
		Menu_Execute(INCALLBACK);
		
		return;
	}
	else if (item->whichcallback == MENU_CBK_ADD)
	{
		if(out2compare_status != compare2_off)
		{
			if(outnonc_status>noncstatus_1o2o)
			{
				outnonc_status=outnonc_status>>1;
			}
			else
			{
				outnonc_status=noncstatus_1c2o;
			}	
		}
		else
		{
			uint32_t temp = outnonc_status & out1;
			
			if(!temp){outnonc_status |= out1;}
			else{outnonc_status &= ~out1;}
		}
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		if(out2compare_status != compare2_off)
		{
			if(outnonc_status<noncstatus_1c2o)
			{
				outnonc_status=outnonc_status<<1;
			}
			else
			{
				outnonc_status=noncstatus_1o2o;
			}	
		}
		else
		{
			uint32_t temp = outnonc_status & out1;
			
			if(!temp){outnonc_status |= out1;}
			else{outnonc_status &= ~out1;}
		}
	}
	
	if(out2compare_status != compare2_off)
	{
		switch(outnonc_status)
		{
			case noncstatus_1o2o:{
				DIV_Disp_ByString(MainScreen,"1o2o");         
				DIV_Disp_ByString(SecondScreen,"NONC");
			}break;
			case noncstatus_1o2c:{
				DIV_Disp_ByString(MainScreen,"1o2c");         
				DIV_Disp_ByString(SecondScreen,"NONC");	
			}break;
			case noncstatus_1c2c:{
				DIV_Disp_ByString(MainScreen,"1c2c");         
				DIV_Disp_ByString(SecondScreen,"NONC");	
			}break;
			case noncstatus_1c2o:{
				DIV_Disp_ByString(MainScreen,"1c2o");         
				DIV_Disp_ByString(SecondScreen,"NONC");
			}break;	
		}
	}
	else
	{
		uint8_t temp = (outnonc_status & out1)?1:0;
		
		switch(temp)
		{
			case out1no:{
				DIV_Disp_ByString(MainScreen," 1NO");         
				DIV_Disp_ByString(SecondScreen,"NONC");
			}break;
			case out1nc:{
				DIV_Disp_ByString(MainScreen," 1NC");         
				DIV_Disp_ByString(SecondScreen,"NONC");
			}break;
		}
	}
}

	


void maincolormode_click(MenuItem* item)
{
	if(item->whichcallback == INCALLBACK)
	{
		
	}
	else if (item->whichcallback == OUTCALLBACK)
	{
		
	}
	else if (item->whichcallback == MENU_CBK_MODE)
	{
		 Menu_Next();
		
		Menu_Execute(INCALLBACK);
		
		return;
	}
	else if (item->whichcallback == MENU_CBK_ADD)
	{
		if(maincolor_status>R_ON)
		{
			maincolor_status--;
		}
		else
		{
			maincolor_status=AL_GREEN;
		}	
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		if(maincolor_status<AL_GREEN)
		{
			maincolor_status++;
		}
		else
		{
			maincolor_status=R_ON;
		}	
	}
	
	switch(maincolor_status)
	{
		case R_ON:{
			DIV_Disp_ByString(MainScreen,"R-oN");         
			DIV_Disp_ByString(SecondScreen,"CLoR");
		}break;
		case G_ON:{
			DIV_Disp_ByString(MainScreen,"G-oN");         
			DIV_Disp_ByString(SecondScreen,"CLoR");	
		}break;
		case AL_RED:{
			DIV_Disp_ByString(MainScreen," RED");         
			DIV_Disp_ByString(SecondScreen,"CLoR");	
		}break;
		case AL_GREEN:{
			DIV_Disp_ByString(MainScreen,"GREN");         
			DIV_Disp_ByString(SecondScreen,"CLoR");
		}break;	
	}

}	


#endif
