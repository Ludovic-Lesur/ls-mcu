/*
 * lssgiu.c
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludo
 */

#include "lssgiu.h"

#include "kvb.h"
#include "gpio.h"
#include "lsmcu.h"
#include "mapping.h"
#include "mode.h"
#include "nvic.h"
#include "usart.h"
#ifdef DEBUG
#include "string.h"
#endif
#include "types.h"

/*** LSSGIU local macros ***/

#define LSSGIU_RX_BUFFER_SIZE	32

/*** LSSGIU local structures ***/

typedef struct {
	volatile uint8_t rx_buf[LSSGIU_RX_BUFFER_SIZE];
	volatile uint32_t rx_write_idx;
	uint32_t rx_read_idx;
} LSSGIU_Context;

/*** LSSGIU external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** LSSGIU local global variables ***/

static LSSGIU_Context lssgiu_ctx;

/*** LSSGIU local functions ***/

/* DECODE THE CURRENT LSSGIU COMMAND.
 * @param:	None.
 * @return: None.
 */
static void _LSSGIU_decode(void) {
	// Read last command.
	uint8_t ls_cmd = lssgiu_ctx.rx_buf[lssgiu_ctx.rx_read_idx];
	if (ls_cmd <= LSMCU_TCH_SPEED_LAST) {
		// Store current speed in global context.
		lsmcu_ctx.speed_kmh = ls_cmd;
	}
	else if (ls_cmd <= LSMCU_SPEED_LIMIT_LAST) {
		// Store speed limit in global context.
		lsmcu_ctx.speed_limit_kmh = 10 * (ls_cmd - LSMCU_SPEED_LIMIT_OFFSET);
	}
	// Increment read index and manage roll-over.
	lssgiu_ctx.rx_read_idx++;
	if (lssgiu_ctx.rx_read_idx >= LSSGIU_RX_BUFFER_SIZE) {
		lssgiu_ctx.rx_read_idx = 0;
	}
}

/*** LSSGIU functions ***/

/* INIT LSSGIU COMMUNICATION MODULE.
 * @param:	None.
 * @return:	None.
 */
void LSSGIU_Init(void) {
	// Init context.
	uint32_t idx = 0;
	for (idx=0 ; idx<LSSGIU_RX_BUFFER_SIZE ; idx++) lssgiu_ctx.rx_buf[idx] = LSMCU_IN_NOP;
	lssgiu_ctx.rx_write_idx = 0;
	lssgiu_ctx.rx_read_idx = 0;
	// Enable USART interrupt.
	NVIC_enable_interrupt(NVIC_INTERRUPT_USART1);
}

/* FILL LSSGIU BUFFER (CALLED BY USART2 INTERRUPT HANDLER).
 * @param lssgiu_cmd:	The new LSSGIU command to store.
 * @return:			None.
 */
void LSSGIU_FillRxBuffer(uint8_t ls_cmd) {
	lssgiu_ctx.rx_buf[lssgiu_ctx.rx_write_idx] = ls_cmd;
	lssgiu_ctx.rx_write_idx++;
	// Roll-over management.
	if (lssgiu_ctx.rx_write_idx >= LSSGIU_RX_BUFFER_SIZE) {
		lssgiu_ctx.rx_write_idx = 0;
	}
}

/* SEND AN LSSGIU COMMAND TO SGKCU.
 * @param lssgiu_cmd: 	LSSGIU command (byte) to transmit.
 * @return: 			None.
 */
void LSSGIU_Send(uint8_t ls_cmd) {
#ifdef DEBUG
	char_t str_value[16];
	STRING_value_to_string(ls_cmd, STRING_FORMAT_DECIMAL, 0, str_value);
	USART1_send_string("\nLSSGIU command = ");
	USART1_send_string(str_value);
	USART1_send_string("\n");
#else
	USART1_send_byte(ls_cmd);
#endif
}

/* MAIN ROUTINE OF LSSGIU COMMAND MANAGER.
 * @param:	None.
 * @return:	None.
 */
void LSSGIU_Task(void) {
	// LSSGIU routine.
	if (lssgiu_ctx.rx_read_idx != lssgiu_ctx.rx_write_idx) {
		_LSSGIU_decode();
	}
}
