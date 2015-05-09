/* 
* sampleapp.h
*
* Created: 09.05.2015 12:44:02
* Author: admin
*/


#ifndef __SAMPLEAPP_H__
#define __SAMPLEAPP_H__

#include "hw/hwi.h"

#include "menuos/MTask.h"
#include "menuos/buttons.h"

class sampleapp
{
//variables
public:
	uint8_t  Setup(uint8_t argc, uint8_t *argv);
	uint8_t  ButtonsLogic(uint8_t button);
protected:
private:

	void Return();

//functions
public:
	sampleapp();
	~sampleapp();
protected:
private:

}; //sampleapp
extern sampleapp SampleApp;

void SampleAppButtonsHandler(uint8_t button);

#endif //__SAMPLEAPP_H__
