# Writing an application

An application is any code started from a `T_APP` file. While it runs it owns the display
and the buttons; when it is done it tells the menu, and the menu redraws itself.

The smallest complete example is [`menuos/apps/sampleapp.cpp`](../menuos/apps/sampleapp.cpp):
it prints a line and a counter that grows every second, and leaves on `RETURN`.

## Life cycle

```
 menu: RIGHT on a T_APP file
   │
   ├─ Task.Add(MENUSLOT, MenuAppStop, 10)   watcher in slot 0, every 10 ms
   ├─ Task.ActiveApp = 1
   └─ YourApp.Setup(level, brCrumbs)
          │
          ├─ Buttons.Add(YourButtonsHandler)   take over the buttons
          ├─ Task.Add(1, YourTask, period_ms)  optional periodic work
          └─ draw the first screen
   ...
 RETURN pressed → your handler:
          ├─ Task.Release(1)                   free your slots
          └─ Task.ActiveApp = 0
   │
 watcher sees ActiveApp == 0 → releases slot 0 → Menu.Return()
```

`Menu.Return()` gives the buttons back to the menu and redraws the folder the app was
started from, with the cursor where it was.

## Skeleton

```cpp
// apps/myapp.h
#include <stdint.h>

class MyApp {
public:
    uint8_t Setup(uint8_t argc, uint8_t *argv);
    void OnButton(uint8_t button);
    void Tick();
};
extern MyApp App;

// apps/myapp.cpp
#include "myapp.h"
#include "../MTask.h"
#include "../buttons.h"
#include "../hw/hwi.h"

MyApp App;
static void ButtonsHandler(uint8_t b) { App.OnButton(b); }
static void TaskHandler()             { App.Tick(); }

uint8_t MyApp::Setup(uint8_t argc, uint8_t *argv) {
    (void)argc; (void)argv;          // menu level and breadcrumbs, see below
    Buttons.Add(ButtonsHandler);
    Task.Add(1, TaskHandler, 500);   // every 500 ms
    HwDispClearScreen();
    HwDispPutString(0, 0, "Hello", HwDispGetStringsLength());
    return 0;
}

void MyApp::OnButton(uint8_t button) {
    if (button == BUTTONRETURN) {
        Task.Release(1);
        Task.ActiveApp = 0;          // back to the menu
    }
}

void MyApp::Tick() { /* periodic work, runs in the main loop */ }
```

Register it in `MMenu::AppStart()` (`menuos/menu.cpp`) under a free number, add the source
to `CORE_SRC` in the `Makefile`, and point a `T_APP` file's `mode1` at that number.

## Building blocks

**Display.** Use the `HwDisp*` functions from `hw/driver/disp/hwdisp.h` and the app works
on every display backend: `HwDispPutString(row, col, text, len)`, `HwDispClearString`,
`HwDispSelectString(row)`, `HwDispDrawCursor(row, col, len)`, `HwDispClearScreen()`. On the
GLCD you can also call the `GLCD` object directly, as `clock.cpp` does, at the cost of
portability.

**Buttons.** `Buttons.Add(handler)` sends every button to one handler; the argument is the
button mask (`BUTTONUP`, `BUTTONRETURN`, …). Buttons are polled every 100 ms and only the
press edge is reported. `Buttons.Add(slot, handler)` connects a single button.

**Tasks.** `Task.Add(slot, fn, period_ms)` runs `fn` from the main loop every `period_ms`
milliseconds; `Task.Release(slot)` stops it. There are 8 slots (`PSLOTS`); slot 0 is the
menu's watcher. Task functions are never called from an interrupt, so they may draw and
take their time — but nothing else runs until they return.

**Time.** `Task.Millis()` returns milliseconds since start.

**Settings.** An app can read and write config cells with `HwConfigGetData()` /
`HwConfigSetData()` (see [menu-table.md](menu-table.md)).

**Where the app was started from.** `Setup()` receives `argc` = the menu level and `argv` =
the breadcrumbs array, 3 bytes per level: parent file id, page, cursor row. An app inside a
dynamic folder can compute which copy it was started for, the same way
`MConfig::Setup()` does.

**Background apps.** If the file's `mode2` is `BACKGROUND`, the menu does not install the
watcher and does not wait for `ActiveApp`; the app is on its own.
