#include "Logic.h"



Task xdata LogicTask ={0};

u8 FlashUartByte(u8 newStatus)//串口命令跟新
{
    if((UartFlag &newStatus)==0)
    {
        UartFlag |=newStatus;
        return 1;
    }
    else
        return 0;
}

void ProgramInit(LogicParaDef*p )//初始化任务
{
    TASK_QUIT
    {
        CtrlPara.execute = 0;
        CtrlPara.quit    = 0;
        CtrlPara.step    = 0;
    }
    TASK_PROCESS
    {
        case 1:
        if(FlashUartByte(CMD_FlowCtrl|CMD_SteamHeat|CMD_LampCtrl))//流量计，蒸汽加热，照明
        {
			//接通电源，机器长响一声(滴1s)，所有灯及显示全亮1s后熄灭；仅“电源”按键灯闪烁（间隔1s/闪）。
            buzctrl.BeepState = BeepOnce;//蜂鸣器鸣响一声 
			AllUp();//灯全开
			
			 LampCtrl.CtrlByte = FULL_POWER;//全功率加热
            FlowCtrl.PulsePeriod = PulsePerMl;//流量每升两个脉冲
            PotHeatCtrl.Period   = 80;//加热周期
			
            buzctrl.BeepDuration = Buz_1S;
            ReadTotalFlow();//读取总流量
            ReadSingleEspTemp(); //读取温度
            ReadDoubleEspTemp();//读取双杯温度
            ReadCoffeTemp();//读取咖啡温度
            ReadAmericanoTemp();//读美式温度
          
            TimerRes(&Tim);///读取当前时间
            CtrlPara.step = 2;//进入步骤2
        }
       break;
        case 2:
            if(GetTime(Tim,DIV_1S)>= 1)//一秒后
            {
                if(FlashUartByte(CMD_LampCtrl))//更新照明控制
                {
                    LampCtrl.CtrlByte = All_Down;//照明全关
                    AllDown();//控制板灯全关
                    CtrlPara.step = 3;//进入步骤3
                }
            }
        break;
        case 3:
            DeviceStatus = status_Down;//初始化完毕关机
            CtrlPara.execute = 0;
            CtrlPara.quit    = 0;
            CtrlPara.step    = 0;
        break;
    }
}



