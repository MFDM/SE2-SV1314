#include "lcd.h"
#include "spi.h"
#include "gpio.h"
#include "SE2_specific.h"

#define DISPLAY_CONTROL				0xCA
#define COMMON_SCAN_DIRECTION		0xBB
#define POWER_CONTROL				0x20
#define INVERSE_DISPLAY				0xA7
#define DATA_SCAN_DIRECTION			0xBC
#define ELECTRONIC_VOLUME_CONTROL	0x81
#define DISPLAY_ON					0xAF
#define RAM_WRITE					0x5c
#define NOP							0x25
#define BLACK						0
#define WHITE						0xfff

unsigned int ram_writable = 0;

const unsigned char font6x8[97][8] = {
0x08,0x08,0x08,0x00,0x00,0x00,0x00,0x00, // columns, rows, num_bytes_per_char
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // space 0x20
0x20,0x20,0x20,0x20,0x20,0x00,0x20,0x00, // !
0x50,0x50,0x50,0x00,0x00,0x00,0x00,0x00, // "
0x50,0x50,0xF8,0x50,0xF8,0x50,0x50,0x00, // #
0x20,0x78,0xA0,0x70,0x28,0xF0,0x20,0x00, // $
0xC0,0xC8,0x10,0x20,0x40,0x98,0x18,0x00, // %
0x40,0xA0,0xA0,0x40,0xA8,0x90,0x68,0x00, // &
0x30,0x30,0x20,0x40,0x00,0x00,0x00,0x00, // '
0x10,0x20,0x40,0x40,0x40,0x20,0x10,0x00, // (
0x40,0x20,0x10,0x10,0x10,0x20,0x40,0x00, // )
0x00,0x20,0xA8,0x70,0x70,0xA8,0x20,0x00, // *
0x00,0x20,0x20,0xF8,0x20,0x20,0x00,0x00, // +
0x00,0x00,0x00,0x00,0x30,0x30,0x20,0x40, // ,
0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00, // -
0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00, // .
0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00, // / (forward slash)
0x70,0x88,0x88,0xA8,0x88,0x88,0x70,0x00, // 0 0x30
0x20,0x60,0x20,0x20,0x20,0x20,0x70,0x00, // 1
0x70,0x88,0x08,0x70,0x80,0x80,0xF8,0x00, // 2
0xF8,0x08,0x10,0x30,0x08,0x88,0x70,0x00, // 3
0x10,0x30,0x50,0x90,0xF8,0x10,0x10,0x00, // 4
0xF8,0x80,0xF0,0x08,0x08,0x88,0x70,0x00, // 5
0x38,0x40,0x80,0xF0,0x88,0x88,0x70,0x00, // 6
0xF8,0x08,0x08,0x10,0x20,0x40,0x80,0x00, // 7
0x70,0x88,0x88,0x70,0x88,0x88,0x70,0x00, // 8
0x70,0x88,0x88,0x78,0x08,0x10,0xE0,0x00, // 9
0x00,0x00,0x20,0x00,0x20,0x00,0x00,0x00, // :
0x00,0x00,0x20,0x00,0x20,0x20,0x40,0x00, // ;
0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x00, // <
0x00,0x00,0xF8,0x00,0xF8,0x00,0x00,0x00, // =
0x40,0x20,0x10,0x08,0x10,0x20,0x40,0x00, // >
0x70,0x88,0x08,0x30,0x20,0x00,0x20,0x00, // ?
0x70,0x88,0xA8,0xB8,0xB0,0x80,0x78,0x00, // @ 0x40
0x20,0x50,0x88,0x88,0xF8,0x88,0x88,0x00, // A
0xF0,0x88,0x88,0xF0,0x88,0x88,0xF0,0x00, // B
0x70,0x88,0x80,0x80,0x80,0x88,0x70,0x00, // C
0xF0,0x88,0x88,0x88,0x88,0x88,0xF0,0x00, // D
0xF8,0x80,0x80,0xF0,0x80,0x80,0xF8,0x00, // E
0xF8,0x80,0x80,0xF0,0x80,0x80,0x80,0x00, // F
0x78,0x88,0x80,0x80,0x98,0x88,0x78,0x00, // G
0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00, // H
0x70,0x20,0x20,0x20,0x20,0x20,0x70,0x00, // I
0x38,0x10,0x10,0x10,0x10,0x90,0x60,0x00, // J
0x88,0x90,0xA0,0xC0,0xA0,0x90,0x88,0x00, // K
0x80,0x80,0x80,0x80,0x80,0x80,0xF8,0x00, // L
0x88,0xD8,0xA8,0xA8,0xA8,0x88,0x88,0x00, // M
0x88,0x88,0xC8,0xA8,0x98,0x88,0x88,0x00, // N
0x70,0x88,0x88,0x88,0x88,0x88,0x70,0x00, // O
0xF0,0x88,0x88,0xF0,0x80,0x80,0x80,0x00, // P 0x50
0x70,0x88,0x88,0x88,0xA8,0x90,0x68,0x00, // Q
0xF0,0x88,0x88,0xF0,0xA0,0x90,0x88,0x00, // R
0x70,0x88,0x80,0x70,0x08,0x88,0x70,0x00, // S
0xF8,0xA8,0x20,0x20,0x20,0x20,0x20,0x00, // T
0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00, // U
0x88,0x88,0x88,0x88,0x88,0x50,0x20,0x00, // V
0x88,0x88,0x88,0xA8,0xA8,0xA8,0x50,0x00, // W
0x88,0x88,0x50,0x20,0x50,0x88,0x88,0x00, // X
0x88,0x88,0x50,0x20,0x20,0x20,0x20,0x00, // Y
0xF8,0x08,0x10,0x70,0x40,0x80,0xF8,0x00, // Z
0x78,0x40,0x40,0x40,0x40,0x40,0x78,0x00, // [
0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00, // \ (back slash)
0x78,0x08,0x08,0x08,0x08,0x08,0x78,0x00, // ]
0x20,0x50,0x88,0x00,0x00,0x00,0x00,0x00, // ^
0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00, // _
0x60,0x60,0x20,0x10,0x00,0x00,0x00,0x00, // ` 0x60
0x00,0x00,0x60,0x10,0x70,0x90,0x78,0x00, // a
0x80,0x80,0xB0,0xC8,0x88,0xC8,0xB0,0x00, // b
0x00,0x00,0x70,0x88,0x80,0x88,0x70,0x00, // c
0x08,0x08,0x68,0x98,0x88,0x98,0x68,0x00, // d
0x00,0x00,0x70,0x88,0xF8,0x80,0x70,0x00, // e
0x10,0x28,0x20,0x70,0x20,0x20,0x20,0x00, // f
0x00,0x00,0x70,0x98,0x98,0x68,0x08,0x70, // g
0x80,0x80,0xB0,0xC8,0x88,0x88,0x88,0x00, // h
0x20,0x00,0x60,0x20,0x20,0x20,0x70,0x00, // i
0x10,0x00,0x10,0x10,0x10,0x90,0x60,0x00, // j
0x80,0x80,0x90,0xA0,0xC0,0xA0,0x90,0x00, // k
0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00, // l
0x00,0x00,0xD0,0xA8,0xA8,0xA8,0xA8,0x00, // m
0x00,0x00,0xB0,0xC8,0x88,0x88,0x88,0x00, // n
0x00,0x00,0x70,0x88,0x88,0x88,0x70,0x00, // o
0x00,0x00,0xB0,0xC8,0xC8,0xB0,0x80,0x80, // p 0x70
0x00,0x00,0x68,0x98,0x98,0x68,0x08,0x08, // q
0x00,0x00,0xB0,0xC8,0x80,0x80,0x80,0x00, // r
0x00,0x00,0x78,0x80,0x70,0x08,0xF0,0x00, // s
0x20,0x20,0xF8,0x20,0x20,0x28,0x10,0x00, // t
0x00,0x00,0x88,0x88,0x88,0x98,0x68,0x00, // u
0x00,0x00,0x88,0x88,0x88,0x50,0x20,0x00, // v
0x00,0x00,0x88,0x88,0xA8,0xA8,0x50,0x00, // w
0x00,0x00,0x88,0x50,0x20,0x50,0x88,0x00, // x
0x00,0x00,0x88,0x88,0x78,0x08,0x88,0x70, // y
0x00,0x00,0xF8,0x10,0x20,0x40,0xF8,0x00, // z
0x10,0x20,0x20,0x40,0x20,0x20,0x10,0x00, // {
0x20,0x20,0x20,0x00,0x20,0x20,0x20,0x00, // |
0x40,0x20,0x20,0x10,0x20,0x20,0x40,0x00, // }
0x40,0xA8,0x10,0x00,0x00,0x00,0x00,0x00, // ~
0x70,0xD8,0xD8,0x70,0x00,0x00,0x00,0x00
}; // DEL


