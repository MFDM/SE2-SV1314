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

int main(void) {

	Inits();

	while(1){
		buttons = check_buttons();
		if(buttons){
			if((buttons&BUTTON_U )== BUTTON_U )
				TEA5767_SearchUp();
			if((buttons&BUTTON_D )== BUTTON_D )
				TEA5767_SearchUp();
		}
	}
    return 0 ;
}
