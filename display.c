/*
 * display.c
 *
 *  Created on: 17 ago 2019
 *      Author: Alessio
 */

#include "display.h"

#include "picture.h"

#define WAITING_TIME 	150

// OLED 128 x 64 Driver SSD1306
uint8_t SSD1306_buffer [SSD1306_bufferLenght];
uint8_t SSD1306_data = 0;

uint8_t SSD1306_xPositionStart = 0;
uint8_t SSD1306_xPositionStop = 0;
uint8_t SSD1306_yPositionStart = 0;
uint8_t SSD1306_yPositionStop = 0;

uint32_t currentTicks = 0;

static uint16_t icrTab[] = {20, 22, 24, 26, 28, 30, 34, 40, 28, 32, 36, 40, 44, 48, 56, 68, 48, 56, 64, 72, 80, 88, 104, 128, 80, 96, 112, 128,
	144, 160, 192, 240, 160, 192, 224, 256, 288, 320, 384, 480, 320, 384, 448, 512, 576, 640, 768, 960, 640, 768, 896, 1024, 1152, 1280, 1536,
	1920, 1280, 1536,  1792, 2048, 2304, 2560, 3072, 3840};

typedef enum _SSD1306_state
{
	init,
	display_on,
	display_off,
	set_xPosition_forFlushPart,
	set_yPosition_forFlushPart,
	flushPart,
	flush,
}SSD1306_state;

SSD1306_state SSD1306state;


void I2C0_IRQHandler(void)
{
	if (SSD1306state == init)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_DISPLAYOFF; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETDISPLAYCLK; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0x80; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETMUXRATIO; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0x3F; // Send Instruction (display high - 1)
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETDISPLAYOFFSET; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0x00; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete



			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETDISPLAYSTARTLINE; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_CHARGEPUMP; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0x14; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETADDRESSINGMODE; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0x00; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


//			time_delay(5);
//			currentTicks = ticks;
//			I2C0->D = SSD1306_sendMoreThanOneCommand;
//			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete
//
//			time_delay(5);
//			currentTicks = ticks;
//			I2C0->D = SSD1306_SEGMENTREMAP | 0x01; // Send Instruction
//			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


//			time_delay(5);
//			currentTicks = ticks;
//			I2C0->D = SSD1306_sendMoreThanOneCommand;
//			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete
//
//			time_delay(5);
//			currentTicks = ticks;
//			I2C0->D = SSD1306_COMSCANDIRECTIONDOWN; // Send Instruction
//			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_COMPINS; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0x12; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETCONTRAST; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0xFF; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETPRECHARGE; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0xF1; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETDESELECTLEVEL; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0x40; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_DISPLAYALLONRESUME; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_DISPLAYNORMAL; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag
		}
		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}

	if (SSD1306state == display_on)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_DISPLAYON; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag
		}

		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}

	if (SSD1306state == display_off)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_DISPLAYOFF; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag
		}

		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}

	if (SSD1306state == set_xPosition_forFlushPart)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D =  SSD1306_SETPAGEADDRESS ; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_yPositionStart; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETCOLUMNADDRESS; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_xPositionStart; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag
		}
		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}

	if (SSD1306state == set_yPosition_forFlushPart)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D =  SSD1306_SETPAGEADDRESS ; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_yPositionStart; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag
		}
		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}

	}


	if (SSD1306state == flushPart)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
//			for(int i = SSD1306_xPositionStart + (SSD1306_yPositionStart / 8); i < (SSD1306_xPositionStop * (SSD1306_yPositionStop / 8)); i++)
//			for(int i = 0; i < (SSD1306_xPositionStop - SSD1306_xPositionStart) * (((SSD1306_yPositionStop - SSD1306_yPositionStart) / 8) + 1); i++)
//			{
//				currentTicks = ticks;
//				I2C0->D = SSD1306_sendMoreThanOneData;
//				while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete
//
//				currentTicks = ticks;
//				I2C0->D = 0xFF;// SSD1306_buffer[i]; // Send Instruction
//				while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete
//			}

			currentTicks = ticks;
			I2C0->D = SSD1306_sendOneData;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			currentTicks = ticks;
			I2C0->D = 0xFF;// SSD1306_buffer[i]; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag

		}
		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}

	if(SSD1306state == flush)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			for (uint16_t i = 0; i < (SSD1306_bufferLenght - 1); ++i)
			{
				SSD1306_data = SSD1306_buffer[i];

				currentTicks = ticks;
				I2C0->D = SSD1306_sendMoreThanOneData;
				while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

				currentTicks = ticks;
				I2C0->D = SSD1306_data; // Send Instruction
				while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			}

			SSD1306_data = SSD1306_buffer[SSD1306_bufferLenght];

			currentTicks = ticks;
			I2C0->D = SSD1306_sendOneData;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			currentTicks = ticks;
			I2C0->D = SSD1306_data; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag

			return;
		}
		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}
}

void I2C_sendStart(SSD1306_state state)
{
	SSD1306state = state;

	time_delay(5);

	I2C0->C1 |= (1 << 5) | (1 << 4); // Start
	I2C0->D = 0x3C << 1; // Send Slave Address 0x3C << 1

	currentTicks = ticks;
	while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete
}

