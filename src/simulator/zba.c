/*
 * zba.c
 *
 *  Created on: 6 feb. 2020
 *      Author: Ludo
 */

#include "zba.h"

#include "lsmcu.h"
#include "lssgiu.h"
#include "mapping.h"
#include "sw2.h"

/*** ZBA external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** ZBA local global variables ***/

static SW2_Context zba;

/*** ZBA functions ***/

/* INIT ZDA MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZBA_Init(void) {
	// Init GPIO.
	SW2_Init(&zba, &GPIO_ZBA, 1, 100); // ZBA active high (+3.3V supply present).
	// Init global context.
	lsmcu_ctx.zba_closed = 0;
}

/* MAIN TASK OF ZBA MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZBA_Task(void) {
	// Update ZBA state.
	SW2_UpdateState(&zba);
	if (zba.state == SW2_ON) {
		// Send command on change.
		if (lsmcu_ctx.zba_closed == 0) {
			LSSGIU_Send(LSMCU_OUT_ZBA_ON);
		}
		lsmcu_ctx.zba_closed = 1;
	}
	else {
		// Send command on change.
		if (lsmcu_ctx.zba_closed != 0) {
			LSSGIU_Send(LSMCU_OUT_ZBA_OFF);
		}
		lsmcu_ctx.zba_closed = 0;
	}
}