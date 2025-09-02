#include "Key.h"

KeyParaTypedef xdata KeyPara ={0};

#define Key_Power       0x01 
#define Key_Grind       0x02
#define Key_Temp        0x04
#define Key_Clean       0x08
#define Key_Steam       0x10
#define Key_Hotwater    0x20
#define Key_Extract     0x40

#define KeyDelay_20MS   2 //按键延时20MS 
#define KeyDelay_3S     300 //按键延时3S 
#define KeyDelay_5S     500 //按键延时5S 

#define Key_on          0x00 
#define Key_off         0x01 

void Scan()
{
    u8 KeyValue = 0;
    if(PowerKey     == Key_on) KeyValue |= Key_Power;
    if(GrindKey     == Key_on) KeyValue |= Key_Grind;
    if(TempKey      == Key_on) KeyValue |= Key_Temp;
    if(CleanKey     == Key_on) KeyValue |= Key_Clean;
    if(SteamKey     == Key_on) KeyValue |= Key_Steam;
    if(HotWaterKey  == Key_on) KeyValue |= Key_Hotwater;
    if(ExtractKey   == Key_on) KeyValue |= Key_Extract;

    if(0 == KeyValue)
    {
        if(KeyPara.PreKey == Key_Power)KeyPara.CurKey= Key_Power;
        else if(KeyPara.PreKey == Key_Grind)KeyPara.CurKey= Key_Grind;
        else if(KeyPara.PreKey == Key_Temp)KeyPara.CurKey= Key_Temp;
        else if(KeyPara.PreKey == Key_Clean)KeyPara.CurKey= Key_Clean;
        else if(KeyPara.PreKey == Key_Steam)KeyPara.CurKey= Key_Steam;
        else if(KeyPara.PreKey == Key_Hotwater)KeyPara.CurKey= Key_Hotwater;
        else if(KeyPara.PreKey == Key_Extract)KeyPara.CurKey= Key_Extract;

        KeyPara.PreKey = 0;
        KeyPara.FilterCount = 0;
    }
    else if (KeyPara.PreKey == KeyValue)
    {
        SleepCut = 0;
        KeyPara.FilterCount++; 
        if (KeyPara.FilterCount>= KeyDelay_5S)
        {
            if(DeviceStatus == status_Idle)
            {
                if(KeyPara.PreKey == (Key_Steam|Key_Hotwater))
                {
                    KeyPara.FilterCount = 0;
                    LT.CleanTask.Logictrl.LogictrlBit.execute=1;
                    Dis_PilotLamp(Led_Power|Led_HotWater|Led_Steam);
                    CodeDis(CLN,0);
                    buzctrl.BeepState   = BeepOnce;
                    buzctrl.BeepDuration= Buz_1S;
                    DeviceStatus = status_Clean;
                }
            }
            else if (DeviceStatus == status_Clean)
            {
                if(KeyPara.PreKey == (Key_Steam|Key_Hotwater)) 
                {
                    KeyPara.FilterCount = 0;
                    LT.CleanTask.Logictrl.LogictrlBit.quit=1;   
                    DeviceStatus = status_Idle;
                }
            }
        }
        else  if(KeyPara.FilterCount>= KeyDelay_3S)
        {
            if(DeviceStatus == status_Idle)
            {
                if(KeyPara.PreKey == Key_Temp)
                {
                    KeyPara.FilterCount = 0;
                    if(curTempUnit==CentigradeDis)  
                        curTempUnit = FahrenheitDis;
                    else
                        curTempUnit = CentigradeDis;
                }
            }
            // else if (DeviceStatus == status_Work)
            // {
            //     if((KeyPara.PreKey == Key_Grind)&&(LT.GrindBeanTask.Logictrl.LogictrlBit.step==2))
            //     {
            //         KeyPara.FilterCount = 0;
            //         LT.GrindBeanTask.Logictrl.LogictrlBit.quit=1;
            //         DeviceStatus = status_Idle;
            //     }
            // }
       }
    }
    else 
    {
        KeyPara.FilterCount++;
        if(KeyPara.FilterCount>= KeyDelay_20MS)
        {
            SleepCut = 0;
            KeyPara.FilterCount = 0;
            KeyPara.PreKey = KeyValue;
        }
    }
}

