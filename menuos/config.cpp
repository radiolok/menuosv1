/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#include "config.h"

#include "menudef.h"

#include "util/log.h"
MConfig::MConfig(){
	
}

MConfig::~MConfig(){
	
}

void ConfigButtonsHandler(uint8_t button){
	Config.ButtonsLogic(button);
}

/**
 *  \brief Setup config utility and start redact
 *  
 *  \param [in] StartX config X position
 *  \param [in] StartY Cnfig Y position
 *  \param [in] ConfigName cnfig name string
 *  \param [in] ConfigNumber 
 *  \param [in] DynConfig dynamic config flag
 *  \return Return_Description
 *  
 *  \details Details
 */
void MConfig::Setup(uint8_t argc, uint8_t *argv){

	Buttons.Add(ConfigButtonsHandler);
	
	shift = 0;
	//calc shift for dynamic folders
	for (uint8_t level = 0; level < argc; level++){
		if (T_DFOLDER == HwFileGetType(argv[level * BRCRUMBSLENGTH])){
			shift += argv[level * BRCRUMBSLENGTH + CRUMBPAGE] * HwDispGetStringsNumb() + argv[level * BRCRUMBSLENGTH + CRUMBCURSOR];
		}
	}
	
	fileid  = argv[argc * BRCRUMBSLENGTH];
	
	row = argv[(argc - 1) * BRCRUMBSLENGTH + CRUMBCURSOR];
	
	configid = HwFileGetInfo(fileid, (uint8_t)BRCRMODE1);
	
	HwConfigGetData(configid, &config, shift);
	
	//check initial values:
	if (config.value > config.max){
		config.value = config.max;
	}
	if (config.value < config.min){
		config.value = config.min;
	}
	
	config.digit = 0;	
	digits = DigitsCount(config.max);
		
	Show();
}

void MConfig::GetString(uint8_t filenumb, char* text, uint16_t shift, uint8_t mode = 0){
	filedata file;
	uint8_t status = 0;
	HwFileGetInfo(filenumb, &file);
	if (mode == 0){
		status = HwConfigGetData(file.mode1, &config, shift);
	}
	if (status){//shift index error
		sprintf(text, "INDEX ERROR");
		return;
	}
	AlignStr(file.name, config.value, config.max, HwDispGetStringsLength(), text);

	return;
}

void MConfig::AlignStr(char *fname, int16_t val, uint8_t mode, uint8_t length, char *str){
	//we show full text to string
	uint8_t symbols = sprintf(str, "%s", fname);
	for (uint8_t i = symbols; i < length; i++){
		str[i] = ' ';
	}
	uint8_t dig = 0;
	char valtext[12];
	if (mode){//numeric
		dig = DigitsCount(val);
		ltoa(val, valtext, 10);
		strcpy(str + length - dig,  valtext);
	}
	else{
		if (val){
			strcpy(str + length - 4, " Yes");
		}
		else{
			strcpy(str + length - 3, " No");
		}
	}
	return;
}

void MConfig::Show(){
	char text[DISPSTRLENGTH * 2];//TODO: use malloc
	GetString(fileid, text, shift, 1);
	HwDispPutString(row, 0, text, HwDispGetStringsLength());
	if (config.max){
		HwDispDrawCursor(row, HwDispGetStringsLength() - config.digit - 1 , 1);
	}
	else{
		HwDispDrawCursor(row, HwDispGetStringsLength() - 4, 5);
	}
}

void MConfig::ButtonsLogic(uint8_t button){
	switch (button){
		case BUTTONLEFT:
			ButtonLeft();
		break;
		case BUTTONRIGHT:
			ButtonRight();
		break;
		case BUTTONRETURN:
			Return();
		break;
		case BUTTONUP:
			ButtonUp();
		break;
		case BUTTONDOWN:
			ButtonDown();
		break;
		default:

		break;
	}
}

void MConfig::ButtonUp(){
	if (config.max != 0){//not boolean
		config.value += UIntPow(10, config.digit);
		if (config.value > config.max){
			config.value = config.max;
		}
	}
	else{//boolean
		if  (config.value){
			config.value = 0;
		}
		else{
			config.value = 1;
		}
	}
	Show();
}

void MConfig::ButtonDown(){
	if (config.max != 0){//not boolean
		config.value -= UIntPow(10, config.digit);
		if (config.value < config.min){
			config.value = config.min;
		}
	}
	else{//boolean
		if  (config.value){
			config.value = 0;
		}
		else{
		config.value = 1;
		}
	}
	Show();
}

void MConfig::ButtonLeft(){
	if (config.digit < (digits - 1))
	{
		config.digit++;
	}
	Show();	
}

void MConfig::ButtonRight(){
	if (0 < config.digit){
		config.digit--;
	}
	Show();
}

void MConfig::Return(){
	//save changes
	HwConfigSetData(configid, &config, shift);
	Task.ActiveApp = 0;
}


uint8_t MConfig::DigitsCount(int16_t val){
	uint8_t dig = 1;
	if (0 > val){
		dig++;
	}
	val = abs(val);
	while (val >= 10){
		val = val / 10;
		dig++;//count digits
	}
	return dig;	
}

/**
 *  \brief Power integer
 *  
 *  \param [in] _data number to power
 *  \param [in] _pow power
 *  \return number^power
 *  
 *  \details 
 */
uint16_t UIntPow(uint16_t _data, uint8_t _pow){
        uint16_t _result = 1;
        if (!_pow)
			return 1;
        while (_pow){
                _result*=_data;
                _pow--;
        }
        return _result;         
}