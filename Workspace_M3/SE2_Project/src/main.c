#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "system_LPC17xx.h"
#include "SE2_specific.h"
#include "LPC1769_Addresses.h"
#include "buttons.h"
#include "tea5767.h"
#include "lcd.h"
#include "uip.h"
#include "uip_arp.h"
#include "enet.h"
#include "tapdev.h"
#include "iap.h"
#include "timer.h"
#include "rtc_17xx_40xx.h"

#define BUF				((struct uip_eth_hdr *)&uip_buf[0])
#define BLACK				0x0
#define _valid_information	0x5a5a

const struct uip_eth_addr macAddr = { { 0x00, 0x60, 0x37, 0x12, 0x34, 0x56 } };

struct timer periodic_timer, arp_timer;
RTC_TIME_T FullTime;
Stations stations;
unsigned int current_station_idx = 0;
static char * frequency_text = "Frequency:";
extern unsigned int init_pll;

static void loadStations(void) {
	PStations flash_stations = FLASH_ADDR;
	unsigned int freq_idx = 0;
	if (Chip_IAP_PreSectorForReadWrite(FLASH_INIT_SECTOR,
	FLASH_END_SECTOR) == IAP_CMD_SUCCESS) {
		stations.frequency[0] = flash_stations->frequency[0];
		stations.frequency[1] = flash_stations->frequency[1];
		stations.frequency[2] = flash_stations->frequency[2];
		stations.frequency[3] = flash_stations->frequency[3];
		stations.valid_information = flash_stations->valid_information;
		if (stations.valid_information != _valid_information) {
			stations.valid_information = _valid_information;
			while (freq_idx < STATIONS_MAX_MEM) {
				stations.frequency[freq_idx++] = init_pll;
			}
		}
	}
}

static void Inits(void) {
	uip_ipaddr_t ipaddr;
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	BUTTONS_Init(MASK_BUTTONS_ALL);
	TEA5767_Init(TEA_FREQ);
	LCD_Init();

	//RTC_init
	Chip_RTC_Init(LPC1769_BASE_RTC);
	FullTime.time[RTC_TIMETYPE_SECOND] = 0;
	FullTime.time[RTC_TIMETYPE_MINUTE] = 0;
	FullTime.time[RTC_TIMETYPE_HOUR] = 14;
	FullTime.time[RTC_TIMETYPE_DAYOFMONTH] = 10;
	FullTime.time[RTC_TIMETYPE_DAYOFWEEK] = 5;
	FullTime.time[RTC_TIMETYPE_DAYOFYEAR] = 279;
	FullTime.time[RTC_TIMETYPE_MONTH] = 7;
	FullTime.time[RTC_TIMETYPE_YEAR] = 2014;
	Chip_RTC_SetFullTime(LPC1769_BASE_RTC, &FullTime);
	Chip_RTC_Enable(LPC1769_BASE_RTC, ENABLE);

	//loadstations from flash
	loadStations();
	//server init
	timer_set(&periodic_timer, CLOCK_SECOND / 2);
	timer_set(&arp_timer, CLOCK_SECOND * 10);
	 tapdev_init();
	 uip_init();

	 uip_setethaddr(macAddr);

	 uip_ipaddr(ipaddr, 192, 168, 0, 2);
	 uip_sethostaddr(ipaddr);
	 uip_ipaddr(ipaddr, 192, 168, 0, 1);
	 uip_setdraddr(ipaddr);
	 uip_ipaddr(ipaddr, 255, 255, 255, 0);
	 uip_setnetmask(ipaddr);

	 httpd_init();
}

static void delaySeconds(unsigned int time) {
	int i = time * 4000000;
	for (; i > 0; --i) {
	}
}

static unsigned int check_buttons() {
	unsigned int second_button_press;
	unsigned int first_button_press = BUTTONS_Read(MASK_BUTTONS_ALL);
	if (first_button_press) {
		delaySeconds(2);
		second_button_press = BUTTONS_Read(MASK_BUTTONS_ALL);
		if (second_button_press == first_button_press)
			return (first_button_press | MASK_BUTTONS_LONG_PRESS); //botao for igual
		return first_button_press;
	}
	return 0; //nenhum botao
}

