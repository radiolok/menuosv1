/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#ifndef CONFIG_H_
#define CONFIG_H_

#include "menu.h"

class MConfig{
	private:
	
		uint8_t fileid;
		
		uint8_t configid;
		
		configdata config;
		
		uint16_t shift;
		
		uint8_t row;
		
		uint8_t digit;
		
		uint8_t digits;
		
		void ButtonLeft(void);
		
		void ButtonRight(void);
		
		void ButtonUp(void);
		
		void ButtonDown(void);
		
		void Return(void);
		
		void Show(void);
		
	public:
	
		MConfig();
		
		~MConfig();
		
		void Setup(uint8_t argc, uint8_t *argv);
		
		void ButtonsLogic(uint8_t button);
		
		/**
		 *  \brief Get config string for menu FolderShow
		 *  
		 *  \param [in] filenumb config number
		 *  \param [in] text Parameter_Description
		 *  \return Return_Description
		 *  
		 *  \details Details
		 */
		void GetString(uint8_t filenumb, char* text, uint16_t shift);
		
};

extern MConfig Config;

void ConfigButtonsHandler(uint8_t button);

uint16_t UIntPow(uint16_t _data, uint8_t _pow);


#endif /* CONFIG_H_ */