/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "hwi.h"

class buttons{
private:
	
	void (*buttslot[BSLOTS])(uint8_t);//buttns slot
	uint8_t buttonoldstate;
	
	uint32_t lastupdatetime;
	
	int32_t updatetime;
	
public:
	/**
	 *  \brief Setup buttons
	 *  
	 *  \param [in] period Buttons Update period in ms
	 *  \return none
	 *  
	 *  \details Details
	 */
	void Setup(uint32_t period);
	
	void Clear(void);
	
	/**
	 *  \brief Add function to whole buttons control
	 *  
	 *  \param [in] _f function pointer
	 *  \return none
	 *  
	 *  \details Details
	 */
	void Add(void (*_f)(uint8_t));
	
	void Add(uint8_t _slot, void (*_f)(uint8_t));
	
	void Search(uint32_t time);
	
};

extern buttons Buttons;
#endif /* BUTTONS_H_ */