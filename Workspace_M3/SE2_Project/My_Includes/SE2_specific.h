#ifndef SE2_specific
#define SE2_specific

#define MASK_BUTTONS_U 					1<<0
#define MASK_BUTTONS_D 					1<<1
#define MASK_BUTTONS_M 					1<<2
#define MASK_BUTTONS_LONG_PRESS 		1<<7
#define MASK_BUTTONS_ALL (MASK_BUTTONS_U|MASK_BUTTONS_D|MASK_BUTTONS_M)

#define LCD_CS							1<<3
#define LCD_RESET						1<<4


#define UIP_ETHADDR0  	0x0
#define UIP_ETHADDR1	0x60
#define UIP_ETHADDR2  	0x37
#define	UIP_ETHADDR3  	0x12
#define UIP_ETHADDR4  	0x34
#define	UIP_ETHADDR5  	0x56

// Station flash position
#define FLASH_ADDR 			0xE000
#define FLASH_INIT_SECTOR	14
#define FLASH_END_SECTOR	14

//Stations
#define STATIONS_MAX_MEM 	4
#define TEA_FREQ 			100


/**
 * @brief Stations struct to be used
 */
typedef struct _sations{
	unsigned int frequency[STATIONS_MAX_MEM];
	unsigned int valid_information;
}Stations, *PStations;

#endif


