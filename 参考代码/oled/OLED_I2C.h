#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "stm32f10x.h"

#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

void I2C_Configuration(void);//0.96寸OLED显示屏IIC引脚初始化-设置引脚
void I2C_WriteByte(uint8_t addr,uint8_t data);//IIC通信协议，写字节函数
void WriteCmd(unsigned char I2C_Command);//IIC通信协议，写命令函数
void WriteDat(unsigned char I2C_Data);//IIC通信协议，写数据函数
void OLED_Init(void);//0.96寸OLED显示屏的初始化
void OLED_SetPos(unsigned char x, unsigned char y);//0.96寸OLED显示屏起始点坐标设置函数(x--表示横坐标，y--表示纵坐标)
void OLED_Fill(unsigned char fill_Data);//0.96寸OLED显示屏全屏填充函数(根据传入的数据fill_Data，使其填充满整个屏幕)
void OLED_CLS(void);//0.96寸OLED显示屏清屏函数
void OLED_ON(void);//0.96寸OLED从唤醒函数(将OLED从休眠中唤醒)
void OLED_OFF(void);//0.96寸OLED休眠函数(让OLED休眠,降低功耗)
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);//0.96寸OLED显示屏英文字符显示函数(字符大小设置参数TextSize，TextSize=1-->6*8，TextSize=2-->8*16)
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);//0.96寸OLED显示屏中文字符显示函数(汉字大小16*16)
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);//0.96寸OLED显示屏位图显示函数


void OLED_Run_Vertical();//0.96寸OLED显示屏垂直滚动函数
void OLED_Run_Left();//0.96寸OLED显示屏向左滚动函数
void OLED_Run_Right();//0.96寸OLED显示屏向右滚动函数

#endif