int LCD_Init() {
	SPI_Init();
	GPIO_Init(LCD_CS);
	GPIO_SetDir(LCD_CS, LCD_CS);
	GPIO_Write(LCD_CS,LCD_CS);
	// Hardware reset
//	LCD_RESET_LOW;
//	Delay(20000);
//	LCD_RESET_HIGH;
//	Delay(20000);

	// Display control
	//WriteSpiCommand(DISCTL);
	SPI_Transfer(DISPLAY_CONTROL, 0);
	//WriteSpiData(0x00); // P1: 0x00 = 2 divisions, switching period=8 (default)
	SPI_Transfer(0x00, 1);
	//WriteSpiData(0x20); // P2: 0x20 = nlines/4 - 1 = 132/4 - 1 = 32)
	SPI_Transfer(0x20, 1);
	//WriteSpiData(0x00); // P3: 0x00 = no inversely highlighted lines
	SPI_Transfer(0x00, 1);
	// COM scan
	//WriteSpiCommand(COMSCN);
	SPI_Transfer(COMMON_SCAN_DIRECTION, 0);
	//WriteSpiData(0x01);  // P1: 0x01 = Scan 1->80, 160<-81
	SPI_Transfer(0x01, 1);
	// Power control
	//WriteSpiCommand(PWRCTR);
	SPI_Transfer(POWER_CONTROL, 0);
	//WriteSpiData(0x0f); // reference voltage regulator on, circuit voltage follower on, BOOST ON
	SPI_Transfer(0x0f, 1);
	// Inverse display
	//WriteSpiCommand(DISINV);
	SPI_Transfer(INVERSE_DISPLAY, 0);
	// Data control
	//WriteSpiCommand(DATCTL);
	SPI_Transfer(DATA_SCAN_DIRECTION, 0);
	//WriteSpiData(0x01); // P1: 0x01 = page address inverted, col address normal, address scan in col direction
	SPI_Transfer(0x01, 1);
	//WriteSpiData(0x00); // P2: 0x00 = RGB sequence (default value)
	SPI_Transfer(0x00, 1);
	//WriteSpiData(0x02); // P3: 0x02 = Grayscale -> 16 (selects 12-bit color, type A)
	SPI_Transfer(0x02, 1);
	// Voltage control (contrast setting)
	//WriteSpiCommand(VOLCTR);
	SPI_Transfer(ELECTRONIC_VOLUME_CONTROL, 0);
	//WriteSpiData(32); // P1 = 32  volume value  (adjust this setting for your display  0 .. 63)
	SPI_Transfer(32, 1);
	//WriteSpiData(3);  // P2 = 3    resistance ratio  (determined by experiment)
	SPI_Transfer(3, 1);
	// allow power supply to stabilize
	//Delay(100000);

	// turn on the display
	//WriteSpiCommand(DISON);
	SPI_Transfer(DISPLAY_ON, 0);
	return 0;
}

