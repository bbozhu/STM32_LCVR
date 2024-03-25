#include "tft.h"
#include "font.h"
#include "stdio.h"

#include "key.h"
#include "encoder.h"

u16 BACK_COLOR, POINT_COLOR;
u16 x_addr,y_addr;

u8 TFT_BUF1[40],TFT_BUF2[40];
u16 TFT_CurveIBUF[CURVE_L];
float TFT_CurveFBUF[CURVE_L];
float TFT_CurMAX=0,TFT_CurMIN=0;
u8 TFT_CurPtr=0;

u8 TFT_NumPos=0;


void TFT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//禁用JTAG，为了使用PB3/PB4
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;////LED接了上拉电阻，可以设成开漏，但此处必须设成推挽
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	
	
 
	TFT_RST=0;
	delay_us(10);	
	TFT_RST=1;
	delay_ms(20);

	TFT_CS =0;  
	
	TFT_WriteCmd(0x21);
	
	TFT_WriteCmd(0xCB);  
	TFT_WriteData(0x39); 
	TFT_WriteData(0x2C); 
	TFT_WriteData(0x00); 
	TFT_WriteData(0x34); 
	TFT_WriteData(0x02); 

	TFT_WriteCmd(0xCF);  
	TFT_WriteData(0x00); 
	TFT_WriteData(0XC1); 
	TFT_WriteData(0X30); 

	TFT_WriteCmd(0xE8);  
	TFT_WriteData(0x85); 
	TFT_WriteData(0x00); 
	TFT_WriteData(0x78); 

	TFT_WriteCmd(0xEA);  
	TFT_WriteData(0x00); 
	TFT_WriteData(0x00); 

	TFT_WriteCmd(0xED);  
	TFT_WriteData(0x64); 
	TFT_WriteData(0x03); 
	TFT_WriteData(0X12); 
	TFT_WriteData(0X81); 

	TFT_WriteCmd(0xF7);  
	TFT_WriteData(0x20); 

	TFT_WriteCmd(0xC0);    //Power control 
	TFT_WriteData(0x23);   //VRH[5:0] 

	TFT_WriteCmd(0xC1);    //Power control 
	TFT_WriteData(0x10);   //SAP[2:0];BT[3:0] 

	TFT_WriteCmd(0xC5);    //VCM control 
	TFT_WriteData(0x3e); //?????
	TFT_WriteData(0x28); 

	TFT_WriteCmd(0xC7);    //VCM control2 
	TFT_WriteData(0x86);  //--

	TFT_WriteCmd(0x36);    // Memory Access Control 
	TFT_WriteData(0x48); //C8	   //48 68??//28 E8 ??

	TFT_WriteCmd(0x3A);    
	TFT_WriteData(0x55); 

	TFT_WriteCmd(0xB1);    
	TFT_WriteData(0x00);  
	TFT_WriteData(0x18); 

	TFT_WriteCmd(0xB6);    // Display Function Control 
	TFT_WriteData(0x08); 
	TFT_WriteData(0x82);
	TFT_WriteData(0x27);  

	TFT_WriteCmd(0xF2);    // 3Gamma Function Disable 
	TFT_WriteData(0x00); 

	TFT_WriteCmd(0x26);    //Gamma curve selected 
	TFT_WriteData(0x01); 

	TFT_WriteCmd(0xE0);    //Set Gamma 
	TFT_WriteData(0x0F); 
	TFT_WriteData(0x31); 
	TFT_WriteData(0x2B); 
	TFT_WriteData(0x0C); 
	TFT_WriteData(0x0E); 
	TFT_WriteData(0x08); 
	TFT_WriteData(0x4E); 
	TFT_WriteData(0xF1); 
	TFT_WriteData(0x37); 
	TFT_WriteData(0x07); 
	TFT_WriteData(0x10); 
	TFT_WriteData(0x03); 
	TFT_WriteData(0x0E); 
	TFT_WriteData(0x09); 
	TFT_WriteData(0x00); 

	TFT_WriteCmd(0XE1);    //Set Gamma 
	TFT_WriteData(0x00); 
	TFT_WriteData(0x0E); 
	TFT_WriteData(0x14); 
	TFT_WriteData(0x03); 
	TFT_WriteData(0x11); 
	TFT_WriteData(0x07); 
	TFT_WriteData(0x31); 
	TFT_WriteData(0xC1); 
	TFT_WriteData(0x48); 
	TFT_WriteData(0x08); 
	TFT_WriteData(0x0F); 
	TFT_WriteData(0x0C); 
	TFT_WriteData(0x31); 
	TFT_WriteData(0x36); 
	TFT_WriteData(0x0F); 
	
	TFT_WriteCmd(0x21);


	TFT_WriteCmd(0x11);    //Exit Sleep 
	delay_ms(60); 

	TFT_WriteCmd(0x29);    //Display on 
