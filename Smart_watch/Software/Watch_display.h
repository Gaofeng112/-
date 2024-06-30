#ifndef __WATCH_DISPLAY_H
#define __WATCH_DISPLAY_H

#include <stdint.h>
#include "stm32f10x.h"
#include "sys.h"
#include "Watch_Management.h"
#include "OLED.h"
#include "OLED_Data.h"
#include "main.h"

void Display(void);//��ʾ����

void display_test(void);//��ʾ���Գ���

//һ��������ʾ����
void display_watch(void);  //��ʾ������ͼ��
void display_heart(void);
void display_tmeperature(void);
void display_clock(void);
void display_step(void);

//����������ʾ����
void display_watch_second(void);		//��ʾ������Ķ�������
void display_heart_second(void);
void display_tmeperature_second(void);
void display_clock_second(void);
void display_step_second(void);

//����������ʾ����
void display_clock_third(void);

void DS1302_Time_Update(void); //ʱ�����



#endif
