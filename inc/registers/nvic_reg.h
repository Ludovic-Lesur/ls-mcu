/*
 * nvic_reg.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludo
 */

#ifndef __NVIC_REG_H__
#define __NVIC_REG_H__

#include "types.h"

/*** NVIC registers ***/

typedef struct {
	volatile uint32_t ISER[8];		// Interrupt set-enable registers 0 to 7.
	uint32_t RESERVED0[24];			// Reserved 0xE000E120.
	volatile uint32_t ICER[8];		// Interrupt clear-enable registers 0 to 7.
	uint32_t RESERVED1[24];			// Reserved 0xE000E1A0.
	volatile uint32_t ISPR[8];		// Interrupt set-pending registers 0 to 7.
	uint32_t RESERVED2[24];			// Reserved 0xE000E220.
	volatile uint32_t ICPR[8];    	// Interrupt clear-pending registers 0 to 7.
	uint32_t RESERVED3[24];			// Reserved 0xE000E300.
	volatile uint32_t IABR[8];		// Interrupt active bit registers 0 to 7.
	uint32_t RESERVED4[56];			// Reserved 0xE000E320.
	volatile uint32_t IPR[60];		// Interrupt priority registers 0 to 59.
	uint32_t RESERVED5[644];		// Reserved 0xE000E4F0.
	volatile uint32_t STIR;    		// Interrupt software trigger register.
} NVIC_registers_t;

/*** NVIC base address ***/

#define NVIC	((NVIC_registers_t*) ((uint32_t) 0xE000E100))

#endif /* __NVIC_REG_H__ */