//	TFT_WriteCmd(0x2c); 
	
}


void TFT_DeInit(void)
{
	TFT_WriteCmd(0x28);
	TFT_WriteCmd(0x10);
}

void TFT_WriteCmd(u8 cmd)
{
	TFT_DC=0;
/*
	int8_t i;
	for(i=7;i>=0;i--)
	{
		if(cmd&(u8)(1<<i))
			TFT_SDI=1;
		else
			TFT_SDI=0;
		TFT_SCK=0;

		TFT_SCK=1;
	}
	*/
	TFT_SDI=(cmd>>7)&0x01;TFT_SCK=0;TFT_SCK=1;
	TFT_SDI=(cmd>>6)&0x01;TFT_SCK=0;TFT_SCK=1;
	TFT_SDI=(cmd>>5)&0x01;TFT_SCK=0;TFT_SCK=1;
	TFT_SDI=(cmd>>4)&0x01;TFT_SCK=0;TFT_SCK=1;
	TFT_SDI=(cmd>>3)&0x01;TFT_SCK=0;TFT_SCK=1;
	TFT_SDI=(cmd>>2)&0x01;TFT_SCK=0;TFT_SCK=1;
	TFT_SDI=(cmd>>1)&0x01;TFT_SCK=0;TFT_SCK=1;
	TFT_SDI=cmd&0x01;TFT_SCK=0;TFT_SCK=1;

}

void TFT_WriteData(u8 data)
{
	TFT_DC=1;
/*
	int8_t i;
	LED0_ON;
	//running time 1.8us
	for(i=7;i>=0;i--)
	{
		if(data&(u8)(1<<i))
			TFT_SDI=1;
		else
			TFT_SDI=0;
		TFT_SCK=0;

		TFT_SCK=1;
	
	}
	LED0_OFF;
	Delay(10);
*/

	//running time 0.7us
	TFT_SDI=(data>>7)&0x01;TFT_SCK=0;TFT_SCK=1;
	TFT_SDI=(data>>6)&0x01;TFT_SCK=0;TFT_SCK=1;
	TFT_SDI=(data>>5)&0x01;TFT_SCK=0;TFT_SCK=1;
	TFT_SDI=(data>>4)&0x01;TFT_SCK=0;TFT_SCK=1;
	TFT_SDI=(data>>3)&0x01;TFT_SCK=0;TFT_SCK=1;
	TFT_SDI=(data>>2)&0x01;TFT_SCK=0;TFT_SCK=1;
	TFT_SDI=(data>>1)&0x01;TFT_SCK=0;TFT_SCK=1;
	TFT_SDI=data&0x01;TFT_SCK=0;TFT_SCK=1;
	
}

void TFT_WriteGram(u16 rgb)
{
	TFT_WriteData((u8)(rgb>>8));
	TFT_WriteData((u8)rgb);
}



