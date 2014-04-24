#ifndef LPC1769
#define LPC1769

typedef volatile unsigned int LPC1769_Reg;

//SYSCLK Pointers
#define LPC1769_BASE_SYSTICK ((LPC1769_SYSTICK*)0xE000E010)	// SYSTICK address

//GPIO Pointers
#define LPC1769_BASE_GPIO0 ((LPC1769_GPIO*)0x2009C000)	// GPIO0 address
#define LPC1769_BASE_GPIO1 ((LPC1769_GPIO*)0x2009C020)	// GPIO1 address
#define LPC1769_BASE_GPIO2 ((LPC1769_GPIO*)0x2009C040)	// GPIO2 address
#define LPC1769_BASE_GPIO3 ((LPC1769_GPIO*)0x2009C060)	// GPIO3 address
#define LPC1769_BASE_GPIO4 ((LPC1769_GPIO*)0x2009C080)	// GPIO4 address

//I2C Pointers
#define LPC1769_BASE_I2C0 ((LPC1769_I2C*)0x4001C000)	// I2C0 address
#define LPC1769_BASE_I2C1 ((LPC1769_I2C*)0x4005C000)	// I2C1 address
#define LPC1769_BASE_I2C2 ((LPC1769_I2C*)0x400AC000)	// I2C2 address

#endif
