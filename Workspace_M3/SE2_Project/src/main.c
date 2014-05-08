
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include "buttons.h"
#include "tea5767.h"

void Inits(void) {
	BUTTONS_Init(BUTTON_U | BUTTON_D);
	TEA5767_Init();
}

int main(void) {

	short changes = 0;
	Inits();

	while (1) {

		if (changes) {
			//LCD_Clear();
			//display_freq(stt);
			changes = 0;
		}

		buttons = BUTTONS_Read(BUTTONS_ALL);

		if (buttons) {
			if ((buttons&BUTTONS_U) == BUTTONS_U){
				TEA5767_SearchUp();
				changes = 1;
			}
			else if ((buttons&BUTTONS_D) == BUTTONS_D){
				TEA5767_SearchDown();
				changes = 1;
			}
		}
	}
	return 0;
}
