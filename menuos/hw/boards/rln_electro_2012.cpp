/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/** \file rln_electro_2012.cpp  Buttons of the RLN Electro 2012 board. */
#include "../hwi.h"

#if defined(MENUOS_BOARD_RLN_ELECTRO_2012)

uint8_t HwButtonsSetup(uint8_t mode){
	(void)mode;
	BUTTONSDDR &= ~BUTTONSMASK;//input
	BUTTONSPORT |= BUTTONSMASK;//pull-up
	return BUTTONSMASK;
}

uint8_t HwButtonsState(uint8_t mode){
	(void)mode;
	//inverted, because pressed button return zero.
	return (~BUTTONSPIN) & BUTTONSMASK;
}

#endif
