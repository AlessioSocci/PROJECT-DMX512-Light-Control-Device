/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    MKL25Z128xxx4_Project.c
 * @brief   Application entry point.
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"

/* TODO: insert other include files here. */

#include "application.h"

#include "encoders.h"
#include "dmx512.h"
#include "stick.h"
#include "display.h"
#include "time.h"

#include "picture.h"

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    SIM->SCGC5 |= (1 << 9); // Clock on PORT A
    SIM->SCGC5 |= (1 << 10); // Clock on PORT B
    SIM->SCGC5 |= (1 << 11); // Clock on PORT C
    SIM->SCGC5 |= (1 << 12); // Clock on PORT D
    SIM->SCGC5 |= (1 << 13); // Clock on PORT E

    time_init();

    stick_init();

    dmx512_init();

    encoder_init();

    display_init(115200);

	display_drawPicture(logo);

	display_flushPart(0, 127, 10, 30);

	time_delay(3000);

	display_drawPicture(clear);

	display_flushPart(0, 127, 0, 63);

	time_delay(1000);

	display_drawPicture(background_pattern); // draw picture

	display_flushPart(0, 127, 0, 50);

	y = 1;

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1)
    {
    	if(arrival_taskA_flag == 1) // task A entry point
        {
    		arrival_taskA_flag = 0;

    		GPIOB->PTOR |= (1 << 19);
        }

    	if(arrival_task0_flag == 1 && (disable_task0_flag == 0)) // task 0 entry point
    	{
    		uint32_t current_time = ticks;

    		arrival_task0_flag = 0;
    		running_task0_flag = 1;
    		queued_task0_flag = 0;

    		encoder_interruptEnable();

    		running_task0_flag = 0;
    		queuedTime_task0 = 0;
    		burstTime_task0 = ticks - current_time;
    	}


    	if((arrival_task1_flag == 1) && (disable_task1_flag == 0)) // task 1 entry point
    	{
    		uint32_t current_time = ticks;

    		arrival_task1_flag = 0;
    		running_task1_flag = 1;
    		queued_task1_flag = 0;

    		application_stickManage();

    		disable_task1_flag = 1;

    		running_task1_flag = 0;
    		queuedTime_task1 = 0;
    		burstTime_task1 = ticks - current_time;
        }


    	if(arrival_task2_flag == 1 && (disable_task2_flag == 0)) // task 2 entry point
        {
    		uint32_t current_time = ticks;

    		arrival_task2_flag = 0;
    		running_task2_flag = 1;
    		queued_task2_flag = 0;

    		encoder_interruptDisable();

    		application_dmxManage();

    		running_task2_flag = 0;
    		queuedTime_task2 = 0;
    		burstTime_task2 = ticks - current_time;
        }


    	if((arrival_task3_flag == 1) && (((encoder_triggerFlag != 0) || (stick_triggerFlag = 1)) && (disable_task3_flag == 0))) // task 3 entry point
    	{
    		uint32_t current_time = ticks;

    		arrival_task3_flag = 0;
    		running_task3_flag = 1;
    		queued_task3_flag = 0;

    		encoder_interruptDisable();

    		application_displayManage();


    		disable_task3_flag = 1;

    		running_task3_flag = 0;
    		queuedTime_task3 = 0;
    		burstTime_task3 = ticks - current_time;
    	}
    	else if(arrival_task3_flag == 1)
    	{
    		arrival_task3_flag = 0;
    	}

    	i++ ;
    }

    return 0;
}
