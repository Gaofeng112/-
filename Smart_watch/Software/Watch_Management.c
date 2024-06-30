#include "Watch_Management.h"
#include "Watch_display.h"

u8 flag_display = 1;//�����ʶ�� 1=һ�����桢2=�������桢3=�������棬OLEDֻ����ʾ��������Ļ��� 
u8 watch_display_first = flag_watch; 	//һ������ͼ��ѡ��5������֮һ��
u8 watch_display_second = 1;			//��������ͼ���л�
u8 watck_clock_swtich = watch_clock_back; //ʱ���ӹ���



//�ú���������������1-5����
void change_first(void)
{
	if(watch_display_first == 0)
		watch_display_first=flag_step;
	else if(watch_display_first == 6)
		watch_display_first= flag_watch;
	else
		watch_display_first = watch_display_first;
}

//��������������
void key_switch_first(void)
{
	//��������ʱ
	if(flag_display==1) 
	{
		if(key0_PRES == 1)
		{
			watch_display_first-=1;  //����0����
			change_first();			 //����������Ƿ�С��1
		}
		else if(key1_PRES == 1)
		{
			watch_display_first+=1;	//����1 ����
			change_first();			//����������Ƿ����5
		}
		else if(key2_PRES == 1)
		{
			flag_display=2;
		}
	}
	
	//�ڶ�������
	else if(flag_display==2)//�ڵڶ�������
	{
		if(watch_display_first == 4) //����ʱ�ӽ���
		{
			if(key0_PRES==1)//���ƹ�����һ������
			{
				watck_clock_swtich = watck_clock_swtich << 7 | watck_clock_swtich >> 1;
			}
			else if(key1_PRES == 1)//���ƹ�����һ������
			{
				watck_clock_swtich = watck_clock_swtich >> 7 | watck_clock_swtich << 1;
			}
			else if(key2_PRES == 1)//ȷ��
			{
				if(watck_clock_swtich == watch_clock_back) //ȷ�Ϸ���
				{
					flag_display = 1;
				}
				else //�������������������
				{
					flag_display = 3;
				}
			}
		}
		else if(key2_PRES == 1)//����2�˳�������
		{
			flag_display = 1;
		}
		else
			flag_display = flag_display;
	}
	
	//����������
	else if(flag_display == 3)
	{
//		DS1302ReadTime();							//��DS1302��ȡ����
//		DS1302_Time_Byte_Transform();				//��ȡ��������ת�����ַ���
		
		
		if(key2_PRES == 1)//���°���2���˳�
		{
			flag_display = 2;
		}
		
		
		else if(watck_clock_swtich == watch_clock_year)//ȷ���޸����
		{
			u8  year=0;
            year=(TIME[6]/16)*10+TIME[6]%16;//BCD����ת��Ϊʮ���� 0X23ȡ��23
			
			if(key0_PRES == 1)
				year+=1;
			else if(key1_PRES == 1)
				year-=1;
			else
				year = year;
			
			
			TIME[6]=(year/10)*16+year%10;//ʮ����ת��ΪBCD����
		
		}
		
		else if(watck_clock_swtich == watch_clock_month)//ȷ���޸��·�
		{
			u8  month=0;
            month=(TIME[4]/16)*10+TIME[4]%16;//BCD����ת��Ϊʮ����
			
			if(key0_PRES == 1)
				month+=1;
			else if(key1_PRES == 1)
				month-=1;
			else
				month = month;
			
			
			if(month>=1&&month<=12)
				TIME[4]=(month/10)*16+month%10;//ʮ����ת��ΪBCD����
			else
				TIME[4]=TIME[4];
		
		}
		
		else if(watck_clock_swtich == watch_clock_day)//ȷ���޸�����
		{
			u8  day=0;
            day=(TIME[3]/16)*10+TIME[3]%16;//BCD����ת��Ϊʮ����
			
			if(key0_PRES == 1)
				day+=1;
			else if(key1_PRES == 1)
				day-=1;
			else
				day = day;
			
			if(day>=1 && day <= 31)
				TIME[3]=(day/10)*16+day%10;//ʮ����ת��ΪBCD����
			else
				TIME[3]=TIME[3];
		}
		
		else if(watck_clock_swtich == watch_clock_week)//ȷ���޸�����
		{
			u8  week=0;
            week=(TIME[5]/16)*10+TIME[5]%16;//BCD����ת��Ϊʮ����
			
			if(key0_PRES == 1)
				week+=1;
			else if(key1_PRES == 1)
				week-=1;
			else
				week = week;
            
			if(week>=1 && week <= 7)
				TIME[5]=(week/10)*16+week%10;//ʮ����ת��ΪBCD����
			else
				TIME[5]=TIME[5];
		}
		
		else if(watck_clock_swtich == watch_clock_hour)//ȷ���޸�Сʱ
		{
			u8  hour=0;
            hour=(TIME[2]/16)*10+TIME[2]%16;//BCD����ת��Ϊʮ����
            
			if(key0_PRES == 1)
				hour+=1;
			else if(key1_PRES == 1)
				hour-=1;
			else
				hour = hour;
			
            if(hour>=1 && hour <= 24)
				TIME[2]=(hour/10)*16+hour%10;//ʮ����ת��ΪBCD����
			else
				TIME[2]=TIME[2];
		
		}
		
		else if(watck_clock_swtich == watch_clock_minute)//ȷ�Ͻ�������޸�
		{
			u8  minute=0;
            minute=(TIME[1]/16)*10+TIME[1]%16;//BCD����ת��Ϊʮ����
            
			if(key0_PRES == 1)
				minute+=1;
			else if(key1_PRES == 1)
				minute-=1;
			else
				minute = minute;
            
            TIME[1]=(minute/10)*16+minute%10;//ʮ����ת��ΪBCD����
		}
		
		else if(watck_clock_swtich == watch_clock_sec)//ȷ�Ͻ������޸�
		{
			 u8  second=0;
            second=(TIME[0]/16)*10+TIME[0]%16;//BCD����ת��Ϊʮ����
            
			if(key0_PRES == 1)
				second+=1;
			else if(key1_PRES == 1)
				second-=1;
			else
				second = second;
            
            TIME[0]=(second/10)*16+second%10;//ʮ����ת��ΪBCD����
		}
		
		else
			flag_display =flag_display;
	}
}




