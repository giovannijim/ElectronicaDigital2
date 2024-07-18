/*
 * LCD_8BITS.c
 * Created: 7/18/2024 11:09:57 AM
 *  Author: Giovanni
 */ 

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>

// Inicializar con 8 bits el LCD
void init_8bits_LCD (void){
	DDRB |= (1<<DDB0)|(1<<DDB1);
	PORTB = 0;
	
	DDRD = 0xFF;
	PORTD = 0;
	
	LCD_CMD(0x38);
	_delay_us(200);
	LCD_CMD(0x0C);
	_delay_us(200);
	LCD_CMD(0x06);
	_delay_us(200);
	LCD_CMD(0x01);
}

void LCD_Port(char a){
	if (a & 1)
	//D0 = 1
	PORTD |= (1<<PORTD0);
	else 
	//D0 = 0
	PORTD &= ~(1<<PORTD0);
	
	if (a & 2)
	//D1 = 1
	PORTD |= (1<<PORTD1);
	else
	//D1 = 0
	PORTD &= ~(1<<PORTD1);
	
	if (a & 3)
	//D2 = 1
	PORTD |= (1<<PORTD2);
	else
	//D2 = 0
	PORTD &= ~(1<<PORTD2);
	
	if (a & 4)
	//D3 = 1
	PORTD |= (1<<PORTD3);
	else
	//D3 = 0
	PORTD &= ~(1<<PORTD3);
	
	if (a & 5)
	//D4 = 1
	PORTD |= (1<<PORTD4);
	else
	//D4 = 0
	PORTD &= ~(1<<PORTD4);
	
	if (a & 6)
	//D5 = 1
	PORTD |= (1<<PORTD5);
	else
	//D5 = 0
	PORTD &= ~(1<<PORTD5);
	
	if (a & 7)
	//D6 = 1
	PORTD |= (1<<PORTD6);
	else
	//D6 = 0
	PORTD &= ~(1<<PORTD6);
	
	if (a & 8)
	//D7 = 1
	PORTD |= (1<<PORTD7);
	else
	//D7 = 0
	PORTD &= ~(1<<PORTD7);
}

void LCD_CMD(char a){
	// RS = 0
	PORTB &= ~(1<<PORTB0);
	LCD_Port(a);
	// EN = 1
	PORTB |= (1<<PORTB1);
	_delay_ms(4);
	// EN = 0
	PORTB &= ~(1<<PORTB1);
}

// Funcion para enviar un caracter
void LCD_Write(char c){
	// RS = 1
	PORTB |= (1<<PORTB0);
	LCD_Port(c);
	// EN = 1
	PORTB |= (1<<PORTB1);
	_delay_ms(4);
	// EN = 0
	PORTB &= ~(1<<PORTB1);
}