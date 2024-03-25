#ifndef _ENCODER_H
#define _ENCODER_H

#include "sys.h"
#include "led.h"

//hardware definition
//#define ENCODER_TA	PAin(8)
#define ENCODER_TB	PCin(9)

extern int32_t	encoder_val;

void ENCODER_Init(void);
void ENCODER_Disable(void);

#endif

