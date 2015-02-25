/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#ifndef BUTTONS_H_
#define BUTTONS_H_

#define BSLOTS 6 //buttons slot number

//output registers definition:
#define BUTTONSPIN PING 
#define BUTTONSPORT PORTG
#define BUTTONSDDR DDRG
#define BUTTONSMASK 0x1F //buttons mask on port
#define BUTTONSLOTRETURN 0 //back button pin position
#define BUTTONSLOTLEFT 1 //left button pin position
#define BUTTONSLOTRIGHT 4 //right buttn pin position
#define BUTTONSLOTUP  2//up button pin position
#define BUTTONSLOTDOWN 3 //down buttn pin position
#define BUTTONSLOTENTER 4
#define BUTTONRETURN (1<<BUTTONSLOTRETURN)
#define BUTTONLEFT (1<<BUTTONSLOTLEFT)
#define BUTTONRIGHT (1<<BUTTONSLOTRIGHT)
#define BUTTONUP (1<<BUTTONSLOTUP)
#define BUTTONDOWN (1<<BUTTONSLOTDOWN)
#define BUTTONENTER (1<<BUTTONSLOTENTER)
#define NULL 0
#include <avr/interrupt.h>
#include <inttypes.h>


void ButtonSetup(void);
void BSlotClear(void);
void BSlot(void (*_f)());
void BSlot(uint8_t _slot, void (*_f)());
void ButtonSearch(void);

#endif /* BUTTONS_H_ */