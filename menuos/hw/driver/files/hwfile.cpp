/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#include "hwfile.h"

/*===================================================================
					MENU TABLE
  This is the part you edit for your own device. See docs/menu-table.md.

  Every file is 4 bytes: type, parent, mode1, mode2.
    T_FOLDER   mode1 = first child id,          mode2 = number of children
    T_DFOLDER  mode1 = id of the repeated file,  mode2 = number of copies
    T_APP      mode1 = application number,      mode2 = BACKGROUND or 0
    T_CONF     mode1 = config id (row of configsLimit[])
  Children of a folder must have consecutive ids.
===================================================================*/

#define FILENUMB 11 //files number

static const uint8_t fileStruct[FILENUMB*FILEREW] MENUOS_ROM =
{
	T_FOLDER, 0, 1, 2,				//0  Root
		T_FOLDER, 0, 3, 3,			//1  Folder 1
		T_FOLDER, 0, 7, 4,			//2  Folder 2
			T_APP,	1, 1, 0,		//3  App 1
			T_APP,	1, 2, 0,		//4  App 2 (sample app)
			T_DFOLDER, 1, 6, 66,	//5  Dyn Folder: 66 copies of file 6
				T_APP,	5, 2, 0,	//6  App
			T_CONF,	2, 0, 0,		//7  config 0 (Yes/No)
			T_CONF,	2, 1, 0,		//8  config 1 (-8099..8096)
			T_CONF,	2, 2, 0,		//9  config 2 (1..48)
			T_APP, 2, 3, 0			//10 Date and Time (clock app)
};

//names array
static const char file_0[] MENUOS_ROM = "Root";
static const char file_1[] MENUOS_ROM = "Folder 1";
static const char file_2[] MENUOS_ROM = "Folder 2";
static const char file_3[] MENUOS_ROM = "App 1";
static const char file_4[] MENUOS_ROM = "App 2";
static const char file_5[] MENUOS_ROM = "Dyn Folder";
static const char file_6[] MENUOS_ROM = "App";
static const char file_7[] MENUOS_ROM = "config 0";
static const char file_8[] MENUOS_ROM = "config 1";
static const char file_9[] MENUOS_ROM = "config 2";
static const char file_10[] MENUOS_ROM = "Date and Time";

static const char * const fileNames[FILENUMB] MENUOS_ROM = {
	file_0,  file_1,  file_2,  file_3,  file_4,  file_5,  file_6,  file_7,  file_8,
	file_9, file_10
};

//storage address (bytes, step by 2), minimal value, maximum value
//max == 0 means a Yes/No config
static const int16_t configsLimit[] MENUOS_ROM = {
	0,0,0,// config  0
	2,-8099,8096,//config 1
	4,1,48,//config	2
};

/*===================================================================
					FILE STRUCTURE ACCESS
===================================================================*/

uint8_t HwFileGetInfo(uint8_t filenumb, struct filedata *file){
	file->type = menuos_rom_byte(&fileStruct[filenumb * FILEREW]);
	file->parent = menuos_rom_byte(&fileStruct[filenumb * FILEREW + 1]);
	file->mode1 = menuos_rom_byte(&fileStruct[filenumb * FILEREW + 2]);
	file->mode2 = menuos_rom_byte(&fileStruct[filenumb * FILEREW + 3]);
	HwFileGetName(filenumb, file->name);
	return 0;
}

uint8_t HwFileGetInfo(uint8_t filenumb, uint8_t pos){
	return menuos_rom_byte(&fileStruct[filenumb * FILEREW + pos]);
}

uint8_t HwFileGetName(uint8_t filenumb, char* name){
	menuos_rom_strcpy(name, menuos_rom_ptr(&fileNames[filenumb]));
	return 0;
}

uint8_t HwFileGetType(uint8_t filenumb){
	return menuos_rom_byte(&fileStruct[filenumb * FILEREW]);
}

/*===================================================================
					CONFIG STRUCTURE ACCESS
===================================================================*/

uint8_t HwConfigGetData(uint8_t configid, struct configdata* config, uint16_t shift){
	uint16_t address = (uint16_t)menuos_rom_word(&configsLimit[configid * CONFARRAYWIDTH+CONFVALUE]) + shift*2;
	if (CONFMAXADDRESS <= address + 1){
		return 1;//address error
	}
	config->value = (int16_t)PortNvRead16(address);
	config->min = (int16_t)menuos_rom_word(&configsLimit[configid * CONFARRAYWIDTH+CONFMIN]);
	config->max = (int16_t)menuos_rom_word(&configsLimit[configid * CONFARRAYWIDTH+CONFMAX]);
	return 0;
}

uint8_t HwConfigSetData(uint8_t configid, struct configdata* config, uint16_t shift){
	//set address
	uint16_t address = (uint16_t)menuos_rom_word(&configsLimit[configid * CONFARRAYWIDTH+CONFVALUE]) + shift*2;
	if (CONFMAXADDRESS <= address + 1){
		return 1;//address error!
	}
	PortNvWrite16(address, (uint16_t)config->value);
	return 0;
}
