#include "ds18b20.h"
#include "usart.h"

char Temp_Buff[5]={0};//温度数据字符数组
float Temp_real=0;

static void DS18B20_GPIO_Config(void)//配置DS18B20用到的I/O口
{		
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE); 
															   
  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(DS18B20_PORT, DS18B20_PIN);	 //DS18B20数据引脚初始化配置为高电平输出
}

static void DS18B20_Mode_IPU(void)//配置使DS18B20-DATA引脚变为输入模式
{
 	  GPIO_InitTypeDef GPIO_InitStructure;
	
	  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN; 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

static void DS18B20_Mode_Out_PP(void)//配置使DS18B20-DATA引脚变为输出模式
{
 	GPIO_InitTypeDef GPIO_InitStructure;
															   
  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

static void DS18B20_Rst(void)//主机给从机发送复位脉冲
{
	DS18B20_Mode_Out_PP();//主机设置为推挽输出 
	
	DS18B20_DATA_OUT(LOW);//主机至少产生480us的低电平复位信号
	delay_us(750);
	DS18B20_DATA_OUT(HIGH);//主机在产生复位信号后，需将总线拉高
	delay_us(15);          //从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
}

static u8 DS18B20_Presence(void)//检测从机给主机返回的存在脉冲(返回值为0及脉冲正确，返回值为1及脉冲错误)
{
	u8 pulse_time = 0;
	
	DS18B20_Mode_IPU();    //主机设置为上拉输入
	
  //等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号，如果存在脉冲没有来则做超时处理
	while( DS18B20_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
		delay_us(1);
	}	

	if( pulse_time >=100 )  //经过100us后，存在脉冲都还没有到来
		return 1;             //存在脉冲错误
	else                    //经过100us后，存在脉冲到来
		pulse_time = 0;       //清零计时变量
	
	while( !DS18B20_DATA_IN() && pulse_time<240 )  // 存在脉冲到来，且存在的时间不能超过240us
	{
		pulse_time++;
		delay_us(1);
	}	
	if( pulse_time >=240 ) // 存在脉冲到来，且存在的时间超过了240us
		return 1;            //存在脉冲错误
	else
		return 0;            //存在脉冲正确
}

static u8 DS18B20_Read_Bit(void)//从DS18B20读取一个bit数据
{
	u8 dat;
	
	//读0和读1的时间至少要大于60us
	DS18B20_Mode_Out_PP();
	//读时间的起始：必须由主机产生 >1us <15us 的低电平信号 
	DS18B20_DATA_OUT(LOW);
	delay_us(10);
	
	//设置成输入，释放总线，由外部上拉电阻将总线拉高
	DS18B20_Mode_IPU();
	
	if( DS18B20_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	delay_us(45);//延时
	
	return dat;
}

u8 DS18B20_Read_Byte(void)//从DS18B20读一个字节，低位先行
{
	u8 i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();//从DS18B20读取一个bit
		dat = (dat) | (j<<i);
	}
	
	return dat;																																																																																
}

void DS18B20_Write_Byte(u8 dat)//写一个字节到DS18B20，低位先行
{
	u8 i, testb;
	DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;//移除最低位		
		//写0和写1的时间至少要大于60us
		if (testb)
		{			
			DS18B20_DATA_OUT(LOW);
			delay_us(8);   //1us < 这个延时 < 15us
			
			DS18B20_DATA_OUT(HIGH);
			delay_us(58);  //58us+8us>60us
		}		
		else
		{			
			DS18B20_DATA_OUT(LOW);  
			//60us < Tx 0 < 120us
			delay_us(70);
			
			DS18B20_DATA_OUT(HIGH);			
			//1us < Trec(恢复时间) < 无穷大
			delay_us(2);
		}
	}
}

void DS18B20_Start(void)//启动DS18B20，并开始温度转换
{
	DS18B20_Rst();	         //主机给DS18B20发送复位脉冲
	DS18B20_Presence();	     //检测DS18B20发送给从机的存在脉冲
	DS18B20_Write_Byte(0XCC);//跳过ROM 
	DS18B20_Write_Byte(0X44);//开始转换 
}

u8 DS18B20_Init(void)//DS18B20初始化函数
{
	DS18B20_GPIO_Config();//初始化驱动DS18B20传感器的引脚
	DS18B20_Rst();        //复位DS18B20传感器
	
	return DS18B20_Presence();//返回存在脉冲的检测结果
}

float DS18B20_Get_Temp(void)//从DS18B20读取温度值
{

	u8 tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
//	DS18B20_Init();
	
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);//跳过 ROM
	DS18B20_Write_Byte(0X44);//开始转换
	
	DS18B20_Rst();
	DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);//跳过 ROM
	DS18B20_Write_Byte(0XBE);//读温度值
	
	tplsb = DS18B20_Read_Byte();		 
	tpmsb = DS18B20_Read_Byte(); 
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
//	printf("原始数据：%d\r\n",s_tem);
	
	if( s_tem < 0 )	//温度
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = (s_tem * 0.625);
	    
	return f_tem; 	
}

void  Temp_Data_Transform(void)//将获取的温度数据进行转换
{
		float Temp_Value=0.0;//温度数据
		Temp_Value=DS18B20_Get_Temp();//DS18B20初始化函数
		Temp_real=Temp_Value/10;
//	  printf("温度：%f\r\n",Temp_Value);
	
	  Temp_Buff[0]=(int)(Temp_Value)%1000/100+'0';	
	  Temp_Buff[1]=(int)(Temp_Value)%100/10+'0';
	  Temp_Buff[2]='.';
	  Temp_Buff[3]=(int)(Temp_Value)%10+'0';
	  Temp_Buff[4]='\0';
	
}
