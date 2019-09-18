/*
 * application.h
 *
 *  Created on: 18 set 2019
 *      Author: Alessio
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"

#include "encoders.h"
#include "dmx512.h"
#include "stick.h"
#include "display.h"
#include "time.h"

#include "picture.h"

void application_stickManage(void);
void application_dmxManage(void);
void application_displayManage(void);

#endif /* APPLICATION_H_ */
