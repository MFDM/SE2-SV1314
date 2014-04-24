#ifndef systick
#define systick
#include cortex.h
typedef struct {
	CORTEX_Reg STCTRL;
	CORTEX_Reg STRELOAD;
	CORTEX_Reg STCURR;
	CORTEX_Reg STCALIB;

}CORTEX_SYSTICK;
#endif
