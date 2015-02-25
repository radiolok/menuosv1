/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#ifndef TABS_H_
#define TABS_H_


#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "glcd/glcd.h"
#include "glcd/fonts/SystemFont5x7.h"
#include "glcd/fonts/Arial_bold_14.h"
#include "glcd/bitmaps/info.h"
#include "glcd/bitmaps/speed.h"
#include "glcd/bitmaps/undo.h"


#define TABSTATUS 0
#define TABDELAY 1
#define TABSPEED 2
#define TABUNDO 3

void TabsViewSetup(void);
void TabButtonsSelect(void);
void TabButtonsSearchRight(void);
void TabButtonsSearchLeft(void);
void TabButtonsReturn(void);
void TabButtonsInfoUp(void);
void TabButtonsInfoDown(void);
void TabButtonsInfoEnter(void);
void TabButtonsSpeedUp(void);
void TabButtonsSpeedDown(void);
void TabButtonsUndoUp(void);
void TabButtonsUndoDown(void);
void TabButtonsDelayUp(void);
void TabButtonsDelayDown(void);
void TabRefresh(void);

void DrawTabs(uint8_t TabStartX, uint8_t TabStartY, uint8_t TabLabelHeight, uint8_t TabWidth, uint8_t TabHeight, uint8_t ActiveTab, uint8_t TabsNumber);
void DrawTabBorder(uint8_t TabStartX, uint8_t TabStartY, uint8_t TabLabelHeight, uint8_t TabWidth, uint8_t TabHeight, uint8_t LabelStartX, uint8_t LabelWidth);
void DrawTabLabels(uint8_t TabStartX, uint8_t TabStartY, uint8_t TabLabelHeight, uint8_t TabWidth, uint8_t LabelBorder, uint8_t LabelWidth, uint8_t TabsNumber);
void DrawTabIcons(uint8_t IconCenterX, uint8_t IconCenterY, uint8_t TabNumber);
void TabShowInfo(uint8_t TabInfoStartX, uint8_t TabInfoStartY, uint8_t TabInfoWidth, uint8_t TabInfoHeight, uint8_t ActiveTab);
void TabStatusShow(uint8_t TabInfoStartX, uint8_t TabInfoStartY, uint8_t TabInfoWidth, uint8_t TabInfoHeight);
void TabDelayShow(uint8_t TabInfoStartX, uint8_t TabInfoStartY, uint8_t TabInfoWidth, uint8_t TabInfoHeight);
void TabSpeedShow(uint8_t TabInfoStartX, uint8_t TabInfoStartY, uint8_t TabInfoWidth, uint8_t TabInfoHeight);
void TabUndoShow(uint8_t TabInfoStartX, uint8_t TabInfoStartY, uint8_t TabInfoWidth, uint8_t TabInfoHeight);


#endif /* TABS_H_ */