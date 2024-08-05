#include <stm32f407xx.h>
#include "CAN.c"

void delayf(int );
int main()
{
	unsigned char rx1;
	unsigned char tx1=0;
	can_testmode();
	can_rx_filter();
	while(1)
	{
			tx1 = tx1+1;
			can_tx(tx1);
			delayf(1000);
	//		rx1=can_rx();
	//		delayf(1000);
	}
	
	return 0;
}
void delayf(int a)
{
	int i,j;
		for(i=0;i<a;i++)
				{
					for(j=0;j<1000;j++)
					{
					}
				}
}