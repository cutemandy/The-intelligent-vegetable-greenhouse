#ifndef __LCD1602_H__
#define __LCD1602_H__

//用户调用函数：
void LCD_Init();//初始化
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char);
//显示一个字符LCD_ShowChar(1,1,'A');

void LCD_ShowString(unsigned char Line,unsigned char Column,char *String);
//显示字符串LCD_ShowString(1,3,"Hello");

void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
//显示十进制数字LCD_ShowNum(1,9,123,3);

void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length);
//显示有符号十进制数字LCD_ShowSignedNum(1,13,-66,2);

void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
//显示十六进制数字LCD_ShowHexNum(2,1,0xA8,2);

void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
//显示二进制数字LCD_ShowBinNum(2,4,0xAA,8);

#endif
