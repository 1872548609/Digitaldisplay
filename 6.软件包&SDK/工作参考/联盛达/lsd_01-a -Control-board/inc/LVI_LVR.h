#ifndef __LVI_H__
#define __LVI_H__

// Example Code
/*
#include "LVI_LVR.h"
#include "OB38R16T1.h"

void main(void)
{
    if (d_LVI_EN == 1)
        LVI_EN();
        
    if (d_LVR_EN == 0)
        LVR_Disable();

    while(1);
}
*/
#include "main.h"

#define d_LVI_EN    0x01
#define d_LVR_EN    0x00

void LVI_EN(void);
void LVR_Disable(void);
 
 #endif
