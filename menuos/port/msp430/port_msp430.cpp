/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/**
 *  \file port_msp430.cpp  MSP430x2xx implementation of the MenuOS port API.
 *
 *  Clocks: DCO 16 MHz (factory calibration) -> MCLK, SMCLK = 8 MHz.
 *  Tick:   Timer_A CCR0, SMCLK/8, up mode, 1 ms.
 *  Console: USCI_A0 UART, pins are selected by the board header.
 *  Config storage: information flash segment D (64 bytes at 0x1000),
 *  mirrored in RAM and rewritten only when a value actually changes.
 */
#include "../port.h"

#if defined(MENUOS_ARCH_MSP430)

#include "../../hw/hwdef.h"

#define TICK_DIV       8UL
#define TICK_CCR0      (MENUOS_SMCLK_HZ / TICK_DIV / 1000UL - 1UL)

#define UART_N         (MENUOS_SMCLK_HZ / MENUOS_CONSOLE_BAUD)
#define UART_BRS       ((MENUOS_SMCLK_HZ * 8UL / MENUOS_CONSOLE_BAUD) - UART_N * 8UL)

/* Flash timing generator must run at 257..476 kHz: 8 MHz / 25 = 320 kHz. */
#define FLASH_DIV      25U

#define NV_SEGMENT     ((uint16_t *)0x1000)   /* info segment D */
#define NV_WORDS       (CONFMAXADDRESS / 2)

static uint16_t nvShadow[NV_WORDS];

static void NvLoad(void){
	for (uint8_t i = 0; i < NV_WORDS; i++){
		nvShadow[i] = NV_SEGMENT[i];
	}
}

static void NvFlush(void){
	uint8_t irq = PortIrqSave();
	FCTL2 = FWKEY | FSSEL_2 | (FLASH_DIV - 1U);
	FCTL3 = FWKEY;                       /* unlock */
	FCTL1 = FWKEY | ERASE;
	*NV_SEGMENT = 0;                     /* dummy write starts segment erase */
	while (FCTL3 & BUSY){
	}
	FCTL1 = FWKEY | WRT;
	for (uint8_t i = 0; i < NV_WORDS; i++){
		NV_SEGMENT[i] = nvShadow[i];
		while (FCTL3 & BUSY){
		}
	}
	FCTL1 = FWKEY;
	FCTL3 = FWKEY | LOCK;
	PortIrqRestore(irq);
}

void PortInit(void){
	WDTCTL = WDTPW | WDTHOLD;

	if (CALBC1_16MHZ != 0xFF){
		DCOCTL = 0;
		BCSCTL1 = CALBC1_16MHZ;
		DCOCTL = CALDCO_16MHZ;
	}
	BCSCTL2 = DIVS_1;                    /* SMCLK = DCO / 2 */

	BOARD_UART_PINS_INIT();
	UCA0CTL1 = UCSWRST;
	UCA0CTL1 |= UCSSEL_2;
	UCA0BR0 = (uint8_t)(UART_N & 0xFF);
	UCA0BR1 = (uint8_t)(UART_N >> 8);
	UCA0MCTL = (uint8_t)(UART_BRS << 1);
	UCA0CTL1 &= ~UCSWRST;

	NvLoad();
}

void PortIrqEnable(void){
	__enable_interrupt();
}

uint8_t PortIrqSave(void){
	uint16_t sr;
	__asm__ __volatile__ ("mov SR, %0" : "=r" (sr));
	uint8_t state = (sr & GIE) ? 1 : 0;
	__disable_interrupt();
	return state;
}

void PortIrqRestore(uint8_t state){
	if (state){
		__enable_interrupt();
	}
}

void PortTickStart(void){
	TACCR0 = (uint16_t)TICK_CCR0;
	TACCTL0 = CCIE;
	TACTL = TASSEL_2 | ID_3 | MC_1 | TACLR;
}

void PortTickStop(void){
	TACTL = TACLR;
	TACCTL0 = 0;
}

__attribute__((interrupt(TIMER0_A0_VECTOR)))
void PortTickIsr(void){
	MTaskTick();
}

void PortConsolePutc(char c){
	if (c == '\n'){
		PortConsolePutc('\r');
	}
	while (!(IFG2 & UCA0TXIFG)){
	}
	UCA0TXBUF = (uint8_t)c;
}

uint16_t PortNvRead16(uint16_t addr){
	if ((addr / 2) >= NV_WORDS){
		return 0xFFFF;
	}
	return nvShadow[addr / 2];
}

void PortNvWrite16(uint16_t addr, uint16_t value){
	if ((addr / 2) >= NV_WORDS){
		return;
	}
	if (nvShadow[addr / 2] != value){
		nvShadow[addr / 2] = value;
		NvFlush();
	}
}

void PortIdle(void){
}

#endif /* MENUOS_ARCH_MSP430 */
