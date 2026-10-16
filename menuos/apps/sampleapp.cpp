/* 
* sampleapp.cpp
*
* Created: 09.05.2015 12:44:02
* Author: admin
*/


#include "sampleapp.h"
#include "../util/console.h"

static const char sampleText[] = "Sample Text";

// default constructor
sampleapp::sampleapp()
{
} //sampleapp

// default destructor
sampleapp::~sampleapp()
{
} //~sampleapp



uint8_t sampleapp::Setup(uint8_t argc, uint8_t *argv)
{
	(void)argc;
	(void)argv;
	tick = 0;
	Buttons.Add(SampleAppButtonsHandler);//add button handler
	Task.Add(1, SampleAppTaskHandler, 1000);//add task ha
	HwDispClearScreen();
	HwDispPutString(HwDispGetStringsNumb()/2 - 1, (HwDispGetStringsLength() - (sizeof(sampleText) - 1))/2,
		sampleText, HwDispGetStringsLength());
	return 0;
}

void SampleAppButtonsHandler(uint8_t button){
	SampleApp.ButtonsLogic(button);
}

void SampleAppTaskHandler(){
	SampleApp.TaskLogic();
}

uint8_t sampleapp::TaskLogic(void){
	char text[8];
	menuos_itoa((int16_t)tick++, text);
	HwDispPutString(HwDispGetStringsNumb()/2, (HwDispGetStringsLength() - (sizeof(sampleText) - 1))/2,
		text, HwDispGetStringsLength());
	return 0;
}

uint8_t sampleapp::ButtonsLogic(uint8_t button){
	switch (button){
		case BUTTONLEFT:
		
		break;
		case BUTTONRIGHT:
	
		break;
		case BUTTONRETURN:
		Return();
		break;
		case BUTTONUP:
		
		break;
		case BUTTONDOWN:

		break;
		default:
		
		break;
		
	}
	return 0;
}


void sampleapp::Return(){
	Task.Release(1);//release slot with our programm
	Task.ActiveApp = 0;
}


