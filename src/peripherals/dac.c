/*
 * dac.c
 *
 *  Created on: 18 sept. 2017
 *      Author: Ludo
 */

#include "dac.h"

#include "adc.h"
#include "dac_reg.h"
#include "gpio.h"
#include "mapping.h"
#include "rcc_reg.h"
#include "types.h"

/*** DAC local macros ***/

// DAC full scale value for 12-bits resolution.
#define DAC_FULL_SCALE 	4095

/*** DAC functions ***/

/* CONFIGURE DAC PERIPHERAL.
 * @param: 	None.
 * @return: None.
 */
void DAC_init(void) {
	// Configure analog GPIOs.
	GPIO_configure(&GPIO_AM, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
	// Enable peripheral clock.
	RCC -> APB1ENR |= (0b1 << 29); // DACEN='1'.
	// Configure peripheral.
	DAC -> CR &= ~(0b1 << 1); // BOFF1='0'.
	DAC -> CR |= (0b1 << 17); // BOFF2='1'.
	DAC -> CR |= (0b1 << 0); // EN1='1'.
	DAC -> CR &= ~(0b1 << 16); // EN2='0'.
	// Reset output.
	DAC -> DHR12R1 = 0;
}

/* SET DAC OUTPUT VOLTAGE.
 * @param voltage: 	Output voltage expressed in mV (between 0 and VCC_MV).
 * @return: 		None.
 */
void DAC_set_voltage_mv(uint32_t voltage_mv) {
	// Ensure new voltage is reachable.
	uint32_t real_voltage_mv = voltage_mv;
	if (real_voltage_mv < 0) {
		real_voltage_mv = 0;
	}
	if (real_voltage_mv > ADC_VCC_DEFAULT_MV) {
		real_voltage_mv = ADC_VCC_DEFAULT_MV;
	}
	DAC -> DHR12R1 = (DAC_FULL_SCALE * real_voltage_mv) / (ADC_VCC_DEFAULT_MV);
}

/* GET DAC CURRENT OUTPUT VOLTAGE.
 * @param:			None.
 * @return voltage:	Current output voltage expressed in mV (between 0 and VCC_MV).
 */
uint32_t DAC_get_voltage_mv(void) {
	uint32_t voltage_mv = ((DAC -> DOR1) * ADC_VCC_DEFAULT_MV) / (DAC_FULL_SCALE);
	return voltage_mv;
}
