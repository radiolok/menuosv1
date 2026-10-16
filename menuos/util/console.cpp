/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

#include "console.h"
#include "../port/port.h"

void console_putc(char c){
	PortConsolePutc(c);
}

void console_puts(const char *s){
	while (*s){
		PortConsolePutc(*s++);
	}
}

void console_print(int16_t val, int8_t base){
	char buf[18];
	uint8_t n = 0;
	uint16_t u;
	if ((base < 2) || (base > 16)){
		base = 10;
	}
	if ((val < 0) && (base == 10)){
		PortConsolePutc('-');
		u = (uint16_t)(-(int32_t)val);
	}
	else{
		u = (uint16_t)val;
	}
	do{
		uint8_t d = u % (uint8_t)base;
		buf[n++] = (char)(d < 10 ? '0' + d : 'A' + d - 10);
		u /= (uint8_t)base;
	} while (u);
	while (n){
		PortConsolePutc(buf[--n]);
	}
}

uint8_t menuos_itoa(int16_t val, char *buf){
	char tmp[6];
	uint8_t n = 0;
	uint8_t k = 0;
	uint16_t u;
	if (val < 0){
		buf[k++] = '-';
		u = (uint16_t)(-(int32_t)val);
	}
	else{
		u = (uint16_t)val;
	}
	do{
		tmp[n++] = (char)('0' + u % 10);
		u /= 10;
	} while (u);
	while (n){
		buf[k++] = tmp[--n];
	}
	buf[k] = '\0';
	return k;
}
