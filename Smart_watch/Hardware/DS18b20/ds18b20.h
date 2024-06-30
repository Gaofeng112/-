#ifndef _DS18B20_H
#define _DS18B20_H

#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

#define HIGH  1
#define LOW   0

#define DS18B20_CLK     RCC_APB2Periph_GPIOB
#define DS18B20_PIN     GPIO_Pin_0                  
#define DS18B20_PORT	GPIOB 

//���κ�,����ߵ�ƽ��͵�ƽ��aΪ1ʱ����ߵ�ƽ,aΪ0ʱ����͵�ƽ��
#define DS18B20_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_0)
					
 //��ȡ���ŵĵ�ƽ
#define  DS18B20_DATA_IN()	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)

extern char Temp_Buff[5];//�¶������ַ�����
extern float Temp_real;
					
static void DS18B20_GPIO_Config(void);//����DS18B20�õ���I/O��
static void DS18B20_Mode_IPU(void);//����ʹDS18B20-DATA���ű�Ϊ����ģʽ
static void DS18B20_Mode_Out_PP(void);//����ʹDS18B20-DATA���ű�Ϊ���ģʽ
static void DS18B20_Rst(void);//�������ӻ����͸�λ����
static u8 DS18B20_Presence(void);//���ӻ����������صĴ�������(����ֵΪ0��������ȷ������ֵΪ1���������)
static u8 DS18B20_Read_Bit(void);//��DS18B20��ȡһ��bit����
u8   DS18B20_Read_Byte(void);//��DS18B20��һ���ֽڣ���λ����
void DS18B20_Write_Byte(u8 dat);//дһ���ֽڵ�DS18B20����λ����
void DS18B20_Start(void);//����DS18B20������ʼ�¶�ת��

u8    DS18B20_Init(void);//DS18B20�¶ȴ�������ʼ��
float DS18B20_Get_Temp(void);//��ȡDS18B20�¶ȴ��������¶�����

void  Temp_Data_Transform(void);//����ȡ���¶����ݽ���ת��

#endif
