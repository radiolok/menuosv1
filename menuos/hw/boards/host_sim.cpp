/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/** \file host_sim.cpp  Buttons of the simulated board. */
#include "../hwi.h"

#if defined(MENUOS_BOARD_HOST_SIM)

uint8_t HwButtonsSetup(uint8_t mode){
	(void)mode;
	return BUTTONSMASK;
}

uint8_t HwButtonsState(uint8_t mode){
	(void)mode;
	return HostGetButtons() & BUTTONSMASK;
}

#endif
