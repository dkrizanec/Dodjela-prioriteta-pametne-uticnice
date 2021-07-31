/*
 * bluetooth.c
 *
 * Created: 19-Nov-18 20:17:28
 *  Author: Domagoj Krizanec
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "bluetooth.h"
#include "lcd.h"
#include "uart.h"


unsigned concatenate(uint8_t x, uint8_t y)
 {	
	uint8_t pow = 10;
	while(y >= pow)
	pow *= 10;
	return x*pow + y;
}

void BluetoothSetUp(struct info *varijable, struct users *korisnik)
{
					
	PETLJA: while (1)	             
	                 
	{	
		varijable->uartRecieve = 0; 									                	
		varijable->uartRecieve = uart0_getc();							                //primanje bluetooth informacije i spremanje u buffer-- informacija se prima u ASCII vrijednosti				
		
		if(varijable->uartRecieve == 33)												//znak '!' - prebacivanje u drugi režim rada -- IZLAZ IZ BLUETOOTH FUNKCIJE
		{	
			varijable->functionFlag = 0;
			varijable->uneseneZnamenke = 0;
			varijable->unosVrijednostiKorisnika = 0;
			varijable->uartRecieve = 0;	
			break;					
		}
		
		if(varijable->uartRecieve == 10)                                                   // bluetooth veza izgubljena
		{
			lcd_gotoxy(0,1);
			uart0_puts("Device lost");
			varijable->functionFlag = 0;
			break;
		}			
		
		if(varijable->uartRecieve == 59)												//novi korisnik- znak ";"
		{
			if(korisnik->brojKorisnika < 10)	 korisnik->brojKorisnika++;	
			else korisnik->brojKorisnika = 0;
			
			varijable->uneseneZnamenke = 0;		
			varijable->unosVrijednostiKorisnika = 0;	
			goto PETLJA;								
		}
	
		if(varijable->uartRecieve == 32)                                                      //odvajanje vrijednosti razmakom
		{
			memset(varijable->FinalUart, 0, sizeof(varijable->FinalUart));										//postavljanje parametra Final Uart u Nulu
			memset(varijable->UART, 0, sizeof(varijable->UART));
			varijable->unosVrijednostiKorisnika++;
			varijable->uneseneZnamenke = 0; 
			goto PETLJA;
		}						
		
		if(varijable->uartRecieve >= 48 && varijable->uartRecieve <= 58)                               //RAD S BROJEVIMA
		{
			
			if(varijable->uartRecieve == 58)												// znak ":"   nova vrijednost -HH/MM/SS                                 
			{
				varijable->uneseneZnamenke++;
				varijable->previousUart = 0;
				goto PETLJA;
			}
			
			varijable->uartRecieve = varijable->uartRecieve - 48;											//oduzimanjem dobijemo stvarnu vrijednost broja					
												
			varijable->concatenatedNumber = concatenate(varijable->previousUart, varijable->uartRecieve);
			
			if(varijable->concatenatedNumber <=0 && varijable->concatenatedNumber >=60)					//ukoliko je unesni broj izvan brojevnog podru?ja
			{				
				uart0_puts("Invalid number input! Please use format HH/MM/SS");
				goto PETLJA;
			}
			
			if (varijable->unosVrijednostiKorisnika == 1)
			{
				if(varijable->uneseneZnamenke == 0) korisnik->openingHour[korisnik->brojKorisnika] = varijable->concatenatedNumber;
				else if(varijable->uneseneZnamenke == 1) korisnik->openingMin[korisnik->brojKorisnika] = varijable->concatenatedNumber;
				else if(varijable->uneseneZnamenke == 2) korisnik->openingSec[korisnik->brojKorisnika] = varijable->concatenatedNumber;		
				else varijable->uneseneZnamenke = 0;	
			}
			if (varijable->unosVrijednostiKorisnika == 2)
			{
				if(varijable->uneseneZnamenke == 0) korisnik->closingHour[korisnik->brojKorisnika] = varijable->concatenatedNumber;
				else if(varijable->uneseneZnamenke == 1) korisnik->closingMin [korisnik->brojKorisnika] = varijable->concatenatedNumber;
				else if(varijable->uneseneZnamenke == 2) korisnik->closingSec[korisnik->brojKorisnika] = varijable->concatenatedNumber;			
				else varijable->uneseneZnamenke = 0;	 
			}
			if(varijable->unosVrijednostiKorisnika == 3) 
			{
				if(varijable->uneseneZnamenke == 0)
				{
					korisnik->relayNumber[korisnik->brojKorisnika] = varijable->uartRecieve;				
				}
			}		
			
			varijable->previousUart = varijable->uartRecieve;
		}   
		
		sprintf(varijable->UART, "%c", varijable->uartRecieve);								//prebacuje iz decimalnog u ASCII		
		strcat(varijable->FinalUart, varijable->UART);										// funkcija dodaje drugi dio prvome, te vraca prvi dio natrag	
		
		if(isalpha(varijable->UART[0]))														// provjerava ukoliko je uneseni char simbol slovo                    
		{			
			strcpy(korisnik->userName[korisnik->brojKorisnika], varijable->FinalUart);				
														
		}
		
		for(uint8_t i = 0; i< korisnik->brojKorisnika; i++)
		{
			if(korisnik->brojKorisnika != 0)
			{
				if((korisnik->openingHour[i] == korisnik->openingHour[korisnik->brojKorisnika])
				 && (korisnik->openingMin[i] == korisnik->openingMin[korisnik->brojKorisnika]) 
				 && (korisnik->openingSec[i] == korisnik->openingSec[korisnik->brojKorisnika]) 
				 && (korisnik->relayNumber[i] == korisnik->relayNumber[korisnik->brojKorisnika]))
				{				
					memset(korisnik->userName[korisnik->brojKorisnika],0, sizeof(korisnik->userName[korisnik->brojKorisnika]));	//termin vec zauzet
					korisnik->openingHour[korisnik->brojKorisnika] = 0;
					korisnik->openingMin[korisnik->brojKorisnika] = 0;
					korisnik->openingSec[korisnik->brojKorisnika] = 0;
					korisnik->relayNumber[korisnik->brojKorisnika] = 0;							
					korisnik->brojKorisnika--;
					uart0_puts("WARNING! Input time and relay are unavailable! User that has that period is ");	
					uart0_puts(korisnik->userName[i]);							
				}
			}			
		}
		break;																	
	}	
}	
