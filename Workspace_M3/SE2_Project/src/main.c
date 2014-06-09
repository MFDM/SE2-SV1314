
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include "buttons.h"
#include "tea5767.h"
#include "SE2_specific.h"
#include "lcd.h"
#include "enet.h"

#define BLACK				0x0

void Inits(void) {
	BUTTONS_Init(MASK_BUTTONS_ALL);
	TEA5767_Init();
//	LCD_Init();
	InitDescriptors();
	Chip_ENET_Init(LPC_ETHERNET,TRUE);
}

int main(void) {
	short _changes = 0;
	unsigned int _buttons =0;

	Inits();

//	LCD_CleanDisplay(BLACK);
//	char * a;
//	a = "HELLO WORLD!";
//	LCD_WriteString(a,10,50);
//	a = "PRESS ANY ";
//	LCD_WriteString(a,10,70);
//	a="BUTTON TO";
//	LCD_WriteString(a,20,80);
//	a="START RADIO";
//	LCD_WriteString(a,14,90);

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

		int debug = ENET_checkBuffer();
	}
	return 0;
}
