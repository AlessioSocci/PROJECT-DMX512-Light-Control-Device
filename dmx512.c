/*
 * dmx512.c
 *
 *  Created on: 17 ago 2019
 *      Author: Alessio
 */

#include "dmx512.h"
#include "time.h"

void dmx512_init(void)
{
	PORTE->PCR[22] |= (1 << 10); // ALT2 mode UART1 TX
	PORTE->PCR[23] |= (1 << 10); // ALT2 mode UART1 RX

	SIM->SCGC4 |= (1 << 12); // UART0 clock enable

	UART2->BDH |= (1 << 5); // 2 stop bit

	UART2->BDL |= (1 << 2) | (1 << 1); // 6 = 250000 Baud Rate -> BDH_BDL = BUSCLK / 16 * 250000

	UART2->C2 |= (1 << 3) | (1 << 2); // TX and RX enable
}

void UARTdmx_send(uint8_t dataOut)
{
	 while(!((UART2->S1) & (1 << 7)))
	 {

	 }
	 UART2->D = dataOut;
}

void dmx512_send(uint8_t dataOut[129])
{
	for (uint16_t i = 0; i < 129; i++)
	{
		if (i == 0)
		{
			UART2->C2 |= (1 << 0);

//			for(uint16_t i = 0; i < 200; i++)
//			{
//
//			}

			UART2->C2 &= ~(1 << 0); // send break

//			for(uint16_t i = 0; i < 110; i++)
//			{
//
//			}

			UARTdmx_send(dataOut[i]);
		}
		else
		{
//			for(uint16_t i = 0; i < 180; i++)
//			{
//
//			}
			UARTdmx_send(dataOut[i]);
		}
	}
}
