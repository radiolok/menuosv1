/*Copyright (c) 2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/**
 *  \file test_menu.cpp
 *  \brief Host tests: drive the real menu code with simulated buttons and
 *  compare what the terminal display shows.
 *
 *  Note on buttons: as on the original hardware, BUTTONUP is "+" (next item,
 *  increase value) and BUTTONDOWN is "-" (previous item, decrease value).
 */
#include <stdio.h>
#include <string.h>

#include "menuos/port/port.h"
#include "menuos/MTask.h"
#include "menuos/buttons.h"
#include "menuos/menu.h"
#include "menuos/hw/driver/disp/term/termdisp.h"

static int failures = 0;
static int checks = 0;

static void Press(uint8_t mask){
	HostPress(mask, 150, 150);
}

static void Expect(const char *name, const char *expected){
	char frame[TERMDISP_FRAME_SIZE];
	TermDispRender(frame, sizeof(frame));
	checks++;
	if (strcmp(frame, expected) != 0){
		failures++;
		printf("FAIL %s\n--- expected\n%s--- got\n%s\n", name, expected, frame);
	}
	else{
		printf("ok   %s\n", name);
	}
}

#define BORDER "+----------------+\n"

static void TestNavigation(void){
	Expect("boot shows root folder",
		BORDER
		">Folder 1        <\n"
		"|Folder 2        |\n"
		"|                |\n"
		"|                |\n"
		"|                |\n"
		BORDER);

	Press(BUTTONUP);
	Expect("UP moves to the next item",
		BORDER
		"|Folder 1        |\n"
		">Folder 2        <\n"
		"|                |\n"
		"|                |\n"
		"|                |\n"
		BORDER);

	Press(BUTTONUP);
	Expect("cursor wraps around",
		BORDER
		">Folder 1        <\n"
		"|Folder 2        |\n"
		"|                |\n"
		"|                |\n"
		"|                |\n"
		BORDER);

	Press(BUTTONRIGHT);
	Expect("RIGHT enters a folder",
		BORDER
		">App 1           <\n"
		"|App 2           |\n"
		"|Dyn Folder      |\n"
		"|                |\n"
		"|                |\n"
		BORDER);

	Press(BUTTONRETURN);
	Expect("RETURN goes back and keeps the cursor",
		BORDER
		">Folder 1        <\n"
		"|Folder 2        |\n"
		"|                |\n"
		"|                |\n"
		"|                |\n"
		BORDER);
}

static void TestDynamicFolder(void){
	Press(BUTTONRIGHT);        // Folder 1
	Press(BUTTONDOWN);         // wrap back to the last item: Dyn Folder
	Press(BUTTONRIGHT);
	Expect("dynamic folder lists numbered copies",
		BORDER
		">App 0           <\n"
		"|App 1           |\n"
		"|App 2           |\n"
		"|App 3           |\n"
		"|App 4           |\n"
		BORDER);

	for (int i = 0; i < 6; i++){
		Press(BUTTONUP);
	}
	Expect("dynamic folder pages by display height",
		BORDER
		"|App 5           |\n"
		">App 6           <\n"
		"|App 7           |\n"
		"|App 8           |\n"
		"|App 9           |\n"
		BORDER);

	Press(BUTTONDOWN);         // to App 5..
	for (int i = 0; i < 6; i++){
		Press(BUTTONDOWN);     // ..and wrap to the last copy
	}
	Expect("dynamic folder wraps to the last page (66 copies: App 0..App 65)",
		BORDER
		">App 65          <\n"
		"|                |\n"
		"|                |\n"
		"|                |\n"
		"|                |\n"
		BORDER);

	Press(BUTTONRETURN);
	Press(BUTTONRETURN);
}

static void TestConfigEditor(void){
	Press(BUTTONUP);           // Folder 2
	Press(BUTTONRIGHT);
	Press(BUTTONUP);           // config 1
	Press(BUTTONRIGHT);        // edit
	Expect("config editor shows a digit cursor",
		BORDER
		"|config 0     Yes|\n"
		"|config 1      -1|\n"
		"|config 2      -1|\n"
		"|Date and Time   |\n"
		"|                |\n"
		BORDER
		"                ^\n");

	Press(BUTTONUP);           // -1 + 1 = 0
	Press(BUTTONLEFT);         // tens
	Press(BUTTONUP);           // +10
	Press(BUTTONUP);           // +10
	Expect("UP adds 10^digit, LEFT selects the next digit",
		BORDER
		"|config 0     Yes|\n"
		"|config 1      20|\n"
		"|config 2      -1|\n"
		"|Date and Time   |\n"
		"|                |\n"
		BORDER
		"               ^ \n");

	Press(BUTTONRETURN);       // save
	Expect("RETURN saves the value",
		BORDER
		"|config 0     Yes|\n"
		">config 1      20<\n"
		"|config 2      -1|\n"
		"|Date and Time   |\n"
		"|                |\n"
		BORDER);

	if ((int16_t)PortNvRead16(2) != 20){
		failures++;
		printf("FAIL config 1 stored in NV: %d\n", (int16_t)PortNvRead16(2));
	}
	else{
		printf("ok   config 1 stored in NV\n");
	}
	checks++;

	Press(BUTTONDOWN);         // config 0, a Yes/No value (erased cell reads as "Yes")
	Press(BUTTONRIGHT);
	Press(BUTTONUP);           // toggle
	Press(BUTTONRETURN);
	Expect("Yes/No config toggles to No",
		BORDER
		">config 0      No<\n"
		"|config 1      20|\n"
		"|config 2      -1|\n"
		"|Date and Time   |\n"
		"|                |\n"
		BORDER);

	Press(BUTTONRIGHT);
	Press(BUTTONDOWN);         // toggle back
	Press(BUTTONRETURN);
	Expect("Yes/No config toggles back to Yes",
		BORDER
		">config 0     Yes<\n"
		"|config 1      20|\n"
		"|config 2      -1|\n"
		"|Date and Time   |\n"
		"|                |\n"
		BORDER);

	Press(BUTTONRETURN);
}

static void TestSampleApp(void){
	Press(BUTTONDOWN);         // root remembers Folder 2, step back to Folder 1
	Press(BUTTONRIGHT);
	Press(BUTTONUP);           // App 2 = sample app
	Press(BUTTONRIGHT);
	HostAdvance(2100);
	Expect("sample app draws and ticks every second",
		BORDER
		"|                |\n"
		"|  Sample Text   |\n"
		"|  1             |\n"
		"|                |\n"
		"|                |\n"
		BORDER);

	Press(BUTTONRETURN);
	Expect("leaving the app returns to the menu",
		BORDER
		"|App 1           |\n"
		">App 2           <\n"
		"|Dyn Folder      |\n"
		"|                |\n"
		"|                |\n"
		BORDER);
}

int main(void){
	PortInit();
	Buttons.Setup(100);
	Task.Setup();
	Menu.Setup();
	Task.HwStart();
	HostAdvance(200);

	TestNavigation();
	TestDynamicFolder();
	TestConfigEditor();
	TestSampleApp();

	printf("\n%d checks, %d failed\n", checks, failures);
	return failures ? 1 : 0;
}
