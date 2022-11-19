#include <REGX52.H>
#include "delay.h"
#include "LCD1602.h"
#include "DHT22.h"
#include "AT24C02.h"
#include "Timer0.h"
#include "Key.h"
#include "BH1750.h"

sbit motor=P2^3;
sbit hot=P2^4;
sbit jiashi=P3^6;
sbit chushi=P3^7;
sbit buzzer=P3^5;
unsigned char keynum,slectflag,flash;
unsigned char RepublicOfChina;
unsigned char mode,lflag;
unsigned char TH_newflag,LX_newflag;
unsigned int Lx,i,j,compare;
unsigned char set[]={0,0,0,0};//依次存RL RH TL TH
unsigned int LXset[]={0,0};//依次存LL LH
unsigned int  dianjif[]={0x01,0x02,0x04,0x08};//步进电机反转数组
unsigned int  dianjiz[]={0x08,0x04,0x02,0x01};//步进电机正转数组
unsigned char dakai=1,guanbi=0;//默认遮阳板关
unsigned char temp1,temp2,temp3,temp4;



void setTH()//设置温湿度报警阈值
{
	if(keynum==2)//按下“切换当前位”按键
	{
		slectflag++;
		if(slectflag>=4){slectflag=0;}
	}
	if(keynum==3)//当前位+1
	{
		set[slectflag]++;
		TH_newflag=1;//阈值更新后 温湿度存储标志位置1
	}
	if(keynum==4)//当前位-1
	{
		set[slectflag]--;
		TH_newflag=1;//阈值更新后 温湿度存储标志位置1
	}
	if(set[0]>=set[1]){set[0]=set[1]-1;}//越界判断
	if(set[2]>=set[3]){set[2]=set[3]-1;}
	
	if(TH_newflag==1)//阈值更新后才存入EEPROM 否则不存
	{
		EA=0;//存值的时候关闭定时中断入口EA 以免出错
		AT24C02_WriteByte(0,set[0]);//存RL
		delay(5); 
		AT24C02_WriteByte(1,set[1]);//存RH
		delay(5);
		AT24C02_WriteByte(2,set[2]);//存TL
		delay(5);
		AT24C02_WriteByte(3,set[3]);//存TH
		delay(5);
		EA=1;
		TH_newflag=0;
	}

	
	//以下为报警阈值在LCD显示部分代码
	LCD_ShowString(1,1,"H:");
	LCD_ShowString(1,5,"%-");
	if(slectflag==0 && flash==1){LCD_ShowString(1,3,"  ");}//闪烁
	else {LCD_ShowNum(1,3,set[0],2);}//H下限
	if(slectflag==1 && flash==1){LCD_ShowString(1,7,"  ");}
	else {LCD_ShowNum(1,7,set[1],2);}//H上限
	LCD_ShowString(1,9,"%");
	LCD_ShowString(1,10,"       ");
	
	
	LCD_ShowString(2,1,"T:");
	LCD_ShowChar(2,5,0xDF);
	LCD_ShowString(2,6,"C-");
	if(slectflag==2 && flash==1){LCD_ShowString(2,3,"  ");}
	else {LCD_ShowNum(2,3,set[2],2);}//T下限
	LCD_ShowChar(2,5,0xDF);
	if(slectflag==3 && flash==1){LCD_ShowString(2,8,"  ");}
	else {LCD_ShowNum(2,8,set[3],2);}//T上限
	LCD_ShowChar(2,10,0xDF);
	LCD_ShowString(2,11,"C");
	LCD_ShowString(2,12,"     ");
}




