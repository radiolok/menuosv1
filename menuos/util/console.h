/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/**
 *  \file console.h
 *  \brief Tiny architecture-independent text output on top of PortConsolePutc()
 *  and a few string helpers that avoid pulling printf() into small MCUs.
 */
#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void console_putc(char c);
void console_puts(const char *s);
/** Print a signed integer in \p base (2..16). */
void console_print(int16_t val, int8_t base);

/**
 *  \brief Convert a signed 16-bit integer to decimal text
 *
 *  \param [in] val value
 *  \param [out] buf at least 7 bytes ("-32768" + zero)
 *  \return number of characters written
 */
uint8_t menuos_itoa(int16_t val, char *buf);

#ifdef __cplusplus
}
#endif

#endif /* CONSOLE_H_ */
