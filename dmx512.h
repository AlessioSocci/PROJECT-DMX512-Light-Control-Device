/*
 * dmx512.h
 *
 *  Created on: 17 ago 2019
 *      Author: Alessio
 */

#ifndef DMX512_H_
#define DMX512_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"

void dmx512_init(void);
void dmx512_send(uint8_t dataOut[129]);


#endif /* DMX512_H_ */
