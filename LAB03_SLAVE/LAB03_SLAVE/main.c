/*
 * LAB03_SLAVE.c
 * Hardware: Atmega328P
 * Created: 7/26/2024 8:29:01 AM
 * Author : Giovanni Jimenez
 */ 

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

#include "ADC/initADC.h"
#include "SPI/SPI.h"

uint8_t valorADC0, valorADC1;

int main(void)
{
	spiInit(SPI_SLAVE_SS, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_FIRST_EDGE);
	SPCR |= (1<<SPIE);
	sei();
    while (1) 
    {
		initADC(0);							// Inicializar ADC [0]
		ADCSRA |= (1<< ADSC);				// Comenzar conversion
		while(ADCSRA&(1<<ADSC));			// Revisar si la conversion ya termino
		valorADC0 = ADCH;
		_delay_ms(100);
		initADC(1);							// Inicializar ADC [1]
		ADCSRA |= (1<< ADSC);				// Comenzar conversion
		while(ADCSRA&(1<<ADSC));			// Revisar si la conversion ya termino
		valorADC1 = ADCH;
		_delay_ms(100);
    }
}


// Vector de interrupcion ADC -------------------------------------------------
ISR(ADC_vect)
{
	
	// Se escribe con un 1 lógico la bandera para apagarla
	ADCSRA |= (1<<ADIF);
}

ISR(SPI_STC_vect){
	uint8_t spiValor = SPDR;
	if(spiValor == 'c'){
		spiWrite(valorADC0);
	}
	else if (spiValor == 'd'){
		spiWrite(valorADC1);
	}
}