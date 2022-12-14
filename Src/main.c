#include "main.h"

#include "segm.h"
#include "application.h"

static volatile i16 temperature;
static volatile ui16 hzCounter;
static volatile ui8 ledCounter, mainTaskSemaphore, oneWireSemaphore = 1;
static struct pt oneWirePT;
ui8 showBuffer[DIGITS] = { 8, 8, 8, 8 };

void TIM17_IRQHandler(void) { // 1ms int.
#define HZ_VALUE 1000
	timClearIFlag();
/// LED section
	drawDigit(showBuffer[ledCounter],ledCounter);
	ledCounter++;
	ledCounter %= DIGITS;
/// 1-Wire section
	if(oneWireSemaphore) {
		if (oneWireTemp(&oneWirePT, &temperature) == PT_ENDED) {
			oneWireSemaphore = 0;
		}
	}
/// 1HZ action section
	if(++hzCounter >= HZ_VALUE) {
		hzCounter = 0;
		mainTaskSemaphore = 1;
	}
}

int main(void) {
	PT_INIT(&oneWirePT);
	initHW();

	while(1) {
		if (mainTaskSemaphore) {
			mainTaskSemaphore = 0;

			if (temperature < ERROR_TEMP) {
				if ((temperature >= 0) && (temperature < 10000)) { // (T >= 0.0) && (T < 100.0)
					showBuffer[0] = temperature / 1000;
					showBuffer[1] = (temperature / 100) % 10 | POINT;
					showBuffer[2] = (temperature / 10) % 10;
					showBuffer[3] = temperature % 10;
				} else if ((temperature >= 10000)) { // T >= 100.0
					showBuffer[0] = temperature / 10000;
					showBuffer[1] = (temperature / 1000) % 10;
					showBuffer[2] = (temperature / 100) % 10 | POINT;
					showBuffer[3] = (temperature / 10) % 10;
				} else { // T < 0.0
					showBuffer[0] = HYPHEN_SYMBOL;
					if (temperature <= (-1000)) { // T <= -10.0
						showBuffer[1] = -(temperature / 1000);
						showBuffer[2] = (-((temperature / 100) % 10)) | POINT;
						showBuffer[3] = -((temperature / 10) % 10);
					} else {  // T > -10.0
						showBuffer[1] = (-(temperature / 100)) | POINT;
						showBuffer[2] = -((temperature / 10) % 10);
						showBuffer[3] = -(temperature % 10);
					}
				}
			} else {
				memset(showBuffer,HYPHEN_SYMBOL,DIGITS);
			}

			oneWireSemaphore = 1;
		}
	}
}
