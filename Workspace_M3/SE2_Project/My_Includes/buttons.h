/*
 * @brief Common Buttons functions
 */
#ifndef buttons
#define buttons

/** @defgroup Buttons : Common Buttons functions
 * @ingroup Buttons
 * This file contains common buttons functions.
 * @{
 */

/**
 * @brief	Read from GPIO ports for a single mask
 * @param	Mask of pin map from buttons
 * @return	State of buttons, 1 for clicked, 0 for not clicked
 * @note	This use GPIO ports for reading
 * LPC175x/6x devices.
 */
unsigned int BUTTONS_Read(unsigned int mask);

/**
 * @brief	Initialize GIPO port for Button function
 * @param	pin map of buttons
 * @return	Nothing
 * @note	This use GPIO ports for initialization
 */
void BUTTONS_Init(unsigned int pinMap);

/**
 * @}
 */
#endif
