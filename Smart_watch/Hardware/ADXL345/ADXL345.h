#ifndef __ADXL345_H
#define __ADXL345_H

#include "stm32f10x.h"

#define uint unsigned int
#define uchar unsigned char
	
extern float temp_X,temp_Y,temp_Z; //三轴加速度
extern float lastZ;//最后一次Z轴加速度
extern int stepCount;//步数统计


void SCL_Set_Output(void);
void SDA_Set_Output(void);
void SDA_Set_Input(void);
void Init_ADXL345(void);    
void  Single_Write_ADXL345(uchar REG_Address,uchar REG_data);   //
uchar Single_Read_ADXL345(uchar REG_Address);                   //
void  Multiple_Read_ADXL345(void);                              //
void ADXL345_Start(void);
void ADXL345_Stop(void);
void ADXL345_SendACK(uchar ack);
uchar  ADXL345_RecvACK(void);
void ADXL345_SendByte(uchar dat);
uchar ADXL345_RecvByte(void);
void ADXL345_ReadPage(void);
void ADXL345_WritePage(void);
void ReadData_x(void);
void processAcceleration(float currentZ);//步数统计
#endif
