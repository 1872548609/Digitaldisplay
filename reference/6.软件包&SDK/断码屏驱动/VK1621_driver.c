/**
  ******************************************************************************
  * @file    vk1621.c
  * @author  kevin_guo
  * @version V1.2
  * @date    10-15-2024
  * @brief   This file contains all the vk1621 functions. 
	*          此文件适用于 VK1621
  ******************************************************************************
  * @attention
  ******************************************************************************
  */	
/* Includes ------------------------------------------------------------------*/
#include "VK1621_driver.h"
  
/* extern variables ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define VK1621_CLK 5 //WR信号线频率,由delay_nus实现 50->10kHz 10->50kHz 5->100kHz
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//segtab[]数组对应实际的芯片到LCD连线，连线见-VK1621参考电路
unsigned char vk1621_segtab[VK1621_SEGNUM]={
	0,1,2,3,4,5,6,7,8,9,						//SEG0-SEG9
	10,11,12,13,14,15,16,17,18,19,	//SEG10-SEG19
	20,21,22,23,24,25,26,27,28,29,	//SEG20-SEG29
	30,31														//SEG30-SEG31
};
//例：LCD实际只接了SEG10到SEG21使用下面数组
//.h文件配置VK1621_SEGNUM=12
//const unsigned char vk1621_segtab[VK1621_SEGNUM]={	
//	10,11,12,13,14,15,16,17,18,19,	//SEG10-SEG19
//	20,21,													//SEG20-SEG21
//};
//vk1621_dispram对应芯片的显示RAM
unsigned char vk1621_dispram[VK1621_SEGNUM];
unsigned char dispram[32];
//对应数组vk1621_segtab[VK1621_SEGNUM]
//显示RAM buffer只使用低4位，bit0->com0,bit1->com1,bit2->com2,bit3->com3
//LCD 实例  3个8字
//1是百位，2是十位，3是个位
/*显示buffer和lcd图案映射关系如下
*/
/*8字点序 
     A
   F   B
     G
	 E   C
	   D
*/
unsigned char num[10]=
{
  //0     1    2    3    4    5    6    7    8   9  
   0xAF,0xA0,0xCB,0xE9,0xE4,0x6D,0x6F,0xA8,0xEF,0xED,
};
unsigned char vk1621_addrbit=6;//驱动ic的ram地址线数A5-A0,
unsigned char vk1621_segi,vk1621_comi;
unsigned char vk1621_maxcom;//驱动的com数可以是4com，3com，2com
/* Private function prototypes -----------------------------------------------*/
/* Private function ----------------------------------------------------------*/
/*******************************************************************************
* Function Name  : delay_nus
* Description    : 延时1uS程序
* Input          : n->延时时间nuS
* Output         : None
* Return         : None
*******************************************************************************/
void delay_nus(unsigned int n)	   
{
	unsigned char i;
	while(n--)
	{
		i=10;
		while(i--)
		{//nop指令根据单片机做相应的修改
			__nop();
		}
	}
}
/*******************************************************************************
* Function Name  : delay_nms
* Description    : 延时1mS程序
* Input          : n->延时时间nmS
* Output         : None
* Return         : None
*******************************************************************************/
void delay_nms(unsigned long int n)
{
	while(n--)
	{
		delay_nus(1000);
	}
}
/*******************************************************************************
* Function Name  : WriteClock
* Description    : WR脚时钟
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WriteClockVk1621(void)
{
	VK1621_WR_L(); 
	delay_nus(VK1621_CLK);
	VK1621_WR_H();	
	delay_nus(VK1621_CLK);	
}
/*******************************************************************************
* Function Name  : WriteCommand
* Description    : 写命令
* Input          : FunctonCode->功能/命令码 
* Output         : None
* Return         : None
*******************************************************************************/
void WriteCommandVk1621(unsigned char FunctonCode)
{
	unsigned char Shift = 0x80; 
	unsigned char i;
	
	VK1621_CS_L();   //CS 片选开   
	delay_nus(VK1621_CLK/2);
	VK1621_DATA_H();  
	WriteClockVk1621();
	VK1621_DATA_L();  
	WriteClockVk1621();
	VK1621_DATA_L();  
	WriteClockVk1621();

	for(i = 0; i < 8; i++) 
	{
	 if(Shift & FunctonCode) 	
		 VK1621_DATA_H(); 
	 else 
		 VK1621_DATA_L();	  

	 WriteClockVk1621();
	 Shift = Shift >> 1;
	}
  VK1621_DATA_L(); 
	WriteClockVk1621();	//发送一个‘0’，命令码中最后一位 X
  VK1621_CS_H(); 			//CS 片选关  
	delay_nus(VK1621_CLK/2);
  VK1621_DATA_H(); 
}
/*******************************************************************************
* Function Name  : WritenData
* Description    : 写n个SEG数据到显示RAM，bit3-COM3 bit2-COM2 bit1-COM1 bit0-COM0
* Input          : Addr->写入ram的起始地址  
* 							 : Databuf->写入ram的数据指针(每个字节数据低4位有效)  
* 							 : Cnt->写入ram的数据个数
* Output         : None
* Return         : None
*******************************************************************************/
void WritenDataVk1621(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt)
{
	unsigned char i,j; 
	unsigned char Shift;
	unsigned char dataval; 
	
	VK1621_CS_L();   //CS 片选开 
	delay_nus(VK1621_CLK/2);

	VK1621_DATA_H();  
	WriteClockVk1621();  //101
	VK1621_DATA_L();  
	WriteClockVk1621();
	VK1621_DATA_H();  
	WriteClockVk1621();

	Shift = 0x20;
	for( i = 0; i < vk1621_addrbit; i++) 
	{   		 
		if (Addr & Shift) 
			VK1621_DATA_H(); 		
		else  
			VK1621_DATA_L();
		WriteClockVk1621();		
		Shift = Shift >> 1; 
	}
	for (j = 0; j < Cnt; j++) 
	{
		Shift = 0x01;
		dataval=*Databuf++;
		for (i = 0; i < 4; i++) 
		{
			if( dataval & Shift) 
				VK1621_DATA_H();		 
			else  
				VK1621_DATA_L();
			WriteClockVk1621();
			Shift = Shift << 1;
		}   
	}
  VK1621_CS_H();   //CS 片选关
	delay_nus(VK1621_CLK/2);	 
  VK1621_DATA_H(); 
}
/*******************************************************************************
* Function Name  : Vk1621_DisAll
* Description    : 所有SEG显示同一个数据，bit3-COM3 bit2-COM2 bit1-COM1 bit0-COM0
* 					     : 例如：0x0f全亮 0x00全灭 0x05灭亮灭亮 0x0a亮灭亮灭 0x03灭灭亮亮 
* Input          ：dat->写入ram的数据(1个字节数据低4位有效)  
* Output         : None
* Return         : None
*******************************************************************************/
void Vk1621_DisAll(unsigned char dat)
{
	unsigned char segi;
	
	for(segi=0;segi<32;segi++)
	{
		vk1621_dispram[segi]=dat;
	}
	WritenDataVk1621(0,vk1621_dispram,32);//这里送4bit，地址自动加1
}
/*******************************************************************************
* Function Name  : DisSegComOn
* Description    : 点亮1个点(1个seg和1个com交叉对应的显示点)
* Input          ：seg->点对应的seg脚  
* 		           ：com->点对应com脚  
* Output         : None
* Return         : None
*******************************************************************************/
void Vk1621_DisDotOn(unsigned char seg,unsigned char com)
{
	unsigned char addrbyte,addrbit,tempdat;
	
	addrbyte=seg;
	addrbit=(1<<com);
	tempdat=vk1621_dispram[seg]|addrbit;
	vk1621_dispram[seg]=tempdat;
	WritenDataVk1621(addrbyte,&tempdat,1);
}
/*******************************************************************************
* Function Name  : DisSegComOff
* Description    : 关闭1个点(1个seg和1个com交叉对应的显示点)
* Input          ：seg->点对应的seg脚  
* 		           ：com->点对应com脚  
* Output         : None
* Return         : None
*******************************************************************************/
void Vk1621_DisDotOff(unsigned char seg,unsigned char com)
{
	unsigned char addrbyte,addrbit,tempdat;
	
	addrbyte=seg;
	addrbit=(1<<com);
	tempdat=vk1621_dispram[seg]&(~addrbit);
	vk1621_dispram[seg]=tempdat;
	WritenDataVk1621(addrbyte,&tempdat,1);	
}
/*******************************************************************************
* Function Name  : Enter_Standby
* Description    : 进入掉电低功耗模式
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void Vk1621_Enter_PowerOff(void)
{	
	//连续使用命令LCDOFF和SYSDIS,系统将处于掉电低功耗状态。
	//只有使用片内RC时钟源时,才能使系统进入掉电低功耗状态。
	WriteCommandVk1621(OSC_OFF);
	WriteCommandVk1621(DISP_OFF);
}
/*******************************************************************************
* Function Name  : Exit_Standby
* Description    : 退出掉电低功耗模式
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void Vk1621_Exit_PowerOff(void)
{	
	//退出掉电低功耗状态重新配置vk1621
	WriteCommandVk1621(OSC_ON);
	WriteCommandVk1621(DISP_ON);
	WriteCommandVk1621(COM_1_3__4);vk1621_maxcom=4;	//1/3bias 4com
//	WriteCommandVk1621(COM_1_3__3);vk1621_maxcom=3;	//1/3bias 3com
//	WriteCommandVk1621(COM_1_3__2);vk1621_maxcom=2;	//1/3bias 2com
//	WriteCommandVk1621(COM_1_2__4);vk1621_maxcom=4;	//1/2bias 4com
//	WriteCommandVk1621(COM_1_2__3);vk1621_maxcom=3;	//1/2bias 3com
//	WriteCommandVk1621(COM_1_2__2);vk1621_maxcom=2;	//1/2bias 2com
}
/*******************************************************************************
* Function Name  : Lowlevel_Init
* Description    : 配置通信线GPIO
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void Vk1621_Lowlevel_Init(void)
{
	//通信线电平不同，建议加电平转换电路
	//此函数根据客户单片机做相应的修改	
	GPIO_SetMode(VK1621_CS_PORT, VK1621_CS_PIN, GPIO_MODE_OUTPUT);
	GPIO_SetMode(VK1621_RD_PORT, VK1621_RD_PIN, GPIO_MODE_OUTPUT);
	GPIO_SetMode(VK1621_WR_PORT, VK1621_WR_PIN, GPIO_MODE_OUTPUT);
	GPIO_SetMode(VK1621_DAT_PORT, VK1621_DAT_PIN, GPIO_MODE_OUTPUT);	//只显示数据可设为输出IO
//	GPIO_SetMode(VK1621_DAT_PORT, VK1621_DAT_PIN, GPIO_MODE_QUASI); //读数据要设为双向IO或者在读数据时设为输入IO
		
	VK1621_CS_H();      
	VK1621_RD_H();  //RD脚不用需输出高电平                  
	VK1621_WR_H();  
	VK1621_DATA_H(); 	
}
/*******************************************************************************
* Function Name  : Init
* Description    : 初始化配置
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void Vk1621_Init(void)
{	
	//管脚配置根据客户单片机做相应的修改
	Vk1621_Lowlevel_Init();
	//基本配置
	WriteCommandVk1621(OSC_ON);
	WriteCommandVk1621(DISP_ON);
//	WriteCommandVk1621(RC32K); //片内RC振荡器
//	WriteCommandVk1621(XTAL32K); //外接晶振
//	WriteCommandVk1621(EXT256K); //外接时钟
	WriteCommandVk1621(COM_1_3__4);vk1621_maxcom=4;	//1/3bias 4com
//	WriteCommandVk1621(COM_1_3__3);vk1621_maxcom=3;	//1/3bias 3com com2/com1/com0
//	WriteCommandVk1621(COM_1_3__2);vk1621_maxcom=2;	//1/3bias 2com com1/com0
//	WriteCommandVk1621(COM_1_2__4);vk1621_maxcom=4;	//1/2bias 4com
//	WriteCommandVk1621(COM_1_2__3);vk1621_maxcom=3;	//1/2bias 3com com2/com1/com0
//	WriteCommandVk1621(COM_1_2__2);vk1621_maxcom=2;	//1/2bias 2com com1/com0
	//上电默认配置(以下未用功能关闭降低功耗)
//	WriteCommandVk1621(BUZZ_2KHZ);
//	WriteCommandVk1621(BUZZ_4KHZ);
//	WriteCommandVk1621(BUZZ_ON);
	WriteCommandVk1621(BUZZ_OFF);
	WriteCommandVk1621(IRQ_DIS);
	WriteCommandVk1621(TIMER_DIS);
	WriteCommandVk1621(WDT_DIS);
}
void disnum(unsigned char dat)
{
  vk1621_dispram[0]=num[dat]&0x0f;
	vk1621_dispram[1]=num[dat]>>4;
	
	vk1621_dispram[2]=num[dat]&0x0f;
	vk1621_dispram[3]=num[dat]>>4;
	
	vk1621_dispram[4]=num[dat]&0x0f;
	vk1621_dispram[5]=num[dat]>>4;
	
	vk1621_dispram[6]=num[dat]&0x0f;
	vk1621_dispram[7]=num[dat]>>4;
	
	vk1621_dispram[8]=num[dat]&0x0f;
	vk1621_dispram[9]=num[dat]>>4;
	
	vk1621_dispram[10]=num[dat]&0x0f;
	vk1621_dispram[11]=num[dat]>>4;

	WritenDataVk1621(0,vk1621_dispram,12);
}
/*******************************************************************************
* Function Name  : Write1Data
* Description    : 
* Input          : Addr->写入ram的起始地址  
* 							 : Databuf->写入ram的数据指针(每个字节数据低4位有效)  
* Output         : None
* Return         : None
*******************************************************************************/
void Write1DataVk1621(unsigned char Addr,unsigned char Dat)
{
	unsigned char i; 
	unsigned char Shift;
	
	Shift = 0x20; //屏蔽高3位 只用5位
	
	VK1621_CS_L();  //CS 片选开
	delay_nus(VK1621_CLK/2);

	VK1621_DATA_H();  
	WriteClockVk1621();  //101
	VK1621_DATA_L();  
	WriteClockVk1621();
	VK1621_DATA_H();  
	WriteClockVk1621();

	for( i = 0; i < vk1621_addrbit; i++) 
	{   		 
		if (Addr & Shift) 
			VK1621_DATA_H(); 		
		else  
			VK1621_DATA_L();
		WriteClockVk1621();		
		Shift = Shift >> 1; 
	}
  Shift = 0x01;
  for (i = 0; i < 4; i++) 
	{

		if( Dat & Shift) 
			VK1621_DATA_H();		 
   	else  
			VK1621_DATA_L();
		WriteClockVk1621();
		Shift = Shift << 1;
	}   
  VK1621_CS_H(); //CS 片选关
	delay_nus(VK1621_CLK/2);	
  VK1621_DATA_H(); 
}
/*******************************************************************************
* Function Name  : test_Main
* Description    : 测试主程序
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void Vk1621_Main(void)
{	
	int i,j;
	Vk1621_Init();
	Vk1621_DisAll(0x00);
	
	while(1)
	{
		
		Write1DataVk1621(0x02,0x01);
		delay_nms(1000);	
		
//		Vk1621_DisAll(0x0f);			//LCD全显
//		delay_nms(1000);					//延时3S
//		Vk1621_DisAll(0x00);			//LCD全关
//		delay_nms(1000);					//延时3S
//		
//		//LCD半显
//		Vk1621_DisAll(0x05);
//		delay_nms(1000);
//		Vk1621_DisAll(0x0A);
//		delay_nms(1000);
//		Vk1621_DisAll(0x00);
//				
//		Vk1621_DisAll(0x00);			//LCD全关
//		for(vk1621_segi=0;vk1621_segi<32;vk1621_segi++)//seg
//		{
//			for(vk1621_comi=0;vk1621_comi<vk1621_maxcom;vk1621_comi++)//com
//			{
//				Vk1621_DisDotOn(vk1621_segi,vk1621_comi);	//LCD单点点亮
//				delay_nms(50);				//延时300mS
//				Vk1621_DisDotOff(vk1621_segi,vk1621_comi);//LCD单点关闭
//			}
//		}
//		delay_nms(1000);
//		Vk1621_DisAll(0x00);
//		delay_nms(1000);

	}
}
/************************END OF FILE****/
