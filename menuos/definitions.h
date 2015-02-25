/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

//File types definitions
#define T_FOLDER         0 //Folder file
#define T_DFOLDER        2 //Dynamic Folder
#define T_SFOLDER        3 //Select Folder
#define T_APP            4 //Application
#define T_CONF           5 //Config
#define T_SCONF          6 //Select Config
#define T_DCONF          7 //Dynamic Config

#define POSXMIN 6
#define POSXMAX 6
#define POSYMIN 10
#define POSYMAX 63
#define MENUFONT SystemFont5x7
#define MENUFONTWEIGHT 6
#define MENUFOTNHIGHT 8

#define abs(x) ((x)>0?(x):-(x))
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
#define DISPSTR 4//display strings number. Currently base of 2!
#define FILENUMB 47 //files number
#define FILEREW  3//bytes for the file
#define MAXDEPTH 4//max menu depth
#define PROGV "v1" //current program version string
#define LCDBITS 0x03//strings number mask. need to update with DISPSTR
#define LCDINVBITS 0xFC//string number maks inverted
#define LCDROW 8//display rows
#define LCDCOL 20//display cumns

//===================GLOBAL DEFINES====================================
#define PHOTOSNUMBSERIAL  0
#define PHOTOSNUMBTEST		1
#define PHOTOSTESTMOTION	2
#define CONFPHDEL1			3
#define CONFPHDEL2			4
#define CONFLIGHTCHARGE		5
#define CONFLIGHTDELAY		6
#define DRIVEMAXSPEED		7
#define DRIVEMAXBOST		8
#define CONFLIGHT1 		9
#define CONFLIGHT2		10
#define CONFLIGHT3		11
#define CONFLIGHT4		12
#define CONFLIGHTIR		13
#define CONFPHTYPE			14
#define CONFPHCONN			15
#define SERIALACTIVE		16
#define SERIALMASTER		17
#define ENCODERIMP			18
#define CAMERATYPE			19
#define DRIVEDIR			35 //20
#define SERIALSPEED			39 //21
#define PHOTOCONNECT		22 //22
//модуль Photo_module.c
#define CANON 0 //идентификатор ф/а Canon
#define NIKON 1 //идентификатор ф/а Nikon
#define PENTAX 2 //идентификатор ф/а Pentax
//Значения задержек для несущих частот ИК-сигнала ф/а
#define CANONDEL 66 //несущая 33кГц
#define NIKONDEL 64 //несущая 38кГц
#define PENTAXDEL 63 //несущая 40кГц
//Модуль Driver_module.c
#define DRIVEMAXSTEP 4096 //количество элементарных градусов.Раняется числу импульсов с датчика положения/скорости. 
#define DRIVESTOPSTEP 10 //кол-во элементарных градусов за которые можно остановить привод без колебаний.
#define KINTENS 5 //величина от 0 до 15, задающая интенсивность разгона, где коэффициент деления есть два в степени коэффициента, т.е. 0 - задатчик выключен, 15 - задатчик имеет наибольшую тормознутость.
#define KREGUL 2 //коэффициент регулятора тока
//Модуль Serial_module.c
#define PHTESTSTART 0x81
#define PHTESTSTOP 0x82
#define PHTEST 0x8A
#define PHSERIALSTART 0x83
#define PHSERIALSTOP 0x84
#define PHSERIAL 0x8B
#define TXSTART 0x42
#define TXEND 0x45
#define PHCHECK 0x43
#define PHREAD  0x52
#define PHWRITE 0x57
#define PHWORK  0x50
#define PHSETTINGS 0x53


#endif /* DEFINITIONS_H_ */