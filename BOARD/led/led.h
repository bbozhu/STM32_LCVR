#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"

#define LED0_ON GPIO_ResetBits(GPIOC, GPIO_Pin_8)
#define LED1_ON GPIO_ResetBits(GPIOC, GPIO_Pin_7)
#define LED2_ON GPIO_ResetBits(GPIOC, GPIO_Pin_6)

#define LED0_OFF GPIO_SetBits(GPIOC, GPIO_Pin_8)
#define LED1_OFF GPIO_SetBits(GPIOC, GPIO_Pin_7)
#define LED2_OFF GPIO_SetBits(GPIOC, GPIO_Pin_6)

void LED_Init(void);

#endif

