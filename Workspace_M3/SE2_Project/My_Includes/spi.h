/*
 * @brief Common SPI functions
 */
#ifndef spi
#define spi

#include "LPC1769_Types.h"


/**
 * @brief SPI registers struct
 */
typedef struct{
	LPC1769_Reg SOSPCR;
	LPC1769_Reg SOSPSR;
	LPC1769_Reg SOSPDR;
	LPC1769_Reg SOSPCCR;
	LPC1769_Reg RESERVED[3];
	LPC1769_Reg SOSPINT;
}LPC1769_SPI;
/** @defgroup SPI : Common SPI functions
 * @ingroup SPI
 * This file contains common SPI functions.
 * @{
 */

/**
 * @brief	Make initialization for SPI communication
 * @param	Nothing
 * @return	Nothing
 */
void SPI_Init(void);

/**
 * @brief	Transfer data via SPI communication
 * @param	data to be written
 * @param	data type, data=1 or command=0
 * @return	Nothing
 */
void SPI_Transfer(char data,char DnC);

/**
 * @}
 */
#endif
