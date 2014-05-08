#ifndef i2c
#define i2c

#include "LPC1769.h"

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

void I2C_Init(void);

unsigned I2C_Transfer(unsigned char addr, int read, void *data, unsigned int size, int freq);

#endif
