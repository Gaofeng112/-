#include "ds1302.h"

//---DS1302写入和读取时分秒的地址命令---//
//---秒分时日月周年 最低位读写位;-------//
uint8_t  READ_RTC_ADDR[7]  = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d};//DS1302时间读取地址命令(顺序为：秒分时日月周年)
uint8_t  WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};//DS1302时间写入地址命令(顺序为：秒分时日月周年)

//---DS1302时钟初始化2024年4月20日星期六16点45分30秒。
uint8_t TIME[7] = {0x00, 0x09, 0x20, 0x06, 0x06, 0x04, 0x24};//时间数据存储数组(存储顺序是秒分时日月周年,存储格式是用BCD码)

uint8_t Year=20;           //世纪
uint8_t YMD_Buff [11]={'0','0','0','0','-','0','0','-','0','0','\0'}; //存储年月日的字符数组
uint8_t Time_Buff[9]={'0','0',':','0','0',':','0','0','\0'}; //存储时分秒的字符数组
uint8_t Week[2]={'0','\0'};       //星期


void DS1302_OUT_GPIO_INIT(void)//配置DS1302的引脚为推挽输出模式
{

	GPIO_InitTypeDef  GPIO_InitStructure;  


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);


}

void DS1302_IN_GPIO_INIT(void)//配置DS1302的SDA引脚为上拉输入模式
{
    GPIO_InitTypeDef  GPIO_InitStructure;  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Delay1us(void)//延时函数
{
    delay_us(1);
}

void DS1302Write(uint8_t addr, uint8_t dat)//主机向从机DS1302写入数据函数
{
	uint8_t n;
    DS1302_OUT_GPIO_INIT();
	RST_L;
	Delay1us();

	SCLK_L;//先将SCLK置低电平。
	Delay1us();
	RST_H; //然后将RST(CE)置高电平。
	Delay1us();

	for (n=0; n<8; n++)//开始传送八位地址命令
	{
        if(addr & 0x01)
        {
            DSIO_H;//数据从低位开始传送
        }
        else
        {
            DSIO_L;
        }
		addr >>= 1;
		SCLK_H;//数据在上升沿时，DS1302读取数据
		Delay1us();
		SCLK_L;
		Delay1us();
	}

	for (n=0; n<8; n++)//写入8位数据
	{
		if(dat & 0x01)
        {
            DSIO_H;//数据从低位开始传送
        }
        else
        {
            DSIO_L;
        }
		dat >>= 1;
		SCLK_H;//数据在上升沿时，DS1302读取数据
		Delay1us();
		SCLK_L;
		Delay1us();	
	}	

	RST_L;//传送数据结束
	Delay1us();
}

uint8_t DS1302Read(uint8_t addr)//主机向从机DS1302读出数据函数
{
	uint8_t n,dat,dat1;
	RST_L;
	Delay1us();

	SCLK_L;//先将SCLK置低电平。
	Delay1us();
	RST_H;//然后将RST(CE)置高电平。
	Delay1us();
    
	for(n=0; n<8; n++)//开始传送八位地址命令
	{
		if(addr & 0x01)
        {
            DSIO_H;//数据从低位开始传送
        }
        else
        {
            DSIO_L;
        }
		addr >>= 1;
		SCLK_H;//数据在上升沿时，DS1302读取数据
		Delay1us();
		SCLK_L;//DS1302下降沿时，放置数据
		Delay1us();
	}
	Delay1us();
    DS1302_IN_GPIO_INIT();
	for(n=0; n<8; n++)//读取8位数据
	{
		dat1 = GET_DSIO;//从最低位开始接收
		dat = (dat>>1) | (dat1<<7);
		SCLK_H;
		Delay1us();
		SCLK_L;//DS1302下降沿时，放置数据
		Delay1us();
	}
    DS1302_OUT_GPIO_INIT();
	RST_L;
	Delay1us();	//以下为DS1302复位的稳定时间,必须的。
	SCLK_H;
	Delay1us();
	DSIO_L;
	Delay1us();
	DSIO_H;
	Delay1us();
	return dat;	
}

void DS1302Init(void)//DS1302初始化函数
{
	uint8_t n;
	DS1302Write(0x8E,0X00);//禁止写保护功能，允许主机向DS1302写入数据或指令
	for (n=0; n<7; n++)//写入7个字节的时钟信号：分秒时日月周年
	{
		DS1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
	}
	DS1302Write(0x8E,0x80);//打开写保护功能，禁止主机向DS1302写入数据或指令
}

void DS1302ReadTime(void)//获取DS1302内部的时间数据函数
{
	uint8_t n;
	for (n=0; n<7; n++)//读取7个字节的时钟信号：分秒时日月周年
	{
		TIME[n] = DS1302Read(READ_RTC_ADDR[n]);
	}
		
}

//uint8_t Year=20;           //世纪
//uint8_t YMD_Buff [11]={0}; //存储年月日的字符数组
//uint8_t Time_Buff[9]={0}; //存储时分秒的字符数组
//uint8_t Week[2]={0};       //星期

void    DS1302_Time_Byte_Transform(void)//将获取的时间数据(BCD编码)进行字符转换
{
		DS1302ReadTime();//获取万年历时间数据
        {//年月日
            //20世纪
            YMD_Buff[0]=Year/10+0x30;//取十位         
            YMD_Buff[1]=Year%10+0x30;//取个位

            //年份
            YMD_Buff[2]=TIME[6]/16+0x30;//取十位            
            YMD_Buff[3]=TIME[6]%16+0x30;//取个位

            //月份
            YMD_Buff[4]='-';
            YMD_Buff[5]=TIME[4]/16+0x30;//取十位
            YMD_Buff[6]=TIME[4]%16+0x30;//取个位

            //日期
            YMD_Buff[7]='-';      
            YMD_Buff[8]=TIME[3]/16+0x30;//取十位             
            YMD_Buff[9]=TIME[3]%16+0x30;//取个位
            YMD_Buff[10]='\0';//结束符号
        }

        //周
            Week[0]=TIME[5]+0x30;
            Week[1] = '\0';

        {//时间
            //小时
            Time_Buff[0]=TIME[2]/16+0x30;//取十位            
            Time_Buff[1]=TIME[2]%16+0x30;//取个位
            
            //分钟
            Time_Buff[2]=':';
            Time_Buff[3]=TIME[1]/16+0x30;
            Time_Buff[4]=TIME[1]%16+0x30;

            //秒钟
            Time_Buff[5]=':';
            Time_Buff[6]=TIME[0]/16+0x30;
            Time_Buff[7]=TIME[0]%16+0x30;
            Time_Buff[8]='\0';//结束符号
        }



}
