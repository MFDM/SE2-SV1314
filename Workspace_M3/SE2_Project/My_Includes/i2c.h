/*
 * @brief Common I2C functions
 */
#ifndef i2c
#define i2c

#include "LPC1769_Types.h"

/**
 * @brief I2C registers struct
 */
typedef struct {
	LPC1769_Reg I2CONSET;
	LPC1769_Reg I2STAT;
	LPC1769_Reg I2DAT;
	LPC1769_Reg I2ADR0;
	LPC1769_Reg I2SCLH;
	LPC1769_Reg I2SCLL;
	LPC1769_Reg I2CONCLR;
	LPC1769_Reg MMCTRL;
	LPC1769_Reg I2ADR1;
	LPC1769_Reg I2ADR2;
	LPC1769_Reg I2ADR3;
	LPC1769_Reg I2DATA_BUFFER;
	LPC1769_Reg I2MAKS0;
	LPC1769_Reg I2MAKS1;
	LPC1769_Reg I2MAKS2;
	LPC1769_Reg I2MAKS3;

}LPC1769_I2C;

/** @defgroup I2C : Common I2C functions
 * @ingroup I2C
 * This file contains common I2C functions.
 * @{
 */

/**
 * @brief	Make initialization for I2C communication
 * @param	Nothing
 * @return	Nothing
 */
void I2C_Init(void);

/**
 * @brief	Transfer data via I2C communication
 * @param	address
 * @param	direction-> read =1 or write=0
 * @param	data to be written
 * @param	data size
 * @param	communication's frequency
 * @return	Response from an I2C transaction
 */
unsigned int I2C_Transfer(unsigned char addr, int read, void *data, unsigned int size, int freq);

/**
 * @}
 */
#endif
