
#include "sys.h"
#include "delay.h"

#include "timer.h"
#include "led.h"
#include "key.h"
#include "encoder.h"
#include "iic.h"
#include "_24cxx.h" 
#include "spi.h"
#include "spi_2.h"
#include "tft.h"
#include "stdio.h"
#include "usart.h"//������ɵĹ���
#include "stdlib.h"


#define AD_BUF_LEN 15
#define AD_BUF_LEN2 15
#define LCVR_THRE_VOLTAGE 0.9
#define LCVR_THRE_VOLTAGE2 1.5

#define a 0.0508
#define b -0.5884
#define c 1.9064
#define a2 0.0508
#define b2 -0.5884
#define c2 1.9064

void BubbleSort(u16 arr[],u8 len);
void BubbleSort2(u16 arr[],u8 len);
u16 PD_Sampling(void);
u16 PD_Sampling2(void);
void Control(void);
void Control2(void);

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

float g_Kp=1.2,g_Ki=0.03,g_Kd=0;  //PID����
float g_Kp2=0.6,g_Ki2=0.03,g_Kd2=0;  //PID����

float err0,err1=0,err2=0;	   //����ź�
float err20,err21=0,err22=0;	   //����ź�

MY_floatE g_SvE,g_KpE,g_KiE,g_KdE;	//�ṹ�� �趨ֵ��PID����
//MY_floatE2 g_SvE2,g_KpE2,g_KiE2,g_KdE2;	//�ṹ�� �趨ֵ��PID����

float sv_temp=0.08;
float pv=LCVR_THRE_VOLTAGE,sv=0.08 ,mv,mv_old=1;	  //pv��������ѹ��ֵ��sv:�趨��ѹֵ��mv:����ֵ
float pv2=LCVR_THRE_VOLTAGE2,sv2=1.1,mv2,mv_old2=1;	  //pv��������ѹ��ֵ��sv:�趨��ѹֵ��mv:����ֵ
float delta,delta2;		   //delta����ʽPID������
float delta_2,delta_22;		   //delta����ʽPID������

u16 AD_AVG;		   //16λADC�ɵõĵ�ѹֵ��Ӧ��������
u16 AD_AVG2;		   //16λADC�ɵõĵ�ѹֵ��Ӧ��������
u16 DA_AMP=0;	   //16λDAC�����������ֵ
u16 DA_AMP2=0;	   //16λDAC�����������ֵ

u16 adBuf[AD_BUF_LEN];
												
MY_floatE t;   //�ṹ��

float op_ratio; //�ֹ��
float ad_avg, ad_avg2;

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


u16 PD_Sampling(void)//70us
{
	u8 i;
	for(i=0;i<AD_BUF_LEN;i++)
	{
		adBuf[i]=AD_SPI_Rd();
	}
	
	BubbleSort(adBuf,AD_BUF_LEN);//ð������
	return adBuf[AD_BUF_LEN/2];
}

u16 PD_Sampling2(void)//70us
{
	u8 i;
	for(i=0;i<AD_BUF_LEN2;i++)
	{
		adBuf[i]=AD_SPI_Rd2();
	}
	
	BubbleSort(adBuf,AD_BUF_LEN2);//ð������
	return adBuf[AD_BUF_LEN2/2];
}

//pv��������ѹ��ֵ��sv:�趨��ѹֵ��mv:����ֵ
void Control(void)//35us
{
	
//	float t;
	
	mv=(float)AD_AVG/65535.0*5.0;	//�� AD�ɹ����ĵ�ѹת��Ϊ��ѹ �ο�5V
								  
	err2=sv-mv;	//�趨ֵ�뷴��ֵ�Ĳ�		  2.3-2.2=0.1
	
	delta=g_Kp*(err2-err1)+g_Ki*err2;	   //PI
		   
	pv=pv-delta;	  //pv0=3.5
	
	if(pv>3.5) pv=3.5;
	else if(pv<1.3) pv=1.3;  
  //  pv=2.3;
	DA_AMP=(u16)(pv/5.0*32767);//range+/-5
	
	err1=err2;	//��һ�ε�ֵ
	mv_old=mv;	
}

//pv��������ѹ��ֵ��sv:�趨��ѹֵ��mv:����ֵ
void Control2(void)//35us
{
	
	float t;
	
	mv2=(float)AD_AVG2/65535.0*5.0;	//�� AD�ɹ����ĵ�ѹת��Ϊ��ѹ �ο�5V
								  
	err22=sv2-mv2;	//�趨ֵ�뷴��ֵ�Ĳ�		  2.3-2.2=0.1
	
	delta_2=g_Kp2*(err22-err21)+g_Ki2*err22;	   //PI
		   
	pv2=pv2-delta_2;	  //pv0=3.5
	
	if(pv2>3.5) pv2=3.5;
	else if(pv2<1.0) pv2=1.0;  
  //  pv=2.3;
	DA_AMP2=(u16)(pv2/5.0*32767);//range+/-5
	
	err21=err22;	//��һ�ε�ֵ
	mv_old2=mv2;	
}


