#include "User_Disp_Div.h"
#include "User_Disp_Font.h"
#include "User_Disp_HAL.h"

Disp_Management disp_management;

void DIV_Disp_ClearAllPoint(uint8_t screen)
{
	if(screen)
	{
	    disp_management.Disp_S1Point&=~(0xe);
	}
	else
	{
		   disp_management.Disp_S2Point&=~(0xe);
	}		 	
}

//取消设置小数点
void DIV_Disp_UnSetPoint(uint8_t screen ,uint8_t p)
{
	if(screen==MainScreen)
	{
	 		disp_management.Disp_S1Point&=~(p);

	}

	 if(screen==SecondScreen)
		{
				disp_management.Disp_S2Point&=~(p);
		}
		
		if(screen==Out1Screen)
		{
					disp_management.Disp_S1Point&=~(p);
		}
		
		if(screen==Out2Screen)
		{
					disp_management.Disp_S2Point&=~(p);
		}
}

//设置小数点
void DIV_Disp_SetPoint(uint8_t screen ,uint8_t p)
{
	if(screen==MainScreen)
	{
	 		disp_management.Disp_S1Point&=~(0x0e-p);
		disp_management.Disp_S1Point|=p;
	
	}

	 if(screen==SecondScreen)
		{
				disp_management.Disp_S2Point&=~(0x0e-p);
		disp_management.Disp_S2Point|=p;
		}
		
		if(screen==Out1Screen)
		{
				disp_management.Disp_S1Point|=p;
		}
		
		if(screen==Out2Screen)
		{
				disp_management.Disp_S2Point|=p;
		}
}

