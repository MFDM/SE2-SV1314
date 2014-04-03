#include "gpio.h"
#include "lpc.h"
#include "sysclk.h"
#include "led.h"

#define waitTimeMillis 1000
#define VICVectAddr0 0xFFFFF100
#define VICVectCntl0 0xFFFFF200


LPC2106_TC* timer0 = LPC2106_BASE_TC0;
LPC2106_VIC* vic = LPC2106_BASE_VIC;
LPC2106_REG* VectAddr0 = (unsigned int*)VICVectAddr0;
LPC2106_REG* VectCntl0 = (unsigned int*)VICVectCntl0;
int state = 0;


void blink_Led(int state){
	LED_SetState(state);
}

void timer0ISR(void) {
	timer0->IR = 0x01; // clear interrupt
	vic->VectAddr = 0; // end of interrupt - dummy write
	if(state == 1) state = 0;
	else state = 1;
	blink_Led(state);
}

void timer0ISRH(void);
void setEnableIntr(void);

int main(){
	volatile int dummy= 0;
	timer0 -> MCR = 0x3;
	timer0 -> MRO = waitTimeMillis;
	SYSCLK_Init(waitTimeMillis);
	LED_Init(state);
	*VectAddr0 = (unsigned int) timer0ISRH;
	*VectCntl0 = (1<<5)|0x4;
	vic -> IntEnable = 1<<4;
	vic -> IntSelect &= ~(1<<4);
	int e = &(vic->VectAddr);
	setEnableIntr();
	while(1){
		++dummy;
	}
	return 1;
}