static void manual_search(unsigned int direction) {
	if (direction == MASK_BUTTONS_U) {
		if (current_station_idx == STATIONS_MAX_MEM)
			current_station_idx = 0;
	} else {
		current_station_idx = STATIONS_MAX_MEM - 1;
	}
	TEA5767_SetFrequency(stations.frequency[current_station_idx]);
}

static short clock_management() {
//	int buttonRead = 0;
//	short changes;
//	LCD_Goto(1, 8);
//	while (BUTTONS_Read(BUTTON_pinMap))
//		;
//	do {
//		buttonRead = 0;
//		buttonRead = BUTTONS_Read(BUTTON_pinMap); // testar se apanha duas vezes quando se quer inc so uma vez
//		if (buttonRead)
//			if (buttonRead != MASK_BUTTON_M) {
//				dateTime->tm_hour = assertField(dateTime->tm_hour, 23,
//						buttonRead);
//				display_date(dateTime);
//				LCD_Goto(1, 8);
//				changes = 1;
//				while (BUTTONS_Read(BUTTON_pinMap))
//					;
//			}
//	} while (buttonRead != BUTTON_M);
//	LCD_Goto(1, 11);
//	while (BUTTONS_Read(BUTTON_pinMap))
//		;
//	do {
//		buttonRead = 0;
//		buttonRead = BUTTONS_Read(BUTTON_pinMap); // testar se apanha duas vezes quando se quer inc so uma vez
//		if (buttonRead)
//			if (buttonRead != BUTTON_M) {
//				dateTime->tm_min = assertField(dateTime->tm_min, 59,
//						buttonRead);
//				display_date(dateTime);
//				LCD_Goto(1, 11);
//				changes = 1;
//				while (BUTTONS_Read(BUTTON_pinMap))
//					;
//			}
//	} while (buttonRead != BUTTON_M);
//	while (BUTTONS_Read(BUTTON_pinMap))
//		;
////blink twice;
//	RTC_Init(dateTime);
//	return changes;
	return 0;
}

static void display_freq(void) {
	unsigned int current_freq = TEA5767_GetFrequency();
	char text_to_print[12];
	text_to_print[0] = 'P';
	text_to_print[1] = 'O';
	text_to_print[2] = 'S';
	text_to_print[3] = ':';
	text_to_print[4] = '0' + current_station_idx;
	text_to_print[5] = ' ';
	text_to_print[8] = '0' + current_freq % 10;
	current_freq /= 10;
	text_to_print[7] = '0' + current_freq % 10;
	current_freq /= 10;
	text_to_print[6] = '0' + current_freq % 10;
	text_to_print[9] = 'F';
	text_to_print[10] = 'M';
	text_to_print[11] = '\0';
	LCD_WriteString(frequency_text, 16, 16);
	//adicionar frequencia
	LCD_WriteString(text_to_print, 24, 24);
}

static void save_current_station(void) {
	unsigned int station_pll = TEA5767_GetFrequencyInPLL();
	if (current_station_idx == STATIONS_MAX_MEM)
		current_station_idx = 0;
	stations.frequency[current_station_idx++] = station_pll;
	if (Chip_IAP_PreSectorForReadWrite(FLASH_INIT_SECTOR,
	FLASH_END_SECTOR) == IAP_CMD_SUCCESS)
		if (Chip_IAP_EraseSector(FLASH_INIT_SECTOR,
		FLASH_END_SECTOR) == IAP_CMD_SUCCESS)
			if (Chip_IAP_PreSectorForReadWrite(FLASH_INIT_SECTOR,
			FLASH_END_SECTOR) == IAP_CMD_SUCCESS)
				station_pll = Chip_IAP_CopyRamToFlash(FLASH_ADDR,
						(void*) &stations, 256);
}

