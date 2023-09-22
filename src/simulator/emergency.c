/*
 * emergency.c
 *
 *  Created on: 26 dec. 2021
 *      Author: Ludo
 */

#include "emergency.h"

#include "gpio.h"
#include "lsmcu.h"
#include "lsagiu.h"
#include "manometer.h"
#include "mapping.h"
#include "sw2.h"
#include "stdint.h"

/*** EMERGENCY local macros ***/

#define EMERGENCY_CG_SPEED_MBAR_PER_SECOND	2000
#define EMERGENCY_RE_SPEED_MBAR_PER_SECOND	2000
#define EMERGENCY_CF_SPEED_MBAR_PER_SECOND	2000

/*** EMERGENCY local structures ***/

typedef struct {
	SW2_context_t bpurg;
	uint8_t previous_state;
} EMERGENCY_Context;

/*** EMERGENCY external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** EMERGENCY local global variables ***/

static EMERGENCY_Context emergency_ctx;

/*** EMERGENCY functions ***/

/* INIT EMERGENCY MODULE.
 * @param:	None.
 * @return:	None.
 */
void EMERGENCY_init(void) {
	// Init GPIO.
	SW2_init(&emergency_ctx.bpurg, &GPIO_BPURG, 0, 100); // EMERGENCY active low.
	// Init context.
	emergency_ctx.previous_state = 0;
	lsmcu_ctx.emergency = 0;
}

/* MAIN TAS OF EMERGENCY MODULE.
 * @param:	None.
 * @return:	None.
 */
void EMERGENCY_task(void) {
	// Update BPURG state.
	SW2_update_state(&emergency_ctx.bpurg);
	if (emergency_ctx.bpurg.state == SW2_ON) {
		// Update global context.
		lsmcu_ctx.emergency = 1;
	}
	// Check global flag.
	if ((lsmcu_ctx.emergency != 0) && (emergency_ctx.previous_state == 0)) {
		// Trigger emergency brake.
		MANOMETER_set_pressure(lsmcu_ctx.manometer_cg, 0, EMERGENCY_CG_SPEED_MBAR_PER_SECOND);
		MANOMETER_set_pressure(lsmcu_ctx.manometer_re, 0, EMERGENCY_RE_SPEED_MBAR_PER_SECOND);
		MANOMETER_set_pressure(lsmcu_ctx.manometer_cf1, ((lsmcu_ctx.manometer_cf1) -> pressure_limit_mbar), EMERGENCY_CF_SPEED_MBAR_PER_SECOND);
		MANOMETER_set_pressure(lsmcu_ctx.manometer_cf2, ((lsmcu_ctx.manometer_cf2) -> pressure_limit_mbar), EMERGENCY_CF_SPEED_MBAR_PER_SECOND);
		// Open DJ.
		lsmcu_ctx.dj_locked = 0;
		// Send sound command.
		LSAGIU_Send(LSMCU_OUT_EMERGENCY);
	}
	// Release emergency state only when train is stopped.
	if ((emergency_ctx.bpurg.state == SW2_OFF) && (lsmcu_ctx.speed_kmh == 0)) {
		lsmcu_ctx.emergency = 0;
	}
	// Update state.
	emergency_ctx.previous_state = lsmcu_ctx.emergency;
}