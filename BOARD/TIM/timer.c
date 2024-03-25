#include "timer.h"

u8 TIM_FLAG=0;
u8 TIM_FLAG2=0;
u8 TIM_FLAG4=0;
//period(in us)=(pres+1)*(period+1)/36000000
void TIM2_Init(u16 pres,u16 period)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	//enable TIM2's clock
	
	//TIM_DeInit(TIM2);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler=pres;//预分频值
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period=period;//重装载值
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//时钟分频，不分频
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);//初始化定时器2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//使能TIM2更新终中断
	
	//设置NVIC，TIM2中断优先级并使能
	NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM2,ENABLE);//启动TIM2
	
}

void TIM2_DISABLE(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	TIM_Cmd(TIM2,DISABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=DISABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	
}

void TIM3_Init(u16 pres,u16 period)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	//enable TIM3's clock
	
//	TIM_DeInit(TIM3);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler=pres;//预分频值
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period=period;//重装载值
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//时钟分频，不分频
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);//初始化定时器2
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//使能TIM3更新终中断
	
	//设置NVIC，TIM3中断优先级并使能
	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM3,ENABLE);//启动TIM3
	
}

void TIM3_DISABLE(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	TIM_Cmd(TIM3,DISABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=DISABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

//period(in us)=(pres+1)*(period+1)/36000000
void TIM4_Init(u16 pres,u16 period)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	//enable TIM2's clock
	
	//TIM_DeInit(TIM2);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler=pres;//预分频值
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period=period;//重装载值
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//时钟分频，不分频
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);//初始化定时器2
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//使能TIM2更新终中断
	
	//设置NVIC，TIM2中断优先级并使能
	NVIC_InitStruct.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM4,ENABLE);//启动TIM2
	
}

void TIM4_DISABLE(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	TIM_Cmd(TIM2,DISABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=DISABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	
}

