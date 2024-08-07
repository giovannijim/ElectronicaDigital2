/*
 * I2C.c
 * Created: 7/31/2024 12:55:05 PM
 * Author: Giovanni Jimenez
 */ 

#define F_CPU 16000000

#include "I2C.h"

#include <avr/io.h>
#include <stdint.h>

// Funcion para inicializar I2C Maestro

void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler){
	DDRC &= ~((1<<DDC4)|(1<<DDC5));	// Pines de I2C como entradas
	
	switch(Prescaler){
		case 1:
			TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
		break;
		case 4:
			TWSR &= ~(1<<TWPS1);
			TWSR |= (1<<TWPS0);
		break;
		case 16:
			TWSR &= ~(1<<TWPS0);
			TWSR |= (1<<TWPS1);
		break;
		case 64:
			TWSR |= (1<<TWPS1)|(1<<TWPS0);
		break;
		default:
			TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
			Prescaler = 1;
		break;
	}
	TWBR = ((F_CPU/SCL_Clock)-16)/(2*Prescaler);
	TWCR |= (1<<TWEN);
}

//Funcion de inicio de la comunciacion I2C
uint8_t I2C_Master_Start(void){
	uint8_t estado;
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // Inicializar condicion de start
	while(!(TWCR & (1<< TWINT))); //Espera a que termine la flag TWINT
	
	estado = TWSR & 0xF8; //Verificar estado
	if ((estado != 0x08)||(estado!=0x10)){
		return 1;
	}
	else {
		return estado;
	}
}

//Funcion de parada para la comunicacion I2C
void I2C_Master_Stop(void){
	TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO);	//Inicia el envio secuencia parada
	while(TWCR & (1<<TWSTO)); // Esperamos a que el bit se limpie
}

//Funcion de transmision de datos del maestro al esclavo
//Esta funcion devolvera un 0 si el esclavo a recibido el dato

uint8_t I2C_Master_Write(uint8_t dato){
	uint8_t estado;
	
	TWDR = dato; //Cargar el dato
	TWCR = (1<<TWEN)|(1<<TWINT); // Inicia el envio
	
	while(!(TWCR & (1<<TWINT)));	// Espera al flag TWINT
	estado = TWSR & 0xF8;			// Verficar estado
	// Verificar si se transmitio una SLA + W con ACK, SLA + R con ACK, o un Dato
	if(estado == 0x18 || estado == 0x28 || estado == 0x40){
		return 1;
	}	else {
		return estado;
	}
}

uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack){
	uint8_t estado;
	if (ack)
	{
		TWCR |= (1<<TWEA); // Lectura con ACK
	} else {
		TWCR &= ~(1<<TWEA); // Lectura sin ACK
	}
	
	TWCR |= (1<<TWINT);	//Iniciamos la lectura
	while(!(TWCR & (1<<TWINT)));	// Espera al flag TWINT
	estado = TWSR & 0xF8;	// Verificar estado
	
	// Verificar dato leido con ACK o sin ACK
	if(estado == 0x58||estado == 0x50){
		*buffer = TWDR;
		return 1;
	} else {
		return estado;
	}
}

//Funcion para inicialiar I2C Esclavo
void I2C_Slave_Init(uint8_t address){
	DDRC &= ~((1<<DDC4)|(1<<DDC5)); // Pines de I2C como entradas
	
	TWAR = address << 1 & 0b11111110; //Se asigna la direccion que tendra
	
	//Se habilita la interfaz, ACK automatico, se habilita la ISR
	TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWIE);
}