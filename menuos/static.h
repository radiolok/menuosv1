/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#ifndef STATIC_H_
#define STATIC_H_

#include "hwdef.h"
#include "menudef.h"

 static const uint8_t fileStruct[FILENUMB*FILEREW] PROGMEM =
{
T_FOLDER,1,2,0, //0  папка, корень
T_APP,0,1,0,//1   основная программа
T_FOLDER,4,1,0, //2  настройки
T_FOLDER,20,4,0,//3  параметры
T_FOLDER,6,4,0,//4   вспышки
T_FOLDER,13,1,0,//5  фотоаппарат
T_CONF,1,0,0,//6  вспышка 1
T_CONF,2,0,0,//7  вспышка 2
T_CONF,3,0,0,//8  вспышка 3
T_CONF,4,0,0,//9  вспышка 4
T_CONF,5,0,0,//10 вспышка беспров
T_SCONF,10,1,0,//11 включить - тип 4 - записывает в конфиг для родителя значение
T_SCONF,10,0,0,//12 выключить
T_SFOLDER,15,2,0,//13 модель камеры
T_SFOLDER,18,1,0,//14 Подключение - 22 конфиг
T_SCONF,6,1,0,//15 Canon - 19 конфиг
T_SCONF,6,2,0,//16 Nikon
T_SCONF,6,3,0,//17 Pentax
T_SCONF,23,1,0,//18 проводное
T_SCONF,23,0,0,//19 беспроводное
T_FOLDER,25,2,0,//20 фотосессия
T_FOLDER,28,1,0,//21 фотоаппарат
T_FOLDER,30,1,0,//22 фотовспышки
T_FOLDER,32,3,0,//23 Привод
T_FOLDER,38,2,0,//24 Коммуникация
T_CONF,6,0,0,//25  кадров сессии
T_CONF,7,0,0,//26  тестовых кадров
T_CONF,8,0,0,//27  вращение в тестах
T_CONF,9,0,0,//28  задержка срабатывания
T_CONF,10,0,0,//29  задержка готовности
T_CONF,11,0,0,//30  контроль заряда
T_CONF,12,0,0,//31  время заряда
T_CONF,13,0,0,//32  макс частота вращения
T_CONF,14,0,0,//33  макс ускорение
T_CONF,15,0,0,//34  число имп.датчика
T_SFOLDER,36,1,0,//35  вращение
T_SCONF,0,0,0,//36  правое 35 конфиг
T_SCONF,0,1,0,//37  левое
T_CONF,16,0,0,//38  активация связи
T_SFOLDER,41,5,0,//39  скорость связи 39 конфиг
T_CONF,17,0,0,//40 поддержка комп. команд
T_SCONF,0,1,0,//41 4800, 
T_SCONF,0,2,0,//42 9600, 
T_SCONF,0,3,0,//43 14400, 
T_SCONF,0,4,0,//44 19200, 
T_SCONF,0,5,0,//45 57600
T_SCONF,0,6,0//46 115200
};

//Массив названий
static PROGMEM const char file_0[] = "Main Menu";
static PROGMEM const char file_1[] = "Start";
static PROGMEM const char file_2[] = "Settings";
static PROGMEM const char file_3[] = "Parameters";
static PROGMEM const char file_4[] = "Lights";
static PROGMEM const char file_5[] = "Photo";
static PROGMEM const char file_6[] = "Light 1";
static PROGMEM const char file_7[] = "Light 2";
static PROGMEM const char file_8[] = "Light 3";
static PROGMEM const char file_9[] = "Light 4";
static PROGMEM const char file_10[] = "Light IR";
static PROGMEM const char file_11[] = "On";
static PROGMEM const char file_12[] = "Off";
static PROGMEM const char file_13[] = "Model";
static PROGMEM const char file_14[] = "Connection";
static PROGMEM const char file_15[] = "Canon";
static PROGMEM const char file_16[] = "Nikon";
static PROGMEM const char file_17[] = "Pentax";
static PROGMEM const char file_18[] = "Wired";
static PROGMEM const char file_19[] = "IR";
static PROGMEM const char file_20[] = "Photosesion";
static PROGMEM const char file_21[] = "Photo";
static PROGMEM const char file_22[] = "Lights";
static PROGMEM const char file_23[] = "Drive";
static PROGMEM const char file_24[] = "Communication";
static PROGMEM const char file_25[] = "Session photos";
static PROGMEM const char file_26[] = "Test photos";
static PROGMEM const char file_27[] = "Motion in test";
static PROGMEM const char file_28[] = "Switch delay";
static PROGMEM const char file_29[] = "Ready delay";
static PROGMEM const char file_30[] = "Charge control";
static PROGMEM const char file_31[] = "Charge time";
static PROGMEM const char file_32[] = "Max speed";
static PROGMEM const char file_33[] = "Max accelerate";
static PROGMEM const char file_34[] = "Encoder imp";
static PROGMEM const char file_35[] = "Direction";
static PROGMEM const char file_36[] = "Right";
static PROGMEM const char file_37[] = "Left";
static PROGMEM const char file_38[] = "Enable connect";
static PROGMEM const char file_39[] = "Bods per sec.";
static PROGMEM const char file_40[] = "PC support";
static PROGMEM const char file_41[] = "4800"; 
static PROGMEM const char file_42[] = "9600";
static PROGMEM const char file_43[] = "14400"; 
static PROGMEM const char file_44[] = "19200";
static PROGMEM const char file_45[] = "57600";
static PROGMEM const char file_46[] = "115200";

static const char* fileNames[] PROGMEM = {   
 file_0,  file_1,  file_2,  file_3,  file_4,  file_5,  file_6,  file_7,  file_8,
  file_9,  file_10,  file_11,  file_12,  file_13,  file_14,  file_15,  file_16, 
  file_17,  file_18,  file_19,  file_20,  file_21,  file_22,  file_23,  file_24,
  file_25,  file_26,  file_27,  file_28,  file_29,  file_30,  file_31,  file_32,
  file_33,  file_34,  file_35,  file_36,  file_37,  file_38,  file_39,  file_40,
  file_41,  file_42,  file_43,  file_44,  file_45, file_46
};

static const PROGMEM uint16_t configsLimit[] = {
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


#endif /* STATIC_H_ */