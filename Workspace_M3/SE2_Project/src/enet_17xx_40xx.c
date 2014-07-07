#include "chip.h"
#include "board.h"
#include <stdio.h>
#include <string.h>
#include "uip.h"
#include "uip_arp.h"
#include "iocon_17xx_40xx.h"
#include "LPC1769_Addresses.h"
#include "LPC1769_Types.h"
#include "SE2_specific.h"
#include "pcb.h"
#include "enet.h"

#define ENET_TXD0 		(1<<0)
#define ENET_TXD1 		(1<<2)
#define ENET_TX_EN 		(1<<8)
#define ENET_CRS 		(1<<16)
#define ENET_RXD0 		(1<<18)
#define ENET_RXD1 		(1<<20)
#define ENET_RX_ER 		(1<<28)
#define ENET_REF_CLK 	(1<<30)
#define ENET_MDC 		(1<<0)
#define ENET_MDIO 		(1<<2)
#define ENET_PINSEL2_CLEAN	((3<<0)|(3<<2)|(3<<8)|(3<<16)|(3<<18)|(3<<20)|(3<<28)|(3<<30))
#define ENET_PINSEL3_CLEAN	((3<<0)|(3<<2))
#define ENET_PINSEL2	((ENET_TXD0)|(ENET_TXD1)|(ENET_TX_EN)|(ENET_CRS)|(ENET_RXD0)|(ENET_RXD1)|(ENET_RX_ER)|(ENET_REF_CLK)|(ENET_REF_CLK))
#define ENET_PINSEL2_JUST_RXD_O_1	((ENET_RXD0)|(ENET_RXD1))
#define ENET_PINSEL3	((ENET_MDC)|(ENET_MDIO))

STATIC uint32_t phyAddr;

STATIC const uint8_t EnetClkDiv[] = { 4, 6, 8, 10, 14, 20, 28, 36, 40, 44, 48,
		52, 56, 60, 64 };

STATIC INLINE void resetENET(LPC_ENET_T *pENET) {
	volatile uint32_t i;

#if defined(CHIP_LPC177X_8X) || defined(CHIP_LPC40XX)
	Chip_SYSCTL_PeriphReset(SYSCTL_RESET_ENET);
#endif

	Chip_ENET_Reset(pENET);
	for (i = 0; i < 100; i++) {
	}
}

STATIC void localMsDelay(uint32_t ms) {
//	ms = ms * 1000000;
	ms = ms * 40000;
	while (ms > 0) {
		ms--;
	}
}

STATIC void Board_Enet_Init(void) {
	////  CODIGO COM TODOS OS PINOS ETHERNET
	//

	LPC1769_Reg* ptr_pcnp = LPC1769_PCONP;
	LPC1769_PCB* pcb_Regs = LPC1769_BASE_PCB;
//	uint32_t debug;
	static unsigned int valueToReg = 0;
	*ptr_pcnp |= (1 << 30); // In the PCONP register, set bit PCENET.
	valueToReg = (ENET_PINSEL2);
//	valueToReg = (ENET_PINSEL2_JUST_RXD_O_1);
	pcb_Regs->PINSEL2 &= (~ENET_PINSEL2_CLEAN);
	pcb_Regs->PINSEL2 |= valueToReg;
//	debug = pcb_Regs->PINSEL2;
	valueToReg = (ENET_PINSEL3);
	pcb_Regs->PINSEL3 &= (~ENET_PINSEL3_CLEAN);
	pcb_Regs->PINSEL3 |= valueToReg;
//	debug = pcb_Regs->PINSEL3;

	////  CODIGO COM PINOS
	//--ENET_MDC Output MIIM clock.
	//--ENET_MDIO Input/Output MI data input and output
	//--ENET_RXD[1:0] Input Receive data, 2 bits.
	// se usarmos o codigo abaixo, COMPILA, MAS NAO ESCREVE NA RAM, DÁ PROBLEMAS
	// E DEPOIS temos qe escrever um codigo sem erros ni IAP com o reset pelo porto 2.10

//	Chip_IOCON_PinMux(LPC_IOCON, 1, 9, IOCON_MODE_PULLUP, IOCON_FUNC1);
//	Chip_IOCON_PinMux(LPC_IOCON, 1, 10, IOCON_MODE_PULLUP, IOCON_FUNC1);
//	Chip_IOCON_PinMux(LPC_IOCON, 1, 16, IOCON_MODE_PULLUP, IOCON_FUNC1);
//	Chip_IOCON_PinMux(LPC_IOCON, 1, 17, IOCON_MODE_PULLUP, IOCON_FUNC1);
}

extern const struct uip_eth_addr macAddr;

