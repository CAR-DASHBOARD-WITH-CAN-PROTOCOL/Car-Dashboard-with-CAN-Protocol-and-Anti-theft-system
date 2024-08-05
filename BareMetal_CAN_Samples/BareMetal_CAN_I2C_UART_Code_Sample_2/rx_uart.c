#include "stm32f303xx.h" 	// Deviceheader
#include <string.h>

void delayf(int);

/**********  CAN   **********/

void can_testmode_setup(void);
void go_init_mode(void);
void exit_init_mode(void);
signed char rx_msg(void);
void filter(void);

/**********  UART  **********/

void uart_init();
void uart_tx(unsigned char);
void uart_str_tx(char *);

int main()
{
	
	signed char x,y,z;
	char s[20];
	RCC_AHBENR |= (1<<21);
	GPIOE_MODER = 0x55550000;
	
	uart_init();
	can_testmode_setup();
	filter();
	
	while(1)
	{	
		x = rx_msg();
	
		y = rx_msg();
		
		z = rx_msg();
		
		sprintf(s,"X : %d  Y : %d  Z : %d",x,y,x);
	
		delayf(4000);
		uart_str_tx(s);
		uart_str_tx("\r\n");
		
	}
	
	
}

void can_testmode_setup(void)
{
		/*
		CAN Rx and Tx is conected on PB8 and PB9.
		Both having AF9 as Alternate Function Slote
			*/
	
		//Enable clock for CAN & GPIOA
		RCC_APB1ENR |= (1<<25);					//CAN
		RCC_AHBENR |= (1<<18);					//GPIOB
	
		//PB8(Rx) and PB9(Tx) as Alternate Function and AF slot Selection
		GPIOB_MODER |= (1<<17) | (1<<19);
		GPIOB_AFRH |= (1<<0)|(1<<3)|(1<<4)|(1<<7);
	
		//For Test Mode, Enter Initialization Mode
		go_init_mode();
		
		//CAN_BTR |= (1<<30)|(1<<31);		//HotSelftest Mode activated	
		
		exit_init_mode();
}

void go_init_mode()
{
	CAN_MCR |= (1<<0);		//INRQ bit set
	CAN_MCR &=~(1<<1);		//SLEEP bit reset
	while(!(CAN_MSR & BIT0));
	while(CAN_MSR & BIT1);
	
}

void exit_init_mode()
{
	CAN_MCR &=~((1<<0) | (1<<1));		//INRQ and SLEEP bit reset
	
	//After Reseting INRQ bit,CAN Controller waits for 11 consicutive resecive bits
	//After it is synchronized and ready for normal mode
	
	while((CAN_MSR & (1<<0))&& (CAN_MSR & (1<<1)));	
	
}


signed char rx_msg()
{
	signed char p;
	
	while(!(CAN_RF0R & (1<<0)));	
	p = CAN_RDL0R;
	CAN_RF0R |= (1<<5);
	
	return p;
}

void filter()
{
	unsigned int id = 11;
	
	CAN_FMR |= BIT0;		//FINIT =1;

	CAN_FA1R &= ~BIT0;	//filter BANK 0 inactive
	
	CAN_FM1R |= BIT0;		 //FILTER BANK 0 SET TO LIST MODE
	
	CAN_FS1R &= ~BIT0; 	//FILTER BANK 0 IN 16 BIT SCALLE CONFIG
	
	CAN_FFA1R &= ~BIT0;	//FILTER BANK 0 ASSIGNED TO FIFO0
	
	CAN_F0R1 = id << 5;		//16 bit
	//CAN_F0R1 = id << 21;	//remaining 16 bit	

	CAN_FA1R |= BIT0;	//filter BANK 0 active

	CAN_FMR &= ~BIT0;		
		
}

void uart_init()
{
	// GPIO config 
	RCC_AHBENR |= (1<<17) | (1<<19) ;
	GPIOC_MODER|=0x00000A00;
	GPIOC_AFRL |= (7<<16) | (7<<20);
	
	// UART config
	RCC_APB2ENR |= (1<<14);
	USART1_CR1 |= (1<<3) | (1<<2);
	USART1_BRR |= 0x0341;	   //   8MHz/9600
	USART1_CR1 |= (1<<0);
}

void uart_tx(unsigned char data)
{
	while(!(USART1_ISR & (1<<7)));
	USART1_TDR=data;
}

void uart_str_tx(char str[5])
{
	int i;
	for(i=0;str[i]!='\0';i++)
	{
		uart_tx(str[i]);
	}
	
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
