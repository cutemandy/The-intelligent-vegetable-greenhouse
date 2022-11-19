#include <REGX52.H>


unsigned char Key_KeyNumber;


unsigned char Key(void)
{
	unsigned char Temp=0;
	Temp=Key_KeyNumber;
	Key_KeyNumber=0;
	return Temp;
}


unsigned char Key_GetState()
{
	unsigned char KeyNumber=0;
	
	if(P3_1==0){KeyNumber=1;}
	if(P3_0==0){KeyNumber=2;}
	if(P3_2==0){KeyNumber=3;}
	if(P3_3==0){KeyNumber=4;}
	
	return KeyNumber;
}


void Key_Loop(void)
{
	static unsigned char NowState,LastState;//定义静态变量 now last
	LastState=NowState;				
	NowState=Key_GetState();//按下后将键值给now	

	if(LastState==1 && NowState==0)//松手后 now为0 刚刚按下的值给last并返回
	{
		Key_KeyNumber=1;
	}
	if(LastState==2 && NowState==0)
	{
		Key_KeyNumber=2;
	}
	if(LastState==3 && NowState==0)
	{
		Key_KeyNumber=3;
	}
	if(LastState==4 && NowState==0)
	{
		Key_KeyNumber=4;
	}
}
