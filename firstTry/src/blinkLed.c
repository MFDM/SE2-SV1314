#include "led.h"
#include "delay.h"

void blink(int time){
	LED_Init(1);
	delay_init();
	LED_SetState(0);
	LED_SetState(1);
}
/*
int main(){
	blink(500);
	return 0;
}
*/
