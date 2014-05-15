#include "LPC1769_Addresses.h"
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

LPC1769_Reg* ptr_pconp = LPC1769_PCONP;
LPC1769_Reg* ptr_pclk0 = LPC1769_PCLKSEL0;
LPC1769_PCB* pcbRegs = LPC1769_BASE_PCB;

void SPI_Init(){
	*ptr_pconp &= pcompPins;
	*ptr_pclk0 &= ~pclksel0_17;
	*ptr_pclk0 &= ~pclksel0_16;

	// SCK reseted
	pcbRegs->PINSEL0 &= ~sck_31;
	pcbRegs->PINSEL0 &= ~sck_30;

	// SSEL reseted
	pcbRegs->PINSEL1 &= ~ssel_01;
	pcbRegs->PINSEL1 &= ~ssel_00;

	// MISO mode denied
	pcbRegs->PINSEL1 &= miso_03;
	pcbRegs->PINSEL1 &= miso_02;

	// MOSI mode
	pcbRegs->PINSEL1 &= ~mosi_05;
	pcbRegs->PINSEL1 &= ~mosi_04;
}

int SPI_Tranfer(){

}
