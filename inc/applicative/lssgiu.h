/*
 * lssgiu.h
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludo
 */

#ifndef LSSGIU_H
#define LSSGIU_H

#include "tch.h"
#include "stdint.h"

/*** LSSGIU structures ***/

typedef enum {
	LSMCU_OUT_ZBA_ON = 0,
	LSMCU_OUT_ZBA_OFF,
	LSMCU_OUT_RSEC_ON,
	LSMCU_OUT_RSEC_OFF,
	LSMCU_OUT_ZDV_ON,
	LSMCU_OUT_ZDV_OFF,
	LSMCU_OUT_ZPT_BACK_UP,
	LSMCU_OUT_ZPT_BACK_DOWN,
	LSMCU_OUT_ZPT_FRONT_UP,
	LSMCU_OUT_ZPT_FRONT_DOWN,
	LSMCU_OUT_ZDJ_OFF,
	LSMCU_OUT_ZEN_ON,
	LSMCU_OUT_COMPRESSOR_AUTO_REG_MIN_ON,
	LSMCU_OUT_COMPRESSOR_AUTO_REG_MAX_ON,
	LSMCU_OUT_COMPRESSOR_DIRECT_ON,
	LSMCU_OUT_COMPRESSOR_OFF,
	LSMCU_OUT_FPB_ON,
	LSMCU_OUT_FPB_OFF,
	LSMCU_OUT_FPB_APPLY,
	LSMCU_OUT_FPB_NEUTRAL,
	LSMCU_OUT_FPB_RELEASE,
	LSMCU_OUT_BPGD,
	LSMCU_OUT_ZVM_ON,
	LSMCU_OUT_ZVM_OFF,
	LSMCU_OUT_MPINV_FORWARD,
	LSMCU_OUT_MPINV_NEUTRAL,
	LSMCU_OUT_MPINV_BACKWARD,
	LSMCU_OUT_MP_0,
	LSMCU_OUT_MP_T_MORE,
	LSMCU_OUT_MP_T_LESS,
	LSMCU_OUT_MP_F_MORE,
	LSMCU_OUT_MP_F_LESS,
	LSMCU_OUT_MP_PR,
	LSMCU_OUT_MP_P,
	LSMCU_OUT_FD_APPLY,
	LSMCU_OUT_FD_NEUTRAL,
	LSMCU_OUT_FD_RELEASE,
	LSMCU_OUT_WHISTLE_HIGH_TONE,
	LSMCU_OUT_WHISTLE_LOW_TONE,
	LSMCU_OUT_WHISTLE_NEUTRAL,
	LSMCU_OUT_BPEV_ON,
	LSMCU_OUT_BPEV_OFF,
	LSMCU_OUT_BPSA_ON,
	LSMCU_OUT_BPSA_OFF,
	LSMCU_OUT_ZFG_ON,
	LSMCU_OUT_ZFG_OFF,
	LSMCU_OUT_ZFD_ON,
	LSMCU_OUT_ZFD_OFF,
	LSMCU_OUT_ZPR_ON,
	LSMCU_OUT_ZPR_OFF,
	LSMCU_OUT_ZLFRG_ON,
	LSMCU_OUT_ZLFRG_OFF,
	LSMCU_OUT_ZLFRD_ON,
	LSMCU_OUT_ZLFRD_OFF,
	LSMCU_OUT_URGENCY,
	LSMCU_OUT_NOP = 0xFF
} LSMCU_To_LSSGIU;

typedef enum {
	// First range is reserved for coding current speed in km/h.
	LSMCU_TCH_SPEED_OFFSET = 0,
	LSMCU_TCH_SPEED_LAST = TCH_SPEED_MAX_KMH,
	// Second range is reserved for coding speed limit in tens of km/h.
	LSMCU_SPEED_LIMIT_OFFSET = (TCH_SPEED_MAX_KMH + 1),
	LSMCU_SPEED_LIMIT_LAST = (TCH_SPEED_MAX_KMH + 1 + (TCH_SPEED_MAX_KMH / 10)),
	// NOP.
	LSMCU_IN_NOP = 0xFF
} LSSGIU_To_LSMCU;

/*** LSSGIU functions ***/

void LSSGIU_Init(void);
void LSSGIU_FillRxBuffer(uint8_t ls_cmd);
void LSSGIU_Send(uint8_t ls_cmd);
void LSSGIU_Task(void);

#endif /* LSSGIU_H */
