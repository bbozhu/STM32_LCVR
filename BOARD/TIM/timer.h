#ifndef _TIM_H
#define _TIM_H

#include "sys.h"

void TIM2_Init(u16 pres,u16 period);
void TIM2_DISABLE(void);
void TIM3_Init(u16 pres,u16 period);
void TIM3_DISABLE(void);
extern u8 TIM_FLAG;
#endif
