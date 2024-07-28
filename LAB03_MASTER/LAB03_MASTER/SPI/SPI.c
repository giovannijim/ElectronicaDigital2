/*
 * SPI.c
 * Created: 7/27/2024 9:20:41 PM
 *  Author: Giovanni Jimenez
 */ 
#include "SPI.h"

#include <avr/io.h>
#include <stdint.h>

void spiInit(Spi_Type sType, Spi_Data_Order sDataOrder, Spi_Clock_Polarity sClockPolarity, Spi_Clock_Phase sClockPhase){
	
	//PB2 -> SS
	//PB3 -> MOSI
	//PB4 -> MISO
	//PB5 -> SCK
	
	if (sType & (1<<MSTR)) //If Master Mode
	{
		DDRB |= (1<<DDB3)|(1<<DDB5)|(1<<DDB2); // MOSI, SCK, SS
		DDRB &= ~(1<<DDB4);					// MISO
		SPCR |= (1<<MSTR);					// MASTER
		
		uint8_t temp = sType & 0b00000111;
		switch(temp){
			case 0: //Div2
				SPCR &= ~((1<<SPR1)|(1<<SPR0));
				SPSR |= (1<<SPI2X);
			break;
			case 1: //Div4
				SPCR &= ~((1<<SPR1)|(1<<SPR0));
				SPSR &= ~(1<<SPI2X);
			break;
			case 2: //Div8
				SPCR |= (1<<SPR0);
				SPCR &= ~(1<<SPR1);
				SPSR |= (1<<SPI2X);
			break;
			case 3: //Div16
				SPCR |= (1<<SPR0);
				SPCR &= ~(1<<SPR1);
				SPSR &= ~(1<<SPI2X);
			break;
			case 4: //Div32
				SPCR &= ~(1<<SPR0);
				SPCR |= (1<<SPR1);
				SPSR |= (1<<SPI2X);
			break;
			case 5: //Div64
				SPCR &= ~(1<<SPR0);
				SPCR |= (1<<SPR1);
				SPSR &= ~(1<<SPI2X);
			break;
			case 6: //Div128
				SPCR |= (1<<SPR0)|(1<<SPR1);
				SPSR &= ~(1<<SPI2X);
			break;
		}
	}
	else //If slave mode selected
	{
		DDRB |= (1<<DDB4);							//MISO
		DDRB &= ~((1<<DDB3)|(1<<DDB5)|(1<<DDB2));	//MOSI, CK, SS	
		SPCR &= ~(1<<MSTR);							// SLAVE
	}
	/* Enable, Data order, CLock Polarity, Clock Phase */
	SPCR |= (1<<SPE)|sDataOrder|sClockPolarity|sClockPhase;
}

static void spiReceiveWait(){
	while (!(SPSR & (1<<SPIF))); // Wait for Data Receive Complete
}

void spiWrite(uint8_t dat){ //Write data to SPI bus
	SPDR = dat;
}

unsigned spiDataReady(){	//Check whether the data is ready to read
	if(SPSR & (1<<SPIF))
	return 1;
	else
	return 0;
}

char spiRead(){
	while (!(SPSR & (1<<SPIF))); // Wait for Data Receive Complete
	return(SPDR); // Read the received data from the buffer
}