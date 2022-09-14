#include "main.h"

#include "segm.h"

static volatile ui8 ledCounter = 0;
ui8 showBuffer[DIGITS];

void TIM17_IRQHandler(void) {
	timClearIFlag();
	drawDigit(showBuffer[ledCounter],ledCounter);
	ledCounter = (++ledCounter) % DIGITS;
}

int main(void) {
	for(ui8 i = 0;i < DIGITS;i++)
		showBuffer[i] = 8;
	initHW();

	while(1) {

	}
}
