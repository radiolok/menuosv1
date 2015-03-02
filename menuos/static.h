/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#ifndef STATIC_H_
#define STATIC_H_

#include "definitions.h"

#include <avr/pgmspace.h>

 static uint8_t fileStruct[FILENUMB*FILEREW] PROGMEM =
{
T_FOLDER,1,2, //0  папка, корень
T_APP,0,1,//1   основная программа
T_FOLDER,4,1, //2  настройки
T_FOLDER,20,4,//3  параметры
T_FOLDER,6,4,//4   вспышки
T_FOLDER,13,1,//5  фотоаппарат
T_CONF,CONFLIGHT1,0,//6  вспышка 1
T_CONF,CONFLIGHT2,0,//7  вспышка 2
T_CONF,CONFLIGHT3,0,//8  вспышка 3
T_CONF,CONFLIGHT4,0,//9  вспышка 4
T_CONF,CONFLIGHTIR,0,//10 вспышка беспров
T_SCONF,10,1,//11 включить - тип 4 - записывает в конфиг для родителя значение
T_SCONF,10,0,//12 выключить
T_SFOLDER,15,2,//13 модель камеры
T_SFOLDER,18,1,//14 Подключение - 22 конфиг
T_SCONF,6,1,//15 Canon - 19 конфиг
T_SCONF,6,2,//16 Nikon
T_SCONF,6,3,//17 Pentax
T_SCONF,23,1,//18 проводное
T_SCONF,23,0,//19 беспроводное
T_FOLDER,25,2,//20 фотосессия
T_FOLDER,28,1,//21 фотоаппарат
T_FOLDER,30,1,//22 фотовспышки
T_FOLDER,32,3,//23 Привод
T_FOLDER,38,2,//24 Коммуникация
T_CONF,PHOTOSNUMBSERIAL,0,//25  кадров сессии
T_CONF,PHOTOSNUMBTEST,0,//26  тестовых кадров
T_CONF,PHOTOSTESTMOTION,0,//27  вращение в тестах
T_CONF,CONFPHDEL1,0,//28  задержка срабатывания
T_CONF,CONFPHDEL2,0,//29  задержка готовности
T_CONF,CONFLIGHTCHARGE,0,//30  контроль заряда
T_CONF,CONFLIGHTDELAY,0,//31  время заряда
T_CONF,DRIVEMAXSPEED,0,//32  макс частота вращения
T_CONF,DRIVEMAXBOST,0,//33  макс ускорение
T_CONF,ENCODERIMP,0,//34  число имп.датчика
T_SFOLDER,36,1,//35  вращение
T_SCONF,0,0,//36  правое 35 конфиг
T_SCONF,0,1,//37  левое
T_CONF,16,0,//38  активация связи
T_SFOLDER,41,5,//39  скорость связи 39 конфиг
T_CONF,17,0,//40 поддержка комп. команд
T_SCONF,0,1,//41 4800, 
T_SCONF,0,2,//42 9600, 
T_SCONF,0,3,//43 14400, 
T_SCONF,0,4,//44 19200, 
T_SCONF,0,5,//45 57600
T_SCONF,0,6//46 115200
};
//Массив названий
prog_char file_0[] PROGMEM = "Main Menu";
prog_char file_1[] PROGMEM = "Start";
prog_char file_2[] PROGMEM = "Settings";
prog_char file_3[] PROGMEM = "Parameters";
prog_char file_4[] PROGMEM = "Lights";
prog_char file_5[] PROGMEM = "Photo";
prog_char file_6[] PROGMEM = "Light 1";
prog_char file_7[] PROGMEM = "Light 2";
prog_char file_8[] PROGMEM = "Light 3";
prog_char file_9[] PROGMEM = "Light 4";
prog_char file_10[] PROGMEM = "Light IR";
prog_char file_11[] PROGMEM = "On";
prog_char file_12[] PROGMEM = "Off";
prog_char file_13[] PROGMEM = "Model";
prog_char file_14[] PROGMEM = "Connection";
prog_char file_15[] PROGMEM = "Canon";
prog_char file_16[] PROGMEM = "Nikon";
prog_char file_17[] PROGMEM = "Pentax";
prog_char file_18[] PROGMEM = "Wired";
prog_char file_19[] PROGMEM = "IR";
prog_char file_20[] PROGMEM = "Photosesion";
prog_char file_21[] PROGMEM = "Photo";
prog_char file_22[] PROGMEM = "Lights";
prog_char file_23[] PROGMEM = "Drive";
prog_char file_24[] PROGMEM = "Communication";
prog_char file_25[] PROGMEM = "Session photos";
prog_char file_26[] PROGMEM = "Test photos";
prog_char file_27[] PROGMEM = "Motion in test";
prog_char file_28[] PROGMEM = "Switch delay";
prog_char file_29[] PROGMEM = "Ready delay";
prog_char file_30[] PROGMEM = "Charge control";
prog_char file_31[] PROGMEM = "Charge time";
prog_char file_32[] PROGMEM = "Max speed";
prog_char file_33[] PROGMEM = "Max accelerate";
prog_char file_34[] PROGMEM = "Encoder imp";
prog_char file_35[] PROGMEM = "Direction";
prog_char file_36[] PROGMEM = "Right";
prog_char file_37[] PROGMEM = "Left";
prog_char file_38[] PROGMEM = "Enable connect";
prog_char file_39[] PROGMEM = "Bods per sec.";
prog_char file_40[] PROGMEM = "PC support";
prog_char file_41[] PROGMEM = "4800"; 
prog_char file_42[] PROGMEM = "9600";
prog_char file_43[] PROGMEM = "14400"; 
prog_char file_44[] PROGMEM = "19200";
prog_char file_45[] PROGMEM = "57600";
prog_char file_46[] PROGMEM = "115200";


PROGMEM const char *fileNames[] = {   
 file_0,  file_1,  file_2,  file_3,  file_4,  file_5,  file_6,  file_7,  file_8,
  file_9,  file_10,  file_11,  file_12,  file_13,  file_14,  file_15,  file_16, 
  file_17,  file_18,  file_19,  file_20,  file_21,  file_22,  file_23,  file_24,
  file_25,  file_26,  file_27,  file_28,  file_29,  file_30,  file_31,  file_32,
  file_33,  file_34,  file_35,  file_36,  file_37,  file_38,  file_39,  file_40,
  file_41,  file_42,  file_43,  file_44,  file_45, file_46
};



#endif /* STATIC_H_ */