/*
 * Lab02.c
 * Created: 7/18/2024 11:05:35 AM
 * Author : Giovanni Jimenez
 * Hardware : Atmega328P
 */ 

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

//Se incluyen las librerias creadas
#include "LCD_8BITS/LCD_8BITS.h"
#include "ADC/initADC.h"
#include "UART/UART.h"

// Se declaran los prototipos a usar
void float_to_char_s1(char value_adch);
void float_to_char_s2(char value_adch);
void int_to_char(int entero);

// Se declara una lista de tipo char
char buffer_s1[5] = {'0'};
char buffer_s2[5] = {'0'};
char buffer_1[3] = {'0'};

// Se declaran las variables a usar
float voltage;
int contador;
volatile uint8_t  bufferRX;

int main(void)
{
	contador = 0;
	cli();						// Se apagan las interrupciones globales
	init_8bits_LCD();			// Se inicializa la pantalla LCD a 8 bits 5 V
	initUART9600();				// Se inicia la comunicacion serial con un baudaje de 9600
	sei();						// Se apagan las interrupciones globales

	LCD_Set_Cursor(0,1);		// Se establece el cursor en x, y
	// Se escriben los siguientes caracteres en la pantalla
	LCD_Write_Char('S');			
	LCD_Write_Char('1');
	LCD_Write_Char(':');
	
	LCD_Set_Cursor(0,6);		// Se establece el cursor en x, y
	// Se escriben los siguientes caracteres en la pantalla
	LCD_Write_Char('S');
	LCD_Write_Char('2');
	LCD_Write_Char(':');

	LCD_Set_Cursor(0,11);		// Se establece el cursor en x, y
	// Se escriben los siguientes caracteres en la pantalla
	LCD_Write_Char('S');
	LCD_Write_Char('3');
	LCD_Write_Char(':');
	
    while (1) 
    {
		initADC(7);							// Inicializar ADC [7]
		ADCSRA |= (1<< ADSC);				// Comenzar conversion
		while(ADCSRA&(1<<ADSC));			// Revisar si la conversion ya termino
		float_to_char_s1(ADCH);				// enviar el valor ADCH a convertirse en una cadena char
		LCD_Set_Cursor(1,1);				//fila y columna
		LCD_Write_String(buffer_s1);			// Mostrar el valor en la pantalla LCD

		initADC(6);							// Inicializar ADC [6]
		ADCSRA |= (1<< ADSC);				// Comenzar conversion
		while(ADCSRA&(1<<ADSC));			// Revisar si la conversion ya termino
		float_to_char_s2(ADCH);				// enviar el valor ADCH a convertirse en una cadena char
		LCD_Set_Cursor(1,6);				// fila y columna
		LCD_Write_String(buffer_s2);			// Mostrar el valor en la pantalla LCD
		
		LCD_Set_Cursor(1,11);				// fila y columna
		LCD_Write_String(buffer_1);			// Mostrar el valor en la pantalla LCD
    }
}

// Subrutina para convertir una variable de tipo float (con valores decimales) a una cadena Char
void float_to_char_s1(char value_adch){
	int a, b;									// Se designa el tipo de variable de a y b
	voltage = value_adch * 0.01960784314;		// Se realizar una regla de 3 para escalar [0-255] a [0.00-5.00]
	
	a = (int)voltage;							// Se obtienen las cifras enteras de la variable float
	b = ((int)(voltage*1000))%1000;				// Se obtienen las cifras decimales de la variable float
	
	snprintf(buffer_s1, 5, "%d.%2.2d", a, b);		// Se convierte une todo en una cadena char
}

// Subrutina para convertir una variable de tipo float (con valores decimales) a una cadena Char
void float_to_char_s2(char value_adch){
	int a, b;									// Se designa el tipo de variable de a y b
	voltage = value_adch * 0.01960784314;		// Se realizar una regla de 3 para escalar [0-255] a [0.00-5.00]
	
	a = (int)voltage;							// Se obtienen las cifras enteras de la variable float
	b = ((int)(voltage*1000))%1000;				// Se obtienen las cifras decimales de la variable float
	
	snprintf(buffer_s2, 5, "%d.%2.2d", a, b);		// Se convierte une todo en una cadena char
}	

// Subrutina para convertir una variable de tipo int a una cadena Char
void int_to_char(int entero){
	// Se convierte la variable entero en una cadena, se especifica que sera un entero decimal,
	// Se almacena en el buffer 1
	sprintf(buffer_1, "%d", entero);
}

// Vector de interrupcion ADC -------------------------------------------------
ISR(ADC_vect)
{
	// Se escribe con un 1 lógico la bandera para apagarla
	ADCSRA |= (1<<ADIF);
}

ISR(USART_RX_vect)
{
	// Escribir en la terminal el valor de cada uno de los potenciometros
	writeUART('\n');
	cadena("Valor S1:");
	cadena(buffer_s1);
	writeUART('\n');
	cadena("Valor S2:");
	cadena(buffer_s2);
	//Se almacena en la variable lo que se recibe de UDR0
	bufferRX = UDR0;
	// Si recibe "0x2B", '+' en ascii, aumenta el contador y lo manda convertir a una cadena Char
	if (bufferRX == 0x2B) {
		contador ++;
		int_to_char(contador);
	}	
	// Si recibe "0x2D", '-' en ascii, disminuye el contador y lo manda convertir a una cadena Char
	else if (bufferRX == 0x2D) {
		contador --;
		int_to_char(contador);
	}
}