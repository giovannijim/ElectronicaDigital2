/*
 * Lab01.c
 * Created: 7/11/2024 10:26:59 PM
 * Author : Giovanni Jiménez
 * Hardware : Atmega328P
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

// Variables y listas
uint8_t mylist [] = {0xFC, 0xC0, 0x6E, 0xEA, 0xD2, 0xBA, 0xBE, 0xE2, 0xFE, 0xF2, 0xF6, 0x9E, 0x3C, 0xCE, 0x3E, 0x36};
uint8_t CuentaRegresiva [] = {0xBA, 0xD2, 0xEA, 0x6E, 0xC0, 0xFC};
uint8_t pointer1;
uint8_t activacion;

void setup(void);

int main(void)
{
	cli();								// Deshabilitar interrupciones globales
	setup();							// Dirigirse a la subrutina setup
	sei();								// Habilitar interrupciones globales
	
    while (1) 
    {
		if (activacion == 0x01)
		{
		PORTD = 0xBA;
		_delay_ms(1000);
		PORTD = 0xD2;
		_delay_ms(1000);
		PORTD = 0xEA;
		_delay_ms(1000);
		PORTD = 0x6E;
		_delay_ms(1000);
		PORTD = 0xC0;
		_delay_ms(1000);
		PORTD = 0xFC;
		_delay_ms(1000);
		activacion = 0;
		}
    }
}

// Subrutina setup ------------------------------------------------------------
void setup(void){
	// Establecer la variable pointer1 en 0
	pointer1 = 0;
	// Se apaga tx y rx
	UCSR0B = 0;
	// Se establece el puerto D como salida
	PORTD = 0x00;
	DDRD |= 0xFF;
	
	// Se establece el puerto C como salida
	PORTC = 0x00;
	DDRC |= 0xFF;
	
	// ESTABLECER PORT PB3-PB7 como salida
	DDRB |= 0XF8;
	
	// ESTABLECER PULLUP EN PUERTO B0, B1 y B2
	PORTB |= (1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2);
	//ESTABLECER PUERTO B0 Y B1 COMO ENTRADA
	DDRB &= ~((1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2));
	//Habilitar la interrupción puerto B
	PCICR |= (1<<PCIE0);
	// Habilitar mascara para pines PB0, PB1, PB2
	PCMSK0 |= 0x07;
}

// Vector de interrupcion PCINT0 ----------------------------------------------
ISR(PCINT0_vect)
{
	if(!(PINB&(1<<PINB0))) // Si PINB0 se encuentra apagado ejecutar instrucción
	{
		activacion = 0x01;
	}
	else if(!(PINB&(1<<PINB1))) // Si PINB0 se encuentra apagado ejecutar instrucción
	{
		PORTB &= ~(1<<PORTB5);
	}
	else if(!(PINB&(1<<PINB2))) // Si PINB0 se encuentra apagado ejecutar instrucción
	{
		PORTD = 0xFC;
	}
	PCIFR |= (1<<PCIF0); // Apagar la bandera de interrupción
}

