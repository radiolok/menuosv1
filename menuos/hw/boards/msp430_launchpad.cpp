/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/** \file msp430_launchpad.cpp  Buttons of the MSP430 LaunchPad board (P2.0..P2.4). */
#include "../hwi.h"

#if defined(MENUOS_BOARD_MSP430_LAUNCHPAD)

uint8_t HwButtonsSetup(uint8_t mode){
	(void)mode;
	P2SEL &= ~BUTTONSMASK;
	P2DIR &= ~BUTTONSMASK;   //input
	P2REN |= BUTTONSMASK;    //resistor on
	P2OUT |= BUTTONSMASK;    //...as pull-up
	return BUTTONSMASK;
}

uint8_t HwButtonsState(uint8_t mode){
	(void)mode;
	//inverted, because pressed button return zero.
	return (uint8_t)(~P2IN) & BUTTONSMASK;
}

#endif
