/*
 * LCD_4BIT.h
 *
 * Created: 7/16/2024 5:19:55 PM
 *  Author: alesa
 */ 


#ifndef LCD_4BIT_H_
#define LCD_4BIT_H_

#define E (1<<PORTC1)

// fUNCION PARA INICIALIZAR lcd EN MODO 4 BITS
void initLCD4bit(void);

// FUNCION PARA COLOCAR EN EL PUERTO UN VALOR

void LCD_Port(char a);

// Funcion para enviar un comando
void LCD_CMD (char a);

#endif /* LCD_4BIT_H_ */