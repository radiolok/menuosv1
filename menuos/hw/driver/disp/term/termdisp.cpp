/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/** \file termdisp.cpp  Terminal display backend (see termdisp.h). */
#include "../hwdisp.h"

#if defined(TERM_DISPLAY)

/* Frame buffer. Rendering happens in HwDispUpdate(), not on every write. */
static char fb[DISPSTRNUMB][DISPSTRLENGTH];
static uint8_t selected[DISPSTRNUMB];
static uint8_t cursorRow = 0xFF;
static uint8_t cursorCol = 0;
static uint8_t cursorLen = 0;
static uint8_t dirty = 1;

static void RowReset(uint8_t row){
	selected[row] = 0;
	if (cursorRow == row){
		cursorRow = 0xFF;
	}
}

/* Row frame: '>'..'<' selected row, '*'..'*' row with the edit cursor. */
static char Border(uint8_t row, uint8_t right){
	if (selected[row]){
		return right ? '<' : '>';
	}
	if (row == cursorRow){
		return '*';
	}
	return '|';
}

uint8_t HwDispSetup(void){
	HwDispClearScreen();
	return 0;
}

void HwDispClearScreen(void){
	for (uint8_t r = 0; r < DISPSTRNUMB; r++){
		memset(fb[r], ' ', DISPSTRLENGTH);
		selected[r] = 0;
	}
	cursorRow = 0xFF;
	dirty = 1;
}

uint8_t HwDispPutChar(uint8_t row, uint8_t col, char symbol){
	if ((row < DISPSTRNUMB) && (col < DISPSTRLENGTH)){
		fb[row][col] = symbol;
		dirty = 1;
	}
	return 0;
}

uint8_t HwDispClearString(uint8_t row, uint8_t col, uint8_t length){
	if (row < DISPSTRNUMB){
		for (uint8_t c = col; (c < DISPSTRLENGTH) && (c < col + length); c++){
			fb[row][c] = ' ';
		}
		RowReset(row);
		dirty = 1;
	}
	return 0;
}

uint8_t HwDispPutString(uint8_t row, uint8_t col, const char* text, uint8_t length){
	if ((row < DISPSTRNUMB) && (col < DISPSTRLENGTH)){
		HwDispClearString(row, col, DISPSTRLENGTH - col);
		for (uint8_t c = col; (c < DISPSTRLENGTH) && (c < col + length) && *text; c++){
			fb[row][c] = *text++;
		}
	}
	return 0;
}

uint8_t HwDispSelectString(uint8_t row){
	if (row < DISPSTRNUMB){
		selected[row] ^= 1;   //like InvertRect on the GLCD
		dirty = 1;
	}
	return 0;
}

uint8_t HwDispDrawCursor(uint8_t row, uint8_t col, uint8_t length){
	if (row < DISPSTRNUMB){
		cursorRow = row;
		cursorCol = col;
		cursorLen = length;
		dirty = 1;
	}
	return 0;
}

uint8_t HwDispGetStringsNumb(void){
	return DISPSTRNUMB;
}

uint8_t HwDispGetStringsLength(void){
	return DISPSTRLENGTH;
}

size_t TermDispRender(char *buf, size_t size){
	size_t n = 0;
	#define EMIT(ch) do { if (n + 1 < size) { buf[n++] = (ch); } } while (0)

	EMIT('+');
	for (uint8_t c = 0; c < DISPSTRLENGTH; c++) EMIT('-');
	EMIT('+'); EMIT('\n');
	for (uint8_t r = 0; r < DISPSTRNUMB; r++){
		EMIT(Border(r, 0));
		for (uint8_t c = 0; c < DISPSTRLENGTH; c++) EMIT(fb[r][c]);
		EMIT(Border(r, 1));
		EMIT('\n');
	}
	EMIT('+');
	for (uint8_t c = 0; c < DISPSTRLENGTH; c++) EMIT('-');
	EMIT('+'); EMIT('\n');
	if (cursorRow != 0xFF){
		EMIT(' ');
		for (uint8_t c = 0; c < DISPSTRLENGTH; c++){
			EMIT(((c >= cursorCol) && (c < cursorCol + cursorLen)) ? '^' : ' ');
		}
		EMIT('\n');
	}
	#undef EMIT
	if (size){
		buf[n] = '\0';
	}
	return n;
}

void HwDispUpdate(void){
	if (!dirty){
		return;
	}
	dirty = 0;
#if defined(MENUOS_TERM_ANSI)
	static const char cls[] = "\x1b[H\x1b[2J";
	for (const char *p = cls; *p; p++){
		PortConsolePutc(*p);
	}
#endif
	/* Render row by row to keep the stack small on tiny MCUs. */
	char line[DISPSTRLENGTH + 4];
	for (uint8_t r = 0; r < DISPSTRNUMB + 2; r++){
		uint8_t k = 0;
		if ((r == 0) || (r == DISPSTRNUMB + 1)){
			line[k++] = '+';
			memset(&line[k], '-', DISPSTRLENGTH);
			k += DISPSTRLENGTH;
			line[k++] = '+';
		}
		else{
			uint8_t row = r - 1;
			line[k++] = Border(row, 0);
			memcpy(&line[k], fb[row], DISPSTRLENGTH);
			k += DISPSTRLENGTH;
			line[k++] = Border(row, 1);
		}
		line[k++] = '\n';
		for (uint8_t i = 0; i < k; i++){
			PortConsolePutc(line[i]);
		}
	}
	if (cursorRow != 0xFF){
		PortConsolePutc(' ');
		for (uint8_t c = 0; c < DISPSTRLENGTH; c++){
			PortConsolePutc(((c >= cursorCol) && (c < cursorCol + cursorLen)) ? '^' : ' ');
		}
		PortConsolePutc('\n');
	}
}

#endif /* TERM_DISPLAY */
