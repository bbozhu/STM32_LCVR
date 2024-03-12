#include "key.h"

KEY_STRUCT KEY_VAL1,KEY_VAL2,KEY_VAL3;

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);	// πƒ‹gpioc,gpiod ±÷”

	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;//KEY0/KEY1
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//pull up
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	TIM3_Init(3600-1,100-1);
}

void KEY_Disable(void)
{
	TIM3_DISABLE();
	
}


void TIM3_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
					
		KEY_VAL1.sta_last=KEY_VAL1.sta_cur;
		KEY_VAL1.sta_cur=(~KEY1)&0x0001;
		 
		if(KEY_VAL1.sta_cur&&!KEY_VAL1.sta_last) KEY_VAL1.cnt++;

		
		KEY_VAL2.sta_last=KEY_VAL2.sta_cur;
		KEY_VAL2.sta_cur=(~KEY2)&0x0001;
		
		if(KEY_VAL2.sta_cur&&!KEY_VAL2.sta_last) KEY_VAL2.cnt++;
	
		
		KEY_VAL3.sta_last=KEY_VAL3.sta_cur;
		KEY_VAL3.sta_cur=(~KEY3)&0x0001;
		
		if(KEY_VAL3.sta_cur&&!KEY_VAL3.sta_last) KEY_VAL3.cnt++;
		
				
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}

/**/
