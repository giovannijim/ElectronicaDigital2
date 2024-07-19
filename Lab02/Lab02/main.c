/*
 * Lab02.c
 * Created: 7/18/2024 11:05:35 AM
 * Author : Giovanni Jimenez
 * Hardware : Atmega328P
 */ 

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>

#include "LCD_8BITS/LCD_8BITS.h"
int main(void)
{
	init_8bits_LCD();
	LCD_Set_Cursor(1,1);
	LCD_Write_Char('H');
	LCD_Write_Char('o');
	LCD_Write_Char('l');
	LCD_Write_Char('a');
    while (1) 
    {
    }
}

