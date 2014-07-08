#include "tea5767.h"
#include "i2c.h"

char c[TEA_SIZE_OF_ARRAY];
static int searchFreq;
unsigned int init_pll, end_pll, pll_value, next_pll;

void TEA5767_Init(int freq) {
	I2C_Init();
	init_pll = (4 * (EUROPE_FREQUENCY_MIN * 1000 - 225)) / (XTAL / 1000);
	end_pll = (4 * (EUROPE_FREQUENCY_MAX * 1000 - 225)) / (XTAL / 1000);
	next_pll = (4 * (92 * 1000 - 225)) / (XTAL / 1000); //START AT MIDDLE FREQUENCY
	pll_value = next_pll;
	searchFreq = freq;
}

/**
 * Método utilitário privado
 * @param freq em hz 916000000 = 91.6
 * @returns PLL do rádio correspondente á freq recebida em parametro
 */
unsigned TEA5767_FMToPLL(unsigned freq){
	return (4 * (freq - 225000)) / 32768;
}

/**
 * Método utilitário privado
 * @param pll
 * @returns Frequencia correspondente á pll recebida em parametro
 */
unsigned TEA5767_PLLToFM(unsigned pll){
	return ((pll * 32768) / 4) + 225000;
}


static void transfer(char search_direction){
	//SEARCH
		I2C_Transfer(ADDR_RADIO, WRITE, c, TEA_SIZE_OF_ARRAY, searchFreq);
		//READ STATE OF SEARCHING..
		I2C_Transfer(ADDR_RADIO, READ, c, TEA_SIZE_OF_ARRAY, searchFreq);
		while (1) {
			if (c[0] & MASK_READY_FLAG) {
				if ((c[0] & MASK_BAND_LIMIT_FLAG) != MASK_BAND_LIMIT_FLAG) {
					pll_value = ((c[0] & MASK_PLL_HIGH) << 8)
							| (c[1] & MASK_PLL_LOW);
					return;
				}
				if (search_direction == MASK_SEARCH_UP)
					next_pll = init_pll;
				else
					next_pll = end_pll;
				transfer(search_direction);
			}
			I2C_Transfer(ADDR_RADIO, READ, c, TEA_SIZE_OF_ARRAY, searchFreq);
		}
}

static void search(char search_direction) {
	c[0] = MASK_SEARCH_MODE_ON | (MASK_PLL_HIGH & (next_pll >> 8));
	c[1] = MASK_PLL_LOW & next_pll;
	c[2] = search_direction | MASK_SSL;
	c[3] = MASK_XTAL;
	c[4] = MASK_PLL_REF;
	transfer(search_direction);
}


void TEA5767_SearchUp() {
	next_pll = pll_value + 12;
	search(MASK_SEARCH_UP);
}

void TEA5767_SearchDown() {
	next_pll = pll_value - 12;
	search(MASK_SEARCH_DOWN);
}

unsigned int TEA5767_GetFrequency() {
	return TEA5767_PLLToFM(pll_value);
}

void TEA5767_SetFrequency(unsigned int pllVal) {
	c[0] = MASK_SEARCH_MODE_OFF | (MASK_PLL_HIGH & (next_pll >> 8));
	c[1] = MASK_PLL_LOW & next_pll;
	c[2] = MASK_SSL;
	c[3] = MASK_XTAL;
	c[4] = MASK_PLL_REF;
	transfer(0);
}


