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

//带参宏,输出高电平或低电平（a为1时输出高电平,a为0时输出低电平）
#define DS18B20_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_0)
					
 //读取引脚的电平
#define  DS18B20_DATA_IN()	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)

extern char Temp_Buff[5];//温度数据字符数组
extern float Temp_real;
					
static void DS18B20_GPIO_Config(void);//配置DS18B20用到的I/O口
static void DS18B20_Mode_IPU(void);//配置使DS18B20-DATA引脚变为输入模式
static void DS18B20_Mode_Out_PP(void);//配置使DS18B20-DATA引脚变为输出模式
static void DS18B20_Rst(void);//主机给从机发送复位脉冲
static u8 DS18B20_Presence(void);//检测从机给主机返回的存在脉冲(返回值为0及脉冲正确，返回值为1及脉冲错误)
static u8 DS18B20_Read_Bit(void);//从DS18B20读取一个bit数据
u8   DS18B20_Read_Byte(void);//从DS18B20读一个字节，低位先行
void DS18B20_Write_Byte(u8 dat);//写一个字节到DS18B20，低位先行
void DS18B20_Start(void);//启动DS18B20，并开始温度转换

u8    DS18B20_Init(void);//DS18B20温度传感器初始化
float DS18B20_Get_Temp(void);//获取DS18B20温度传感器的温度数据

void  Temp_Data_Transform(void);//将获取的温度数据进行转换

#endif
