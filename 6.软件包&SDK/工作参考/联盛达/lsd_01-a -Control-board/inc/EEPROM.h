#ifndef __EEPROM_H__
#define __EEPROM_H__

// Example Code
/*
#include "EEPROM.h"
#include "OB38R16T1.h"

void main()
{
    Write_EEPROM(0x05,0x33);
    if(Read_EEPROM(0x05)==0x33)
    {
        ROM_2ByteProgram(0x1004,0x3322);
        if(ROM_ByteRead(0x1004)==0x22)
        {
            ROM_1ByteProgram(0x1014,0x22);
        }
    }
    while(1);
}
*/
#include "main.h"

#define AddrTotalFlowL    0 //流量累积低
#define AddrTotalFlowH    1 //流量累积高
#define AddrSingleEspTemp 2 //单杯意式温度  
#define AddrDoubleEspTemp 3 //双杯意式温度
#define AddrCofffeTemp    4 //咖啡温度
#define AddrAmericanoTemp 5 //美式温度

#define DefSingleEspTemp 93 
#define DefDoubleEspTemp 93
#define DefCoffeTemp     93
#define DefAmericanoTemp 93

#define MaxHotDrinkTemp  98 
#define MinHotDrinkTemp  80 


void ISP_Enable(void);
void ISP_Disable(void);
// void ROM_1ByteProgram(unsigned int Addr, unsigned char Data);
// void ROM_2ByteProgram(unsigned int Addr, unsigned int Data);
void Write_EEPROM(unsigned char Addr, unsigned char Data);
unsigned char Read_EEPROM(unsigned char Addr);

// unsigned char ROM_ByteRead(unsigned int Addr);

void ReadTotalFlow();       //读取总流量
void WriteTotalFlow();      //写入总流量
void ReadSingleEspTemp();   //读取单杯意式温度
void ReadDoubleEspTemp();   //读取双杯意式温度
void ReadCoffeTemp();       //读取咖啡温度
void ReadAmericanoTemp();   //读取美式咖啡温度


#endif
