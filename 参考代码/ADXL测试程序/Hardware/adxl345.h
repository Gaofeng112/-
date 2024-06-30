

#define uint unsigned int
#define uchar unsigned char
	
extern float temp_X,temp_Y,temp_Z;


void SCL_Set_Output(void);
void SDA_Set_Output(void);
void SDA_Set_Input(void);
void Init_ADXL345(void);    
void  Single_Write_ADXL345(uchar REG_Address,uchar REG_data);   //单字节写入
uchar Single_Read_ADXL345(uchar REG_Address);                   //单字节读取
void  Multiple_Read_ADXL345(void);                              //连续读出ADXL345内部加速度数据
void ADXL345_Start(void);
void ADXL345_Stop(void);
void ADXL345_SendACK(uchar ack);
uchar  ADXL345_RecvACK(void);
void ADXL345_SendByte(uchar dat);
uchar ADXL345_RecvByte(void);
void ADXL345_ReadPage(void);
void ADXL345_WritePage(void);
void ReadData_x(void);

