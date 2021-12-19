/*
 * sw2.h
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludo
 */

#ifndef SW2_H
#define SW2_H

#include "gpio.h"

/*** SW2 structures ***/

// Output state.
typedef enum {
	SW2_OFF = 0,
	SW2_ON
} SW2_State;

// Internal state machine.
typedef enum {
	SW2_STATE_OFF = 10,
	SW2_STATE_CONFIRM_ON,
	SW2_STATE_ON,
	SW2_STATE_CONFIRM_OFF
} SW2_InternalState;

// SW2 structure.
typedef struct {
	const GPIO* gpio;
	unsigned char active_state; // Depends on switch wiring.
	SW2_InternalState internal_state; // Current state in SW2 state machine.
	SW2_State state; // State after anti-bouncing (used in higher levels).
	unsigned int debouncing_ms; // Delay before validating ON/OFF state (in ms).
	unsigned int confirm_start_time;
} SW2_Context;

/*** SW2 functions ***/

void SW2_Init(SW2_Context* sw2, const GPIO* gpio, unsigned char active_state, unsigned int debouncing_ms);
void SW2_UpdateState(SW2_Context* sw2);

#endif /* SW2_H */