u8 TFT_ReadData(void)
{
	/*
	int8_t i;
	for(i=7;i>=0;i--)
	{	
		TFT_SCK=0;
		if(TFT_SDO)
			temp|=(1<<i);
		
		TFT_SCK=1;
	
	}
	*/
	
	u8 temp=0;
	TFT_SCK=0;temp|=(u8)(TFT_SDO<<7);TFT_SCK=1;
	TFT_SCK=0;temp|=(u8)(TFT_SDO<<6);TFT_SCK=1;
	TFT_SCK=0;temp|=(u8)(TFT_SDO<<5);TFT_SCK=1;
	TFT_SCK=0;temp|=(u8)(TFT_SDO<<4);TFT_SCK=1;
	TFT_SCK=0;temp|=(u8)(TFT_SDO<<3);TFT_SCK=1;
	TFT_SCK=0;temp|=(u8)(TFT_SDO<<2);TFT_SCK=1;
	TFT_SCK=0;temp|=(u8)(TFT_SDO<<1);TFT_SCK=1;
	TFT_SCK=0;temp|=(u8)(TFT_SDO);TFT_SCK=1;	
	
	return temp;
}

u8 TFT_Read8bitReg(u8 cmd)
{
	TFT_WriteCmd(cmd);
	
	return TFT_ReadData();
}

u32 TFT_Read24bitReg(u8 cmd)
{
	u32 temp32;
	
	TFT_WriteCmd(cmd);
	
	TFT_SCK=0;TFT_SCK=1;//dummy clock
	
	temp32=(u32)((TFT_ReadData()<<16)|(TFT_ReadData()<<8)|(TFT_ReadData()));
	
	return temp32;
}

u32 TFT_Read32bitReg(u8 cmd)
{
	u32 temp32;
	
	TFT_WriteCmd(cmd);
	
	TFT_SCK=0;TFT_SCK=1;//dummy clock
	
	temp32=(u32)((TFT_ReadData()<<24)|(TFT_ReadData()<<16)|(TFT_ReadData()<<8)|(TFT_ReadData()));
	
	return temp32;
}

void TFT_SetAddr(u16 x1,u16 y1,u16 x2,u16 y2)
{  
	TFT_WriteCmd(0x2a);// column address
	TFT_WriteData(x1>>8);
	TFT_WriteData(x1);
	TFT_WriteData(x2>>8);
	TFT_WriteData(x2);

	TFT_WriteCmd(0x2b);//page address
	TFT_WriteData(y1>>8);
	TFT_WriteData(y1);
	TFT_WriteData(y2>>8);
	TFT_WriteData(y2);

}

void TFT_ClrScr(void)
{
	u16 i,j;
	
	TFT_SetAddr(0,0,TFT_W-1,TFT_H-1);

	TFT_WriteCmd(0x2C);
	for(i=0;i<TFT_W;i++)
		for (j=0;j<TFT_H;j++)
			TFT_WriteGram(BACK_COLOR);
}

void TFT_DrawPoint(u16 x,u16 y)
{
	TFT_SetAddr(x,y,x,y);
	TFT_WriteCmd(0x2C);
	TFT_WriteGram(POINT_COLOR); 	    
} 

void TFT_ClrPoint(u16 x,u16 y)
{
	TFT_SetAddr(x,y,x,y);
	TFT_WriteCmd(0x2C);
	TFT_WriteGram(BACK_COLOR); 	    
} 

void TFT_SetColor(u16 back_color,u16 point_color)
{
	BACK_COLOR=~back_color;
	POINT_COLOR=~point_color;
}

void TFT_ShowASCII(u16 x,u16 y,u8 ch,u8 mod)
{
	u8 i,j,temp;
	u16 c1,c2;
	
	if(mod)
	{
		c1=POINT_COLOR;
		c2=BACK_COLOR;
	}
	else
	{
		c2=POINT_COLOR;
		c1=BACK_COLOR;
	}
	
	TFT_SetAddr(x,y,x+7,y+15);

	TFT_WriteCmd(0x2C);
	
	for (j=0;j<16;j++)
	{
		temp=asc2_1608[(ch-32)*16+j];
		for(i=0;i<8;i++)	
		{
			if(temp&(1<<i))
				TFT_WriteGram(c1);
			else
				TFT_WriteGram(c2);
		}
	}
}


u16 TFT_ShowString(u16 x,u16 y,u8 *str,u8 pos)
{
	u16 num=0,xaddr=x,yaddr=y;
	while(*(str+num)!='\0')
	{
	
		if(xaddr>TFT_W-8)
		{
			xaddr=0;
			yaddr+=16;
		}
		if(yaddr>TFT_H-16)
		{
			yaddr=0;
		}
		TFT_ShowASCII(xaddr,yaddr,*(str+num),num==pos);
		num++;	
		xaddr+=8;
	}
	return num;	
}

