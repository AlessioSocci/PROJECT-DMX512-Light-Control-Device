/*
 * Encoders.c
 *
 *  Created on: 17 ago 2019
 *      Author: Alessio
 */

#include "encoders.h"
#include "time.h"

#define ENCODER_NUMBER_IMPULSE_THRESHOLD 	3

uint8_t encoderOut[129];
uint8_t encoderOutBuffer[129];
uint8_t flashmode[129];
uint8_t fineadjust = 1;
uint8_t encoder_triggerFlag = 0;

uint8_t encoder_impulseCounter[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

uint8_t y = 0;

void encoder_init(void)
{
	for (int i = 0; i < 129; i++)
	{
		encoderOut[i] = 0;
		flashmode[i] = 0;
	}

	// Encoder 1
	PORTA->PCR[1] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTA->PCR[2] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTC->PCR[7] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on, pull up

	GPIOA->PDDR &= ~(1 << 1);  // PORTA pin 1 set as input
	GPIOA->PDDR &= ~(1 << 2);  // PORTA pin 2 set as input
	GPIOC->PDDR &= ~(1 << 7);  // PORTC pin 7 set as input

	PORTA->PCR[1] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTA pin 1, on falling edge
	PORTA->PCR[2] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTA pin 2, on falling edge
	PORTC->PCR[7] &= ~((1 << 18) | (1 << 16));

	// Encoder 2
	PORTA->PCR[4] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTA->PCR[5] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTC->PCR[0] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on

	GPIOA->PDDR &= ~(1 << 4);  // PORTA pin 4 set as input
	GPIOA->PDDR &= ~(1 << 5);  // PORTA pin 5 set as input
	GPIOC->PDDR &= ~(1 << 0);  // PORTC pin 0 set as input

	PORTA->PCR[4] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTA pin 4, on falling edge
	PORTA->PCR[5] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTA pin 5, on falling edge
	PORTC->PCR[0] &= ~((1 << 18) | (1 << 16));

	// Encoder 3
	PORTA->PCR[12] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTA->PCR[13] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTC->PCR[3] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on

	GPIOA->PDDR &= ~(1 << 12);  // PORTA pin 4 set as input
	GPIOA->PDDR &= ~(1 << 13);  // PORTA pin 5 set as input
	GPIOC->PDDR &= ~(1 << 3);  // PORTC pin 0 set as input

	PORTA->PCR[12] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTA pin 4, on falling edge
	PORTA->PCR[13] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTA pin 5, on falling edge
	PORTC->PCR[3] &= ~((1 << 18) | (1 << 16));

	// Encoder 4
	PORTA->PCR[16] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTA->PCR[17] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTC->PCR[4] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on

	GPIOA->PDDR &= ~(1 << 16);  // PORTA pin 4 set as input
	GPIOA->PDDR &= ~(1 << 17);  // PORTA pin 5 set as input
	GPIOC->PDDR &= ~(1 << 4);  // PORTC pin 0 set as input

	PORTA->PCR[16] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTA pin 4, on falling edge
	PORTA->PCR[17] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTA pin 5, on falling edge
	PORTC->PCR[4] &= ~((1 << 18) | (1 << 16));

	// Encoder 5
	PORTD->PCR[0] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTD->PCR[1] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTC->PCR[5] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on

	GPIOD->PDDR &= ~(1 << 0);  // PORD pin 0 set as input
	GPIOD->PDDR &= ~(1 << 1);  // PORTD pin 1 set as input
	GPIOC->PDDR &= ~(1 << 5);  // PORTC pin 5 set as input

	PORTD->PCR[0] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTD pin 0, on falling edge
	PORTD->PCR[1] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTD pin 1, on falling edge
	PORTC->PCR[5] &= ~((1 << 18) | (1 << 16));

	// Encoder 6
	PORTD->PCR[2] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTD->PCR[3] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTC->PCR[6] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on

	GPIOD->PDDR &= ~(1 << 2);  // PORTD pin 2 set as input
	GPIOD->PDDR &= ~(1 << 3);  // PORTD pin 3 set as input
	GPIOC->PDDR &= ~(1 << 6);  // PORTC pin 6 set as input

	PORTD->PCR[2] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTD pin 2, on falling edge
	PORTD->PCR[3] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTD pin 3, on falling edge
	PORTC->PCR[6] &= ~((1 << 18) | (1 << 16));

	// Encoder 7
	PORTD->PCR[4] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTD->PCR[5] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTC->PCR[10] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on

	GPIOD->PDDR &= ~(1 << 4);  // PORTD pin 4 set as input
	GPIOD->PDDR &= ~(1 << 5);  // PORTD pin 5 set as input
	GPIOC->PDDR &= ~(1 << 10);  // PORTC pin 10 set as input

	PORTD->PCR[4] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTA pin 4, on falling edge
	PORTD->PCR[5] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTA pin 5, on falling edge
	PORTC->PCR[10] &= ~((1 << 18) | (1 << 16));

	// Encoder 8
	PORTD->PCR[6] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTD->PCR[7] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on
	PORTC->PCR[11] |= (1 << 8) | (1 << 4) | (1 << 1) | (1 << 0); // Set Alt 1 mode for GPIO, passive filter on

	GPIOD->PDDR &= ~(1 << 6);  // PORTA pin 6 set as input
	GPIOD->PDDR &= ~(1 << 7);  // PORTA pin 7 set as input
	GPIOC->PDDR &= ~(1 << 11);  // PORTC pin 11 set as input

	PORTD->PCR[6] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTA pin 6, on falling edge
	PORTD->PCR[7] |= (1 << 19) | (1 << 17);  // interrupt configuration on PORTA pin 7, on falling edge
	PORTC->PCR[11] &= ~((1 << 18) | (1 << 16));

	NVIC->IP[7] |= (1 << 23); // preemptive priority set as 2; min value is 3, max and default is 0
	NVIC->ISER[0] |= (1 << 30); // PORTA interrupt enable in NVIC

	NVIC->IP[7] |= (1 << 30); // preemptive priority set as 2; min value is 3, max and default is 0
	NVIC->ISER[0] |= (1 << 31); // PORTD interrupt enable in NVIC
}

void encoder_interruptDisable(void)
{
	NVIC->ICER[0] = (1 << 31) | (1 << 30); // PORTA and PORTD interrupt disable in NVIC -- ATTENTION! use with |= operator disable all other interrupts!!
}

void encoder_interruptEnable(void)
{
	NVIC->ISER[0] |= (1 << 31) | (1 << 30); // PORTA and PORTD interrupt enable in NVIC
}

void PORTA_IRQHandler(void)
{
	// Encoder 1 acquisition

	// Push Button
	if((GPIOA->PDIR & (1 << 2)) != (1 << 2))
	{
		if(flashmode[y] == 0)
		{
			encoderOutBuffer[y] = encoderOut[y];
			encoderOut[y] = 255;
			flashmode[y] = 1;
		}
		else if(flashmode[y] == 1)
		{
			encoderOut[y] = encoderOutBuffer[y];
			flashmode[y] = 0;
		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		encoder_triggerFlag = 1;
		encoder_interruptDisable();

		return;
	}

	// anti-clockwise rotation
	if(((GPIOC->PDIR & (1 << 7)) == (1 << 7)) && (GPIOA->PDIR & (1 << 1)) != (1 << 1))
	{
		encoder_impulseCounter[0]++;

		if(encoder_impulseCounter[0] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y] >= 250)
				{
					encoderOut[y] = 255;
				}
				else
				{
					encoderOut[y] += 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y] >= 240)
				{
					encoderOut[y] = 255;
				}
				else
				{
					encoderOut[y] += 20;
				}
			}

			encoder_interruptDisable();
			encoder_triggerFlag = 1;
		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		return;
	}

	// clockwise rotation
	if(((GPIOC->PDIR & (1 << 7)) != (1 << 7)) && (GPIOA->PDIR & (1 << 1)) != (1 << 1))
	{
		encoder_impulseCounter[1]++;

		if(encoder_impulseCounter[1] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y] <= 10)
				{
					encoderOut[y] = 0;
				}
				else
				{
					encoderOut[y] -= 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y] <= 20)
				{
					encoderOut[y] = 0;
				}
				else
				{
					encoderOut[y] -= 20;
				}
			}

			encoder_triggerFlag = 1;
			encoder_interruptDisable();
		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);
		return;
	}

	// Encoder 2 acquisition

	// Push Button
	if((GPIOA->PDIR & (1 << 4)) != (1 << 4))
	{
		if(flashmode[y + 1] == 0)
		{
			encoderOutBuffer[y + 1] = encoderOut[y + 1];
			encoderOut[y + 1] = 255;
			flashmode[y + 1] = 1;
		}
		else if(flashmode[y + 1] == 1)
		{
			encoderOut[y + 1] = encoderOutBuffer[y + 1];
			flashmode[y + 1] = 0;
		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		encoder_triggerFlag = 2;
		encoder_interruptDisable();

		return;
	}

	// anti-clockwise rotation
	if(((GPIOC->PDIR & (1 << 0)) == (1 << 0)) && (GPIOA->PDIR & (1 << 5)) != (1 << 5))
	{
		encoder_impulseCounter[2]++;

		if(encoder_impulseCounter[2] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y + 1] >= 250)
				{
					encoderOut[y + 1] = 255;
				}
				else
				{
					encoderOut[y + 1] += 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y + 1] >= 240)
				{
					encoderOut[y + 1] = 255;
				}
				else
				{
					encoderOut[y + 1] += 20;
				}
			}

			encoder_triggerFlag = 2;
			encoder_interruptDisable();

		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		return;
	}

	// clockwise rotation
	if(((GPIOC->PDIR & (1 << 0)) != (1 << 0)) && (GPIOA->PDIR & (1 << 5)) != (1 << 5))
	{
		encoder_impulseCounter[3]++;

		if(encoder_impulseCounter[3] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y + 1] <= 10)
				{
					encoderOut[y + 1] = 0;
				}
				else
				{
					encoderOut[y + 1] -= 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y + 1] <= 20)
				{
					encoderOut[y + 1] = 0;
				}
				else
				{
					encoderOut[y + 1] -= 20;
				}
			}

			encoder_triggerFlag = 2;
			encoder_interruptDisable();

		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);
		return;
	}

	// Encoder 3 acquisition

	// Push Button
	if((GPIOA->PDIR & (1 << 12)) != (1 << 12))
	{
		if(flashmode[y + 2] == 0)
		{
			encoderOutBuffer[y + 2] = encoderOut[y + 2];
			encoderOut[y + 2] = 255;
			flashmode[y + 2] = 1;
		}
		else if(flashmode[y + 2] == 1)
		{
			encoderOut[y + 2] = encoderOutBuffer[y + 2];
			flashmode[y + 2] = 0;
		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		encoder_triggerFlag = 3;
		encoder_interruptDisable();

		return;
	}

	// anti-clockwise rotation
	if(((GPIOC->PDIR & (1 << 3)) == (1 << 3)) && ((GPIOA->PDIR & (1 << 13)) != (1 << 13)))
	{
		encoder_impulseCounter[4]++;

		if(encoder_impulseCounter[4] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y + 2] >= 250)
				{
					encoderOut[y + 2] = 255;
				}
				else
				{
					encoderOut[y + 2] += 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y + 2] >= 240)
				{
					encoderOut[y + 2] = 255;
				}
				else
				{
					encoderOut[y + 2] += 20;
				}
			}

			encoder_triggerFlag = 3;
			encoder_interruptDisable();
		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		return;
	}

	// clockwise rotation
	if(((GPIOC->PDIR & (1 << 3)) != (1 << 3)) && ((GPIOA->PDIR & (1 << 13)) != (1 << 13)))
	{
		encoder_impulseCounter[5]++;

		if(encoder_impulseCounter[5] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y + 2] <= 10)
				{
					encoderOut[y + 2] = 0;
				}
				else
				{
					encoderOut[y + 2] -= 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y + 2] <= 20)
				{
					encoderOut[y + 2] = 0;
				}
				else
				{
					encoderOut[y + 2] -= 20;
				}
			}

			encoder_triggerFlag = 3;
			encoder_interruptDisable();
		}

		// clear all interrupt flag
		PORTA->PCR[13] |= (1 << 24);

		return;
	}

	// Encoder 4 acquisition

	// Push Button
	if((GPIOA->PDIR & (1 << 16)) != (1 << 16))
	{
		if(flashmode[y + 3] == 0)
		{
			encoderOutBuffer[y + 3] = encoderOut[y + 3];
			encoderOut[y + 3] = 255;
			flashmode[y + 3] = 1;
		}
		else if(flashmode[y + 3] == 1)
		{
			encoderOut[y + 3] = encoderOutBuffer[y + 3];
			flashmode[y + 3] = 0;
		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		encoder_triggerFlag = 4;
		encoder_interruptDisable();

		return;
	}

	// anti-clockwise rotation
	if(((GPIOC->PDIR & (1 << 4)) == (1 << 4)) && (GPIOA->PDIR & (1 << 17)) != (1 << 17))
	{
		encoder_impulseCounter[6]++;

		if(encoder_impulseCounter[6] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y + 3] >= 250)
				{
					encoderOut[y + 3] = 255;
				}
				else
				{
					encoderOut[y + 3] += 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y + 3] >= 240)
				{
					encoderOut[y + 3] = 255;
				}
				else
				{
					encoderOut[y + 3] += 20;
				}
			}

			encoder_triggerFlag = 4;
			encoder_interruptDisable();

		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		return;
	}

	// clockwise rotation
	if(((GPIOC->PDIR & (1 << 4)) != (1 << 4)) && (GPIOA->PDIR & (1 << 17)) != (1 << 17))
	{
		encoder_impulseCounter[7]++;

		if(encoder_impulseCounter[7] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y + 3] <= 10)
				{
					encoderOut[y + 3] = 0;
				}
				else
				{
					encoderOut[y + 3] -= 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y + 3] <= 20)
				{
					encoderOut[y + 3] = 0;
				}
				else
				{
					encoderOut[y + 3] -= 20;
				}
			}

			encoder_triggerFlag = 4;
			encoder_interruptDisable();

		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		return;
	}

	// clear all interrupt flag
	PORTA->PCR[1] |= (1 << 24);
	PORTA->PCR[2] |= (1 << 24);
	PORTA->PCR[4] |= (1 << 24);
	PORTA->PCR[5] |= (1 << 24);
	PORTA->PCR[12] |= (1 << 24);
	PORTA->PCR[13] |= (1 << 24);
	PORTA->PCR[16] |= (1 << 24);
	PORTA->PCR[17] |= (1 << 24);
}

