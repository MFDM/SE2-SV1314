#include "chip.h"
#include "lpc_types.h"
#include "enet_17xx_40xx.h"
#include "uip.h"
#include "enet.h"
#include <string.h>

#define useRMMI TRUE

void tapdev_init(void) {
	Chip_ENET_Init(LPC_ETHERNET, useRMMI);
}

unsigned int tapdev_read(void) {
	void* buff = NULL;
	uint32_t bytes = 0;
	buff = ENET_RXGet(&bytes);
	if (bytes) {
		memcpy(uip_buf, buff, bytes);
		uip_len = bytes;
	}
	Chip_ENET_IncRXConsumeIndex(LPC_ETHERNET);
	return bytes;
}

void tapdev_send(void) {
	void* bufAddr = ENET_TXBuffGet();
	if (bufAddr) {
		memcpy(bufAddr, uip_buf, uip_len);
		ENET_TXQueue(uip_len);
	}
}
