#ifndef SE2_specific
#define SE2_specific

#define MASK_BUTTONS_U 		1<<0
#define MASK_BUTTONS_D 		1<<1
#define MASK_BUTTONS_M 		1<<2
#define MASK_BUTTONS_ALL (MASK_BUTTONS_U|MASK_BUTTONS_D|MASK_BUTTONS_M)
#define LCD_CS				1<<3
#define LCD_RESET			1<<4


#define UIP_ETHADDR0  0x0
#define UIP_ETHADDR1 0x60
#define UIP_ETHADDR2  0x37
#define	UIP_ETHADDR3  0x12
#define UIP_ETHADDR4  0x34
#define	UIP_ETHADDR5  0x56

// Station flash position
#define FLASH_ADDR 			0x78000

//Stations
#define NUM_STATION_MEM 	4
#define TEA_FREQ 			100

typedef struct _sation{
	unsigned int frequency;
	unsigned int valid;
} Station, *PStation;

#endif


