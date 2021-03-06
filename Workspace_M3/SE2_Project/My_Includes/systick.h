/*
 * @brief SYSTICK struct
 */
#ifndef systick
#define systick

#include "LPC1769_Types.h"

/**
 * @brief SYSTICK registers struct
 */
typedef struct {

LPC1769_Reg STCTRL;
LPC1769_Reg STRELOAD;
LPC1769_Reg STCURR;
LPC1769_Reg STCALIB;

}LPC1769_SYSTICK;

#endif