MY_int int2MY_int(int32_t num)
{
	u8 ptr1=0,ptr2=0;
	int32_t temp;
	MY_int ii;
	if(num<0)
	{
		ii.sign=-1;
		temp=-num;
	}
	else
	{
		ii.sign=1;
		temp=num;
	}
	
	do
	{
		TFT_BUF2[ptr2++]=temp%10;
		temp/=10;
	}
	while(temp);
	
	ii.len=ptr2;
	
	while(ptr2)
		ii.digits[ptr1++]=TFT_BUF2[--ptr2];	
	
	return ii;
	
}


void TFT_ShowNumInt(u16 x,u16 y,int32_t num)
{
	u8 ptr1=0,i;

	MY_int ii=int2MY_int(num);
	if(ii.sign<0)
		TFT_BUF1[ptr1++]='-';
	else
		TFT_BUF1[ptr1++]=' ';
	i=0;
	while(i<ii.len)
		TFT_BUF1[ptr1++]=ii.digits[i++]+'0';
	
	TFT_BUF1[ptr1]='\0';
		
	TFT_ShowString(x,y,TFT_BUF1,0xff);
}



void TFT_ShowNumFloat(u16 x,u16 y,float num,u16 len)
{
	u8 ptr1=0,ptr2=0;
	float temp;
	int32_t integer;
	float decimal;
	
	if(num<0)
	{
		TFT_BUF1[ptr1++]='-';
		temp=-num;
	}
	else
	{
		TFT_BUF1[ptr1++]=' ';
		temp=num;
	}
	
	integer=(u32)temp;
	decimal=temp-integer;
	
	do
	{
		TFT_BUF2[ptr2++]=integer%10+'0';
		integer/=10;
	}while(integer);
	
	while(ptr2)
		TFT_BUF1[ptr1++]=TFT_BUF2[--ptr2];	
	TFT_BUF1[ptr1++]='.';
	
	while(len--)
	{
		decimal*=10;
		integer=(u32)decimal;
		decimal-=integer;
		TFT_BUF1[ptr1++]=integer%10+'0';
	}
	
	TFT_BUF1[ptr1]='\0';
	
	TFT_ShowString(x,y,TFT_BUF1,0xff);
}

/*
void TFT_ShowNumEform(u16 x,u16 y,float num,u16 len)
{
	u8 ptr1=0,ptr2=0;
	float temp;
	int32_t integer;
	float decimal;
	int8_t cnt=0,index=0;
	int8_t i;
	
	if(num<0)
	{
		TFT_BUF1[ptr1++]='-';
		temp=-num;
	}
	else
		temp=num;
	
	integer=(int32_t)temp;
	decimal=temp-integer;
	
	while(integer)
	{
		TFT_BUF2[ptr2+cnt%len]=integer%10+'0';
		
		integer/=10;
		cnt++;
		
	}
	index=cnt;
	
	if(cnt>=len)
	{
		for(i=1;i<=len;i++)
		{
			TFT_BUF1[ptr1++]=TFT_BUF2[ptr2+(cnt-i)%len];
			if(i==1)
				TFT_BUF1[ptr1++]='.';
		}			
		
	}
	else
	{
		for(i=1;i<=cnt;i++)
		{
			TFT_BUF1[ptr1++]=TFT_BUF2[ptr2+(cnt-i)%len];
			if(i==1)
				TFT_BUF1[ptr1++]='.';
		}	
		if(cnt==0)
		{
			do
			{
				decimal*=10;
				integer=(int32_t)(decimal);
				decimal-=integer;
				
			
				index--;
				
			}while(!integer);
			
		}
		else
		{
				decimal*=10;
				integer=(int32_t)(decimal);
				decimal-=integer;
		}
		
		do
		{

			TFT_BUF1[ptr1++]=integer+'0';
			cnt++;
			
			if(cnt==1)
				TFT_BUF1[ptr1++]='.';
			
			decimal*=10;
			integer=(int32_t)(decimal);
			decimal-=integer;
			
		}while(cnt<len);
	}


		
	TFT_BUF1[ptr1++]='E';
	
	if(index<0)
	{
		TFT_BUF1[ptr1++]='-';
		integer=-index;
	}
	else
		integer=index;
	ptr2=0;
	do
	{
		TFT_BUF2[ptr2++]=integer%10+'0';
		integer/=10;
	}
	while(integer);
	
	while(ptr2)
		TFT_BUF1[ptr1++]=TFT_BUF2[--ptr2];	
	
	TFT_BUF1[ptr1]='\0';

	TFT_ShowString(x,y,TFT_BUF1);
}

*/



