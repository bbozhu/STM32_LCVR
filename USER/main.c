
#include "sys.h"
#include "delay.h"

#include "timer.h"
#include "led.h"
#include "key.h"
#include "encoder.h"
#include "iic.h"
#include "_24cxx.h" 
#include "spi.h"
#include "tft.h"
#include "stdio.h"
#include "usart.h"//������ɵĹ���


#define AD_BUF_LEN 15
#define LCVR_THRE_VOLTAGE 1.5

#define a 0.0508
#define b -0.5884
#define c 1.9064

void PID_SetParams(void);
void BubbleSort(u16 arr[],u8 len);
u16 PD_Sampling(char SPI_Selectted);
void Control(void);
void USRAT_send(u16 angle_tsend);
void USART_Configuration(void);
u16 Uart_Send_Float(float value);
float test_cur[75]={
	1.21,1.22,1.23,1.24,1.25,1.26,1.27,1.28,1.29,1.2,1.21,1.22,1.23,1.24,1.25,
	1.3,1.3,1.3,1.3,1.3,1.3,1.3,1.3,1.3,1.3,1.3,1.3,1.3,1.3,1.3,
	1.4,1.4,1.4,1.4,1.4,1.4,1.4,1.4,1.4,1.4,1.4,1.4,1.4,1.4,1.4,
	1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,
	1.6,1.6,1.6,1.6,1.6,1.6,1.6,1.6,1.6,1.6,1.6,1.6,1.6,1.6,1.6,
};
vu16 table_nema0183[9]={'H','E','A','D','0','0','0','0','0'};
vu16 m=0;
u16 temp;	 

float g_Kp=0.6,g_Ki=0.05,g_Kd=0;  //PID����
float err0,err1=0,err2=0;	   //����ź�
MY_floatE g_SvE,g_KpE,g_KiE,g_KdE;	//�ṹ�� �趨ֵ��PID����
float pv=LCVR_THRE_VOLTAGE,sv=1.1,mv,mv_old=1;	  //pv��������ѹ��ֵ��sv:�趨��ѹֵ��mv:����ֵ
float delta,delta2;		   //delta����ʽPID������

u16 AD_AVG;		   //16λADC�ɵõĵ�ѹֵ��Ӧ��������
u16 DA_AMP=0;	   //16λDAC�����������ֵ

u16 adBuf[AD_BUF_LEN];
												
MY_floatE t;   //�ṹ��

void PID_SetParams(void)
{
	u8 len=sizeof(MY_floatE);	//sizeof���ڼ�������������ռ���ֽ��� len=10
	
	if(AT24CXX_ReadOneByte(200)==0x56)//��24c02�����ϴε��趨ֵ
	{
	//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
	//pBuffer  :���������׵�ַ
	//NumToRead:Ҫ�������ݵĸ���
		AT24CXX_Read(0,((int8_t *)&g_KpE),len);

		AT24CXX_Read(len,((int8_t *)&g_KiE),len);

		AT24CXX_Read(len*2,((int8_t *)&g_KdE),len);
		
		AT24CXX_Read(len*3,((int8_t *)&g_SvE),len);
	}
	else//��δ�����趨ֵ
	{
		g_KpE=float2MY_floatE(1);
		g_KiE=float2MY_floatE(1);
		g_KdE=float2MY_floatE(1);
		AT24CXX_WriteOneByte(200,0x56);
	/*	if(AT24CXX_ReadOneByte(200)==0x56)
			printf("Params Init OK\n");
		else
			printf("Params Init ERROR\n"); */
	}

//	printf("SET SV:\n");
	
	TFT_GetNum(40,0,&g_SvE);
	
//	printf("\nSET PID Parameters:");

//	printf("\nKP:");

	TFT_GetNum(40,100,&g_KpE);
	
//	printf("\nKI:");
	
	TFT_GetNum(40,120,&g_KiE);
	
//	printf("\nKD:");
	
//	TFT_GetNum(x_addr,y_addr,&g_KdE);
	
	sv=MY_floatE2float(g_SvE);
	g_Kp=MY_floatE2float(g_KpE);
	g_Ki=MY_floatE2float(g_KiE);
	g_Kd=MY_floatE2float(g_KdE);
	
	AT24CXX_Write(0,((int8_t *)&g_KpE),MY_floatE_LEN+2);
	AT24CXX_Write(len,((int8_t *)&g_KiE),MY_floatE_LEN+2);
	AT24CXX_Write(len*2,((int8_t *)&g_KdE),MY_floatE_LEN+2);
	AT24CXX_Write(len*3,((int8_t *)&g_SvE),MY_floatE_LEN+2);
}

