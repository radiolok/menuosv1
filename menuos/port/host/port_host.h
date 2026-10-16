/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/**
 *  \file port_host.h  Host (PC) port: runs MenuOS as a normal process.
 *
 *  Used by the simulator and by the CI tests. Time does not run by itself:
 *  the harness advances it with HostAdvance(), so tests are deterministic.
 */
#ifndef MENUOS_PORT_HOST_H_
#define MENUOS_PORT_HOST_H_

#define MENUOS_ROM
#define menuos_rom_byte(p)         (*(const uint8_t *)(p))
#define menuos_rom_word(p)         (*(const uint16_t *)(p))
#define menuos_rom_ptr(p)          (*(const char * const *)(p))
#define menuos_rom_strcpy(dst, s)  strcpy((dst), (s))

#ifdef __cplusplus
extern "C" {
#endif

/** Run \p ms milliseconds of simulated time: tick + one scheduler pass per ms. */
void HostAdvance(uint32_t ms);

/** Set the raw buttons state (bit mask of pressed buttons). */
void HostSetButtons(uint8_t mask);

/** Press a button: hold it for \p hold_ms, then release and wait \p gap_ms. */
void HostPress(uint8_t mask, uint32_t hold_ms, uint32_t gap_ms);

/** Read current raw buttons state (used by the host board). */
uint8_t HostGetButtons(void);

/** Enable/disable console output to stdout (log traces). */
void HostConsoleEnable(uint8_t on);

#ifdef __cplusplus
}
#endif

#endif /* MENUOS_PORT_HOST_H_ */
