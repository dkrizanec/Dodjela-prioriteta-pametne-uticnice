/*
 * usart.c
 *
 * Created: 29-Nov-18 14:47:05
 *  Author: Domagoj Krizanec
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include "usart.h"
#include "lcd.h"

 void UsartInit(uint16_t baud)
 {
	 uint16_t baudPrescaler; 	 
	 baudPrescaler = (F_CPU/(16UL*baud))-1;
	 
	 UBRRH = (uint8_t)(baudPrescaler>>8);
	 UBRRL = (uint8_t) baudPrescaler;
	 
	 UCSRA &=~ (1<<U2X);
	 UCSRB = (1<<RXEN)|(1<<TXEN);
	 UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<< UCSZ0);	 
 }
 
void UsartTransmit(uint8_t data)
{
	
	while (!(UCSRA &(1<<UDRE)));				 // wait while register is free
	
	UDR = data;                                   // load data in the register
}

uint8_t UsartRecieve(void)
{	
	while(!(UCSRA & (1<<RXC)));      			  // wait while data is being received
	return UDR;									 // return 8-bit data
	
}

void USART_message (char *data)
{
	while (*data != '\0')
	UsartTransmit(*data++);
}

