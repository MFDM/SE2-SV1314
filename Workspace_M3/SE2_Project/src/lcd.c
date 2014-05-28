#include "lcd.h"
#include "spi.h"

#define DISPLAY_CONTROL				0xCA
#define COMMON_SCAN_DIRECTION		0xBB
#define POWER_CONTROL				0x20
#define INVERSE_DISPLAY				0xA7
#define DATA_SCAN_DIRECTION			0xBC
#define ELECTRONIC_VOLUME_CONTROL	0x81
#define DISPLAY_ON					0xAF


int LCD_Init() {
	SPI_Init();
	// Hardware reset
	LCD_RESET_LOW;
	Delay(20000);
	LCD_RESET_HIGH;
	Delay(20000);

	// Display control
	//WriteSpiCommand(DISCTL);
	SPI_Tranfer(DISPLAY_CONTROL, 0);
	//WriteSpiData(0x00); // P1: 0x00 = 2 divisions, switching period=8 (default)
	SPI_Transfer(0x00,1);
	//WriteSpiData(0x20); // P2: 0x20 = nlines/4 - 1 = 132/4 - 1 = 32)
	SPI_Transfer(0x20,1);
	//WriteSpiData(0x00); // P3: 0x00 = no inversely highlighted lines
	SPI_Transfer(0x00,1);
	// COM scan
	//WriteSpiCommand(COMSCN);
	SPI_Transfer(COMMON_SCAN_DIRECTION,0);
	//WriteSpiData(0x01);  // P1: 0x01 = Scan 1->80, 160<-81
	SPI_Transfer(0x01,1);
	// Power control
	//WriteSpiCommand(PWRCTR);
	SPI_Transfer(POWER_CONTROL,0);
	//WriteSpiData(0x0f); // reference voltage regulator on, circuit voltage follower on, BOOST ON
	SPI_Transfer(0x0f,1);
	// Inverse display
	//WriteSpiCommand(DISINV);
	SPI_Transfer(INVERSE_DISPLAY,0);
	// Data control
	//WriteSpiCommand(DATCTL);
	SPI_Transfer(DATA_SCAN_DIRECTION,0);
	//WriteSpiData(0x01); // P1: 0x01 = page address inverted, col address normal, address scan in col direction
	SPI_Transfer(0x01,1);
	//WriteSpiData(0x00); // P2: 0x00 = RGB sequence (default value)
	SPI_Transfer(0x00,1);
	//WriteSpiData(0x02); // P3: 0x02 = Grayscale -> 16 (selects 12-bit color, type A)
	SPI_Transfer(0x02,1);
	// Voltage control (contrast setting)
	//WriteSpiCommand(VOLCTR);
	SPI_Transfer(ELECTRONIC_VOLUME_CONTROL,0);
	//WriteSpiData(32); // P1 = 32  volume value  (adjust this setting for your display  0 .. 63)
	SPI_Transfer(32,1);
	//WriteSpiData(3);  // P2 = 3    resistance ratio  (determined by experiment)
	SPI_Transfer(3,1);
	// allow power supply to stabilize
	Delay(100000);

	// turn on the display
	//WriteSpiCommand(DISON);
	SPI_Transfer(DISPLAY_ON,0);
}
