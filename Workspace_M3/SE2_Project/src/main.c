
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include "buttons.h"
#include "tea5767.h"
#include "SE2_specific.h"
#include "lcd.h"
#include "enet.h"

void Inits(void) {
	BUTTONS_Init(MASK_BUTTONS_ALL);
	TEA5767_Init();
	LCD_Init();
}

int main(void) {
	short _changes = 0;
	unsigned int _buttons =0;

	Inits();

	while (1) {

		if (_changes) {
			//LCD_Clear();
			//display_freq(stt);
			_changes = 0;
		}

		_buttons = BUTTONS_Read(MASK_BUTTONS_ALL);

		if (_buttons) {
			if ((_buttons&MASK_BUTTONS_U) == MASK_BUTTONS_U){
				TEA5767_SearchUp(10);
				_changes = 1;
				while(BUTTONS_Read(MASK_BUTTONS_ALL));
			}
			else if ((_buttons&MASK_BUTTONS_D) == MASK_BUTTONS_D){
				TEA5767_SearchDown(10);
				_changes = 1;
				while(BUTTONS_Read(MASK_BUTTONS_ALL));
			}
		}
		LCD_CleanDisplay(0x0);
		LCD_WriteChar('a',70,70);
		LCD_WriteChar('b',80,80);
		LCD_WriteChar('c',60,90);
		char * a;
		a = "ola mundo";
		LCD_WriteString(a,30,30);


		ENET_checkBuffer();
	}
	return 0;
}
