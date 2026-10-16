/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/** \file port_host.cpp  Host implementation of the MenuOS port API. */
#include "../port.h"

#if defined(MENUOS_ARCH_HOST)

#include <stdio.h>
#include "../../hw/hwdef.h"

static uint8_t tickRunning = 0;
static uint8_t buttons = 0;
static uint8_t consoleOn = 0;
static uint16_t nv[CONFMAXADDRESS / 2];

void PortInit(void){
	for (uint16_t i = 0; i < CONFMAXADDRESS / 2; i++){
		nv[i] = 0xFFFF;   /* like an erased EEPROM */
	}
}

void PortIrqEnable(void){
}

uint8_t PortIrqSave(void){
	return 0;
}

void PortIrqRestore(uint8_t state){
	(void)state;
}

void PortTickStart(void){
	tickRunning = 1;
}

void PortTickStop(void){
	tickRunning = 0;
}

void PortConsolePutc(char c){
	if (consoleOn){
		fputc(c, stderr);
	}
}

uint16_t PortNvRead16(uint16_t addr){
	if ((addr / 2) >= CONFMAXADDRESS / 2){
		return 0xFFFF;
	}
	return nv[addr / 2];
}

void PortNvWrite16(uint16_t addr, uint16_t value){
	if ((addr / 2) < CONFMAXADDRESS / 2){
		nv[addr / 2] = value;
	}
}

void PortIdle(void){
}

void HostAdvance(uint32_t ms){
	while (ms--){
		if (tickRunning){
			MTaskTick();
		}
		MTaskPoll();
	}
}

void HostSetButtons(uint8_t mask){
	buttons = mask;
}

uint8_t HostGetButtons(void){
	return buttons;
}

void HostPress(uint8_t mask, uint32_t hold_ms, uint32_t gap_ms){
	HostSetButtons(mask);
	HostAdvance(hold_ms);
	HostSetButtons(0);
	HostAdvance(gap_ms);
}

void HostConsoleEnable(uint8_t on){
	consoleOn = on;
}

#endif /* MENUOS_ARCH_HOST */