void BubbleSort(u16 arr[],u8 len)
{
	u8 i,j,flag;
	u16 tp;
	for(i=1;i<=len;i++)
	{
		flag=0;
		for(j=0;j<len-i;j++)
		{
			tp=arr[j];
			if(arr[j+1]<tp)
			{
				arr[j]=arr[j+1];
				arr[j+1]=tp;
				flag=1;
			}
		}
		if(!flag)
			break;
	}
	
}

u16 PD_Sampling(char SPI_Selectted)//70us
{
	u8 i;
	for(i=0;i<AD_BUF_LEN;i++)
	{
		adBuf[i]=AD_SPI_Rd(SPI_Selectted);
	}
	
	BubbleSort(adBuf,AD_BUF_LEN);//ð������
	return adBuf[AD_BUF_LEN/2];
}

//pv��������ѹ��ֵ��sv:�趨��ѹֵ��mv:����ֵ
void Control(void)//35us
{
	
	float t;
	
	mv=(float)AD_AVG/65535.0*5.0;	//�� AD�ɹ����ĵ�ѹת��Ϊ��ѹ �ο�5V
								  
	err2=sv-mv;	//�趨ֵ�뷴��ֵ�Ĳ�		  2.3-2.2=0.1
	
   //t=(1-mv/mv_old);
	//if(t>0.005||t<-0.005)
//		delta=0;
//	else	 
		delta=g_Kp*(err2-err1)+g_Ki*err2;	   //PI
		   
	pv=pv-delta;	  //pv0=3.5
	
	if(pv>3.5) pv=3.5;
	else if(pv<1.4) pv=1.4;  
  //  pv=2.3;
	DA_AMP=(u16)(pv/5.0*32767);//range+/-5
	
	err1=err2;	//��һ�ε�ֵ
	mv_old=mv;	
}



int main(void)
{	
	char SPI_Selectted = 1;
	delay_init();
	NVIC_Configuration();
//	USART_Configuration();
	LED_Init();
 	KEY_Init();
 	ENCODER_Init();
	AT24CXX_Init();
	SPI_Init();
	AD5422_Init(SPI_Selectted);
//	TFT_Init();
//	TFT_SetColor(BLUE,YELLOW);
//	TFT_ClrScr();
	uart_init(115200);

/*	TFT_SetPrintfAddr(0,0);
	printf("   LCVR control\n");
	if(!AT24CXX_Check())
		printf("24c02 checked OK\n");
	else
		printf("24c02 checked ERROR\n");	  */

	
	KEY_VAL1.cnt=0;
	KEY_VAL2.cnt=0;
	KEY_VAL3.cnt=0;
	
 	//PID_SetParams();
	LED2_OFF;
//	ENCODER_Disable();
//	KEY_Disable();

	
	
	TIM2_Init(600-1,30-1);	 //2KHz 72000000/1200/30=2KHz
	
	TFT_ClrScr();
	TFT_SetColor(BLUE,RED);
	TFT_DrawAxis();
	TFT_SetColor(BLUE,YELLOW);	 
//	DA_Wr(32767-DA_AMP);
//	while(1)
//	{}
	for(;;)
	{
	if(!KEY1)
		{
		/*	TFT_Point2Curve(mv);
			if(TFT_CurPtr==0)
			{
				TFT_ClrBuf();
				TFT_ClrArea(CURVE_SPx+10,TFT_H-CURVE_SPy-CURVE_H,CURVE_L-1,TFT_H-1-CURVE_SPy-10);
			}
			TFT_DrawCurve(TFT_CurveFBUF,TFT_CurMAX,TFT_CurMIN);		   */
			KEY_Init();
 			ENCODER_Init();
			PID_SetParams();
			ENCODER_Disable();
			KEY_Disable();
			//delay_ms(50);
		}
		else
		{	
		//	AD_AVG=AD_SPI_Rd();

//			TFT_ShowString(0,0,"SV:",0xff);
//			TFT_ShowNumFloat(40,0,sv,10);
//			TFT_ShowString(0,20,"MV:",0xff);
//			TFT_ShowNumFloat(40,20,mv,10);	
//		//	USRAT_send(32.1);
//			TFT_ShowString(0,40,"PV:",0xff);
//			TFT_ShowNumFloat(40,40,pv,10);
//			TFT_ShowNumInt(40,60,AD_AVG);
//			TFT_ShowNumFloat(40,80,delta,10);
//			
//			TFT_ShowString(0,100,"Kp:",0xff);
//			TFT_ShowNumFloat(40,100,g_Kp,10);	

//			TFT_ShowString(0,120,"Ki:",0xff);
//			TFT_ShowNumFloat(40,120,g_Ki,10);
			
		//	delay_ms(1);
			
//			if (SPI_Selectted >= 2 ) 
//			{
//				SPI_Selectted = 1;
//				printf("\r\n");
//			} 
//			else 
//			{
//				SPI_Selectted++;
//			}
			
			AD_AVG=PD_Sampling(SPI_Selectted);
			Control();
//			Uart_Send_Float(mv);
			printf("SV:%.3f\r\n",sv);
			printf("MV:%.3f\r\n",mv);
			printf("PV:%.3f\r\n",pv);
			printf("AD_AVG:%d\r\n",AD_AVG);
			printf("delta:%d\r\n",delta);
			printf("Kp:%.3f\r\n",g_Kp);
			printf("Ki:%.3f\r\n",g_Ki);
			printf("\r\n");
			
			
		}	
	}		 
	
}

