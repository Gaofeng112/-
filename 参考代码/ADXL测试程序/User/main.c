#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "adxl345.h"

uint16_t Num;			//定义在定时器中断里自增的变量
unsigned char ReadAdxl345;	   	//定时读取adxl345数据
unsigned char ErrorNum=0;		//记录错误次数
unsigned char CheckNum=0;   	//检测次数
unsigned int BuShu=0;			//步数
unsigned int Normal_num=0;		//正常次数
unsigned int Error_num=0;		//倾斜次数


int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Timer_Init();		//定时中断初始化
	Init_ADXL345();
	OLED_Clear();
	OLED_ShowString(1, 1, "Num:");			//1行1列显示字符串Num:
	
	OLED_ShowString(2, 1, "N_num:");			//1行1列显示字符串Num:
	
	OLED_ShowString(3, 1, "E_num:");			//1行1列显示字符串Num:
	
	while(1)
	{
		if(Single_Read_ADXL345(0X00)==0xe5)	
		{
			Delay_ms(5);
			
		}
		else
		{
			Delay_ms(3);
			
		}

		if(Num==50)
		{
			Num=0;
			ReadAdxl345=1;
		}

		if(ReadAdxl345== 1)   //定时读取adxl345数据     当数据重新准备好时会置高电平
		{
			ReadAdxl345= 0;
			ReadData_x();  						//三轴检测函数
			CheckNum++;
			if(temp_Z<950) //查看正常次数     
			{
				Normal_num++;  //正常次数++
			}
			else
			{
				Error_num++;//倾斜次数
			}

		 }
		/*显示静态字符串*/
		
		 OLED_ShowNum(1, 7, Num, 5);			//不断刷新显示Num变量
		 
		OLED_ShowNum(2,7,Normal_num,5);
		
		OLED_ShowNum(3,7,Error_num,5);
		
	}
}
/**
  * 函    数：TIM2中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
	{
		Num ++;												//Num变量自增，用于测试定时中断
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
															//中断标志位必须清除
															//否则中断将连续不断地触发，导致主程序卡死
	}
}
