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
LPC1769_GPIO* gpio = LPC1769_BASE_GPIO0;

void local_gpio_Init(){
	gpio->GPIO_FIODIR = 1<<LED_PIN;//1 de output
	gpio->GPIO_FIOCLR = 1 <<LED_PIN;
	gpio->GPIO_FIOSET = state <<LED_PIN;
}

void blinkLed() {
	if(state){
		state = 0;
		gpio->GPIO_FIOCLR = 1 <<LED_PIN;
	}else{
		state=1;
		gpio->GPIO_FIOSET = state <<LED_PIN;
	}

}
int main(void) {

	local_gpio_Init();

    while(1) {
        blinkLed();
    }
    return 0 ;
}
