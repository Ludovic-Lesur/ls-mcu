/*
 * tim.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_TIM_H
#define PERIPHERALS_TIM_H

/*** TIM functions ***/

// Milliseconds count.
void TIM2_Init(void);
unsigned int TIM2_GetMs(void);
void TIM2_DelayMs(unsigned ms_to_wait);

// KVB.
void TIM6_Init(void);
void TIM6_Start(void);
void TIM6_Stop(void);

// LVAL PWM.
void TIM8_Init(void);
void TIM8_SetDutyCycle(unsigned char duty_cycle);
void TIM8_Start(void);
void TIM8_Stop(void);

#endif /* PERIPHERALS_TIM_H */
