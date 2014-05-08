#include "LPC1769.h"
#include "i2c.h"
#include "pinsel.h"
#include "LPC17xx.h"

#define pcompPins 	(1<<7)
#define pclksel0_15 (1<<15)
#define pclksel0_14 (1<<14)
#define sda0_23 	(1<<23)
#define sda0_22		(1<<22)
#define scl0_25 	(1<<25)
#define scl0_24		(1<<24)
#define i2cpadcfg	0x0

#define I2C_AA (1<<2)
#define I2C_SI (1<<3)
#define I2C_STOP (1<<4)
#define I2C_START (1<<5)
#define I2C_EN (1<<6)

#define set(mask) (LPC1769_BASE_I2C0->I2CONSET = mask)

#define clear(mask) (LPC1769_BASE_I2C0->I2CONCLR = mask)



LPC1769_I2C* i2c0Regs = LPC1769_BASE_I2C0;
LPC1769_PCB* pcbRegs = LPC1769_BASE_PCB;

void I2C_Init(){
	// Setting up the I2C0
	LPC1769_PCONP &= pcompPins;
	LPC1769_PCLKSEL0 &= ~pclksel0_15;
	LPC1769_PCLKSEL0 &= ~pclksel0_14;
	pcbRegs->I2CPADCFG &= i2cpadcfg;

	// Setting up the Pin Select
	pcbRegs->PINSEL1 &= ~sda0_23;
	pcbRegs->PINSEL1 &= sda0_22;
	pcbRegs->PINSEL1 &= ~(scl0_25);
	pcbRegs->PINSEL1 &= scl0_24;

}

unsigned I2C_Transfer(unsigned char addr, int read, void *data, unsigned int size, int freq){
	SystemCoreClockUpdate();
	unsigned div = (SystemCoreClock/4) / (freq * 1000);
	char * cdata = data;
	unsigned idx = 0;
	LPC1769_BASE_I2C->I2SCLH = div / 2;
	LPC1769_BASE_I2C->I2SCLL = div - (LPC1769_BASE_I2C->I2SCLH);

	// MASTER CONFIGURATION
	clear(I2C_SI | I2C_START | I2C_AA);
	set(I2C_EN);
	set(I2C_START); // MASTER MODE


	while(1){
		while(!(LPC1769_BASE_I2C->I2CONSET & I2C_SI));
		switch(LPC1769_BASE_I2C->I2STAT){
			case 0x8:
				// MASTER MODE
				// A condição de start foi enviada
			case 0x10:
				// Repetida a condição de start
				clear(I2C_START);
				LPC1769_BASE_I2C->I2DAT = (addr << 1) | (read? 0x1 : 0x0);
				break;

			case 0x18:
				// MASTER TRANSMITTER
				// Slave escolhido em modo de escrita; ACK foi recebido.
			case 0x28:
				// MASTER TRANSMITTER
				// I2DAT byte de Data foi transmitido; ACK foi recebido.
				LPC1769_BASE_I2C->I2DAT = cdata[idx++];
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
				cdata[idx++] = LPC1769_BASE_I2C->I2DAT;
				if(idx < (size - 1)){
					set(I2C_AA);
				}else{
					clear(I2C_AA);
				}
				break;
			case 0x58:
				// MASTER RECEIVER
				// Ultimo byte
				cdata[idx] = LPC1769_BASE_I2C->I2DAT;
				set(I2C_STOP);
				clear(I2C_SI);
				return 0;

		}
		clear(I2C_SI);
	}


}
