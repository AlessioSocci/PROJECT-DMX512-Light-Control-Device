/*
 * Encoders.h
 *
 *  Created on: 17 ago 2019
 *      Author: Alessio
 */

#ifndef ENCODERS_H_
#define ENCODERS_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"

extern uint8_t encoderOut[129];
extern uint8_t y;
extern uint8_t fineadjust;
extern uint8_t encoder_triggerFlag;
extern uint8_t encoder_1_to_4_acquisitionFlag;
extern uint8_t encoder_5_to_8_acquisitionFlag;

void encoder_init(void);
void encoder_interruptDisable(void);
void encoder_interruptEnable(void);

#endif /* ENCODERS_H_ */