static void EthernetHandle(void) {
	int i;
	uip_len = tapdev_read();
	if (uip_len > 0) {
		if (BUF->type == htons(UIP_ETHTYPE_IP)) {
			uip_arp_ipin();
			uip_input();
			/* If the above function invocation resulted in data that
			 should be sent out on the network, the global variable
			 uip_len is set to a value > 0. */
			if (uip_len > 0) {
				uip_arp_out();
				tapdev_send();
			}
		} else if (BUF->type == htons(UIP_ETHTYPE_ARP)) {
			uip_arp_arpin();
			/* If the above function invocation resulted in data that
			 should be sent out on the network, the global variable
			 uip_len is set to a value > 0. */
			if (uip_len > 0) {
				tapdev_send();
			}
		}

	} else if (timer_expired(&periodic_timer)) {
		timer_reset(&periodic_timer);
		for (i = 0; i < UIP_CONNS; i++) {
			uip_periodic(i);
			/* If the above function invocation resulted in data that
			 should be sent out on the network, the global variable
			 uip_len is set to a value > 0. */
			if (uip_len > 0) {
				uip_arp_out();
				tapdev_send();
			}
		}

#if UIP_UDP
		for(i = 0; i < UIP_UDP_CONNS; i++) {
			uip_udp_periodic(i);
			/* If the above function invocation resulted in data that
			 should be sent out on the network, the global variable
			 uip_len is set to a value > 0. */
			if(uip_len > 0) {
				uip_arp_out();
				tapdev_send();
			}
		}
#endif /* UIP_UDP */

		/* Call the ARP timer function every 10 seconds. */
		if (timer_expired(&arp_timer)) {
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
	}
}

static void showTime(void) {
	static unsigned int aux = 0;
	char time[6];
	aux = FullTime.time[RTC_TIMETYPE_HOUR] / 10;
	time[0] = '0' + (aux % 10);
	aux = FullTime.time[RTC_TIMETYPE_HOUR];
	time[1] = '0' + (aux % 10);
	time[2] = ':';
	aux = FullTime.time[RTC_TIMETYPE_MINUTE] / 10;
	time[3] = '0' + (aux % 10);
	aux = FullTime.time[RTC_TIMETYPE_MINUTE];
	time[4] = '0' + (aux % 10);
	time[5] = '\0';
	LCD_WriteString(time, 40, 40);
}

static void printInformation() {
	LCD_CleanDisplay(BLACK);
	display_freq();
	Chip_RTC_GetFullTime(LPC1769_BASE_RTC, &FullTime);
	showTime();
}

int main(void) {
	static short _changes = 0;
	static unsigned int _buttons = 0;
//	static int debug = 0;
	Inits();
//
	LCD_CleanDisplay(BLACK);
	char * a;
	a = "HELLO WORLD!";
	LCD_WriteString(a, 8, 40);
	a = "PRESS ANY ";
	LCD_WriteString(a, 16, 56);
	a = "BUTTON TO";
	LCD_WriteString(a, 24, 64);
	a = "START RADIO";
	LCD_WriteString(a, 32, 80);

	while (1) {
		if (_changes) {
			printInformation();
			_changes = 0;
		}

		if(((LPC_RTC_T*)LPC1769_BASE_RTC)->TIME[RTC_TIMETYPE_MINUTE] != FullTime.time[RTC_TIMETYPE_MINUTE]){
			Chip_RTC_GetFullTime(LPC1769_BASE_RTC, &FullTime);
			_changes =1;
		}

		_buttons = check_buttons();
		if (_buttons) {
			if ((_buttons
					& (MASK_BUTTONS_U | MASK_BUTTONS_D | MASK_BUTTONS_LONG_PRESS))
					== (MASK_BUTTONS_U | MASK_BUTTONS_D
							| MASK_BUTTONS_LONG_PRESS)) {
				//acerto do relogio
				//clock_management
			}
			if (_buttons & MASK_BUTTONS_U) {
				if (_buttons & MASK_BUTTONS_LONG_PRESS) {
					//automatic search
					TEA5767_SearchUp();
				} else {
					//manual search on saved stations
					manual_search(MASK_BUTTONS_U);
				}
			}
			if (_buttons & MASK_BUTTONS_D) {
				if (_buttons & MASK_BUTTONS_LONG_PRESS) {
					//automatic search
					TEA5767_SearchDown();
				} else {
					//manual search on saved stations
					manual_search(MASK_BUTTONS_D);
				}
			}
//				_changes = radio_management(_buttons, stt, &current_station);
			if ((_buttons & (MASK_BUTTONS_M | MASK_BUTTONS_LONG_PRESS))
					== (MASK_BUTTONS_M | MASK_BUTTONS_LONG_PRESS)) {
				//save current station
				save_current_station();
			}
			_changes = 1;
		}
		EthernetHandle();
	}
	return 0;
}

void SysTick_Handler(void) {
}
void uip_log(char *m) {
}
