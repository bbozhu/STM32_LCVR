#include "encoder.h"


int32_t	encoder_val=0;


void ENCODER_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);	//使能gpioc,gpioa,afio时钟
	
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;//floating
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;//ENCODER_TA
	GPIO_Init(GPIOA,&GPIO_InitStruct);//初始化
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;//ENCODER_TB
	GPIO_Init(GPIOC,&GPIO_InitStruct);//初始化
	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line8;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	
}

void ENCODER_Disable(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=DISABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	
	EXTI_ClearITPendingBit(EXTI_Line8);
}


void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line8)==SET)
	{
		
		if(ENCODER_TB)
		{
			encoder_val++;// clockwise
			LED0_ON;
			LED1_OFF;
		}
		else
		{
			encoder_val--;// counter clockwise
			LED1_ON;
			LED0_OFF;
		}
					
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
}
