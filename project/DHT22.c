#include <REGX52.H>
sbit DA=P2^2;
unsigned char wendu1,wendu2,shidu1,shidu2;
unsigned int wendu,shidu;
unsigned char TM1,TM2,TM3,TM4;
void DMS(unsigned int x_ms)		
{
	unsigned char i, j;
  while(x_ms)
	{	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
  x_ms--;
	}
}


unsigned char  DHT22_init()//初始化
{
	unsigned char i;
	DA=1;
	DA=0;
	

	DMS(20);								
	DA=1;
	while(DA==1);
	i = 27;while (--i);     
	if(DA==1) return 1;
	i = 37;while (--i);			
	if(DA==0) return 1;
	while(DA==1); 			
	
	return 0;
}


unsigned char ReadHH()		//读湿度高八位
{
	unsigned char i,n,HH=0x00;
	for(n=0;n<8;n++)
	{
		while(DA==0);
		
		while (DA==1)
		{
			i = 17;while (--i);	
			if(DA==1)
			HH=HH|(0X01<<(7-n));
		}
	}
	
	return HH;
}
	
unsigned char ReadHL()			//读湿度低八位
{
	unsigned char i,n,HL=0x00;
	for(n=0;n<8;n++)
	{
		while(DA==0);
		
		while (DA==1)
		{
			i = 17;while (--i);	
			if(DA==1)
			HL=HL|(0X01<<(7-n));
		}
	}
	
	return HL;
	
}

unsigned char ReadTH()				//读温度高八位
{
	unsigned char i,n,TH=0x00;
	for(n=0;n<8;n++)
	{
		while(DA==0);
		
		while (DA==1)
		{
			i = 17;while (--i);	
			if(DA==1)
			TH=TH|(0X01<<(7-n));
		}
	}
	
	return TH;
	
}

unsigned char ReadTL()			//读温度低八位
{
	unsigned char i,n,TL=0x00;
	for(n=0;n<8;n++)
	{
		while(DA==0);
		
		while (DA==1)
		{
			i = 17;while (--i);	
			if(DA==1)
			TL=TL|(0X01<<(7-n));
		}
	}
	
	return TL;
}

void getHT()//读出温湿度值
{
		EA=0;
		DHT22_init();		
		wendu1=ReadHH();
		wendu2=ReadHL();
		shidu1=ReadTH();
		shidu2=ReadTL();
		wendu=wendu1<<8|wendu2;//左移8位 加上低八位 组成16位，根据手册说明，该值为真实值的10倍
		shidu=shidu1<<8|shidu2;
		TM1=shidu/10;//得到实际值
		TM2=shidu%10;
		TM3=wendu/10;
		TM4=wendu%10;
		EA=1;
}





//#include <REGX52.H>
//#include <intrins.h>
//#define uchar unsigned char
//#define uint unsigned int
//uchar rec_dat[9];
//uint shidu,wendu;
//uchar shidu1,shidu2,wendu1,wendu2;
//sbit Data=P1^7;   //定义数据线

//void DHT22_delay_us(uchar n)
//{
//    while(--n);
//}

//void DHT22_delay_ms(uint z)
//{
//   uint i,j;
//   for(i=z;i>0;i--)
//      for(j=110;j>0;j--);
//}

//void DHT22_start()
//{
//   Data=1;
//   DHT22_delay_us(2);
//   Data=0;
//   DHT22_delay_ms(20);   //延时18ms以上
//   Data=1;
//   DHT22_delay_us(30);
//}

//uchar DHT22_rec_byte()      //接收一个字节
//{
//   uchar i,dat=0;
//  for(i=0;i<8;i++)    //从高到低依次接收8位数据
//   {          
//      while(!Data);  // 等待50us低电平过去
//      DHT22_delay_us(8);     //延时60us，如果还为高则数据为1，否则为0 
//      dat<<=1;           //移位使正确接收8位数据，数据为0时直接移位
//      if(Data==1)    //数据为1时，使dat加1来接收数据1
//         dat+=1;
//      while(Data);  //等待数据线拉低    
//    }  
//    return dat;
//}

//void DHT22_receive()      //接收40位的数据
//{
//    uchar R_H,R_L,T_H,T_L,revise; 
//		EA=0;
//    DHT22_start();
//    if(Data==0)
//    {
//        while(Data==0);   //等待拉高     
//        DHT22_delay_us(40);  //拉高后延时80us
//        R_H=DHT22_rec_byte();    //接收湿度高八位  
//        R_L=DHT22_rec_byte();    //接收湿度低八位  
//        T_H=DHT22_rec_byte();    //接收温度高八位  
//        T_L=DHT22_rec_byte();    //接收温度低八位
//        revise=DHT22_rec_byte(); //接收校正位

//        DHT22_delay_us(25);    //结束

////        if((R_H+R_L+T_H+T_L)==revise)      //校正
////        {
//            shidu=R_H*256+R_L;
//						wendu=T_H*256+T_L;
//						shidu1=shidu/10;//整数
//						shidu2=shidu%10;//小数
//						wendu1=wendu/10;
//						wendu2=wendu%10;
////        } 
//       
//				EA=1;
//    }
//}



//#include <REGX52.H>
//sbit DA=P1^7;

//void dms(unsigned int x_ms)		
//{
//	unsigned char i, j;
//  while(x_ms)
//	{	i = 2;
//	j = 239;
//	do
//	{
//		while (--j);
//	} while (--i);
//  x_ms--;
//	}
//}


//unsigned char  DHT22_init()
//{
//	unsigned char i;
//	DA=1;
//	DA=0;
//	

//	dms(20);								//delay 1ms
//	DA=1;
//	while(DA==1);
//	i = 27;while (--i);     //delay 60us
//	if(DA==1) return 1;
//	i = 37;while (--i);			//delay 80us
//	if(DA==0) return 1;
//	while(DA==1); 			
//	
//	return 0;
//}


//unsigned char ReadHH()		//读湿度高八位
//{
//	unsigned char i,n,HH=0x00;
//	for(n=0;n<8;n++)
//	{
//		while(DA==0);
//		
//		while (DA==1)
//		{
//			i = 17;while (--i);	//delau 40us
//			if(DA==1)
//			HH=HH|(0X01<<(7-n));
//		}
//	}
//	
//	return HH;
//}
//	
//unsigned char ReadHL()			//读湿度低八位
//{
//	unsigned char i,n,HL=0x00;
//	for(n=0;n<8;n++)
//	{
//		while(DA==0);
//		
//		while (DA==1)
//		{
//			i = 17;while (--i);	//delau 40us
//			if(DA==1)
//			HL=HL|(0X01<<(7-n));
//		}
//	}
//	
//	return HL;
//	
//}

//unsigned char ReadTH()				//读温度高八位
//{
//	unsigned char i,n,TH=0x00;
//	for(n=0;n<8;n++)
//	{
//		while(DA==0);
//		
//		while (DA==1)
//		{
//			i = 17;while (--i);	//delau 40us
//			if(DA==1)
//			TH=TH|(0X01<<(7-n));
//		}
//	}
//	
//	return TH;
//	
//}

//unsigned char ReadTL()			//读温度低八位
//{
//	unsigned char i,n,TL=0x00;
//	for(n=0;n<8;n++)
//	{
//		while(DA==0);
//		
//		while (DA==1)
//		{
//			i = 17;while (--i);	//delau 40us
//			if(DA==1)
//			TL=TL|(0X01<<(7-n));
//		}
//	}
//	
//	return TL;
//}







