
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include "buttons.h"
#include "tea5767.h"
#include "SE2_specific.h"

void Inits(void) {
	BUTTONS_Init(MASK_BUTTONS_U | MASK_BUTTONS_D);
//	TEA5767_Init();
}

int main(void) {
	short _changes = 0;
	unsigned int _buttons =0;

	Inits();

	while (1) {

		if (_changes!=0) {
			//LCD_Clear();
			//display_freq(stt);
			_changes = 0;
		}

		_buttons = BUTTONS_Read(MASK_BUTTONS_ALL);

		if (_buttons!=0) {
			if ((_buttons&MASK_BUTTONS_U) == MASK_BUTTONS_U){
				//TEA5767_SearchUp(0);
				_changes = 1;
			}
			else if ((_buttons&MASK_BUTTONS_D) == MASK_BUTTONS_D){
				//TEA5767_SearchDown(0);
				_changes = 1;
			}
		}
		else _buttons =-1;
	}
	return 0;
}