void Respone()
{
   switch (KeyPara.CurKey)//当前键值
   {
	//=====================================================================//	1
    case Key_Power://电源
    KeyPara.CurKey = 0;//清除键值
    if(DeviceStatus== status_Down)//如果是关机
    {
		//短按“电源”按键进行开机，蜂鸣1声（0.5s/声），“电源”按键灯由闪烁转为长亮，显示屏显示默认界面“意式咖啡单杯，温度92C”压力显示“00”
        buzctrl.BeepState   = BeepOnce;
        buzctrl.BeepDuration= Buz_500MS;//蜂鸣器鸣响一声
        CurFuntion = SingleEspersso;//当前功能为单杯
        CurSettingTemp = &TempSingleEsp;//当前设置温度
		
        Dis_Function(CurFuntion);//显示当前功能灯，即单杯
        Dis_PilotLamp(Led_Power);//数码管灯
        Dis_Pressure(0,1);//显示压力
        LT.PotHeatTask.Logictrl.LogictrlBit.execute=1;//执行热水功能
        DeviceStatus = status_Idle;//进入显示状态
    }
    else if(DeviceStatus>status_Down)//否则关闭机器
    {
		AllDown();
		ErrCode =0;
        DeviceStatus = status_Down;//进入关闭状态
    }
    break;
	//=====================================================================//	2
    case Key_Grind://磨豆
    KeyPara.CurKey = 0;
    if(DeviceStatus== status_Idle)//显示中
    {
        Dis_PilotLamp(Led_Power|Led_Grind);//显示磨豆和电源灯
        LT.GrindBeanTask.Logictrl.LogictrlBit.execute = 1; //执行磨豆任务    
            DeviceStatus  = status_Work;//进入工作状态
    }
    else if (DeviceStatus == status_Work)//工作状态
    {
        if(LT.GrindBeanTask.Logictrl.LogictrlBit.execute)//关闭磨豆任务
        {
            LT.GrindBeanTask.Logictrl.LogictrlBit.quit= 1;
            DeviceStatus = status_Idle;//回到显示温度
        }
    }
    break;
	//=====================================================================//	3
    case Key_Temp://温度按键
    KeyPara.CurKey = 0;
    if(CurFuntion<ColdBrew)//不是冷翠功能
	((*CurSettingTemp)==MaxCoffeTemp)?((*CurSettingTemp)=MinCoffeTemp):((*CurSettingTemp)++);//不是最大温度值就增加
    break;
	//=====================================================================//	4
    case Key_Clean://清洁按键
    KeyPara.CurKey = 0;
    if(DeviceStatus == status_Idle)
    {
        // LT.CleanTask.Logictrl.LogictrlBit.execute = 1;
        LT.PipelineFlushingTask.Logictrl.LogictrlBit.execute = 1;//执行清洁任务
        Dis_PilotLamp(Led_Power|Led_Clean);//点亮清洁指示灯
        DeviceStatus = status_Work;
    }
    else if(LT.PipelineFlushingTask.Logictrl.LogictrlBit.execute)
    {
            LT.PipelineFlushingTask.Logictrl.LogictrlBit.quit= 1;
            DeviceStatus = status_Idle;
    }
    break;
	//=====================================================================//	5
    case Key_Steam://蒸汽按键
    KeyPara.CurKey = 0;
    if(DeviceStatus == status_Idle)//如果是显示状态
    {
        // LT.MakeSteamTask.Logictrl.LogictrlBit.execute = 1;
        // Dis_PilotLamp(Led_Power|Led_Steam|Led_Grind);
        DeviceStatus = status_Steam;//跳转到蒸汽
    }
    else if (DeviceStatus == status_Steam)//如果是蒸汽模式，退出任务
    {
        if(LT.MakeSteamTask.Logictrl.LogictrlBit.execute)
        {
            LT.MakeSteamTask.Logictrl.LogictrlBit.quit= 1;
            DeviceStatus = status_Idle;
        }
        else 
            LT.MakeSteamTask.Logictrl.LogictrlBit.execute = 1;  
    }

    break;
	//=====================================================================//	6
    case Key_Hotwater://热水按键
    KeyPara.CurKey = 0;
    if(DeviceStatus == status_Idle)
    {
        LT.MakeHotWaterTask.Logictrl.LogictrlBit.execute = 1;
        Dis_PilotLamp(Led_Power|Led_HotWater);
        DeviceStatus = status_Work;
    }
    else if (DeviceStatus == status_Work)
    {
        if(LT.MakeHotWaterTask.Logictrl.LogictrlBit.execute)
        {
            LT.MakeHotWaterTask.Logictrl.LogictrlBit.quit= 1;
            DeviceStatus = status_Idle;
        }
    }
    break;
	//=====================================================================//	7
    case Key_Extract://萃取按键，进入制作咖啡和退出制作咖啡
    KeyPara.CurKey = 0;
    if(DeviceStatus == status_Idle)//如果是显示状态，则切换到对应的工作，设置好相应的流量控制和预注水时间
    {
        if(CurFuntion==SingleEspersso)//切换到单杯功能
        {
            PreInjectionTime = PreSingleEspTime;//单杯浓缩预注水时间
            MakeFlow = FlowSingleEspe; //单杯浓缩流量
        }
        else if (CurFuntion==DoubleEspersso)
        {
            PreInjectionTime = PreDoubleEspTime;
            MakeFlow = FlowDoubleEsp;
        }
        else if (CurFuntion==Coffe)
        {
            PreInjectionTime = PreCoffeeTime;
            MakeFlow = FlowCoffee; 
        }
        else if (CurFuntion==Americano)
        {
            PreInjectionTime = PreAmericanoTime;
            MakeFlow = FlowAmericano; 
        }
        else if (CurFuntion==ColdBrew)
        {
            MakeFlow = FlowColdBrew; 
        }
        else if (CurFuntion==EsperssoCool)
        {
            PreInjectionTime = perEspCoolTime;
            MakeFlow = FlowEspCool; 
        }
        Dis_PilotLamp(Led_Power);//显示电源灯
        LT.MakeCoffeTask.Logictrl.LogictrlBit.execute = 1;//做咖啡功能执行
        DeviceStatus = status_Work;//工作状态
    }
    else if (DeviceStatus == status_Work)//如果在工作状态
    {
        if(LT.MakeCoffeTask.Logictrl.LogictrlBit.execute)//退出制作咖啡
        {
            LT.MakeCoffeTask.Logictrl.LogictrlBit.quit= 1;
            DeviceStatus = status_Idle;
        }
    }
    break;
   default:
    break;
   } 
}

