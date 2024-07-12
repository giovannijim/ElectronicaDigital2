/*
 * main.c
 *
 * Created: 7/12/2024 11:06:28 AM
 *  Author: angel
 */ 


#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t iniciar=0;
volatile uint8_t tabla[]= {0b00111111, 0b00000110, 0b01011011 , 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111, 0b01110111, 0b01111100, 0b00111001, 0b01011110, 0b01111001, 0b01110001};

void init_pins(){
	// Configurar los pines PC0 como entradas y habilitar resistencias de pull-up
	DDRC &= ~(1 << DDC0);
	PORTC |= (1 << PORTC0);
	
	DDRD |= (1 << DDD0) | (1 << DDD1) | (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7); // PD0-PD7 como salidas
	
	
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT8);        // Habilitar interrupción para PC0 (PCINT8)
	
	// Habilitar interrupciones globales
	sei();
}

ISR(PCINT1_vect) {
	// Incrementar el contador
	if (iniciar==0){
		iniciar = 1;
		}
	
}


int main(void)
{
	init_pins();
    while(1)
    {
        if (iniciar==1){
			PORTD &=0;
			PORTD |=0b01101101;
			_delay_ms(200);
			PORTD &=0;
			PORTD |=0b01100110;
			_delay_ms(200);
			PORTD &=0;
			PORTD |=0b01001111;
			_delay_ms(200);
			PORTD &=0;
			PORTD |=0b01011011;
			_delay_ms(200);
			PORTD &=0;
			PORTD |=0b00000110;
			_delay_ms(200);
			PORTD &=0;
			PORTD |=0b00111111;
			_delay_ms(200);
			PORTD &=0;
			iniciar = 0;
    }
	}
}

