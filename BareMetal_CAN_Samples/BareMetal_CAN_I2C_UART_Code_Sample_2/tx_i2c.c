#include "stm32f303xx.h"

void delayf(int);

/*********   CAN   ********/

void can_testmode_setup(void);
void go_init_mode(void);
void exit_init_mode(void);
void tx_msg(unsigned char p);

/*********   I2C   ********/
void i2c_init();
void i2c_write_byte(unsigned char acc_reg_addr,unsigned char data);
int8_t i2c_read_byte(unsigned char acc_reg_addr);


int main()
{
	int i,j;
	int8_t xh,yh,zh;
	RCC_AHBENR |= (1<<21);
	can_testmode_setup();
	
	i2c_init();
	i2c_write_byte(0x20,0x37);
	
	while(1)
	{
		
		xh = i2c_read_byte(0x29);

		yh = i2c_read_byte(0x2B);

		zh = i2c_read_byte(0x2D);
		
		delayf(1000);
		
		tx_msg(xh);
		delayf(10);
		
		tx_msg(yh);
		delayf(10);
		
		tx_msg(zh);
	  delayf(10);
		
		delayf(1000);
	}
}

void can_testmode_setup(void)
{
		/*
		CAN Rx and Tx is conected on PB8 and PB9.
		Both having AF9 as Alternate Function Slot
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

void tx_msg(unsigned char p)
{
		//filter();
		//unsigned int id = 10;
		
		//CAN_TI0R = (1<<24)|(1<<22)|(1<<21);
	
		CAN_TI0R = 0x01600000;	// 11
		CAN_TI0R &=~ (1<<1);		//RTR- DATA
		
		while(!(CAN_TSR & BIT26));
	CAN_TDT0R |= (BIT0);
	CAN_TDT0R &=~((BIT1)|(BIT2)|(BIT3));
		CAN_TDL0R = p;
		
	
		CAN_TI0R |= BIT0;
		//while((CAN_TI0R & BIT0));
		while(!(CAN_TSR & (1<<1)));
		
		
}

void i2c_init()
{
	//Enable clocks
	RCC_AHBENR |= BIT18;							//GPIO B clock enabled
	RCC_APB1ENR |= BIT21;							//I2C1 clock enable
	
	//Alternate function enable
	GPIOB_MODER|=(BIT15|BIT13);				//Alternate function for PB6(SCL) and PB7(SDA)
	GPIOB_AFRL |=(BIT26|BIT30);				//SCL and SDA is connected on AF4
	
	//I2C Initialization
	I2C1_CR1 &= ~BIT0;									//disable peripheral 
	I2C1_CR1 |= BIT12;									//analog filter off
	//I2C1_CR1 |= BIT8|BIT9|BIT10;				//digital filter enabled 
		
	
	//I2C Timing Setting (10KHz)(Standard Mode)
	I2C1_TIMINGR |= (1<<28) |(4<<20)|(2<<16)|(199<<0)|(195<<8);						//10KHz config according to table
	I2C1_CR2 |= ((0x19)<<1);					//Slave Address of Accelerometer
																		//Fixed since only one slave
	
	I2C1_CR1 |= BIT0;
	
}

void i2c_write_byte(unsigned char acc_reg_addr,unsigned char data) {
    I2C1_CR2 &= ~(BIT10);									
    I2C1_CR2 |= (2 << 16);
    I2C1_CR2 |= BIT13;
		while(I2C1_CR2 & BIT13);
    
		I2C1_TXDR = acc_reg_addr;
    while (!(I2C1_ISR & BIT0));

    I2C1_TXDR = data;
    while (!(I2C1_ISR & BIT0));
    I2C1_CR2 |= BIT14;
    while(I2C1_CR2 & BIT14);
}
	
int8_t i2c_read_byte(unsigned char acc_reg_addr)
{
		int8_t data;
		I2C1_CR2 &= ~(BIT10);
		I2C1_CR2 &=~(0xff<<16);
    I2C1_CR2 |= (1 << 16);
		I2C1_CR2 |= BIT13;
		
		while(I2C1_CR2 & BIT13);
    
		I2C1_TXDR = acc_reg_addr;
    while (!(I2C1_ISR & BIT0));

		I2C1_CR2 |= BIT10;									
    I2C1_CR2 |= (1 << 16);
		I2C1_CR2 |= BIT13;
	
		while(I2C1_CR2 & BIT13);
	
		//while(!(I2C1_ISR & BIT2));
		data = I2C1_RXDR;
		
		
		
		I2C1_CR2 |= BIT14;
		//while(I2C1_CR2 & BIT14);
	
		return data;
}	
void delayf(int a)
{
	int i,j;
 for(i=0;i<a;i++)
			for(j=0;j<1000;j++);
}