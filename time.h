/*
 * time.h
 *
 *  Created on: 15 mar 2019
 *      Author: Alessio
 */

#ifndef TIME_H_
#define TIME_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"

extern uint32_t ticks;


extern uint32_t burstTime_task0;
extern uint32_t queuedTime_task0;

extern uint32_t burstTime_task1;
extern uint32_t queuedTime_task1;

extern uint32_t burstTime_task2;
extern uint32_t queuedTime_task2;

extern uint32_t burstTime_task3;
extern uint32_t queuedTime_task3;


extern uint8_t arrival_taskA_flag;


extern uint8_t arrival_task0_flag;
extern uint8_t disable_task0_flag;
extern uint8_t running_task0_flag;
extern uint8_t queued_task0_flag;

extern uint8_t arrival_task1_flag;
extern uint8_t disable_task1_flag;
extern uint8_t running_task1_flag;
extern uint8_t queued_task1_flag;

extern uint8_t arrival_task2_flag;
extern uint8_t disable_task2_flag;
extern uint8_t running_task2_flag;
extern uint8_t queued_task2_flag;

extern uint8_t arrival_task3_flag;
extern uint8_t disable_task3_flag;
extern uint8_t running_task3_flag;
extern uint8_t queued_task3_flag;


void time_delay(uint32_t mseconds);
void time_init(void);

#endif /* TIME_H_ */
