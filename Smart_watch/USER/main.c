#include "main.h"

//int num0 = 0,num1 = 0,num2 = 0,num3 = 0;//��������


int main(void)
{

	delay_init();		//�ӳٳ�ʼ��
	OLED_Init();		//OLED��ʼ��
	key_init();			//������ʼ��
	
	Usart1_Init(9600);  //�������ڳ�ʼ��
	Usart2_Init(115200);//���ڶ���ʼ��  ������ʹ�ã������ʼ��
	DS18B20_Init();		//�¶ȴ�������ʼ��
	Init_ADXL345();		//������ٶȴ�������ʼ��
//	DS1302Init();  //�Ѿ����ʱ�䣬�����ٳ�ʼ��	
	Timer_Init();		//��ʱ�жϳ�ʼ��
	IIC_GPIO_INIT();	//����Ѫ��IIC��ʼ��
	MAX30102_GPIO();	//����Ѫ��GPIO��ʼ��
	Max30102_reset();
	MAX30102_Config();
		
		
	while(1)
	{	
		key_switch_first(); //�������������Ϳ���
		Display();			//������ʾ������������
	}
	

}

//��ʱ���жϺ��������Ը��Ƶ�ʹ�����ĵط�
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//�ж��Ƿ���TIM2�ĸ����¼��������ж�
	{
		processAcceleration(temp_Z);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);			//���TIM2�����¼����жϱ�־λ
															//�жϱ�־λ�������
															//�����жϽ��������ϵش�����������������
	}
}



