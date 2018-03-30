/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#include "hwfile.h"

static const uint8_t fileStruct[FILENUMB*FILEREW] PROGMEM =
{
	T_FOLDER, 0, 1, 2,				//0
		T_FOLDER, 0, 3, 3,			//1
		T_FOLDER, 0, 7, 4,			//2
			T_APP,	1, 1, 0,		//3
			T_APP,	1, 2, 0,		//4
			T_DFOLDER, 1, 6, 66,	//5
				T_APP,	5, 2, 0,	//6
			
			T_CONF,	2, 0, 0,		//7
			T_CONF,	2, 1, 0,		//8
			T_CONF,	2, 2, 0,		//9
			T_APP, 2, 3, 0			//10
		
	
};

//names array
static PROGMEM const char file_0[] = "Root";
static PROGMEM const char file_1[] = "Folder 1";
static PROGMEM const char file_2[] = "Folder 2";
static PROGMEM const char file_3[] = "App 1";
static PROGMEM const char file_4[] = "App 2";
static PROGMEM const char file_5[] = "Dyn Folder";
static PROGMEM const char file_6[] = "App";
static PROGMEM const char file_7[] = "config 0";
static PROGMEM const char file_8[] = "config 1";
static PROGMEM const char file_9[] = "config 2";
static PROGMEM const char file_10[] = "Date and Time";

 PGM_P const fileNames[] PROGMEM = {
	file_0,  file_1,  file_2,  file_3,  file_4,  file_5,  file_6,  file_7,  file_8,
	file_9, file_10
};

//number of cell(step by 2), minimal value, maximum value
static const PROGMEM int16_t configsLimit[] = {
	0,0,0,// config  0
	2,-8099,8096,//config 1
	4,1,48,//config	2
};



/*===================================================================
					FILE STRUCTURE ACCESS
===================================================================*/


uint8_t HwFileGetInfo(uint8_t filenumb, struct filedata *file){
	
	file->type = pgm_read_byte(&fileStruct[filenumb * FILEREW]);
	file->parent = pgm_read_byte(&fileStruct[filenumb * FILEREW + 1]);
	file->mode1 = pgm_read_byte(&fileStruct[filenumb * FILEREW + 2]);
	file->mode2 = pgm_read_byte(&fileStruct[filenumb * FILEREW + 3]);
	HwFileGetName(filenumb, file->name);
	return 0;
}
uint8_t HwFileGetInfo(uint8_t filenumb, uint8_t pos){
	return pgm_read_byte(&fileStruct[filenumb * FILEREW + pos]);
}

uint8_t HwFileGetName(uint8_t filenumb, char* name){
	strcpy_P(name, (char*)pgm_read_word(&(fileNames[filenumb])));	

	
	return 0;
}

uint8_t HwFileGetType(uint8_t filenumb){
	return pgm_read_byte(&fileStruct[filenumb * FILEREW]);
}

/*===================================================================
					CONFIG STRUCTURE ACCESS
===================================================================*/

uint8_t HwConfigGetData(uint8_t configid, struct configdata* config, uint16_t shift){
	uint16_t address = pgm_read_word(&configsLimit[configid * CONFARRAYWIDTH+CONFVALUE]) + shift*2;
	if (CONFMAXADDRESS < address){
		return 1;//address error
	}
	config->value = eeprom_read_word ((uint16_t *)address);
	config->min = pgm_read_word(&configsLimit[configid * CONFARRAYWIDTH+CONFMIN]);
	config->max = pgm_read_word(&configsLimit[configid * CONFARRAYWIDTH+CONFMAX]);
	
	return 0;
}

uint8_t HwConfigSetData(uint8_t configid, struct configdata* config, uint16_t shift){
	//set address
	uint16_t address = pgm_read_word(&configsLimit[configid * CONFARRAYWIDTH+CONFVALUE]) + shift*2;
	if (CONFMAXADDRESS < address){
		return 1;//address error!
	}
	eeprom_write_word ((uint16_t *) address, config->value);
	return 0;
}