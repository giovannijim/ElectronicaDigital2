/*
 * I2C.h
 * Created: 7/31/2024 12:54:32 PM
 *  Author: alesa
 */ 

#ifndef I2C_H_
#define I2C_H_


#include <avr/io.h>
#include <stdint.h>

void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler);
uint8_t I2C_Master_Start(void);
void I2C_Master_Stop(void);
uint8_t I2C_Master_Write(uint8_t dato);
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack);
void I2C_Slave_Init(uint8_t address);


#endif /* I2C_H_ */