void TFT_ShowNumEform(u16 x,u16 y,MY_floatE fE,u8 TFT_NumPos)
{
	u8 ptr1=0;
	int8_t i;

	MY_int ii;
	
	if(fE.sign==-1)
		TFT_BUF1[ptr1++]='-';
	else
		TFT_BUF1[ptr1++]=' ';
	TFT_BUF1[ptr1++]=fE.digits[0]+'0';
	TFT_BUF1[ptr1++]='.';
	i=1;
	while(i<MY_floatE_LEN)
		TFT_BUF1[ptr1++]=fE.digits[i++]+'0';
	TFT_BUF1[ptr1++]='E';
	
	ii=int2MY_int(fE.index);
	if(ii.sign<0)
		TFT_BUF1[ptr1++]='-';
	else
		TFT_BUF1[ptr1++]=' ';
	
	i=0;
	while(i<ii.len)
		TFT_BUF1[ptr1++]=ii.digits[i++]+'0';
	
	TFT_BUF1[ptr1]='\0';
		
	
	
	
	switch(TFT_NumPos)
	{
		case 0:
			
		case 1:
			TFT_ShowString(x,y,TFT_BUF1,TFT_NumPos);
			//TFT_ShowASCII(x+8*TFT_NumPos,y,TFT_BUF1[TFT_NumPos],0);
			break;
		case 2:
			
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			
		case 9:
			TFT_ShowString(x,y,TFT_BUF1,TFT_NumPos+1);
			//TFT_ShowASCII(x+8*(TFT_NumPos+1),y,TFT_BUF1[TFT_NumPos+1],0);
			break;
		default:
			TFT_ShowString(x,y,TFT_BUF1,TFT_NumPos+1);
			break;
	}

}


///*使用microLib的方法*/
////重定义fputc函数 u16 x_addr,y_addr;
//int fputc(int ch, FILE *f)
//{ 
//	if(ch>=32)
//	{
//		if(x_addr>TFT_W-8)
//		{
//			x_addr=0;
//			y_addr+=16;
//		}
//		if(y_addr>TFT_H-16)
//		{
//			y_addr=0;
//		}

//		TFT_ShowASCII(x_addr,y_addr,ch,1);
//		x_addr+=8;
//	}	
//	else
//	{
//		switch(ch)
//		{
//			case '\n':
//				y_addr+=16;
//				x_addr=0;
//				if(y_addr>TFT_H-16)
//				{
//					y_addr=0;
//				}
//				break;
//			case '\t':
//				x_addr+=16;
//				if(x_addr>TFT_W-8)
//				{
//					x_addr=0;
//					y_addr+=16;
//				}
//				break;
//			
//		}
//	}
//	
//	return ch;
//	
//}

void TFT_SetPrintfAddr(u16 x,u16 y)
{
	x_addr=x;y_addr=y;
}

void TFT_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //?????? 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //?????? 
	else if(delta_x==0)incx=0;//??? 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//??? 
	else{incy=-1;delta_y=-delta_y;} 

	if( delta_x>delta_y)distance=delta_x; //????????? 
	else distance=delta_y; 
	for(t=0;t<=distance;t++ )//???? 
	{  
		TFT_DrawPoint(uRow,uCol);//?? 
		
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}

}    

void TFT_ClrLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //?????? 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //?????? 
	else if(delta_x==0)incx=0;//??? 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//??? 
	else{incy=-1;delta_y=-delta_y;} 

	if( delta_x>delta_y)distance=delta_x; //????????? 
	else distance=delta_y; 
	for(t=0;t<=distance;t++ )//???? 
	{  
		TFT_ClrPoint(uRow,uCol);//?? 
		
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}

}    

