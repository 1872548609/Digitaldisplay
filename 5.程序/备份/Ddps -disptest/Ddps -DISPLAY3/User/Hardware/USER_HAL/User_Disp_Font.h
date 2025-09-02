#ifndef _USER_DISP_FONT_H
#define _USER_DISP_FONT_H

#include "superhead.h"

//fha = D K E F  \j GN G H \	C B I A  \
								
//ahf  	C B I A  \j GN G H \	D K E F  \

//s1    P1 P2 P3 S1
//s2    P8 P7 P6 S2
								
uint8_t num4[26] = {
0x3,0x6,0xd			,   //7     fha
0x3,0x6,0xd			,	  //6     fha
0x3,0x6,0xd			,   //5     fha
0xd,0x6,0x3			,   //4     ahf
0xd,0x6,0x3			,   //3     ahf
0xd,0x6,0x3			,   //2     ahf
0xd,0x6,0x3			,			//1     ahf
0xe,              //s1
0xf,										    //s2
0x3,0x6,0xd   				//8     fha
};


#endif