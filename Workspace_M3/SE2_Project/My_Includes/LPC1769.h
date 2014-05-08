#ifndef LPC1769
#define LPC1769

typedef volatile unsigned int LPC1769_Reg;

//SYSCLK Pointers
#define LPC1769_BASE_SYSTICK 	((LPC1769_SYSTICK*)0xE000E010)	// SYSTICK address

//GPIO Pointers
#define LPC1769_BASE_GPIO0 		((LPC1769_GPIO *)0x2009C000)	// GPIO0 address
#define LPC1769_BASE_GPIO1 		((LPC1769_GPIO *)0x2009C020)	// GPIO1 address
#define LPC1769_BASE_GPIO2 		((LPC1769_GPIO *)0x2009C040)	// GPIO2 address
#define LPC1769_BASE_GPIO3 		((LPC1769_GPIO *)0x2009C060)	// GPIO3 address
#define LPC1769_BASE_GPIO4 		((LPC1769_GPIO *)0x2009C080)	// GPIO4 address

//I2C Pointers
#define LPC1769_BASE_I2C0 		((LPC1769_I2C *)0x4001C000)		// I2C0 address
#define LPC1769_BASE_I2C1 		((LPC1769_I2C *)0x4005C000)		// I2C1 address
#define LPC1769_BASE_I2C2 		((LPC1769_I2C *)0x400AC000)		// I2C2 address

//PCB Pointers
#define LPC1769_BASE_PCB		((LPC1769_PCB *)0x4002C000) 	// PCB address


//--------- Clock and Power Control ----------
//Phase Locked Loop (PLL0, Main PLL)
#define LPC1769_PLL0CON 		0x400FC080
#define LPC1769_PLL0CFG 		0x400FC084
#define LPC1769_PLL0STAT 		0x400FC088
#define LPC1769_PLL0FEED 		0x400FC08C

//Clock dividers
#define LPC1769_CCLKCFG 		0x400FC104
#define LPC1769_USBCLKCFG 		0x400FC108
#define LPC1769_PCLKSEL0  		0x400FC1A8

//Power control
#define LPC1769_PCON 			0x400FC0C0
#define LPC1769_PCONP 			0x400FC0C4
#define LPC1769_CLKOUTCFG 		0x400FC1C8
//-------------------------------------------

#endif
