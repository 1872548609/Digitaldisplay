#ifndef _USER_DISP_DIV_H
#define _USER_DISP_DIV_H

#include "superhead.h"

#define MainScreen 1
#define SecondScreen 0

#define NUM0     0
#define NUM1     1
#define NUM2     2
#define NUM3     3
#define NUM4     4   
#define NUM5     5
#define NUM6     6
#define NUM7     7
#define NUM8     8
#define NUM9     9
#define NUMNULL     10

#define Symbol_slash 0     // /
#define Symbol_minus 1     // -
#define Symbol_saw 		2     // |
#define Symbol_point 3     // .
#define Symbol_Negativeone  4 //-1

#define NIXIE1    0
#define NIXIE2    1
#define NIXIE3    2
#define NIXIE4    3
#define NIXIE5    4
#define NIXIE6    5
#define NIXIE7    6
#define NIXIE8    7


#define  S1  0x01
#define 	P3  0x02
#define  P2  0x04
#define 	P1  0x08
#define  S2  0x01
#define  P6  0x02
#define  P7  0x04
#define  P8  0x08
#define NUllPoint 0x00


typedef enum {
    Alphabet_A = 0, 	Alphabet_B ,  	   Alphabet_C ,  
				Alphabet_D ,  			Alphabet_E ,  				Alphabet_F ,  
	   Alphabet_G ,  	  Alphabet_H ,  	   Alphabet_I ,  
	   Alphabet_J , 	   Alphabet_K , 	   	Alphabet_L , 	   
				Alphabet_M , 				Alphabet_N ,						Alphabet_O ,
    Alphabet_P ,    	Alphabet_Q ,    		Alphabet_R ,
    Alphabet_S ,    	Alphabet_T ,    		Alphabet_U ,
    Alphabet_V ,   		Alphabet_W ,    		Alphabet_X ,
    Alphabet_Y ,    	Alphabet_Z ,	   	 Alphabet_a , 					
				Alphabet_b ,  	  Alphabet_c, 						Alphabet_d ,  				
				Alphabet_e ,  			Alphabet_f, 						Alphabet_g ,  	   
				Alphabet_h ,  	  Alphabet_i, 						Alphabet_j , 	   	
				Alphabet_k , 	   Alphabet_l,							Alphabet_m , 					
				Alphabet_n ,					Alphabet_o,							Alphabet_p ,    			
				Alphabet_q ,    	Alphabet_r,							Alphabet_s ,    		
				Alphabet_t ,    	Alphabet_u,							Alphabet_v ,   			
				Alphabet_w ,    	Alphabet_x,							Alphabet_y ,    		
				Alphabet_z ,	    Alphabet_NULL
}Alphabet; 


  
typedef enum {
				Disp_Null= 0,
    Disp_Num = 1,
				Disp_Alphabet = 2
}Disp_Mode;

typedef enum{
    Run_Frame = 0,
				NormSet_Frame,
				Run_Init
	
}Disp_Frames;

typedef struct{                               
			 union{
				    uint32_t memory;
								struct{
									uint8_t Nixie_DisMod : 2;
									uint8_t Nixie_Num : 8 ;
									uint8_t Nixie_Alphabet : 8;							
								}Nixie1_Set;			
				}Nixie1_Ctr;
					union{
				    uint32_t memory;
								struct{
									uint8_t Nixie_DisMod : 2;
									uint8_t Nixie_Num : 8;
									uint8_t Nixie_Alphabet : 8;							
								}Nixie2_Set;
				}Nixie2_Ctr;
					union{
				    uint32_t memory;
								struct{
									uint8_t Nixie_DisMod : 2;
									uint8_t Nixie_Num : 8;
									uint8_t Nixie_Alphabet : 8;							
								}Nixie3_Set;
				}Nixie3_Ctr;
				union{
				    uint32_t memory;
								struct{
									uint8_t Nixie_DisMod : 2;
									uint8_t Nixie_Num : 8;
									uint8_t Nixie_Alphabet : 8;									
								}Nixie4_Set;			
				}Nixie4_Ctr;
				union{
				    uint32_t memory;
								struct{
									uint8_t Nixie_DisMod : 2;
									uint8_t Nixie_Num : 8;
									uint8_t Nixie_Alphabet : 8;							
								}Nixie5_Set;
				}Nixie5_Ctr;
				union{
				    uint32_t memory;
								struct{
									uint8_t Nixie_DisMod : 2;
									uint8_t Nixie_Num : 8;
									uint8_t Nixie_Alphabet : 8;							
								}Nixie6_Set;
				}Nixie6_Ctr;
				union{
				    uint32_t memory;
								struct{
									uint8_t Nixie_DisMod : 2;
									uint8_t Nixie_Num : 8;
									uint8_t Nixie_Alphabet : 8;							
								}Nixie7_Set;
				}Nixie7_Ctr;
				union{
				    uint32_t memory;
								struct{
									uint8_t Nixie_DisMod : 2;
									uint8_t Nixie_Num : 8;
									uint8_t Nixie_Alphabet : 8;							
								}Nixie8_Set;
				}Nixie8_Ctr;
				union{
				    uint32_t memory;
								struct{
									uint8_t Nixie_DisMod : 2;
									uint8_t Nixie_Num : 8;
									uint8_t Nixie_Alphabet : 8;							
								}Nixie9_Set;
				}Nixie9_Ctr;
				uint8_t Disp_WichOneFrame;
				uint8_t Disp_S1Point;
				uint8_t Disp_S2Point;
}Disp_Management;

extern Disp_Management disp_management;




void DIV_Disp_MultiplefloatNum(uint8_t screen,float num,uint8_t xbit);



void DIV_Disp_Snprintf(int screen,const char * data1,...);

void DIV_Disp_ByString(int screen,char * string);	

void DIV_Disp_FloatByString(uint8_t screen,char * string);

void DIV_Disp_floatNum(uint8_t screen,float num);

void DIV_Disp_Uint16Num(uint8_t screen,uint16_t num);


void DIV_Disp_Alphabet(uint8_t nixie,uint8_t alphabet);

void DIV_Disp_Num(uint8_t nixie,uint8_t num);

void DIV_Disp_Symbol(uint8_t nixie,uint8_t symbol);


void DIV_Disp_Init(void);

void DIV_Disp_OneFrame(uint8_t  frame);

void DIV_Disp_ClearPoint(void);

#endif
