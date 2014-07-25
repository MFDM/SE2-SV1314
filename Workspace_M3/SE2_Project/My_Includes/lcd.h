/*
 * @brief Common LCD functions
 */
#ifndef lcd
#define lcd

/** @defgroup LCD : Common LCD functions
 * @ingroup LCD
 * This file contains common LCD functions.
 * @{
 */
/**
 * @brief	Make initialization for LCD
 * @param	Nothing
 * @return	Nothing
 * @note	This initialization use SPI communication
 */
void LCD_Init(void);

/**
 * @brief	Write single char on LCD
 * @param	data to be written
 * @param	x initial coordenate
 * @param	y initial coordenate
 * @return	Nothing
 * @note	This operation use SPI communication
 */
void LCD_WriteChar(char data,int x, int y);

/**
 * @brief	Write a string on LCD
 * @param	data to be written
 * @param	x initial coordenate
 * @param	y initial coordenate
 * @return	Nothing
 * @note	This operation calls n times WriteChar till value == '\0'
 */
void LCD_WriteString(char * data,int x, int y);
/**
 * @brief	Clean lcd's display
 * @param	color to be on foreground
 * @return	Nothing
 */
void LCD_CleanDisplay(char color);
/**
 * @}
 */
#endif
