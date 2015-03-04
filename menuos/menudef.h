/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#ifndef MENUDEF_H_
#define MENUDEF_H_

#include <stdint.h>

//File types definitions
#define T_FOLDER         0 //Folder file
#define T_DFOLDER        2 //Dynamic Folder
#define T_SFOLDER        3 //Select Folder
#define T_APP            4 //Application
#define T_CONF           5 //Config
#define T_SCONF          6 //Select Config
#define T_DCONF          7 //Dynamic Config

#define BRCRUMBSLENGTH 3

//Set Mode2 for App if you don't want to start programm in background
#define BACKGROUND 0x1F


enum{
	CRUMBPARENT,
	CRUMBPAGE,
	CRUMBCURSOR
};

enum{
	BRCRTYPE = 0,
	BRCRPARENT = 1,
	BRCRMODE1 = 2,
	BRCRMODE2 = 3	
	};


/**
 *  Main Filedata structure
 *  
 */
struct filedata{
	uint8_t type;
	uint8_t parent;
	uint8_t mode1;
	uint8_t mode2;
	char name[20];
};

struct configdata{
	int16_t value;
	int16_t min;
	int16_t max;
};

#endif