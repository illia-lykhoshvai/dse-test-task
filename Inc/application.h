/*
 * application.h
 *
 *  Created on: 16 сент. 2022 г.
 *      Author: illia-lykhoshvai
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "main.h"
#include "pt/pt.h"

#include "onewire.h"

#define ERROR_TEMP 0xFFFF

PT_THREAD(oneWireTemp(struct pt* pt, volatile i16* returnTemperature));

#endif /* APPLICATION_H_ */
