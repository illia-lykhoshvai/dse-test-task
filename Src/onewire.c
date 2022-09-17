/*
 * onewire.c
 *
 *  Created on: Sep 13, 2022
 *      Author: illia-lykhoshvai
 */

#include "onewire.h"

ui8 dsCRC(ui8 *addr, ui8 len) {
	uint8_t crc = 0;
	while (len--) {
		uint8_t inbyte = *addr++;
		for (uint8_t i = 8; i; i--) {
		  uint8_t mix = (crc ^ inbyte) & 0x01;
		  crc >>= 1;
		  if (mix) crc ^= 0x8C;
		  inbyte >>= 1;
		}
	}
	return crc;
}

void delayUs(ui32 time) {
	for(time *= 4;time > 0; time--) {
		asm("nop");
	}
}

void checkPresence(ui8* presence) {
	__disable_irq();
	W1_OUT |= W1_OUT_BMASK;
	W1_SET_OUT;
	delayUs(60);
	W1_OUT &= ~W1_OUT_BMASK;
	delayUs(490); // 480 us
	W1_SET_IN;
	delayUs(60); // 60 us
	*presence = (W1_IN & (W1_IN_BMASK)) ? no : present; // no presence detect
	delayUs(490); // 480 us
	if ((W1_IN & (W1_IN_BMASK)) == 0) { // short circuit or lagg on bus
		*presence = no;
	}
	__enable_irq();
}

void writeByte(ui8 byte) {
	ui8 temp = byte, i;
	__disable_irq();
	for (i = 0; i < 8; i++) {
		W1_OUT &= ~W1_OUT_BMASK;
		W1_SET_OUT;
		if(temp & 1) {
			delayUs(2);
		} else {
			delayUs(60);
		}
		W1_OUT |= W1_OUT_BMASK;
		delayUs(60);
//		W1_SET_IN;
		temp >>= 1;
	  }
	__enable_irq();
}

ui8 readByte(void) {
	ui8 byte = 0, i;
	__disable_irq();
	for (i = 0; i < 8; i++) {
		byte >>= 1;
		W1_OUT &= ~W1_OUT_BMASK;
		W1_SET_OUT;
		delayUs(1);
		W1_SET_IN;
		delayUs(15);
		if ((W1_IN & W1_IN_BMASK) != 0) {
			byte |= 0x80;
		}
		W1_OUT |= W1_OUT_BMASK;
		W1_SET_OUT;
		delayUs(45);
	}
	__enable_irq();
	return byte;
}

PT_THREAD(startDS(struct pt* pt)) {
	ui8 dsPresent;
	PT_BEGIN(pt);
	checkPresence(&dsPresent);
	if (dsPresent) {
		PT_YIELD(pt); writeByte(SKIP_ROM);
		PT_YIELD(pt); writeByte(START_DS);
#ifdef PARASITE_POWER
		W1_SET_OUT;
		W1_OUT |= W1_OUT_BMASK;
#endif
	}
	PT_END(pt);
}

static ui8 packet[9];

PT_THREAD(readDS(struct pt* pt, ui8* returnCode, i16* temperature)) {
	ui8 dsPresent;
	i16 i;
	PT_BEGIN(pt);
	checkPresence(&dsPresent);
	if (dsPresent) {
		PT_YIELD(pt); writeByte(SKIP_ROM);
		PT_YIELD(pt); writeByte(READ_DS);
		for(i = 0; i < sizeof(packet); i++) {
			PT_YIELD(pt); packet[i] = readByte();
		}
		i =  packet[0] | (packet[1] << 8);
		if (i == (85*16)) {
			*returnCode = defaultValue;
		} else {
			i = dsCRC(packet, sizeof(packet)-1);
			if (i == packet[8]) { // crc match
				i =  packet[0] | (packet[1] << 8);
				if (i & (1 << 11)) {
					i = ~i;
//					i = -((i >> 4) * 10 + (i & 0x000F) * 10 / 16);
					i = -((i >> 4) * 100 + (i & 0x000F) * 6);
				} else {
//					i = (i >> 4) * 10 + (i & 0x000F) * 10 / 16;
					i = (i >> 4) * 100 + (i & 0x000F) * 6;
				}
				*returnCode = correctValue;
				*temperature = i;
			} else {
				*returnCode = error;
			}
		}
	}
	PT_END(pt);
}
