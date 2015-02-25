/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/
#include "buttons.h"
#include "taskmanager.h"

taskmanager TManager;

void (*buttslot[BSLOTS])();//buttns slot
uint8_t _pcintbuttold=0;
uint8_t _blacklight;

void ButtonSetup(void){
	DDRL|=(1<<PL1);//backlight enable
	BUTTONSDDR&=~BUTTONSMASK; 
	BUTTONSPORT=BUTTONSMASK;//setup buttons port
	BSlotClear();//clear slots
	TManager.TReload(0,*ButtonSearch,2);//Add buttons to TManager
}


void BSlotClear(void){//clear slots
  for (uint8_t _i=0;_i<BSLOTS;_i++)
    buttslot[_i]=NULL;
}

void BSlot(void (*_f)()){//connect only zero slot. Any button call one function

	BSlotReload();
  buttslot[0]=_f;
}

void BSlot(uint8_t _slot, void (*_f)()){//button for separate function
  if (_slot < BSLOTS){
	buttslot[_slot]=_f;//put function into slot
  }
}

void ButtonSearch(void){//Tmanager function
	 if (_blacklight<120) _blacklight++;//backlight
	 uint8_t _pcintbutt=(~BUTTONSPIN)&BUTTONSMASK;
  if (_pcintbutt)//if any button was pressed
  {   
	_blacklight=0;
	PORTL|=(1<<PL1);
	 for (uint8_t _ipcint=0;_ipcint<BSLOTS;_ipcint++)
    {//only edge
      if (_pcintbutt==(1<<_ipcint) && !(_pcintbuttold&(1<<_ipcint))) 
      {
        if (buttslot[_ipcint]) (*buttslot[_ipcint])();//call button slot
        else (*buttslot[0])();
      }
    }
  }
  if (_blacklight>100) PORTL&=~(1<<PL1);
  _pcintbuttold=_pcintbutt;
}