#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "main.h"

#define CentigradeDis 1 //摄氏度显示
#define FahrenheitDis 2 //华氏度显示
#define NoUnitDis     3 //无单位显示


#define SingleEspersso  0x01
#define DoubleEspersso  0x02
#define Coffe           0x04
#define Americano       0x08
#define ColdBrew        0x10
#define EsperssoCool    0x20

#define Descaling           0x01
#define ErrUniversal        0x02
#define ErrTankLack         0x04
#define ErrBeanLack         0x08
#define ErrBeanWareHouse    0x10
#define ErrPotNtcShort      0x20
#define ErrPotNtcOpen       0x40
#define ErrPotNtcHot        0x80

#define Led_Power       0x01
#define Led_Grind       0x02
#define Led_Temp        0x04
#define Led_Clean       0x08
#define Led_Steam       0x10
#define Led_HotWater    0x20

#define ERR      0x01
#define CLN      0x02

void Dis_Pressure(u8 pressure,u8 tag);
void Dis_Temperature(u16 temperature,u8 tag);
void Dis_Function(u8 function);
void Dis_Error(u8 error);
void CodeDis(u8 modle,u8 errCode);
void Dis_PilotLamp(u8 lamp);
void AllDown();
void AllUp();
void DisRefresh();
void Delay_us(u8 us);

#endif /* DISPLAY_H_ */