void display_clearAllPixel(void)
{
	time_delay(50);

	for(uint16_t i = 0; i < SSD1306_bufferLenght; ++i)
	{
		SSD1306_buffer[i] = 0;
	}
}

void display_clearPixel(uint8_t xPos, uint8_t yPos)
{
	uint16_t pos = xPos + ((yPos / 8) * 128);

	SSD1306_buffer[pos] = 0;
}

void display_drawPixel(uint8_t xPos, uint8_t yPos)
{
	uint16_t pos = xPos + ((yPos / 8) * 128);

	SSD1306_buffer[pos] |= (1 << (yPos % 8));
}

void display_flush (void)
{
	I2C_sendStart(flush);
}

void display_flushPart(uint8_t xPosStart, uint8_t xPosStop, uint8_t yPosStart, uint8_t yPosStop)
{
	time_delay(50);

	SSD1306_xPositionStart = xPosStart;

	time_delay(50);
//
//	SSD1306_xPositionStop = xPosStop;
//
//	time_delay(50);

	SSD1306_yPositionStart = yPosStart / 8;

	time_delay(50);
//
//	SSD1306_yPositionStop = yPosStop / 8;
//
//	time_delay(50);

//	I2C_sendStart(set_yPosition_forFlushPart);

	I2C_sendStart(set_xPosition_forFlushPart);

	I2C_sendStart(flushPart);
}


void display_drawPicture(uint8_t bitmap[SSD1306_bufferLenght])
{
	for (uint16_t i = 0; i <= SSD1306_bufferLenght; ++i)
	{
		SSD1306_buffer[i] = bitmap[i];
	}

	display_flush();
}

void display_drawChar(uint16_t xPos, uint16_t yPos, uint8_t c)
{
	uint16_t pos = xPos + ((yPos / 8) * 128);

	for (uint8_t i = 0; i < 6; i++)
	{
		SSD1306_buffer[pos + i]= numberFont[c][i];
	}
}

void display_turnOn(void)
{
	I2C_sendStart(display_on);

}
void display_turnOff(void)
{
	I2C_sendStart(display_off);
}

void display_init(uint32_t inputBaudRate)
{
	// I2C (OLED 128x64 driver SSD1306)

	time_delay(200);

	PORTC->PCR[8] |= (1 << 9); // I2C SCL (alt2 mode)
	PORTC->PCR[9] |= (1 << 9); // I2C SDA (alt2 mode)

	NVIC->IP[2] = (1 << 7); // Preemptive Priority (2), one less than PIT to allow usage of delay into ISR

	SIM->SCGC4 |= (1 << 6); // CLock on I2C0 Module

	uint32_t newError;
	uint32_t oldError = 0xFFFF;
	uint8_t resultIcr = 0xFF;
	uint8_t icrTabValue = 0;
	uint32_t resultBaudRate = 0;

	for(icrTabValue = 0; icrTabValue < 64; icrTabValue++) // compute icr value
	{
		resultBaudRate = 60000000 / icrTab[icrTabValue]; // supposing mul fixed to 0

		newError = inputBaudRate - resultBaudRate;

		if(newError < oldError)
		{
			oldError = newError;
			resultIcr = icrTabValue;
		}

		else if(newError > oldError)
		{
			// wrong inputBaudRate, fix this blocking state !
		}
	}

	uint8_t regValue = resultIcr & 0b00111111;

	I2C0->F = regValue; // Set Baud Rate

	I2C0->C1 |= (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4); // I2C0 Enable, Interrupt Enable, Master Mode, TX Mode

	NVIC->ISER[0] |= (1 << 8);

	time_delay(100);

	I2C_sendStart(init);

	time_delay(100);

	display_clearAllPixel();

	time_delay(100);

	display_flush();

	time_delay(100);

	display_turnOn();
}


void display_drawBar(uint8_t position, uint8_t value)
{
	for(int j = 0; j < (value / 4); j++)
	{
		switch(position)
		{
			case 1:
				for (uint8_t i = 0; i < 10; i++)
				{
					display_drawPixel(3 + i, 40 - j);
				}
				break;

			case 2:
				for (uint8_t i = 0; i < 11; i++)
				{
					display_drawPixel(18 + i, 40 - j);
				}
				break;

			case 3:
				for (uint8_t i = 0; i < 11; i++)
				{
					display_drawPixel(34 + i, 40 - j);
				}
				break;

			case 4:
				for (uint8_t i = 0; i < 11; i++)
				{
					display_drawPixel(50 + i, 40 - j);
				}
				break;

			case 5:
				for (uint8_t i = 0; i < 11; i++)
				{
					display_drawPixel(66 + i, 40 - j);
				}
				break;

			case 6:
				for (uint8_t i = 0; i < 11; i++)
				{
					display_drawPixel(82 + i, 40 - j);
				}
				break;

			case 7:
				for (uint8_t i = 0; i < 11; i++)
				{
					display_drawPixel(98 + i, 40 - j);
				}
				break;

			case  8:
				for (uint8_t i = 0; i < 11; i++)
				{
					display_drawPixel(114 + i, 40 - j);
				}
				break;
		}
	}
}

