#include "i2c.h"
#include "pcb.h"
#include "LPC1769_Addresses.h"
#include "LPC1769_Types.h"
#include "LPC17xx.h"

#define pcompPins 	(1<<7)
#define pclksel0_15 (1<<15)//NUNO estava 15 o qe da bit 16
#define pclksel0_14 (1<<14)//NUNO estava 14 o qe da bit 15
#define sda0_23 	(1<<23)
#define sda0_22		(1<<22)
#define scl0_25 	(1<<25)
#define scl0_24		(1<<24)
#define i2cpadcfg	0xf

#define I2C_AA (1<<2)
#define I2C_SI (1<<3)
#define I2C_STOP (1<<4)
#define I2C_START (1<<5)
#define I2C_EN (1<<6)

#define set(mask) (i2c0Regs->I2CONSET = mask)

#define clear(mask) (i2c0Regs->I2CONCLR = mask)

LPC1769_Reg* ptr_pconp = LPC1769_PCONP;
LPC1769_Reg* ptr_pclk0 = LPC1769_PCLKSEL0;
LPC1769_I2C* i2c0Regs = LPC1769_BASE_I2C0;
LPC1769_PCB* pcbRegs = LPC1769_BASE_PCB;

void I2C_Init(){
	// Setting up the I2C0
	*ptr_pconp |= pcompPins;// NUNO estava AND ..isso limpa os outros
//	*ptr_pclk0 &= ~pclksel0_15; NUNO
//	*ptr_pclk0 &= ~pclksel0_14; NUNO
	*ptr_pclk0 &= ~(pclksel0_15| pclksel0_14);
	*ptr_pclk0 |= (pclksel0_15| pclksel0_14);
	pcbRegs->I2CPADCFG &= ~i2cpadcfg; // NUNO estava 0x0  e ptt limpava os outros

	// Setting up the Pin Select
	pcbRegs->PINSEL1 &= ~sda0_23;
	pcbRegs->PINSEL1 |= sda0_22; // NUNO estava AND
	pcbRegs->PINSEL1 &= ~(scl0_25);
	pcbRegs->PINSEL1 |= scl0_24; //NUNO AND

}

unsigned int I2C_Transfer(unsigned char addr, int read, void *data, unsigned int size, int freq){
	/*
	 *  Data transfer from a master transmitter to a slave receiver. The first byte transmitted
		by the master is the slave address. Next follows a number of data bytes. The slave
		returns an acknowledge bit after each received byte, unless the slave device is unable
		to accept more data.
	 */

//	LER PAGINA 440 MANUAL

	clear(I2C_SI | I2C_START | I2C_AA | I2C_STOP | I2C_EN);
	SystemCoreClockUpdate();
	unsigned div = (SystemCoreClock/8) / (freq * 1000);
	char * cdata = (char *)data;
	unsigned idx = 0;
	i2c0Regs->I2SCLH = div / 2;
	i2c0Regs->I2SCLL = div - (i2c0Regs->I2SCLH);

	// MASTER CONFIGURATION
	set(I2C_EN);
	set(I2C_START); // MASTER MODE


	while(1){
		while(!(i2c0Regs->I2CONSET & I2C_SI));
		switch(i2c0Regs->I2STAT){
			case 0x8:
				// MASTER MODE
				// A condição de start foi enviada
			case 0x10:
				// Repetida a condição de start
				clear(I2C_START);
				i2c0Regs->I2DAT = (addr << 1) | (read? 0x1 : 0x0);
				break;

			case 0x18:
				// MASTER TRANSMITTER
				// Slave escolhido em modo de escrita; ACK foi recebido.
			case 0x28:
				// MASTER TRANSMITTER
				// I2DAT byte de Data foi transmitido; ACK foi recebido.
				i2c0Regs->I2DAT = cdata[idx++];
				if(idx == size){
					 set(I2C_STOP);
					 clear(I2C_SI);
					return 0;
				 }
				break;
			case 0x40:
				// MASTER RECEIVER
				// Slave foi escolhido em modo de leitura; ACK foi recebido.
				if(idx < (size - 1)){
					set(I2C_AA);
				}else{// messagem de 1 byte
					clear(I2C_AA);
				}
				break;
			case 0x50:
				// MASTER RECEIVER
				// Byte de Data recebido; ACK foi recebido.
				cdata[idx++] = i2c0Regs->I2DAT;
				if(idx < (size - 1)){
					set(I2C_AA);
				}else{
					clear(I2C_AA);
				}
				break;
			case 0x58:
				// MASTER RECEIVER
				// Ultimo byte
				cdata[idx] = i2c0Regs->I2DAT;
				set(I2C_STOP);
				clear(I2C_SI);
				return 0;

		}
		clear(I2C_SI);
	}

	return 0;
}