void setLX()//设置光强报警阈值
{
	if(keynum==2)//按下“切换当前位”按键
	{
		lflag++;
		if(lflag>=2){lflag=0;}
	}
	if(keynum==3)//当前位+1
	{
		LXset[lflag]++;
		LX_newflag=1;//阈值更新后 光照存储标志位置1
	}
	if(keynum==4)//当前位-1
	{
		LXset[lflag]--;
		LX_newflag=1;//阈值更新后 光照存储标志位置1
	}
	if(LXset[0]>=LXset[1]){LXset[0]=LXset[1]--;}//越值判断
	temp1=LXset[0]/256;//将16位光照阈值 拆为两个8位 存EEPROM
	temp2=LXset[0]%256;
	temp3=LXset[1]/256;
	temp4=LXset[1]%256;
	if(LX_newflag==1)//更新阈值后才存数据 否则不存
	{
		EA=0;//存值时期关闭定时中断入口 避免出错
		AT24C02_WriteByte(4,temp1);
		delay(5); 
		AT24C02_WriteByte(5,temp2);//存LL
		delay(5);
		AT24C02_WriteByte(6,temp3);
		delay(5); 
		AT24C02_WriteByte(7,temp4);//存LH
		delay(5);
		EA=1;
		LX_newflag=0;
	}
	//以下为光强阈值在LCD显示部分代码
	
	LCD_ShowString(1,1,"light limit is:");
	
	if(lflag==0 && flash==1){LCD_ShowString(2,1,"     ");}
	else {LCD_ShowNum(2,1,LXset[0],5);}//LX下限
	if(lflag==1 && flash==1){LCD_ShowString(2,9,"     ");}
	else {LCD_ShowNum(2,9,LXset[1],5);}//LX上限
	LCD_ShowString(2,6,"Lx-");
	LCD_ShowString(2,14,"Lx");
	
}




void THwarn()
{
	
		if(TM1>=set[3] || TM1<set[2])//温度异常
			{
				
				if(TM1>=set[3])//温度过高 制冷
					{
						if(TM1<(set[3]/2)+set[3])//不超过1.5倍
						{
							compare=1500;//占空比调为50% 半速运行
						}
						else
						{
							compare=3000;//占空比100% 全速运行
						}
					}
				else
					{
						compare=0;//关闭制冷
					}
				if(TM1<set[2])//温度过低 开启加热
					{
						hot=0;//开启加热
						//compare=0;
					}
				else
					{
						//compare=0;
						//motor=1;
						hot=1;//关闭加热
					}
			}
			else//温度正常 关闭温控设备
			{
				compare=0;
				hot=1;
			}
		
		if(TM3>=set[1] || TM3<set[0])//湿度异常
		{
			
			if(TM3>=set[1]){chushi=0;}//过高 开启除湿机
			if(TM3<set[0]){jiashi=0;}//过低 开启加湿器
		}
		else//湿度正常 关闭湿控设备
		{
			
			jiashi=1;
			chushi=1;
		}
		
}



void LXwarn()
{
	if(Lx>=LXset[1] || Lx<LXset[0])
	{
		if(Lx<LXset[0] && dakai==1 && guanbi==0)//光弱 步进电机正转
		{
			EA=0;//关闭定时中断入口
			for(i=0;i<2150;i++)
				{
					j=i;j=j%4;
					P1=dianjiz[j];
					delay(2);
				}
				dakai=0;guanbi=1;//标志位反转 遮阳棚已打开 如果继续光弱 电机也不能转动了
			EA=1;
		}
		if(Lx>=LXset[1] && guanbi==1 && dakai==0 )//光强 步进电机反转
		{
			EA=0;
			for(i=0;i<2150;i++)
				{
					j=i;j=j%4;
					P1=dianjif[j];
					delay(2);
				}
				dakai=1;guanbi=0;//标志位反转
			EA=1;
		}
		
	}
	else//光照正常 步进电机不动
	{
		P1=0x00;
	}
}



void buzzerwarn()//异常状态蜂鸣器报警
{
	if(TM1>=set[3] || TM1<set[2] || TM3>=set[1] || TM3<set[0] || Lx>=LXset[1] || Lx<LXset[0])
	{
		buzzer=0;
	}
	else
	{
		buzzer=1;
	}
}



