#include "gpio.h"

LPC2106_GPIO* ptr_GPIO = LPC2106_BASE_GPIO;
LPC2106_PCB* ptr_PCB = LPC2106_BASE_PCB;

void GPIO_Init(unsigned int pinMap){//pinMap usa idx de pins mascaras para pins USADOS!
	int i;
	int auxSel0=0;
	int auxSel1=0;
	for(i=0;i<32;i++)
	{
		if(pinMap%2 == 1){
			if(i<16)
				auxSel0 |= 3<<i*2;
			else
				auxSel1 |= 3<<(32-(i*2));
		}
		pinMap= pinMap/2;
	}
	ptr_PCB->PinSel0&=~auxSel0;	//selecciona todos os primeiros 16 bits => 32 bits, 2 bits por pino
	ptr_PCB->PinSel1&=~auxSel1;	//selecciona todos os segundos 16 bits => 32 bits, 2 bits por pino
}
unsigned int GPIO_Read(){
	return ptr_GPIO->IOPin;
}
/*
*dir: 0 = read; 1 = write;
*/
void GPIO_SetDir(unsigned int pinDirectionMap){ // actualiza apenas de 0 para 1 a direccao do pino
	ptr_GPIO->IODir |=pinDirectionMap;
}
/*
* IOSET: 1 ->HIGH
* IOClr: 1->LOW and clear IOSET bit, ptt tem mais peso que o IOSet
*/
void GPIO_Write(unsigned int preparedValue, unsigned int mask){
	unsigned int valueToWriteSET = ((ptr_GPIO->IOSet & ~mask) | preparedValue) ;
	LPC2106_BASE_GPIO->IOSet = valueToWriteSET & ptr_GPIO->IODir;
	unsigned int valueToWriteCLR = ((ptr_GPIO->IOClr & ~mask) | (~preparedValue & mask)) ;
	LPC2106_BASE_GPIO->IOClr = (valueToWriteCLR) & ptr_GPIO->IODir;
}