void TFT_Point2Curve(float newpoint)
{
	
	if(TFT_CurMAX<newpoint)
		TFT_CurMAX=newpoint;
	if(TFT_CurMIN>newpoint)
		TFT_CurMIN=newpoint;
	TFT_CurveFBUF[TFT_CurPtr++]=newpoint;
	if(TFT_CurPtr>=CURVE_L)
	{
		TFT_CurPtr=0;
		TFT_CurMAX=0;
		TFT_CurMIN=0;
	}

}


void TFT_DrawCurve(float *cur,float max,float min)
{
	u8 i;
	float trans;
	
	if(fabs(max-min)<1e-6)
		return;
	trans=CURVE_H/(max-min);	
	
	TFT_CurveIBUF[0]=TFT_H-1-CURVE_SPy-(u16)((*cur-min)*trans);
	TFT_DrawPoint(0+CURVE_SPx,TFT_CurveIBUF[0]);

	for(i=1;i<CURVE_L;i++)
	{
		TFT_CurveIBUF[i]=TFT_H-1-CURVE_SPy-(u16)((*(cur+i)-min)*trans);
		TFT_DrawPoint(i+CURVE_SPx,TFT_CurveIBUF[i]);
	//	TFT_DrawLine(i-1+CURVE_SPx,TFT_CurveIBUF[i-1],i+CURVE_SPx,TFT_CurveIBUF[i]);
	}
}

void TFT_ClrCurve(void)
{
	u8 i;
	for(i=1;i<CURVE_L;i++)
	{
	
		TFT_ClrPoint(i+CURVE_SPx,TFT_CurveIBUF[i]);
	//	TFT_ClrLine(i-1+CURVE_SPx,TFT_CurveIBUF[i-1],i+CURVE_SPx,TFT_CurveIBUF[i]);
		
	}
}

void TFT_ClrArea(u16 x1,u16 y1,u16 x2,u16 y2)
{
	u16 i,j;
	
	TFT_SetAddr(x1,y1,x2,y2);

	TFT_WriteCmd(0x2C);
	for(i=x1;i<=x2;i++)
		for (j=y1;j<=y2;j++)
			TFT_WriteGram(BACK_COLOR);
}

void TFT_ClrBuf(void)
{
	u16 i;
	for(i=0;i<CURVE_L;i++)
		TFT_CurveFBUF[i]=0;
}
MY_floatE float2MY_floatE(float num)

{
	u8 ptr1=0,ptr2=0;
	float temp;
	int32_t integer;
	float decimal;
	int8_t cnt=0,index=0;
	int8_t i;

	MY_floatE ff;
	
	if(num<0)
	{
		ff.sign=-1;
		temp=-num;
	}
	else
	{
		ff.sign=1;
		temp=num;
	}
	
	integer=(int32_t)temp;
	decimal=temp-integer;
	
	while(integer)
	{
		TFT_BUF2[ptr2+cnt%MY_floatE_LEN]=integer%10;
		
		integer/=10;
		cnt++;
		
	}
	index=cnt;
	
	if(cnt>=MY_floatE_LEN)
	{
		for(i=1;i<=MY_floatE_LEN;i++)
		{
			ff.digits[ptr1++]=TFT_BUF2[ptr2+(cnt-i)%MY_floatE_LEN];
		
		}			
		
	}
	else
	{
		for(i=1;i<=cnt;i++)
		{
			ff.digits[ptr1++]=TFT_BUF2[ptr2+(cnt-i)%MY_floatE_LEN];
			
		}	
		if(cnt==0)
		{
			do
			{
				decimal*=10;
				integer=(int32_t)(decimal);
				decimal-=integer;
				
			
				index--;
				
			}while(!integer);
			
		}
		else
		{
				decimal*=10;
				integer=(int32_t)(decimal);
				decimal-=integer;
		}
		
		do
		{

			ff.digits[ptr1++]=integer;
			cnt++;
			
			decimal*=10;
			integer=(int32_t)(decimal);
			decimal-=integer;
			
		}while(cnt<MY_floatE_LEN);
	}

	ff.index=index;
	
	return ff;

}