//显示浮点带0到3位小数的
void DIV_Disp_MultiplefloatNum(uint8_t screen,float num,uint8_t xbit)   //显示浮点数
{
	  char buffer[30]={0};
	
	  int length=0;
			
			float temp=fabsf(num);	
			
			switch(xbit)
			{
				case 0:{
				   length = snprintf(buffer, sizeof(buffer), "%.1f", num);  //转几位
					   buffer[length-1]='\0';
				}break;
				case 1:{
				  length = snprintf(buffer, sizeof(buffer), "%.2f", num);  //转几位
					   buffer[length-1]='\0';
				}break;
				case 2:{
				  length = snprintf(buffer, sizeof(buffer), "%.3f", num);  //转几位
					   buffer[length-1]='\0';
				}break;
				case 3:{
				  length = snprintf(buffer, sizeof(buffer), "%.4f", num);  //转几位
					  buffer[length-1]='\0';
				}break;
				default:{length=0;}break; 
			}     
				
			 if (length < 0 || length >= sizeof(buffer)) {
        return;
    }
			
			if(length<sizeof(buffer))
			{
				switch(xbit)
				{
					case 0:{
					  if(num>=0)
						{
							 if((num-0)<1e-6f)
								{
								   DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
									   if(screen)
												{
													DIV_Disp_Num(NIXIE1,NUMNULL);
													 DIV_Disp_Num(NIXIE2,NUMNULL);
													 DIV_Disp_Num(NIXIE3,NUMNULL);
												}
												else
												{
														DIV_Disp_Num(NIXIE8,NUMNULL);
														 DIV_Disp_Num(NIXIE7,NUMNULL);
													  DIV_Disp_Num(NIXIE6,NUMNULL);
												}
								}
								else if(num>0&&num<10.0)
								{
										  DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
									   if(screen)
												{
													DIV_Disp_Num(NIXIE1,NUMNULL);
													 DIV_Disp_Num(NIXIE2,NUMNULL);
													DIV_Disp_Num(NIXIE3,NUMNULL);
												}
												else
												{
														DIV_Disp_Num(NIXIE8,NUMNULL);
														 DIV_Disp_Num(NIXIE7,NUMNULL);
													DIV_Disp_Num(NIXIE6,NUMNULL);
												}
								}
								else if(num>=10.0&&num<100)
								{
											 DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数 
											if(screen)
												{
													DIV_Disp_Num(NIXIE1,NUMNULL);
													DIV_Disp_Num(NIXIE2,NUMNULL);
												}
												else
												{
														DIV_Disp_Num(NIXIE8,NUMNULL);
													DIV_Disp_Num(NIXIE7,NUMNULL);
												}
								}
								else if(num>=100&&num<1000)
								{
									    DIV_Disp_Num(NIXIE1,NUMNULL);
											  DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
								}																				
									else if(num>=1000&&num<1050)
								{      
									    DIV_Disp_Num(NIXIE8,NUMNULL);
											  DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
								}	
						}
						else
						{	
							if((temp-0)<1e-1f)
								{
								   DIV_Disp_FloatByString(screen,buffer+1); //可以显示最多三位小数
									   if(screen)
												{
													DIV_Disp_Symbol(NIXIE1,Symbol_minus);
													 DIV_Disp_Num(NIXIE2,NUMNULL);
													 DIV_Disp_Num(NIXIE3,NUMNULL);
												}
												else
												{
														DIV_Disp_Symbol(NIXIE8,Symbol_minus);
														 DIV_Disp_Num(NIXIE7,NUMNULL);
													  DIV_Disp_Num(NIXIE6,NUMNULL);
												}
								}
								else if(temp>0&&temp<10)
								{
										  DIV_Disp_FloatByString(screen,buffer+1); //可以显示最多三位小数
									    if(screen)
												{
															DIV_Disp_Num(NIXIE2,NUMNULL);
													 	DIV_Disp_Symbol(NIXIE1,Symbol_minus);
													  DIV_Disp_Num(NIXIE3,NUMNULL);
												}
												else
												{
														 DIV_Disp_Num(NIXIE7,NUMNULL);
													DIV_Disp_Symbol(NIXIE8,Symbol_minus);
													DIV_Disp_Num(NIXIE6,NUMNULL);
												}
									   
								}
								else if(temp>=10&&temp<100)
								{
											 DIV_Disp_FloatByString(screen,buffer+1); //可以显示最多三位小数
									   if(screen)
												{
													 	DIV_Disp_Symbol(NIXIE1,Symbol_minus);
												}
												else
												{
													DIV_Disp_Symbol(NIXIE8,Symbol_minus);
												}
								}
								else if(temp>=100&&temp<1000)
								{
											  DIV_Disp_FloatByString(screen,buffer+2); //可以显示最多三位小数
												if(screen)
												{
															DIV_Disp_Symbol(NIXIE1,Symbol_Negativeone);
												}
												else
												{
															DIV_Disp_Symbol(NIXIE8,Symbol_Negativeone);
												}
									    
								}																				
									else if(temp>=1000&&temp<=1050)
								{
											  DIV_Disp_FloatByString(screen,buffer+2); //可以显示最多三位小数
									    if(screen)
												{
																DIV_Disp_Num(NIXIE2,NUM0);
															DIV_Disp_Symbol(NIXIE1,Symbol_Negativeone);
												}
												else
												{
															DIV_Disp_Num(NIXIE7,NUM0);
													DIV_Disp_Symbol(NIXIE8,Symbol_Negativeone);
												}					
								}		
						}
					}break;
					case 1:{    //正常显示
					  if(num>=0)
						{
							 if((num-0)<1e-6f)
								{
								   DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
									   if(screen)
												{
													DIV_Disp_Num(NIXIE1,NUMNULL);
													 DIV_Disp_Num(NIXIE2,NUMNULL);
												}
												else
												{
														DIV_Disp_Num(NIXIE8,NUMNULL);
														 DIV_Disp_Num(NIXIE7,NUMNULL);
												}
								}
								else if(num>0&&num<10.0)
								{
										  DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
									   if(screen)
												{
													DIV_Disp_Num(NIXIE1,NUMNULL);
													 DIV_Disp_Num(NIXIE2,NUMNULL);
												}
												else
												{
														DIV_Disp_Num(NIXIE8,NUMNULL);
														 DIV_Disp_Num(NIXIE7,NUMNULL);
												}
								}
								else if(num>=10.0&&num<100)
								{
											 DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数 
											if(screen)
												{
													DIV_Disp_Num(NIXIE1,NUMNULL);
												}
												else
												{
														DIV_Disp_Num(NIXIE8,NUMNULL);
												}
								}
								else if(num>=100&&num<1000)
								{
											  DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
								}																				
									else if(num>=1000&&num<1050)
								{
											  DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
								}	
						}
						else
						{	
							if((temp-0)<1e-6f)
								{
								   DIV_Disp_FloatByString(screen,buffer+1); //可以显示最多三位小数
									   if(screen)
												{
													DIV_Disp_Num(NIXIE1,Symbol_minus);
													 DIV_Disp_Num(NIXIE2,NUMNULL);
												}
												else
												{
														DIV_Disp_Num(NIXIE8,Symbol_minus);
														 DIV_Disp_Num(NIXIE7,NUMNULL);
												}
								}
								else if(temp>0&&temp<10)
								{
										  DIV_Disp_FloatByString(screen,buffer+1); //可以显示最多三位小数
									    if(screen)
												{
													 DIV_Disp_Num(NIXIE2,NUMNULL);
													 	DIV_Disp_Symbol(NIXIE1,Symbol_minus);
												}
												else
												{
														 DIV_Disp_Num(NIXIE7,NUMNULL);
													DIV_Disp_Symbol(NIXIE8,Symbol_minus);
												}
									   
								}
								else if(temp>=10&&temp<100)
								{
											 DIV_Disp_FloatByString(screen,buffer+1); //可以显示最多三位小数
									   if(screen)
												{
													 	DIV_Disp_Symbol(NIXIE1,Symbol_minus);
												}
												else
												{
													DIV_Disp_Symbol(NIXIE8,Symbol_minus);
												}
								}
								else if(temp>=100&&temp<1000)
								{
											  DIV_Disp_FloatByString(screen,buffer+2); //可以显示最多三位小数
												if(screen)
												{
															DIV_Disp_Symbol(NIXIE1,Symbol_Negativeone);
												}
												else
												{
															DIV_Disp_Symbol(NIXIE8,Symbol_Negativeone);
												}
									    
								}																				
									else if(temp>=1000&&temp<=1050)
								{
											  DIV_Disp_FloatByString(screen,buffer+2); //可以显示最多三位小数
									    if(screen)
												{
																DIV_Disp_Num(NIXIE2,NUM0);
															DIV_Disp_Symbol(NIXIE1,Symbol_Negativeone);
												}
												else
												{
															DIV_Disp_Num(NIXIE7,NUM0);
													DIV_Disp_Symbol(NIXIE8,Symbol_Negativeone);
												}					
								}		
						}
					}break;
					case 2:{    //正常显示
						
						DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
					 if(num>=0)
						{
							 if((num-0)<1e-6f)
								{
								   DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
									   if(screen)
												{
													DIV_Disp_Num(NIXIE1,NUMNULL);
													 DIV_Disp_Num(NIXIE2,NUMNULL);
												}
												else
												{
														DIV_Disp_Num(NIXIE8,NUMNULL);
														 DIV_Disp_Num(NIXIE7,NUMNULL);
												}
								}
								else if(num>0&&num<10.0)
								{
										  DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
									   if(screen)
												{
													DIV_Disp_Num(NIXIE1,NUMNULL);
												}
												else
												{
														DIV_Disp_Num(NIXIE8,NUMNULL);
												}
								}
								else if(num>=10.0&&num<100)
								{
											 DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数 
											if(screen)
												{
													DIV_Disp_Num(NIXIE1,NUMNULL);
												}
												else
												{
														DIV_Disp_Num(NIXIE8,NUMNULL);
												}
								}
								else if(num>=100&&num<1000)
								{
											  DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
								}																				
									else if(num>=1000&&num<1050)
								{
											  DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
								}	
						}
						else
						{	
							if((temp-0)<1e-1f)
								{
								   DIV_Disp_FloatByString(screen,buffer+1); //可以显示最多三位小数
									   if(screen)
												{
													DIV_Disp_Num(NIXIE1,NUMNULL);
													 DIV_Disp_Num(NIXIE2,NUMNULL);
												}
												else
												{
														DIV_Disp_Num(NIXIE8,NUMNULL);
														 DIV_Disp_Num(NIXIE7,NUMNULL);
												}
								}
								else if(temp>0&&temp<10)
								{
										  DIV_Disp_FloatByString(screen,buffer+1); //可以显示最多三位小数
									    if(screen)
												{
													 	DIV_Disp_Symbol(NIXIE1,Symbol_minus);
												}
												else
												{
													DIV_Disp_Symbol(NIXIE8,Symbol_minus);
												}
									   
								}
								else if(temp>=10&&temp<100)
								{
											 DIV_Disp_FloatByString(screen,buffer+1); //可以显示最多三位小数
									   if(screen)
												{
													 	DIV_Disp_Symbol(NIXIE1,Symbol_minus);
												}
												else
												{
													DIV_Disp_Symbol(NIXIE8,Symbol_minus);
												}
								}
								else if(temp>=100&&temp<1000)
								{
											  DIV_Disp_FloatByString(screen,buffer+2); //可以显示最多三位小数
												if(screen)
												{
															DIV_Disp_Symbol(NIXIE1,Symbol_Negativeone);
												}
												else
												{
															DIV_Disp_Symbol(NIXIE8,Symbol_Negativeone);
												}
									    
								}																				
									else if(temp>=1000&&temp<=1050)
								{
											  DIV_Disp_FloatByString(screen,buffer+2); //可以显示最多三位小数
									    if(screen)
												{
																DIV_Disp_Num(NIXIE2,NUM0);
															DIV_Disp_Symbol(NIXIE1,Symbol_Negativeone);
												}
												else
												{
															DIV_Disp_Num(NIXIE7,NUM0);
													DIV_Disp_Symbol(NIXIE8,Symbol_Negativeone);
												}					
								}		
						}
			
					}break;
					case 3:{    //1000以后没有考虑
					 	  if(num>=0)
						{
							 if((num-0)<1e-6f)
								{
								   DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
											if(screen)
											{
											    	DIV_Disp_Num(NIXIE1,NUMNULL);
											}
											else
											{
											   	DIV_Disp_Num(NIXIE8,NUMNULL);
											}
									  
								}
								else if(num>0&&num<10.0)
								{
										  DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
									  if(screen)
											{
											    	DIV_Disp_Num(NIXIE1,NUMNULL);
											}
											else
											{
											   	DIV_Disp_Num(NIXIE8,NUMNULL);
											}
									  
								}
								else if(num>=10.0&&num<100)
								{
											 DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数 
									    if(screen)
											{
											    	DIV_Disp_Num(NIXIE1,NUMNULL);
											}
											else
											{
											   	DIV_Disp_Num(NIXIE8,NUMNULL);
											}
								}
								else if(num>=100&&num<1000)
								{
											  DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
									     if(screen)
											{
											    	DIV_Disp_Num(NIXIE1,NUMNULL);
											}
											else
											{
											   	DIV_Disp_Num(NIXIE8,NUMNULL);
											}
								}																				
									else if(num>=1000&&num<1050)
								{
											  DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
								}	
						}
						else
						{	
							if((temp-0)<1e-1f)
								{
								   DIV_Disp_FloatByString(screen,buffer+1); //可以显示最多三位小数
									   		if(screen)
											{
											    	DIV_Disp_Num(NIXIE1,NUMNULL);
											}
											else
											{
											   	DIV_Disp_Num(NIXIE8,NUMNULL);
											}
								}
								else if(temp>0&&temp<10)
								{
										  DIV_Disp_FloatByString(screen,buffer+1); //可以显示最多三位小数
									    if(screen)
												{
													 	DIV_Disp_Symbol(NIXIE1,Symbol_minus);
												}
												else
												{
													DIV_Disp_Symbol(NIXIE8,Symbol_minus);
												}
									   
								}
								else if(temp>=10&&temp<100)
								{
											 DIV_Disp_FloatByString(screen,buffer+1); //可以显示最多三位小数
									   if(screen)
												{
													 	DIV_Disp_Symbol(NIXIE1,Symbol_minus);
												}
												else
												{
													DIV_Disp_Symbol(NIXIE8,Symbol_minus);
												}
								}
								else if(temp>=100&&temp<1000)
								{
											  DIV_Disp_FloatByString(screen,buffer+2); //可以显示最多三位小数
												if(screen)
												{
															DIV_Disp_Symbol(NIXIE1,Symbol_Negativeone);
												}
												else
												{
															DIV_Disp_Symbol(NIXIE8,Symbol_Negativeone);
												}
									    
								}																				
									else if(temp>=1000&&temp<=1050)
								{
											  DIV_Disp_FloatByString(screen,buffer+2); //可以显示最多三位小数
									    if(screen)
												{
																DIV_Disp_Num(NIXIE2,NUM0);
															DIV_Disp_Symbol(NIXIE1,Symbol_Negativeone);
												}
												else
												{
															DIV_Disp_Num(NIXIE7,NUM0);
													DIV_Disp_Symbol(NIXIE8,Symbol_Negativeone);
												}					
								}		
						}			
					}break;
				}
				
				}
	
}


