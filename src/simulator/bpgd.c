/*
 * bpgd.c
 *
 *  Created on: 26 dec. 2021
 *      Author: Ludo
 */

#include "bpgd.h"

#include "gpio.h"
#include "lsmcu.h"
#include "lssgiu.h"
#include "manometer.h"
#include "mapping.h"
#include "sw2.h"
#include "tim.h"

/*** BPGD local macros ***/

#define BPGD_CG_RE_PRESSURE_DECIBARS	50
#define BPGD_INHIBIT_DELAY_MS			5000

/*** BPGD local structures ***/

typedef struct {
	SW2_Context sw2;
	unsigned char enable;
	unsigned int inhibit_start_ms;
} BPGD_Context;

/*** BPGD external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** BPGD local global variables ***/

static BPGD_Context bpgd_ctx;

/*** BPGD functions ***/

/* INIT BPGD MODULE.
 * @param:	None.
 * @eturn:	None.
 */
void BPGD_Init(void) {
	// Init GPIO.
	SW2_Init(&bpgd_ctx.sw2, &GPIO_BPGD, 0, 100); // BPGD active low.
	// Init global context.
	bpgd_ctx.inhibit_start_ms = 0;
	bpgd_ctx.enable = 1;
}

/* MAIN TASK OF BPGD MODULE.
 * @param:	None.
 * @return:	None.
 */
void BPGD_Task(void) {
	// Update BPGD state.
	SW2_UpdateState(&bpgd_ctx.sw2);
	if (bpgd_ctx.sw2.state == SW2_ON) {
		// Send commands on change.
		if (bpgd_ctx.enable != 0) {
			// Update manometers.
			MANOMETER_SetPressure(lsmcu_ctx.manometer_cg, BPGD_CG_RE_PRESSURE_DECIBARS);
			MANOMETER_SetPressure(lsmcu_ctx.manometer_re, BPGD_CG_RE_PRESSURE_DECIBARS);
			MANOMETER_SetPressure(lsmcu_ctx.manometer_cf1, 0);
			MANOMETER_SetPressure(lsmcu_ctx.manometer_cf2, 0);
			// Send sound command.
			LSSGIU_Send(LSMCU_OUT_BPGD);
			// Disable module and start delay.
			bpgd_ctx.enable = 0;
			bpgd_ctx.inhibit_start_ms = TIM2_GetMs();
		}
	}
	// Check delay.
	if (TIM2_GetMs() > (bpgd_ctx.inhibit_start_ms + BPGD_INHIBIT_DELAY_MS)) {
		bpgd_ctx.enable = 1;
	}
}