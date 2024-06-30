#include "Watch_display.h"



void Display(void)
{
	if(flag_display == 1)
	{
		if(watch_display_first == flag_watch)//手表图形界面
			display_watch();
		else if(watch_display_first == flag_heart)//心率图形界面
			display_heart();
		else if(watch_display_first == flag_tmeperature)//温度图形界面
			display_tmeperature();
		else if(watch_display_first == flag_clock)//时钟图形界面
			display_clock();
		else if(watch_display_first == flag_step)//步数图形界面
			display_step();
		else
			display_test();
	}
	else if(flag_display == 2)
	{
		if(watch_display_first == flag_watch)//手表界面
			display_watch_second();
		else if(watch_display_first == flag_heart)//心率检测界面
			display_heart_second();
		else if(watch_display_first == flag_tmeperature)//温度检测界面
			display_tmeperature_second();
		else if(watch_display_first == flag_clock)//时钟界面
			display_clock_second();
		else if(watch_display_first == flag_step)//步数检测界面
			display_step_second();
		else
			display_test();
	}
	else if(flag_display == 3) //时钟三级界面
	{
		display_clock_third();//时钟三级界面
	}
	else
		display_test();
}

void display_test(void)
{
	OLED_Clear();
	OLED_Printf(30,30,OLED_8X16,"你好");
	OLED_Update();
}

void display_watch(void) //主界面
{
	OLED_Clear();
//	OLED_ShowChar(20,20,'1',OLED_8X16);
	OLED_ShowImage(0,0,128,64,BMP1);
	OLED_ClearArea(0,32,128,16);
	OLED_Update();
}

void display_heart(void)
{
	OLED_Clear();
	OLED_ShowImage(34,2,60,60,Image_heart);
	OLED_Update();
}

void display_tmeperature(void)
{
	OLED_Clear();
	OLED_ShowImage(34,2,60,60,Image_temperature);
	OLED_Update();
}

void display_clock(void)
{
	OLED_Clear();
	OLED_ShowImage(34,2,60,60,Image_clock);
	OLED_Update();
}

void display_step(void)
{
	OLED_Clear();
	OLED_ShowImage(34,2,60,60,Image_run);
	OLED_Update();
}

void display_watch_second(void) //主界面-子节面
{
	DS1302ReadTime();							//从DS1302读取数据
	DS1302_Time_Byte_Transform();				//读取的数据再转化成字符串
	
	OLED_Clear();								//OLED清屏
	OLED_ShowString(20,3,YMD_Buff,OLED_8X16);	//显示年月日字符串
	OLED_ShowString(30,23,Time_Buff,OLED_8X16);	//显示字符串时分秒
	OLED_ShowChinese(44,43,"周");				//显示汉字“周”
	OLED_ShowString(64,43,Week,OLED_8X16);		//显示字符串6
	
	OLED_Update();

}

void display_heart_second(void)
{
//	OLED_ShowString(1,1,"start APP.....",OLED_6X8);

	int i = 0;
	for(i = 0;i < 128;i++) 
	{
		while(MAX30102_INTPin_Read()==0)
		{
			//读取FIFO
			max30102_read_fifo();
		}
	}
	
	blood_Loop();				//获取血氧和心率

//	OLED_Clear();
//	OLED_ShowString(20,20,"2_B",OLED_8X16);
//	OLED_Update();
}

void display_tmeperature_second(void)
{
	DS18B20_Get_Temp();
	Temp_Data_Transform();
	OLED_Clear();
	OLED_ShowChinese(0,0,"温度");
	OLED_ShowChinese(30,0,"：");
	OLED_Printf(40,0,OLED_8X16,"%s",Temp_Buff);
	OLED_ShowChinese(75,0,"℃");
	if(Temp_real<31)
	{
		OLED_ShowChinese(0,20,"建议：天气较凉，");
		OLED_ShowChinese(0,40,"多穿衣服");
	}
	else
	{
		OLED_ShowChinese(0,20,"建议：天气较热，");
		OLED_ShowChinese(0,40,"少穿衣服");
	}
	OLED_Update();
	UsartPrintf(USART1,"温度：%s\r\n",Temp_Buff);
}

