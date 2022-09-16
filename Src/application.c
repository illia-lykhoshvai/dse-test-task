/*
 * application.c
 *
 *  Created on: 16 сент. 2022 г.
 *      Author: illia-lykhoshvai
 */

#include "application.h"

PT_THREAD(oneWireTemp(struct pt* pt, volatile i16* returnTemperature)) {
	static struct pt tempPt;
	static i16 temperature;
	static ui8 dsStatus, errorsCounter;
	PT_BEGIN(pt);

	PT_INIT(pt);
	PT_YIELD_UNTIL(pt, (readDS(&tempPt, &dsStatus, &temperature) == PT_ENDED));

	if (dsStatus == correctValue) { // good data
		if (errorsCounter > 0) {
			errorsCounter--;
		} else {
			*returnTemperature = temperature;
		}
		PT_YIELD_UNTIL(pt, (startDS(&tempPt) == PT_ENDED));
	} else if (dsStatus == defaultValue) { // after reset
		PT_YIELD_UNTIL(pt, (startDS(&tempPt) == PT_ENDED));
		goto errorProcessing;
	} else { // error
		errorProcessing:
		if (errorsCounter >= 2) {
			*returnTemperature = ERROR_TEMP;
		} else {
			errorsCounter++;
		}
	}

	PT_END(pt);
}
