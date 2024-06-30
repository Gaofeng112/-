#include "Watch_Management.h"
#include "Watch_display.h"

u8 flag_display = 1;//界面标识符 1=一级界面、2=二级界面、3=三级界面，OLED只能显示三级界面的画面 
u8 watch_display_first = flag_watch; 	//一级界面图形选择（5个界面之一）
u8 watch_display_second = 1;			//二级界面图形切换
u8 watck_clock_swtich = watch_clock_back; //时钟子功能



//该函数保持主界面在1-5跳动
void change_first(void)
{
	if(watch_display_first == 0)
		watch_display_first=flag_step;
	else if(watch_display_first == 6)
		watch_display_first= flag_watch;
	else
		watch_display_first = watch_display_first;
}

//按键控制主界面
void key_switch_first(void)
{
	//在主界面时
	if(flag_display==1) 
	{
		if(key0_PRES == 1)
		{
			watch_display_first-=1;  //开关0左移
			change_first();			 //检测主界面是否小于1
		}
		else if(key1_PRES == 1)
		{
			watch_display_first+=1;	//开关1 右移
			change_first();			//检测主界面是否大于5
		}
		else if(key2_PRES == 1)
		{
			flag_display=2;
		}
	}
	
	//在二级界面
	else if(flag_display==2)//在第二个界面
	{
		if(watch_display_first == 4) //进入时钟界面
		{
			if(key0_PRES==1)//下移光标和下一个功能
			{
				watck_clock_swtich = watck_clock_swtich << 7 | watck_clock_swtich >> 1;
			}
			else if(key1_PRES == 1)//上移光标和上一个功能
			{
				watck_clock_swtich = watck_clock_swtich >> 7 | watck_clock_swtich << 1;
			}
			else if(key2_PRES == 1)//确认
			{
				if(watck_clock_swtich == watch_clock_back) //确认返回
				{
					flag_display = 1;
				}
				else //其他情况进入三级界面
				{
					flag_display = 3;
				}
			}
		}
		else if(key2_PRES == 1)//按键2退出主界面
		{
			flag_display = 1;
		}
		else
			flag_display = flag_display;
	}
	
	//在三级界面
	else if(flag_display == 3)
	{
//		DS1302ReadTime();							//从DS1302读取数据
//		DS1302_Time_Byte_Transform();				//读取的数据再转化成字符串
		
		
		if(key2_PRES == 1)//按下按键2就退出
		{
			flag_display = 2;
		}
		
		
		else if(watck_clock_swtich == watch_clock_year)//确认修改年份
		{
			u8  year=0;
            year=(TIME[6]/16)*10+TIME[6]%16;//BCD编码转换为十进制 0X23取出23
			
			if(key0_PRES == 1)
				year+=1;
			else if(key1_PRES == 1)
				year-=1;
			else
				year = year;
			
			
			TIME[6]=(year/10)*16+year%10;//十进制转换为BCD编码
		
		}
		
		else if(watck_clock_swtich == watch_clock_month)//确认修改月份
		{
			u8  month=0;
            month=(TIME[4]/16)*10+TIME[4]%16;//BCD编码转换为十进制
			
			if(key0_PRES == 1)
				month+=1;
			else if(key1_PRES == 1)
				month-=1;
			else
				month = month;
			
			
			if(month>=1&&month<=12)
				TIME[4]=(month/10)*16+month%10;//十进制转换为BCD编码
			else
				TIME[4]=TIME[4];
		
		}
		
		else if(watck_clock_swtich == watch_clock_day)//确认修改日期
		{
			u8  day=0;
            day=(TIME[3]/16)*10+TIME[3]%16;//BCD编码转换为十进制
			
			if(key0_PRES == 1)
				day+=1;
			else if(key1_PRES == 1)
				day-=1;
			else
				day = day;
			
			if(day>=1 && day <= 31)
				TIME[3]=(day/10)*16+day%10;//十进制转换为BCD编码
			else
				TIME[3]=TIME[3];
		}
		
		else if(watck_clock_swtich == watch_clock_week)//确认修改周期
		{
			u8  week=0;
            week=(TIME[5]/16)*10+TIME[5]%16;//BCD编码转换为十进制
			
			if(key0_PRES == 1)
				week+=1;
			else if(key1_PRES == 1)
				week-=1;
			else
				week = week;
            
			if(week>=1 && week <= 7)
				TIME[5]=(week/10)*16+week%10;//十进制转换为BCD编码
			else
				TIME[5]=TIME[5];
		}
		
		else if(watck_clock_swtich == watch_clock_hour)//确认修改小时
		{
			u8  hour=0;
            hour=(TIME[2]/16)*10+TIME[2]%16;//BCD编码转换为十进制
            
			if(key0_PRES == 1)
				hour+=1;
			else if(key1_PRES == 1)
				hour-=1;
			else
				hour = hour;
			
            if(hour>=1 && hour <= 24)
				TIME[2]=(hour/10)*16+hour%10;//十进制转换为BCD编码
			else
				TIME[2]=TIME[2];
		
		}
		
		else if(watck_clock_swtich == watch_clock_minute)//确认进入分钟修改
		{
			u8  minute=0;
            minute=(TIME[1]/16)*10+TIME[1]%16;//BCD编码转换为十进制
            
			if(key0_PRES == 1)
				minute+=1;
			else if(key1_PRES == 1)
				minute-=1;
			else
				minute = minute;
            
            TIME[1]=(minute/10)*16+minute%10;//十进制转换为BCD编码
		}
		
		else if(watck_clock_swtich == watch_clock_sec)//确认进入秒修改
		{
			 u8  second=0;
            second=(TIME[0]/16)*10+TIME[0]%16;//BCD编码转换为十进制
            
			if(key0_PRES == 1)
				second+=1;
			else if(key1_PRES == 1)
				second-=1;
			else
				second = second;
            
            TIME[0]=(second/10)*16+second%10;//十进制转换为BCD编码
		}
		
		else
			flag_display =flag_display;
	}
}




