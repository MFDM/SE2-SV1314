#ifndef gpio
#define gpio

#include "LPC1769.h"

typedef struct {

	LPC1769_Reg FIODIR;
	LPC1769_Reg dummy[3];
	LPC1769_Reg FIOMASK;
	LPC1769_Reg FIOPIN;
	LPC1769_Reg FIOSET;
	LPC1769_Reg FIOCLR;

}LPC1769_GPIO;

#endif
