#ifndef spi
#define spi

#include "LPC1769_Types.h"

typedef struct{
	LPC1769_Reg SOSPCR;
	LPC1769_Reg SOSPSR;
	LPC1769_Reg SOSPDR;
	LPC1769_Reg SOSPCCR;
	LPC1769_Reg RESERVED[3];
	LPC1769_Reg SOSPINT;
}LPC1769_SPI;

void SPI_Init(void);

int SPI_Transfer();

#endif
