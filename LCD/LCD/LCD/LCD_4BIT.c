/*
 * LCD_4BIT.c
 *
 * Created: 7/16/2024 5:21:36 PM
 *  Author: alesa
 */ 

void initLCD4bit(void){
	DDRC |= (1<<DDC0)|(1<<DDC1)|(1<<DDC2)(1<<DDC3)(1<<DDC4)(1<<DDC5);
	PORTC = 0;
	// se sigue con la secuencia descrita en el diagrama...
}

void LCD_Port(char a){
	if (a & 1){
		// D4 EN 1
		PORTC |= (1<<PORTC2);
	}
	else {
		PORTC &= ~(1<<PORTC2);
	}
	if (a & 2){
		// D4 EN 1
		PORTC |= (1<<PORTC3);
	}
	else {
		PORTC &= ~(1<<PORTC3);
	}
	if (a & 3){
		// D4 EN 1
		PORTC |= (1<<PORTC4);
	}
	else {
		PORTC &= ~(1<<PORTC4);
	}
	if (a & 8){
		// D4 EN 1
		PORTC |= (1<<PORTC5);
	}
	else {
		PORTC &= ~(1<<PORTC5);
	}
}

void LCD_CMD (char a){
	// RS = 0
	PORTC &= ~ (1<<PORTC0);
	LCD_Port(a);
	//Enable = 1
	PORTC |= (1<<PORTC1);
	_delay_ms(4);
	
	PORTC &= ~E;
	
}