void showTH()
{
		
		LCD_ShowString(1,1,"H:");//环境参数送显示
		LCD_ShowNum(1,3,TM3,2);	
		LCD_ShowString(1,5,".");
		LCD_ShowNum(1,6,TM4,1);
		LCD_ShowString(1,7,"%");
		LCD_ShowString(1,8," ");
	

	
	
		LCD_ShowString(1,9,"T:"); 
		LCD_ShowNum(1,11,TM1,2);
		LCD_ShowString(1,13,".");
		LCD_ShowNum(1,14,TM2,1);
		LCD_ShowChar(1,15,0xDF);
		LCD_ShowString(1,16,"C");
		
	
	
	
		LCD_ShowString(2,1,"L:");
		LCD_ShowNum(2,3,Lx,5);
		LCD_ShowString(2,8,"Lx");
		LCD_ShowString(2,10,"        ");
		
		
		
}




void main()
{
	LCD_Init();//LCD初始化
	set[0]=AT24C02_ReadByte(0);//读出报警阈值
	set[1]=AT24C02_ReadByte(1);
	set[2]=AT24C02_ReadByte(2);
	set[3]=AT24C02_ReadByte(3);
	temp1=AT24C02_ReadByte(4);
	temp2=AT24C02_ReadByte(5);
	temp3=AT24C02_ReadByte(6);
	temp4=AT24C02_ReadByte(7);
	if(set[0]>=set[1] || set[2]>=set[3] || set[0]>99 || set[1]>99 || set[2]>99 || set[3]>99
	  	|| set[0]<0 || set[1]<0 || set[2]<0 || set[3]<0)//若读出的报警阈值异常 则置初值
	{
		EA=0;
		set[0]=40;
		
		set[1]=60;
		set[2]=21;
		set[3]=28;
		AT24C02_WriteByte(0,set[0]);//存RL
		delay(5);
		AT24C02_WriteByte(1,set[1]);//存RH
		delay(5);
		AT24C02_WriteByte(2,set[2]);//存TL
		delay(5);
		AT24C02_WriteByte(3,set[3]);//存TH
		delay(5);
		EA=1;
	}
//	if(temp2<=0 || temp4<=0 || temp1<=0 || temp3<=0 || temp1>5 || temp3>5 )
//	{
//		temp1=0;
//		temp2=20;
//		temp3=0;
//		temp4=50;
//		EA=0;
//		AT24C02_WriteByte(4,temp1);
//		delay(5); 
//		AT24C02_WriteByte(5,temp2);//存LL
//		delay(5);
//		AT24C02_WriteByte(6,temp3);
//		delay(5); 
//		AT24C02_WriteByte(7,temp3);//存LH
//		delay(5);
//		EA=1;
//	}
	LXset[0]=(temp1*256)+temp2;//读出光强阈值后 数据处理得到16位
	LXset[1]=(temp3*256)+temp4;
	Timer0Init();//定时器初始化 1ms进入一次定时中断
	motor=1;
	hot=1;
	buzzer=1;
	chushi=1;
	jiashi=1;
	P1=0x00;//以上几行为初始化
	delay(2000);
	while(1)
	{
		keynum=Key();//获取键盘扫描
		getHT();//采集温湿度并读出
		delay(2000);//2s采集一次温湿度
		BH1750_Update();
		delay(500);//500ms采集一次光强
		Lx=BH1750_Read();//采集光强并读出
		if(keynum==1)//按下设置键 改变模式标志位0、1、2循环
		{
			
			if(mode==0){mode=1;slectflag=0;}
			else if(mode==1){mode=2;lflag=0;}
			else {mode=0;}
		}
		switch(mode)
		{
			case 0:showTH();break;//环境参数送显
			case 1:setTH();break;//设置温湿度报警阈值
			case 2:setLX();break;//设置光强报警阈值
			default: break;
		}
		buzzerwarn();//蜂鸣器模块
		THwarn();//温、湿控制模块
		LXwarn();//光控模块
	
	}
}



void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count,count2,count3;
		
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count>=20)
	{
		T0Count=0;
		Key_Loop();//20ms扫描一次键盘
	}
	count2++;
	if(count2>=500)
	{
		count2=0;
		flash=!flash;//设置阈值功能 闪烁用 500ms标志位反转
	}
	
		count3++;
		count3%=3000;//PWM周期为3秒
	
		if(count3<compare)
		{
			motor=0;
		}

		else
		{
			motor=1;//关闭降温
		}
	
	
	
}