/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/**
 *  \file host_sim.h
 *  \brief Simulated board for the host build: buttons are set by the
 *  harness, the menu is drawn into a text frame buffer.
 */
#ifndef BOARD_HOST_SIM_H_
#define BOARD_HOST_SIM_H_

#if !defined(MENUOS_ARCH_HOST)
	#error "Host simulator board is for the host build only"
#endif

#define MENUOS_BOARD_NAME "host_sim"

#define BUTTONSMASK 0x1F
#define BSLOTS 5

enum{
	BUTTONRETURN = 0x01,
	BUTTONLEFT = 0x02,
	BUTTONDOWN = 0x04,
	BUTTONUP = 0x08,
	BUTTONRIGHT = 0x10
};

#define TERM_DISPLAY

#define DISPSTRNUMB 5
#define DISPSTRLENGTH 16

#define MAXDEPTH 4

#define CONFMAXADDRESS 64

#endif /* BOARD_HOST_SIM_H_ */
