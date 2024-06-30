#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "stm32f10x.h"

#define OLED_ADDRESS	0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78

void I2C_Configuration(void);//0.96��OLED��ʾ��IIC���ų�ʼ��-��������
void I2C_WriteByte(uint8_t addr,uint8_t data);//IICͨ��Э�飬д�ֽں���
void WriteCmd(unsigned char I2C_Command);//IICͨ��Э�飬д�����
void WriteDat(unsigned char I2C_Data);//IICͨ��Э�飬д���ݺ���
void OLED_Init(void);//0.96��OLED��ʾ���ĳ�ʼ��
void OLED_SetPos(unsigned char x, unsigned char y);//0.96��OLED��ʾ����ʼ���������ú���(x--��ʾ�����꣬y--��ʾ������)
void OLED_Fill(unsigned char fill_Data);//0.96��OLED��ʾ��ȫ����亯��(���ݴ��������fill_Data��ʹ�������������Ļ)
void OLED_CLS(void);//0.96��OLED��ʾ����������
void OLED_ON(void);//0.96��OLED�ӻ��Ѻ���(��OLED�������л���)
void OLED_OFF(void);//0.96��OLED���ߺ���(��OLED����,���͹���)
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);//0.96��OLED��ʾ��Ӣ���ַ���ʾ����(�ַ���С���ò���TextSize��TextSize=1-->6*8��TextSize=2-->8*16)
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);//0.96��OLED��ʾ�������ַ���ʾ����(���ִ�С16*16)
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);//0.96��OLED��ʾ��λͼ��ʾ����


void OLED_Run_Vertical();//0.96��OLED��ʾ����ֱ��������
void OLED_Run_Left();//0.96��OLED��ʾ�������������
void OLED_Run_Right();//0.96��OLED��ʾ�����ҹ�������

#endif
