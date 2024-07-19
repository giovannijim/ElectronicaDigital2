/*
 * LCD_8BITS.c
 * Created: 7/18/2024 11:09:57 AM
 *  Author: Giovanni
 */ 

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>


void LCD_Port(char a){
	if (a & 1)
	//D0 = 1
	PORTD |= (1<<PORTD4);
	else
	//D0 = 0
	PORTD &= ~(1<<PORTD4);
	
	if (a & 2)
	//D1 = 1
	PORTD |= (1<<PORTD5);
	else
	//D1 = 0
	PORTD &= ~(1<<PORTD5);
	
	if (a & 3)
	//D2 = 1
	PORTD |= (1<<PORTD6);
	else
	//D2 = 0
	PORTD &= ~(1<<PORTD6);
	
	if (a & 4)
	//D3 = 1
	PORTD |= (1<<PORTD7);
	else
	//D3 = 0
	PORTD &= ~(1<<PORTD7);
	
	if (a & 5)
	//D4 = 1
	PORTB |= (1<<PORTB0);
	else
	//D4 = 0
	PORTB &= ~(1<<PORTB0);
	
	if (a & 6)
	//D5 = 1
	PORTB |= (1<<PORTB1);
	else
	//D5 = 0
	PORTB &= ~(1<<PORTB1);
	
	if (a & 7)
	//D6 = 1
	PORTB |= (1<<PORTB2);
	else
	//D6 = 0
	PORTB &= ~(1<<PORTB2);
	
	if (a & 8)
	//D7 = 1
	PORTB |= (1<<PORTB3);
	else
	//D7 = 0
	PORTB &= ~(1<<PORTB3);
}

void LCD_CMD(char a){
	// RS = 0
	PORTD &= ~(1<<PORTD2);
	LCD_Port(a);
	// EN = 1
	PORTD |= (1<<PORTD3);
	_delay_ms(4);
	// EN = 0
	PORTD &= ~(1<<PORTD3);
}

// Inicializar con 8 bits el LCD
void init_8bits_LCD (void){
	DDRB |= (1<<DDB0)|(1<<DDB1)|(1<<DDB2)|(1<<DDB3);
	PORTB = 0;
	
	DDRD = (1<<DDD2)|(1<<DDD3)|(1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7);
	PORTD = 0;
	
	LCD_CMD(0x00);
	_delay_ms(20);
	LCD_CMD(0x30);
	_delay_ms(5);
	LCD_CMD(0x30);
	_delay_ms(5);
	LCD_CMD(0x30);
	_delay_ms(5);
	LCD_CMD(0x38);
	_delay_ms(1);
	LCD_CMD(0x08);
	_delay_ms(1);
	LCD_CMD(0x01);
	_delay_ms(1);
	LCD_CMD(0x05);
	
	LCD_CMD(0x0C);
}

// Funcion para enviar un caracter
void LCD_Write_Char(char c){
	// RS = 1
	PORTD |= (1<<PORTD2);
	LCD_Port(c);
	// EN = 1
	PORTD |= (1<<PORTD3);
	_delay_ms(4);
	// EN = 0
	PORTD &= ~(1<<PORTD3);
}

// Funcion para mandar una cadena de caracteres
void LCD_Write_String(char *a){
	int i;
	for (i=0; a[i] != '\0'; i++)
		LCD_Write_Char(a[i]);
}

void LCD_Shift_Right(void){
	LCD_CMD(0x01);
	LCD_CMD(0x0C);
}

void LCD_Shift_Left(void){
	LCD_CMD(0x01);
	LCD_CMD(0x08);
}

void LCD_Set_Cursor(char c, char f){
	char temp, z, y;
	if (f == 1){
		temp = 0x80 + c -1;
		z = temp >> 4;
		y = temp & 0x0F;
		LCD_CMD(z);
		LCD_CMD(y);
	}	else if (f == 2) {
		temp = 0xC0 + c -1;
		z = temp >> 4;
		y = temp & 0x0F;
		LCD_CMD(z);
		LCD_CMD(y);
	}
}