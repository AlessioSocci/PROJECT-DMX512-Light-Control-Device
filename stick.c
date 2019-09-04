/*
 * stick.c
 *
 *  Created on: 17 ago 2019
 *      Author: Alessio
 */


#include "stick.h"
#include "time.h"

uint8_t adc_interruptFlag = 0;
uint8_t stick_triggerFlag = 0;

void stick_init(void)
{
	// ADC

    SIM->SCGC6 |= (1 << 27); // enable clock for ADC0

    PORTB->PCR[1] |= 0;  // PORTB pin1 as ADC default mode
    PORTB->PCR[2] |= 0;  // PORTB pin2 as ADC default mode

    ADC0->CFG1 |= (1 << 7) | (1 << 6) | (1 << 5); // low power mode on, freq. of sampling set as input clock / 8, quantization bit is 8

    ADC0->SC3 = 0; /* ADC0 SC3 */

    // Press Button

    GPIOB->PDDR &= ~(1 << 0); // PORTB pin 0 set as input

    PORTB->PCR[0] = (1 << 8) | (1 << 1) | (1 << 0); // PTB1 in Alt 0 (GPIO mode), Pull Enable, Pull Up select ;
}

uint16_t stick_panRead(void)
{
	uint16_t dataADC = 0;

	/* B2 is ch 12 of ADC0, conversion start */
	ADC0->SC1[0] = (1 << 3) | (1 << 2);

	/* Check for conversion is complete or not*/
	while((ADC0->SC1[0] & (1<<7)) == 0)
	{

	}

	dataADC = ADC0->R[0];
	dataADC = (dataADC * 3300) / 255;

	return dataADC;
}

uint16_t stick_tiltRead(void)
{
	uint16_t dataADC = 0;

	/* B1 is ch 9 of ADC0, conversion start */
	ADC0->SC1[0] = (1 << 3) | (1 << 0);

	/* Check for conversion is complete or not*/
	while((ADC0->SC1[0] & (1<<7)) == 0)
	{

	}

	dataADC = ADC0->R[0];
	dataADC = (dataADC * 3300) / 255;

	return dataADC;
}

uint8_t stick_pushRead(void)
{
	if (((GPIOB->PDIR & (1 << 0)) == (1 << 0)))
	{
		return 0;
	}
	else
	{
		return 1;
	}

}
