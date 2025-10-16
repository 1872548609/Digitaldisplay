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
uint32_t out1compare_status = compare1_EASY;	
	
uint32_t out2compare_status = compare2_off;	

uint32_t outnonc_status = noncstatus_1o2o;

uint32_t maincolor_status = R_ON;

uint32_t unitconver_status = KPR;

uint32_t secscreen_status = Std;

uint32_t dispeed_status = dispeed_250ms;

uint32_t diflevel_status = diflevel_level1;

uint32_t coloract_status = out1;

uint32_t eco_status = eco_off;

uint32_t copy_status = copy_off;

uint32_t facrecover_status = facrecoff;
	
// 菜单回调
// callback 1

void root_Click(MenuItem* item) 
{
	if (item->whichcallback == OUTCALLBACK)
	{
		DIV_Disp_ByString(MainScreen,"    ");         
		DIV_Disp_ByString(SecondScreen,"    ");	
	}
}

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

void proset_click(MenuItem* item) 
{
	if(item->whichcallback == INCALLBACK)
	{
		DIV_Disp_ByString(MainScreen," PRO");         
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
		iot_mainbacklight_set(BACKLIGHT_RED);
	}
	else if (item->whichcallback == OUTCALLBACK)
	{
		iot_mainbacklight_set(BACKLIGHT_GREEN);
	}
	else if (item->whichcallback == MENU_CBK_MODE)
	{
		 Menu_Execute(OUTCALLBACK);
		
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
				outnonc_status--;
			}
			else
			{
				outnonc_status=noncstatus_1c2c;
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
			if(outnonc_status<noncstatus_1c2c)
			{
				outnonc_status++;
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

void unitconvermode_click(MenuItem* item)
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
		if(unitconver_status>bAr)
		{
			unitconver_status--;
		}
		else
		{
			unitconver_status=inHG;
		}	
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		if(unitconver_status<inHG)
		{
			unitconver_status++;
		}
		else
		{
			unitconver_status=bAr;
		}	
	}
	
	switch(unitconver_status)
	{
		case bAr:{
			DIV_Disp_ByString(MainScreen,"bAr");         
			DIV_Disp_ByString(SecondScreen,"Unit");
		}break;
		case KgF:{
			DIV_Disp_ByString(MainScreen,"KgF");         
			DIV_Disp_ByString(SecondScreen,"Unit");	
		}break;
		case KPR:{
			DIV_Disp_ByString(MainScreen," KPR");         
			DIV_Disp_ByString(SecondScreen,"Unit");	
		}break;
		case MPR:{
			DIV_Disp_ByString(MainScreen," MPR");         
			DIV_Disp_ByString(SecondScreen,"Unit");
		}break;
    case PSI:{
			DIV_Disp_ByString(MainScreen," PSI");         
			DIV_Disp_ByString(SecondScreen,"Unit");
		}break;
    case MMHG:{
			DIV_Disp_ByString(MainScreen,"MMHG");         
			DIV_Disp_ByString(SecondScreen,"Unit");
		}break;
    case inHG:{
			DIV_Disp_ByString(MainScreen,"inHG");         
			DIV_Disp_ByString(SecondScreen,"Unit");
		}break;
	}

}	

void secscreenmode_click(MenuItem* item)
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
		if(secscreen_status>Std)
		{
			secscreen_status--;
		}
		else
		{
			secscreen_status=CuSt;
		}	
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		if(secscreen_status<CuSt)
		{
			secscreen_status++;
		}
		else
		{
			secscreen_status=Std;
		}	
	}
	switch(secscreen_status)
	{
		case OFF:{
			DIV_Disp_ByString(MainScreen," OFF");         
			DIV_Disp_ByString(SecondScreen," Sub");
		}break;
		case UnIt:{
			DIV_Disp_ByString(MainScreen,"UnIt");         
			DIV_Disp_ByString(SecondScreen," Sub");	
		}break;
		case CuSt:{
			DIV_Disp_ByString(MainScreen,"CuSt");         
			DIV_Disp_ByString(SecondScreen," Sub");	
		}break;
		case No:{
			DIV_Disp_ByString(MainScreen,"No  ");
      DIV_Disp_Symbol(NIXIE3,Symbol_x);
			DIV_Disp_Symbol(NIXIE4,Symbol_x);			
			DIV_Disp_ByString(SecondScreen," Sub");
			DIV_Disp_Symbol(NIXIE2,Symbol_point);
		}break;
    case Std:{
			DIV_Disp_ByString(MainScreen," Std");         
			DIV_Disp_ByString(SecondScreen," Sub");
		}break;
	}

}	


void dispeedmode_click(MenuItem* item)
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
		if(dispeed_status>dispeed_250ms)
		{
			dispeed_status--;
		}
		else
		{
			dispeed_status=dispeed_1000ms;
		}	
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		if(dispeed_status<dispeed_1000ms)
		{
			dispeed_status++;
		}
		else
		{
			dispeed_status=dispeed_250ms;
		}	
	}
	switch(dispeed_status)
	{
		case dispeed_250ms:{
			DIV_Disp_ByString(MainScreen," 250");         
			DIV_Disp_ByString(SecondScreen,"D-SP");
		}break;
		case dispeed_500ms:{
			DIV_Disp_ByString(MainScreen," 500");         
			DIV_Disp_ByString(SecondScreen,"D-SP");	
		}break;
		case dispeed_1000ms:{
			DIV_Disp_ByString(MainScreen,"1000");         
			DIV_Disp_ByString(SecondScreen,"D-SP");	
		}break;
	}
}

