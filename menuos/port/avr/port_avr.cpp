/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/** \file port_avr.cpp  AVR implementation of the MenuOS port API. */
#include "../port.h"

#if defined(MENUOS_ARCH_AVR)

#include "../../util/uart.h"

void PortInit(void){
	uart_init(UART_BAUD_SELECT(MENUOS_CONSOLE_BAUD, F_CPU));
}

void PortIrqEnable(void){
	sei();
}

uint8_t PortIrqSave(void){
	uint8_t sreg = SREG;
	cli();
	return sreg;
}

void PortIrqRestore(uint8_t state){
	SREG = state;
}

/*
 * 1 ms tick.
 * ATmega640/1280/2560: Timer5, fast PWM 8-bit, prescaler 64, reloaded with
 * TIMETICK on every overflow (this is the original MenuOS v1 timing).
 * Other parts: Timer1 in CTC mode, prescaler 64.
 */
#if defined(TCCR5A)

#define TIMETICK (256 - (F_CPU / 64UL / 1000UL))

void PortTickStart(void){
	TCCR5A |= (1 << WGM50);
	TCCR5B |= (1 << WGM52);
	TCNT5 = TIMETICK;
	TCCR5B |= (1 << CS51) | (1 << CS50);
	TIMSK5 |= (1 << TOIE5);
}

void PortTickStop(void){
	TCCR5B &= ~((1 << CS51) | (1 << CS50));
	TIMSK5 &= ~(1 << TOIE5);
}

ISR(TIMER5_OVF_vect){
	TCNT5 = TIMETICK;
	MTaskTick();
}

#else

void PortTickStart(void){
	TCCR1A = 0;
	TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
	OCR1A = (uint16_t)(F_CPU / 64UL / 1000UL - 1);
	TIMSK1 |= (1 << OCIE1A);
}

void PortTickStop(void){
	TCCR1B &= ~((1 << CS11) | (1 << CS10));
	TIMSK1 &= ~(1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect){
	MTaskTick();
}

#endif

void PortConsolePutc(char c){
	uart_putc((unsigned char)c);
}

uint16_t PortNvRead16(uint16_t addr){
	return eeprom_read_word((const uint16_t *)addr);
}

void PortNvWrite16(uint16_t addr, uint16_t value){
	eeprom_update_word((uint16_t *)addr, value);
}

void PortIdle(void){
}

#endif /* MENUOS_ARCH_AVR */
