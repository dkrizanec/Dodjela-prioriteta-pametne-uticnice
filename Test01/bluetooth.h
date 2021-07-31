/*
 * bluetooth.h
 *
 * Created: 19-Nov-18 20:17:47
 *  Author: Domagoj Krizanec
 */ 
#ifndef BLUETOOTH
#define BLUETOOTH

struct users
{
	char userName[20][8];
	uint8_t openingHour[20];
	uint8_t closingHour[20];
	uint8_t openingMin[20];
	uint8_t closingMin[20];
	uint8_t openingSec[20];
	uint8_t closingSec[20];
	uint8_t relayNumber[20];
	uint8_t brojKorisnika; 
	
};

struct info
{
	char UART[8];
	char FinalUart[8];
	uint8_t uartRecieve; 
	uint8_t previousUart;
	uint8_t concatenatedNumber;	
	uint8_t uneseneZnamenke;			//HH/MM/SS
	uint8_t unosVrijednostiKorisnika;  	//Otvaranje-Zatvaranje-BrojReleja
	uint8_t testI;
	uint8_t functionFlag;
};

void BluetoothSetUp(struct info *varijable, struct users *korisnik);

#endif