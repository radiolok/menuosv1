/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/



#ifndef CLOCK_H_
#define CLOCK_H_

#include "hw/hwi.h"

#include "hw/driver/i2c/Wire.h"
#include "hw/driver/DS1307.h"

#include "menuos/MTask.h"
#include "menuos/buttons.h"

class MClock{
public:

MClock();

~MClock();

uint8_t Setup(uint8_t argc, uint8_t *argv);

uint8_t ButtonsLogic(uint8_t button);

void DrawStatic();

void DrawTime();

void DrawDate();

void DrawCursor();		
	
private:

	void ButtonDown();

	void ButtonUp();
	
	void Return();
	
	uint8_t cursorSt;
	int rtc[8];
};
extern MClock Clock;

void ClockButtonsHandler(uint8_t button);


#endif /* CLOCK_H_ */