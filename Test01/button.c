/*
 * button.c
 *
 * Created: 11-Oct-18 23:29:46
 *  Author: Domagoj Krizanec
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "button.h"
#include "display.h"
#include "rtc3231.h"
#include "lcd.h"
#include "bluetooth.h"
#include "uart.h"
#define WAIT_MS 50 //vrijeme cekanja u ms

void ButtonInitialisation()
{	
	DDRD = 0x00; // svi pinovi u PORTD stavljeni kao input - pocetno stanje
	DDRD &=~ (1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5)|(1<<PD6); // input mode na pinovima PD2- PD6
	DDRA |= (1<<PA1)|(1<<PA4)|(1<<PA5)|(1<<PA6)|(1<<PA7); // output mode na pinu PA1, PA4, PA5, PA6, PA7	
	PORTD = 0x00; //onemoguceni pull-up otpornici za sve PIN-ove porta D
	PORTD |= (1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5)|(1<<PD6); // stavljanje 1 na tipke PD2- PD6			
}

void ButtonPress()
{				
		int state = 0; 					
		uint8_t current = PIND;
		uint8_t previous = PIND;
		uint8_t uartValue = 0;		
		uint16_t baudPrescaler, baud = 9600;								//definirana vrijednost baud ratea
		baudPrescaler = (F_CPU/(16UL*baud))-1;
		
		struct rtc_date date;
		struct unesenoVrijeme vrijeme;
		struct uneseniDatum datum;		
		struct vrijemeTrajanja trajanje; 
		struct info varijable;
		struct users korisnik; 
		
		rtc3231_read_date(&date);
				
		vrijeme.horizontalno = 0;		
		vrijeme.hour = 0xC;   //postavljanje vremena unosa- samo jednom
		vrijeme.min = 0x1E;   //sve vrijednosti se moraju unijeti u hex obliku
		vrijeme.sec = 0x00;
		
		datum.day = date.day;    //0xF;   //postavljanje datuma- samo jednom
		datum.month = date.month;  //0x06;  //sve vrijednosti moraju se unijeti u hex obliku
		datum.year =  date.year;  //0x12;
		
		trajanje.trajanjeHorizontalno = 0; //postavljanje pocetnog stanja za horizontalni dio displaya trajanja
		trajanje.stanjeZaOutputCheck = 0; //postavljanje pocetnog stanja za flag stanja 
		trajanje.brojReleja = 0x01; //postavljanje broja releja na pocetnu vrijednost
		trajanje.hour = 0xC;   //postavljanje vremena gasenja- samo jednom
		trajanje.min = 0x1E;   //sve vrijednosti se moraju unijeti u hex obliku
		trajanje.sec = 0x00;		
		
		varijable.concatenatedNumber = 0; 
		varijable.uneseneZnamenke = 0; 
		varijable.unosVrijednostiKorisnika = 0; 
		varijable.previousUart = 0; 
		varijable.uartRecieve = 0; 		
		varijable.testI = 0;		
		varijable.functionFlag = 0;
		memset(varijable.FinalUart, 0, sizeof(varijable.FinalUart));	//postavljanje parametra Final Uart u Nulu
		memset(varijable.UART, 0, sizeof(varijable.UART));
		
		korisnik.brojKorisnika = 0;
		uart0_init(baudPrescaler);
		
	while(1)
	{				
		while (state == 0)
		{						
			uartValue = uart0_getc();
			
			if(uartValue == 33)											   //znak '!'
			{ 
				varijable.functionFlag = 1;                                      
			}
			while(varijable.functionFlag == 1)
			{
				BluetoothSetUp(&varijable, &korisnik);
			}
			
						
			DateTime(&vrijeme, &datum, &trajanje, &korisnik);
				
			current = PIND;						    //debounce loop for PIND3
			if((previous &(1<<PIND3)) > (current &(1<<PIND3)))  //rise edge on PIND3
			{
				state = 1;
			}
			_delay_ms(WAIT_MS);
			previous = PIND;									
		};
		
		while(state == 1)	
		{		
			uartValue = uart0_getc();
			if(uartValue == 33)											   //znak '!'
			{
				varijable.functionFlag = 1;
			}
			while(varijable.functionFlag == 1)
			{
				BluetoothSetUp(&varijable, &korisnik);
			}
			
			UserDefinedTime(&vrijeme, &datum);				
			current = PIND;	
			if((previous &(1<<PIND3)) > (current &(1<<PIND3)))  //rise edge on PIND3
			{					
				state = 2;					
			}				
			_delay_ms(WAIT_MS);		
			previous = PIND;
									 
		};
		
		while(state == 2)
		{
			uartValue = uart0_getc();
			if(uartValue == 33)											   //znak '!'
			{
				varijable.functionFlag = 1;
			}
			while(varijable.functionFlag == 1)
			{
				BluetoothSetUp(&varijable, &korisnik);
			}
					 
			Duration(&trajanje);
			current = PIND;
			if((previous &(1<<PIND3)) > (current &(1<<PIND3)))  //rise edge on PIND3
			{
				state = 3;
			}
			_delay_ms(WAIT_MS);
			previous = PIND;
		};		
		
		while(state == 3)
		{
			uartValue = uart0_getc();
			if(uartValue == 33)											   //znak '!'
			{
				varijable.functionFlag = 1;
			}
			while(varijable.functionFlag == 1)
			{
				BluetoothSetUp(&varijable, &korisnik);
			}
			
			RelayNumber(&trajanje);
			current = PIND;
			if((previous &(1<<PIND3)) > (current &(1<<PIND3)))  //rise edge on PIND3
			{
				state = 0;
			}
			_delay_ms(WAIT_MS);
			previous = PIND;
		};
																					
	}
}

void OutputCheck(struct unesenoVrijeme *vrijeme, struct uneseniDatum *datum, struct vrijemeTrajanja *trajanje , struct users *korisnik)  //funkcija za upravljanjem izlazom
{	
	struct rtc_time time;
	struct rtc_date date;	
			
	rtc3231_init();
	rtc3231_read_datetime(&time, &date);		
	
	
	if(date.year == datum->year && date.month == datum->month 
	&& date.day == datum->day && time.hour == vrijeme->hour 
	&& time.min == vrijeme->min &&  time.sec == vrijeme->sec) //provjera postavljenog vremena
	{
		trajanje->stanjeZaOutputCheck = 1; 
		switch(trajanje->brojReleja)
		{
			case 1:
			trajanje->relay1 = 1;
			break;
			case 2:
			trajanje->relay2 = 1;
			break;
			case 3:
			trajanje->relay3 = 1;
			break;
			case 4:
			trajanje->relay4 = 1;
			break;
		}
	}
	
	if(time.hour == trajanje->hour && time.min == trajanje->min  
	&&  time.sec == trajanje->sec) // provjera vremena zavrsetka
	{
		trajanje->stanjeZaOutputCheck = 0;
		switch(trajanje->brojReleja)
		{
			case 1:
			trajanje->relay1 = 0;
			break;
			case 2:
			trajanje->relay2 = 0;
			break;
			case 3:
			trajanje->relay3 = 0;
			break;
			case 4:
			trajanje->relay4 = 0;
			break;
		}
	}	 
	
	for(uint8_t i = 0; i <= korisnik->brojKorisnika; i++)
	{				
		if(time.hour == korisnik->openingHour[i] && time.min == korisnik->openingMin[i] &&  time.sec == korisnik->openingSec[i]) //postavljenog vremena
		{			
			switch(korisnik->relayNumber[i])
			{
				case 1:
						trajanje->relay1 = 1; 
				break;
				case 2:
						trajanje->relay2 = 1;
				break;
				case 3:
						trajanje->relay3 = 1; 
				break;
				case 4:
						trajanje->relay4 = 1;
				break;
			}	
		}		
		if(time.hour == korisnik->closingHour[i] && time.min == korisnik->closingMin[i] &&  time.sec == korisnik->closingSec[i])
		{
			switch(korisnik->relayNumber[i])
			{
				case 1:
				trajanje->relay1 = 0;
				break;
				case 2:
				trajanje->relay2 = 0;
				break;
				case 3:
				trajanje->relay3 = 0;
				break;
				case 4:
				trajanje->relay4 = 0;
				break;
			}
			memset(korisnik->userName[i],0, sizeof(korisnik->userName[i]));
			korisnik->openingHour[i] = 0; 
			korisnik->openingMin[i] = 0; 
			korisnik->openingSec[i]= 0; 
			korisnik->closingHour[i]=0;
			korisnik->closingMin[i] = 0; 
			korisnik->closingSec[i]= 0; 
			korisnik->relayNumber[i]= 0; 			
		}
	}		
			
	if(trajanje->relay1 == 1)
	{
		PORTA &=~ (1<<PA4);					//RELAY 1		
	}
	else
	{
		PORTA |= (1<<PA4);
	}
	
	if(trajanje->relay2 == 1)
	{
		PORTA &=~ (1<<PA5);					//RELAY 2
	}
	else
	{
		PORTA |= (1<<PA5);
	}
		
	if(trajanje->relay3 == 1)
	{
		PORTA &=~ (1<<PA6);					//RELAY 3
	}
	else
	{
		PORTA |= (1<<PA6);
	}
	
	if(trajanje->relay4 == 1)
	{
		PORTA &=~ (1<<PA7);					//RELAY 4
	}
	else
	{
		PORTA |= (1<<PA7);
	}
		
	if(trajanje->stanjeZaOutputCheck == 1) PORTA |= (1<<PA1);
	else PORTA &=~ (1<<PA1);
}
		
		
		 	
