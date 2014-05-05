#ifndef buttons
#define buttons
/**
 * Buttons Driver
 * Responsible for initialize and read the value of buttons. 
 */
unsigned int BUTTONS_Read(unsigned int mask);
void BUTTONS_Init(unsigned int pinMap);
#endif
