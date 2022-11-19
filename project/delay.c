#include <intrins.h>
void delay(unsigned int xms)		//@11.0592MHz
{
	while(xms--)
	{
			unsigned char i, j;
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}

}

