#include "sysclk.h"

/*! Faz a iniciação do sistema para permitir o acesso ao periférico Timer 0 como
 * 	relógio de sistema. O Timer 0 deve ser iniciado em modo contínuo e à frequência
 * 	indicada no parâmetro frequency. 
 */

void SYSCLK_Init(unsigned int frequency){
	LPC2106_BASE_TC0->TCR |= 0x2; //!reset timer TCR1 = Counter Reset
	LPC2106_BASE_TC0->TCR &= 0xfc; //enable = 0
	LPC2106_BASE_TC0->CTCR = 0; //!Colocar Timer/Counter em modo Timer
	LPC2106_BASE_TC0->PR = PCLK / frequency; //! ftimer = PCCLK/PR
	LPC2106_BASE_TC0->TCR |= 0x1;//enable = 1
	LPC2106_BASE_TC0->IR = 0x1;
}

/*! Devolve o valor corrente do relógio de sistema, em unidades de contagem
 *  (ticks). 
 */
unsigned int SYSCLK_GetValue(void){
	return LPC2106_BASE_TC0->TC;
}

/*! Devolve o valor decorrido, em unidades de contagem (ticks), desde lastRead
 *	até ao instante corrente, fazendo a subtração entre o utlimo valor lido e o 
 * 	valor actual. 
 */
unsigned int SYSCLK_Elapsed(unsigned int lastRead){
	return SYSCLK_GetValue() - lastRead;
}
