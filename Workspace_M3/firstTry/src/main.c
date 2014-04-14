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

int state=1;
int LED_PIN = 22;
unsigned int * GPIO_FIO0DIR = 0x2009C000;
unsigned int * GPIO_FIO0MASK = 0x2009C010;
unsigned int * GPIO_FIO0SET = 0x2009C018;
unsigned int * GPIO_FIO0CLR =  0x2009C01C;


void local_gpio_Init(){
	*GPIO_FIO0DIR = 1<<LED_PIN;//1 de output
	*GPIO_FIO0CLR = 1 <<LED_PIN;
	*GPIO_FIO0SET = state <<LED_PIN;
}

void blinkLed() {
	if(state){
		state = 0;
		*GPIO_FIO0CLR = 1 <<LED_PIN;
	}else{
		state=1;
		*GPIO_FIO0SET = state <<LED_PIN;
	}

}
int main(void) {

	local_gpio_Init();

    while(1) {
        blinkLed();
    }
    return 0 ;
}
