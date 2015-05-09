/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/
#include "buttons.h"

void buttons::Setup(uint32_t period){
	HwButtonsSetup(0);
	if (period == 0){
		period = 100;//0.1s
	}
	updatetime = period;
	lastupdatetime = 0;
	
	Clear();//clear all slots
}


void buttons::Clear(void){//clear slots
	uint8_t slots = HwButtonsCount();
  for (uint8_t _i = 0; _i < slots; _i++){
    buttslot[_i] = 0;
  }
}

void buttons::Add(void (*_f)(uint8_t)){//connect only zero slot. Any button call one 	
	uint8_t slots = HwButtonsCount();
	for (uint8_t _i = 0; _i < slots; _i++){
	 buttslot[_i] = _f;
  }
}

void buttons::Add(uint8_t _slot, void (*_f)(uint8_t)){//button for separate function
  if (_slot < HwButtonsCount()){
	buttslot[_slot]=_f;//put function into slot
  }
}

void buttons::Search(uint32_t time){//Tmanager function
	uint8_t slots = HwButtonsCount();
	uint8_t buttonscurrstate = HwButtonsState(0);
	//update buttons every some time, with overflow prevention
	if (abs(time - lastupdatetime) > updatetime){
		if (buttonscurrstate)//if any button was pressed
			{ //check all buttons slot
			for (uint8_t buttonslot = 0; buttonslot < slots ; buttonslot++)
				{//detect edge only
				if ((buttonscurrstate&(1 << buttonslot)) && (!(buttonoldstate&(1 << buttonslot))))
				{
					if (buttslot[buttonslot]);
					{
						(*buttslot[buttonslot])(buttonscurrstate&(1 << buttonslot));
					}
				}
			}
		}
		buttonoldstate = buttonscurrstate;//save old state
		lastupdatetime = time;
	}
}