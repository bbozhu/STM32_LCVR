#include "LED.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	//使能gpioc时钟
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;//led0~2
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStruct);//初始化
}
