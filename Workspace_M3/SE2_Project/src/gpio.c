#include "gpio.h"
#include "pcb.h"
#include "LPC1769_Addresses.h"

LPC1769_GPIO* ptr_GPIO0 = LPC1769_BASE_GPIO0;
LPC1769_PCB* ptr_PCB = LPC1769_BASE_PCB;
LPC1769_PINMODE * ptr_PINMODE = LPC1769_BASE_PINMODE;


/**
 *adjust a 32bit pinMap to set the pinsel0 and pinsel1
 */
void pinSelect(int pinMap, int* sel0, int* sel1){
	int i;
	for(i=0;i<32;i++)
	{
		if(pinMap%2 == 1){
			if(i<16)
				*sel0 |= 3<<i*2;
			else
				*sel1 |= 3<<(32-(i*2));
		}
		pinMap= pinMap/2;
	}
}

/**
 * set the GPIO port for the pinMap
 */
void GPIO_Init(unsigned int pinMap){
	int auxSel0=0;
	int auxSel1=0;

	pinSelect(pinMap, &auxSel0, &auxSel1);
	ptr_PCB->PINSEL0&=~auxSel0;	//coloca a 0's os bits seleccionados como GPIO
	ptr_PCB->PINSEL1&=~auxSel1;	//coloca a 0's os bits seleccionados como GPIO
	ptr_PINMODE->PINMODE0 = auxSel0;
ptr_PINMODE->PINMODE1 = auxSel1;
}

/**
 * read from the GPIO PIN
 */
unsigned int GPIO_Read(void){
	return ptr_GPIO0->FIOPIN;
}

/**
 *set pin direction for the specific pinMap
 *dir: 0 = input; 1 = output;
 */
void GPIO_SetDir(unsigned int pinMap, unsigned int pinDirectionMap){
	ptr_GPIO0->FIODIR &=(~pinMap);//to clean previous direction
	ptr_GPIO0->FIODIR |=pinDirectionMap;
}

/**
 * write the preparedValue into the specific pinMap
 */
void GPIO_Write(unsigned int preparedValue, unsigned int pinMap){
	unsigned int valueToWriteSET = ((ptr_GPIO0->FIOPIN & ~pinMap) | preparedValue) ;
	ptr_GPIO0->FIOSET = valueToWriteSET;

	unsigned int valueToWriteCLR = ((ptr_GPIO0->FIOPIN & ~pinMap) | (~preparedValue & pinMap)) ;
	ptr_GPIO0->FIOCLR = valueToWriteCLR;
}

