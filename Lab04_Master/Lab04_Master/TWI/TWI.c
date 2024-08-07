/*
 * TWI.c
 * Created: 8/2/2024 4:52:12 PM
 *  Author: alesa
 */ 

#include <avr/io.h>

#include "TWI.h"

void TWI_SETTING(void){
	TWBR = 0x02;	// Frecuencia del MCU entre 20
	TWSR = 0;		// Factor de prescala en 1
	TWCR = 1 << TWEN;	// Habilita a la interfaz
}

uint8_t TWI_INIT(void){
	uint8_t status;
	
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // Inicializar condicion de start
	while(!(TWCR & (1<< TWINT))); //Espera a que termine la flag TWINT
	
	status = TWSR & 0xF8; //Verificar estado
	if ((status == 0x08)||(status==0x10))
		return 1;
		
	return status;
}

uint8_t TWI_WRITE(uint8_t dato){
	uint8_t status;
	
	TWDR = dato; //Cargar el dato
	TWCR = (1<<TWEN)|(1<<TWINT); // Inicia el envio
	
	while(!(TWCR & (1<<TWINT)));	// Espera al flag TWINT
	status = TWSR & 0xF8;			// Verficar estado
	// Verificar si se transmitio una SLA + W con ACK, SLA + R con ACK, o un Dato
	if(status == 0x18 || status == 0x28 || status == 0x40)
		return 1;
		
	return status;
}

void TWI_STOP(void){
	TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO);	//Inicia el envio secuencia parada
	while(TWCR & (1<<TWSTO)); // Esperamos a que el bit se limpie
}

uint8_t TWI_READ(uint8_t *dato, uint8_t ack){
	uint8_t status;
	if (ack)
	{
		TWCR |= (1<<TWEA); // Lectura con ACK
		} else {
		TWCR &= ~(1<<TWEA); // Lectura sin ACK
	}
	
	TWCR |= (1<<TWINT);	//Iniciamos la lectura
	while(!(TWCR & (1<<TWINT)));	// Espera al flag TWINT
	status = TWSR & 0xF8;	// Verificar estado
	
	// Verificar dato leido con ACK o sin ACK
	if(status == 0x58||status == 0x50){
		*dato = TWDR;
		return 1;
	}
	return status;
}