/* Basic Ethernet interface initialization */
void Chip_ENET_Init(LPC_ENET_T *pENET, bool useRMII) {
	Board_Enet_Init();

//// ORIGINAL CHIP_ENET_INIT CODE
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_ENET);
	resetENET(pENET);

	/* Initial MAC configuration for  full duplex,
	 100Mbps, inter-frame gap use default values */
	pENET->MAC.MAC1 = ENET_MAC1_PARF;
	pENET->MAC.MAC2 = ENET_MAC2_FULLDUPLEX | ENET_MAC2_CRCEN
			| ENET_MAC2_PADCRCEN;

	if (useRMII) {
		pENET->CONTROL.COMMAND = ENET_COMMAND_FULLDUPLEX
				| ENET_COMMAND_PASSRUNTFRAME | ENET_COMMAND_RMII;
	} else {
		pENET->CONTROL.COMMAND = ENET_COMMAND_FULLDUPLEX
				| ENET_COMMAND_PASSRUNTFRAME;
	}

	pENET->MAC.IPGT = ENET_IPGT_FULLDUPLEX;
	pENET->MAC.IPGR = ENET_IPGR_P2_DEF;
	pENET->MAC.SUPP = ENET_SUPP_100Mbps_SPEED;
	pENET->MAC.MAXF = ENET_ETH_MAX_FLEN;
	pENET->MAC.CLRT = ENET_CLRT_DEF;

	/* Setup default filter */
	pENET->CONTROL.COMMAND |= ENET_COMMAND_PASSRXFILTER;
	/* Clear all MAC interrupts */
	pENET->MODULE_CONTROL.INTCLEAR = 0xFFFF;

	/* Disable MAC interrupts */
	pENET->MODULE_CONTROL.INTENABLE = 0;

	pENET->RXFILTER.CONTROL |= 0x22;
	//When set to ’1’, the frames with a destination address identical to the station address are accepted.

	/////// END OF ORIGINAL CODE

	/* Setup MII clock rate and PHY address */
	Chip_ENET_SetupMII(pENET, Chip_ENET_FindMIIDiv(pENET, 2500000), 1);

	lpc_phy_init(true, localMsDelay);

	/* Setup MAC address for device */
	Chip_ENET_SetADDR(pENET, macAddr.addr);

	/* Setup descriptors */
	InitDescriptors();

	/* Enable RX/TX after descriptors are setup */
	Chip_ENET_TXEnable(pENET);
	Chip_ENET_RXEnable(pENET);
}

void Chip_ENET_Setup(LPC_ENET_T *pENET, bool useRMII) {
}

/* Ethernet interface shutdown */
void Chip_ENET_DeInit(LPC_ENET_T *pENET) {
	/* Disable packet reception */
	pENET->MAC.MAC1 &= ~ENET_MAC1_RXENABLE;
	pENET->CONTROL.COMMAND = 0;

	/* Clear all MAC interrupts */
	pENET->MODULE_CONTROL.INTCLEAR = 0xFFFF;

	/* Disable MAC interrupts */
	pENET->MODULE_CONTROL.INTENABLE = 0;

	Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_ENET);
}

/* Sets up the PHY link clock divider and PHY address */
void Chip_ENET_SetupMII(LPC_ENET_T *pENET, uint32_t div, uint8_t addr) {
	/* Save clock divider and PHY address in MII address register */
	phyAddr = ENET_MADR_PHYADDR(addr);

	/*  Write to MII configuration register and reset */
	pENET->MAC.MCFG = ENET_MCFG_CLOCKSEL(div) | ENET_MCFG_RES_MII;

	/* release reset */
	pENET->MAC.MCFG &= ~(ENET_MCFG_RES_MII);
}

/* Find the divider index for a desired MII clock rate */
uint32_t Chip_ENET_FindMIIDiv(LPC_ENET_T *pENET, uint32_t clockRate) {
	uint32_t tmp, divIdx = 0;

	/* Find desired divider value */
	tmp = Chip_Clock_GetENETClockRate() / clockRate;

	/* Determine divider index from desired divider */
	for (divIdx = 0; divIdx < (sizeof(EnetClkDiv) / sizeof(EnetClkDiv[0]));
			divIdx++) {
		/* Closest index, but not higher than desired rate */
		if (EnetClkDiv[divIdx] >= tmp) {
			return divIdx;
		}
	}

	/* Use maximum divider index */
	return (sizeof(EnetClkDiv) / sizeof(EnetClkDiv[0])) - 1;
}

/* Starts a PHY write via the MII */
void Chip_ENET_StartMIIWrite(LPC_ENET_T *pENET, uint8_t reg, uint16_t data) {
	/* Write value at PHY address and register */
	pENET->MAC.MCMD = 0;
	pENET->MAC.MADR = phyAddr | ENET_MADR_REGADDR(reg);
	pENET->MAC.MWTD = data;
}