void Makecoffe(LogicParaDef*p )//做咖啡，按下萃取后的动作
{
    TASK_QUIT
    {
        if(FlashUartByte(CMD_DirOutput|CMD_SteamPump|CMD_FlowCtrl))
        {
            DirectOutput.OutputCtrlBit.CoffeValve = DirOutPut_OFF;
            DirectOutput.OutputCtrlBit.HotWaterValve= DirOutPut_OFF;
            FlowCtrl.FlowCtrlByte = Flow_End;
            PumpCtrl.workFlag = OFF;
            CtrlPara.execute = 0;
            CtrlPara.quit    = 0;
            CtrlPara.step    = 0;
        }
    }
    TASK_PROCESS
    {
		//=====================================================================//	1
        case 1:
			Dis_Temperature(0,NoUnitDis);//温度显示0
        if(FlashUartByte(CMD_DirOutput|CMD_SteamPump|CMD_FlowCtrl))//更新串口命令
        {
            DirectOutput.OutputCtrlBit.CoffeValve = DirOutPut_ON;//开启咖啡电磁阀
            if(CurFuntion == ColdBrew)//冷翠功能
            {
                PumpCtrl.Duty   = 400;  //开0.5关0.5
                PumpCtrl.Period = 800;
                CtrlPara.step = 4;//进步骤4
            }
            else//不是冷翠功能
            {
                PumpCtrl.Duty   = 100; //水泵占空比
                PumpCtrl.Period = 100;
                CtrlPara.step = 2;//进步骤2
            }
            FlowCtrl.FlowCtrlByte = Flow_Scan;//流量启动
            PumpCtrl.workFlag = ON;//水泵PWM开启
            TimerRes(&Tim);//获取当前时间
        }
        break;
		//=====================================================================//	2
        case 2:
        EndDis=GetTime(Tim,DIV_1S);//获取当前时间间隔
        Dis_Temperature(EndDis,NoUnitDis);//显示时间
        if(EndDis>= PreInjectionTime)//大于预注水时间，注水完成
        {
            if(FlashUartByte(CMD_DirOutput|CMD_SteamPump|CMD_FlowCtrl)) 
            {
                DirectOutput.OutputCtrlBit.CoffeValve= DirOutPut_OFF;//咖啡电磁阀关闭
                FlowCtrl.FlowCtrlByte = Flow_End;//关闭流控
                PumpCtrl.workFlag = OFF;//停止泵
                CtrlPara.step = 3;//进步骤3
            }
        }
        break;
		//=====================================================================//	3
        case 3:
        EndDis=GetTime(Tim,DIV_1S);
        Dis_Temperature(EndDis,NoUnitDis);//延时时间
        if(EndDis>= (4+PreInjectionTime))//延时时间大于注水后停止4s
        {
            if(FlashUartByte(CMD_DirOutput|CMD_SteamPump|CMD_FlowCtrl))//打开蒸汽泵，打开流控
            {
                DirectOutput.OutputCtrlBit.CoffeValve= DirOutPut_ON;//打开咖啡电磁阀
                FlowCtrl.FlowCtrlByte = Flow_Scan;//开启流控
                PumpCtrl.workFlag= ON; //关闭水泵
                CtrlPara.step = 4;//进入步骤4
            }
        }
        break;
		//=====================================================================//	4
        case 4:
        EndDis=GetTime(Tim,DIV_1S);
        Dis_Temperature(EndDis,NoUnitDis);
        if(CurFlow >= MakeFlow)//当前流量，达到工作流量
        {
            if(FlashUartByte(CMD_DirOutput|CMD_SteamPump|CMD_FlowCtrl))
            {
                DirectOutput.OutputCtrlBit.CoffeValve= DirOutPut_OFF;//关闭电磁阀
                FlowCtrl.FlowCtrlByte = Flow_End;//流控关闭
                PumpCtrl.workFlag = OFF;//水泵关闭
                TimerRes(&Tim);//获取当前时间
                if(CurFuntion == Americano)//当前是美式
                CtrlPara.step = 5;//进步骤5
                else
                CtrlPara.step = 7;//否则进步骤7
            }
        }
        break;
		//=====================================================================//	5
        case 5://美式步骤1
        EndDis += GetTime(Tim,DIV_1S);
        Dis_Temperature(EndDis,NoUnitDis);
        if(GetTime(Tim,DIV_1S)>= 1)     
        {
            if(FlashUartByte(CMD_DirOutput|CMD_SteamPump|CMD_FlowCtrl))
            {
                DirectOutput.OutputCtrlBit.HotWaterValve= DirOutPut_ON;//开启咖啡电磁阀
                FlowCtrl.FlowCtrlByte = Flow_Scan;//打开流控
                PumpCtrl.workFlag= ON; //开启水泵
                CtrlPara.step = 6;//进步骤6
            }
        }
        break;
		//=====================================================================//	6
        case 6://美式步骤2
        EndDis += GetTime(Tim,DIV_1S);
        Dis_Temperature(EndDis,NoUnitDis);
        if(CurFlow >= 85)//流量大于85
        {
            if(FlashUartByte(CMD_DirOutput|CMD_SteamPump|CMD_FlowCtrl))
            {
                DirectOutput.OutputCtrlBit.HotWaterValve= DirOutPut_OFF;//关闭咖啡电磁阀
                FlowCtrl.FlowCtrlByte = Flow_End;//关闭流控
                PumpCtrl.workFlag = OFF;//关闭水泵
                CtrlPara.step = 7;//进步骤7退出制作咖啡
            }
        }
        break;
		//=====================================================================//	7
        case 7://回归显示状态，完成制作咖啡
            DeviceStatus = status_Idle;
            CtrlPara.execute = 0;
            CtrlPara.quit    = 0;
            CtrlPara.step    = 0;
        break;
    }
}

