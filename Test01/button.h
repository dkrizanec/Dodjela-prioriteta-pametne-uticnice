/*
 * button.h
 *
 * Created: 11-Oct-18 23:32:51
 *  Author: Domagoj Krizanec
 */ 
#ifndef TIPKE
#define TIPKE

  struct Tipka
{
	 _Bool LEFT;
	 _Bool UP;
	 _Bool DOWN;	 
	 _Bool RIGHT; 
	 _Bool SELECT;
	
}; 

struct unesenoVrijeme;
struct uneseniDatum;
struct vrijemeTrajanja;
struct users;


void ButtonInitialisation();
void ButtonPress();
void OutputCheck(struct unesenoVrijeme *vrijeme, struct uneseniDatum *datum, struct vrijemeTrajanja *trajanje , struct users *korisnik);

#endif