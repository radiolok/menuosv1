/*Copyright (c) 2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/**
 *  \file sim_main.cpp
 *  \brief Interactive MenuOS simulator for a POSIX terminal.
 *
 *  Keys (the five board buttons):
 *    Down  / s / +   UP button     - next item, increase value
 *    Up    / w / -   DOWN button   - previous item, decrease value
 *    Right / d / Enter  RIGHT      - enter folder, start app, next digit
 *    Left  / a       LEFT          - previous digit
 *    Backspace / q   RETURN        - back / save config
 *    Ctrl-C or Esc Esc             - quit
 *
 *  Like on the original board, "UP" is the "+" button: it moves the menu
 *  cursor to the next (lower) item and increases values.
 */
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>

#include "menuos/port/port.h"
#include "menuos/MTask.h"
#include "menuos/buttons.h"
#include "menuos/menu.h"
#include "menuos/hw/driver/disp/term/termdisp.h"

static struct termios saved;

static void RawMode(void){
	struct termios t;
	tcgetattr(STDIN_FILENO, &saved);
	t = saved;
	t.c_lflag &= ~(ICANON | ECHO);
	t.c_cc[VMIN] = 0;
	t.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

static void RestoreMode(void){
	tcsetattr(STDIN_FILENO, TCSANOW, &saved);
	printf("\x1b[?25h");
}

static int ReadKey(int timeout_ms){
	fd_set fds;
	struct timeval tv = { 0, timeout_ms * 1000 };
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) <= 0){
		return -1;
	}
	unsigned char c;
	if (read(STDIN_FILENO, &c, 1) != 1){
		return -1;
	}
	return c;
}

/* Returns a button mask, 0 for "nothing", -1 for quit. */
static int KeyToButton(int c){
	if (c == 0x1b){
		int c2 = ReadKey(20);
		if (c2 == 0x1b){
			return -1;
		}
		if (c2 == '['){
			switch (ReadKey(20)){
				case 'A': return BUTTONDOWN;
				case 'B': return BUTTONUP;
				case 'C': return BUTTONRIGHT;
				case 'D': return BUTTONLEFT;
			}
		}
		return 0;
	}
	switch (c){
		case 's': case '+': return BUTTONUP;
		case 'w': case '-': return BUTTONDOWN;
		case 'd': case '\n': case '\r': return BUTTONRIGHT;
		case 'a': return BUTTONLEFT;
		case 'q': case 0x7f: case 0x08: return BUTTONRETURN;
		case 0x03: return -1;
	}
	return 0;
}

static void Draw(void){
	static char last[TERMDISP_FRAME_SIZE];
	char frame[TERMDISP_FRAME_SIZE];
	TermDispRender(frame, sizeof(frame));
	if (strcmp(frame, last) == 0){
		return;
	}
	strcpy(last, frame);
	printf("\x1b[H\x1b[2J\x1b[?25l");
	printf("MenuOS v1 simulator (%s)\n\n%s\n", MENUOS_ARCH_NAME, frame);
	printf("arrows/wasd: move   Enter: open   q/Backspace: back   Esc Esc: quit\n");
	fflush(stdout);
}

int main(void){
	if (!isatty(STDIN_FILENO)){
		fprintf(stderr, "menuos-sim needs an interactive terminal\n");
		return 1;
	}
	RawMode();

	PortInit();
	Buttons.Setup(100);
	Task.Setup();
	Menu.Setup();
	Task.HwStart();

	int quit = 0;
	while (!quit){
		int c = ReadKey(10);
		if (c >= 0){
			int b = KeyToButton(c);
			if (b < 0){
				quit = 1;
			}
			else if (b > 0){
				HostPress((uint8_t)b, 120, 0);
			}
		}
		HostAdvance(10);
		Draw();
	}

	RestoreMode();
	printf("\n");
	return 0;
}
