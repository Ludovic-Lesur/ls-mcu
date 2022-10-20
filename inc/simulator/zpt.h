/*
 * zpt.h
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludo
 */

#ifndef __ZPT_H__
#define __ZPT_H__

/*** ZPT functions ***/

void ZPT_init(void);
void ZPT_set_voltage_mv(unsigned int zpt_voltage_mv);
void ZPT_task(void);

#endif /* __ZPT_H__ */