void display_clock_second(void)  //时钟子菜单显示和选择
{
//	DS1302_Time_Update();
	DS1302ReadTime();							//从DS1302读取数据
	DS1302_Time_Byte_Transform();				//读取的数据再转化成字符串
	
	u8 year[5]={YMD_Buff[0],YMD_Buff[1],YMD_Buff[2],YMD_Buff[3]};
    u8 weak[2]={Week[0]};
    u8 month[3]={YMD_Buff[5],YMD_Buff[6]};
    u8 day[3]={YMD_Buff[8],YMD_Buff[9]};
    u8 hour[3]={Time_Buff[0],Time_Buff[1]};
    u8 minute[3]={Time_Buff[3],Time_Buff[4]};
    u8 second[3]={Time_Buff[6],Time_Buff[7]};
						
	
	OLED_Clear();	
	OLED_Printf(0,0,OLED_6X8,"back");
	OLED_Printf(0,7,OLED_6X8,"Year:%s",year);
	OLED_Printf(0,15,OLED_6X8,"Month:%s",month);
	OLED_Printf(0,23,OLED_6X8,"Day:%s",day);
	OLED_Printf(0,31,OLED_6X8,"Week:%s",weak);
	OLED_Printf(0,39,OLED_6X8,"Hour:%s",hour);
	OLED_Printf(0,47,OLED_6X8,"Minute:%s",minute);
	OLED_Printf(0,55,OLED_6X8,"Second:%s",second);
	OLED_Update();




	switch(watck_clock_swtich)  //光标移动函数
	{
		case watch_clock_back: 		
		{
			OLED_ShowImage(100,0,15,8,Image_arrows);
			OLED_Update();
			break;
		}
		case watch_clock_year: 	
		{	
			OLED_ShowImage(100,7,15,8,Image_arrows);
			OLED_Update();
			break;
		}
		case watch_clock_month:		
		{
			OLED_ShowImage(100,15,15,8,Image_arrows);
			OLED_Update();
			break;
		}
		case watch_clock_day: 	
		{	
			OLED_ShowImage(100,23,15,8,Image_arrows);
			OLED_Update();
			break;
		}
		case watch_clock_week: 		
		{	
			OLED_ShowImage(100,31,15,8,Image_arrows);
			OLED_Update();
			break;
		}
		case watch_clock_hour: 		
		{
			OLED_ShowImage(100,39,15,8,Image_arrows);
			OLED_Update();
			break;
		}
		case watch_clock_minute: 	
		{
			OLED_ShowImage(100,47,15,8,Image_arrows);
			OLED_Update();
			break;
		}
		case watch_clock_sec: 		
		{
			OLED_ShowImage(100,55,15,8,Image_arrows);
			OLED_Update();
			break;
		}
		default:
		{
			OLED_ShowImage(100,0,15,8,Image_arrows);
			OLED_Update();
		}
	}
	
}

void display_step_second(void)
{
	TIM_Cmd(TIM2, ENABLE);			//使能TIM2，定时器开始运行
	ReadData_x();//从计步模块中获取x,y,z三轴数据
//	stepCount=0;
//	int w = 6;
	OLED_Clear();
	OLED_ShowChinese(0,0,"步数：");
	OLED_Printf(50,0,OLED_8X16,"%d",stepCount);
//	OLED_Printf(20,40,OLED_8X16,"Z:%f",temp_Z);
	if(stepCount>5)
		OLED_ShowChinese(0,20,"建议：需要补水");
	else
		OLED_ShowChinese(0,20,"建议：正常运动中");
	
	OLED_Update();
	UsartPrintf(USART1,"步数%3d\r\n",stepCount);
	if(key3_PRES)stepCount=0;
	
}


void display_clock_third(void)
{
	
//	DS1302ReadTime();							//从DS1302读取数据

	DS1302_Time_Update();						//更新修改好的数据
	DS1302_Time_Byte_Transform();				//读取的数据再转化成字符串
	
	//读取时内的时间数据
	u8 year[5]={YMD_Buff[0],YMD_Buff[1],YMD_Buff[2],YMD_Buff[3]};
    u8 weak[2]={Week[0]};
    u8 month[3]={YMD_Buff[5],YMD_Buff[6]};
    u8 day[3]={YMD_Buff[8],YMD_Buff[9]};
    u8 hour[3]={Time_Buff[0],Time_Buff[1]};
    u8 minute[3]={Time_Buff[3],Time_Buff[4]};
    u8 second[3]={Time_Buff[6],Time_Buff[7]};
						
	
	OLED_Clear();	
	OLED_Printf(0,0,OLED_6X8,"back");
	
	if(watck_clock_swtich==watch_clock_year)		//选择修改年份，就会显示到中间供修改
		OLED_Printf(30,7,OLED_6X8,"Year:%s",year);
	else
		OLED_Printf(0,7,OLED_6X8,"Year:%s",year);
	
	if(watck_clock_swtich==watch_clock_month)
		OLED_Printf(30,15,OLED_6X8,"Month:%s",month);
	else
		OLED_Printf(0,15,OLED_6X8,"Month:%s",month);
	
	if(watck_clock_swtich==watch_clock_day)
		OLED_Printf(30,23,OLED_6X8,"Day:%s",day);
	else
		OLED_Printf(0,23,OLED_6X8,"Day:%s",day);
	
	if(watck_clock_swtich==watch_clock_week)
		OLED_Printf(30,31,OLED_6X8,"Week:%s",weak);
	else
		OLED_Printf(0,31,OLED_6X8,"Week:%s",weak);
	
	if(watck_clock_swtich==watch_clock_hour)
		OLED_Printf(30,39,OLED_6X8,"Hour:%s",hour);
	else
		OLED_Printf(0,39,OLED_6X8,"Hour:%s",hour);
	
	if(watck_clock_swtich==watch_clock_minute)
		OLED_Printf(30,47,OLED_6X8,"Minute:%s",minute);
	else
		OLED_Printf(0,47,OLED_6X8,"Minute:%s",minute);
	
	if(watck_clock_swtich==watch_clock_sec)
		OLED_Printf(30,55,OLED_6X8,"Second:%s",second);
	else
		OLED_Printf(0,55,OLED_6X8,"Second:%s",second);
	OLED_Update();
		
}


void DS1302_Time_Update(void)//更新时间数据
{
    uint8_t i;
    DS1302Write(0x8E,0x00);//关闭DS302的写保护，允许用户写入数据
        for(i=0;i<7;i++)
            DS1302Write(WRITE_RTC_ADDR[i],TIME[i]);//初始化时间
    DS1302Write(0x8E,0x80);//打开DS302的写保护，禁止用户写入数据
}



