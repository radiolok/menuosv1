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

	Buttons.Add(SampleAppButtonsHandler);
	GLCD.ClearScreen();
	GLCD.CursorTo(HwDispGetStringsNumb()/2,(HwDispGetStringsLength()-11)/2);
	GLCD.Puts("Sample Text");	
	return 0;
}

void SampleAppButtonsHandler(uint8_t button){
	SampleApp.ButtonsLogic(button);
	
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
	Task.ActiveApp = 0;
}


