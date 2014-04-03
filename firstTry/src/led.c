#include "led.h"
#include "gpio.h"

#define ledOutPut 0x2000



/* Faz a iniciação do sistema para permitir a manipulação do periférico. Deixa o
LED apagado quando state toma o valor 0 ou aceso no caso contrário. */
void LED_Init(int state){
	GPIO_InitOutputPins(ledOutPut);
	GPIO_PinSelect(ledOutPut);
	int outState = state? ledOutPut : 0;
	GPIO_Write(outState, ledOutPut);
}

/* Devolve 0 se o LED está apagado e um valor diferente de zero no caso
contrário. */
int LED_GetState(){
	return ledOutPut & GPIO_Read();
}

/* Apaga o LED se state toma o valor 0 ou acende o LED no caso contrário. */
void LED_SetState(int value){
	int outState = value? ledOutPut : 0;
	GPIO_Write(outState, ledOutPut);
}


