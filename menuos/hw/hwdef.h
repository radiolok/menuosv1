/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#ifndef HW_DEF_H_
#define HW_DEF_H_

#define F_CPU 16000000ul

#include <math.h>
#include <util/delay.h>
#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/eeprom.h>
#include <stdlib.h>

 #define RLN_ELECTRO_2012_SE

//buttons are connected to this MCU port
 #define BUTTONSDDR DDRB
 #define BUTTONSPORT PORTB
 #define BUTTONSPIN PINB
 #define BUTTONSMASK 0x1F
 #define BSLOTS 6
 
 /**Button mask*/
 enum{
	BUTTONRETURN = 0x01,
	BUTTONLEFT = 0x02,
	BUTTONRIGHT = 0x10,
	BUTTONUP = 0x08,
	BUTTONDOWN = 0x04,
	BUTTONENTER = 0x10
 };
 
 /**button slot number
  *  NOTICE: RIGHT and ENTER hase the same slot by default! MenuOS do not use ENTER button!
  */
 enum{
	BUTTONSLOTRETURN = 0,
	BUTTONSLOTLEFT = 1,
	BUTTONSLOTRIGHT = 2,
	BUTTONSSLOTUP = 3,
	BUTTONSLOTDOWN = 4,
	BUTTONSLOTENTER = 2	
 };
  
	//configured LCD type
	//#define TEXT_DISPLAY 
	#define GRAPH_DISPLAY 



#define DISPSTRNUMB 5//display strings number.


#define DISPSTRLENGTH 16 //stirng length
 
#define FILENUMB 11 //files number
#define FILEREW  4//bytes for the file

enum{
	FILETYPE,
	FILEPARENT,
	FILEMODE1,
	FILEMODE2
};

#define MAXDEPTH 4//max menu depth



//used MTask slots
#define MENUSLOT 0

#define CONFARRAYWIDTH 3

enum {
	CONFVALUE,
	CONFMIN,
	CONFMAX	
};
//define store memory space
#define CONFMAXADDRESS 4096

#define PROGVERSIONTEXT ("v1.00") //current program version string

#endif /* HW_DEF_H_ */