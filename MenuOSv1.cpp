/*Copyright (c) 2010, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

//if any problem shoot in the leg
	
#include "menuos/util/uart.h"
#include "menuos/util/log.h"

//system OSC
#define  UART0_BAUD_RATE 38400


#include "menuos/MTask.h"//TManager Setup
#include "menuos/buttons.h"//buttons Setup
#include "menuos/menu.h"
#include "menuos/apps/sampleapp.h"

//Define system classes
MTask Task;
buttons Buttons;
MMenu Menu;
MClock Clock;
sampleapp SampleApp;


int main(void)
{
	sei();
	uart_init( UART_BAUD_SELECT(UART0_BAUD_RATE,F_CPU));

	Buttons.Setup(100);
	Task.Setup();
	Menu.Setup();		

	log_trace("System started");
	Task.Start();//never turn back

	
   while(1)
    {
    }
	return 0;
}