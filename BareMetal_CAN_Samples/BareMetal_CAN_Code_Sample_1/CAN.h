#ifndef __CAN_H
	#define __CAN_H

#define sbi(port,bit) port |= (1<<bit);
#define cbi(port,bit) port &= ~(1<<bit);
#define cb(port,bit)	port&(1<<bit)

void can_setup();
void can_enter_init();
void can_exit_init();
void can_tx(unsigned char tx);
unsigned char can_rx();
void can_rx_filter();
void can_testmode();

#endif