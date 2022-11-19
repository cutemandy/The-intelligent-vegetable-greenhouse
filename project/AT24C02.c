#include <REGX52.H>
#include "I2C.h"

#define AT24C02_ADDRESS		0xA0



void AT24C02_WriteByte(unsigned char WordAddress,Data)
{
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS);
	I2C_ReceiveAck();
	I2C_SendByte(WordAddress);//发送要 存入的地址
	I2C_ReceiveAck();//接受应答位
	I2C_SendByte(Data);//写入要存入的数据
	I2C_ReceiveAck();
	I2C_Stop();
}



unsigned char AT24C02_ReadByte(unsigned char WordAddress)
{
	unsigned char Data;
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS);//发出写入命令0X40
	I2C_ReceiveAck();//接受应答位
	I2C_SendByte(WordAddress);//发出要读出的地址
	I2C_ReceiveAck();
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS|0x01);//发送读出命令0xA1
	I2C_ReceiveAck();
	Data=I2C_ReceiveByte();//读出该地址的数据
	I2C_SendAck(1);
	I2C_Stop();
	return Data;
}
