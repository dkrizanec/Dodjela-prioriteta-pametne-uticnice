/*
 * display.h
 *
 * Created: 10-Oct-18 13:24:41
 *  Author: Domagoj Krizanec
 */ 


#ifndef DISPLAY
#define DISPLAY

void DateTime(struct unesenoVrijeme *vrijeme, struct uneseniDatum *datum, struct vrijemeTrajanja *trajanje,  struct users *korisnik); // prototip funkcije DateTime
void UserDefinedTime (struct unesenoVrijeme *vrijeme, struct uneseniDatum *datum);//prototip funkcije UserDefinedTime
void Duration(struct vrijemeTrajanja *trajanje); //prototip funkcije Duration
void RelayNumber(struct vrijemeTrajanja *trajanje); //broj izlaza koji se koristi

#endif