void MakeSteam(LogicParaDef*p )//开蒸汽
{
    TASK_QUIT
    {
        if(FlashUartByte(CMD_DirOutput|CMD_SteamPump|CMD_FlowCtrl))
        {
            DirectOutput.OutputCtrlBit.SteamValve = DirOutPut_OFF;
            DirectOutput.OutputCtrlBit.CoffeValve = DirOutPut_OFF;
            FlowCtrl.FlowCtrlByte = Flow_End;
            PumpCtrl.workFlag = OFF;
            CtrlPara.execute = 0;
            CtrlPara.quit    = 0;
            CtrlPara.step    = 0;
        }
    }
    TASK_PROCESS
    {
        case 1:
#if !DEBUG_TEMP 
        Dis_Temperature(0,NoUnitDis);//显示0
#endif
        if(FlashUartByte(CMD_DirOutput|CMD_SteamPump|CMD_FlowCtrl))
        {
            DirectOutput.OutputCtrlBit.SteamValve = DirOutPut_ON;//开启蒸汽电磁阀
            FlowCtrl.FlowCtrlByte = Flow_Scan;//打开流量计
            PumpCtrl.Duty       = 16;//水泵占空比
            PumpCtrl.Period     = 280;
            PumpCtrl.workFlag  = ON;//水泵工作
            TimerRes(&Tim);//获取当前时间
            CtrlPara.step = 2;//进入步骤2
        }
        break;
        case 2:
#if !DEBUG_TEMP 
        EndDis=GetTime(Tim,DIV_1S);
        Dis_Temperature(EndDis,NoUnitDis);//显示时间
#endif
        if(GetTime(Tim,DIV_1S)>= MaxSteamTime)//时间大于最大出蒸汽时间
        {
            DirectOutput.OutputCtrlBit.SteamValve = DirOutPut_OFF;//关闭蒸汽电磁阀
            FlowCtrl.FlowCtrlByte = Flow_End;//关闭流量计
            PumpCtrl.workFlag = OFF;//停止水泵
            CtrlPara.step = 3;//进入步骤4
        }
        break;
        case 3:
            DeviceStatus = status_Idle;//回到显示模式
            CtrlPara.execute = 0;
            CtrlPara.quit    = 0;
            CtrlPara.step    = 0;
        break;
    }
}

void MakeHotWater(LogicParaDef*p )//开热水
{
    TASK_QUIT
    {
        if(FlashUartByte(CMD_DirOutput|CMD_SteamPump|CMD_FlowCtrl))
        {
            DirectOutput.OutputCtrlBit.HotWaterValve= DirOutPut_OFF;
            FlowCtrl.FlowCtrlByte = Flow_End;
            PumpCtrl.workFlag = OFF;
            CtrlPara.execute = 0;
            CtrlPara.quit    = 0;
            CtrlPara.step    = 0;
        }
    }
    TASK_PROCESS
    {
        case 1:
        Dis_Temperature(0,NoUnitDis);
        if(FlashUartByte(CMD_DirOutput|CMD_SteamPump|CMD_FlowCtrl))
        {
            DirectOutput.OutputCtrlBit.HotWaterValve= DirOutPut_ON;//开启热水电磁阀
            FlowCtrl.FlowCtrlByte = Flow_Scan;//打开流量计
            PumpCtrl.Duty   = 100;//设置水泵工作占空比
            PumpCtrl.Period = 100;
            PumpCtrl.workFlag = ON;
            TimerRes(&Tim);//获取当前时间
            CtrlPara.step = 2;//进入步骤2
        }
        break;
        case 2:
        EndDis=GetTime(Tim,DIV_1S);
        Dis_Temperature(EndDis,NoUnitDis);
        if(GetTime(Tim,DIV_1S)>= MaxWaterTime)//延时最大热水时间
        {
            if(FlashUartByte(CMD_DirOutput|CMD_SteamPump|CMD_FlowCtrl))
            {
                DirectOutput.OutputCtrlBit.HotWaterValve= DirOutPut_OFF;//关闭热水电磁阀
                FlowCtrl.FlowCtrlByte = Flow_End;//关闭流量计
                PumpCtrl.workFlag = OFF;//停止水泵
                CtrlPara.step = 3;
            }
        }
        break;
        case 3:
		DeviceStatus = status_Idle;//回到显示模式
        CtrlPara.execute = 0;
        CtrlPara.quit    = 0;
        CtrlPara.step    = 0;
        break;
    }                                                               

}

