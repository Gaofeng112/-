#ifndef  __DS1302_H
#define  __DS1302_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

//DS1302--SCL(PB11)  DS1302--SDA(PB10)  DS1302--RST(PB1)
#define RST_H    GPIO_SetBits(GPIOB,GPIO_Pin_1)  //复位引脚--高电平
#define RST_L    GPIO_ResetBits(GPIOB,GPIO_Pin_1)//复位引脚--低电平

#define SCLK_H   GPIO_SetBits(GPIOB,GPIO_Pin_11)  //时钟引脚--高电平
#define SCLK_L   GPIO_ResetBits(GPIOB,GPIO_Pin_11)//时钟引脚--低电平

#define DSIO_H   GPIO_SetBits(GPIOB,GPIO_Pin_10)  //数据引脚--高电平
#define DSIO_L   GPIO_ResetBits(GPIOB,GPIO_Pin_10)//数据引脚--低电平
#define GET_DSIO GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//获取数据引脚的电平状态

void    DS1302Write(uint8_t addr, uint8_t dat);//主机向从机DS1302写入数据函数
uint8_t DS1302Read(uint8_t addr);//主机向从机DS1302读出数据函数
void    DS1302Init(void);//DS1302初始化函数
void    DS1302ReadTime(void);//获取DS1302内部的时间数据函数

void    DS1302_Time_Byte_Transform(void);//将获取的时间数据类型(BCD编码)进行字符转换

extern uint8_t  READ_RTC_ADDR[7];//DS1302时间读取地址命令(顺序为：秒分时日月周年)
extern uint8_t  WRITE_RTC_ADDR[7];//DS1302时间写入地址命令(顺序为：秒分时日月周年)

//DS1302时间存放数组
extern uint8_t TIME[7];	//加入全局变量

extern uint8_t Year;          //世纪
extern uint8_t YMD_Buff [11]; //存储年月日的字符数组
extern uint8_t Time_Buff[9];  //存储时分秒的字符数组
extern uint8_t Week[2];       //星期

#endif