//把字符串从左往右依次显示在屏幕上字符占一位，无则不显示
void DIV_Disp_Snprintf(int screen,const char * data1,...)
{  
	  char data[9]={0};
			int size = sizeof(data);
   va_list ps;
			va_start(ps,data1);
	  vsnprintf(data,size,data1,ps);
			DIV_Disp_ByString(screen,data);
			va_end(ps);   
}

//显示浮点带一位小数的
void DIV_Disp_floatNum(uint8_t screen,float num)   //显示浮点数
{
	  char buffer[10];
	  int length = snprintf(buffer, sizeof(buffer), "%.1f", num);  //转几位
			
			if(length<10)
			{
        DIV_Disp_FloatByString(screen,buffer); //可以显示最多三位小数
			}
	
}
//显示字符串
void DIV_Disp_ByString(int mainscreen,char * string)
{ 
	uint16_t sz;
	uint16_t length; 
	int temp;
	int i;     

 sz=0;
	length=4;
	
		  for(i=sz;i<length;i++)
			{  
				temp= i;
				if(!mainscreen)
					{
							if(i==0){temp=7;}if(i==1){temp=6;}if(i==2){temp=5;}if(i==3){temp=4;}	
					}
						
					if((string[i]>='a'&& string[i]<='z')||(string[i]>='A'&& string[i]<='Z'))
					{
						switch(string[i]){
								case 'a':{DIV_Disp_Alphabet(temp,Alphabet_a);}break;
								case 'b':{DIV_Disp_Alphabet(temp,Alphabet_b);}break;
								case 'c':{DIV_Disp_Alphabet(temp,Alphabet_c);}break;
								case 'd':{DIV_Disp_Alphabet(temp,Alphabet_d);}break;
								case 'e':{DIV_Disp_Alphabet(temp,Alphabet_e);}break;
								case 'f':{DIV_Disp_Alphabet(temp,Alphabet_f);}break;
								case 'g':{DIV_Disp_Alphabet(temp,Alphabet_g);}break;
								case 'h':{DIV_Disp_Alphabet(temp,Alphabet_h);}break;
								case 'i':{DIV_Disp_Alphabet(temp,Alphabet_i);}break;
								case 'j':{DIV_Disp_Alphabet(temp,Alphabet_j);}break;
								case 'k':{DIV_Disp_Alphabet(temp,Alphabet_k);}break;
								case 'l':{DIV_Disp_Alphabet(temp,Alphabet_l);}break;
								case 'm':{DIV_Disp_Alphabet(temp,Alphabet_m);}break;
								case 'n':{DIV_Disp_Alphabet(temp,Alphabet_n);}break;
								case 'o':{DIV_Disp_Alphabet(temp,Alphabet_o);}break;
								case 'p':{DIV_Disp_Alphabet(temp,Alphabet_p);}break;
								case 'q':{DIV_Disp_Alphabet(temp,Alphabet_q);}break;
								case 'r':{DIV_Disp_Alphabet(temp,Alphabet_r);}break;
								case 's':{DIV_Disp_Alphabet(temp,Alphabet_s);}break;
								case 't':{DIV_Disp_Alphabet(temp,Alphabet_t);}break;
								case 'u':{DIV_Disp_Alphabet(temp,Alphabet_u);}break;
								case 'v':{DIV_Disp_Alphabet(temp,Alphabet_v);}break;
								case 'w':{DIV_Disp_Alphabet(temp,Alphabet_w);}break;
								case 'x':{DIV_Disp_Alphabet(temp,Alphabet_x);}break;
								case 'y':{DIV_Disp_Alphabet(temp,Alphabet_y);}break;
								case 'z':{DIV_Disp_Alphabet(temp,Alphabet_z);}break;
								case 'A':{DIV_Disp_Alphabet(temp,Alphabet_A);}break;
								case 'B':{DIV_Disp_Alphabet(temp,Alphabet_B);}break;
								case 'C':{DIV_Disp_Alphabet(temp,Alphabet_C);}break;
								case 'D':{DIV_Disp_Alphabet(temp,Alphabet_D);}break;
								case 'E':{DIV_Disp_Alphabet(temp,Alphabet_E);}break;
								case 'F':{DIV_Disp_Alphabet(temp,Alphabet_F);}break;
								case 'G':{DIV_Disp_Alphabet(temp,Alphabet_G);}break;
								case 'H':{DIV_Disp_Alphabet(temp,Alphabet_H);}break;
								case 'I':{DIV_Disp_Alphabet(temp,Alphabet_I);}break;
								case 'J':{DIV_Disp_Alphabet(temp,Alphabet_J);}break;
								case 'K':{DIV_Disp_Alphabet(temp,Alphabet_K);}break;
								case 'L':{DIV_Disp_Alphabet(temp,Alphabet_L);}break;
								case 'M':{DIV_Disp_Alphabet(temp,Alphabet_M);}break;
								case 'N':{DIV_Disp_Alphabet(temp,Alphabet_N);}break;
								case 'O':{DIV_Disp_Alphabet(temp,Alphabet_O);}break;
								case 'P':{DIV_Disp_Alphabet(temp,Alphabet_P);}break;
								case 'Q':{DIV_Disp_Alphabet(temp,Alphabet_Q);}break;
								case 'R':{DIV_Disp_Alphabet(temp,Alphabet_R);}break;
								case 'S':{DIV_Disp_Alphabet(temp,Alphabet_S);}break;
								case 'T':{DIV_Disp_Alphabet(temp,Alphabet_T);}break;
								case 'U':{DIV_Disp_Alphabet(temp,Alphabet_U);}break;
								case 'V':{DIV_Disp_Alphabet(temp,Alphabet_V);}break;
								case 'W':{DIV_Disp_Alphabet(temp,Alphabet_W);}break;
								case 'X':{DIV_Disp_Alphabet(temp,Alphabet_X);}break;
								case 'Y':{DIV_Disp_Alphabet(temp,Alphabet_Y);}break;
								case 'Z':{DIV_Disp_Alphabet(temp,Alphabet_Z);}break;}	
					}
					else if(string[i]>='0' && string[i]<='9')
					{
					    switch(string[i])
									{
										case '0':{DIV_Disp_Num(temp,NUM0);}break;
										case '1':{DIV_Disp_Num(temp,NUM1);}break;
										case '2':{DIV_Disp_Num(temp,NUM2);}break;
										case '3':{DIV_Disp_Num(temp,NUM3);}break;
										case '4':{DIV_Disp_Num(temp,NUM4);}break;
										case '5':{DIV_Disp_Num(temp,NUM5);}break;
										case '6':{DIV_Disp_Num(temp,NUM6);}break;
										case '7':{DIV_Disp_Num(temp,NUM7);}break;
										case '8':{DIV_Disp_Num(temp,NUM8);}break;
										case '9':{DIV_Disp_Num(temp,NUM9);}break;
									} 
					}
					else if(string[i]=='-'||string[i]=='|'||string[i]=='/')
					{
					    switch(string[i])
									{
										case '-':{DIV_Disp_Symbol(temp,Symbol_minus);}break;
										case '|':{DIV_Disp_Symbol(temp,Symbol_saw);}break;
										case '/':{DIV_Disp_Symbol(temp,Symbol_slash);}break;
									} 
					}
				else{DIV_Disp_Num(temp,NUMNULL);}
			}		
}


