#ifndef __WATCH_DISPLAY_H
#define __WATCH_DISPLAY_H

#include <stdint.h>
#include "stm32f10x.h"
#include "sys.h"
#include "Watch_Management.h"
#include "OLED.h"
#include "OLED_Data.h"
#include "main.h"

void Display(void);//显示函数

void display_test(void);//显示测试程序

//一级界面显示函数
void display_watch(void);  //显示主界面图形
void display_heart(void);
void display_tmeperature(void);
void display_clock(void);
void display_step(void);

//二级界面显示函数
void display_watch_second(void);		//显示主界面的二级界面
void display_heart_second(void);
void display_tmeperature_second(void);
void display_clock_second(void);
void display_step_second(void);

//三级界面显示函数
void display_clock_third(void);

void DS1302_Time_Update(void); //时间更新



#endif
