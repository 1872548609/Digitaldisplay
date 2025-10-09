#ifndef __MENU_H
#define __MENU_H

#include "superhead.h"

//状态枚举===================================================================


//一级菜单================
typedef enum{
			status_init= 1,
			status_run,
			status_normset,
			status_proset,
}DeviceStatus;
 

//二级菜单================
typedef enum{
	noset_status =1,
	compareset1_status,
	compareset2_status,
	noncset_status,
	reactiontimeset_status,
	maincolorset_status,
	unitconversion_status,
}NormsetStatus;


typedef  enum{
	pnoset_status=1,
 secscreendisset_status,
	disspeedset_status,
	difleveset_status,
	coloractset_status,
	ecoset_status,
	factorydatereset_status
}PromodsetStatus;


typedef enum {
    Rstate_Run_mode=0,
    Rstate_Peakvalley_mode

}RunMode;

//三级菜单==================
//run
typedef enum{
	 runstate_run=1,
		runstate_set,
}RunState;


//norset
typedef enum{ 
			compare1_EASY 	=0x01,
			compare1_WCMP 	=0x02,
			compare1_HSY			=0x04, 
}CompareStatus1;

typedef enum{
   compare2_off 	=0x08,
			compare2_EASY =0x10,
			compare2_WCMP =0x20,
			compare2_HSY 	=0x40,
}CompareStatus2;



typedef enum{
   noncstatus_1o2o = 0x80,
			noncstatus_1o2c = 0x100,
			noncstatus_1c2c =0x200,
			noncstatus_1c2o =0x400,

}NONCStatus;
	

typedef enum{
     reaction_2ms 			=0x800,
					reaction_10ms 		=0x1000 ,
					reaction_50ms 		=0x2000,
					reaction_100ms 	=0x4000,
					reaction_250ms 	=0x8000,
					reaction_500ms 	=0x10000,
					reaction_2000ms =0x20000 ,

}ReactionTime;


typedef enum{
   R_ON 				= 0x40000,
			G_ON 				= 0x80000,
			AL_RED 		= 0x100000,
			AL_GREEN = 0x200000,

}MainScreenColor;


typedef enum{
 Unit_Mpr	=0x400000,
	Unit_Kpr	=0x800000,
	Unit_Kgf	=0x1000000,
	Unit_Bar	=0x2000000,
	Unit_Psi	=0x4000000,
 Unit_Mmhg=0x8000000,
	Unit_Inhg=0x10000000,
}Unitconversion;


//proet

typedef enum{
 secscreen_off=0x01,
	secscreen_disunit=0x02,
	secscreen_disoutchoice=0x04, 
 secscreen_discustom=0x08,
}SecScreen;


typedef enum{
	dispeed_250ms=0x10,
	dispeed_500ms=0x20,
	dispeed_1000ms=0x40,
}DisSpeed;           


typedef enum{
diflevel_level1=0x80,
diflevel_level2=0x100,
diflevel_level3=0x200,
diflevel_level4=0x400,
diflevel_level5=0x800,
diflevel_level6=0x1000,
diflevel_level7=0x2000,
diflevel_level8=0x4000
}DifLevel;
	

typedef enum{
 colorreaction_out1=0x8000,
	colorreaction_out2=0x10000,
}ColorReaction;


typedef enum{
 eco_std=0x20000,
	eco_FULL=0x30000,

}Ecomode;


typedef enum{
	facrecoff=0x40000,
	facrecon=0x80000,
	
}FactoryRecover;

#endif
