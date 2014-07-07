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
	uint8_t *buff;
	int32_t bytes = 0;
	buff = ENET_RXGet(&bytes);
	if (bytes) {
		uip_len = bytes;
		memcpy(uip_buf, buff, bytes);
	}
	Chip_ENET_IncRXConsumeIndex(LPC_ETHERNET);
	return bytes;
}

void tapdev_send(void) {
	uint16_t size;
	void* bufAddr = ENET_TXBuffGet();
	if (bufAddr) {
		size = uip_len;
		memcpy(bufAddr, uip_buf, size);
		ENET_TXQueue(size);
	}
}
