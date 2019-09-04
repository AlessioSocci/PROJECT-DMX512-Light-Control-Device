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

	time_delay(3000);

	display_drawPicture(clear);

	time_delay(1000);

	display_drawPicture(background_pattern); // draw picture

    // stick variable
    uint16_t stickPanValue, stickTiltValue, stickPushValue = 0;

    // dmx512 variable
	uint8_t dataOut[129];

	for (int i = 0; i < 129; i++)
	{
		dataOut[i] = 0;
	}

	y = 1;

	uint8_t fineadjustflipflop = 0;

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1)
    {
    	if(arrival_task0_flag == 1)
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

    	if((arrival_task1_flag == 1) && (disable_task1_flag == 0)) // task 0 entry point
    	{
    		uint32_t current_time = ticks;

    		arrival_task1_flag = 0;
    		running_task1_flag = 1;
    		queued_task1_flag = 0;

    		stickPanValue = stick_panRead();
    		stickTiltValue = stick_tiltRead();
    		stickPushValue = stick_pushRead();

    		if((stickPanValue > 2400) && (y != 129))
    		{
    			y++;
    			disable_task1_flag = 1;
    			stick_triggerFlag = 1;
    		}
    		else if((stickPanValue < 1000) && (y != 1))
    		{
    			y--;
    			disable_task1_flag = 1;
    			stick_triggerFlag = 1;
    		}

    		if((stickTiltValue > 2400) && (y <= 121))
    		{
    			y += 8;
    			disable_task1_flag = 1;
    			stick_triggerFlag = 1;
    		}
    		else if((stickTiltValue < 1000) && (y >= 9))
    		{
    			y -= 8;
    			disable_task1_flag = 1;
    			stick_triggerFlag = 1;
    		}

    		if(stickPushValue == 1)
    		{
    			disable_task1_flag = 1;

    			if(fineadjustflipflop == 0)
    			{
    				fineadjust = 1;
    				fineadjustflipflop = 1;
    			}
    			else if(fineadjustflipflop == 1)
    			{
    				fineadjust = 0;
    				fineadjustflipflop = 0;
    			}
    		}
        }
    	else if(arrival_task1_flag == 1)
    	{
    		arrival_task1_flag = 0;
    	}

    	if(arrival_task2_flag == 1) // task 2 entry point
        {
    		uint32_t current_time = ticks;

    		arrival_task2_flag = 0;
    		running_task2_flag = 1;
    		queued_task2_flag = 0;

    		for(int i = y; i < (y + 8); i++)
    	    {
    			dataOut[i] = encoderOut[i];
    	    }

    		encoder_interruptDisable();

    		dmx512_send(dataOut);

    		running_task2_flag = 0;
    		queuedTime_task2 = 0;
    		burstTime_task2 = ticks - current_time;
        }

    	if((arrival_task3_flag == 1) && (((encoder_triggerFlag == 1) || (stick_triggerFlag = 1)) && (disable_task3_flag == 0))) // task 4 entry point
    	{
    		uint32_t current_time = ticks;

    		arrival_task3_flag = 0;
    		running_task3_flag = 1;
    		queued_task3_flag = 0;

    		disable_task3_flag = 1;

    		encoder_interruptDisable();

    		for(int i = 0; i < 8; i++)
    		{
    			display_clearBar (i + 1);
    		}

    		for(int i = 0; i < 8; i++)
    		{
    			display_drawBar(i + 1, dataOut[i + y]);
    		}


    		display_drawNumber(1, y);
    	    display_drawNumber(2, y + 1);
    	    display_drawNumber(3, y + 2);
    	    display_drawNumber(4, y + 3);
    	    display_drawNumber(5, y + 4);
    	    display_drawNumber(6, y + 5);
    	    display_drawNumber(7, y + 6);
    	    display_drawNumber(8, y + 7);

    		display_flush();

    		encoder_triggerFlag = 0;
    		stick_triggerFlag = 0;

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
