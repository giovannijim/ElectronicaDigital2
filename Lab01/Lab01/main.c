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
uint8_t pointer1, activacion, playable, puntos_jugador1, puntos_jugador2, ganador1, ganador2;

// Declaracion de subrutinas
void setup(void);
void jugador1(void);
void jugador2(void);
void ganador(void);

int main(void)
{
	cli();								// Deshabilitar interrupciones globales
	setup();							// Dirigirse a la subrutina setup
	sei();								// Habilitar interrupciones globales
	
    while (1) 
    {
		jugador1();
		jugador2();
		ganador();	
		
		//  Si se ha presionado el boton de activacion, comenzar la cuenta regresiva
		if (activacion)
		{
			// Mostrar en el SevSeg Display el numero en funcion del contador utilizando la lista
			PORTD = CuentaRegresiva [pointer1]; 
			if (pointer1 < 5){
				pointer1 ++; // Desplazar el pointer
				_delay_ms(1000); // Delay de 1s}
			else {
				activacion = 0; // Desactivar la cuenta regresiva
				pointer1 = 0;	// Reiniciar la posicion del pointer
				_delay_ms(1000); // Delay de 1s
				playable = 1; // Activar la bandera para que se permita jugar
			}
		
		}
    }
}

// Subrutina setup ------------------------------------------------------------
void setup(void){
	// Establecer las variables en 0
	pointer1 = 0;
	puntos_jugador1 = 0;
	puntos_jugador2 = 0;
	ganador1 = 0;
	ganador2 = 0;
	
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
		activacion = 0x01; // Activar la badera para que comienze la cuenta regresiva
		// Colocar en 0 las siguientes variables
		puntos_jugador1 = 0;
		puntos_jugador2 = 0;
		ganador1 = 0;
		ganador2 = 0;
	}
	else if(!(PINB&(1<<PINB1))) // Si PINB1 se encuentra apagado ejecutar instrucción
	{
		// si se encuentra la bandera playable y ningun jugador ha ganado, aumentar los puntos del jugador 1
		if (playable){
			if(!ganador2){
				puntos_jugador1++;
			}
		}
	}
	else if(!(PINB&(1<<PINB2))) // Si PINB2 se encuentra apagado ejecutar instrucción
	{
		// si se encuentra la bandera playable y ningun jugador ha ganado, aumentar los puntos del jugador 2
		if (playable){
			if(!ganador1){
			puntos_jugador2++;
			}
		}
	}
	PCIFR |= (1<<PCIF0); // Apagar la bandera de interrupción
}

void jugador1(void){
	// En funcion de la puntuacion del jugador, prender los leds
	if (puntos_jugador1==0)
	{
		PORTB &= ~((1<<PORTB3)|(1<<PORTB4)|(1<<PORTB5));
		PORTC &= ~(1<<PORTC0);
	}
	else if (puntos_jugador1 == 1)
	{
		PORTB |= (1<<PORTB4);
		PORTB &= ~((1<<PORTB3)|(1<<PORTB5));
		PORTC &= ~(1<<PORTC0);	
	}
	else if (puntos_jugador1 == 2)
	{
		PORTB |= (1<<PORTB3);
		PORTB &= ~((1<<PORTB4)|(1<<PORTB5));
		PORTC &= ~(1<<PORTC0);
	}
	else if (puntos_jugador1 == 3)
	{
		PORTB |= (1<<PORTB5);
		PORTB &= ~((1<<PORTB3)|(1<<PORTB4));
		PORTC &= ~(1<<PORTC0);
	}
	else if (puntos_jugador1 == 4){
		PORTB &= ~((1<<PORTB3)|(1<<PORTB4)|(1<<PORTB5));
		PORTC |= (1<<PORTC0);
		ganador1 = 1; // Si el jugador 1 gano prender su bandera de ganador
	}
	else {
		puntos_jugador1 = 0;	
	}
}

void jugador2(void){
	// En funcion de la puntuacion del jugador, prender los leds
	if (puntos_jugador2==0)
	{
		PORTC &= ~((1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4));
	}
	else if (puntos_jugador2 == 1)
	{
		PORTC |= (1<<PORTC1);
		PORTC &= ~((1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4));
	}
	else if (puntos_jugador2 == 2)
	{
		PORTC |= (1<<PORTC2);
		PORTC &= ~((1<<PORTC1)|(1<<PORTC3)|(1<<PORTC4));
	}
	else if (puntos_jugador2 == 3)
	{
		PORTC |= (1<<PORTC3);
		PORTC &= ~((1<<PORTC1)|(1<<PORTC2)|(1<<PORTC4));
	}
	else if (puntos_jugador2 == 4){
		PORTC |= (1<<PORTC4);
		PORTC &= ~((1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3));
		ganador2 = 1; // Si el jugador 2 gano prender su bandera de ganador
	}
	else {
		puntos_jugador2 = 0;
	}
}
//RUTINA PARA CHECKEAR EL GANADOR
void ganador(void){
	// En funcion de que jugador gano, prender los leds
	if (ganador1)
	{
		PORTB |= ((1<<PORTB3)|(1<<PORTB4)|(1<<PORTB5));
		PORTC |= (1<<PORTC0);
		PORTC &= ~((1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4));
		PORTD = 0xC0;
	}
	else if (ganador2){
		PORTC |= ((1<<PORTC4)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3));
		PORTB &= ~((1<<PORTB3)|(1<<PORTB4)|(1<<PORTB5));
		PORTC &= ~(1<<PORTC0);
		PORTD = 0x6E;
	}
}