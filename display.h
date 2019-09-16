/*
 * display.h
 *
 *  Created on: 17 ago 2019
 *      Author: Alessio
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"

#include "time.h"


// OLED 128 x 64 Driver SSD1306
#define SSD1306_SETADDRESSINGMODE          			0x20
#define SSD1306_HORIZONTALADDRESSINGMODE         	0x00
#define SSD1306_VERTICALADDRESSINGMODE          	0x01
#define SSD1306_PAGEADDRESSINGMODE          		0x02

#define SSD1306_SETCOLUMNADDRESS          			0x21
#define SSD1306_SETPAGEADDRESS             			0x22
#define SSD1306_SETSTARTADDRESSATPAGE0             	0xB0
#define SSD1306_SETSTARTADDRESSATPAGE7             	0xB7

#define SSD1306_DEACTIVATESCROLL           			0x2E
#define SSD1306_ACTIVATESCROLL             			0x2F
#define SSD1306_SETDISPLAYSTARTLINE        			0x40 // to 0x7F
#define SSD1306_SETCONTRAST                			0x81
#define SSD1306_CHARGEPUMP                 			0x8D
#define SSD1306_SEGMENTREMAP_0_to_127  	   			0xA0
#define SSD1306_SEGMENTREMAP_127_to_0              	0xA1
#define SSD1306_DISPLAYALLONRESUME         			0xA4
#define SSD1306_DISPLAYALLON               			0xA5
#define SSD1306_DISPLAYNORMAL              			0xA6
#define SSD1306_DISPLAYINVERSE             			0xA7
#define SSD1306_SETMUXRATIO                			0xA8
#define SSD1306_DISPLAYOFF                 			0xAE
#define SSD1306_DISPLAYON                  			0xAF
#define SSD1306_COMSCANDIRECTION_N_to_0         	0xC0
#define SSD1306_COMSCANDIRECTION_0_to_N			   	0xC8
#define SSD1306_SETDISPLAYOFFSET           			0xD3
#define SSD1306_SETDISPLAYCLK              			0xD5
#define SSD1306_COMPINS                    			0xDA
#define SSD1306_SETDESELECTLEVEL           			0xDB
#define SSD1306_SETPRECHARGE						0xD9
#define SSD1306_EXTERNAL_PRECHARGE					0x22
#define SSD1306_INTERNAL_PRECHARGE					0xF1

#define SSD1306_bufferLenght						(8*128)
#define SSD1306_sendOneCommand 						0x00
#define SSD1306_sendMoreThanOneCommand 				0x80
#define SSD1306_sendOneData 						0x40
#define SSD1306_sendMoreThanOneData					0xC0


void display_clearAllPixel(void);
void display_clearPixel(uint8_t xPos, uint8_t yPos);
void display_drawPixel (uint8_t xPos, uint8_t yPos);
void display_flushPart(uint8_t xPosStart, uint8_t xPosStop, uint8_t yPosStart, uint8_t yPosStop);
void display_drawPicture (uint8_t bitmap[]);
void display_drawChar (uint16_t xPos, uint16_t yPos, uint8_t c);
void display_init(uint32_t inputBaudRate);
void display_turnOn(void);
void display_turnOff(void);

void display_drawBar (uint8_t position, uint8_t value);
void display_clearBar (uint8_t position);
void display_drawNumber(uint8_t position, uint8_t number);

#endif /* DISPLAY_H_ */
