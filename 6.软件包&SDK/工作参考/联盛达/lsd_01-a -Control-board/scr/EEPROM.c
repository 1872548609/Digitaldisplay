#include "OB38R16T1.h"
#include <absacc.h>    /* Include Macro Definitions */
#include "EEPROM.h"

/*ISPFC.ISPF[2:0]*/
#define d_1Byte_Program_ROM     0
#define d_2Byte_Program_ROM     1
#define d_Write_EEPROM          3
#define d_Read_EEPROM           4

//====================================================================


void ISP_Enable(void)
{
    EA     = 0;
    TAKEY  = 0x55;
    TAKEY  = 0xAA;
    TAKEY  = 0x5A;
    IFCON |= 0x01;      // ISPE=1, Enable ISP function
    EA     = 1;
}

void ISP_Disable(void)
{
    EA     = 0;
    TAKEY  = 0x55;
    TAKEY  = 0xAA;
    TAKEY  = 0x5A;
    IFCON &= 0xFE;      // ISPE=0, Disable  ISP function
    EA     = 1;
}

// void ROM_1ByteProgram(unsigned int Addr, unsigned char Data)
// {
//     ISP_Enable();
//     ISPFAH = Addr>>8;
//     ISPFAL = Addr;
//     ISPFDL = Data;
//     ISPFC  = d_1Byte_Program_ROM;
//     ISP_Disable();
// }

// void ROM_2ByteProgram(unsigned int Addr, unsigned int Data)
// {
//     ISP_Enable();
//     ISPFAH = Addr>>8;
//     ISPFAL = Addr;
//     ISPFDL = Data;
//     ISPFDH = Data>>8;
//     ISPFC  = d_2Byte_Program_ROM;
//     ISP_Disable();
// }

void Write_EEPROM(unsigned char Addr, unsigned char Data)
{
    ISP_Enable();
    ISPFAH = 0x00;
    ISPFAL = Addr;
    ISPFDL = Data;
    ISPFC  = d_Write_EEPROM;
    ISP_Disable();
}

unsigned char Read_EEPROM(unsigned char Addr)
{
    unsigned char Data;
    ISP_Enable();
    ISPFAH = 0x00;
    ISPFAL = Addr;
    ISPFC  = d_Read_EEPROM;
    Data   = ISPFDL;
    ISP_Disable();
    return Data;
}

//读取总流量
void ReadTotalFlow()
{
   u8 temp =0;
   TotalFlow = 0;
   temp = Read_EEPROM(AddrTotalFlowL);
   TotalFlow = temp;
   temp = Read_EEPROM(AddrTotalFlowH);
   TotalFlow |=  (temp<<8); 
}

void WriteTotalFlow()
{
   u8 temp =0;
   temp = TotalFlow & 0x00FF;
   Write_EEPROM(AddrTotalFlowL,temp);
   temp = (TotalFlow & 0xFF00) >> 8;
   Write_EEPROM(AddrTotalFlowH,temp);
}

//读取单杯意式温度
void ReadSingleEspTemp()
{
   TempSingleEsp= Read_EEPROM(AddrSingleEspTemp);
   if((TempSingleEsp> MaxHotDrinkTemp)||(TempSingleEsp< MinHotDrinkTemp))
       TempSingleEsp= DefSingleEspTemp;
}

//读取双杯意式温度
void ReadDoubleEspTemp()
{
   TempDoubleEsp= Read_EEPROM(AddrDoubleEspTemp);
   if((TempDoubleEsp> MaxHotDrinkTemp)||(TempDoubleEsp< MinHotDrinkTemp))
       TempDoubleEsp= DefDoubleEspTemp;
}

//读取咖啡温度
void ReadCoffeTemp()
{
    TempCoffee= Read_EEPROM(AddrCofffeTemp);
    if((TempCoffee> MaxCoffeTemp)||(TempCoffee< MinCoffeTemp))
        TempCoffee= DefCoffeTemp;
}

//读取美式咖啡温度
void ReadAmericanoTemp()
{
    TempAmericano= Read_EEPROM(AddrAmericanoTemp);
    if((TempAmericano> MaxHotDrinkTemp)||(TempAmericano< MinHotDrinkTemp))
        TempAmericano= DefAmericanoTemp;
}


// unsigned char ROM_ByteRead(unsigned int Addr)
// {
//     return CBYTE[Addr];         //The range of valid index values for this macro is 0-255
// }

