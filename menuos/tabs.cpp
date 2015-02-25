/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#include "/menuos/tabs.h"
#include "/menuos/buttons.h"

#include "taskmanager.h"
#include "definitions.h"

uint8_t ActiveTab=0;
uint8_t ActiveTabOld=5;

/**
 *  \brief Draw Tabs panel
 *  
 *  \param [in] TabStartX upper left X position
 *  \param [in] TabStartY upper left Y position
 *  \param [in] TabLabelHeight Tabs label height
 *  \param [in] TabWidth Tabs panel width
 *  \param [in] TabHeight Tabs panel height
 *  \param [in] ActiveTab current active tab
 *  \param [in] TabsNumber tabs number
 *  \return void
 *  
 *  \details Details
 */
void DrawTabs(uint8_t TabStartX, uint8_t TabStartY, uint8_t TabLabelHeight, uint8_t TabWidth, uint8_t TabHeight, uint8_t ActiveTab, uint8_t TabsNumber){
	uint8_t LabelWidth = (TabWidth-9) / TabsNumber;//label width
	uint8_t LabelBorder = ((TabWidth-(LabelWidth+1)*TabsNumber)>>1);//label indent from left and right corner
	uint8_t LabelStartX = TabStartX+LabelBorder+(1+LabelWidth)*ActiveTab;//active label start position
	if (ActiveTab!=ActiveTabOld){//если открываем новый таб:
		DrawTabBorder(TabStartX, TabStartY,  TabLabelHeight,  TabWidth,  TabHeight,  LabelStartX,  LabelWidth);
		DrawTabLabels(TabStartX,TabStartY,  TabLabelHeight,  TabWidth,  LabelBorder,  LabelWidth,  TabsNumber);
		GLCD.FillRect(TabStartX+1,TabStartY+TabLabelHeight+1,TabWidth-2,TabHeight-TabLabelHeight-2,WHITE);//clear active region
	}	
	TabShowInfo(TabStartX, TabStartY+TabLabelHeight, TabWidth, TabHeight-TabLabelHeight, ActiveTab);//output data
	ActiveTabOld=ActiveTab;//
}

/**
 *  \brief Draw tabs panel main border
 *  
 *  \param [in] TabStartX upper left X position
 *  \param [in] TabStartY upper left Y position
 *  \param [in] TabLabelHeight Tabs label height
 *  \param [in] TabWidth Tabs panel width
 *  \param [in] TabHeight Tabs panel height
 *  \param [in] LabelStartX Active label start position
 *  \param [in] LabelWidth label width
 *  \return coid
 *  
 *  \details Details
 */
void DrawTabBorder(uint8_t TabStartX, uint8_t TabStartY, uint8_t TabLabelHeight, uint8_t TabWidth, uint8_t TabHeight, uint8_t LabelStartX, uint8_t LabelWidth){
	GLCD.DrawLine(TabStartX,TabStartY+TabLabelHeight,TabStartX,TabStartY+TabHeight);//left
	if (TabStartY+TabHeight>GLCD.Height) TabHeight=GLCD.Height-1-TabStartY;//boundary check
	GLCD.DrawLine(TabStartX,TabStartY+TabHeight,TabStartX+TabWidth,TabStartY+TabHeight);//down
	GLCD.DrawLine(TabStartX+TabWidth,TabStartY+TabHeight,TabStartX+TabWidth,TabStartY+TabLabelHeight);//right
	GLCD.DrawLine(TabStartX,TabStartY+TabLabelHeight,TabStartX+TabWidth,TabStartY+TabLabelHeight);//up
	GLCD.DrawHLine(LabelStartX,TabStartY+TabLabelHeight,LabelWidth,WHITE);//do not cut active tab label
}

/**
 *  \brief Draw tabs labels
 *  
 *  \param [in] TabStartX upper left X position
 *  \param [in] TabStartY upper left Y position
 *  \param [in] TabLabelHeight Tabs label height
 *  \param [in] TabWidth Tabs panel width
 *  \param [in] LabelBorder 
 *  \param [in] LabelWidth label width in pixels
 *  \param [in] TabsNumber tabs number
 *  \return void
 *  
 *  \details Details
 */
void DrawTabLabels(uint8_t TabStartX, uint8_t TabStartY, uint8_t TabLabelHeight, uint8_t TabWidth, uint8_t LabelBorder, uint8_t LabelWidth, uint8_t TabsNumber){
		GLCD.DrawLine(TabStartX+LabelBorder,TabStartY,TabStartX+TabWidth-LabelBorder,TabStartY);
		for (TabsNumber;TabsNumber<128;TabsNumber--){
			DrawTabIcons(TabStartX+LabelBorder+(TabsNumber-1)*(LabelWidth+1)+LabelWidth/2,TabStartY+TabLabelHeight/2,TabsNumber-1);
			GLCD.DrawVLine(TabStartX+LabelBorder+TabsNumber*(LabelWidth+1),TabStartY,TabLabelHeight);
		}			
}

