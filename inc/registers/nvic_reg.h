/*
 * nvic_reg.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludo
 */

#ifndef __NVIC_REG_H__
#define __NVIC_REG_H__

/*** NVIC registers ***/

typedef struct {
	volatile unsigned int ISER[8];		// Interrupt set-enable registers 0 to 7.
	unsigned int RESERVED0[24];			// Reserved 0xE000E120.
	volatile unsigned int ICER[8];		// Interrupt clear-enable registers 0 to 7.
	unsigned int RESERVED1[24];			// Reserved 0xE000E1A0.
	volatile unsigned int ISPR[8];		// Interrupt set-pending registers 0 to 7.
	unsigned int RESERVED2[24];			// Reserved 0xE000E220.
	volatile unsigned int ICPR[8];    	// Interrupt clear-pending registers 0 to 7.
	unsigned int RESERVED3[24];			// Reserved 0xE000E300.
	volatile unsigned int IABR[8];		// Interrupt active bit registers 0 to 7.
	unsigned int RESERVED4[56];			// Reserved 0xE000E320.
	volatile unsigned int IPR[60];		// Interrupt priority registers 0 to 59.
	unsigned int RESERVED5[644];		// Reserved 0xE000E4F0.
	volatile unsigned int STIR;    		// Interrupt software trigger register.
} NVIC_base_address_t;

/*** NVIC base address ***/

#define NVIC	((NVIC_base_address_t*) ((unsigned int) 0xE000E100))

#endif /* __NVIC_REG_H__ */
