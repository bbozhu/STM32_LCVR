#ifndef _IIC_H
#define _IIC_H
#include "sys.h"
#include "stm32f10x.h"



//IO��������
#define SDA_IN()  {GPIOA->CRH&=0X0FFFFFFF;GPIOA->CRH|=8<<28;}
#define SDA_OUT() {GPIOA->CRH&=0X0FFFFFFF;GPIOA->CRH|=3<<28;}

//IO��������	 
#define IIC_SCL    PCout(10) //SCL
#define IIC_SDA    PAout(15) //SDA	 
#define READ_SDA   PAin(15)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

#endif
