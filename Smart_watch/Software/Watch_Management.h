#ifndef __WATCH_MANAGEMENT_H
#define __WATCH_MANAGEMENT_H

#include <stdint.h>
#include "stm32f10x.h"
#include "sys.h"
#include "key.h"

extern	u8 flag_display;//�����ʶ�� 1=һ�����桢2=�������桢3=��������ex
extern	u8 watch_display_first; 	//һ������ͼ��ѡ��
extern	u8 watch_display_second;	//��������ͼ���л�
extern	u8 watck_clock_swtich;		//ʱ���Ӳ˵�����ѡ�� ��8λ ÿλ����ͬ���� �������ƿ���


//һ����������
#define	flag_watch 			1
#define	flag_heart 			2
#define flag_tmeperature	3
#define	flag_clock			4
#define	flag_step			5


//��������ʱ�ӹ���
#define watch_clock_back	0X01 //0000 0001
#define watch_clock_year 	0X02 //0000 0010
#define watch_clock_month	0X04
#define watch_clock_day		0X08
#define watch_clock_week	0X10
#define watch_clock_hour	0X20
#define watch_clock_minute	0X40
#define watch_clock_sec		0X80




void change_first(void);
void key_switch_first(void);

#endif

