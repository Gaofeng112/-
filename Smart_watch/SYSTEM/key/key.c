#include "key.h"
#include "delay.h"


void key_init(void){//��ʼ������
	
	GPIO_InitTypeDef GPIO_InitStruture;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//��PBʱ��
	
	GPIO_InitStruture.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_InitStruture.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14| GPIO_Pin_13| GPIO_Pin_12; //���ö˿�
	GPIO_Init(GPIOB,&GPIO_InitStruture);//��ʼ��POIB
}

//�������а������¾����1
int key_scan(int key)
{
	if(key==0)//��������ʱ
	{
		delay_ms(200);//��ʱ��������
		if(key==0)
		{
			return 1;
		}
	}
	return 0;//�ް�������
}


//uint8_t	Key_GetNum(void)
//{
//	uint8_t KeyNum = 0;
//	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0)
//	{
//		delay_ms(20);
//		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0);
//		delay_ms(20);
//		KeyNum = 1;
//	}
//	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)
//	{
//		delay_ms(20);
//		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0);
//		delay_ms(20);
//		KeyNum = 2;
//	}
//	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
//	{
//		delay_ms(20);
//		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0);
//		delay_ms(20);
//		KeyNum = 3;
//	}
//	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0)
//	{
//		delay_ms(20);
//		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0);
//		delay_ms(20);
//		KeyNum = 4;
//	}
//	return KeyNum;
//}

