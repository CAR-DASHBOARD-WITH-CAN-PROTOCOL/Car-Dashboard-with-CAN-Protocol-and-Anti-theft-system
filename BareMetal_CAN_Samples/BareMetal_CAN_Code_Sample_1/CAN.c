#ifndef __CAN_C
	#define __CAN_C

#include <stm32f407xx.h>
#include "CAN.h"

void can_setup()
{
													//Reset and Clock Control Register
		sbi(RCC->APB1ENR,25); //Peripheral clock enable for CAN1		//On 25 we got CAN on APB Bus 
		sbi(RCC->AHB1ENR,1);	//peripheral clock enable for GPIOB		//GPIO B (A-I,9) 
		
													//GPIO Register	Pg.283
		sbi(GPIOB->MODER,17);	//set GPIOB_8 as alternate function for CAN_Rx	17??
													//10: Alternate function mode
		sbi(GPIOB->MODER,19);	//set GPIOB_9 as alternate function for CAN_Tx	19??
													//10: Alternate function mode	
		
	  sbi(GPIOB->AFR[1],0);	//set GPIOB_8 as AF8	0,3??
	  sbi(GPIOB->AFR[1],3);
		sbi(GPIOB->AFR[1],4); //set GPIOB_9 as AF9  4,7??
		sbi(GPIOB->AFR[1],7);
}
													//CAN Registers Pg.1100
void can_enter_init()
{
		sbi(CAN1->MCR,0);			     //Enter into initialization mode by setting INRQ in MCR
		cbi(CAN1->MCR,1);			     //Exit from Sleep mode by clearing SLEEP bit
		sbi(CAN1->MCR,4);					 //No Retransmit
		while(!(cb(CAN1->MSR,0))); //normally it is 0 when it set-->initialization mode
		while(cb(CAN1->MSR,1));		 //in Sleep mode SLAK=1 when SLAK=0-->exit from sleep mode
}

void can_exit_init()
{
		cbi(CAN1->MCR,0);		//Enter into normal mode by clearing INRQ bit in MCR
		cbi(CAN1->MCR,1);		//Exit from Sleep mode by clearing SLEEP bit	
												
												//After Reseting INRQ bit,CAN Controller waits for 11 consicutive resecive bits
												//After it is synchronized and ready for normal mode
		while((cb(CAN1->MSR,0)) && (cb(CAN1->MSR,1)));
	
}

void can_tx(unsigned char tx)
{
		CAN1->sTxMailBox[0].TIR = 0x01600000;
  	
		//there are 3 transmit mailbox available //TIR,TDTR,TDLR,TDHR
		cbi(CAN1->sTxMailBox[0].TIR,1); //clear 1st bit of sTxMailBox[0]->TIOR to set as dataframe 
		cbi(CAN1->sTxMailBox[0].TIR,2); //clear IDE bit to select Standard Identifier
		//CAN1->sTxMailBox[0].TIR |= ((1<<21) | (1<<23) | (1<<24));    //set standard Id as 11
		
		while(!(cb(CAN1->TSR ,26)));	//check bit 26 for Transmit mailbox 0 as empty --> when it is set -->empty
		CAN1->sTxMailBox[0].TDTR |= (1<<0);	//set bit0 of TDTR to set datalength as 1 byte
		CAN1->sTxMailBox[0].TDTR &= ~((1<<1)|(1<<2)|(1<<3));
		CAN1->sTxMailBox[0].TDLR = tx;
	
		sbi((CAN1->sTxMailBox[0].TIR),0); //set to request to transmit data 
		while(!(cb(CAN1->TSR,0)));	//set when last request has been performed
}

unsigned char can_rx()
{
		unsigned char rx;
		while(!(cb(CAN1->RF0R,0)));		//check if ant msg is pending to receive or not if it is set that means no msg is pending
		rx=CAN1->sFIFOMailBox[0].RDLR;
		sbi(CAN1->RF0R,5);						//set this bit to relese FIFO 0 output mailbox
		return rx;
}
														//CAN Filter Register Pg.1117
void can_rx_filter()
{
			unsigned int id = 11;
			sbi(CAN1->FMR, 0);		//set FINT to 1 for Initialization mode for the filters.
			cbi(CAN1->FA1R, 0);		//clear filter 0 to inactive it
			sbi(CAN1->FM1R, 0);		//set 0th bit to select filter 1 as Two 32-bit registers of filter bank 0 are in Identifier List mode
														//so every bit of identifier must match for both
			cbi(CAN1->FS1R,0);		//clear 0th bit of FS1R to set Dual 16-bit scale configuration
			cbi(CAN1->FFA1R,0);		//clear 0th bit of FIFO Assignment Register to assign FIFO 0
		
			CAN1->sFilterRegister[0].FR1 = (id<<5); //filter bank register to save identifier which must be matched with received identifier
			sbi(CAN1->FA1R, 0);		//set 0th bit of FA1R to active 0th filter	
			cbi(CAN1->FMR, 0);		//clear 0th bit to exit from initialization mode and comes into active filter mode
}

void can_testmode()
{
			unsigned int brp;
			can_setup();
			can_enter_init();
			brp  = (42000000 / 7) / 500000;         /* baudrate is set to 500k bit/s    */
                                                                          
  /* set BTR register so that sample point is at about 71% bit time from bit start */
  /* TSEG1 = 4, TSEG2 = 2, SJW = 3 => 1 CAN bit = 7 TQ, sample at 71%      */
  CAN1->BTR &= ~(((        0x03) << 24) | ((        0x07) << 20) | ((        0x0F) << 16) | (          0x3FF));
  CAN1->BTR |=  ((((3-1) & 0x03) << 24) | (((2-1) & 0x07) << 20) | (((4-1) & 0x0F) << 16) | ((brp-1) & 0x3FF));
	CAN1->BTR &= ~((1<<30)|(1<<31));
//	CAN1->BTR |= ((1<<30)|(1<<31));
	can_exit_init();
}
#endif