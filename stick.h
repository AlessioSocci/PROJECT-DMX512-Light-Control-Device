/*
 * stick.h
 *
 *  Created on: 17 ago 2019
 *      Author: Alessio
 */

#ifndef STICK_H_
#define STICK_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"

extern uint8_t stick_triggerFlag;

void stick_init(void);
uint16_t stick_panRead(void);
uint16_t stick_tiltRead(void);
uint8_t stick_pushRead(void);

#endif /* STICK_H_ */
