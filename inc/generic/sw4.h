/*
 * sw4.h
 *
 *  Created on: 26 dec. 2017
 *      Author: Ludovic
 */

#ifndef GENERIC_SW4_H
#define GENERIC_SW4_H

/*** 4-poles switch structures ***/

// Output state.
typedef enum {
	SW4_P0 = 0,
	SW4_P1,
	SW4_P2,
	SW4_P3
} SW4_State;

// Internal state machine.
typedef enum {
	SW4_STATE_P0 = 10,
	SW4_STATE_CONFIRM_P0,
	SW4_STATE_P1,
	SW4_STATE_CONFIRM_P1,
	SW4_STATE_P2,
	SW4_STATE_CONFIRM_P2,
	SW4_STATE_P3,
	SW4_STATE_CONFIRM_P3
} SW4_InternalState;

typedef struct {
	unsigned int voltage; // Current voltage measured by ADC.
	SW4_InternalState internal_state; // Current state in SW4 state machine.
	SW4_State state; // State after anti-bouncing (used in higher levels).
	unsigned int debouncing_ms; // Delay before validating states (in ms).
	unsigned int confirm_start_time;
} SW4_Context;

/*** 4-poles switch functions ***/

void SW4_Init(SW4_Context* sw4, unsigned int sw4_debouncing_ms);
void SW4_SetVoltage(SW4_Context* sw4, unsigned int new_voltage);
void SW4_UpdateState(SW4_Context* sw4);

#endif /* GENERIC_SW4_H */
