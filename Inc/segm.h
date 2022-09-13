/*
 * 7segm.h
 *
 *  Created on: Sep 13, 2022
 *      Author: illia-lykhoshvai
 */

#ifndef SEGM_H_
#define SEGM_H_

#include "main.h"

#define segmA (1 << 0)
#define segmB (1 << 1)
#define segmC (1 << 2)
#define segmD (1 << 3)
#define segmE (1 << 4)
#define segmF (1 << 5)
#define segmG (1 << 6)
#define segmDP (1 << 7)
#define segmAll (segmA + segmB + segmC + segmD + segmE + segmF + segmG + segmDP)

#define digit1 (1 << 0)
#define digit2 (1 << 1)
#define digit3 (1 << 2)
#define digit4 (1 << 3)
#define digitAll (digit1 + digit2 + digit3 + digit4)

void drawDigit(ui8 digit, ui8 position);

#endif /* SEGM_H_ */