void TIM2_IRQHandler(char SPI_Selectted)
{
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		// ����ʱ���ж�
//		AD_AVG=PD_Sampling();
		switch(TIM_FLAG)
		{
			case 0:
					
				   //	AD_AVG=AD_SPI_Rd();
			//		AD_AVG=PD_Sampling();
//			delta2=(1-mv/mv_old)*(b*pv+c);
//					Control();
//					LED1_OFF;
					DA_Wr(32767+DA_AMP,SPI_Selectted);
//			
					break;
			case 1:
					
				//	AD_AVG=AD_SPI_Rd();
//					Control();
				//DA_AMP=16383;
//					LED1_ON;
					DA_Wr(32767-DA_AMP,SPI_Selectted);
					break;
			
		}
		
		TIM_FLAG++;
		if(TIM_FLAG>=2) TIM_FLAG=0;
		// ����жϱ�־λ
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

///*******************************************************************************
//* ������  		: USART_Configuration
//* ��������    	: ����USART1
//* �������      : ��
//* ������      : ��
//* ����ֵ        : ��
//*******************************************************************************/
//void USART_Configuration(void)
//{
//	/* ���� USART ��ʼ���ṹ�� USART_InitStructure */
//  	USART_InitTypeDef USART_InitStructure;
//	  GPIO_InitTypeDef GPIO_InitStructure;
//	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART1��GPIOAʱ
//		/* ���� USART1 ��Tx�ţ�PA.9��Ϊ�ڶ���������������� */
//	
//	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//����AFIOʱ��
//	  GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);//������ӳ��
//	
//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_Init(GPIOB , &GPIO_InitStructure);
//    
//  	/* ���� USART1 ��Rx�ţ�PA.10��Ϊ��������� */
//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  	GPIO_Init(GPIOB , &GPIO_InitStructure);
//	/*	������Ϊ115200bps;
//	*	8λ���ݳ���;
//	*	1��ֹͣλ����У��;
//	*	����Ӳ��������;
//	*	��ֹUSARTʱ��;
//	*	ʱ�Ӽ��Ե�;
//	*	�ڵ�2�����ز�������
//	*	���һλ���ݵ�ʱ�����岻�� SCLK ����� 
//	*/

//	USART_InitStructure.USART_BaudRate = 9600;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No ;
//	USART_InitStructure.USART_HardwareFlowControl =  USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	USART_Init(USART1 , &USART_InitStructure);
//    
//  	/* ʹ�� USART1 */
//  	USART_Cmd(USART1 , ENABLE);
//}
////��AD�ɻ����Ĺ�ǿֵAD_AVGͨ�����ڷ��͸���λ��
//void USRAT_send(u16 angle_tsend)
//{
//	
//	//+48ת��Ϊ��Ӧ���ֵ�ASCII��
///*	table_nema0183[8]=(u16)((uint)angle_tsend%10+48);	  //GE
//	table_nema0183[7]=(u16)((uint)angle_tsend/10%10+48);  //SHI
//	table_nema0183[6]=(u16)((uint)angle_tsend/100+48);	  //BAI
//	table_nema0183[10]=(u16)(((uint)(angle_tsend*10))%10+48);	//FEN
//	 */
//	table_nema0183[8]=(u16)((unsigned int)angle_tsend%10+48);	  //GE
//	table_nema0183[7]=(u16)((unsigned int)angle_tsend/10%10+48);  //SHI
//	table_nema0183[6]=(u16)((unsigned int)angle_tsend/100%10+48);	  //BAI
//	table_nema0183[5]=(u16)(((unsigned int)(angle_tsend*10))%10+48);	//QIAN
//	table_nema0183[4]=(u16)(((unsigned int)(angle_tsend*10))%10+48);	//WAN
//						
//	for(m=4;m<9;m++)
//	{
//		 /* �򴮿�1�������� */
//            USART_SendData(USART1 , table_nema0183[m]);
//            /* ����ʱ����֤�շ��ȶ��� */
//          // for(n=0;n<500;n++);
//		   while(USART_GetFlagStatus(USART1 , USART_FLAG_TC) == RESET);
//	}
//}
//	