void Grind(LogicParaDef*p )//磨豆
{
    TASK_QUIT
    {
        if(FlashUartByte(CMD_DirOutput|CMD_LampCtrl))
        {
            LampCtrl.CtrlByte = All_Down;
            DirectOutput.OutputCtrlBit.Plasma    = DirOutPut_OFF;
            DirectOutput.OutputCtrlBit.GrindBean = DirOutPut_OFF;
			
            CtrlPara.execute = 0;
            CtrlPara.quit    = 0;
            CtrlPara.step    = 0;
        }
    }
    TASK_PROCESS
    {
        case 1:
        TimerRes(&Tim);//获取当前时间
        CtrlPara.step = 2;//进入步骤2
        break;
        case 2:
			Dis_Temperature(GrindTime,NoUnitDis);//显示磨豆温度，无单位转换
        if(GetTime(Tim,DIV_1S)>= 3)//3s后
        {
            CtrlPara.step = 3;//进入步骤3
        }
		break;
        case 3:
        // Dis_Temperature(GrindTime,NoUnitDis);
        if(FlashUartByte(CMD_DirOutput|CMD_LampCtrl))//串口命令跟新，打开照明，直接输出控制
        {
            LampCtrl.CtrlByte = FULL_POWER;//全功率开灯
            DirectOutput.OutputCtrlBit.Plasma    = DirOutPut_ON;//打开等离子
            DirectOutput.OutputCtrlBit.GrindBean = DirOutPut_ON;//打开磨豆
            TimerRes(&Tim);//获取当前时间
            CtrlPara.step = 4;//进入步骤4
        }
		break;
        case 4:
		Dis_Temperature((GrindTime-GetTime(Tim,DIV_1S)),NoUnitDis);//显示温度位置显示时间倒计时后自动结束
        if(GetTime(Tim,DIV_1S)>= GrindTime)//如果延时大于磨豆时间
        {
            if(FlashUartByte(CMD_DirOutput|CMD_LampCtrl))//关闭灯显
            {
                LampCtrl.CtrlByte = All_Down;//关闭
                DirectOutput.OutputCtrlBit.Plasma    = DirOutPut_OFF;//关闭等离子和磨豆
                DirectOutput.OutputCtrlBit.GrindBean = DirOutPut_OFF;
                CtrlPara.step = 5;
            }
        }
		break;
        case 5:
            DeviceStatus = status_Idle;//回到显示状态
            CtrlPara.execute = 0;
            CtrlPara.quit    = 0;
            CtrlPara.step    = 0;
		break;
    }
}


