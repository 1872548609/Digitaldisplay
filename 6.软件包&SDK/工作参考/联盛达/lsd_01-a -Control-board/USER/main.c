#include "main.h"

DutyCycleOutputType xdata    PumpCtrl            ={0};  
DutyCycleOutputType xdata    PotHeatCtrl         ={0};  
DirectOutputType    xdata    DirectOutput        ={0};
FlowCtrlTypedef     xdata    FlowCtrl            ={0};
DeviceStatusTypedef xdata    DeviceStatus        ={0};
AdParaTypedef       xdata    AdPara              ={0};
LampCtrlTypedef		xdata   LampCtrl			 ={0};
volatile LoopUartType xdata  LoopUart            ={0};


u8 *CurSettingTemp  	  = &TempSingleEsp ; //当前设定温度
u8 xdata cmd_buffer[64]={0};
u8 xdata curTempUnit      = CentigradeDis;//当前温度单位
u8 xdata MakeFlow         = 0; //工作流量
u8 xdata ErrCode          = 0; //错误代码
u8 xdata UartFlag         = 0; //串口接收标志
u8 xdata CurPotAimTemp    = 90; //当前煲目标温度
u8 xdata TempSingleEsp    = 0; //单杯意式温度
u8 xdata TempDoubleEsp    = 0; //双杯意式温度
u8 xdata TempCoffee       = 0; //咖啡温度 
u8 xdata TempAmericano    = 0; //美式
u8 xdata TempColdDrink    = 30; //冷饮温度
u8 xdata CurFuntion       = 0; //当前功能
u8 xdata GrindTime        = 10; //磨豆时间
u8 xdata PreInjectionTime = 0; //预注水时间
u8 xdata EndDis           = 0; //结束显示
u8 xdata EndDisCnt        = 0; //结束显示计数
u16 xdata CurFlow         = 0; //当前流量
u16 xdata ProcessTime     = 0;
u16 xdata TotalFlow       = 0;
u32 xdata SleepCut        = 0; //休眠计数   

//芯片初始化
void MCU_init(void)
{
    Gpio_init();
    Uart_Init();
    LVI_EN();//数据存储
    PWM_initialize();
    TIMER0_initialize();
    QueueInit(&LoopUart);
    EA  =1;
}

void ProcessMessage(PCTRL_MSG msg)
{
    u8 cmdType = msg->cmd_type;
    u8 len     = msg->cmd_len;
    switch (cmdType)
    {
    case CMD_SteamHeat:
    UartFlag &= ~CMD_SteamHeat;

        break;
    case CMD_SteamPump:
    UartFlag &= ~CMD_SteamPump;

        break;
    case CMD_DirOutput:
    UartFlag &= ~CMD_DirOutput;

        break;
    case CMD_ADSensor:
    UartFlag &= ~CMD_ADSensor;
    memcpy(&AdPara,msg->param,sizeof(AdParaTypedef));

    break;
    case CMD_LampCtrl:
    UartFlag &= ~CMD_LampCtrl;
    
        break;
    case CMD_Flow:
    UartFlag &= ~CMD_Flow;
    memcpy(&CurFlow,msg->param,sizeof(CurFlow));

        break;

    case CMD_FlowCtrl:
    UartFlag &= ~CMD_FlowCtrl;
    
        break;
    default:
        break;
    }
}

void FlowCheck()
{
   static u8 FlowFlushCnt = 0;
   static u16 NoFlowCnt = 0;
   static u16 Preflow = 0;
   if(FlowCtrl.FlowCtrlByte == Flow_Scan)
   {
        FlashUartByte(CMD_Flow);
        if((CurFlow - Preflow) < 2)
        {
            NoFlowCnt++;
            if(NoFlowCnt >= Delay_10s)
            {
                    ErrCode     |= ErrTankLack;
                //    DeviceStatus = status_Err;  
                    if(LT.MakeCoffeTask.Logictrl.LogictrlBit.execute)LT.MakeCoffeTask.Logictrl.LogictrlBit.quit= 1;
                    if(LT.MakeSteamTask.Logictrl.LogictrlBit.execute)LT.MakeSteamTask.Logictrl.LogictrlBit.quit= 1;
                    if(LT.MakeHotWaterTask.Logictrl.LogictrlBit.execute)LT.MakeHotWaterTask.Logictrl.LogictrlBit.quit= 1;
                    if(LT.PipelineFlushingTask.Logictrl.LogictrlBit.execute)LT.PipelineFlushingTask.Logictrl.LogictrlBit.quit= 1;
                    DeviceStatus = status_Idle;
            }
        }
		else 
        {
            ErrCode &= (~ErrTankLack);
			NoFlowCnt=0;
        }
       FlowFlushCnt++;
       if (FlowFlushCnt>= Delay_1S)
       {
           FlowFlushCnt= 0;
           Preflow = CurFlow;
       }
   }
   else
   {
	    if((UartFlag&CMD_Flow)==0)
		{
			TotalFlow += CurFlow;
            if(TotalFlow >= CleanFlow)
                ErrCode |= Descaling;
			CurFlow = 0;
			Preflow = 0;
			NoFlowCnt=0;
		}
   }
   if((DeviceStatus > status_Down)&&(DeviceStatus < status_Err))
   {
        Dis_Error(ErrCode); //需要清洁
        Dis_Pressure(AdPara.Press,1); //显示压力
   }
}


