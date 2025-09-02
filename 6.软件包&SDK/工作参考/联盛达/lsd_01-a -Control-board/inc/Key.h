#ifndef KEY_H_
#define KEY_H_

#include "main.h"

#define Clockwise       0x01 //顺时针
#define Anticlockwise   0x02 //逆时针

extern KeyParaTypedef xdata KeyPara;
void Scan();
void Respone();
u8 ScanEncoder();


#endif /* KEY_H_ */




