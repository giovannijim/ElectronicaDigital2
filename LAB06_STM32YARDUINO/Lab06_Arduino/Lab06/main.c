/*
 * Lab06.c
 * Hardware: Atmega328P
 * Created: 9/19/2024 9:46:47 PM
 * Author : Giovanni Jimenez
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "UART/UART.h"

uint8_t bufferRX;

int main(void)
{
	cli();
	initUART9600();
    // ESTABLECER PULLUP EN PUERTO
    PORTC |= (1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4)|(1<<PORTC5);
    
    //ESTABLECER PUERTO C0-C5 COMO ENTRADA
    DDRC &= ~((1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4)|(1<<PORTC5));
    
    //Habilitar la interrupción puerto 1
    PCICR |= (1<<PCIE1);
    
    // Habilitar mascara para pines 
    PCMSK1 |= 0x3F;
	sei();
    while (1) 
    {
    }
}

// Vector de interrupcion PCINT0 ----------------------------------------------
ISR(PCINT1_vect)
{
	if(!(PINC&(1<<PINC0))) // Si PINC0 se encuentra apagado ejecutar instrucción
	{
		writeUART('1');
	}
	else if(!(PINC&(1<<PINC1))) // Si PINC1 se encuentra apagado ejecutar instrucción
	{
		writeUART('2');
	}
	else if(!(PINC&(1<<PINC2))) // Si PINC2 se encuentra apagado ejecutar instrucción
	{
		writeUART('3');
	}
	else if(!(PINC&(1<<PINC3))) // Si PINC3 se encuentra apagado ejecutar instrucción
	{
		writeUART('4');
	}
	else if(!(PINC&(1<<PINC4))) // Si PINC4 se encuentra apagado ejecutar instrucción
	{
		writeUART('5');
	}
	else if(!(PINC&(1<<PINC5))) // Si PINC5 se encuentra apagado ejecutar instrucción
	{
		writeUART('6');
	}
	PCIFR |= (1<<PCIF1); // Apagar la bandera de interrupción
}

ISR(USART_RX_vect)
{
	bufferRX = UDR0;
}