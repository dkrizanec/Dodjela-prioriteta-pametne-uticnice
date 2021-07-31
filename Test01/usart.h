/*
 * usart.h
 *
 * Created: 29-Nov-18 14:47:17
 *  Author: Domagoj Krizanec
 */ 

#ifndef USART
#define USART

void UsartInit(uint16_t);
void UsartTransmit(uint8_t);
uint8_t UsartRecieve(void);
void USART_message (char *data);

#endif