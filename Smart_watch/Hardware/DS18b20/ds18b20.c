#include "ds18b20.h"
#include "usart.h"

char Temp_Buff[5]={0};//�¶������ַ�����
float Temp_real=0;

static void DS18B20_GPIO_Config(void)//����DS18B20�õ���I/O��
{		
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE); 
															   
  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(DS18B20_PORT, DS18B20_PIN);	 //DS18B20�������ų�ʼ������Ϊ�ߵ�ƽ���
}

static void DS18B20_Mode_IPU(void)//����ʹDS18B20-DATA���ű�Ϊ����ģʽ
{
 	  GPIO_InitTypeDef GPIO_InitStructure;
	
	  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN; 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

static void DS18B20_Mode_Out_PP(void)//����ʹDS18B20-DATA���ű�Ϊ���ģʽ
{
 	GPIO_InitTypeDef GPIO_InitStructure;
															   
  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

static void DS18B20_Rst(void)//�������ӻ����͸�λ����
{
	DS18B20_Mode_Out_PP();//��������Ϊ������� 
	
	DS18B20_DATA_OUT(LOW);//�������ٲ���480us�ĵ͵�ƽ��λ�ź�
	delay_us(750);
	DS18B20_DATA_OUT(HIGH);//�����ڲ�����λ�źź��轫��������
	delay_us(15);          //�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
}

static u8 DS18B20_Presence(void)//���ӻ����������صĴ�������(����ֵΪ0��������ȷ������ֵΪ1���������)
{
	u8 pulse_time = 0;
	
	DS18B20_Mode_IPU();    //��������Ϊ��������
	
  //�ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�źţ������������û����������ʱ����
	while( DS18B20_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
		delay_us(1);
	}	

	if( pulse_time >=100 )  //����100us�󣬴������嶼��û�е���
		return 1;             //�����������
	else                    //����100us�󣬴������嵽��
		pulse_time = 0;       //�����ʱ����
	
	while( !DS18B20_DATA_IN() && pulse_time<240 )  // �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us
	{
		pulse_time++;
		delay_us(1);
	}	
	if( pulse_time >=240 ) // �������嵽�����Ҵ��ڵ�ʱ�䳬����240us
		return 1;            //�����������
	else
		return 0;            //����������ȷ
}

static u8 DS18B20_Read_Bit(void)//��DS18B20��ȡһ��bit����
{
	u8 dat;
	
	//��0�Ͷ�1��ʱ������Ҫ����60us
	DS18B20_Mode_Out_PP();
	//��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� 
	DS18B20_DATA_OUT(LOW);
	delay_us(10);
	
	//���ó����룬�ͷ����ߣ����ⲿ�������轫��������
	DS18B20_Mode_IPU();
	
	if( DS18B20_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	delay_us(45);//��ʱ
	
	return dat;
}

u8 DS18B20_Read_Byte(void)//��DS18B20��һ���ֽڣ���λ����
{
	u8 i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();//��DS18B20��ȡһ��bit
		dat = (dat) | (j<<i);
	}
	
	return dat;																																																																																
}

void DS18B20_Write_Byte(u8 dat)//дһ���ֽڵ�DS18B20����λ����
{
	u8 i, testb;
	DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;//�Ƴ����λ		
		//д0��д1��ʱ������Ҫ����60us
		if (testb)
		{			
			DS18B20_DATA_OUT(LOW);
			delay_us(8);   //1us < �����ʱ < 15us
			
			DS18B20_DATA_OUT(HIGH);
			delay_us(58);  //58us+8us>60us
		}		
		else
		{			
			DS18B20_DATA_OUT(LOW);  
			//60us < Tx 0 < 120us
			delay_us(70);
			
			DS18B20_DATA_OUT(HIGH);			
			//1us < Trec(�ָ�ʱ��) < �����
			delay_us(2);
		}
	}
}

void DS18B20_Start(void)//����DS18B20������ʼ�¶�ת��
{
	DS18B20_Rst();	         //������DS18B20���͸�λ����
	DS18B20_Presence();	     //���DS18B20���͸��ӻ��Ĵ�������
	DS18B20_Write_Byte(0XCC);//����ROM 
	DS18B20_Write_Byte(0X44);//��ʼת�� 
}

u8 DS18B20_Init(void)//DS18B20��ʼ������
{
	DS18B20_GPIO_Config();//��ʼ������DS18B20������������
	DS18B20_Rst();        //��λDS18B20������
	
	return DS18B20_Presence();//���ش�������ļ����
}

float DS18B20_Get_Temp(void)//��DS18B20��ȡ�¶�ֵ
{

	u8 tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
//	DS18B20_Init();
	
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);//���� ROM
	DS18B20_Write_Byte(0X44);//��ʼת��
	
	DS18B20_Rst();
	DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);//���� ROM
	DS18B20_Write_Byte(0XBE);//���¶�ֵ
	
	tplsb = DS18B20_Read_Byte();		 
	tpmsb = DS18B20_Read_Byte(); 
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
//	printf("ԭʼ���ݣ�%d\r\n",s_tem);
	
	if( s_tem < 0 )	//�¶�
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = (s_tem * 0.625);
	    
	return f_tem; 	
}

void  Temp_Data_Transform(void)//����ȡ���¶����ݽ���ת��
{
		float Temp_Value=0.0;//�¶�����
		Temp_Value=DS18B20_Get_Temp();//DS18B20��ʼ������
		Temp_real=Temp_Value/10;
//	  printf("�¶ȣ�%f\r\n",Temp_Value);
	
	  Temp_Buff[0]=(int)(Temp_Value)%1000/100+'0';	
	  Temp_Buff[1]=(int)(Temp_Value)%100/10+'0';
	  Temp_Buff[2]='.';
	  Temp_Buff[3]=(int)(Temp_Value)%10+'0';
	  Temp_Buff[4]='\0';
	
}
