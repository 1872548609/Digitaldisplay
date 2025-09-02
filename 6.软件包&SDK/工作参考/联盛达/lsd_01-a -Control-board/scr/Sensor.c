#include "Sensor.h"

#define TempMaxLimit    180 //温度最大限制    
#define PressureErr     0xFE//压力传感器错误码
#define ErrDelay        150 //延时1.5s

void CheckSensor(void)
{
   static u8 DelayCount  =0;
//    static u8 DelayCount1 =0;
	static u8 DelayCount2 =0;
	DelayCount2++;
	if(DelayCount2>=50)
	{
		DelayCount2=0;
		FlashUartByte(CMD_ADSensor);
	}
   if((DeviceStatus > status_Down)&&(DeviceStatus < status_Err))
   {
       if(AdPara.CurCoffeTemp>= TempMaxLimit)
       {
           DelayCount++;
           if(DelayCount>=ErrDelay)
           {
                DelayCount=0;
                if(AdPara.CurCoffeTemp== SHORTERR)
                ErrCode |= ErrPotNtcShort|ErrUniversal;
                else if(AdPara.CurCoffeTemp== BROKENERR)
                ErrCode |= ErrPotNtcOpen|ErrUniversal;
                else
                ErrCode |= ErrPotNtcHot|ErrUniversal;
                DeviceStatus = status_Err;
           }
       }
       else
           DelayCount =0;

        // if(AdPara.Press>= PressureErr)
        // {
        //     DelayCount1++;
        //     if(DelayCount1>=ErrDelay)
        //     {
        //         DelayCount1=0;
        //         ErrCode |= ErrPressure;
        //         DeviceStatus = status_Err;
        //     }
        // }
        // else
        //     DelayCount1 =0;
   }
}




