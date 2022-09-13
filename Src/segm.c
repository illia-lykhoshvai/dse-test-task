/*
 * segm.c
 *
 *  Created on: Sep 13, 2022
 *      Author: illia-lykhoshvai
 */

#include "segm.h"
#include "stm32f0xx.h"

ui8 segmentToPort[] = {
		(segmA + segmB + segmC + segmD + segmE + segmF),			// 0
		(segmB + segmC),											// 1
		(segmA + segmB + segmD + segmE + segmG),					// 2
		(segmA + segmB + segmC + segmD + segmG),					// 3
		(segmB + segmC + segmF + segmG),							// 4
		(segmA + segmC + segmD + segmF + segmG),					// 5
		(segmA + segmC + segmD + segmE + segmF + segmG),			// 6
		(segmA + segmB + segmC),				 					// 7
		(segmA + segmB + segmC + segmD + segmE + segmF + segmG),	// 8
		(segmA + segmB + segmC + segmD + segmF + segmG)				// 9
};

void drawDigit(ui8 digit, ui8 position) {
	// turn off segments and digit
	GPIOB->BRR = segmAll;
	GPIOC->BSRR = digitAll;


	if(digit & 0x80) {
		GPIOB->BSRR = segmDP;
		digit &= ~0x80;
	}
	if(digit < sizeof(segmentToPort)) {
		GPIOB->BSRR = segmentToPort[digit];
	}

	GPIOC->BRR = (1 << position);
}
