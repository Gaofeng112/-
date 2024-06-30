#include "ds1302.h"

//---DS1302д��Ͷ�ȡʱ����ĵ�ַ����---//
//---���ʱ�������� ���λ��дλ;-------//
uint8_t  READ_RTC_ADDR[7]  = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d};//DS1302ʱ���ȡ��ַ����(˳��Ϊ�����ʱ��������)
uint8_t  WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};//DS1302ʱ��д���ַ����(˳��Ϊ�����ʱ��������)

//---DS1302ʱ�ӳ�ʼ��2024��4��20��������16��45��30�롣
uint8_t TIME[7] = {0x00, 0x09, 0x20, 0x06, 0x06, 0x04, 0x24};//ʱ�����ݴ洢����(�洢˳�������ʱ��������,�洢��ʽ����BCD��)

uint8_t Year=20;           //����
uint8_t YMD_Buff [11]={'0','0','0','0','-','0','0','-','0','0','\0'}; //�洢�����յ��ַ�����
uint8_t Time_Buff[9]={'0','0',':','0','0',':','0','0','\0'}; //�洢ʱ������ַ�����
uint8_t Week[2]={'0','\0'};       //����


void DS1302_OUT_GPIO_INIT(void)//����DS1302������Ϊ�������ģʽ
{

	GPIO_InitTypeDef  GPIO_InitStructure;  


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);


}

void DS1302_IN_GPIO_INIT(void)//����DS1302��SDA����Ϊ��������ģʽ
{
    GPIO_InitTypeDef  GPIO_InitStructure;  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Delay1us(void)//��ʱ����
{
    delay_us(1);
}

void DS1302Write(uint8_t addr, uint8_t dat)//������ӻ�DS1302д�����ݺ���
{
	uint8_t n;
    DS1302_OUT_GPIO_INIT();
	RST_L;
	Delay1us();

	SCLK_L;//�Ƚ�SCLK�õ͵�ƽ��
	Delay1us();
	RST_H; //Ȼ��RST(CE)�øߵ�ƽ��
	Delay1us();

	for (n=0; n<8; n++)//��ʼ���Ͱ�λ��ַ����
	{
        if(addr & 0x01)
        {
            DSIO_H;//���ݴӵ�λ��ʼ����
        }
        else
        {
            DSIO_L;
        }
		addr >>= 1;
		SCLK_H;//������������ʱ��DS1302��ȡ����
		Delay1us();
		SCLK_L;
		Delay1us();
	}

	for (n=0; n<8; n++)//д��8λ����
	{
		if(dat & 0x01)
        {
            DSIO_H;//���ݴӵ�λ��ʼ����
        }
        else
        {
            DSIO_L;
        }
		dat >>= 1;
		SCLK_H;//������������ʱ��DS1302��ȡ����
		Delay1us();
		SCLK_L;
		Delay1us();	
	}	

	RST_L;//�������ݽ���
	Delay1us();
}

uint8_t DS1302Read(uint8_t addr)//������ӻ�DS1302�������ݺ���
{
	uint8_t n,dat,dat1;
	RST_L;
	Delay1us();

	SCLK_L;//�Ƚ�SCLK�õ͵�ƽ��
	Delay1us();
	RST_H;//Ȼ��RST(CE)�øߵ�ƽ��
	Delay1us();
    
	for(n=0; n<8; n++)//��ʼ���Ͱ�λ��ַ����
	{
		if(addr & 0x01)
        {
            DSIO_H;//���ݴӵ�λ��ʼ����
        }
        else
        {
            DSIO_L;
        }
		addr >>= 1;
		SCLK_H;//������������ʱ��DS1302��ȡ����
		Delay1us();
		SCLK_L;//DS1302�½���ʱ����������
		Delay1us();
	}
	Delay1us();
    DS1302_IN_GPIO_INIT();
	for(n=0; n<8; n++)//��ȡ8λ����
	{
		dat1 = GET_DSIO;//�����λ��ʼ����
		dat = (dat>>1) | (dat1<<7);
		SCLK_H;
		Delay1us();
		SCLK_L;//DS1302�½���ʱ����������
		Delay1us();
	}
    DS1302_OUT_GPIO_INIT();
	RST_L;
	Delay1us();	//����ΪDS1302��λ���ȶ�ʱ��,����ġ�
	SCLK_H;
	Delay1us();
	DSIO_L;
	Delay1us();
	DSIO_H;
	Delay1us();
	return dat;	
}

void DS1302Init(void)//DS1302��ʼ������
{
	uint8_t n;
	DS1302Write(0x8E,0X00);//��ֹд�������ܣ�����������DS1302д�����ݻ�ָ��
	for (n=0; n<7; n++)//д��7���ֽڵ�ʱ���źţ�����ʱ��������
	{
		DS1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
	}
	DS1302Write(0x8E,0x80);//��д�������ܣ���ֹ������DS1302д�����ݻ�ָ��
}

void DS1302ReadTime(void)//��ȡDS1302�ڲ���ʱ�����ݺ���
{
	uint8_t n;
	for (n=0; n<7; n++)//��ȡ7���ֽڵ�ʱ���źţ�����ʱ��������
	{
		TIME[n] = DS1302Read(READ_RTC_ADDR[n]);
	}
		
}

//uint8_t Year=20;           //����
//uint8_t YMD_Buff [11]={0}; //�洢�����յ��ַ�����
//uint8_t Time_Buff[9]={0}; //�洢ʱ������ַ�����
//uint8_t Week[2]={0};       //����

void    DS1302_Time_Byte_Transform(void)//����ȡ��ʱ������(BCD����)�����ַ�ת��
{
		DS1302ReadTime();//��ȡ������ʱ������
        {//������
            //20����
            YMD_Buff[0]=Year/10+0x30;//ȡʮλ         
            YMD_Buff[1]=Year%10+0x30;//ȡ��λ

            //���
            YMD_Buff[2]=TIME[6]/16+0x30;//ȡʮλ            
            YMD_Buff[3]=TIME[6]%16+0x30;//ȡ��λ

            //�·�
            YMD_Buff[4]='-';
            YMD_Buff[5]=TIME[4]/16+0x30;//ȡʮλ
            YMD_Buff[6]=TIME[4]%16+0x30;//ȡ��λ

            //����
            YMD_Buff[7]='-';      
            YMD_Buff[8]=TIME[3]/16+0x30;//ȡʮλ             
            YMD_Buff[9]=TIME[3]%16+0x30;//ȡ��λ
            YMD_Buff[10]='\0';//��������
        }

        //��
            Week[0]=TIME[5]+0x30;
            Week[1] = '\0';

        {//ʱ��
            //Сʱ
            Time_Buff[0]=TIME[2]/16+0x30;//ȡʮλ            
            Time_Buff[1]=TIME[2]%16+0x30;//ȡ��λ
            
            //����
            Time_Buff[2]=':';
            Time_Buff[3]=TIME[1]/16+0x30;
            Time_Buff[4]=TIME[1]%16+0x30;

            //����
            Time_Buff[5]=':';
            Time_Buff[6]=TIME[0]/16+0x30;
            Time_Buff[7]=TIME[0]%16+0x30;
            Time_Buff[8]='\0';//��������
        }



}