//显示浮点字符串，最多三个小数,长度四位 空位不显示，需要清除  范围-1050~1050
void DIV_Disp_FloatByString(uint8_t screen ,char * string)
{	
	int len = strlen(string);
	
	if (len >= 20) return; // 或报错处理
	
	
	char date2[10]={0};
	
	char date3[10]={0};
	
	int num = 0;
	int count1 = 0;
	
	int num1 = 0;
	int count = 0;
		
	uint8_t head2=0;     //不要逗号初始化然后第一个不写初始值
	uint8_t head3=0;
	
	int ader =0;
	
 int i;
	
	for(i=0;i<len;i++)
	{
			  if(string[i]=='.')
					{
							ader = i;
					}
	}
	
	if(ader<len)
	{
		  	for(i=0;i<ader;i++)
					{
								date2[i]=string[i];
								head2++;
					}  
					date2[head2]='\0';
					
					for(i=ader+1;string[i]!='\0';i++)
					{  
								date3[head3]=string[i];
								head3++;
					}
					date3[head3]='\0';
	}
	
	num = atoi(date2);
	count1 = strlen(date2);
	
	num1 = atoi(date3);
	count = strlen(date3);
	
	
	if(screen)
	{
	  	if(count==3){
	//DIV_Disp_Num(NIXIE1,num%10);	
					
					//配合特殊显示
	if(num==0)
	{
	   DIV_Disp_Num(NIXIE1,NUMNULL);
	}
	else
	{
			DIV_Disp_Num(NIXIE1,num%10);
	}
		
	//DIV_Disp_Symbol(NIXIE1,Symbol_point);
	DIV_Disp_SetPoint(MainScreen,P1);
	
	DIV_Disp_Num(NIXIE2,num1/100%10);
	DIV_Disp_Num(NIXIE3,num1/10%10);
	DIV_Disp_Num(NIXIE4,num1%10);
	}
	else if(count==2){
		if(count1==1)
		{		
					//DIV_Disp_Num(NIXIE2,num%10);
			//配合特殊显示
	if(num==0)
	{
	   DIV_Disp_Num(NIXIE2,NUMNULL);
	}
	else
	{
			DIV_Disp_Num(NIXIE2,num%10);
	}
			
		}else if(count1==2){
//				DIV_Disp_Num(NIXIE1,num/10%10);
//				DIV_Disp_Num(NIXIE2,num%10);
			
			//配合特殊显示
			if(num==0)
			{
							DIV_Disp_Num(NIXIE1,NUMNULL);
				DIV_Disp_Num(NIXIE2,NUMNULL);
			}
			else
			{
						DIV_Disp_Num(NIXIE1,num/10%10);
				DIV_Disp_Num(NIXIE2,num%10);
			}
		}
			
		DIV_Disp_Num(NIXIE3,num1/10%10);
		DIV_Disp_Num(NIXIE4,num1%10);
		//DIV_Disp_Symbol(NIXIE2,Symbol_point);
		DIV_Disp_SetPoint(MainScreen,P2);
	}
	else	if(count==1){
			if(count1==3)
		{		
				DIV_Disp_Num(NIXIE1,num/100%10);
				DIV_Disp_Num(NIXIE2,num/10%10);
				DIV_Disp_Num(NIXIE3,num%10);
		}else if(count1==2){
				DIV_Disp_Num(NIXIE2,num/10%10);
				DIV_Disp_Num(NIXIE3,num%10);
		} else if(count1==1)
		{
			 	DIV_Disp_Num(NIXIE3,num%10);
		}	
		
	 DIV_Disp_Num(NIXIE4,num1%10);
	//DIV_Disp_Symbol(NIXIE3,Symbol_point);
		DIV_Disp_SetPoint(MainScreen,P3);
	}
else if(count==0)   //没有小数
{
	 DIV_Disp_Num(NIXIE1,num/1000%10);
				DIV_Disp_Num(NIXIE2,num/100%10);
				DIV_Disp_Num(NIXIE3,num/10%10);
	  	DIV_Disp_Num(NIXIE4,num%10);
}	
	
	}
	else
	{
		if(count==3){ //小数个数
	//DIV_Disp_Num(NIXIE8,num%10);	
			
	//配合特殊显示
	if(num==0)
	{
	   DIV_Disp_Num(NIXIE8,NUMNULL);
	}
	else
	{
			DIV_Disp_Num(NIXIE8,num%10);
	}
		
	//DIV_Disp_Symbol(NIXIE8,Symbol_point);
	DIV_Disp_SetPoint(SecondScreen,P8);
	DIV_Disp_Num(NIXIE7,num1/100%10);
	DIV_Disp_Num(NIXIE6,num1/10%10);
	DIV_Disp_Num(NIXIE5,num1%10);
	}
	else if(count==2){//两位小数
		if(count1==1)  //一位整数
		{		
					//DIV_Disp_Num(NIXIE7,num%10);
			  //配合特殊显示
	if(num==0)
	{
	   DIV_Disp_Num(NIXIE7,NUMNULL);
	}
	else
	{
			DIV_Disp_Num(NIXIE7,num%10);
	}
			
		}else if(count1==2){
//				DIV_Disp_Num(NIXIE8,num/10%10);
//				DIV_Disp_Num(NIXIE7,num%10);
			
			//配合特殊显示
			if(num==0)
			{
							DIV_Disp_Num(NIXIE1,NUMNULL);
				DIV_Disp_Num(NIXIE2,NUMNULL);
			}
			else
			{
						DIV_Disp_Num(NIXIE1,num/10%10);
				DIV_Disp_Num(NIXIE2,num%10);
			}
		}
		//二三位整数	
		DIV_Disp_Num(NIXIE6,num1/10%10);
		DIV_Disp_Num(NIXIE5,num1%10);
	//	DIV_Disp_Symbol(NIXIE7,Symbol_point);
		DIV_Disp_SetPoint(SecondScreen,P7);
	}
	else	if(count==1){
			if(count1==3)
		{		
				DIV_Disp_Num(NIXIE8,num/100%10);
				DIV_Disp_Num(NIXIE7,num/10%10);
				DIV_Disp_Num(NIXIE6,num%10);
		}else if(count1==2){
				DIV_Disp_Num(NIXIE7,num/10%10);
				DIV_Disp_Num(NIXIE6,num%10);
		} else if(count1==1)
		{
			 	DIV_Disp_Num(NIXIE6,num%10);
		}	
		
	 DIV_Disp_Num(NIXIE5,num1%10);
//	DIV_Disp_Symbol(NIXIE6,Symbol_point);
		DIV_Disp_SetPoint(SecondScreen,P6);
	}else if(count==0)   //没有小数
{
	 //DIV_Disp_Num(NIXIE8,num/1000%10);   //负数最高位是个特殊符号不显示
				DIV_Disp_Num(NIXIE7,num/100%10);
				DIV_Disp_Num(NIXIE6,num/10%10);
	  	DIV_Disp_Num(NIXIE5,num%10);
}	
}
}


