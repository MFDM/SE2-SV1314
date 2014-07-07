#include "LPC1769_Types.h"
#include "chip.h"
#include "enet_17xx_40xx.h"
#include "board.h"
#include <stdio.h>
#include <string.h>

#define ENET_NUM_TX_DESC 4
#define ENET_NUM_RX_DESC 4

#define ENET_RX_DESC_BASE        (0x2007C000)
#define ENET_RX_STAT_BASE		 (ENET_RX_DESC_BASE + ENET_NUM_RX_DESC * sizeof(ENET_RXDESC_T))
#define ENET_TX_DESC_BASE        (ENET_RX_STAT_BASE + ENET_NUM_RX_DESC * sizeof(ENET_RXSTAT_T))
#define ENET_TX_STAT_BASE        (ENET_TX_DESC_BASE + ENET_NUM_TX_DESC * sizeof(ENET_TXDESC_T))
#define ENET_RX_BUF_BASE         (ENET_TX_STAT_BASE + ENET_NUM_TX_DESC * sizeof(ENET_TXSTAT_T))
#define ENET_TX_BUF_BASE         (ENET_RX_BUF_BASE  + ENET_NUM_RX_DESC * ENET_ETH_MAX_FLEN)
#define ENET_RX_BUF(i)           (ENET_RX_BUF_BASE + ENET_ETH_MAX_FLEN * i)
#define ENET_TX_BUF(i)           (ENET_TX_BUF_BASE + ENET_ETH_MAX_FLEN * i)


/* Initialize MAC descriptors for simple packet receive/transmit */
void InitDescriptors(void);

/* Get the pointer to the Rx buffer storing new received frame */
void *ENET_RXGet(int32_t *bytes);

/* Release Rx Buffer */
void ENET_RXBuffClaim(void);

/*	 Debugging purposes	*/
int ENET_checkBuffer(void);

/* Get Tx Buffer for the next transmission */
void *ENET_TXBuffGet(void);

/* Queue a new frame for transmission */
void ENET_TXQueue(int32_t bytes);

/* Check if tranmission finished */
bool ENET_IsTXFinish(void);