float MY_floatE2float(MY_floatE fE)
{
	float tf;
	int8_t ti;
	int8_t i;
	tf=0;
	
	i=MY_floatE_LEN;
	while(i--)
	{
		tf=tf*0.1+fE.digits[i];
	}
	
	if(fE.index<0)
	{
		ti=-fE.index;
		while(ti--)
			tf=tf*0.1;
	}
	else
	{
		ti=fE.index;
		while(ti--)
			tf=tf*10;
	}
	
	if(fE.sign==1)
		return tf;
	else
		return -tf;
}


void TFT_GetNum(u16 x,u16 y,MY_floatE *floatE)
{
	int8_t tp;
	
	u8 pos=0,last_pos=0;

	
	TFT_ShowNumEform(x,y,*floatE,0xff);

	while(KEY_VAL2.cnt%2==0)
	{
		encoder_val=last_pos;
		while(KEY_VAL1.cnt%2==0&&KEY_VAL2.cnt%2==0)
		{
			pos=(u8)encoder_val%(2+MY_floatE_LEN);
			TFT_ShowNumEform(x,y,*floatE,pos);

		}
		last_pos=pos;
		if(pos==0)
		{
			encoder_val=0;
			while(KEY_VAL1.cnt%2==1&&KEY_VAL2.cnt%2==0)
			{
				tp=encoder_val%2;
				if(tp)
					floatE->sign=1;
				else
					floatE->sign=-1;

				TFT_ShowNumEform(x,y,*floatE,pos);
			}
			
		}
		else if(pos==1+MY_floatE_LEN)
		{
			encoder_val=floatE->index;
			while(KEY_VAL1.cnt%2==1&&KEY_VAL2.cnt%2==0)
			{
				tp=encoder_val;
				floatE->index=tp;
			//	TFT_ClrPageASCII(x+(3+MY_floatE_LEN)*8,y);
				TFT_ShowNumEform(x,y,*floatE,pos);
			}
			
		}
		else
		{
			encoder_val=floatE->digits[pos-1];
			while(KEY_VAL1.cnt%2==1&&KEY_VAL2.cnt%2==0)
			{
				tp=(u8)encoder_val%10;
				floatE->digits[pos-1]=tp;
				TFT_ShowNumEform(x,y,*floatE,pos);
			}
			
		}
	}
	
	KEY_VAL1.cnt=0;
	KEY_VAL2.cnt=0;
	TFT_ShowNumEform(x,y,*floatE,0xff);
}

void TFT_ClrPageASCII(u16 x,u16 y)
{
	u16 i,j;

	TFT_SetAddr(x,y,TFT_W-1,y+15);

	TFT_WriteCmd(0x2C);
	
	for (j=0;j<16;j++)
		for(i=x;i<TFT_W;i++)	
				TFT_WriteGram(BACK_COLOR);

}

void TFT_DrawAxis(void)
{
	u8 i;
	u8 len;
	TFT_DrawLine(CURVE_SPx,TFT_H-1-CURVE_SPy,CURVE_SPx,TFT_H-1-CURVE_SPy-CURVE_H);//y_axis
	for(i=0;i<=CURVE_H/10;i++)
	{
		if(i%5==0)
			len=10;
		else
			len=5;
		TFT_DrawLine(CURVE_SPx,TFT_H-1-CURVE_SPy-i*10,CURVE_SPx+len,TFT_H-1-CURVE_SPy-i*10);//scale
	}
	TFT_DrawLine(CURVE_SPx,TFT_H-1-CURVE_SPy,CURVE_SPx+CURVE_L,TFT_H-1-CURVE_SPy);//x_aixs
	for(i=0;i<=CURVE_L/10;i++)
	{
		if(i%5==0)
			len=10;
		else
			len=5;
		TFT_DrawLine(CURVE_SPx+10*i,TFT_H-1-CURVE_SPy,CURVE_SPx+10*i,TFT_H-1-CURVE_SPy-len);//scale
	}
}