/**
 *  \brief Draw icns inside labels
 *  
 *  \param [in] IconCenterX icon X center position
 *  \param [in] IconCenterY icon Y center position
 *  \param [in] TabNumber number of the tab
 *  \return Return_Description
 *  
 *  \details Details
 */
void DrawTabIcons(uint8_t IconCenterX, uint8_t IconCenterY, uint8_t TabNumber){
	switch (TabNumber){
		case TABSTATUS://status
			GLCD.DrawBitmap(info,IconCenterX-9,IconCenterY-4);
		break;
		case TABDELAY://delay
			GLCD.DrawCircle(IconCenterX,IconCenterY,4);
			GLCD.DrawVLine(IconCenterX,IconCenterY-4,4);
			GLCD.DrawLine(IconCenterX,IconCenterY,IconCenterX-3,IconCenterY-3);
		break;
		case TABSPEED://speed
			GLCD.DrawBitmap(speed,IconCenterX-9,IconCenterY-4);
		break;
		case TABUNDO://Undo/Redo
			GLCD.DrawBitmap(undo,IconCenterX-9,IconCenterY-4);
		break;
		
	}
}

/**
 *  \brief Show current tab
 *  
 *  \param [in] TabInfoStartX Parameter_Description
 *  \param [in] TabInfoStartY Parameter_Description
 *  \param [in] TabInfoWidth Parameter_Description
 *  \param [in] TabInfoHeight Parameter_Description
 *  \param [in] ActiveTab Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void TabShowInfo(uint8_t TabInfoStartX, uint8_t TabInfoStartY, uint8_t TabInfoWidth, uint8_t TabInfoHeight, uint8_t ActiveTab){
	TabButtonsSelect();//set buttns for the current tab
	switch (ActiveTab){
		case TABSTATUS:
			TabStatusShow(TabInfoStartX,TabInfoStartY,TabInfoWidth,TabInfoHeight);
		break;
		case TABDELAY:
			TabDelayShow(TabInfoStartX,TabInfoStartY,TabInfoWidth,TabInfoHeight);
		break;
		case TABSPEED:
			TabSpeedShow(TabInfoStartX,TabInfoStartY,TabInfoWidth,TabInfoHeight);
		break;
		case TABUNDO:
			TabUndoShow(TabInfoStartX,TabInfoStartY,TabInfoWidth,TabInfoHeight);
		break;
	}
}

/**
 *  \brief Main tabs showing function
 *  
 *  \return void
 *  
 *  \details Details
 */
void TabsViewSetup(){
	GLCD.FillRect(0,0,128,64,WHITE);
	ActiveTab=0;
	ActiveTabOld=5;
	BSlot(BUTTONSLOTRETURN,*TabButtonsReturn);//return to main menu
	BSlot(BUTTONSLOTLEFT,TabButtonsSearchLeft);//list tabs left
	BSlot(BUTTONSLOTRIGHT,TabButtonsSearchRight);//list tabs right
	TabButtonsSelect();//select other tabs buttons
	TabRefresh();
}

/**
 *  \brief Select active tab buttons function
 *  
 *  \return coid
 *  
 *  \details Details
 */
void TabButtonsSelect(){
	switch(ActiveTab){
		case TABSTATUS:
			BSlot(BUTTONSLOTUP,*TabButtonsInfoUp);//start/stop
			BSlot(BUTTONSLOTDOWN,*TabButtonsInfoDown);//pause/start
			BSlot(BUTTONSLOTENTER,*TabButtonsInfoEnter);//start/stop
		break;
		case TABDELAY:
			BSlot(BUTTONSLOTUP,*TabButtonsDelayUp);//delay increase
			BSlot(BUTTONSLOTDOWN,*TabButtonsDelayDown);//delay decrease
			BSlot(BUTTONSLOTENTER,NULL);//no function
		break;
		case TABSPEED:
			BSlot(BUTTONSLOTUP,*TabButtonsSpeedUp);//speed up
			BSlot(BUTTONSLOTDOWN,*TabButtonsSpeedDown);//speed down
			BSlot(BUTTONSLOTENTER,NULL);//no function	
		break;
		case TABUNDO:
			BSlot(BUTTONSLOTUP,*TabButtonsUndoUp);//undo up
			BSlot(BUTTONSLOTDOWN,*TabButtonsUndoDown);//undo down
			BSlot(BUTTONSLOTENTER,NULL);//no function
		break;		
	}
	
}

/**
 *  \brief Refresh active tab
 *  
 *  \return coid
 *  
 *  \details all coordinates are started here
 */
void TabRefresh(){
   /*put your code here*/
   DrawTabs(3,23,11, 120, 40, ActiveTab, 4);
}

