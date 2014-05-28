#include "LPC1769_Addresses.h"
#include "LPC1769_Types.h"
#include "pcb.h"
#include "spi.h"

#define spi_freq 400000

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
#define CPOC 	(1<<4)
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

#define set(mask) (spiRegs->SOSPCR = mask)
#define clear(mask) (spiRegs->SOSPCR = mask)

void SPI_Init(){
	*ptr_pcnp |= pcompPins;
	*ptr_pclksel0 |= pclksel0_17;
	*ptr_pclksel0 |= pclksel0_16;

	// SCK reseted
	pcb_Regs->PINSEL0 |= sck_31;
	pcb_Regs->PINSEL0 |= sck_30;

	// SSEL reseted
	pcb_Regs->PINSEL1 |= ssel_01;
	pcb_Regs->PINSEL1 |= ssel_00;

	// MISO mode denied
	pcb_Regs->PINSEL1 |= miso_03;
	pcb_Regs->PINSEL1 |= miso_02;

	// MOSI mode
	pcb_Regs->PINSEL1 |= mosi_05;
	pcb_Regs->PINSEL1 |= mosi_04;

	// SPI0 SCK = : PCLK_SPI / SPCCR0
	spiRegs->SOSPCCR &= (~0xff);
	spiRegs->SOSPCCR |= (0x8);

	// Setting up master mode
	set( BE | CPHA | CPOL | MSTR | B8 | B11 );
	spiRegs->SOSPCR &= (~LSBF);
	spiRegs->SOSPCR &= (~SPIE);
	spiRegs->SOSPCR &= (~B9);
	spiRegs->SOSPCR &= (~B10);
	unsigned int val = spiRegs->SOSPCR; // for debug
}

int SPI_Transfer(char data, char DnC){
	// Setting up clock frequency
	SystemCoreClockUpdate();
	unsigned div = (SystemCoreClock/4) / spi_freq;
	short first_bit = 0x1;
	DnC &= first_bit;

	//transfer
	spiRegs->SOSPDR =((DnC<<7)|data);
	while((spiRegs->SOSPSR & SPIF) != SPIF);
	spiRegs->SOSPDR;// This bit is cleared by first reading this
	//register, then accessing the SPI Data Register.

	return 0;
}

