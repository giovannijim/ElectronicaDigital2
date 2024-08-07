/*
 * Lab04_Slave.c
 * Created: 8/1/2024 9:59:49 PM
 * Author : Giovanni Jimenez
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#include "TWI_I2C/TWI_I2C.h"
#include "ADC/initADC.h"

#define SlaveAddress 0x30

uint8_t buffer = 0;

int main(void)
{
    I2C_SETTING_SLAVE(SlaveAddress);
    sei();

    while (1) 
    {	
		initADC(0);							// Inicializar ADC [0]
		ADCSRA |= (1<< ADSC);				// Comenzar conversion
		while(ADCSRA&(1<<ADSC));			// Revisar si la conversion ya termino
		_delay_ms(20);
	
    }
}

// Vector de interrupcion ADC -------------------------------------------------
ISR(ADC_vect)
{
	// Se escribe con un 1 lógico la bandera para apagarla
	ADCSRA |= (1<<ADIF);
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
		TWDR = ADCH; // Cargar el dato en el registro de datos*****************
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

