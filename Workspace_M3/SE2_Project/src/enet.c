#include "enet.h"

STATIC ENET_RXDESC_T *pRXDescs = (ENET_RXDESC_T *) ENET_RX_DESC_BASE;
STATIC ENET_RXSTAT_T *pRXStats = (ENET_RXSTAT_T *) ENET_RX_STAT_BASE;
STATIC ENET_TXDESC_T *pTXDescs = (ENET_TXDESC_T *) ENET_TX_DESC_BASE;
STATIC ENET_TXSTAT_T *pTXStats = (ENET_TXSTAT_T *) ENET_TX_STAT_BASE;

/* Transmit/receive buffers and indices */
STATIC int32_t rxConsumeIdx;
STATIC int32_t txProduceIdx;


/* Initialize MAC descriptors for simple packet receive/transmit */
void InitDescriptors(void)
{
	int i;

	/* Setup the descriptor list to a default state */
	memset(pTXDescs, 0, ENET_NUM_TX_DESC * sizeof(ENET_TXDESC_T));
	memset(pTXStats, 0, ENET_NUM_TX_DESC * sizeof(ENET_TXSTAT_T));
	memset(pRXDescs, 0, ENET_NUM_RX_DESC * sizeof(ENET_RXDESC_T));
	memset(pRXStats, 0, ENET_NUM_RX_DESC * sizeof(ENET_RXSTAT_T));

	rxConsumeIdx = 0;
    rxConsumeIdx = 0;

	/* Build linked list, CPU is owner of descriptors */
	for (i = 0; i < ENET_NUM_RX_DESC; i++) {
		pRXDescs[i].Packet = (uint32_t) ENET_RX_BUF(i);
		pRXDescs[i].Control = ENET_RCTRL_SIZE(ENET_ETH_MAX_FLEN);
	}
	for (i = 0; i < ENET_NUM_TX_DESC; i++) {
		pTXDescs[i].Packet = (uint32_t) ENET_TX_BUF(i);
	}

	/* Setup list pointers in Ethernet controller */
	Chip_ENET_InitTxDescriptors(LPC_ETHERNET, pTXDescs, pTXStats, ENET_NUM_TX_DESC);
	Chip_ENET_InitRxDescriptors(LPC_ETHERNET, pRXDescs, pRXStats, ENET_NUM_RX_DESC);
}

/* Get the pointer to the Rx buffer storing new received frame */
void *ENET_RXGet(int32_t *bytes)
{
	uint16_t produceIdx;
	void *buffer=NULL;

	produceIdx = Chip_ENET_GetRXProduceIndex(LPC_ETHERNET);
	/* This doesn't check status of the received packet */
	if (Chip_ENET_GetBufferStatus(LPC_ETHERNET, produceIdx, rxConsumeIdx, ENET_NUM_RX_DESC) != ENET_BUFF_EMPTY) {
		/* CPU owns descriptor, so a packet was received */
		buffer = (void *) pRXDescs[rxConsumeIdx].Packet;
		*bytes = (int32_t) (ENET_RINFO_SIZE(pRXStats[rxConsumeIdx].StatusInfo) - 4);/* Remove CRC */
	}
	else {
		/* Nothing received */
		*bytes = 0;
//		buffer = NULL;
	}

	return buffer;
}

/* Release Rx Buffer */
void ENET_RXBuffClaim(void)
{
	rxConsumeIdx = Chip_ENET_IncRXConsumeIndex(LPC_ETHERNET);
}

// ping = 0x0806

int ENET_checkBuffer(void){
	int32_t bytes;
	void* buffer;
	buffer= ENET_RXGet(&bytes);

	if(bytes != 0) {
		ENET_RXBuffClaim();
		return 1;
	}

	return 0;
}





