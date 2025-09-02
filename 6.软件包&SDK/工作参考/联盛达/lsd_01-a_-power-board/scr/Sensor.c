#include "Sensor.h"

AdParaTypedef AdPara ={0};

void CheckSensor(void)
{
    static u8 delayCount =0;
	delayCount ++;//10ms自增
    switch (delayCount)
    {
    case 1:
        AdPara.Temp1=Temper_Conver(d_ADC_CH0_IN);//温度1
        break;
    case 2:
        AdPara.Temp2=Temper_Conver(d_ADC_CH1_IN);//温度2
        break;
    case 3:
        AdPara.Press=Press_Conver(d_ADC_CH4_IN); //压力
        break;
	case 4:
		ADMotorCheck();
		break;
    default:
        delayCount =0;
        break;
    }
}