void PORTD_IRQHandler(void)
{
	// Encoder 5 acquisition

	// Push Button
	if((GPIOD->PDIR & (1 << 0)) != (1 << 0))
	{
		if(flashmode[y + 4] == 0)
		{
			encoderOutBuffer[y + 4] = encoderOut[y + 4];
			encoderOut[y + 4] = 255;
			flashmode[y + 4] = 1;
		}
		else if(flashmode[y + 4] == 1)
		{
			encoderOut[y + 4] = encoderOutBuffer[y + 4];
			flashmode[y + 4] = 0;
		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		encoder_triggerFlag = 5;
		encoder_interruptDisable();

		return;
	}

	// anti-clockwise rotation
	if(((GPIOC->PDIR & (1 << 5)) == (1 << 5)) && (GPIOD->PDIR & (1 << 1)) != (1 << 1))
	{
		encoder_impulseCounter[8]++;

		if(encoder_impulseCounter[8] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y + 4] >= 250)
				{
					encoderOut[y + 4] = 255;
				}
				else
				{
					encoderOut[y + 4] += 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y + 4] >= 240)
				{
					encoderOut[y + 4] = 255;
				}
				else
				{
					encoderOut[y + 4] += 20;
				}
			}

			encoder_triggerFlag = 5;
			encoder_interruptDisable();

		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		return;
	}

	// clockwise rotation
	if(((GPIOC->PDIR & (1 << 5)) != (1 << 5)) && (GPIOD->PDIR & (1 << 1)) != (1 << 1))
	{
		encoder_impulseCounter[9]++;

		if(encoder_impulseCounter[9] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y + 4] <= 10)
				{
					encoderOut[y + 4] = 0;
				}
				else
				{
					encoderOut[y + 4] -= 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y + 4] <= 20)
				{
					encoderOut[y + 4] = 0;
				}
				else
				{
					encoderOut[y + 4] -= 20;
				}
			}

			encoder_triggerFlag = 5;
			encoder_interruptDisable();

		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		return;
	}

	// Encoder 6 acquisition

	// Push Button
	if((GPIOD->PDIR & (1 << 2)) != (1 << 2))
	{
		if(flashmode[y + 5] == 0)
		{
			encoderOutBuffer[y + 5] = encoderOut[y + 5];
			encoderOut[y + 5] = 255;
			flashmode[y + 5] = 1;
		}
		else if(flashmode[y + 5] == 1)
		{
			encoderOut[y + 5] = encoderOutBuffer[y + 5];
			flashmode[y + 5] = 0;
		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		encoder_triggerFlag = 6;
		encoder_interruptDisable();

		return;
	}

	// anti-clockwise rotation
	if(((GPIOC->PDIR & (1 << 6)) == (1 << 6)) && (GPIOD->PDIR & (1 << 3)) != (1 << 3))
	{
		encoder_impulseCounter[10]++;

		if(encoder_impulseCounter[10] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y + 5] >= 250)
				{
					encoderOut[y + 5] = 255;
				}
				else
				{
					encoderOut[y + 5] += 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y + 5] >= 240)
				{
					encoderOut[y + 5] = 255;
				}
				else
				{
					encoderOut[y + 5] += 20;
				}
			}

			encoder_triggerFlag = 6;
			encoder_interruptDisable();
		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		return;
	}

	// clockwise rotation
	if(((GPIOC->PDIR & (1 << 6)) != (1 << 6)) && (GPIOD->PDIR & (1 << 3)) != (1 << 3))
	{
		encoder_impulseCounter[11]++;

		if(encoder_impulseCounter[11] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y + 5] <= 10)
				{
					encoderOut[y + 5] = 0;
				}
				else
				{
					encoderOut[y + 5] -= 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y + 5] <= 20)
				{
					encoderOut[y + 5] = 0;
				}
				else
				{
					encoderOut[y + 5] -= 20;
				}
			}

			encoder_triggerFlag = 6;
			encoder_interruptDisable();

		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		return;
	}

	// Encoder 7 acquisition

	// Push Button
	if((GPIOD->PDIR & (1 << 4)) != (1 << 4))
	{
		if(flashmode[y + 6] == 0)
		{
			encoderOutBuffer[y + 6] = encoderOut[y + 6];
			encoderOut[y + 6] = 255;
			flashmode[y + 6] = 1;
		}
		else if(flashmode[y + 6] == 1)
		{
			encoderOut[y + 6] = encoderOutBuffer[y + 6];
			flashmode[y + 6] = 0;
		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		encoder_triggerFlag = 7;
		encoder_interruptDisable();

		return;
	}

	// anti-clockwise rotation
	if(((GPIOC->PDIR & (1 << 10)) == (1 << 10)) && (GPIOD->PDIR & (1 << 5)) != (1 << 5))
	{
		encoder_impulseCounter[12]++;

		if(encoder_impulseCounter[12] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y + 6] >= 250)
				{
					encoderOut[y + 6] = 255;
				}
				else
				{
					encoderOut[y + 6] += 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y + 6] >= 240)
				{
					encoderOut[y + 6] = 255;
				}
				else
				{
					encoderOut[y + 6] += 20;
				}
			}

			encoder_triggerFlag = 7;
			encoder_interruptDisable();
		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);
		return;
	}

	// clockwise rotation
	if(((GPIOC->PDIR & (1 << 10)) != (1 << 10)) && (GPIOD->PDIR & (1 << 5)) != (1 << 5))
	{
		encoder_impulseCounter[13]++;

		if(encoder_impulseCounter[13] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y + 6] <= 10)
				{
					encoderOut[y + 6] = 0;
				}
				else
				{
					encoderOut[y + 6] -= 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y + 6] <= 20)
				{
					encoderOut[y + 6] = 0;
				}
				else
				{
					encoderOut[y + 6] -= 20;
				}
			}
			encoder_triggerFlag = 7;
			encoder_interruptDisable();

		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		return;
	}

	// Encoder 8 acquisition

	// Push Button
	if((GPIOD->PDIR & (1 << 6)) != (1 << 6))
	{
		if(flashmode[y + 7] == 0)
		{
			encoderOutBuffer[y + 7] = encoderOut[y + 7];
			encoderOut[y + 7] = 255;
			flashmode[y + 7] = 1;
		}
		else if(flashmode[y + 7] == 1)
		{
			encoderOut[y + 7] = encoderOutBuffer[y + 7];
			flashmode[y + 7] = 0;
		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		encoder_triggerFlag = 8;
		encoder_interruptDisable();

		return;
	}

	// anti-clockwise rotation
	if(((GPIOC->PDIR & (1 << 11)) == (1 << 11)) && (GPIOD->PDIR & (1 << 7)) != (1 << 7))
	{
		encoder_impulseCounter[14]++;

		if(encoder_impulseCounter[14] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y + 7] >= 250)
				{
					encoderOut[y + 7] = 255;
				}
				else
				{
					encoderOut[y + 7] += 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y + 7] >= 240)
				{
					encoderOut[y + 7] = 255;
				}
				else
				{
					encoderOut[y + 7] += 20;
				}
			}

			encoder_triggerFlag = 8;
			encoder_interruptDisable();
		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		return;
	}

	// clockwise rotation
	if(((GPIOC->PDIR & (1 << 11)) != (1 << 11)) && (GPIOD->PDIR & (1 << 7)) != (1 << 7))
	{
		encoder_impulseCounter[15]++;

		if(encoder_impulseCounter[15] > ENCODER_NUMBER_IMPULSE_THRESHOLD)
		{
			for(int i=0; i < 16; i++)
			{
				encoder_impulseCounter[i] = 0;
			}

			if(fineadjust == 0)
			{
				if(encoderOut[y + 7] <= 10)
				{
					encoderOut[y + 7] = 0;
				}
				else
				{
					encoderOut[y + 7] -= 10;
				}
			}

			if(fineadjust == 1)
			{
				if(encoderOut[y + 7] <= 20)
				{
					encoderOut[y + 7] = 0;
				}
				else
				{
					encoderOut[y + 7] -= 20;
				}
			}

			encoder_triggerFlag = 8;
			encoder_interruptDisable();

		}

		// clear all interrupt flag
		PORTD->PCR[0] |= (1 << 24);
		PORTD->PCR[1] |= (1 << 24);
		PORTD->PCR[2] |= (1 << 24);
		PORTD->PCR[3] |= (1 << 24);
		PORTD->PCR[4] |= (1 << 24);
		PORTD->PCR[5] |= (1 << 24);
		PORTD->PCR[6] |= (1 << 24);
		PORTD->PCR[7] |= (1 << 24);

		return;
	}

	// clear all interrupt flag
	PORTD->PCR[0] |= (1 << 24);
	PORTD->PCR[1] |= (1 << 24);
	PORTD->PCR[2] |= (1 << 24);
	PORTD->PCR[3] |= (1 << 24);
	PORTD->PCR[4] |= (1 << 24);
	PORTD->PCR[5] |= (1 << 24);
	PORTD->PCR[6] |= (1 << 24);
	PORTD->PCR[7] |= (1 << 24);

}