//数码管扫描
u8 ScanEncoder()
{
   static bit Enc0,Enc1=0;
   static u8 EncOld,EncX=0;
   u8 EncNow=0;
   EnCoder_A =1;
   EnCoder_B =1;
   if(Enc0==0)EncOld=(EnCoder_A?0x02:0x00)+(EnCoder_B?0x01:0x00),Enc0 =1;
   EncNow=(EnCoder_A?0x02:0x00)+(EnCoder_B?0x01:0x00);
   if(EncNow == EncOld)return 0;
   if((EncOld==0x00&&EncNow==0x02)||(EncOld==0x03&&EncNow==0x01))EncX=EncNow;
   if((EncOld==0x00&&EncX==0x02&&EncNow ==0x03)||(EncOld==0x03&&EncX ==0x01&&EncNow==0x00))
   {
       EncOld =EncNow,EncX=0;
       if(Enc1==0)Enc1=1;
       else
       {
		   Delay_us(150);
           SleepCut = 0;
           if(DeviceStatus == status_Idle)
           {
                (CurFuntion&SingleEspersso)?(CurFuntion=EsperssoCool):(CurFuntion>>=1);
                switch(CurFuntion)
                {
                    case SingleEspersso:
                    CurSettingTemp = &TempSingleEsp;
                    break;
                    case DoubleEspersso:
                    CurSettingTemp = &TempDoubleEsp;
                    break;
                    case Coffe:
                    CurSettingTemp = &TempCoffee;
                    break;
                    case Americano:
                    CurSettingTemp = &TempAmericano;
                    break;
                    case ColdBrew:
                    CurSettingTemp = &TempColdDrink;
                    break;
                    case EsperssoCool:
                    CurSettingTemp = &TempColdDrink;
                    break;
                }
           }
			else if ((LT.GrindBeanTask.Logictrl.LogictrlBit.step==2)&&LT.GrindBeanTask.Logictrl.LogictrlBit.execute)
			{
				if(GrindTime>MinGrindTime)GrindTime--;
				TimerRes(&LT.GrindBeanTask.tm);
			}
           Enc1 =0;
           return Anticlockwise;
       }
   }
   if((EncOld==0x00&&EncNow==0x01)||(EncOld==0x03&&EncNow==0x02))EncX=EncNow;
   if((EncOld==0x00&&EncX==0x001&&EncNow==0x03)||(EncOld==0x03&&EncX==0x02&&EncNow==0x00))
   {
       EncOld=EncNow,EncX=0;
       if(Enc1==0)Enc1=1;
       else
       {
            Delay_us(150);
            SleepCut = 0;
            if(DeviceStatus == status_Idle)
            {
                (CurFuntion&EsperssoCool)?(CurFuntion=SingleEspersso):(CurFuntion<<=1);
                switch(CurFuntion)
                {
                    case SingleEspersso:
                    CurSettingTemp = &TempSingleEsp;
                    break;
                    case DoubleEspersso:
                    CurSettingTemp = &TempDoubleEsp;
                    break;
                    case Coffe:
                    CurSettingTemp = &TempCoffee;
                    break;
                    case Americano:
                    CurSettingTemp = &TempAmericano;
                    break;
                    case ColdBrew:
                    CurSettingTemp = &TempColdDrink;
                    break;
                    case EsperssoCool:
                    CurSettingTemp = &TempColdDrink;
                    break;
                }
            }
            else if ((LT.GrindBeanTask.Logictrl.LogictrlBit.step==2)&&LT.GrindBeanTask.Logictrl.LogictrlBit.execute)
            {
                if(GrindTime<MaxGrindTime)GrindTime++;
                TimerRes(&LT.GrindBeanTask.tm);
            }
           Enc1 =0;
           return Clockwise;
       }
   }
   return 0;
}

