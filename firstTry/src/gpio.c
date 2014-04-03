#include "gpio.h"

//pins = 0x0001 0000
//pin sel 0 = 0x0
//pin sel 1 = 0x3

/** Faz a seleção de quais pins, entre PINSEL0 e PINSEL1, que podem ficar dedicados ao GPIO 
 *  ou com outras funções previamente presentes.
 *  Recebe int pins como parametro, o qual é uma mascara com os pins desejados para o GPIO.
 * 	
 */
void GPIO_PinSelect(unsigned int pins){
	int pos = 0;
	int pinsel0 = LPC2106_BASE_PCB->PINSEL0;
	int pinsel1 = LPC2106_BASE_PCB->PINSEL1;
	for(;pins != 0; pins >>= 1, ++pos){
		if(pins & 1){
			if(pos > 15){
				pinsel1 &= ~(0x3 << (((pos - 16) << 2))); 	//tem de se shiftar 2: 
			}												//pos = 0 -> mask = 0x3 pos = 1 -> mask = 0x3 << 2 = 0xC0
			else{
				pinsel0 &= ~(0x3 << (pos << 2));
			}
		}
	}
	LPC2106_BASE_PCB->PINSEL0 = pinsel0; 
	LPC2106_BASE_PCB->PINSEL1 = pinsel1;
	
}

/** Dita quais os pins de input vão ser usados para o GPIO. 
 *  Chamando o GPIO_PinSelect com os input pins, previamente mencionado, 
 *  e atribuindo ao GPIO os portos de input que vai usar. 
 */
void GPIO_InitInputPins(unsigned int inputPins){
	GPIO_PinSelect(inputPins);
	LPC2106_BASE_GPIO->IODIR &= inputPins;
}

/** Dita quais os pins de output vão ser usados para o GPIO. 
 *  Chamando o GPIO_PinSelect com os output pins, previamente mencionado, 
 *  e atribuindo ao GPIO os portos de output que vai usar.
 */
void GPIO_InitOutputPins(unsigned int outputPins){
	GPIO_PinSelect(outputPins);
	LPC2106_BASE_GPIO->IODIR |= outputPins;
}

/** Esta função tem a tarefa de ler e retornar a informação 
 * presente nos portos de input do GPIO. 
 */
unsigned int GPIO_Read(){
	return LPC2106_BASE_GPIO->IOPIN;
}

/** Esta função faz a escrita de value & pinBitMap, ambos parametros recebidos,
 *  para os portos de output do GPIO
 */
void GPIO_Write(unsigned int value, unsigned int pinBitMap){
	LPC2106_BASE_GPIO->IOSET = value & pinBitMap;
	LPC2106_BASE_GPIO->IOCLR = (~value) & pinBitMap;
}


