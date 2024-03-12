#include "spi.h"

////硬件端口定义
//#define DA_CS2 PBout(10)		//DA chip select
//#define ADA_SCK2 PBout(1)	//AD&DA clock
//#define DA_SDI2 PBout(0)		//DA data input
//#define DA_SDO2 PCin(5)		//DA data output
//#define AD_SDO2 PCin(4)		//AD data output
//#define AD_CS2	PBout(11)		//AD chip select

void SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);		
	
	//2-AD DA spi通信	
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_1|GPIO_Pin_0;//1-DA_SDI(PB0)、1-ADA_SCK(PB1)、1-DA_CS(PB10)
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11;//1-AD_CS(PB11)
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;//1-DA_SDO(PC5)
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4;//1-AD_SDO(PC4)
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_1);//1-ADA_SCK(PB1)set to be high
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);//1-DA_SDI(PB0)、1-DA_CS(PB10)set to be low
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);//1-DA_CS(PB10)set to be low
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);//1-AD_CS(PB111)set to be low

//		//1-AD DA spi通信	
//		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_15;//1-DA_SDI(PB14)、1-ADA_SCK(PB15)
//		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
//		GPIO_Init(GPIOB,&GPIO_InitStruct);
//		
//		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;//1-DA_CS(PC6)
//		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
//		GPIO_Init(GPIOC,&GPIO_InitStruct);
//		
//		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7;//1-AD_CS(PC7)
//		GPIO_Init(GPIOC,&GPIO_InitStruct);
//		
//		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//		
//		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;//1-DA_SDO(PB13)
//		GPIO_Init(GPIOB,&GPIO_InitStruct);
//		
//		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12;//1-AD_SDO(PB12)
//		GPIO_Init(GPIOB,&GPIO_InitStruct);
//		
//		GPIO_SetBits(GPIOB,GPIO_Pin_15);//1-ADA_SCK(PB15)set to be high
//		GPIO_ResetBits(GPIOB,GPIO_Pin_14);//1-DA_SDI(PB14)、1-DA_CS(PC6)set to be low
//		GPIO_ResetBits(GPIOC,GPIO_Pin_6);//1-DA_CS(PC6)set to be low
//		GPIO_ResetBits(GPIOC,GPIO_Pin_7);//1-AD_CS(PC7)set to be low
	
}
/*
//硬件端口定义
#define DA_CS PCout(3)		//DA chip select
#define ADA_SCK PCout(2)	//AD&DA clock
#define DA_SDI PCout(1)		//DA data input
#define DA_SDO PCin(0)		//DA data output
#define AD_SDO PAin(1)		//AD data output
#define AD_CS	PAout(0)		//AD chip select
*/

u16 AD_SPI_Rd(void)//12us
{
	u16 temp=0;
	int8_t i;
	AD_CS2=0;
	ADA_SCK2=1;
//	Delay(1);
	AD_CS2=1;
	delay_us(1);
	AD_CS2=0;
//	Delay(1);
	
	if(AD_SDO2)
		temp=temp|((u16)(1<<15));
	
	for(i=14;i>=0;i--)
	{
		ADA_SCK2=0;
		if(AD_SDO2)
		temp=temp|((u16)(1<<i));
	//	Delay(1);
		ADA_SCK2=1;
	//	Delay(1);
	}		
	return temp;
}

void DA_SPI_Wr(u8 addr,u16 data)//33us
{
	int8_t i;

	ADA_SCK2=0;
	DA_CS2=0;
	Delay(5);
	
	for(i=7;i>=0;i--)
	{
		if(addr&(1<<i))
			DA_SDI2=1;
		else
			DA_SDI2=0;
		ADA_SCK2=1;
		Delay(1);
		ADA_SCK2=0;
		Delay(1);
	}
	
	for(i=15;i>=0;i--)
	{
		if(data&(1<<i))
			DA_SDI2=1;
		else
			DA_SDI2=0;
		ADA_SCK2=1;
		Delay(1);
		ADA_SCK2=0;
		Delay(1);
	}
	
	DA_CS2=1;
	Delay(5);
	DA_CS2=0;
}



void DA_Wr(u16 data)
{
	DA_SPI_Wr(AD5422_DATA_Reg_Addr,data);
}

void AD5422_Init(void)
{
	DA_SPI_Wr(AD5422_RESET_Reg_Addr,0x0001);
	DA_SPI_Wr(AD5422_CTRL_Reg_Addr,(u16)0x1002);//0x1000->0~5;0x1001->0~10;0x1002->+/-5;0x1003->+/-10
}
