/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#include "menu.h"

#include "config.h"

#include "util/log.h"

MConfig Config;

//we can't easily add class function to button slot
void MenuButtonsHandler(uint8_t button){
	Menu.ButtonsLogic(button);
}

MMenu::MMenu(){
	
	
}

MMenu::~MMenu(){
	
	
}


//Configure system
uint8_t MMenu::Setup(){
	//Prepare display with text LCD
	level = 0;
	HwDispSetup();
	
	SetButtons();
	
	//Return to root folder
	Return();
	return 0;
}
//устанавливает курсор в необходимую позицию.

uint8_t MMenu::ButtonsLogic(uint8_t button){
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
		//Menu start from top to bottom, so we has inverted buttons implementation
		case BUTTONUP:
			ButtonDown();
		break;
		case BUTTONDOWN:
			ButtonUp();
		break;
		default:
		
		break;
	
	}
	return 0;
}

uint8_t MMenu::SetButtons(void){
	//Get all buttons
	Buttons.Add(MenuButtonsHandler);
	return 0;
}

//Смена указателя вверх
void MMenu::ButtonUp(){
	log_trace("Button Up");
	if (cursor > 0){
		cursor--;
	}
    else{
		cursor = file.mode2 - 1;//go to first file in folder
	}
	brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBPAGE] = cursor / DISPSTRNUMB;
	brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBCURSOR] = cursor % DISPSTRNUMB;
	ShowFolder();
}

void MMenu::ButtonDown(){
	log_trace("Button Down");
	if (cursor < file.mode2 - 1){
		cursor++;
	}
    else{
		cursor = 0;//go to first file in folder
	}
	brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBPAGE] = cursor / DISPSTRNUMB;
	brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBCURSOR] = cursor % DISPSTRNUMB;
	ShowFolder();
}

void MMenu::ButtonLeft(){
	log_trace("Button Left");
	//n/a
}

void MMenu::ButtonRight(){
	log_trace("Button Up");
	//define type of current folder
	uint8_t newfilenum = 0;
	switch (file.type){
		case T_FOLDER://usual folder
			//in usual folder get start child number(mode1) and add cursor position
			newfilenum = file.mode1 + cursor;
		break;
		case T_DFOLDER://dynamic folder
			//in dnamic folder we has some copies of one file, so we use it
			newfilenum = file.mode1;
		break;
		case T_SFOLDER://currently not supported
		default:
			//n/a
		break;
		
	}
	Enter(newfilenum);
}


void MMenu::Return(){
	log_trace("Return");
	SetButtons();
	HwDispClearScreen();
	brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBPARENT] = 0;
	brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBPAGE] = 0;
	brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBCURSOR] = 0;
	if (level > 0){ 
		level--;
	}
	uint8_t filenumb=brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBPARENT];//go to parent file
	cursor = brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBPAGE] * DISPSTRNUMB + brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBCURSOR];
	HwFileGetInfo(filenumb, &file);
	ShowFolder();	
}

void MMenu::Enter(uint8_t filenumb){
	log_trace("Enter");
	if (MAXDEPTH < level){
		return; //do nothing
	}
	//go to next level:
	level++;
	brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBPARENT] = filenumb;//save our positions
	
	cursor = 0;
	brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBPAGE] = cursor / DISPSTRNUMB;
	brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBCURSOR] = cursor % DISPSTRNUMB;

	//Get new file data:
	HwFileGetInfo(filenumb, &file);
	
	//Do file action
	Action();
}

void MMenu::Action(void){
	
	switch (file.type){
		case T_FOLDER:
		case T_DFOLDER:
		case T_SFOLDER:
			ShowFolder();
		break;
		case T_APP:
			AppStart();
		break;
		case T_CONF:
			Task.Add(MENUSLOT, MenuAppStop, 10);//100 ms update
			Task.ActiveApp = 1;//app should release AtiveApp to zero itself
			Config.Setup(level, brCrumbs);
		break;
		default://Unsupported
			Return();
		break;
	}
}

//show current folder
void MMenu::ShowFolder(){
		
	//select folder type:
	uint8_t maxcursor = file.mode2 - brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBPAGE] * HwDispGetStringsNumb();
	char fname[DISPSTRLENGTH] = "\0";
	uint8_t ftype = 0;
	uint8_t fstart = 0;
	switch(file.type){
		case T_FOLDER:
			//first file index to output
			fstart = file.mode1 + brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBPAGE] * HwDispGetStringsNumb();
			for (uint8_t i = 0; i < maxcursor; i++){
				ftype = HwFileGetType(fstart + i);
				if (T_CONF == ftype){
					Config.GetString(fstart + i, fname, 0, 0);
					log_trace_txt("Item: ", fname);
					log_trace_val("Pos: ", i);
					HwDispPutString(i, 0, fname, HwDispGetStringsLength());
				}
				else{//show usual string
					HwFileGetName(fstart + i, fname);
					log_trace_txt("Item: ", fname);
					log_trace_val("Pos: ", i);
					HwDispPutString(i, 0, fname, HwDispGetStringsLength());
				}
			}
 		break;
		case T_DFOLDER:
			//read share name
			HwFileGetName(file.mode1, fname);
			fstart = brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBPAGE] * HwDispGetStringsNumb();
			char buff[20] = "\0";
			for (uint8_t i = 0; i < maxcursor; i++){
				//give filename look like: item0, item 1, item 2,  etc
				sprintf(buff, "%s %d", fname, fstart + i);
				log_trace_txt("Item: ", buff);
				log_trace_val("Pos: ", i);
				HwDispPutString(i, 0, buff, HwDispGetStringsLength());
			}
		break;	
	}
	//Crear another strings
	for (uint8_t i = maxcursor; i < HwDispGetStringsNumb(); i++){
		HwDispClearString(i, 0, HwDispGetStringsLength());
	}
	//Select current cursor position:
	HwDispSelectString(brCrumbs[(level*BRCRUMBSLENGTH) + CRUMBCURSOR]);
}


void MMenu::AppStart(void){
	if (file.mode2 != BACKGROUND){
		Task.Add(MENUSLOT, MenuAppStop, 10);//100 ms update
		Task.ActiveApp = 1;//app should release AtiveApp to zero itself
	}
	switch (file.mode1){//AppNumber
		case 2:
			SampleApp.Setup(level, brCrumbs);
		break;
		case 3:
			Clock.Setup(level, brCrumbs);
		break;
		default:
			Task.ActiveApp = 0;		
		break;
	}
}

//non-class C Task function
void MenuAppStop(void){
	if (!Task.ActiveApp)
	{
		Task.Release(MENUSLOT);//disable current handle
		Menu.Return();//return to menu
	}	
}