/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#include "menu.h"

//we can't easily add class function to button slot
void MenuButtonsHandler(uint8_t button){
	Menu.ButtonsLogic(button);
}


//Configure system
uint8_t Menu::Setup(){
	//Prepare display with text LCD
	HwDispSetup();
	
	SetButtons();
	
	//Return to root folder
	Return();
}
//устанавливает курсор в необходимую позицию.

uint8_t Menu::ButtonsLogic(uint8_t button){
	switch (button){
		case BUTTONLEFT:
			ButtonLeft();
		break;
		case BUTTONRIGHT:
		case BUTTONENTER:
			ButonRight();
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
	return 0;
}

uint8_t Menu::SetButtons(void){
	//Get all buttons
	Buttons.AddSlot(MenuButtonsHandler);
	
}

//Смена указателя вверх
void Menu::ButtonUp(){
	if (cursor > 0){
		cursor--;
	}
    else{
		cursor = file.mode2;//go to first file in folder
	}
	brCrumbs[level][CRUMBPAGE] = cursor / DISPSTRNUMB;
	brCrumbs[level][CRUMBCURSOR] = cursor % DISPSTRNUMB;
	ShowFolder();
}

void Menu::ButtonDown(){
	if (cursor < file.mode2){
		cursor++;
	}
    else{
		cursor = 0;//go to first file in folder
	}
	brCrumbs[level][CRUMBPAGE] = cursor / DISPSTRNUMB;
	brCrumbs[level][CRUMBCURSOR] = cursor % DISPSTRNUMB;
	ShowFolder();
}

void Menu::ButtonLeft(){
	//n/a
}

void Menu::ButtonRight(){
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


void Menu::Return(){
	SetButtons();
	HwDispClearScreen();
	brCrumbs[level][CRUMBPARENT] = 0;
	brCrumbs[level][CRUMBPAGE] = 0;
	brCrumbs[level][CRUMBCURSOR] = 0;
	if (level > 0){ 
		level--;
	}
	filenumb=brCrumbs[level][CRUMBPARENT];//go to parent file
	cursor = brCrumbs[level][CRUMBPAGE] * DISPSTRNUMB + brCrumbs[level][CRUMBCURSOR];
	HwFileGetInfo(filenumb, file);
	ShowFolder();	
}

void Menu::Enter(uint8_t filenumb){
	if (MAXDEPTH < level){
		return; //do nothing
	}
	//go to next level:
	level++;
	brCrumbs[level][CRUMBPARENT] = filenumb;//save our positions
	brCrumbs[level][CRUMBPAGE] = 0;
	brCrumbs[level][CRUMBCURSOR] = 0;

	//Get new file data:
	HwFileGetInfo(filenumb, file);
	
	//Do file action
	Action();
}

void Menu::Action(void){
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
			ConfigSetup(file, brCrumbs[level - 1][CRUMBCURSOR]);
		break;
		default://Unsupported
			Return();
		break;
	}
}

//show current folder
void Menu::ShowFolder(){
	
	
	//select folder type:
	uint8_t maxcursor = file.mode2 - brCrumbs[level][CRUMBPAGE] * HwGetSringsNumb();
	//TODO: do not forget to rewrite
	uint8_t fname[HwGetSringsLength()] = "\0";
	uint8_t ftype = 0;
	switch(file.type){
		case T_FOLDER:
			//first file index to output
			fstart=file.mode1 + brCrumbs[level][CRUMBPAGE] * HwGetSringsNumb());
			for (uint8_t i = 0; i < maxcursor; i++){
				ftype = HwGetFileType(fstart + i);
				if (T_CONF == ftype){
					uint8_t length = Config.GetString(fstart + i, fname, 0);
					HwDispPutString(brCrumbs[level][CRUMBSURCOR], 0, fname, length);
				}
				else{//show usual string
					HwFileGetName(fstart + i, fname);
					HwDispPutString(brCrumbs[level][CRUMBSURCOR], 0, fname, HwGetSringsLength());
				}
			}
 		break;
		case T_DFOLDER:
			//read share name
			HwGetFileName(file.mode1, fname);
			uint8_t buff[20] = "\0";
			for (uint8_t i = 0; i < maxcursor; i++){
				//give filename look like: item0, item 1, item 2,  etc
				sprintf(buff, "%s %d", fname, cursor);
				HwDispPutString(i, 0, fname, HwGetSringsLength());
			}
		break;	
	}
	//Crear another strings
	for (uint8_t i = maxcursor; i < HwGetSringsNumb(); i++){
		HwDispClearString(i, 0, HwGetSringsLength());
	}
	//Select current cursor position:
	HwDispSelectString(brCrumb[level][CRUMBCURSOR]);
}


void Menu::AppStart(void){
	if (file.mode2 != BACKGROUND){
		Task.Add(1, MenuAppStop, 10);//100 ms update
		Task.ActiveApp = 1;//app should release AtiveApp to zero itself
	}
	switch (file.mode1){//AppNumber
		case 1:
			VersionInfo();
			Task.ActiveApp = 0;
		break;
		default:
		
		break;
	}
}

//non-class C Task function
void MenuAppStop(void){
	if (!TManager.ActiveApp)
	{
		Task.Release(MENUSLOT);//disable current handle
		Menu.Return();//return to menu
	}	
}