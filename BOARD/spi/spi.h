#ifndef _SPI_H
#define _SPI_H

#include "sys.h"
#include "delay.h"
			
#define	DA_CS
#define	ADA_SCK
#define	DA_SDI
#define	DA_SDO
#define	AD_SDO
#define	AD_CS


//硬件端口定义
#define DA_CS2 PBout(10)		//DA chip select
#define ADA_SCK2 PBout(1)	//AD&DA clock
#define DA_SDI2 PBout(0)		//DA data input
#define DA_SDO2 PCin(5)		//DA data output
#define AD_SDO2 PCin(4)		//AD data output
#define AD_CS2	PBout(11)		//AD chip select

//硬件端口定义
#define DA_CS1 PCout(6)		//DA chip select
#define ADA_SCK1 PBout(15)	//AD&DA clock
#define DA_SDI1 PBout(14)		//DA data input
#define DA_SDO1 PBin(13)		//DA data output
#define AD_SDO1 PBin(12)		//AD data output
#define AD_CS1	PCout(7)		//AD chip select

//AD5422寄存器定义
#define AD5422_NOP_Reg_Addr (u8)0
#define AD5422_DATA_Reg_Addr (u8)1
#define AD5422_RDBACK_Reg_Addr (u8)2
#define AD5422_CTRL_Reg_Addr (u8)0x55
#define AD5422_RESET_Reg_Addr (u8)0x56

void SPI_Init();
void AD5422_Init(char SPI_Selectted);
u16 AD_SPI_Rd(char SPI_Selectted);
void DA_SPI_Wr(u8 addr,u16 data, char SPI_Selectted);
void DA_Wr(u16 data, char SPI_Selectted);

#endif
