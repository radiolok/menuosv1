/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#ifndef HW_DEF_H_
#define HW_DEF_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "../port/port.h"

/*===================================================================
					BOARD SELECTION
  Pick a board with -DMENUOS_BOARD_xxx, otherwise a default board
  is used for the detected architecture.
===================================================================*/
#if !defined(MENUOS_BOARD_RLN_ELECTRO_2012) && \
    !defined(MENUOS_BOARD_MSP430_LAUNCHPAD) && \
    !defined(MENUOS_BOARD_HOST_SIM)
	#if defined(MENUOS_ARCH_AVR)
		#define MENUOS_BOARD_RLN_ELECTRO_2012
	#elif defined(MENUOS_ARCH_MSP430)
		#define MENUOS_BOARD_MSP430_LAUNCHPAD
	#else
		#define MENUOS_BOARD_HOST_SIM
	#endif
#endif

#if defined(MENUOS_BOARD_RLN_ELECTRO_2012)
	#include "boards/rln_electro_2012.h"
#elif defined(MENUOS_BOARD_MSP430_LAUNCHPAD)
	#include "boards/msp430_launchpad.h"
#elif defined(MENUOS_BOARD_HOST_SIM)
	#include "boards/host_sim.h"
#endif

/*
 * A board header must define:
 *   BSLOTS                      number of buttons (1..8)
 *   BUTTONRETURN..BUTTONDOWN    button masks, each equals (1 << slot)
 *   one of TEXT_DISPLAY / GRAPH_DISPLAY / TERM_DISPLAY
 *   DISPSTRNUMB, DISPSTRLENGTH  visible menu rows and their length
 *   MAXDEPTH                    max menu nesting depth
 *   CONFMAXADDRESS              size of config storage in bytes
 */
#if !defined(BSLOTS) || !defined(DISPSTRNUMB) || !defined(DISPSTRLENGTH) || \
    !defined(MAXDEPTH) || !defined(CONFMAXADDRESS)
	#error "MenuOS: board header is incomplete"
#endif

/**button slot number
 *  NOTICE: RIGHT and ENTER has the same slot by default! MenuOS do not use ENTER button!
 */
enum{
	BUTTONSLOTRETURN = 0,
	BUTTONSLOTLEFT = 1,
	BUTTONSLOTRIGHT = 2,
	BUTTONSSLOTUP = 3,
	BUTTONSLOTDOWN = 4
};

#define FILEREW  4//bytes for the file

enum{
	FILETYPE,
	FILEPARENT,
	FILEMODE1,
	FILEMODE2
};

//used MTask slots
#define MENUSLOT 0

#define CONFARRAYWIDTH 3

enum {
	CONFVALUE,
	CONFMIN,
	CONFMAX
};

#define PROGVERSIONTEXT ("v1.01") //current program version string

#endif /* HW_DEF_H_ */
