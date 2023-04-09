/*
 * urgency.c
 *
 *  Created on: 26 dec. 2021
 *      Author: Ludo
 */

#include "urgency.h"

#include "gpio.h"
#include "lsmcu.h"
#include "lsagiu.h"
#include "manometer.h"
#include "mapping.h"
#include "sw2.h"
#include "stdint.h"

/*** URGENCY local macros ***/

#define URGENCY_CG_SPEED_MBAR_PER_SECOND	500
#define URGENCY_RE_SPEED_MBAR_PER_SECOND	500
#define URGENCY_CF_SPEED_MBAR_PER_SECOND	500

/*** URGENCY local structures ***/

typedef struct {
	SW2_context_t bpurg;
	uint8_t previous_state;
} URGENCY_Context;

/*** URGENCY external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** URGENCY local global variables ***/

static URGENCY_Context urgency_ctx;

/*** URGENCY functions ***/

/* INIT URGENCY MODULE.
 * @param:	None.
 * @return:	None.
 */
void URGENCY_init(void) {
	// Init GPIO.
	SW2_init(&urgency_ctx.bpurg, &GPIO_BPURG, 0, 100); // URGENCY active low.
	// Init context.
	urgency_ctx.previous_state = 0;
	lsmcu_ctx.urgency = 0;
}

/* MAIN TAS OF URGENCY MODULE.
 * @param:	None.
 * @return:	None.
 */
void URGENCY_task(void) {
	// Update BPURG state.
	SW2_update_state(&urgency_ctx.bpurg);
	if (urgency_ctx.bpurg.state == SW2_ON) {
		// Update global context.
		lsmcu_ctx.urgency = 1;
	}
	// Check global flag.
	if ((lsmcu_ctx.urgency != 0) && (urgency_ctx.previous_state == 0)) {
		// Trigger urgency brake.
		MANOMETER_set_pressure(lsmcu_ctx.manometer_cg, 0, URGENCY_CG_SPEED_MBAR_PER_SECOND);
		MANOMETER_set_pressure(lsmcu_ctx.manometer_re, 0, URGENCY_RE_SPEED_MBAR_PER_SECOND);
		MANOMETER_set_pressure(lsmcu_ctx.manometer_cf1, ((lsmcu_ctx.manometer_cf1) -> pressure_limit_mbar), URGENCY_CF_SPEED_MBAR_PER_SECOND);
		MANOMETER_set_pressure(lsmcu_ctx.manometer_cf2, ((lsmcu_ctx.manometer_cf2) -> pressure_limit_mbar), URGENCY_CF_SPEED_MBAR_PER_SECOND);
		// Open DJ.
		lsmcu_ctx.dj_locked = 0;
		// Send sound command.
		LSAGIU_Send(LSMCU_OUT_URGENCY);
	}
	// Release urgency state only when train is stopped.
	if ((urgency_ctx.bpurg.state == SW2_OFF) && (lsmcu_ctx.speed_kmh == 0)) {
		lsmcu_ctx.urgency = 0;
	}
	// Update state.
	urgency_ctx.previous_state = lsmcu_ctx.urgency;
}