int main(void)
{	
	u16 usart_t;  
	u16 len;
	u8 delay_i=0;
	u16 times=0;
//	u8 USART_RX_BUF_TEMP[USART_REC_LEN + 1];     //���ջ���,���USART_REC_LEN���ֽ�.
//	u16 times=0;
//	char str1;
	delay_init();
	NVIC_Configuration();
//	USART_Configuration();
	LED_Init();
 	KEY_Init();
 	ENCODER_Init();
	AT24CXX_Init();
	SPI_Init();
	SPI_Init2 ();
	AD5422_Init();
	AD5422_Init2();

	uart_init(115200);

	KEY_VAL1.cnt=0;
	KEY_VAL2.cnt=0;
	KEY_VAL3.cnt=0;

	LED2_OFF;
	
	TIM2_Init(600-1,30-1);	 //2KHz 72000000/1200/30=2KHz
//	TIM4_Init(6000-1,30-1);	 //2KHz 72000000/1200/30=2KHz

	while(1){
		op_ratio = AD_AVG2/AD_AVG;
		
		// printf("MV2:%.9f\r\n",mv2);
		// printf("OP:%.6f\r\n",op_ratio);
				
			if(USART_RX_STA&0x8000)
			{					   
				len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
				// printf("\r\n�����͵���ϢΪ:\r\n\r\n");
//				for(usart_t=0;usart_t<len;usart_t++)
//				{
//					USART_SendData(USART1, USART_RX_BUF[usart_t]);//�򴮿�1��������
//					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//					
//				}
				sv = atof(USART_RX_BUF);
				delay_ms(10);
//				printf("sv_temp:%.3f\r\n",sv_temp);
//				printf("\r\n\r\n");//���뻻��
				USART_RX_STA=0;
			}else
			{
					
				times++;
			if(times%5000==0)
			{
				printf("SV:%.5f\r\n",sv);
				printf("MV:%.9f\r\n",mv);
				printf("PV:%.9f\r\n",pv);
				printf("AD_AVG:%d\r\n",AD_AVG);
				printf("delta:%d\r\n",delta);
				printf("Kp:%.3f\r\n",g_Kp);
				printf("Ki:%.3f\r\n",g_Ki);
				printf("\r\n");
				ad_avg = (float)AD_AVG;
				ad_avg2 = (float)AD_AVG2;
				op_ratio = ad_avg2 / ad_avg;			
				printf("AD_AVG2:%d\r\n",AD_AVG2);
			}
			if(times%30==0)
			{
			AD_AVG=PD_Sampling();
			AD_AVG2=PD_Sampling2();
			Control();
			}
				
			}

			

//			printf("SV:%.5f\r\n",sv);
//			printf("MV:%.9f\r\n",mv);
//			printf("PV:%.9f\r\n",pv);
//			printf("AD_AVG:%d\r\n",AD_AVG);
//			printf("delta:%d\r\n",delta);
//			printf("Kp:%.3f\r\n",g_Kp);
//			printf("Ki:%.3f\r\n",g_Ki);
//			printf("\r\n");
//			ad_avg = (float)AD_AVG;
//			ad_avg2 = (float)AD_AVG2;
//			op_ratio = ad_avg2 / ad_avg;			
//			printf("AD_AVG2:%d\r\n",AD_AVG2);
//			delay_ms(100);

////			for (delay_i=0; delay_i<=1000; delay_i++);
////			{
////			delay_i=0;
////			}
//				AD_AVG=PD_Sampling();
//				AD_AVG2=PD_Sampling2();
//				Control();
		
			
		}		
}

void TIM2_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		// ����ʱ���ж�
//		AD_AVG=PD_Sampling();
		switch(TIM_FLAG)
		{
			case 0:					
					DA_Wr(32767+DA_AMP);
					break;
			case 1:
					
					DA_Wr(32767-DA_AMP);
					break;			
		}		
		TIM_FLAG++;
		if(TIM_FLAG>=2) TIM_FLAG=0;
		// ����жϱ�־λ
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

//void TIM4_IRQHandler(void)
//{
//	
//	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
//	{
//		// ����ʱ���ж�

//		AD_AVG=PD_Sampling();
//		AD_AVG2=PD_Sampling2();
//		Control();
//		// ����жϱ�־λ
//		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
//	}
//}