void PotHeat(LogicParaDef*p )//加热
{
    TASK_QUIT
    {
        if(FlashUartByte(CMD_SteamHeat))
        {
            PotHeatCtrl.workFlag = OFF;
            CtrlPara.execute = 0;
            CtrlPara.quit    = 0;
            CtrlPara.step    = 0;
        }
    }
    TASK_PROCESS
    {
       case 1:
       if(LT.MakeCoffeTask.Logictrl.LogictrlBit.execute)//执行制作咖啡
           CtrlPara.step = 2;
       else if(LT.MakeSteamTask.Logictrl.LogictrlBit.execute)//执行蒸汽
            CtrlPara.step = 3;
       else if (LT.MakeHotWaterTask.Logictrl.LogictrlBit.execute)//执行热水
            CtrlPara.step = 4;
       else if(DeviceStatus == status_Steam)//蒸汽状态
            CtrlPara.step = 5;
       else if(DeviceStatus == status_Idle)//显示状态
            CtrlPara.step = 6;
       else
       {
            if(FlashUartByte(CMD_SteamHeat))
                PotHeatCtrl.workFlag = OFF;//关闭加热
       }
       break;
       case 2: //做咖啡加热
       if(LT.MakeCoffeTask.Logictrl.LogictrlBit.execute==0)
            CtrlPara.step = 1;
       if(AdPara.CurCoffeTemp<CurPotAimTemp)
       {
            if(FlashUartByte(CMD_SteamHeat))
            {
                PotHeatCtrl.Duty   = 80;
                PotHeatCtrl.workFlag = ON;
            }
       }
       else 
       {
            if(FlashUartByte(CMD_SteamHeat))
                PotHeatCtrl.workFlag = OFF;
       }
       break;
       case 3: //做蒸汽加热
        if(LT.MakeSteamTask.Logictrl.LogictrlBit.execute==0)
            CtrlPara.step = 1;
        if(AdPara.CurCoffeTemp<142)
        {
            if(FlashUartByte(CMD_SteamHeat))
            {
                PotHeatCtrl.Duty   = 80;
                PotHeatCtrl.workFlag = ON;
            }
        }
        else if (AdPara.CurCoffeTemp<152)
        {
            if(FlashUartByte(CMD_SteamHeat))
            {
                PotHeatCtrl.Duty   = 55;
                PotHeatCtrl.workFlag = ON;
            }
        }
        else if (AdPara.CurCoffeTemp<160)
        {
            if(FlashUartByte(CMD_SteamHeat))
            {
                PotHeatCtrl.Duty   = 20;
                PotHeatCtrl.workFlag = ON;
            }
        }
        else 
        {
            if(FlashUartByte(CMD_SteamHeat))
                PotHeatCtrl.workFlag = OFF;
        }
       break;
       case 4: //做热水加热
        if(LT.MakeHotWaterTask.Logictrl.LogictrlBit.execute==0)
            CtrlPara.step = 1;
        if(AdPara.CurCoffeTemp<110)
        {
            if(FlashUartByte(CMD_SteamHeat))    
            {
                PotHeatCtrl.Duty   = 80;
                PotHeatCtrl.workFlag = ON;
            }
        }
        else 
        {
            if(FlashUartByte(CMD_SteamHeat))
                PotHeatCtrl.workFlag = OFF;
        }
       break;
       case 5: //蒸汽预热
        if(LT.MakeSteamTask.Logictrl.LogictrlBit.execute||(DeviceStatus != status_Steam))
            CtrlPara.step = 1;
        if(AdPara.CurCoffeTemp<100)
        {
            if(FlashUartByte(CMD_SteamHeat))
            {
                PotHeatCtrl.Duty   = 80;
                PotHeatCtrl.workFlag = ON;
            }
        }
        else if (AdPara.CurCoffeTemp<120)
        {
            if(FlashUartByte(CMD_SteamHeat))
            {
                PotHeatCtrl.Duty   = 37;
                PotHeatCtrl.workFlag = ON;
            }
        }
        else if (AdPara.CurCoffeTemp<142)
        {
            if(FlashUartByte(CMD_SteamHeat))
            {
                PotHeatCtrl.Duty   = 10;
                PotHeatCtrl.workFlag = ON;
            }
        }
        else 
        {
            if(FlashUartByte(CMD_SteamHeat))
                PotHeatCtrl.workFlag = OFF;
        }

       break;
       case 6://咖啡预热
       if(DeviceStatus != status_Idle)
            CtrlPara.step = 1;
        if(AdPara.CurCoffeTemp<(CurPotAimTemp-25))
        {
            if(FlashUartByte(CMD_SteamHeat))
            {
                PotHeatCtrl.Duty   = 80;
                PotHeatCtrl.workFlag = ON;
            }
        }
        else if(AdPara.CurCoffeTemp<CurPotAimTemp)
        {
            if(FlashUartByte(CMD_SteamHeat))
            {
                PotHeatCtrl.Duty   = 20;
                PotHeatCtrl.workFlag = ON;
            }
        }
        else 
        {
            if(FlashUartByte(CMD_SteamHeat))
                PotHeatCtrl.workFlag = OFF;
        }
       break;
    }
}

