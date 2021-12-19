/*
 * mpinv.c
 *
 *  Created on: 8 apr. 2018
 *      Author: Ludo
 */

#include "mpinv.h"

#include "gpio.h"
#include "lssgiu.h"
#include "mapping.h"
#include "sw3.h"

/*** MPINV local structures ***/

typedef struct {
	SW3_Context mpinv_sw3;
	SW3_State mpinv_previous_state;
} MPINV_Context;

/*** MPINV local global variables ***/

static MPINV_Context mpinv_ctx;

/*** MPINV functions ***/

/* INIT MPINV MODULE.
 * @param:	None.
 * @return:	None.
 */
void MPINV_Init(void) {
	// Init GPIO.
	SW3_Init(&mpinv_ctx.mpinv_sw3, &GPIO_MPINV, 100);
	mpinv_ctx.mpinv_previous_state = SW3_NEUTRAL;
}

/* UPDATE THE VOLTAGE READ ON MPINV SELECTOR (CALLED BY ADC ROUTINE).
 * @param new_voltage:	New voltage measured.
 * @return:				None.
 */
void MPINV_SetVoltageMv(unsigned int mpinv_voltage_mv) {
	SW3_SetVoltageMv(&mpinv_ctx.mpinv_sw3, mpinv_voltage_mv);
}

/* MAIN ROUTINE OF MPINV MODULE.
 * @param:	None.
 * @return:	None.
 */
void MPINV_Task(void) {
	// Update current state.
	SW3_UpdateState(&mpinv_ctx.mpinv_sw3);
	// Perform actions according to state.
	switch (mpinv_ctx.mpinv_sw3.sw3_state) {
	case SW3_BACK:
		if (mpinv_ctx.mpinv_previous_state != SW3_BACK) {
			// Backward.
			LSSGIU_Send(LSMCU_OUT_MPINV_BACKWARD);
		}
		break;
	case SW3_NEUTRAL:
		if (mpinv_ctx.mpinv_previous_state != SW3_NEUTRAL) {
			// Forward.
			LSSGIU_Send(LSMCU_OUT_MPINV_NEUTRAL);
		}
		break;
	case SW3_FRONT:
		if (mpinv_ctx.mpinv_previous_state != SW3_FRONT) {
			// Forward.
			LSSGIU_Send(LSMCU_OUT_MPINV_FORWARD);
		}
		break;
	default:
		// Unknown state.
		break;
	}
	// Update previous state.
	mpinv_ctx.mpinv_previous_state = mpinv_ctx.mpinv_sw3.sw3_state;
}
