/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/**
 *  \file rln_electro_2012.h
 *  \brief "RLN Electro 2012 SE" board: ATmega1280/2560, KS0108 128x64 GLCD,
 *  five buttons on PORTB[4:0] (active low), DS1307 RTC on TWI.
 *  This is the board MenuOS v1 was originally written for.
 */
#ifndef BOARD_RLN_ELECTRO_2012_H_
#define BOARD_RLN_ELECTRO_2012_H_

#if !defined(MENUOS_ARCH_AVR)
	#error "RLN Electro 2012 board is AVR only"
#endif

#define RLN_ELECTRO_2012_SE
#define MENUOS_BOARD_NAME "rln_electro_2012"

//buttons are connected to this MCU port
#define BUTTONSDDR DDRB
#define BUTTONSPORT PORTB
#define BUTTONSPIN PINB
#define BUTTONSMASK 0x1F
#define BSLOTS 5

/**Button mask*/
enum{
	BUTTONRETURN = 0x01,
	BUTTONLEFT = 0x02,
	BUTTONDOWN = 0x04,
	BUTTONUP = 0x08,
	BUTTONRIGHT = 0x10
};

//configured LCD type
//#define TEXT_DISPLAY
#define GRAPH_DISPLAY

/** Board has a DS1307 RTC: enables the "Date and Time" app. */
#define MENUOS_HAS_RTC_CLOCK

#define DISPSTRNUMB 5//display strings number.
#define DISPSTRLENGTH 16 //string length

#define MAXDEPTH 4//max menu depth

//define store memory space (EEPROM bytes)
#define CONFMAXADDRESS 4096

#endif /* BOARD_RLN_ELECTRO_2012_H_ */
