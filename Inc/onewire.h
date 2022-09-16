/*
 * onewire.h
 *
 *  Created on: Sep 13, 2022
 *      Author: illia-lykhoshvai
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include "main.h"
#include "pt/pt.h"

#include "stm32f0xx.h"


//#define PARASITE_POWER

#define REGISTER_SIZE ui32

// user-defined functions:
// set w1_pin in output mode
#define W1_SET_OUT do { GPIOA->MODER &= ~GPIO_MODER_MODER0; GPIOA->MODER |= GPIO_MODER_MODER0_0; } while(0)
// set w1 pin in input mode
#define W1_SET_IN do { GPIOA->MODER &= ~GPIO_MODER_MODER0; } while(0)
// w1 pin input register
#define W1_IN GPIOA->IDR
// w1 pin output register
#define W1_OUT GPIOA->ODR
// w1 pin input bit mask
#define W1_IN_BMASK (1 << 0)
// w1 pin output bit mask
#define W1_OUT_BMASK W1_IN_BMASK

#define SKIP_ROM 0xCC
#define START_DS 0x44
#define READ_DS 0xBE

enum presenceEnum {
	no,
	present
};

typedef enum w1ReturnCodeEnum {
	error,
	defaultValue,
	correctValue
} w1ReturnCode_t;

// protothread to start DS18 temperature convertion
PT_THREAD(startDS(struct pt* pt));

// protothread to read DS18 memory and check data integrity and after that - return temperature
PT_THREAD(readDS(struct pt* pt, w1ReturnCode_t* returnCode, i16* temperature));

#endif /* ONEWIRE_H_ */
