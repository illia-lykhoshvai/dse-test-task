/*
 * function.h
 *
 *  Created on: Sep 13, 2022
 *      Author: illia-lykhoshvai
 */

#ifndef MISC_H_
#define MISC_H_

#define DATA_FREQ 1000
#define TIM_PSC ((SystemCoreClock/1000000) - 1)
#define TIM_ARR (((SystemCoreClock/(TIM_PSC+1))/DATA_FREQ) - 1)

void initHW(void);

void timClearIFlag(void);

#endif /* MISC_H_ */
