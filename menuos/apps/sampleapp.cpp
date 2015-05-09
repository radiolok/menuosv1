/* 
* sampleapp.cpp
*
* Created: 09.05.2015 12:44:02
* Author: admin
*/


#include "sampleapp.h"

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
	tick = 0;
	Buttons.Add(SampleAppButtonsHandler);//add button handler
	Task.Add(1, SampleAppTaskHandler, 1000);//add task ha
	GLCD.ClearScreen();
	GLCD.CursorTo((HwDispGetStringsLength()-11)/2, HwDispGetStringsNumb()/2);
	GLCD.Puts("Sample Text");	
	return 0;
}

void SampleAppButtonsHandler(uint8_t button){
	SampleApp.ButtonsLogic(button);
}

void SampleAppTaskHandler(){
	SampleApp.TaskLogic();
}

uint8_t sampleapp::TaskLogic(void){
	GLCD.CursorTo((HwDispGetStringsLength()-11)/2, HwDispGetStringsNumb()/2+1);
	GLCD.PrintNumber(tick++);	
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


