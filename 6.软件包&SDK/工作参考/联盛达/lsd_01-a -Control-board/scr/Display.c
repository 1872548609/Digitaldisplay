#include "Display.h"

const u8 code Dis_Table[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

typedef enum
{
   Brightness_level_1      =  0X88,  // 亮度1/16
   Brightness_level_3      =  0X8A,  // 亮度2/16
   Brightness_level_4      =  0X8B,  // 亮度4/16
   Brightness_level_5      =  0X8C,  // 亮度10/16
   Brightness_level_6      =  0X8D,  // 亮度11/16
   Brightness_level_7      =  0X8E,  // 亮度13/16
   Brightness_level_8      =  0X8F,  // 亮度14/16
}  Brightness_level_t;

//显示寄存器
u8 DisRegister[12]={0};

void Delay_us(u8 us)
{
    u8 i=0;
    for(i=0;i<us;i++);
}


void AIP_Start()
{
    CLK = 1;
    DIN = 1;
    Delay_us(2);
    DIN = 0;
    Delay_us(2);
}

void AIP_Stop()
{
    CLK = 0;
    Delay_us(2);
    DIN = 0;
    Delay_us(2);
    CLK = 1;
    Delay_us(2);
    DIN = 1;
}

void SendByte(u8 Para)
{
    u8 i =0;
    for(i=0;i<8;i++)
    {
        CLK = 0; 
        Delay_us(2);
        if((Para>>i) & 0x01)
            DIN = 1;
        else
            DIN = 0;
        Delay_us(3);
        CLK = 1;
        Delay_us(3);
    }
}

//地址自加1显示
void SendData(u8 Addr,u8 *arry,u8 len)
{
    u8 i =0;
    AIP_Start();
    SendByte(Addr);
    for(i=0;i<len;i++)
        SendByte(arry[i]);
    AIP_Stop();
}
//=====================================================================//
//压力显示
void Dis_Pressure(u8 pressure,u8 tag)
{
    DisRegister[1] = 0;
    DisRegister[2] = 0;
    DisRegister[2] = Dis_Table[pressure%10];
    if(pressure>9)//压力值不超过10
        DisRegister[1] = Dis_Table[(pressure/10)%10];
    if(tag)
        DisRegister[1] |= 0x80;	
}

//温度显示
void Dis_Temperature(u16 temperature,u8 tag)
{
    if(tag==FahrenheitDis)//单位转换
        temperature = (temperature*9/5)+32; //华氏度转换为摄氏度
    DisRegister[10] &= 0x80; 
    DisRegister[9] = 0;
    DisRegister[8] = 0; //低位
	
    DisRegister[8] = Dis_Table[temperature%10];
    if(temperature>9)
        DisRegister[9] = Dis_Table[(temperature/10)%10];
    else
        DisRegister[9] = 0;

    if(temperature>99)
        DisRegister[10] = Dis_Table[(temperature/100)%10];
    else
        DisRegister[10] = 0;

    if(tag == CentigradeDis) //显示摄氏度
        DisRegister[9] |= 0x80;
    else if(tag == FahrenheitDis) //显示华氏度
        DisRegister[8] |= 0x80;
}

#define STR_E    0x79 
#define STR_C    0x39 
#define STR_L    0x38 
#define STR_N    0x54 
#define STR_WAIT 0x40

//代码显示
void CodeDis(u8 modle,u8 errCode)
{
    DisRegister[10] &= 0x80; 
    DisRegister[9] = 0;
    DisRegister[8] = 0; //低位
    switch (modle)
    {
    case ERR:
        DisRegister[10] = STR_E;
        DisRegister[9]  = STR_WAIT;
        DisRegister[8]  = Dis_Table[errCode%10];
    break;
    case CLN:
        DisRegister[10] = STR_C;
        DisRegister[9]  = STR_L;
        DisRegister[8]  = STR_N;
    break;
    }
}


//不同功能显示不同按键，当前功能灯
void Dis_Function(u8 function)
{
     DisRegister[10] &= 0x7F;
     DisRegister[7] = 0;
     DisRegister[6] = 0;
     DisRegister[5] = 0;
	 DisRegister[4] = 0;
     DisRegister[3] &= 0x1F;

	if(function & SingleEspersso)//显示单杯
     {
         DisRegister[7] |= 0x07;
         DisRegister[6] |= 0x07;
     }
     if(function & DoubleEspersso)//显示双杯
     {
         DisRegister[7] |= 0x38;
         DisRegister[6] |= 0x38;
     }
     if(function & Coffe)//显示咖啡
     {
         DisRegister[7] |= 0xC0;
         DisRegister[6] |= 0xC0;
         DisRegister[10]|= 0x80;
         DisRegister[3] |= 0x20;
     }
     if(function & Americano)//显示美式
     {
         DisRegister[5] |= 0x07;
         DisRegister[4] |= 0x07;
     }
     if(function & ColdBrew)//显示冷翠
     {
         DisRegister[5] |= 0x38;
         DisRegister[4] |= 0x38;

     }
     if(function & EsperssoCool)//冷密浓缩
     {
         DisRegister[5] |= 0xC0;
         DisRegister[4] |= 0xC0;
         DisRegister[3] |= 0xC0;
     }
}


//错误灯
void Dis_Error(u8 error)
{
    DisRegister[3] &= 0xE0;
    if(error & Descaling)
        DisRegister[3] |= 0x01;
    if(error & ErrUniversal)
        DisRegister[3] |= 0x02;
    if(error & ErrTankLack)
        DisRegister[3] |= 0x04;
    if(error & ErrBeanLack)
        DisRegister[3] |= 0x08;
    if(error & ErrBeanWareHouse)
        DisRegister[3] |= 0x10;
}



void Dis_PilotLamp(u8 lamp)//灯显示
{
    DisRegister[11] = 0;
    if(lamp & Led_Power)
        DisRegister[11] |= 0x04;//电源灯
    if(lamp & Led_Grind)
        DisRegister[11] |= 0x02;//磨豆
    if(lamp & Led_Temp)
        DisRegister[11] |= 0x01;//温度
    if(lamp & Led_Clean)
        DisRegister[11] |= 0x08;//清洁
    if(lamp & Led_Steam)
        DisRegister[11] |= 0x10;//蒸汽
    if(lamp & Led_HotWater)
        DisRegister[11] |= 0x20;//热水
}

void AllDown()
{
    u8 i =0;
    for(i=0;i<12;i++)
        DisRegister[i] = 0;
}

void AllUp()
{
    u8 i =0;
    for(i=0;i<12;i++)
        DisRegister[i] = 0xFF;
}
//主循环更新显示
void DisRefresh()
{
    AIP_Start();
    SendByte(0x40);
    AIP_Stop();
    SendData(0xC0,DisRegister,12);
    AIP_Start();
    SendByte(Brightness_level_8);
    AIP_Stop();
}
