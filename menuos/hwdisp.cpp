/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#include "hwdisp.h"

/*===================================================================
					DISPLAY HW ROUTINE ACCESS
===================================================================*/

 
uint8_t HwDispSetup(void){
	#if defined(TLCD)
		
	#endif
	
	#if defined(GLCD)
		GLCD.SelectFont(MENUFONT);
	#endif
	return 0;
}

uint8_t HwDispPutChar(uint8_t row, uint8_t col, char symbol){
//check stirng bound
if ((row >= 0) && (row < DISPSTRNUMB)){
	#if defined(TLCD)
	
	#endif
	
	#if defined(GLCD)
	HwDispClearString(row, col, DISPSTRNUMB-col);
	GLCD.CursorToXY(XINDENT+1+col*MENUFONTWIDTH,YINDENT+1+row*MENUFOTNHEIGHT);//set cursor
	GLCD.PutChar(symbol);//write
	#endif
	
}
return 0;	
}


uint8_t HwDispPutString(uint8_t row, uint8_t col, char* text, uint8_t length){
	//check stirng bound
	if ((row >= 0) && (row < DISPSTRNUMB)){
	#if defined(TLCD)
		
	#endif
	
	#if defined(GLCD)
		HwDispClearString(row, col, DISPSTRNUMB-col);
		GLCD.CursorToXY(XINDENT+1+col*MENUFONTWIDTH,YINDENT+1+row*MENUFOTNHEIGHT);//set cursor
		GLCD.Puts(text);//write
	#endif
		
	}
	return 0;
}

uint8_t HwDispClearString(uint8_t row, uint8_t col, uint8_t length){
	//check stirng bound
	if ((row >= 0) && (row < DISPSTRNUMB)){
	#if defined(TLCD)
		
	#endif
	
	#if defined(GLCD)
		GLCD.DrawRectangle(XINDENT+1+col*MENUFONTWIDTH,
							YINDENT+1+row*MENUFOTNHEIGHT,
							XINDENT+1+(col+length)*MENUFONTWIDTH,
							YINDENT+1+(row+1)*MENUFOTNHEIGHT)
	#endif
		
	}	
	return 0;
}

uint8_t HwDispSelectString(uint8_t row){
//check stirng bound
	if ((row >= 0) && (row < DISPSTRNUMB)){
	#if defined(TLCD)
		
	#endif
	
	#if defined(GLCD)
		GLCD.InvertRect(XINDENT, row*MENUFOTNHIGHT, LCDWIDTH - XINDENT,MENUFOTNHIGHT);
	#endif
		
	}
	return 0;
}


void HwDispClearScreen(void){
	#if defined(TLCD)
		
	#endif
	
	#if defined(GLCD)
		GLCD.ClearScreen();
	#endif	
}

uint8_t HwDispGetStringsNumb(void){
	return DISPSTRNUMB;
}

uint8_t HwDispGetStringsLength(void){
	return DISPSTRLENGTH;
}