void mainTask()//主状态
{
    switch (DeviceStatus)
    {
		//=====================================================================//	1
		case status_Init://初始化，完成后自动进入关闭状态
		LT.InitTask.Logictrl.LogictrlBit.execute = 1;//执行初始化任务//接通电源，机器长响一声(滴1s)，所有灯及显示全亮1s后熄灭；仅“电源”按键灯闪烁（间隔1s/闪）。
		break;
		//=====================================================================//	2
		case status_Down://关机，仅“电源”按键灯闪烁（间隔1s/闪）。
		ProcessTime++;
		if(ProcessTime < Delay_1S)//一秒
			Dis_PilotLamp(Led_Power);//显示电源灯闪烁
		else if(ProcessTime < (2*Delay_1S))//两秒
			Dis_PilotLamp(0);//电源灯关闭
		else//三秒关闭所有任务
		   ProcessTime =0; 
		if(LT.CleanTask.Logictrl.LogictrlBit.execute)LT.CleanTask.Logictrl.LogictrlBit.quit= 1;
		if(LT.GrindBeanTask.Logictrl.LogictrlBit.execute)LT.GrindBeanTask.Logictrl.LogictrlBit.quit= 1;
		if(LT.MakeCoffeTask.Logictrl.LogictrlBit.execute)LT.MakeCoffeTask.Logictrl.LogictrlBit.quit= 1;
		if(LT.MakeSteamTask.Logictrl.LogictrlBit.execute)LT.MakeSteamTask.Logictrl.LogictrlBit.quit= 1;
		if(LT.PotHeatTask.Logictrl.LogictrlBit.execute)LT.PotHeatTask.Logictrl.LogictrlBit.quit= 1;
		if(LT.MakeHotWaterTask.Logictrl.LogictrlBit.execute)LT.MakeHotWaterTask.Logictrl.LogictrlBit.quit= 1;
		if(LT.PipelineFlushingTask.Logictrl.LogictrlBit.execute)LT.PipelineFlushingTask.Logictrl.LogictrlBit.quit= 1;
		break;
		//=====================================================================//	3
		case status_Idle://显示预热温度状态，同时底下机器持续预热，发送温度状态，然后控制板显示
		// if(AdPara.CurCoffeTemp<90)
		if((AdPara.CurCoffeTemp<(*CurSettingTemp))&&((*CurSettingTemp)-AdPara.CurCoffeTemp>=10)) //低温提示低10°
		{
			ProcessTime++;//10ms计数++
			if(ProcessTime < Delay_1S)//1s到
				Dis_PilotLamp(Led_Power|Led_Steam|Led_HotWater|Led_Clean|Led_Grind|Led_Temp);
			else if(ProcessTime < (2*Delay_1S))
				Dis_PilotLamp(Led_Power|Led_Clean|Led_Grind|Led_Temp);
			else
				ProcessTime =0;
		}
		else if((AdPara.CurCoffeTemp>(*CurSettingTemp))&&(AdPara.CurCoffeTemp-(*CurSettingTemp)>=15)) //高温提示高15°
		{
			ProcessTime++;
			if(ProcessTime < Delay_250ms)
				Dis_PilotLamp(Led_Power|Led_Steam|Led_HotWater|Led_Clean|Led_Grind|Led_Temp);
			else if(ProcessTime < (2*Delay_250ms))
				Dis_PilotLamp(Led_Power|Led_Clean|Led_Grind|Led_Temp);
			else
				ProcessTime =0;
		}
		else//“蒸汽”“热水”按键灯由闪烁转为长亮，代表预热完成
		{
			Dis_PilotLamp(Led_Power|Led_Steam|Led_HotWater|Led_Clean|Led_Grind|Led_Temp);
		}
	 #if   DEBUG_TEMP//忽略 
			 Dis_Temperature(AdPara.CurCoffeTemp,curTempUnit); 
	 #else
			Dis_Temperature((*CurSettingTemp),curTempUnit);
	 #endif // DEBUG    
		Dis_Function(CurFuntion);//显示当前功能灯，实时更新模式灯
		break;
		//=====================================================================//	4
		case status_Work://工作状态
		// if(LT.GrindBeanTask.Logictrl.LogictrlBit.execute==0)
		//     Dis_Temperature(AdPara.CurCoffeTemp,curTempUnit);
		break;
		//=====================================================================//	5
		case status_Steam://蒸汽状态
	#if   DEBUG_TEMP 
		Dis_Temperature(AdPara.CurCoffeTemp,curTempUnit);
	#endif // DEBUG    
		if(AdPara.CurCoffeTemp <140) 
		{
			ProcessTime++;
			if(ProcessTime < Delay_1S)
				Dis_PilotLamp(Led_Power|Led_Steam);
			else if(ProcessTime < (2*Delay_1S))
				Dis_PilotLamp(Led_Power);
			else
				ProcessTime =0;
		}
		else
			Dis_PilotLamp(Led_Power|Led_Steam);
		break;
		//=====================================================================//	6
		case status_Clean://清洁状态
		ProcessTime++; 
		if(ProcessTime < Delay_500ms)//500ms延时
			ErrCode |= Descaling;//清洁警告
		else if(ProcessTime < (2*Delay_500ms))
			ErrCode &= (~Descaling);//取消
		else
			ProcessTime =0;
		break;
		//=====================================================================//	7
		case status_Set:

		break;
		//=====================================================================//	8
		case status_Err:
		Dis_PilotLamp(Led_Power);
		Dis_Error(ErrCode);
		if(ErrCode & ErrPotNtcHot)
		   CodeDis(ERR,4);  //煲超温 E-4
		else if(ErrCode & ErrPotNtcOpen)
		   CodeDis(ERR,2);  //NTC断路 E-2
		else if(ErrCode & ErrPotNtcShort)
		   CodeDis(ERR,1);  //NTC短路 E-1
		if(LT.CleanTask.Logictrl.LogictrlBit.execute)LT.CleanTask.Logictrl.LogictrlBit.quit= 1;
		if(LT.GrindBeanTask.Logictrl.LogictrlBit.execute)LT.GrindBeanTask.Logictrl.LogictrlBit.quit= 1;
		if(LT.MakeCoffeTask.Logictrl.LogictrlBit.execute)LT.MakeCoffeTask.Logictrl.LogictrlBit.quit= 1;
		if(LT.MakeSteamTask.Logictrl.LogictrlBit.execute)LT.MakeSteamTask.Logictrl.LogictrlBit.quit= 1;
		if(LT.PotHeatTask.Logictrl.LogictrlBit.execute)LT.PotHeatTask.Logictrl.LogictrlBit.quit= 1;
		if(LT.MakeHotWaterTask.Logictrl.LogictrlBit.execute)LT.MakeHotWaterTask.Logictrl.LogictrlBit.quit= 1;
		if(LT.PipelineFlushingTask.Logictrl.LogictrlBit.execute)LT.PipelineFlushingTask.Logictrl.LogictrlBit.quit= 1;
		break;
		//=====================================================================//	9
		default:
			break;
    }
}