/*Starts a PHY read via the MII */
void Chip_ENET_StartMIIRead(LPC_ENET_T *pENET, uint8_t reg) {
	/* Read value at PHY address and register */
	pENET->MAC.MADR = phyAddr | ENET_MADR_REGADDR(reg);
	pENET->MAC.MCMD = ENET_MCMD_READ;
}

/* Read MII data */
uint16_t Chip_ENET_ReadMIIData(LPC_ENET_T *pENET) {
	pENET->MAC.MCMD = 0;
	return pENET->MAC.MRDD;
}

/* Sets full duplex for the ENET interface */
void Chip_ENET_SetFullDuplex(LPC_ENET_T *pENET) {
	pENET->MAC.MAC2 |= ENET_MAC2_FULLDUPLEX;
	pENET->CONTROL.COMMAND |= ENET_COMMAND_FULLDUPLEX;
	pENET->MAC.IPGT = ENET_IPGT_FULLDUPLEX;
}

/* Sets half duplex for the ENET interface */
void Chip_ENET_SetHalfDuplex(LPC_ENET_T *pENET) {
	pENET->MAC.MAC2 &= ~ENET_MAC2_FULLDUPLEX;
	pENET->CONTROL.COMMAND &= ~ENET_COMMAND_FULLDUPLEX;
	pENET->MAC.IPGT = ENET_IPGT_HALFDUPLEX;
}

/* Configures the initial ethernet transmit descriptors */
void Chip_ENET_InitTxDescriptors(LPC_ENET_T *pENET, ENET_TXDESC_T *pDescs,
		ENET_TXSTAT_T *pStatus, uint32_t descNum) {
	/* Setup descriptor list base addresses */
	pENET->CONTROL.TX.DESCRIPTOR = (uint32_t) pDescs;
	pENET->CONTROL.TX.DESCRIPTORNUMBER = descNum - 1;
	pENET->CONTROL.TX.STATUS = (uint32_t) pStatus;
	pENET->CONTROL.TX.PRODUCEINDEX = 0;
}

/* Configures the initial ethernet receive descriptors */
void Chip_ENET_InitRxDescriptors(LPC_ENET_T *pENET, ENET_RXDESC_T *pDescs,
		ENET_RXSTAT_T *pStatus, uint32_t descNum) {
	/* Setup descriptor list base addresses */
	pENET->CONTROL.RX.DESCRIPTOR = (uint32_t) pDescs;
	pENET->CONTROL.RX.DESCRIPTORNUMBER = descNum - 1;
	pENET->CONTROL.RX.STATUS = (uint32_t) pStatus;
	pENET->CONTROL.RX.CONSUMEINDEX = 0;
}

/* Get status for the descriptor list */
ENET_BUFF_STATUS_T Chip_ENET_GetBufferStatus(LPC_ENET_T *pENET,
		uint16_t produceIndex, uint16_t consumeIndex, uint16_t buffSize) {
	/* Empty descriptor list */
	if (consumeIndex == produceIndex) {
		return ENET_BUFF_EMPTY;
	}

	/* Full descriptor list */
	if ((consumeIndex == 0) && (produceIndex == (buffSize - 1))) {
		return ENET_BUFF_FULL;
	}

	/* Wrap-around, full descriptor list */
	if (consumeIndex == (produceIndex + 1)) {
		return ENET_BUFF_FULL;
	}

	return ENET_BUFF_PARTIAL_FULL;
}

/* Get the number of descriptor filled */
uint32_t Chip_ENET_GetFillDescNum(LPC_ENET_T *pENET, uint16_t produceIndex,
		uint16_t consumeIndex, uint16_t buffSize) {
	/* Empty descriptor list */
	if (consumeIndex == produceIndex) {
		return 0;
	}

	if (consumeIndex > produceIndex) {
		return (buffSize - consumeIndex) + produceIndex;
	}

	return produceIndex - consumeIndex;
}

/* Increase the current Tx Produce Descriptor Index */
uint16_t Chip_ENET_IncTXProduceIndex(LPC_ENET_T *pENET) {
	/* Get current TX produce index */
	uint32_t idx = pENET->CONTROL.TX.PRODUCEINDEX;

	/* Start frame transmission by incrementing descriptor */
	idx++;
	if (idx > pENET->CONTROL.TX.DESCRIPTORNUMBER) {
		idx = 0;
	}
	pENET->CONTROL.TX.PRODUCEINDEX = idx;

	return idx;
}

/* Increase the current Rx Consume Descriptor Index */
uint16_t Chip_ENET_IncRXConsumeIndex(LPC_ENET_T *pENET) {
	/* Get current RX consume index */
	uint32_t idx = pENET->CONTROL.RX.CONSUMEINDEX;

	/* Consume descriptor */
	idx++;
	if (idx > pENET->CONTROL.RX.DESCRIPTORNUMBER) {
		idx = 0;
	}
	pENET->CONTROL.RX.CONSUMEINDEX = idx;

	return idx;
}
