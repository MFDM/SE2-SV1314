#ifndef pcb
#define pcb

#include "LPC1769_Types.h"


/**
 * @brief PCB registers struct
 */
typedef struct {
	LPC1769_Reg PINSEL0;
	LPC1769_Reg PINSEL1;
	LPC1769_Reg PINSEL2;
	LPC1769_Reg PINSEL3;
	LPC1769_Reg PINSEL4;
	LPC1769_Reg PINSEL5;
	LPC1769_Reg PINSEL6;
	LPC1769_Reg PINSEL7;
	LPC1769_Reg PINSEL8;
	LPC1769_Reg PINSEL9;
	LPC1769_Reg PINSEL10;
	LPC1769_Reg PINMODE0;
	LPC1769_Reg PINMODE1;
	LPC1769_Reg PINMODE2;
	LPC1769_Reg PINMODE3;
	LPC1769_Reg PINMODE4;
	LPC1769_Reg PINMODE5;
	LPC1769_Reg PINMODE6;
	LPC1769_Reg PINMODE7;
	LPC1769_Reg PINMODE8;
	LPC1769_Reg PINMODE9;
	LPC1769_Reg dummy[5];
	LPC1769_Reg	I2CPADCFG;

}LPC1769_PCB;


/**
 * @brief PINMODE registers struct
 */
typedef struct {

	LPC1769_Reg PINMODE0;
	LPC1769_Reg PINMODE1;
	LPC1769_Reg PINMODE2;
	LPC1769_Reg PINMODE3;
	LPC1769_Reg PINMODE4;
	LPC1769_Reg PINMODE5;
	LPC1769_Reg PINMODE6;
	LPC1769_Reg PINMODE7;
	LPC1769_Reg PINMODE8;
	LPC1769_Reg PINMODE9;

}LPC1769_PINMODE;

#endif
