#ifndef _TIM_H
#define _TIM_H

#include "sys.h"

void TIM2_Init(u16 pres,u16 period);
void TIM2_DISABLE(void);
void TIM3_Init(u16 pres,u16 period);
void TIM3_DISABLE(void);
void TIM4_Init(u16 pres,u16 period);
void TIM4_DISABLE(void);
extern u8 TIM_FLAG;
extern u8 TIM_FLAG2;
extern u8 TIM_FLAG4;
#endif
