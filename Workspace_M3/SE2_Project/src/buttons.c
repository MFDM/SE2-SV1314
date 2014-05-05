#include "buttons.h"
#include "gpio.h"

unsigned int BUTTONS_Read(unsigned int mask){
	return (GPIO_Read()&mask);
}
void BUTTONS_Init(unsigned int pinMap){
	GPIO_Init(pinMap);
	GPIO_SetDir(pinMap);
}
