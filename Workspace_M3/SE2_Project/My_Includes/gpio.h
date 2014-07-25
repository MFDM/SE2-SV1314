/*
 * @brief Common GPIO functions
 */
#ifndef gpio
#define gpio

#include "LPC1769_Types.h"

/**
 * @brief GPIO registers struct
 */
typedef struct {

	LPC1769_Reg FIODIR;
	LPC1769_Reg dummy[3];
	LPC1769_Reg FIOMASK;
	LPC1769_Reg FIOPIN;
	LPC1769_Reg FIOSET;
	LPC1769_Reg FIOCLR;

}LPC1769_GPIO;

/** @defgroup GPIO : Common GPIO functions
 * @ingroup GPIO
 * This file contains common GPIO functions.
 * @{
 */

/**
 * @brief	Read from GPIO ports
 * @param	Nothing
 * @return	Value in GPIO ports
 */
unsigned int GPIO_Read(void);

/**
 * @brief	Enable GPIO ports
 * @param	pinMap to enable GPIO ports
 * @return	Nothing
 */
void GPIO_Init(unsigned int pinMap);

/**
 * @brief	Initialize directions for GPIO ports, input or output
 * @param	GPIO ports to select
 * @param	directions for selected GPIO ports
 * @return	Nothing
 */
void GPIO_SetDir(unsigned int pinMap, unsigned int pinDirectionMap);

/**
 * @brief	Write on GPIO ports
 * @param	value to be written on selected ports
 * @param 	ports to be selected for write operation
 * @return	Nothing
 */
void GPIO_Write(unsigned int preparedValue, unsigned int pinMap);

/**
 * @}
 */
#endif
