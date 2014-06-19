#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include "buttons.h"
#include "tea5767.h"
#include "SE2_specific.h"
#include "lcd.h"
#include "enet.h"
#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"

#include "timer.h"

#define BUF 				((struct uip_eth_hdr *)&uip_buf[0])
#define BLACK				0x0

void Inits(void) {
	BUTTONS_Init(MASK_BUTTONS_ALL);
	TEA5767_Init();
	LCD_Init();
	tapdev_init();
	uip_init();
}

int main(void) {
//	short _changes = 0;
//	unsigned int _buttons = 0;
//	int debug = 0;
	int i;
	uip_ipaddr_t ipaddr;
	struct timer periodic_timer, arp_timer;
	SystemCoreClockUpdate();
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

	timer_set(&periodic_timer, CLOCK_SECOND / 2);
	timer_set(&arp_timer, CLOCK_SECOND * 10);

//	tapdev_init();
//	uip_init();
//
	uip_ipaddr(ipaddr, 192, 168, 0, 2);
	uip_sethostaddr(ipaddr);
	uip_ipaddr(ipaddr, 192, 168, 0, 1);
	uip_setdraddr(ipaddr);
	uip_ipaddr(ipaddr, 255, 255, 255, 0);
	uip_setnetmask(ipaddr);

	httpd_init();

	/*  telnetd_init();*/

	/*  {
	 u8_t mac[6] = {1,2,3,4,5,6};
	 dhcpc_init(&mac, 6);
	 }*/

	/*uip_ipaddr(ipaddr, 127,0,0,1);
	 smtp_configure("localhost", ipaddr);
	 SMTP_SEND("adam@sics.se", NULL, "uip-testing@example.com",
	 "Testing SMTP from uIP",
	 "Test message sent by uIP\r\n");*/

	/*
	 webclient_init();
	 resolv_init();
	 uip_ipaddr(ipaddr, 195,54,122,204);
	 resolv_conf(ipaddr);
	 resolv_query("www.sics.se");*/

	while (1) {
//		if (_changes) {
//			//LCD_Clear();
//			//display_freq(stt);
//			_changes = 0;
//		}
//
//		_buttons = BUTTONS_Read(MASK_BUTTONS_ALL);
//
//		if (_buttons) {
//			if ((_buttons & MASK_BUTTONS_U) == MASK_BUTTONS_U) {
//				TEA5767_SearchUp(10);
//				_changes = 1;
//				while (BUTTONS_Read(MASK_BUTTONS_ALL))
//					;
//			} else if ((_buttons & MASK_BUTTONS_D) == MASK_BUTTONS_D) {
//				TEA5767_SearchDown(10);
//				_changes = 1;
//				while (BUTTONS_Read(MASK_BUTTONS_ALL))
//					;
//			}
//		}

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
			}
			else if (BUF->type == htons(UIP_ETHTYPE_ARP)) {
				uip_arp_arpin();
				/* If the above function invocation resulted in data that
				 should be sent out on the network, the global variable
				 uip_len is set to a value > 0. */
				if (uip_len > 0) {
					tapdev_send();
				}
			}

		}
		else if (timer_expired(&periodic_timer)) {
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
	return 0;
}
void
uip_log(char *m)
{
  //printf("uIP log message: %s\n", m);
}
