#include "main.h"

//int num0 = 0,num1 = 0,num2 = 0,num3 = 0;//按键测试


int main(void)
{

	delay_init();		//延迟初始化
	OLED_Init();		//OLED初始化
	key_init();			//按键初始化
	
	Usart1_Init(9600);  //蓝牙串口初始化
	Usart2_Init(115200);//串口二初始化  心率有使用，必须初始化
	DS18B20_Init();		//温度传感器初始化
	Init_ADXL345();		//三轴加速度传感器初始化
//	DS1302Init();  //已经配好时间，无须再初始化	
	Timer_Init();		//定时中断初始化
	IIC_GPIO_INIT();	//心率血氧IIC初始化
	MAX30102_GPIO();	//心率血氧GPIO初始化
	Max30102_reset();
	MAX30102_Config();
		
		
	while(1)
	{	
		key_switch_first(); //按键控制驱动和控制
		Display();			//所有显示函数及其内容
	}
	

}

//定时器中断函数，可以复制到使用它的地方
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
	{
		processAcceleration(temp_Z);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
															//中断标志位必须清除
															//否则中断将连续不断地触发，导致主程序卡死
	}
}