void Clean(LogicParaDef*p )//清洁任务
{
    static u8 CleanCount = 0;
    TASK_QUIT
    {
        if(FlashUartByte(CMD_DirOutput|CMD_FlowCtrl|CMD_SteamPump))
        {
            DirectOutput.OutputCtrlBit.CoffeValve = DirOutPut_OFF;
            FlowCtrl.FlowCtrlByte = Flow_End;
            PumpCtrl.workFlag = OFF;
            CtrlPara.execute = 0;
            CtrlPara.quit    = 0;
            CtrlPara.step    = 0;
        }
    }
    TASK_PROCESS    
    {
        case 1:
        if(FlashUartByte(CMD_DirOutput|CMD_FlowCtrl|CMD_SteamPump))
        {
            TotalFlow = 0;
            DirectOutput.OutputCtrlBit.CoffeValve = DirOutPut_ON;//打开咖啡电磁阀
            FlowCtrl.FlowCtrlByte = Flow_Scan;//打开流量计
            PumpCtrl.Duty   = 100;
            PumpCtrl.Period = 100;//水泵占空比
            PumpCtrl.workFlag = ON;//水泵工作
            TimerRes(&Tim);//当前时间
            CleanCount = 0;
            CtrlPara.step = 2;//进入步骤2
        }
        break;
        case 2:
        if(GetTime(Tim,DIV_1S)>= 8)//8秒延时
        {
            if(FlashUartByte(CMD_DirOutput|CMD_FlowCtrl|CMD_SteamPump))
            {
                DirectOutput.OutputCtrlBit.CoffeValve = DirOutPut_OFF;//关闭
                FlowCtrl.FlowCtrlByte = Flow_End;//关闭流量计
                PumpCtrl.workFlag = OFF;//关闭水泵
                TimerRes(&Tim);//获取当前时间
                CtrlPara.step = 3;
            }
        }
        break;
        case 3: 
        if(GetTime(Tim,DIV_1S)>= 4) //4s延时
        {
            if(FlashUartByte(CMD_DirOutput|CMD_FlowCtrl|CMD_SteamPump))//流量计和蒸汽水泵控制
            {
                DirectOutput.OutputCtrlBit.CoffeValve = DirOutPut_ON;//开启
                FlowCtrl.FlowCtrlByte = Flow_Scan;//流量计打开
                PumpCtrl.workFlag = ON;//工作
                TimerRes(&Tim);//获取当前时间
                CleanCount++;//计时50ms
                if(CleanCount >= 5)
                    CtrlPara.step = 4;//大于50ms进入步骤4
                else
                    CtrlPara.step = 2;//回到步骤2
            }
        }
        break;
        case 4:
        DeviceStatus = status_Idle;//回到状态
        CtrlPara.execute = 0;
        CtrlPara.quit    = 0;
        CtrlPara.step    = 0;
        break;
    }
}

void PipelineFlushing(LogicParaDef*p)//冲洗管道
{
    TASK_QUIT
    {
        if(FlashUartByte(CMD_SteamPump|CMD_DirOutput|CMD_FlowCtrl))
        {
            DirectOutput.OutputCtrlBit.CoffeValve= DirOutPut_OFF;
            FlowCtrl.FlowCtrlByte = Flow_End;
            PumpCtrl.workFlag = OFF;
            CtrlPara.execute = 0;
            CtrlPara.quit    = 0;
            CtrlPara.step    = 0;
        }
    }
    TASK_PROCESS
    {
        case 1:
        Dis_Temperature(0,NoUnitDis);
        if(FlashUartByte(CMD_SteamPump|CMD_FlowCtrl|CMD_DirOutput))
        {
            DirectOutput.OutputCtrlBit.CoffeValve= DirOutPut_ON;//开咖啡电磁阀
            FlowCtrl.FlowCtrlByte = Flow_Scan;
            PumpCtrl.Duty   = 100;
            PumpCtrl.Period = 100;
            PumpCtrl.workFlag = ON;
            TimerRes(&Tim);
            CtrlPara.step = 2;
        }
        break;
        case 2:
        EndDis=GetTime(Tim,DIV_1S);
        Dis_Temperature(EndDis,NoUnitDis);
        if(EndDis>= 15)//15s
        {
            if(FlashUartByte(CMD_SteamPump|CMD_FlowCtrl|CMD_DirOutput))//关闭咖啡电磁阀
            {
                DirectOutput.OutputCtrlBit.CoffeValve= DirOutPut_OFF;
                FlowCtrl.FlowCtrlByte = Flow_End;
                PumpCtrl.workFlag = OFF;
                CtrlPara.step = 3;
            }
        }
        break;
        case 3:
            DeviceStatus = status_Idle;//回到显示状态
            CtrlPara.execute = 0;
            CtrlPara.quit    = 0;
            CtrlPara.step    = 0;
        break;
    }
}




