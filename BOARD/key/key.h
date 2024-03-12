#ifndef _KEY_H
#define _KEY_H

#include "sys.h"
#include "timer.h"

#define KEY1 PCin(11)
#define KEY2 PCin(12)
#define KEY3 PDin(2)


typedef struct
{
	u8 sta_cur;
	u8 sta_last;
	u8 cnt;
}KEY_STRUCT;

extern KEY_STRUCT KEY_VAL1,KEY_VAL2,KEY_VAL3;

void KEY_Init(void);
void KEY_Disable(void);

#endif