void diflevelmode_click(MenuItem* item)
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
		if(diflevel_status>diflevel_level1)
		{
			diflevel_status--;
		}
		else
		{
			diflevel_status=diflevel_level8;
		}	
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		if(diflevel_status<diflevel_level8)
		{
			diflevel_status++;
		}
		else
		{
			diflevel_status=diflevel_level1;
		}	
	}
	switch(diflevel_status)
	{
		case diflevel_level1:{
			DIV_Disp_ByString(MainScreen,"   1");         
		}break;
		case diflevel_level2:{
			DIV_Disp_ByString(MainScreen,"   2");         	
		}break;
		case diflevel_level3:{
			DIV_Disp_ByString(MainScreen,"   3");         
		}break;
		case diflevel_level4:{
			DIV_Disp_ByString(MainScreen,"   4");         
		}break;
		case diflevel_level5:{
			DIV_Disp_ByString(MainScreen,"   5");         	
		}break;
		case diflevel_level6:{
			DIV_Disp_ByString(MainScreen,"   6");         
		}break;
		case diflevel_level7:{
			DIV_Disp_ByString(MainScreen,"   7");         
		}break;
		case diflevel_level8:{
			DIV_Disp_ByString(MainScreen,"   8");         	
		}break;
	}
	  DIV_Disp_ByString(SecondScreen," HYS");
}

void coloractmode_click(MenuItem* item)
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
		if(coloract_status>colorreaction_out1)
		{
			coloract_status--;
		}
		else
		{
			coloract_status=colorreaction_out2;
		}	
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		if(coloract_status<colorreaction_out2)
		{
			coloract_status++;
		}
		else
		{
			coloract_status=colorreaction_out1;
		}	
	}
	switch(coloract_status)
	{
		case colorreaction_out1:{
			DIV_Disp_ByString(MainScreen,"Out1");         
			DIV_Disp_ByString(SecondScreen,"DiSP");
		}break;
		case colorreaction_out2:{
			DIV_Disp_ByString(MainScreen,"Out2");         
			DIV_Disp_ByString(SecondScreen,"DiSP");	
		}break;
	}
}

void eocmode_click(MenuItem* item)
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
		if(eco_status>eco_off)
		{
			eco_status--;
		}
		else
		{
			eco_status=eco_FULL;
		}	
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		if(eco_status<eco_FULL)
		{
			eco_status++;
		}
		else
		{
			eco_status=eco_off;
		}	
	}
	switch(eco_status)
	{
		case eco_off:{
			DIV_Disp_ByString(MainScreen," OFF");         
			DIV_Disp_ByString(SecondScreen," ECO");
		}break;
		case eco_std:{
			DIV_Disp_ByString(MainScreen," StD");         
			DIV_Disp_ByString(SecondScreen," ECO");	
		}break;
		case eco_FULL:{
			DIV_Disp_ByString(MainScreen,"FULL");         
			DIV_Disp_ByString(SecondScreen," ECO");	
		}break;
	}
}

void copymode_click(MenuItem* item)
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
		if(copy_status>copy_off)
		{
			copy_status--;
		}
		else
		{
			copy_status=copy_redy;
		}	
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		if(copy_status<copy_redy)
		{
			copy_status++;
		}
		else
		{
			copy_status=copy_off;
		}	
	}
	switch(copy_status)
	{
		case copy_off:{
			DIV_Disp_ByString(MainScreen," OFF");         
			DIV_Disp_ByString(SecondScreen,"COPY");
		}break;
		case copy_on:{
			DIV_Disp_ByString(MainScreen,"  ON");         
			DIV_Disp_ByString(SecondScreen,"COPY");	
		}break;
		case copy_on_l:{
			DIV_Disp_ByString(MainScreen,"ON-L");         
			DIV_Disp_ByString(SecondScreen,"COPY");	
		}break;
		case copy_redy:{
			DIV_Disp_ByString(MainScreen,"REDY");         
			DIV_Disp_ByString(SecondScreen,"COPY");	
		}break;
	}
}

void facrecovermode_click(MenuItem* item)
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
		if(facrecover_status>facrecoff)
		{
			facrecover_status--;
		}
		else
		{
			facrecover_status=facrecon;
		}	
	}
	else if (item->whichcallback == MENU_CBK_SUB)
	{
		if(facrecover_status<facrecon)
		{
			facrecover_status++;
		}
		else
		{
			facrecover_status=facrecoff;
		}	
	}
	switch(facrecover_status)
	{
		case facrecoff:{
			DIV_Disp_ByString(MainScreen," OFF");         
			DIV_Disp_ByString(SecondScreen,"RESt");
		}break;
		case facrecon:{
			DIV_Disp_ByString(MainScreen,"  ON");         
			DIV_Disp_ByString(SecondScreen,"RESt");	
		}break;
	}
}
#endif
