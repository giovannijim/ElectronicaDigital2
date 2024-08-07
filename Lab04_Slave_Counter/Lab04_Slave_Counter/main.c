/*
 * Lab04_Slave_Counter.c
 * Hardware: Atmega328P
 * Created: 8/2/2024 9:52:38 PM
 * Author : Giovanni Jimenez
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "TWI_I2C/TWI_I2C.h"

#define SlaveAddress 0x40

uint8_t buffer = 0;
uint8_t counter = 0;

void setup(void);
void MostrarLEDs(uint8_t a);

int main(void)
{
    cli();
	I2C_SETTING_SLAVE(SlaveAddress);
	setup();
	sei();
	
    while (1) 
    {
		MostrarLEDs(counter);
    }
}

// Subrutina setup ------------------------------------------------------------
void setup(void){
	// Se apaga tx y rx
	UCSR0B = 0;
	
	// Se establece el puerto D como salida
	PORTD = 0x00;
	DDRD |= 0xFF;
	
	
	// ESTABLECER PULLUP EN PUERTO B0, B1 y B2
	PORTB |= (1<<PORTB1)|(1<<PORTB2);
	
	//ESTABLECER PUERTO B0 Y B1 COMO ENTRADA
	DDRB &= ~((1<<PORTB1)|(1<<PORTB2));
	
	//Habilitar la interrupción puerto B
	PCICR |= (1<<PCIE0);
	
	// Habilitar mascara para pines PB1, PB2
	PCMSK0 |= 0x06;
}

void MostrarLEDs(uint8_t a){
	if(a & 1)
	PORTD |= (1<<PORTD2);
	else
	PORTD &= ~(1<<PORTD2);
	
	if(a & 2)
	PORTD |= (1<<PORTD3);
	else
	PORTD &= ~(1<<PORTD3);
	if(a & 4)
	PORTD |= (1<<PORTD4);
	else
	PORTD &= ~(1<<PORTD4);
	if(a & 8)
	PORTD |= (1<<PORTD5);
	else
	PORTD &= ~(1<<PORTD5);
	
}

// Vector de interrupcion PCINT0 ----------------------------------------------
ISR(PCINT0_vect)
{
	if(!(PINB&(1<<PINB1))) // Si PINB1 se encuentra apagado ejecutar instrucción
	{
		counter++;
	}
	else if(!(PINB&(1<<PINB2))) // Si PINB2 se encuentra apagado ejecutar instrucción
	{
		counter--;
	}
	PCIFR |= (1<<PCIF0); // Apagar la bandera de interrupción
}

ISR(TWI_vect){
	uint8_t estado;
	estado = TWSR & 0xFC;
	switch(estado){
		case 0x60:
		case 0x70:
		TWCR |= (1<<TWINT);
		break;
		case 0x80:
		case 0x90:
		buffer = TWDR;
		TWCR |= (1<<TWINT); //Se limpia la bandera
		break;
		case 0xA8: // SLA+R recibido, maestro solicita lectura
		case 0xB8: // Dato transmitido y ACK recibido
		TWDR = counter; // Cargar el dato en el registro de datos*****************
		TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWIE)| (1 << TWEA); // Listo para la proxima operacion
		break;
		case 0xC0: // Dato transmitido y NACK recibido
		case 0xC8: // Ultimo dato transmitido y ACK recibido
		TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // Listo para la proxima operacion
		break;
		default:	//Se libera el bus de cualquier error
		TWCR |= (1<<TWINT)|(1<<TWSTO);
		break;
	}
}