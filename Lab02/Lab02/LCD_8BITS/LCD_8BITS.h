/*
 * LCD_8BITS.h
 * Created: 7/18/2024 11:09:43 AM
 *  Author: Giovanni
 */ 


#ifndef LCD_8BITS_H_
#define LCD_8BITS_H_

#define E (1<<PORTB1)

// Inicializar con 8 bits el LCD
void init_8bits_LCD (void);

// Funcion para colocar en el puerto un valor
void LCD_Port(char a);

// Funcion para mandar un comando
void LCD_CMD(char a);

// Funcion para enviar un caracter
void LCD_Write(char c);

#endif /* LCD_8BITS_H_ */