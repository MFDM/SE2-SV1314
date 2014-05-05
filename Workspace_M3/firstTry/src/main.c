/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include "gpio.h"
#include "LPC1769.h"

int state=1;
int LED_PIN = 22;


void local_gpio_Init(LPC1769_GPIO * _gpio){
	_gpio->FIODIR = (1<<LED_PIN);//1 de output
	_gpio->FIOCLR = (1<<LED_PIN);
	_gpio->FIOSET = (state <<LED_PIN);
}

void blinkLed(LPC1769_GPIO * _gpio) {
	if(state){
		state = 0;
		_gpio->FIOCLR = (1 <<LED_PIN);
	}else{
		state=1;
		_gpio->FIOSET = (state <<LED_PIN);
	}

}

int main(void) {
	LPC1769_GPIO * _gpio = LPC1769_BASE_GPIO0;

	local_gpio_Init(_gpio);

    while(1) {
        blinkLed(_gpio);
    }
    return 0 ;
}
