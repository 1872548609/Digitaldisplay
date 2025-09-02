#include "OB38R16T1.h"
#include "LVI_LVR.h"

void LVI_EN(void)
{
    LVC   |= 0x83; //4V 掉电存储
    IELVI  = 1;
    // EA     = 1;
}

void LVR_Disable(void)
{
    LVC &= 0xDF;
}

void LVI(void) interrupt d_LVI_Vector
{
    Write_EEPROM(AddrSingleEspTemp,TempSingleEsp);
    Write_EEPROM(AddrDoubleEspTemp,TempDoubleEsp);
    Write_EEPROM(AddrCofffeTemp,TempCoffee);
    Write_EEPROM(AddrAmericanoTemp,TempAmericano);
    Write_EEPROM(AddrTotalFlowL,TotalFlow);
    Write_EEPROM(AddrTotalFlowH,TotalFlow>>8);
}