//显示整数
void DIV_Disp_Uint16Num(uint8_t screen,uint16_t num)
{
	if(screen==MainScreen)
	{
		if(num<10)
		{
						DIV_Disp_Num(NIXIE4,num%10);
		}
		if(num>=10&&num<100)
		{
						DIV_Disp_Num(NIXIE3,num/10%10);
						DIV_Disp_Num(NIXIE4,num%10);
		}
		if(num>=100&&num<1000)
		{
		    DIV_Disp_Num(NIXIE2,num/100%10);
		    DIV_Disp_Num(NIXIE3,num/10%10);
		    DIV_Disp_Num(NIXIE4,num%10);
		}
		if(num>=1000&&num<=9999)
		{
						DIV_Disp_Num(NIXIE1,num/1000%10);
			   DIV_Disp_Num(NIXIE2,num/100%10);
		    DIV_Disp_Num(NIXIE3,num/10%10);
		    DIV_Disp_Num(NIXIE4,num%10);
		}	
	}

		HAL_Disp_W_Data(dataid,0x15,0x00);
		HAL_Disp_W_Data(dataid,0x16,0x00);
}



//显示符号
void DIV_Disp_Symbol(uint8_t nixie,uint8_t symbol)
{
	uint8_t address1,address2,address3;
	uint8_t data1,data2,data3;
	uint8_t pointdat1=0,pointdat2=0;
		switch(symbol)
		{
			case Symbol_slash:{
			     data1 = 0x4;
				    data2 = 0x0;
				    data3 = 0x2;
			}break;
			case Symbol_minus:{
								data1 = 0x0;
				    data2 = 0x6;
				    data3 = 0x0;
			}break;
			case Symbol_saw:{
			     data1 = 0x3;
				    data2 = 0x0;
				    data3 = 0x0;
			}break;
			case Symbol_point:{
				  
			}break;	
			case Symbol_Negativeone:{
			     data1 = 0x0;
				    data2 = 0x6;
				    data3 = 0xc;
			}break;
			case Symbol_x:{
							 data1 = 0x4;
				    data2 = 0x9;
				    data3 = 0x2;
			}break;
		}		
		switch(nixie)
		{
			case NIXIE1:{
				address1  = 0x14;
				address2  = 0x13;
				address3  = 0x12;
				pointdat1 = 0x08;
			}break;
			case NIXIE2:{
				address1  = 0x11;
				address2  = 0x10;
				address3  = 0x0f;
				pointdat1 = 0x04;
			}break;
			case NIXIE3:{
				address1  = 0x0e;
				address2  = 0x0d;
				address3  = 0x0c;
				pointdat1 = 0x02;
			}break;
			case NIXIE4:{
				address1  = 0x0b;
				address2  = 0x0a;
				address3  = 0x09;
			}break;
			case NIXIE5:{
				address1  = 0x06;
				address2  = 0x07;
				address3  = 0x08;
			}break;
			case NIXIE6:{
				address1  = 0x03;
				address2  = 0x04;
				address3  = 0x05;
				pointdat2 = 0x02;
			}break;
			case NIXIE7:{
				address1  = 0x00;
				address2  = 0x01;
				address3  = 0x02;
				pointdat2 = 0x04;
			}break;
			case NIXIE8:{
				address1  = 0x17;
				address2  = 0x18;
				address3  = 0x19;
				pointdat2 = 0x08;
			}break;
			
		}
		if(symbol==Symbol_point)
		{
			if(nixie<4){	
			HAL_Disp_W_Data(dataid,0x15,pointdat1);
			}
			 else {
				  				HAL_Disp_W_Data(dataid,0x16,pointdat2);

				}
		}
		else
		{
			 HAL_Disp_W_Data(dataid,address1,data1);
				HAL_Disp_W_Data(dataid,address2,data2);	
				HAL_Disp_W_Data(dataid,address3,data3);
		}
}
//nixie参数可以是 NIXIE1  alphabet参数可以是 Alphabet_A
void DIV_Disp_Alphabet(uint8_t nixie,uint8_t alphabet)
{
	  uint8_t address1,address2,address3;
	uint8_t data1,data2,data3;
		switch(alphabet)
		{
			case Alphabet_A:{data1 = 0x3;data2 = 0x6;data3 = 0xd;}break;
			case Alphabet_B:{data1 = 0xd;data2 = 0x2;data3 = 0xd;}break;
			case Alphabet_C:{data1 = 0xb;data2 = 0x0;data3 = 0x1;}break;
			case Alphabet_D:{data1 = 0xc;data2 = 0x3;data3 = 0xd;}break;
			case Alphabet_E:{data1 = 0xb;data2 = 0x6;data3 = 0x1;}break;
			case Alphabet_F:{data1 = 0x3;data2 = 0x6;data3 = 0x1;}break;
			case Alphabet_G:{data1 = 0xb;data2 = 0x4;data3 = 0x9;}break;
			case Alphabet_H:{data1 = 0x3;data2 = 0x6;data3 = 0xc;}break;
			case Alphabet_I:{data1 = 0x3;data2 = 0x0;data3 = 0x0;}break;
			case Alphabet_J:{data1 = 0xa;data2 = 0x0;data3 = 0xc;}break;
			case Alphabet_K:{data1 = 0x3;data2 = 0xa;data3 = 0x2;}break;
			case Alphabet_L:{data1 = 0xb;data2 = 0x0;data3 = 0x0;}break;
			case Alphabet_M:{data1 = 0x3;data2 = 0x1;data3 = 0xe;}break;
			case Alphabet_N:{data1 = 0x3;data2 = 0x9;data3 = 0xc;}break;
			case Alphabet_O:{data1 = 0xb;data2 = 0x0;data3 = 0xd;}break;
			case Alphabet_P:{data1 = 0x3;data2 = 0x6;data3 = 0x5;}break;
			case Alphabet_Q:{data1 = 0xb;data2 = 0x0;data3 = 0xd;}break;
			case Alphabet_R:{data1 = 0x3;data2 = 0xe;data3 = 0x5;}break;
			case Alphabet_S:{data1 = 0x9;data2 = 0x6;data3 = 0x9;}break;
			case Alphabet_T:{data1 = 0x8;data2 = 0x6;data3 = 0xc;}break;
			case Alphabet_U:{data1 = 0xb;data2 = 0x0;data3 = 0xc;}break;
			case Alphabet_V:{data1 = 0xb;data2 = 0x0;data3 = 0xc;}break;
			case Alphabet_W:{data1 = 0x7;data2 = 0x8;data3 = 0xc;}break;
			case Alphabet_X:{data1 = 0x4;data2 = 0x9;data3 = 0x2;}break;
			case Alphabet_Y:{data1 = 0x9;data2 = 0x6;data3 = 0xc;}break;
			case Alphabet_Z:{data1 = 0xc;data2 = 0x0;data3 = 0x3;}break;
			case Alphabet_a:{data1 = 0x3;data2 = 0x6;data3 = 0xd;}break;
			case Alphabet_b:{data1 = 0xb;data2 = 0x6;data3 = 0x8;}break;
			case Alphabet_c:{data1 = 0xa;data2 = 0x6;data3 = 0x0;}break;
			case Alphabet_d:{data1 = 0xa;data2 = 0x6;data3 = 0xc;}break;
			case Alphabet_e:{data1 = 0xb;data2 = 0x6;data3 = 0x1;}break;
			case Alphabet_f:{data1 = 0x3;data2 = 0x6;data3 = 0x1;}break;
			case Alphabet_g:{data1 = 0x9;data2 = 0x6;data3 = 0xd;}break;
			case Alphabet_h:{data1 = 0x3;data2 = 0x6;data3 = 0x8;}break;
			case Alphabet_i:{data1 = 0x0;data2 = 0x0;data3 = 0x8;}break;
			case Alphabet_j:{data1 = 0xa;data2 = 0x0;data3 = 0xc;}break;
			case Alphabet_k:{data1 = 0x3;data2 = 0x6;data3 = 0x0;}break;
			case Alphabet_l:{data1 = 0xb;data2 = 0x0;data3 = 0x0;}break;
			case Alphabet_m:{data1 = 0x3;data2 = 0x1;data3 = 0xe;}break;
			case Alphabet_n:{data1 = 0x2;data2 = 0x6;data3 = 0x8;}break;
			case Alphabet_o:{data1 = 0xa;data2 = 0x6;data3 = 0x8;}break;
			case Alphabet_p:{data1 = 0x3;data2 = 0x6;data3 = 0x5;}break;
			case Alphabet_q:{data1 = 0x1;data2 = 0x6;data3 = 0xd;}break;
			case Alphabet_r:{data1 = 0x2;data2 = 0x6;data3 = 0x0;}break;
			case Alphabet_s:{data1 = 0x9;data2 = 0x6;data3 = 0x9;}break;
			case Alphabet_t:{data1 = 0xb;data2 = 0x6;data3 = 0x0;}break;
			case Alphabet_u:{data1 = 0xa;data2 = 0x0;data3 = 0x8;}break;
			case Alphabet_v:{data1 = 0xb;data2 = 0x0;data3 = 0xc;}break;
			case Alphabet_w:{data1 = 0x7;data2 = 0x8;data3 = 0xc;}break;
			case Alphabet_x:{data1 = 0x4;data2 = 0x9;data3 = 0x2;}break;
			case Alphabet_y:{data1 = 0x9;data2 = 0x6;data3 = 0xc;}break;
			case Alphabet_z:{data1 = 0xc;data2 = 0x0;data3 = 0x3;}break;					
			case Alphabet_NULL	:					{data1 = 0x0;data2 = 0x0;data3 = 0x0;}break;			
		}		
		switch(nixie)
		{  
			case NIXIE1:{address1  = 0x14;address2  = 0x13;address3  = 0x12;}break;
			case NIXIE2:{address1  = 0x11;address2  = 0x10;address3  = 0x0f;}break;
			case NIXIE3:{address1  = 0x0e;address2  = 0x0d;address3  = 0x0c;}break;
			case NIXIE4:{address1  = 0x0b;address2  = 0x0a;address3  = 0x09;}break;
			case NIXIE5:{address1  = 0x06;address2  = 0x07;address3  = 0x08;}break;
			case NIXIE6:{address1  = 0x03;address2  = 0x04;address3  = 0x05;}break;
			case NIXIE7:{address1  = 0x00;address2  = 0x01;address3  = 0x02;}break;
			case NIXIE8:{address1  = 0x17;address2  = 0x18;address3  = 0x19;}break;
		}
		HAL_Disp_W_Data(dataid,address1,data1);
		HAL_Disp_W_Data(dataid,address2,data2);	
		HAL_Disp_W_Data(dataid,address3,data3);		
}


