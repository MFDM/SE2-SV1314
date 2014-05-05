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
#endif

#define I2C_AA (1<<2)
#define I2C_SI (1<<3)
#define I2C_STOP (1<<4)
#define I2C_START (1<<5)
#define I2C_EN (1<<6)

#define set(mask) (LPC1769_I2C->I2CONSET = mask)

#define clear(mask) (LPC1769_I2C->I2CONCLR = mask)

void I2C_Init(void);

unsigned int I2C_Transfer(unsigned char addr, int read, void *data, unsigned int size, int freq);
