#ifndef _TFT_H
#define _TFT_H
#include "sys.h"
#include "delay.h"

#include "encoder.h"
#include "key.h"

//PORT Definition
/*
TFT_CS PB3
TFT_RST PB4
TFT_DC PB5
TFT_SDI PB6
TFT_SCK PB7
TFT_LED PB8
TFT_SDO PB9
*/

#define TFT_CS 	PBout(3)
#define TFT_RST	PBout(4)
#define TFT_DC	PBout(5)
#define TFT_SDI	PBout(6)
#define TFT_SCK	PBout(7)
#define TFT_LED	PBout(8)
#define TFT_SDO	PBin(9)



#define TFT_W 240
#define TFT_H 320


//????
#define WHITE      	0xFFFF
#define BLACK      	0x0000	  
#define BLUE      	0x001F  
#define BRED      	0XF81F
#define GRED 				0XFFE0
#define GBLUE			 	0X07FF
#define RED        	0xF800
#define MAGENTA    	0xF81F
#define GREEN     	0x07E0
#define CYAN       	0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //??
#define BRRED 			0XFC07 //???
#define GRAY  			0X8430 //??
//GUI??

#define DARKBLUE    0X01CF	//???
#define LIGHTBLUE   0X7D7C	//???  
#define GRAYBLUE    0X5458 //???
//?????PANEL??? 

#define LIGHTGREEN  0X841F //???
#define LGRAY 			0XC618 //???(PANNEL),?????

#define LGRAYBLUE   0XA651 //????(?????)
#define LBBLUE      0X2B12 //????(???????)


#define CURVE_L		200
#define CURVE_H		200
#define CURVE_SPx	0
#define CURVE_SPy	0

#define MY_floatE_LEN	8
#define MY_int_LEN	8

extern u16 BACK_COLOR, POINT_COLOR;
extern u16 x_addr,y_addr;

extern float TFT_CurveFBUF[CURVE_L];

extern float TFT_CurMAX,TFT_CurMIN;
extern u8 TFT_CurPtr;
extern u8 TFT_NumPos;


typedef union 
{
	float ff;
	u8	fBuf[4];
}MY_float;

typedef struct
{
	int8_t sign;					   //   signed   char	有符号字符型变量
	int8_t digits[MY_floatE_LEN];	   //MY_floatE_LEN=8
	int8_t index;
}MY_floatE;

typedef struct
{
	int8_t sign;
	int8_t digits[MY_int_LEN];
	int8_t len;
}MY_int;


void TFT_Init(void);
void TFT_DeInit(void);
void TFT_WriteCmd(u8 cmd);
void TFT_WriteData(u8 data);
u8 TFT_ReadData(void);
void TFT_WriteGram(u16 rgb);
u8 TFT_Read8bitReg(u8 cmd);
u32 TFT_Read24bitReg(u8 cmd);
u32 TFT_Read32bitReg(u8 cmd);

void TFT_SetAddr(u16 x1,u16 y1,u16 x2,u16 y2);
void TFT_ClrScr(void);
void TFT_DrawPoint(u16 x,u16 y);
void TFT_ClrPoint(u16 x,u16 y);
void TFT_SetColor(u16 back_color,u16 point_color);
void TFT_ShowASCII(u16 x,u16 y,u8 ch,u8 mod);

u16 TFT_ShowString(u16 x,u16 y,u8 *str,u8 pos);
void TFT_ClrPageASCII(u16 x,u16 y);
void TFT_ShowNumInt(u16 x,u16 y,int32_t num);
void TFT_ShowNumFloat(u16 x,u16 y,float num,u16 len);
//void TFT_ShowNumEform(u16 x,u16 y,MY_floatE fe);
void TFT_ShowNumEform(u16 x,u16 y,MY_floatE fE,u8 TFT_NumPos);
void TFT_SetPrintfAddr(u16 x,u16 y);

void TFT_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void TFT_ClrLine(u16 x1, u16 y1, u16 x2, u16 y2);

void TFT_DrawCurve(float *cur,float max,float min);

void TFT_Point2Curve(float newpoint);

void TFT_ClrCurve(void);

void TFT_ClrArea(u16 x1,u16 y1,u16 x2,u16 y2);

void TFT_ClrBuf(void);

void TFT_DrawAxis(void);

MY_floatE float2MY_floatE(float num);
float MY_floatE2float(MY_floatE fe);
MY_int int2MY_int(int32_t num);

void TFT_GetNum(u16 x,u16 y,MY_floatE *floatE);
#endif
