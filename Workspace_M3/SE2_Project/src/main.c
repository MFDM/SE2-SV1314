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
#include "buttons.h"
#include "tea5767.h"

void Inits(void){

}

int main(void) {

	Inits();

	while(1){
		buttons = BUTTONS_Read(BUTTON_U | BUTTON_D);
		if(buttons){
			if((buttons&BUTTON_U )== BUTTON_U )
				TEA5767_SearchUp();
			if((buttons&BUTTON_D )== BUTTON_D )
				TEA5767_SearchUp();
		}
	}
    return 0 ;
}
