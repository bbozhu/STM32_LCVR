#ifndef _SPI_2_H
#define _SPI_2_H

#include "sys.h"
#include "delay.h"


//硬件端口定义
#define DA_CS2 PBout(10)		//DA chip select
#define ADA_SCK2 PBout(1)	//AD&DA clock
#define DA_SDI2 PBout(0)		//DA data input
#define DA_SDO2 PCin(5)		//DA data output
#define AD_SDO2 PCin(4)		//AD data output
#define AD_CS2	PBout(11)		//AD chip select

//AD5422寄存器定义
#define AD5422_NOP_Reg_Addr2 (u8)0
#define AD5422_DATA_Reg_Addr2 (u8)1
#define AD5422_RDBACK_Reg_Addr2 (u8)2
#define AD5422_CTRL_Reg_Addr2 (u8)0x55
#define AD5422_RESET_Reg_Addr2 (u8)0x56

void SPI_Init2(void);
void AD5422_Init2(void);
u16 AD_SPI_Rd2(void);
void DA_SPI_Wr2(u8 addr,u16 data);
void DA_Wr2(u16 data);

#endif
