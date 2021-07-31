/*
 * display.c
 *
 * Created: 10-Oct-18 13:24:58
 *  Author: Domagoj Krizanec
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "lcd.h"
#include "rtc3231.h"
#include "button.h"
#include "display.h"

void DateTime(struct unesenoVrijeme *vrijeme, struct uneseniDatum *datum, struct vrijemeTrajanja *trajanje, struct users *korisnik)  //funkcija za postavljanje vremena i datuma
{
	struct rtc_time time;
	struct rtc_date date;		
	
	char datetimebuffer[12];	//buffer za vrijeme i datum
			
	lcd_init(LCD_DISP_ON);
	rtc3231_init();	
		
	/* time.hour = 0x13;   //postavljanje vremena- samo jednom
	time.min = 0x36;   //sve vrijednosti se moraju unijeti u hex obliku
	time.sec = 0x00;
	
	date.day = 0x1E;   //postavljanje datuma- samo jednom
	date.month = 0xA;  //sve vrijednosti moraju se unijeti u hex obliku 
	date.year = 0x12;
	//date.wday = 4; */	
	
	//rtc3231_write_time(&time);  //postavljanje vremena- samo jednom
	//rtc3231_write_date(&date); //postavljanje datuma- samo jednom 		
	
	rtc3231_read_datetime(&time, &date);  //reading time and date from RTC_3231		
	
	sprintf(datetimebuffer, "%02u:%02u:%02u", time.hour, time.min, time.sec);  //castanje int-a u string. %02u format stavlja leading zero na jednoznamenkaste brojeve	
	lcd_gotoxy(0,0); // postavlja se pokaziva? na 1. red (row0) i 1. znak
	lcd_puts("TIME: ");
	lcd_gotoxy(6,0); // postavlja se pokaziva? na 2. red (row1) i 1. znak
	lcd_puts(datetimebuffer);	
	
	sprintf(datetimebuffer, "%02u/%02u/20%02u", date.day, date.month, date.year);
	lcd_gotoxy(0,1);
	lcd_puts("DATE: ");
	lcd_gotoxy(6,1);
	lcd_puts(datetimebuffer);

	OutputCheck(vrijeme, datum, trajanje, korisnik);   //funkcija za omogucavanje izlaza   	
}

void UserDefinedTime(struct unesenoVrijeme *vrijeme, struct uneseniDatum *datum)
{
	struct Tipka tipka; 	          //svim tipkama pocetno stanje je "1" 	
	
	char datetimebuffer[12];
	
	tipka.LEFT = (PIND >> PD6)&1;
	tipka.UP =  (PIND >> PD5)&1;
	tipka.DOWN = (PIND >> PD2)&1;
	tipka.RIGHT = (PIND >> PD4)&1;
	tipka.SELECT = (PIND >> PD3)&1; 		
	
	lcd_init(LCD_DISP_ON);			
	
	if(tipka.RIGHT== 0)
		vrijeme->horizontalno++;
	if(tipka.LEFT == 0)
		vrijeme->horizontalno--;				
				
	switch(vrijeme->horizontalno)
		{
			case 0: 
				{	
					lcd_gotoxy(0,1);
					lcd_puts("HOUR");										
					if(tipka.UP == 0)
					{	
						if(vrijeme->hour <= 0x16)						
							vrijeme->hour++;
						else
							vrijeme->hour = 0x00;								
					}
					
					if(tipka.DOWN == 0)
					{
						if(vrijeme->hour >= 0x01)
							vrijeme->hour--;	
						else
							vrijeme->hour = 0x17;		
					}
					break;	
				}
			
			case 1:
			{			lcd_gotoxy(0,1);
						lcd_puts("MIN");				
						if(tipka.UP == 0)
						{
							if(vrijeme->min <= 0x3A)
								vrijeme->min++;
							else
								vrijeme->min = 0x00;
						}
												
						if(tipka.DOWN == 0)
						{
							if(vrijeme->min >= 0x01)
								vrijeme->min--;
							else
								vrijeme->min = 0x3B;
						}
					break; 
			}
			
			case 2:
			{
				lcd_gotoxy(0,1);
				lcd_puts("SEC");
				if(tipka.UP == 0)
				{
					if(vrijeme->sec <= 0x3A)
					vrijeme->sec++;
					else
					vrijeme->sec = 0x00;
				}
				
				if(tipka.DOWN == 0)
				{
					if(vrijeme->sec >= 0x01)
					vrijeme->sec--;
					else
					vrijeme->sec = 0x3B;					
				}
				break;
			}
			case 3:
			{
						lcd_gotoxy(0,1);
						lcd_puts("DAY");
						if(tipka.UP == 0)
						{
							if(datum->day <= 0x1E)
							datum->day++;
							else
							datum->day = 0x00;
						}
						
						if(tipka.DOWN == 0)
						{
							if(datum->day >= 0x01)
							datum->day--;
							else
							datum->day = 0x1F;							
						}
					break;	
			}
			case 4:
			{				lcd_gotoxy(0,1);
							lcd_puts("MNTH");
							if(tipka.UP == 0)
							{
								if(datum->month <= 0xB)
								datum->month++;
								else
								datum->month = 0x00;
							}
							
							if(tipka.DOWN == 0)
							{
								if(datum->month >= 0x01)
								datum->month--;
								else
								datum->month = 0xC;								
							}
				break;			
			}
			
			case 5:
			{				lcd_gotoxy(0,1);
							lcd_puts("YEAR");
							if(tipka.UP == 0)
							{
								if(datum->year <= 0x62)
								datum->year++;
								else
								datum->year = 0x00;
							}
							
							if(tipka.DOWN == 0)
							{
								if(datum->year >= 0x01)
								datum->year--;
								else
								datum->year = 0x63;								
							}
					break;		
			}
			default: vrijeme->horizontalno = 0;
			break; 							
		}		
			
	sprintf(datetimebuffer, "%02u:%02u:%02u", vrijeme->hour, vrijeme->min, vrijeme->sec);	
	lcd_gotoxy(0,0); // postavlja se pokaziva? na 1. red (row0) i 1. znak 
	lcd_puts("SET: ");	
	lcd_gotoxy(6,0); // postavlja se pokaziva? na 2. red (row1) i 1. znak
	lcd_puts(datetimebuffer);
	
	sprintf(datetimebuffer, "%02u/%02u/20%02u", datum->day, datum->month, datum->year);		
	lcd_gotoxy(6,1);
	lcd_puts(datetimebuffer);	
}

