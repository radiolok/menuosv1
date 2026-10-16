/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/** \file port_msp430.h  MSP430x2xx (msp430-elf-gcc / TI MSP430-GCC) definitions. */
#ifndef MENUOS_PORT_MSP430_H_
#define MENUOS_PORT_MSP430_H_

#include <msp430.h>

/* Flash is directly addressable on MSP430: no special access needed. */
#define MENUOS_ROM
#define menuos_rom_byte(p)         (*(const uint8_t *)(p))
#define menuos_rom_word(p)         (*(const uint16_t *)(p))
#define menuos_rom_ptr(p)          (*(const char * const *)(p))
#define menuos_rom_strcpy(dst, s)  strcpy((dst), (s))

/** MCLK frequency. DCO is set from the factory 16 MHz calibration. */
#define MENUOS_MCLK_HZ  16000000UL
/** SMCLK = MCLK / 2. Feeds Timer_A, USCI_A0 and the flash controller. */
#define MENUOS_SMCLK_HZ 8000000UL

/** Console baud rate on USCI_A0 (9600 also works over the LaunchPad back channel). */
#ifndef MENUOS_CONSOLE_BAUD
	#define MENUOS_CONSOLE_BAUD 9600UL
#endif

#endif /* MENUOS_PORT_MSP430_H_ */