/**
 *  \brief Tabs switch right button
 *  
 *  \return void
 *  
 *  \details Details
 */
void TabButtonsSearchRight(){
		ActiveTab++;
		if (ActiveTab>3) ActiveTab=0;
		TabButtonsSelect();
TabRefresh();
}

/**
 *  \brief Tabs switch left button
 *  
 *  \return void
 *  
 *  \details Details
 */
void TabButtonsSearchLeft(){
		ActiveTab--;
		if (ActiveTab>128) ActiveTab=3;
		TabButtonsSelect();
TabRefresh();
}

/**
 *  \brief Tabs switch return button. 
 *  
 *  \return Return_Description
 *  
 *  \details Return to main menu
 */
void TabButtonsReturn(){
	/*
	Put you code here
	*/
		TManager.WorkType=0;//return to main menu

}

void TabButtonsInfoUp(){
	/*
	Put you code here
	*/
TabRefresh();
}

void TabButtonsInfoDown(){
	/*
	Put you code here
	*/
TabRefresh();
}

void TabButtonsInfoEnter(){
	/*
	Put you code here
	*/
TabRefresh();
}

void TabButtonsSpeedUp(){
	/*
	Put you code here
	*/
TabRefresh();	
}

void TabButtonsSpeedDown(){
	/*
	Put you code here
	*/
	TabRefresh();
}

void TabButtonsUndoUp(){
	/*
	Put you code here
	*/
	TabRefresh();
}

void TabButtonsUndoDown(){
	/*
	Put you code here
	*/
TabRefresh();
}

void TabButtonsDelayUp(){
	/*
	Put you code here
	*/
TabRefresh();	
}

void TabButtonsDelayDown(){
	/*
	Put you code here
	*/
TabRefresh();
}


void TabStatusShow(uint8_t TabInfoStartX, uint8_t TabInfoStartY, uint8_t TabInfoWidth, uint8_t TabInfoHeight){
		GLCD.SelectFont(SystemFont5x7);
		GLCD.CursorToXY(TabInfoStartX+2,TabInfoStartY+2);
		char buffer[10];
		strcpy_P(buffer, (char*)pgm_read_word(&(PhotoStatus[Photo.Status])));
		GLCD.Puts(buffer);//status string
		GLCD.CursorToXY(TabInfoStartX+2,TabInfoStartY+10);
		if (1){//drive stopped
			GLCD.Puts("Enter to Start");
			GLCD.CursorToXY(TabInfoStartX+2,TabInfoStartY+18);
				GLCD.Puts("                  ");
		}				
		else{
			GLCD.Puts("Enter to Stop ");
			GLCD.CursorToXY(TabInfoStartX+2,TabInfoStartY+18);
			if (Photo.Status==1)
				GLCD.Puts("V or ^ to Continue");
			else 
				GLCD.Puts("V or ^ to Pause   ");
		}
}

void TabDelayShow(uint8_t TabInfoStartX, uint8_t TabInfoStartY, uint8_t TabInfoWidth, uint8_t TabInfoHeight){
	 uint8_t DataDigits[3];
	  uint8_t DataUnsignedLog=Photo.PhotoDelay;
	 for (uint8_t digits_count=0;digits_count<3;digits_count++)
        {
      if (DataUnsignedLog>0){
        DataDigits[digits_count] =DataUnsignedLog % 10;
        DataUnsignedLog /= 10;
          }
          else
          {
                DataDigits[digits_count]=0;                               
          }
        }
	GLCD.SelectFont(SystemFont5x7);
	GLCD.CursorToXY(TabInfoStartX+2,TabInfoStartY+2);
	GLCD.Puts("Delay: ");
	if (DataDigits[2])
		GLCD.PrintNumber(DataDigits[2]);
	GLCD.PrintNumber(DataDigits[1]);
	GLCD.Puts(".");
	GLCD.PrintNumber(DataDigits[0]);	
	GLCD.Puts(" s   ");
	
}

void TabSpeedShow(uint8_t TabInfoStartX, uint8_t TabInfoStartY, uint8_t TabInfoWidth, uint8_t TabInfoHeight){
	GLCD.SelectFont(SystemFont5x7);
	GLCD.CursorToXY(TabInfoStartX+2,TabInfoStartY+2);
	GLCD.Puts("Speed: ");
	GLCD.PrintNumber(0);
	GLCD.Puts(" %   ");	
	
}

void TabUndoShow(uint8_t TabInfoStartX, uint8_t TabInfoStartY, uint8_t TabInfoWidth, uint8_t TabInfoHeight){
	GLCD.SelectFont(SystemFont5x7);
	GLCD.CursorToXY(TabInfoStartX+2,TabInfoStartY+2);
	GLCD.Puts("Photo: ");
	GLCD.PrintNumber(1);
	GLCD.Puts(" of ");	
	GLCD.PrintNumber(10);	
	GLCD.Puts("   ");
}