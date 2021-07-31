/*
 * Test01.c
 *
 * Created: 08-Oct-18 21:23:12
 * Author : Domagoj Krizanec
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include "button.h"


int main (void)
{	 	
	ButtonInitialisation(); //pozivanje funkcije tipka s postavljenim vrijednostima	
	ButtonPress();     //rad centralnog sustava		
}
		



