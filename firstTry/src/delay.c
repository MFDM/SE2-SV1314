#include "delay.h"
/*! delay.c
 * 	Utilizando o SYSCLK.c construiu-se uma função que permite criar
 * 	um breve atraso, para pudermos ter as condições necessárias para 
 * 	alguns processos. 
 */
 
 //DELAY EM MILLIS
 
void delay_init(){
	//! Inicializa a estrutura em SYSCLK com o valor passado por parametro.
	SYSCLK_Init(1000); // 1000 hertz = 1ms
}

void delay(unsigned int millis){
	//! De acordo com o valor do parametro, é criado um atraso de millis ms.
	unsigned initial = SYSCLK_GetValue();
	while(SYSCLK_Elapsed(initial) <  millis);
	
}

unsigned int TicksToMS(unsigned int ticks){
	//! Retorna o numero de Ticks passados por parametro em ms.
	return ticks;
	
}
