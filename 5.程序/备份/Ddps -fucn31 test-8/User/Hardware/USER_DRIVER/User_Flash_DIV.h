#ifndef _USER_FLASH_DIV_H
#define _USER_FLASH_DIV_H

#include "superhead.h"


//数据结构示意
typedef struct{           //flash写
		uint16_t rank; 
		float F_P1_Value;
	 float F_P2_Value;
	 float F_Hi1_Value;
	 float F_Lo1_Value;		
	 float F_Hi2_Value;
	 float F_Lo2_Value;  
	 uint32_t F_NormolmodeSetValue;
		uint32_t F_PromodeSetValue;
	 float k;
		float b;
		uint8_t calibrationOrNo;
		uint8_t NOserialnum;
}Datahander;

extern Datahander datahander;

void rank_write_data(Datahander* handler);
void rank_read_data(Datahander* handler);

void write_date(Datahander* handler);
void read_date(Datahander * handler);


#endif
