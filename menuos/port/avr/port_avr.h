/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/** \file port_avr.h  AVR (avr-gcc + avr-libc) definitions. */
#ifndef MENUOS_PORT_AVR_H_
#define MENUOS_PORT_AVR_H_

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#define MENUOS_ROM PROGMEM
#define menuos_rom_byte(p)         pgm_read_byte(p)
#define menuos_rom_word(p)         pgm_read_word(p)
#define menuos_rom_ptr(p)          ((const char *)pgm_read_word(p))
#define menuos_rom_strcpy(dst, s)  strcpy_P((dst), (s))

/** Console baud rate (Fleury UART driver, USART0). */
#ifndef MENUOS_CONSOLE_BAUD
	#define MENUOS_CONSOLE_BAUD 38400UL
#endif

#endif /* MENUOS_PORT_AVR_H_ */
