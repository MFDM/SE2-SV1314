#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "system_LPC17xx.h"
#include "SE2_specific.h"
#include "buttons.h"
#include "tea5767.h"
#include "lcd.h"
#include "uip.h"
#include "uip_arp.h"
#include "enet.h"
#include "tapdev.h"
#include "iap.h"
#include "timer.h"

#define BUF				((struct uip_eth_hdr *)&uip_buf[0])
#define BLACK				0x0

const struct uip_eth_addr macAddr = { { 0x00, 0x60, 0x37, 0x12, 0x34, 0x56 } };

struct timer periodic_timer, arp_timer;
static int BUTTON_LONG_PRESSED = 0x1;

static void Inits(void) {
	uip_ipaddr_t ipaddr;
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
//	//	BUTTONS_Init(MASK_BUTTONS_ALL);
//	//	TEA5767_Init();
//	//	LCD_Init();
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

static void delaySeconds(int time) {
	int i = time * 1000000;
	for (; i > 0; --i) {
	}
}

static unsigned int check_buttons() {
	unsigned int second_button_press;
	unsigned int first_button_press = BUTTONS_Read(MASK_BUTTONS_ALL);
	if (first_button_press) {
		delaySeconds(1);
		second_button_press = BUTTONS_Read(MASK_BUTTONS_ALL);
		if (second_button_press == first_button_press)
			return first_button_press | BUTTON_LONG_PRESSED; //botao for igual
		else if ((second_button_press | first_button_press)
				== (MASK_BUTTONS_U | MASK_BUTTONS_D))
			return (MASK_BUTTONS_D | MASK_BUTTONS_U); //modo de acertar relogio
		return first_button_press; //discutivel
	}
	return 0; //nenhum botao
}

//static short clock_management(struct tm *dateTime) {
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
//}

//static short radio_management(unsigned int buttons, Station * _stt,
//		short * current_station) {
//	if (buttons & BUTTON_LONG_PRESSED) {
//		if (buttons & MASK_BUTTONS_U)
//			TEA5767_SearchUp(TEA_FREQ);
//		else if (buttons & MASK_BUTTONS_D)
//			TEA5767_SearchDown(TEA_FREQ);
//		return 1;
//	}
//	if (buttons & MASK_BUTTONS_U) {
//		if ((*current_station) + 1 >= (NUM_STATION_MEM + 1))
//			*current_station = 1;
//		else
//			*current_station = *current_station + 1;
//		TEA5767_SetFrequency(*(_stt + (*current_station - 1)), 1);
//	} else if (buttons & MASK_BUTTONS_D) {
//		if ((*current_station) - 1 <= 0)
//			*current_station = NUM_STATION_MEM;
//		else
//			*current_station = *current_station - 1;
//		TEA5767_SetFrequency(*(_stt + (*current_station - 1)), 0);
//	}
//	return 1;
//}

static short save_frequency(Station * stt, short * current_station) {
	//TODO nao eh freq é PLL
	//*(stt + (*current_station)) = TEA5767_GetFrequency();
	return 1;
}

static void save_stations(Station* stt) {
	//TODO copiar array de estaçoess...plural
	Chip_IAP_PreSectorForReadWrite(FLASH_ADDR, FLASH_ADDR);
	Chip_IAP_CopyRamToFlash(FLASH_ADDR, (void*) &stt,
			sizeof(Station) * NUM_STATION_MEM);
}

void EthernetHandle(void) {
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

int main(void) {
	static short _changes = 0;
	static unsigned int _buttons = 0;
//	static int debug = 0;
	Inits();

	LCD_CleanDisplay(BLACK);
	char * a;
	a = "HELLO WORLD!";
	LCD_WriteString(a, 10, 50);
	a = "PRESS ANY ";
	LCD_WriteString(a, 10, 70);
	a = "BUTTON TO";
	LCD_WriteString(a, 20, 80);
	a = "START RADIO";
	LCD_WriteString(a, 14, 90);

	while (1) {
		if (_changes) {
			LCD_CleanDisplay(BLACK);
//			display_freq(stt);
			_changes = 0;
		}

		_buttons = check_buttons();
		if (_buttons) {
			if ((_buttons & (MASK_BUTTONS_U | MASK_BUTTONS_D))
					== (MASK_BUTTONS_U | MASK_BUTTONS_D))
//				_changes = clock_management(&dateTime);
			if ((_buttons & MASK_BUTTONS_U) == MASK_BUTTONS_U
					|| (_buttons & MASK_BUTTONS_D) == MASK_BUTTONS_D)
//				_changes = radio_management(_buttons, stt, &current_station);
			if ((_buttons & (MASK_BUTTONS_M | BUTTON_LONG_PRESSED))
					== (MASK_BUTTONS_M | BUTTON_LONG_PRESSED)) {
//				save_frequency(stt, &current_station);
//				save_stations(stt);
			}
		}
		EthernetHandle();
	}
	return 0;
}

void SysTick_Handler(void) {
}
void uip_log(char *m) {
}
