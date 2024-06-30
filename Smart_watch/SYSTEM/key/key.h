
#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

#define key0  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)
#define key1  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
#define key2  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)
#define key3  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)

#define key0_PRES  key_scan(key0)
#define key1_PRES  key_scan(key1)
#define key2_PRES  key_scan(key2)
#define key3_PRES  key_scan(key3)

void key_init(void);
int key_scan(int key);////消抖后有按键按下就输出1
//uint8_t	Key_GetNum(void);

#endif