void display_clearBar(uint8_t position)
{
	for(int j = 2; j < 37; j++)
	{
		switch(position)
		{
			case 1:
				for (uint8_t i = 0; i < 10; i++)
				{
					display_clearPixel(3 + i, 39 - j);
				}
				break;

			case 2:
				for (uint8_t i = 0; i < 11; i++)
				{
					display_clearPixel(18 + i, 39 - j);
				}
				break;

			case 3:
				for (uint8_t i = 0; i < 11; i++)
				{
					display_clearPixel(34 + i, 39 - j);
				}
				break;

			case 4:
				for (uint8_t i = 0; i < 11; i++)
				{
					display_clearPixel(50 + i, 39 - j);
				}
				break;

			case 5:
				for (uint8_t i = 0; i < 11; i++)
				{
					display_clearPixel(66 + i, 39 - j);
				}
				break;

			case 6:
				for (uint8_t i = 0; i < 11; i++)
				{
					display_clearPixel(82 + i, 39 - j);
				}
				break;

			case 7:
				for (uint8_t i = 0; i < 11; i++)
				{
					display_clearPixel(98 + i, 39 - j);
				}
				break;

			case  8:
				for (uint8_t i = 0; i < 11; i++)
				{
					display_clearPixel(114 + i, 39 - j);
				}
				break;
		}
	}
}

void display_drawNumber(uint8_t position, uint8_t number)
{
	uint8_t dozen_of_number = number / 10;
	uint8_t unit_of_number = number - (dozen_of_number * 10);

	switch(position)
	{
		case 1:

			if(number < 10)
			{
				display_drawChar(2, 56, 10);
				display_drawChar(6, 56, 10);
				display_drawChar(5, 56, number);
			}

			else if(number >= 10)
			{
				display_drawChar(2, 56, 10);
				display_drawChar(6, 56, 10);
				display_drawChar(2, 56, dozen_of_number);
				display_drawChar(6, 56, unit_of_number);
			}

			break;

		case 2:
			if(number < 10)
			{
				display_drawChar(18, 56, 10);
				display_drawChar(21, 56, 10);
				display_drawChar(20, 56, number);
			}

			else if(number >= 10)
			{
				display_drawChar(18, 56, 10);
				display_drawChar(21, 56, 10);
				display_drawChar(18, 56, dozen_of_number);
				display_drawChar(21, 56, unit_of_number);
			}

			break;

		case 3:

			if(number < 10)
			{
				display_drawChar(35, 56, 10);
				display_drawChar(38, 56, 10);
				display_drawChar(37, 56, number);
			}
			else if(number >= 10)
			{
				display_drawChar(35, 56, 10);
				display_drawChar(38, 56, 10);
				display_drawChar(35, 56, dozen_of_number);
				display_drawChar(38, 56, unit_of_number);
			}

			break;

		case 4:

			if(number < 10)
			{
				display_drawChar(50, 56, 10);
				display_drawChar(53, 56, 10);
				display_drawChar(52, 56, number);
			}
			else if(number >= 10)
			{
				display_drawChar(50, 56, 10);
				display_drawChar(53, 56, 10);
				display_drawChar(50, 56, dozen_of_number);
				display_drawChar(53, 56, unit_of_number);
			}

			break;

		case 5:
			if(number < 10)
			{
				display_drawChar(66, 56, 10);
				display_drawChar(69, 56, 10);
				display_drawChar(68, 56, number);
			}
			else if(number >= 10)
			{
				display_drawChar(66, 56, 10);
				display_drawChar(69, 56, 10);
				display_drawChar(66, 56, dozen_of_number);
				display_drawChar(69, 56, unit_of_number);
			}

			break;

		case 6:
			if(number < 10)
			{
				display_drawChar(82, 56, 10);
				display_drawChar(85, 56, 10);
				display_drawChar(84, 56, number);
			}
			else if(number >= 10)
			{
				display_drawChar(82, 56, 10);
				display_drawChar(85, 56, 10);
				display_drawChar(82, 56, dozen_of_number);
				display_drawChar(85, 56, unit_of_number);
			}

			break;

		case 7:

			if(number < 10)
			{
				display_drawChar(98, 56, 10);
				display_drawChar(101, 56, 10);
				display_drawChar(100, 56, number);
			}
			else if(number >= 10)
			{
				display_drawChar(98, 56, 10);
				display_drawChar(101, 56, 10);
				display_drawChar(98, 56, dozen_of_number);
				display_drawChar(101, 56, unit_of_number);
			}

			break;

		case  8:

			if(number < 10)
			{
				display_drawChar(114, 56, 10);
				display_drawChar(117, 56, 10);
				display_drawChar(116, 56, number);
			}
			else if(number >= 10)
			{
				display_drawChar(114, 56, 10);
				display_drawChar(117, 56, 10);
				display_drawChar(114, 56, dozen_of_number);
				display_drawChar(117, 56, unit_of_number);
			}

			break;
	}
}