#define TimeOut 200
void OutPutFlash()   
{
    static u8 DelayCnt1 =0;
    static u8 DelayCnt2 =0;
    static u8 DelayCnt3 =0;
    static u8 DelayCnt4 =0;
    static u8 DelayCnt5 =0;
    static u8 DelayCnt6 =0;
    static u8 DelayCnt7 =0;
    if(UartFlag&CMD_ADSensor)
    {
        DelayCnt1--;
        if(DelayCnt1==0)
        {
            DelayCnt1=TimeOut;
            DataFrameTransmission(&AdPara,CMD_ADSensor,sizeof(AdParaTypedef));
        }
    }
    else
        DelayCnt1=0;
    if(UartFlag&CMD_FlowCtrl)
    {
        DelayCnt2--;
        if(DelayCnt2==0)
        {
            DelayCnt2=TimeOut;
            DataFrameTransmission(&FlowCtrl,CMD_FlowCtrl,sizeof(FlowCtrlTypedef));  
        }
    }
    else
        DelayCnt2=0;
    if(UartFlag&CMD_DirOutput)
    {
        DelayCnt3--;
        if(DelayCnt3==0)
        {
            DelayCnt3=TimeOut;
            DataFrameTransmission(&DirectOutput,CMD_DirOutput,sizeof(DirectOutputType));
        }
    }
    else
        DelayCnt3=0;
    if(UartFlag&CMD_SteamPump)
    {
        DelayCnt4--;
        if(DelayCnt4==0)
        {
            DelayCnt4=TimeOut;
            DataFrameTransmission(&PumpCtrl,CMD_SteamPump,sizeof(DutyCycleOutputType));
        }
    }
    else
        DelayCnt4=0;
    if(UartFlag&CMD_SteamHeat)
    {
        DelayCnt5--;
        if(DelayCnt5==0)
        {
            DelayCnt5=TimeOut;
            DataFrameTransmission(&PotHeatCtrl,CMD_SteamHeat,sizeof(DutyCycleOutputType));
        }
    }
    else
        DelayCnt5=0;
    if(UartFlag&CMD_LampCtrl)
    {
        DelayCnt6--;
        if(DelayCnt6==0)
        {
            DelayCnt6=TimeOut;
            DataFrameTransmission(&LampCtrl,CMD_LampCtrl,sizeof(LampCtrlTypedef));
        }
    }
    else
        DelayCnt6=0;
    if(UartFlag&CMD_Flow)
    {
        DelayCnt7--;
        if(DelayCnt7==0)
        {
            DelayCnt7=TimeOut;
            DataFrameTransmission(&CurFlow,CMD_Flow,sizeof(CurFlow));
        }
    }
    else
        DelayCnt7=0;
}

void Logic()
{
    ProgramInit(&LT.InitTask);//初始化任务//接通电源，机器长响一声(滴1s)，所有灯及显示全亮1s后熄灭；仅“电源”按键灯闪烁（间隔1s/闪）。
    Makecoffe(&LT.MakeCoffeTask);
    MakeSteam(&LT.MakeSteamTask);
    MakeHotWater(&LT.MakeHotWaterTask);
    Grind(&LT.GrindBeanTask);
    PotHeat(&LT.PotHeatTask);
    Clean(&LT.CleanTask);
    PipelineFlushing(&LT.PipelineFlushingTask);
	OutPutFlash();
}


