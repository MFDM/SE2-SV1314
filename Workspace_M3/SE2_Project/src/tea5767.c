#include "tea5767.h"
#include "i2c.h"

char c[TEA_SIZE_OF_ARRAY];
unsigned int init_pll, end_pll, pll_value, next_pll;

void TEA5767_Init() {
	I2C_Init();
	init_pll = (4 * (EUROPE_FREQUENCY_MIN * 1000 - 225)) / (XTAL / 1000);
	end_pll = (4 * (EUROPE_FREQUENCY_MAX * 1000 - 225)) / (XTAL / 1000);
	next_pll = (4 * (92 * 1000 - 225)) / (XTAL / 1000); //START AT MIDDLE FREQUENCY
	pll_value = next_pll;
}

void search(int freq, char search_direction) {
	c[0] = MASK_SEARCH_MODE | (MASK_PLL_HIGH & (next_pll >> 8));
	c[1] = MASK_PLL_LOW & next_pll;
	c[2] = search_direction | MASK_SSL;
	c[3] = MASK_XTAL;
	c[4] = MASK_PLL_REF;
	//SEARCH
	I2C_Transfer(ADDR_RADIO, WRITE, c, TEA_SIZE_OF_ARRAY, freq);
	//READ STATE OF SEARCHING..
	I2C_Transfer(ADDR_RADIO, READ, c, TEA_SIZE_OF_ARRAY, freq);
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
			return search(freq, search_direction);
		}
		I2C_Transfer(ADDR_RADIO, READ, c, TEA_SIZE_OF_ARRAY, freq);
	}
}

void TEA5767_SearchUp(int freq) {
	next_pll = pll_value + 12;
	search(freq, MASK_SEARCH_UP);
}

void TEA5767_SearchDown(int freq) {
	next_pll = pll_value - 12;
	search(freq, MASK_SEARCH_DOWN);
}

unsigned int TEA5767_GetFrequency() {
	return ((((XTAL / 1000) * pll_value) / 4) + 225) / 1000;
}

unsigned int Frequency_to_Pll(unsigned int frequency) {
	return ((4 * ((frequency * 1000) - 225)) / (XTAL / 1000));
}

void TEA5767_SetFrequency(unsigned int RFrequency, unsigned int UPnDOWN) {
	next_pll = Frequency_to_Pll(RFrequency);
	if (UPnDOWN) {
		next_pll -= 200;
		search(100, MASK_SEARCH_UP);
	} else {
		next_pll = 200;
		search(100, MASK_SEARCH_DOWN);
	}
}
