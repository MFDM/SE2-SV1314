/*
 * @brief Common Ethernet functions
 */
#ifndef ethernet
#define ethernet

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


/** @defgroup Ethernet : Common Ethernet functions
 * @ingroup Ethernet
 * This file contains common Ethernet functions.
 * @{
 */

/**
 * @brief	Initialize MAC descriptors for simple packet receive/transmit
 * @param	Nothing
 * @return	Nothing
 */
void InitDescriptors(void);

/**
 * @brief	Get the pointer to the Rx buffer storing new received frame
 * @param	Nothing
 * @return	Nothing
 */
void *ENET_RXGet(int32_t *bytes);

/**
 * @brief	Release Rx Buffer
 * @param	Nothing
 * @return	Nothing
 */
void ENET_RXBuffClaim(void);

/**
 * @brief	Debugging purposes
 * @param	Nothing
 * @return	Nothing
 */
int ENET_checkBuffer(void);

/**
 * @brief	Get Tx Buffer for the next transmission
 * @param	Nothing
 * @return	Nothing
 */
void *ENET_TXBuffGet(void);

/**
 * @brief	Queue a new frame for transmission
 * @param	Nothing
 * @return	Nothing
 */
void ENET_TXQueue(int32_t bytes);

/**
 * @brief	Check if tranmission finished
 * @param	Nothing
 * @return	Nothing
 */
bool ENET_IsTXFinish(void);

/**
 * @}
 */
#endif
