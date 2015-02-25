/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#ifndef CONFIG_H_
#define CONFIG_H_

#define CONFIGARRAYWEIGHT 3

#include "taskmanager.h"
#include "buttons.h"
#include "glcd/glcd.h"
//#include "static.h"
#include "glcd/fonts/SystemFont5x7.h"


uint16_t ConfigReadWord(uint16_t EEPROMCell);
void ConfigWriteWord(uint16_t EEPROMCell, uint16_t EEPROMValue);
uint8_t ConfigReadByte(uint16_t EEPROMCell);
void ConfigWriteByte(uint16_t EEPROMCell, uint16_t EEPROMValue);
void ConfigSetup(uint8_t StartX, uint8_t StartY,char* ConfigName, uint8_t ConfigNumber,uint8_t DynConfig);
void ConfigReadLimits(uint8_t ConfigNumber,uint8_t DynConfig);
void ConfigView(uint8_t StartX, uint8_t StartY, char* ConfigName,uint8_t ConfigNumber,uint8_t ConfigMode);
void ConfigRefresh(void);
void ConfigUpButton(void);
void ConfigDownButton(void);
void ConfigLeftButton(void);
void ConfigLeftButton(void);
void ConfigRightButton(void);
void ConfigEnterButton(void);
void ConfigReturnButton(void);
void CursorMaxDefine(uint16_t _Value, uint8_t _mode);
uint16_t UIntPow(uint16_t _data, uint8_t _pow);

PROGMEM static uint16_t configsLimit[] = {
6,127,0,// PHOTOSNUMBERSERIAL  0
0,127,2,//PHTOSNUMBTEST		1
0,1,4,//PHOTOSTESTMOTION	2
0,50,6,//CONFPHDEL1		3
0,50,8,//CONFPHDEL2		4
0,1,10,//CONFLIGHTCHARGE	5
0,50,12,//CONFLIGHTDELAY	6
1,100,14,//DRIVEMAXSPEED		7
1,100,16,//DRIVEMAXBOST		8
0,1,18,//CONFIGLIGHT1 		9
0,1,24,//CONFIGLIGHT2		10
0,1,26,//CONFIGLIGHT3		11
0,1,28,//CONFIGLIGHT4		12
0,1,30,//CONFIGLIGHTIR		13
0,6,32,//CONFPHTYPE		14
0,1,37,//CONFPHCONN		15
0,1,41,//SERIALACTIVE		16
0,1,43,//SERIALMASTER		17
1,18000,45//DRIVEIMP	18
};


#endif /* CONFIG_H_ */