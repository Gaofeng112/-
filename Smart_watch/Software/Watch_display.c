#include "Watch_display.h"



void Display(void)
{
	if(flag_display == 1)
	{
		if(watch_display_first == flag_watch)//�ֱ�ͼ�ν���
			display_watch();
		else if(watch_display_first == flag_heart)//����ͼ�ν���
			display_heart();
		else if(watch_display_first == flag_tmeperature)//�¶�ͼ�ν���
			display_tmeperature();
		else if(watch_display_first == flag_clock)//ʱ��ͼ�ν���
			display_clock();
		else if(watch_display_first == flag_step)//����ͼ�ν���
			display_step();
		else
			display_test();
	}
	else if(flag_display == 2)
	{
		if(watch_display_first == flag_watch)//�ֱ����
			display_watch_second();
		else if(watch_display_first == flag_heart)//���ʼ�����
			display_heart_second();
		else if(watch_display_first == flag_tmeperature)//�¶ȼ�����
			display_tmeperature_second();
		else if(watch_display_first == flag_clock)//ʱ�ӽ���
			display_clock_second();
		else if(watch_display_first == flag_step)//����������
			display_step_second();
		else
			display_test();
	}
	else if(flag_display == 3) //ʱ����������
	{
		display_clock_third();//ʱ����������
	}
	else
		display_test();
}

void display_test(void)
{
	OLED_Clear();
	OLED_Printf(30,30,OLED_8X16,"���");
	OLED_Update();
}

void display_watch(void) //������
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

void display_watch_second(void) //������-�ӽ���
{
	DS1302ReadTime();							//��DS1302��ȡ����
	DS1302_Time_Byte_Transform();				//��ȡ��������ת�����ַ���
	
	OLED_Clear();								//OLED����
	OLED_ShowString(20,3,YMD_Buff,OLED_8X16);	//��ʾ�������ַ���
	OLED_ShowString(30,23,Time_Buff,OLED_8X16);	//��ʾ�ַ���ʱ����
	OLED_ShowChinese(44,43,"��");				//��ʾ���֡��ܡ�
	OLED_ShowString(64,43,Week,OLED_8X16);		//��ʾ�ַ���6
	
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
			//��ȡFIFO
			max30102_read_fifo();
		}
	}
	
	blood_Loop();				//��ȡѪ��������

//	OLED_Clear();
//	OLED_ShowString(20,20,"2_B",OLED_8X16);
//	OLED_Update();
}

void display_tmeperature_second(void)
{
	DS18B20_Get_Temp();
	Temp_Data_Transform();
	OLED_Clear();
	OLED_ShowChinese(0,0,"�¶�");
	OLED_ShowChinese(30,0,"��");
	OLED_Printf(40,0,OLED_8X16,"%s",Temp_Buff);
	OLED_ShowChinese(75,0,"��");
	if(Temp_real<31)
	{
		OLED_ShowChinese(0,20,"���飺����������");
		OLED_ShowChinese(0,40,"�ഩ�·�");
	}
	else
	{
		OLED_ShowChinese(0,20,"���飺�������ȣ�");
		OLED_ShowChinese(0,40,"�ٴ��·�");
	}
	OLED_Update();
	UsartPrintf(USART1,"�¶ȣ�%s\r\n",Temp_Buff);
}

void display_clock_second(void)  //ʱ���Ӳ˵���ʾ��ѡ��
{
//	DS1302_Time_Update();
	DS1302ReadTime();							//��DS1302��ȡ����
	DS1302_Time_Byte_Transform();				//��ȡ��������ת�����ַ���
	
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




	switch(watck_clock_swtich)  //����ƶ�����
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
	TIM_Cmd(TIM2, ENABLE);			//ʹ��TIM2����ʱ����ʼ����
	ReadData_x();//�ӼƲ�ģ���л�ȡx,y,z��������
//	stepCount=0;
//	int w = 6;
	OLED_Clear();
	OLED_ShowChinese(0,0,"������");
	OLED_Printf(50,0,OLED_8X16,"%d",stepCount);
//	OLED_Printf(20,40,OLED_8X16,"Z:%f",temp_Z);
	if(stepCount>5)
		OLED_ShowChinese(0,20,"���飺��Ҫ��ˮ");
	else
		OLED_ShowChinese(0,20,"���飺�����˶���");
	
	OLED_Update();
	UsartPrintf(USART1,"����%3d\r\n",stepCount);
	if(key3_PRES)stepCount=0;
	
}


void display_clock_third(void)
{
	
//	DS1302ReadTime();							//��DS1302��ȡ����

	DS1302_Time_Update();						//�����޸ĺõ�����
	DS1302_Time_Byte_Transform();				//��ȡ��������ת�����ַ���
	
	//��ȡʱ�ڵ�ʱ������
	u8 year[5]={YMD_Buff[0],YMD_Buff[1],YMD_Buff[2],YMD_Buff[3]};
    u8 weak[2]={Week[0]};
    u8 month[3]={YMD_Buff[5],YMD_Buff[6]};
    u8 day[3]={YMD_Buff[8],YMD_Buff[9]};
    u8 hour[3]={Time_Buff[0],Time_Buff[1]};
    u8 minute[3]={Time_Buff[3],Time_Buff[4]};
    u8 second[3]={Time_Buff[6],Time_Buff[7]};
						
	
	OLED_Clear();	
	OLED_Printf(0,0,OLED_6X8,"back");
	
	if(watck_clock_swtich==watch_clock_year)		//ѡ���޸���ݣ��ͻ���ʾ���м乩�޸�
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


void DS1302_Time_Update(void)//����ʱ������
{
    uint8_t i;
    DS1302Write(0x8E,0x00);//�ر�DS302��д�����������û�д������
        for(i=0;i<7;i++)
            DS1302Write(WRITE_RTC_ADDR[i],TIME[i]);//��ʼ��ʱ��
    DS1302Write(0x8E,0x80);//��DS302��д��������ֹ�û�д������
}



