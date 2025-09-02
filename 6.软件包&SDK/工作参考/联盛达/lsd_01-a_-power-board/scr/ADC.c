#include "ADC.h"

unsigned int n_data    = 0x0000;
bit          ADCfinish = 0;

const unsigned int code TemTable[ADC_TABLE_SIZE] = {
    // 4055, 4052, 4050, 4047, 4044, 4041, 4038, 4035, 4031, 4028, 		// -20~-11
    // 4024, 4020, 4016, 4012, 4007, 4002, 3997, 3992, 3987, 3981, 		// -10~-1
    3975, 3969, 3963, 3956, 3949, 3941, 3934, 3926, 3918, 3909, 		// 0~9
    3900, 3891, 3881, 3871, 3861, 3850, 3839, 3827, 3815, 3803, 		// 10~19
    3790, 3777, 3763, 3749, 3734, 3719, 3703, 3687, 3671, 3654, 		// 20~29
    3636, 3618, 3600, 3581, 3562, 3542, 3521, 3500, 3479, 3457, 		// 30~39
    3434, 3411, 3387, 3363, 3339, 3314, 3288, 3262, 3236, 3209, 		// 40~49
    3182, 3154, 3125, 3097, 3068, 3038, 3008, 2978, 2947, 2917, 		// 50~59
    2885, 2854, 2822, 2790, 2757, 2725, 2692, 2659, 2626, 2592, 		// 60~69
    2559, 2525, 2491, 2457, 2424, 2390, 2356, 2322, 2288, 2254, 		// 70~79
    2220, 2186, 2153, 2119, 2086, 2052, 2019, 1986, 1954, 1921, 		// 80~89
    1889, 1857, 1825, 1793, 1762, 1731, 1700, 1670, 1640, 1610, 		// 90~99
    1580, 1551, 1522, 1494, 1466, 1438, 1411, 1384, 1358, 1331, 		// 100~109
    1306, 1280, 1255, 1231, 1206, 1183, 1159, 1136, 1113, 1091, 		// 110~119
    1069, 1048, 1026, 1006, 985, 965, 946, 926, 907, 889, 				// 120~129
    871, 853, 835, 818, 801, 785, 769, 753, 738, 722, 					// 130~139
    707, 693, 679, 665, 651, 638, 625, 612, 599, 587, 					// 140~149
    575, 563, 552, 540, 529, 518, 508, 498, 487, 478, 					// 150~159
    468, 458, 449, 440, 431, 423, 414, 406, 398, 390, 					// 160~169
    382, 374, 367, 360, 353, 346, 339, 332, 326, 319, 					// 170~179
    313, 307, 301, 295, 290, 284, 279, 273, 268, 263, 					// 180~189
    258, 253, 248, 244, 239, 235, 230, 226, 222, 218, 					// 190~199
    214, 
    // 210, 206, 202, 198, 195, 191, 188, 184, 181, 					// 200~209
//     178, 175, 171, 168, 165, 162, 160, 157, 154, 151, 					// 210~219
//     149, 146, 144, 141, 139, 136, 134, 132, 129, 127, 					// 220~229
//     125, 123, 121, 119, 117, 115, 113, 111, 109, 108, 					// 230~239
//     106, 104, 102, 101, 99, 98, 96, 94, 93, 91, 90 						// 240~250
};


const unsigned int code PresureTable[PRESSURE_TABLE_SIZE] = {
    410,//0 bar
    541,//1 bar
    672,//2 bar
    803,//3 bar
    934,//4 bar
    1065,//5 bar
    1196,//6 bar
    1327,//7 bar
    1458,//8 bar
    1589,//9 bar
    1720,//10 bar
    1851,//11 bar
    1982,//12 bar
    2113,//13 bar
    2244,//14 bar    
    2376,//15 bar
    2507,//16 bar
    2638,//17 bar
    2769,//18 bar
    2900,//19 bar
    3031,//20 bar
    3162,//21 bar
    3293,//22 bar
    3424,//23 bar
    3555,//24 bar
    3686,//25 bar
};

void ADC_Init(void)
{
    ADCC1 = d_ADCnEN0;                 //Set ADC channel 
    ADCCS = d_ADCnEN1 | d_ADC_CLK_Sel; //Set ADC channel & Select ADC clock
    IEADC = 1;                         //Enable ADC interrupt.
    // EA    = 1;
}

unsigned int ADC_Read12() //12-bit ADC
{
    unsigned int n_ADCD12 = 0x0000;
    n_ADCD12 = (ADCDH<<4) | ADCDL;     //12-bit ADC
    return n_ADCD12;
}
    
