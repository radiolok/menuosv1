/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/**
 *  \file port.h
 *  \brief Architecture abstraction layer of MenuOS.
 *
 *  Everything MenuOS needs from a particular MCU lives behind this header:
 *   - a 1 ms system tick that calls MTaskTick();
 *   - read-only storage for the menu table (flash on AVR, plain const elsewhere);
 *   - 16-bit non-volatile storage for config values (EEPROM / info flash / RAM);
 *   - a byte console used by the logger and by the terminal display.
 *
 *  The architecture is detected from the compiler; the board (pins, display
 *  type, sizes) is selected in hw/hwdef.h.
 */
#ifndef MENUOS_PORT_H_
#define MENUOS_PORT_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#if defined(__AVR__)
	#define MENUOS_ARCH_AVR 1
	#define MENUOS_ARCH_NAME "avr"
	#include "avr/port_avr.h"
#elif defined(__MSP430__)
	#define MENUOS_ARCH_MSP430 1
	#define MENUOS_ARCH_NAME "msp430"
	#include "msp430/port_msp430.h"
#elif defined(MENUOS_HOST)
	#define MENUOS_ARCH_HOST 1
	#define MENUOS_ARCH_NAME "host"
	#include "host/port_host.h"
#else
	#error "MenuOS: unsupported architecture (expected AVR, MSP430 or -DMENUOS_HOST)"
#endif

/*
 * Each port header must provide:
 *   MENUOS_ROM                 - attribute that places const tables in program memory
 *   menuos_rom_byte(p)         - read uint8_t from MENUOS_ROM
 *   menuos_rom_word(p)         - read uint16_t/int16_t from MENUOS_ROM
 *   menuos_rom_ptr(p)          - read a pointer stored in MENUOS_ROM
 *   menuos_rom_strcpy(dst, s)  - copy a string that lives in MENUOS_ROM
 */

#ifdef __cplusplus
extern "C" {
#endif

/** Clocks, watchdog, console. Called first thing in main(). */
void PortInit(void);

/** Globally enable interrupts. */
void PortIrqEnable(void);

/** Disable interrupts and return the previous state. */
uint8_t PortIrqSave(void);

/** Restore interrupt state returned by PortIrqSave(). */
void PortIrqRestore(uint8_t state);

/** Start the 1 ms tick. The tick ISR must call MTaskTick(). */
void PortTickStart(void);

/** Stop the 1 ms tick. */
void PortTickStop(void);

/** Put one byte to the debug console (UART, stdout...). */
void PortConsolePutc(char c);

/** Read a 16-bit config cell. \p addr is a byte address, always even. */
uint16_t PortNvRead16(uint16_t addr);

/** Write a 16-bit config cell. \p addr is a byte address, always even. */
void PortNvWrite16(uint16_t addr, uint16_t value);

/** Called by the scheduler from the main loop between passes. */
void PortIdle(void);

/** Implemented by the scheduler (MTask.cpp); the port calls it every 1 ms. */
void MTaskTick(void);

/** Implemented by the scheduler: one pass of the main loop (buttons + due tasks). */
void MTaskPoll(void);

#ifdef __cplusplus
}
#endif

#endif /* MENUOS_PORT_H_ */
