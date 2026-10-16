/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/**
 *  \file msp430_launchpad.h
 *  \brief Generic MSP430x2xx board: MSP-EXP430G2 LaunchPad (MSP430G2553)
 *  or any MSP430F261x board.
 *
 *  - Five buttons on P2.0..P2.4 to GND (internal pull-ups enabled).
 *  - The menu is drawn as text on the USCI_A0 UART ("terminal display"):
 *      MSP430G2xx3: P1.2 = TXD, P1.1 = RXD (LaunchPad back channel, 9600 8N1)
 *      MSP430F261x: P3.4 = TXD, P3.5 = RXD
 *  - Config values are stored in information flash segment D.
 */
#ifndef BOARD_MSP430_LAUNCHPAD_H_
#define BOARD_MSP430_LAUNCHPAD_H_

#if !defined(MENUOS_ARCH_MSP430)
	#error "MSP430 LaunchPad board is MSP430 only"
#endif

#define MENUOS_BOARD_NAME "msp430_launchpad"

#define BUTTONSMASK 0x1F
#define BSLOTS 5

/**Button mask: P2.0 .. P2.4*/
enum{
	BUTTONRETURN = 0x01,
	BUTTONLEFT = 0x02,
	BUTTONDOWN = 0x04,
	BUTTONUP = 0x08,
	BUTTONRIGHT = 0x10
};

#define TERM_DISPLAY
/** Redraw with ANSI "clear screen" so a terminal shows a steady picture. */
#define MENUOS_TERM_ANSI 1
/** The UART is busy with the screen: keep log traces off. */
#define MENUOS_NO_LOG

#define DISPSTRNUMB 5
#define DISPSTRLENGTH 16

#define MAXDEPTH 4

/** Bytes of config storage: one 64-byte info flash segment. */
#define CONFMAXADDRESS 64

#if defined(__MSP430G2553__) || defined(__MSP430G2453__) || defined(__MSP430G2353__) || \
    defined(__MSP430G2253__) || defined(__MSP430G2153__)
	#define BOARD_UART_PINS_INIT() do { \
		P1SEL  |= (BIT1 | BIT2); \
		P1SEL2 |= (BIT1 | BIT2); \
	} while (0)
#elif defined(__MSP430F2618__) || defined(__MSP430F2617__) || defined(__MSP430F2616__) || \
      defined(__MSP430F2619__)
	#define BOARD_UART_PINS_INIT() do { \
		P3SEL |= (BIT4 | BIT5); \
	} while (0)
#else
	#error "MSP430 LaunchPad board: add UART pin setup for this MCU"
#endif

#endif /* BOARD_MSP430_LAUNCHPAD_H_ */
