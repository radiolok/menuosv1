/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#ifndef MENU_H_
#define MENU_H_

#include "menudef.h"
#include "buttons.h"
#include "MTask.h"
#include "hwi.h"


class MMenu{
	private:
	
	//current menulevel position
	uint8_t level;
	
	//current cursor position
	uint8_t cursor;
	
	
	uint8_t brCrumbs[(MAXDEPTH+1) * BRCRUMBSLENGTH];
	
	//current file info structure
	filedata file;
	
	//buttons logic
	void ButtonUp(void);
	
	void ButtonDown(void);
	
	void ButtonLeft(void);
	
	void ButtonRight(void);
	
	
	//show cureent folder
	void ShowFolder(void);
	
	//Start application
	void AppStart(void);
	
			
	public:
		MMenu(void);
		
		~MMenu();
		
		/**
		 *  \brief Setup all internal menu logic
		 *   
		 *  \return 0
		 *  
		 *  \details 
		 */
		uint8_t Setup(void);
		
		/**
		 *  \brief Configure all buttons to menu logic
		 *  
		 *  \return 0
		 *  
		 *  \details Details
		 */
		uint8_t SetButtons(void);
		
		/**
		 *  \brief Brief
		 *  
		 *  \param [in] button - pressed button
		 *  \return Return_Description
		 *  
		 *  \details Details
		 */
		uint8_t ButtonsLogic(uint8_t button);
				
		/**
		 *  \brief Return to parent folder
		 *  
		 *  \return 0
		 *  
		 *  \details Cursor n main folder stay on last position
		 *  if we already in root folder, cursor become to zero
		 */
		void Return(void);
		
		/**
		 *  \brief Go to child file
		 *  
		 *  \param [in] filenumb child file number
		 *  \return 0
		 *  
		 *  \details Details
		 */
		void Enter(uint8_t filenumb);
		
		
		/**
		 *  \brief Execute selected file
		 *  
		 *  \return none
		 *  
		 *  \details Details
		 */
		void Action(void);

};

extern MMenu Menu;

/**
 *  \brief Buttons function toggle for Menu Buttons
 *  
 *  \return void
 *  
 *  \details called every time, if button is pressed
 */
void MenuButtonsHandler(void);

/**
 *  \brief Task function handlaer for automaticaly returning to menu
 *  
 *  \return none
 *  
 *  \details Details
 */
void MenuAppStop(void);

#endif /* MENU_H_ */