void Duration(struct vrijemeTrajanja *trajanje)
{
	struct Tipka tipka;   //svim tipkama pocetno stanje je "1"
	
	char timeBuffer[12];
	
	tipka.LEFT = (PIND >> PD6)&1;
	tipka.UP =  (PIND >> PD5)&1;
	tipka.DOWN = (PIND >> PD2)&1;
	tipka.RIGHT = (PIND >> PD4)&1;
	tipka.SELECT = (PIND >> PD3)&1;
	
	lcd_init(LCD_DISP_ON);	
	
	if(tipka.RIGHT== 0)
	trajanje->trajanjeHorizontalno++;
	if(tipka.LEFT == 0)
	trajanje->trajanjeHorizontalno--;
	
	switch(trajanje->trajanjeHorizontalno)
	{
		case 0:
		{
			lcd_gotoxy(0,1);
			lcd_puts("HOUR");
			if(tipka.UP == 0)
			{
				if(trajanje->hour <= 0x16)
				trajanje->hour++;
				else
				trajanje->hour = 0x00;
			}
			
			if(tipka.DOWN == 0)
			{
				if(trajanje->hour >= 0x01)
				trajanje->hour--;
				else
				trajanje->hour = 0x17;
			}
			break;
		}
		
		case 1:
		{	
			lcd_gotoxy(0,1);
			lcd_puts("MIN");
			if(tipka.UP == 0)
			{
				if(trajanje->min <= 0x3A)
				trajanje->min++;
				else
				trajanje->min = 0x00;
			}
			
			if(tipka.DOWN == 0)
			{
				if(trajanje->min >= 0x01)
				trajanje->min--;
				else
				trajanje->min = 0x3B;
			}
			break;
		}
		
		case 2:
		{
			lcd_gotoxy(0,1);
			lcd_puts("SEC");
			if(tipka.UP == 0)
			{
				if(trajanje->sec <= 0x3A)
				trajanje->sec++;
				else
				trajanje->sec = 0x00;
			}
			
			if(tipka.DOWN == 0)
			{
				if(trajanje->sec >= 0x01)
				trajanje->sec--;
				else
				trajanje->sec = 0x3B;
			}
			break;
		}
		
		default: trajanje->trajanjeHorizontalno = 0;
		break;
	}
		
	sprintf(timeBuffer, "%02u:%02u:%02u", trajanje->hour, trajanje->min, trajanje->sec);
	
	lcd_gotoxy(0,0); // postavlja se pokaziva? na 1. red (row0) i 1. znak
	lcd_puts("ENDING: ");
	lcd_gotoxy(8,1); // postavlja se pokaziva? na 2. red (row1) i 1. znak
	lcd_puts(timeBuffer);		
}

void RelayNumber(struct vrijemeTrajanja *trajanje)
{	
	struct Tipka tipka;	
	char relayNumber[4];
	
	tipka.UP =  (PIND >> PD5)&1;
	tipka.DOWN = (PIND >> PD2)&1;
	
	lcd_init(LCD_DISP_ON);
	
	if(tipka.UP == 0)
	{
		if(trajanje->brojReleja <= 0x03)
			trajanje->brojReleja++;
		else
			trajanje->brojReleja = 0x01;
	}	
	
	if(tipka.DOWN == 0)
	{
		if(trajanje->brojReleja >= 0x02)
		trajanje->brojReleja--;
		else
		trajanje->brojReleja = 0x04;
	}	
	
	itoa(trajanje->brojReleja, relayNumber, 10);
	
	lcd_gotoxy(0,0); // postavlja se pokaziva? na 1. red (row0) i 1. znak
	lcd_puts("RELAY NUMBER: ");
	lcd_gotoxy(14,0); // postavlja se pokaziva? na 2. red (row1) i 1. znak
	lcd_puts(relayNumber);		
}