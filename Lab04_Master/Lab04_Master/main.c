/*
 * Lab04_Master.c
 * Created: 7/31/2024 12:52:23 PM
 * Author : Giovanni Jimenez
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#include "TWI_I2C/TWI_I2C.h"
#include "LCD/LCD_8BITS.h"

#define slave1 0x30
#define slave2 0x40

void float_to_char_s1(uint8_t value_adch);
void int_to_char(int entero);
char buffer_s1[5] = {'0'};
char buffer_1[3] = {'\0','\0','\0'};
uint8_t Valor_ADC = 0;
uint8_t data = 0;
float voltage = 0;

uint8_t Valor_Contador = 0;

int main(void)
{
	DDRB |= (1<<DDB5);
	cli();						// Se apagan las interrupciones globales
	init_8bits_LCD();			// Se inicializa la pantalla LCD a 8 bits 5 V
	//I2C_Master_Init(100000, 1);	// Inicializar como Master Fsc1 100 kHz, prescaler 1
	I2C_SETTING_MASTER(4, 200);
	sei();						// Se apagan las interrupciones globales
    
	LCD_Set_Cursor(0,3);		// Se establece el cursor en y, x
	// Se escriben los siguientes caracteres en la pantalla
	LCD_Write_Char('S');
	LCD_Write_Char('1');
	LCD_Write_Char(':');
	
	LCD_Set_Cursor(0,10);		// Se establece el cursor en y, x
	// Se escriben los siguientes caracteres en la pantalla
	LCD_Write_Char('S');
	LCD_Write_Char('2');
	LCD_Write_Char(':');
	
    while (1) 
    {
		//PORTB |= (1<<PORTB5);
		
		Valor_ADC = I2C_READ(slave1, &data);
		float_to_char_s1(Valor_ADC);
		LCD_Set_Cursor(1,3);		// Se establece el cursor en y, x
		LCD_Write_String(buffer_s1);
		
		Valor_Contador = I2C_READ(slave2, &data);
		
		int_to_char(Valor_Contador);
		LCD_Set_Cursor(1,10);		// Se establece el cursor en y, x
		LCD_Write_String(buffer_1);
		
    }
}

// Subrutina para convertir una variable de tipo float (con valores decimales) a una cadena Char
void float_to_char_s1(uint8_t value_adch){
	int a, b;									// Se designa el tipo de variable de a y b
	voltage = value_adch * 0.01960784314;		// Se realizar una regla de 3 para escalar [0-255] a [0.00-5.00]
	
	a = (int)voltage;							// Se obtienen las cifras enteras de la variable float
	b = ((int)(voltage*1000))%1000;				// Se obtienen las cifras decimales de la variable float
	
	snprintf(buffer_s1, 5, "%d.%2.2d", a, b);		// Se convierte une todo en una cadena char
}

// Subrutina para convertir una variable de tipo int a una cadena Char
void int_to_char(int entero){
	memset(buffer_1, 0, 3);
	// Se almacena en el buffer 1
	sprintf(buffer_1, "%d", entero);
}