void DIV_Disp_Num(uint8_t nixie,uint8_t num)
{	
	uint8_t address1,address2,address3;
	uint8_t data1,data2,data3;
		switch(num)
		{
			case NUMNULL:{
			     data1 = 0x0;
				    data2 = 0x0;
				    data3 = 0x0;
			}break;
			case NUM0:{
								data1 = 0xb;
				    data2 = 0x0;
				    data3 = 0xd;
			}break;
			case NUM1:{
			     data1 = 0x0;
				    data2 = 0x0;
				    data3 = 0xc;
			}break;
			case NUM2:{
				    data1 = 0xa;
				    data2 = 0x6;
				    data3 = 0x5;
			}break;	
				case NUM3:{
				    data1 = 0x8;
				    data2 = 0x6;
				    data3 = 0xd;
			}break;	
				case NUM4:{
				    data1 = 0x1;     
				    data2 = 0x6;
				    data3 = 0xc;
			}break;	
				case NUM5:{
				    data1 = 0x9;     
				    data2 = 0x6;
				    data3 = 0x9;
			}break;			
			case NUM6:{
				    data1 = 0xb;
				    data2 = 0x6;
				    data3 = 0x9;
			}break;					
			case NUM7:{
								data1 = 0x0;
								data2 = 0x0;
								data3 = 0xd;
						}break;		
			case NUM8:{
								data1 = 0xb;
								data2 = 0x6;
								data3 = 0xd;
						}break;		
			case NUM9:{
				    data1 = 0x9;
				    data2 = 0x6;
				    data3 = 0xd;
			}break;		
		}		
		switch(nixie)
		{
			case NIXIE1:{
				address1  = 0x14;
				address2  = 0x13;
				address3  = 0x12;
			}break;
			case NIXIE2:{
				address1  = 0x11;
				address2  = 0x10;
				address3  = 0x0f;
			}break;
			case NIXIE3:{
				address1  = 0x0e;
				address2  = 0x0d;
				address3  = 0x0c;
			}break;
			case NIXIE4:{
				address1  = 0x0b;
				address2  = 0x0a;
				address3  = 0x09;
			}break;
			case NIXIE5:{
				address1  = 0x06;
				address2  = 0x07;
				address3  = 0x08;
			}break;
			case NIXIE6:{
				address1  = 0x03;
				address2  = 0x04;
				address3  = 0x05;
			}break;
			case NIXIE7:{
				address1  = 0x00;
				address2  = 0x01;
				address3  = 0x02;
			}break;
			case NIXIE8:{
				address1  = 0x17;
				address2  = 0x18;
				address3  = 0x19;
			}break;
			
		}
		
		HAL_Disp_W_Data(dataid,address1,data1);
		HAL_Disp_W_Data(dataid,address2,data2);	
		HAL_Disp_W_Data(dataid,address3,data3);		
}


//初始化
void DIV_Disp_Init(void)
{
		HAL_Disp_Init();
}

void DIV_Disp_OneFrame(uint8_t  frame)
{
				disp_management.Disp_WichOneFrame = frame;
		
    switch(disp_management.Disp_WichOneFrame){
					case Run_Init:{HAL_Disp_WrSeriesData(dataid,0x00,RunInit_FrameFont,Disp_Max_Length);}break;
				 case Run_Frame:{HAL_Disp_WrSeriesData(dataid,0x00,RunInit_FrameFont,Disp_Max_Length);}break;
					case NormSet_Frame:{HAL_Disp_WrSeriesData(dataid,0x00,NormSet_FrameFont,Disp_Max_Length);}break;		
				}

}

void DIV_Disp_ClearPoint(void)
{
			HAL_Disp_W_Data(dataid,0x15,0x0);
		 HAL_Disp_W_Data(dataid,0x16,0x0);
}
          




void DIV_disp_out12andpoint(void)
{
		 HAL_Disp_W_Data(dataid,0x15,disp_management.Disp_S1Point);
			HAL_Disp_W_Data(dataid,0x16,disp_management.Disp_S2Point);
}
