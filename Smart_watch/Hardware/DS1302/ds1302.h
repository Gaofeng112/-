#ifndef  __DS1302_H
#define  __DS1302_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

//DS1302--SCL(PB11)  DS1302--SDA(PB10)  DS1302--RST(PB1)
#define RST_H    GPIO_SetBits(GPIOB,GPIO_Pin_1)  //��λ����--�ߵ�ƽ
#define RST_L    GPIO_ResetBits(GPIOB,GPIO_Pin_1)//��λ����--�͵�ƽ

#define SCLK_H   GPIO_SetBits(GPIOB,GPIO_Pin_11)  //ʱ������--�ߵ�ƽ
#define SCLK_L   GPIO_ResetBits(GPIOB,GPIO_Pin_11)//ʱ������--�͵�ƽ

#define DSIO_H   GPIO_SetBits(GPIOB,GPIO_Pin_10)  //��������--�ߵ�ƽ
#define DSIO_L   GPIO_ResetBits(GPIOB,GPIO_Pin_10)//��������--�͵�ƽ
#define GET_DSIO GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//��ȡ�������ŵĵ�ƽ״̬

void    DS1302Write(uint8_t addr, uint8_t dat);//������ӻ�DS1302д�����ݺ���
uint8_t DS1302Read(uint8_t addr);//������ӻ�DS1302�������ݺ���
void    DS1302Init(void);//DS1302��ʼ������
void    DS1302ReadTime(void);//��ȡDS1302�ڲ���ʱ�����ݺ���

void    DS1302_Time_Byte_Transform(void);//����ȡ��ʱ����������(BCD����)�����ַ�ת��

extern uint8_t  READ_RTC_ADDR[7];//DS1302ʱ���ȡ��ַ����(˳��Ϊ�����ʱ��������)
extern uint8_t  WRITE_RTC_ADDR[7];//DS1302ʱ��д���ַ����(˳��Ϊ�����ʱ��������)

//DS1302ʱ��������
extern uint8_t TIME[7];	//����ȫ�ֱ���

extern uint8_t Year;          //����
extern uint8_t YMD_Buff [11]; //�洢�����յ��ַ�����
extern uint8_t Time_Buff[9];  //�洢ʱ������ַ�����
extern uint8_t Week[2];       //����

#endif
