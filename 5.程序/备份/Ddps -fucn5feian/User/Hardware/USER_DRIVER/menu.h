#ifndef __MENU_H
#define __MENU_H

#include "superhead.h"

//状态枚举===================================================================
 //一级菜单================
typedef enum{
			status_init,
			status_run,
			status_normset,
			status_proset,
}DeviceStatus;
 

//二级菜单================
typedef enum{
	noset_status =0,
	compareset1_status,
	compareset2_status,
	noncset_status,
	reactiontimeset_status,
	maincolorset_status,
	unitconversion_status,
}NormsetStatus;


typedef  enum{
	pnoset_status=0,
 secscreendisset_status,
	disspeedset_status,
	difleveset_status,
	coloractset_status,
	ecoset_status,
	factorydatereset_status
}PromodsetStatus;



//三级菜单==================
//run
typedef enum{
	 runstate_run=0,
		runstate_set,
}RunState;


//norset
typedef enum{ 
			compare1_EASY = 1,
			compare1_WCMP = 2,
			compare1_HSY, 
}CompareStatus1;

typedef enum{
   compare2_off =0,
			compare2_EASY ,
			compare2_WCMP ,
			compare2_HSY 
}CompareStatus2;



typedef enum{
   noncstatus_1o2o = 0,
			noncstatus_1o2c,
			noncstatus_1c2c,
			noncstatus_1c2o,

}NONCStatus;
	

typedef enum{
     reaction_2ms = 2,
					reaction_10ms = 10,
					reaction_50ms = 50,
					reaction_100ms = 100,
					reaction_250ms = 250,
					reaction_500ms = 500,
					reaction_2000ms = 2000,

}ReactionTime;


typedef enum{
   R_ON = 0,
			G_ON,
			AL_RED,
			AL_GREEN,

}MainScreenColor;


typedef enum{
 Unit_Mpr=0,
	Unit_Kpr,
	Unit_Kgf,
	Unit_Bar,
	Unit_Psi,
 Unit_Mmhg,
	Unit_Inhg,
}Unitconversion;


//proet

typedef enum{
 secscreen_off=0,
	secscreen_disunit,
	secscreen_disoutchoice,
 secscreen_discustom,
}SecScreen;


typedef enum{
	dispeed_500ms=0,
	dispeed_1000ms,
}DisSpeed;


typedef enum{
diflevel_level1=0,
diflevel_level2,
diflevel_level3,
}DifLevel;
	

typedef enum{
 colorreaction_out1=0,
	colorreaction_out2,
}ColorReaction;


typedef enum{
 eco_std=0,
	eco_FULL,

}Ecomode;


typedef enum{
	facrecoff=0,
	facrecon,
	
}FactoryRecover;

#endif
