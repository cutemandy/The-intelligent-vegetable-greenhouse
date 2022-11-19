#include <REGX52.H>
#include "delay.h"
#include "I2C.h"
#include <intrins.h>
#define BH1750_WriteCommand  0x46//IIC对BH1750写入指令
#define BH1750_ReceiveCommand 0x47//读出指令




void BH1750_Init()//通电
{
	I2C_Start();
	I2C_SendByte(BH1750_WriteCommand);
	I2C_ReceiveAck();
	I2C_SendByte(0x01);
	I2C_ReceiveAck();
	I2C_Stop();
}
void BH1750_Update()//高分辨率测量
{
	BH1750_Init();//通电
	I2C_Start();
	I2C_SendByte(BH1750_WriteCommand);
	I2C_ReceiveAck();
	I2C_SendByte(0x10);//一次高分辨率指令为0001 0000
	I2C_ReceiveAck();
	I2C_Stop();
	
}

unsigned int BH1750_Read()
{
	
	unsigned char hbyte,lbyte;
	unsigned int guangzhao;
	EA=0;
	I2C_Start();
	I2C_SendByte(BH1750_ReceiveCommand);
	I2C_ReceiveAck();
	hbyte=I2C_ReceiveByte();
	I2C_SendAck(0);
	lbyte=I2C_ReceiveByte();
	I2C_SendAck(1);
	I2C_Stop();
	guangzhao=(hbyte*256+lbyte)/1.2;//数据处理 实际值为16位数值除以1.2
	EA=1;
	return guangzhao;
	
}
