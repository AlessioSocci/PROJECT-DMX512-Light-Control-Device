/*
 * application.c
 *
 *  Created on: 18 set 2019
 *      Author: Alessio
 */

#include "application.h"
#include "time.h"


// stick variable
uint16_t stickPanValue, stickTiltValue, stickPushValue = 0;
uint8_t fineadjustflipflop = 0;

// dmx512 variable
uint8_t dataOut[129] = {0};

void application_stickManage(void)
{
	stickPanValue = stick_panRead();
	stickTiltValue = stick_tiltRead();
	stickPushValue = stick_pushRead();

	if((stickPanValue > 2400) && (y != 129))
	{
		y++;
		stick_triggerFlag = 1;
	}
	else if((stickPanValue < 1000) && (y != 1))
	{
		y--;
		stick_triggerFlag = 1;
	}

	if((stickTiltValue > 2400) && (y <= 121))
	{
		y += 8;
		stick_triggerFlag = 1;
	}
	else if((stickTiltValue < 1000) && (y >= 9))
	{
		y -= 8;
		stick_triggerFlag = 1;
	}

	if(stickPushValue == 1)
	{
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

void application_dmxManage(void)
{
	for(int i = y; i < (y + 8); i++)
	{
		dataOut[i] = encoderOut[i];
	}

	dmx512_send(dataOut);
}

void application_displayManage(void)
{
	switch(encoder_triggerFlag)
	{
	case 1 :

		display_clearBar(1);
		display_drawBar(1, dataOut[y]);
		display_flushPart(4, 16, 0, 40);

		break;

	case 2 :

		display_clearBar(2);
		display_drawBar(2, dataOut[1 + y]);
		display_flushPart(19, 32, 0, 40);

		break;

	case 3 :

		display_clearBar(3);
		display_drawBar(3, dataOut[2 + y]);
		display_flushPart(35, 48, 0, 40);

		break;

	case 4 :

		display_clearBar(4);
		display_drawBar(4, dataOut[3 + y]);
		display_flushPart(51, 64, 0, 44);

		break;

	case 5 :

		display_clearBar(5);
		display_drawBar(5, dataOut[4 + y]);
		display_flushPart(67, 80, 0, 40);

		break;

	case 6 :

		display_clearBar(6);
		display_drawBar(6, dataOut[5 + y]);
		display_flushPart(83, 96, 0, 40);

		break;

	case 7 :

		display_clearBar(7);
		display_drawBar(7, dataOut[6 + y]);
		display_flushPart(99, 112, 0, 40);

		break;

	case 8 :

		display_clearBar(8);
		display_drawBar(8, dataOut[7 + y]);
		display_flushPart(115, 127, 0, 40);

		break;
	}

	if(stick_triggerFlag == 1)
	{
		display_clearBar(1);
		display_drawBar(1, dataOut[y]);
		display_clearBar(2);
		display_drawBar(2, dataOut[1 + y]);
		display_clearBar(3);
		display_drawBar(3, dataOut[2 + y]);
		display_clearBar(4);
		display_drawBar(4, dataOut[3 + y]);
		display_clearBar(5);
		display_drawBar(5, dataOut[4 + y]);
		display_clearBar(6);
		display_drawBar(6, dataOut[5 + y]);
		display_clearBar(7);
		display_drawBar(7, dataOut[6 + y]);
		display_clearBar(8);
		display_drawBar(8, dataOut[7 + y]);

		display_drawNumber(1, y);
		display_drawNumber(2, y + 1);
		display_drawNumber(3, y + 2);
		display_drawNumber(4, y + 3);
		display_drawNumber(5, y + 4);
		display_drawNumber(6, y + 5);
		display_drawNumber(7, y + 6);
		display_drawNumber(8, y + 7);

		display_flushPart(0, 127, 0, 63);

		stick_triggerFlag = 0;
	}

	encoder_triggerFlag = 0;
	stick_triggerFlag = 0;
}