void ADCInterrupt(void) interrupt d_ADC_Vector // ADC Interrupt Vector Address 0053h => interrupt 10
{
    n_data = ADC_Read12(); //Read ADC
    ADCfinish = 1;
}

u16 ADC_Start(u8 channel)
{
    ADCfinish = 0;
    ADCC2  = channel; //Set a new channel
    ADCC2 |= 0x80;      //ADC start conversion
    while(!ADCfinish);
    return n_data;
}

// AD中值滤波
#define N 5
u16 Value_Filter(u8 channel)
{
    u16 arr[N]=0;
    u16 temp;
    u8 i,j;
    for(i = 0;i < N ;i++)
    {
        arr[i] = ADC_Start(channel);
    }
	for (i = 0; i < N-1; i++)
			for (j = 0; j < N-1-i; j++)
					if (arr[j] > arr[j + 1]) {
							temp = arr[j];
							arr[j] = arr[j + 1];
							arr[j + 1] = temp;
					}
    return arr[(N-1)/2];
}

//温度AD值2分法查表
u8 Temper_Conver(u8 channel )
{
    u16 AdValue =0;
    u8 Tail ,Front,middle;
    Tail =0;
    Front = ADC_TABLE_SIZE-1;
    AdValue = Value_Filter(channel);
    if(AdValue <TemTable[Tail]&& AdValue > TemTable[Front])
    {
        middle = (Front - Tail)/2;
        while((Front - Tail)>1)
        {
            if(AdValue < TemTable[middle])
            {
                Tail = middle;
            //    middle +=(Front - Tail)/2;
                middle = (Front + Tail)/2;
            }
            else if(AdValue > TemTable[middle])
            {
                Front = middle;
            //    middle -=(Front - Tail)/2;
                middle = (Front + Tail)/2;
            }
            else
            return middle;
        }
        if(abs(AdValue-TemTable[Tail])<abs(AdValue-TemTable[Front]))
        return Tail;else return Front;
    }
    else if(AdValue >=LOWTEMP_CRITICAL)
    return BROKENERR;    //开路
    else if(AdValue >=TemTable[0])
    return 0;          //低温
    else
    return SHORTERR;    //短路
}



u8 Press_Conver(u8 channel)
{
    u16 AdValue =0;
    u8 Tail ,Front,middle;
    Tail =0;
    Front = PRESSURE_TABLE_SIZE-1;
    AdValue = Value_Filter(channel);
    if(AdValue >PresureTable[Tail]&& AdValue < PresureTable[Front])
    {
        middle = (Front - Tail)/2;
        while((Front - Tail)>1)
        {
            if(AdValue > PresureTable[middle])
            {
                Tail = middle;
            //    middle +=(Front - Tail)/2;
                middle = (Front + Tail)/2;
            }
            else if(AdValue < PresureTable[middle])
            {
                Front = middle;
            //    middle -=(Front - Tail)/2;
                middle = (Front + Tail)/2;
            }
            else
            return middle;
        }
        if(abs(AdValue-PresureTable[Tail])<abs(AdValue-PresureTable[Front]))
        return Tail;else return Front;
    }
    else if(AdValue >=3800)
    return BROKENERR;    //开路
    else if(AdValue >=PresureTable[25])
    return 25;          //超压
    else
    return SHORTERR;    //短路   
}

#define NoWork      0x01    //无工作
#define NoLoad      0x02    //空转
#define Grinding    0x04    //磨豆
#define Stall       0x08    //堵转

void ADMotorCheck(void)
{
    static u8 tempValue =0;
    static u8 DelayCnt =0;
    u8 i =0;
    u16 AdValue =0;
    for(i = 0;i < 10;i++)
        AdValue += ADC_Start(d_ADC_CH2_IN);
    AdValue /= 10;
    if(AdValue >= 0 && AdValue < 20)
        tempValue = NoWork;
    else if ((AdValue >= 20) && (AdValue < 200))
        tempValue = NoLoad;
    else if((AdValue >= 200) && (AdValue < 1000)) 
        tempValue = Grinding;
	else
		tempValue = Stall;
    if(tempValue!= AdPara.AdMotor)
    {
        DelayCnt++;
        switch (tempValue)
        {
            case NoWork:
            if(DelayCnt >= 40)
                AdPara.AdMotor = tempValue;
            break;
            case NoLoad:
            if(DelayCnt >= 3)
                AdPara.AdMotor = tempValue;
            break;
            case Grinding:
            if(DelayCnt >= 3)
                AdPara.AdMotor = tempValue;
            break;
            case Stall: 
            if(DelayCnt >= 2)
                AdPara.AdMotor = tempValue;
            break;
        default:
            break;
        }
    }
    else
        DelayCnt = 0;
}