int LCD_WriteChar(char data) {
	int i, j;
	unsigned int cols;
	unsigned int rows;
	unsigned int bytes;
	unsigned char pixelRow;
	unsigned int Word0;
	unsigned int Word1;
	unsigned char *pFont;
	unsigned char *pChar;

	pFont = (unsigned char *) font6x8;
	cols = *pFont;
	rows = *(pFont + 1);
	bytes = *(pFont + 2);
	pChar = pFont + (bytes * (data - 0x1F)) + bytes - 1;

	if (!ram_writable)
		SPI_Transfer(RAM_WRITE, 0);

	for (i = 0; i < bytes; i++) {
		pixelRow = *pChar--;
		for (j = 0; j < cols / 2; j++) {
			Word0 = ((pixelRow & 0x1) != 0) ? WHITE : BLACK;
			pixelRow >>= 1;
			Word1 = ((pixelRow & 0x1) != 0) ? WHITE : BLACK;
			pixelRow >>= 1;
//			LCDData((Word0 >> 4) & 0xFF);
			SPI_Transfer((Word0 >> 4) & 0xFF,1);
//			LCDData(((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF));
			SPI_Transfer(((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF),1);
//			LCDData(Word1 & 0xFF);
			SPI_Transfer(Word1 & 0xFF,1);
		}
	}
	SPI_Transfer(NOP, 0);
	return 0;
}

int LCD_WriteString(char * data) {
	SPI_Transfer(RAM_WRITE,0);
	ram_writable =1;
	while (*data != '\0') {
		LCD_WriteChar(*data);
		data++;
	}
	ram_writable=0;
	return 0;
}

