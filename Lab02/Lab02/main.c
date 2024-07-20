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

#include "LCD_8BITS/LCD_8BITS.h"
#include "ADC/initADC.h"
#include "UART/UART.h"


void conversion1(char value_adch);
void conversion2(char value_adch);
void conversion3(char value_adch);

uint8_t millares, centenas, decenas, unidades;
// LISTADOS
char lista[10] = {'0','1','2','3','4','5','6','7','8','9'};
char lista1[4];
char lista2[4];
char lista3[4] = {'0','0','0'};
char buffer[5] = {0};
float voltage;

	

int main(void)
{
	cli();
	init_8bits_LCD();
	initUART9600();
	sei();

	LCD_Set_Cursor(0,1);
	LCD_Write_Char('S');
	LCD_Write_Char('1');
	LCD_Write_Char(':');
	
	LCD_Set_Cursor(0,6);
	LCD_Write_Char('S');
	LCD_Write_Char('2');
	LCD_Write_Char(':');
	
	
    while (1) 
    {
		initADC(7);
		ADCSRA |= (1<< ADSC);				// Comenzar conversion
		while(ADCSRA&(1<<ADSC));			// Revisar si la conversion ya termino
		conversion3(ADCH);
		LCD_Set_Cursor(1,1);  //fila y columna
		LCD_Write_String(buffer);
		initADC(6);
		ADCSRA |= (1<< ADSC);				// Comenzar conversion
		while(ADCSRA&(1<<ADSC));			// Revisar si la conversion ya termino
		conversion3(ADCH);
		LCD_Set_Cursor(1,6);  //fila y columna
		LCD_Write_String(buffer);
    }
}

void conversion3(char value_adch){
	int a, b;
	voltage = value_adch * 0.01960784314;
	
	a = (int)voltage;
	b = ((int)(voltage*1000))%1000;
	
	snprintf(buffer, 5, "%d.%0.3d", a, b);
}

void conversion1(char value_adch){
	//Obtener el dato del potenciometro descompuesto:
	centenas = (value_adch)/100;
	decenas = (value_adch-(centenas*100))/10;
	unidades = value_adch-(centenas*100 + decenas*10 );
	
	
	switch(centenas){   //Mostrar centenas
		case 0:
		lista1[0] = lista[0];   //almacenar cada valor, en la lista a mostrar en la pantalla
		break;
		
		
		case 1:
		lista1[0] = lista[1];
		break;
		
		case 2:
		lista1[0] = lista[2];
		break;
		
		
		case 3:
		lista1[0] = lista[3];
		break;
		
		case 4:
		lista1[0] = lista[4];
		break;
		
		
		case 5:
		lista1[0] = lista[5];
		break;
		
		
		case 6:
		lista1[0] = lista[6];
		break;
		
		
		case 7:
		lista1[0] = lista[7];
		break;
		
		case 8:
		lista1[0] = lista[8];
		break;
		
		
		case 9:
		lista1[0] = lista[9];
		break;
		
	}
	
	
	switch(decenas){   //Mostrar decenas
		case 0:
		lista1[1] = lista[0];
		break;
		
		
		case 1:
		lista1[1] = lista[1];
		break;
		
		case 2:
		lista1[1] = lista[2];
		break;
		
		
		case 3:
		lista1[1] = lista[3];
		break;
		
		case 4:
		lista1[1] = lista[4];
		break;
		
		
		case 5:
		lista1[1] = lista[5];
		break;
		
		
		case 6:
		lista1[1] = lista[6];
		break;
		
		
		case 7:
		lista1[1] = lista[7];
		break;
		
		case 8:
		lista1[1] = lista[8];
		break;
		
		
		case 9:
		lista1[1] = lista[9];
		break;
		
	}
	
	switch(unidades){   //Mostrar unidades
		case 0:
		lista1[2] = lista[0];
		break;
		
		
		case 1:
		lista1[2] = lista[1];
		break;
		
		case 2:
		lista1[2] = lista[2];
		break;
		
		
		case 3:
		lista1[2] = lista[3];
		break;
		
		case 4:
		lista1[2] = lista[4];
		break;
		
		
		case 5:
		lista1[2] = lista[5];
		break;
		
		
		case 6:
		lista1[2] = lista[6];
		break;
		
		
		case 7:
		lista1[2] = lista[7];
		break;
		
		case 8:
		lista1[2] = lista[8];
		break;
		
		
		case 9:
		lista1[2] = lista[9];
		break;
		
	}
}

void conversion2(char value_adch){
	//Obtener el dato del potenciometro descompuesto:
	centenas = (value_adch)/100;
	decenas = (value_adch-( centenas*100))/10;
	unidades = value_adch-( centenas*100 + decenas*10 );
	
	
	switch(centenas){   //Mostrar centenas
		case 0:
		lista2[0] = lista[0];   //almacenar cada valor, en la lista a mostrar en la pantalla
		break;
		
		
		case 1:
		lista2[0] = lista[1];
		break;
		
		case 2:
		lista2[0] = lista[2];
		break;
		
		case 3:
		lista2[0] = lista[3];
		break;
		
		case 4:
		lista2[0] = lista[4];
		break;
		
		case 5:
		lista2[0] = lista[5];
		break;
		
		case 6:
		lista2[0] = lista[6];
		break;
		
		case 7:
		lista2[0] = lista[7];
		break;
		
		case 8:
		lista2[0] = lista[8];
		break;
		
		case 9:
		lista2[0] = lista[9];
		break;
		
	}
	
	
	switch(decenas){   //Mostrar decenas
		case 0:
		lista2[1] = lista[0];
		break;
		
		case 1:
		lista2[1] = lista[1];
		break;
	
		case 2:
		lista2[1] = lista[2];
		break;
		
		case 3:
		lista2[1] = lista[3];
		break;
		
		case 4:
		lista2[1] = lista[4];
		break;
		
		case 5:
		lista2[1] = lista[5];
		break;
		
		case 6:
		lista2[1] = lista[6];
		break;
		
		case 7:
		lista2[1] = lista[7];
		break;
		
		case 8:
		lista2[1] = lista[8];
		break;
		
		case 9:
		lista2[1] = lista[9];
		break;
		
	}
	
	switch(unidades){   //Mostrar unidades
		case 0:
		lista1[2] = lista[0];
		break;
		
		case 1:
		lista2[2] = lista[1];
		break;
		
		case 2:
		lista2[2] = lista[2];
		break;
			
		case 3:
		lista2[2] = lista[3];
		break;
	
		case 4:
		lista2[2] = lista[4];
		break;
			
		case 5:
		lista2[2] = lista[5];
		break;
		
		case 6:
		lista2[2] = lista[6];
		break;
		
		case 7:
		lista2[2] = lista[7];
		break;
		
		case 8:
		lista2[2] = lista[8];
		break;
		
		case 9:
		lista2[2] = lista[9];
		break;
		
	}
}


// Vector de interrupcion ADC -------------------------------------------------
ISR(ADC_vect)
{
	// Se escribe con un 1 lógico la bandera para apagarla
	ADCSRA |= (1<<ADIF);
}