#define ErrdisTime 500//磨豆错误显示延时5秒
void CheckGrining()//磨豆检测
{
    static u16 ErrDisCnt=0;
    if(DirectOutput.OutputCtrlBit.GrindBean == DirOutPut_ON)
    {
		
        if(AdPara.AdMotor == NoWork)
        {
			//磨豆电机未工作 豆仓安装异常
			ErrDisCnt++;
			if(ErrDisCnt >= 200)
			{
				ErrDisCnt = ErrdisTime;
				ErrCode |= ErrBeanWareHouse;
			}
        }
        else if (AdPara.AdMotor == NoLoad)
        {
            //电机空转无豆
			ErrDisCnt++;
			if(ErrDisCnt >= 200)
			{
				ErrDisCnt = ErrdisTime;
				ErrCode |= ErrBeanLack;
			}
        }
        else if(AdPara.AdMotor ==Stall)
        {
            //电机堵转
        }
        else if(AdPara.AdMotor == Grinding)
        {
            ErrDisCnt++;
            if(ErrDisCnt >= 100)
            {
                ErrCode &= (~ErrBeanWareHouse);
                ErrCode &= (~ErrBeanLack);
            }
        }
    }
    else if(ErrDisCnt) 
    {
       ErrDisCnt--; 
    }
	else
	{
       ErrCode &= (~ErrBeanWareHouse);
       ErrCode &= (~ErrBeanLack);
	}
}

void SleepCtrl()//休眠设置
{
    if(DeviceStatus > status_Down)
    {
        if(SleepCut >= SleepTime)
        {
            AllDown();
            DeviceStatus = status_Down;
        }
    }
}

//串口数据刷新
void SerialPortDataRefresh()
{
    if(QueueFinDataFrame(&LoopUart,cmd_buffer)>0)
        ProcessMessage((PCTRL_MSG)cmd_buffer);
}


void main(void)
{
    MCU_init();
    while (1)
    {
       SerialPortDataRefresh();
       ScanEncoder();
       Logic();
       if(ProcessCount>=DIV_10MS)
       {
            ProcessCount=0;
            Scan();//按键扫描
            Respone();//按键回调
            FlowCheck();//流控
            CheckSensor();//温度检测
            CheckGrining();//磨豆检测
			BeepCtrl();//蜂鸣器控制
            mainTask();//主状态
            SleepCtrl();//休眠检测
			DisRefresh();//显示控制
       }
    }
}
