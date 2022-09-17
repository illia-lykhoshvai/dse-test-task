/*
 * misc.c
 *
 *  Created on: Sep 13, 2022
 *      Author: illia-lykhoshvai
 */

#include "stm32f0xx.h"

#include "misc.h"
#include "segm.h"

void initRCC(void);
void initGPIO(void);
void initTIM(void);

void initHW(void) {
	initRCC();
	initTIM();
	initGPIO();
}

void initRCC(void) {
	// turning pll off for safety purposes.
	RCC->CR &= ~RCC_CR_PLLON;
	while(RCC->CR & RCC_CR_PLLRDY);
	// configuring pll
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2 + RCC_CFGR_PLLMUL12;
	// turning pll on
	RCC->CR |= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY) == 0);
	// selecting pll as system clock
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
	SystemCoreClockUpdate();
}

void initTIM(void) {
	// input on tim17 is 48mhz
	// purpose: generate int. every 1 ms
	RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
	TIM17->PSC = TIM_PSC;
	TIM17->ARR = TIM_ARR;
	TIM17->CR1 = (1 << 7) ; // only arr enable
	TIM17->DIER = 1; // enable int on update (overflow)
	NVIC_EnableIRQ(TIM17_IRQn);
	TIM17->CR1 |= 1; // turn on counter
}

void initGPIO(void) {
	// PB[0-7] = indicator segments, common cathode
	// PC[0-3] = indicator digits, common cathode
	// PA0 = onewire
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN + RCC_AHBENR_GPIOBEN + RCC_AHBENR_GPIOCEN;
	// open-drain
	GPIOA->OTYPER |= GPIO_OTYPER_OT_0;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR0;
	// output push-pull
	GPIOB->MODER |= (GPIO_MODER_MODER0_0 + GPIO_MODER_MODER1_0 + GPIO_MODER_MODER2_0 + GPIO_MODER_MODER3_0
			+ GPIO_MODER_MODER4_0 + GPIO_MODER_MODER5_0 + GPIO_MODER_MODER6_0 + GPIO_MODER_MODER7_0);
	GPIOB->BRR = segmAll;
	// output open-drain
	GPIOC->MODER |= (GPIO_MODER_MODER0_0 + GPIO_MODER_MODER1_0 + GPIO_MODER_MODER2_0 + GPIO_MODER_MODER3_0);
	GPIOC->OTYPER |= (GPIO_OTYPER_OT_0 + GPIO_OTYPER_OT_1 + GPIO_OTYPER_OT_2 + GPIO_OTYPER_OT_3);
}


void timClearIFlag(void) {
	TIM17->SR = 0;
}
