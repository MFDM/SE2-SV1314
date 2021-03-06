#include "LPC1769_Addresses.h"
#include "LPC1769_Types.h"
#include "pcb.h"
#include "spi.h"

#define pcompPins 	(1<<8)
#define pclksel0_17 (1<<17)
#define pclksel0_16 (1<<16)
#define sck_31 		(1<<31)
#define sck_30		(1<<30)
#define ssel_01 	(1<<1)
#define ssel_00		(1<<0)
#define miso_03 	(1<<3)
#define miso_02		(1<<2)
#define mosi_05 	(1<<5)
#define mosi_04		(1<<4)

#define BE 		(1<<2)
#define CPHA 	(1<<3)
#define CPOL 	(1<<4)
#define MSTR	(1<<5)
#define LSBF	(1<<6)
#define SPIE	(1<<7)
#define B8		(1<<8)
#define B9		(1<<9)
#define B10 	(1<<10)
#define B11 	(1<<11)
#define SPIF 	(1<<7)

LPC1769_Reg* ptr_pcnp = LPC1769_PCONP;
LPC1769_Reg* ptr_pclksel0 = LPC1769_PCLKSEL0;
LPC1769_PCB* pcb_Regs = LPC1769_BASE_PCB;
LPC1769_SPI* spiRegs = LPC1769_BASE_SPI;

void SPI_Init(void) {
	*ptr_pcnp |= pcompPins;
	*ptr_pclksel0 &= ((~pclksel0_17) & (~pclksel0_16));

	// SCK reseted
	pcb_Regs->PINSEL0 |= (sck_31 | sck_30);
	// In master mode, the
	//clock must be an even number
	//greater than or equal to 8 (see Section 17.7.4).

	// MOSI mode
	pcb_Regs->PINSEL1 |= (mosi_05 | mosi_04);

	// SPI0 SCK = : PCLK_SPI / SPCCR0
	spiRegs->SOSPCCR &= (~0xff);	// counter a 0?
	spiRegs->SOSPCCR = 8;

	// Setting up master mode
	unsigned int v = 0;
	v = ( BE | CPHA | CPOL | MSTR | B8 | B11);
	spiRegs->SOSPCR |= v;
	v = spiRegs->SOSPCR; // for debug
}

void SPI_Transfer(char data, char DnC) {
	// Setting up clock frequency
	//SystemCoreClockUpdate();
	unsigned int div;
	short first_bit = 0x1;
	DnC &= first_bit;

	//transfer
	div = ((DnC << 8) | data);
	spiRegs->SOSPDR = div;

	while ((spiRegs->SOSPSR & SPIF) != SPIF)
		;
	spiRegs->SOSPDR;	// This bit is cleared by first reading this
	//register, then accessing the SPI Data Register.
}

