# The menu table

The whole menu lives in [`menuos/hw/driver/files/hwfile.cpp`](../menuos/hw/driver/files/hwfile.cpp)
as three constant arrays placed in program memory:

| Array | What it holds |
|---|---|
| `fileStruct[]` | one 4-byte record per file: `type, parent, mode1, mode2` |
| `fileNames[]` | a name for every file, in the same order |
| `configsLimit[]` | for every config: storage address, minimum, maximum |

A file is addressed by its index in `fileStruct[]` — its **id**. File `0` is the root folder.
`FILENUMB` at the top of the file must match the number of records.

![Default menu tree](img/menu-tree.svg)

## File types

| Type | `mode1` | `mode2` | Status |
|---|---|---|---|
| `T_FOLDER` | id of the first child | number of children | works |
| `T_DFOLDER` | id of the file to repeat | number of copies | works |
| `T_APP` | application number (see `MMenu::AppStart`) | `0`, or `BACKGROUND` | works |
| `T_CONF` | config id: row in `configsLimit[]` | unused | works |
| `T_SFOLDER`, `T_SCONF`, `T_DCONF` | — | — | reserved, not implemented |

`parent` is informational; navigation uses the breadcrumbs the menu keeps while you move
down the tree, so a file can be reached from more than one folder.

### Folders

Children of a folder must have **consecutive ids**: the folder stores only the first one
and the count. Lay the table out level by level, as in the default table:

```c
T_FOLDER, 0, 1, 2,     // 0  Root: children 1..2
    T_FOLDER, 0, 3, 3, // 1  Folder 1: children 3..5
    T_FOLDER, 0, 7, 4, // 2  Folder 2: children 7..10
        T_APP,  1, 1, 0, // 3
        ...
```

If a folder has more children than the display has rows, the menu pages through them.

### Dynamic folders

A `T_DFOLDER` shows one file `mode2` times, named `"<name> 0"`, `"<name> 1"`, … The copies
are not stored anywhere; the only thing that changes between them is the position. Configs
reached through a dynamic folder use it to pick their own storage cell: the copy number is
added to the config address (in 16-bit words). With nested dynamic folders the positions
add up.

This is how one table can describe, say, 8 channels × 4 settings without listing 32
configs — the "10^616 files" in the original description comes from nesting dynamic folders.

### Applications

`mode1` selects the application in `MMenu::AppStart()` (`menuos/menu.cpp`):

```cpp
switch (file.mode1){//AppNumber
    case 2:
        SampleApp.Setup(level, brCrumbs);
    break;
    ...
```

Unless `mode2` is `BACKGROUND`, the menu starts a watcher in task slot 0 and comes back as
soon as the application sets `Task.ActiveApp = 0`. See [apps.md](apps.md).

### Configs

```c
//storage address (bytes, step by 2), minimal value, maximum value
static const int16_t configsLimit[] MENUOS_ROM = {
    0,     0,    0,   // config 0: Yes/No
    2, -8099, 8096,   // config 1
    4,     1,   48,   // config 2
};
```

- Values are `int16_t`, stored as 16-bit words at the given **byte** address of the
  config storage (EEPROM on AVR, info flash on MSP430). Keep addresses even and below the
  board's `CONFMAXADDRESS`.
- `max == 0` makes a **Yes/No** switch; `UP`/`DOWN` toggle it.
- Otherwise the value is edited digit by digit: `LEFT`/`RIGHT` pick the digit, `UP`/`DOWN`
  add or subtract 10^digit, clamped to `min`…`max`.
- `RETURN` writes the value. A cell that was never written reads as `0xFFFF` (`-1`).

In a folder a config is shown as `name` on the left and its value right-aligned.

## Limits

| | |
|---|---|
| files | 255 (ids are `uint8_t`) |
| children of one folder | 255 |
| nesting depth | `MAXDEPTH` from the board header |
| name length | up to `DISPSTRLENGTH` characters |
| visible rows | `DISPSTRNUMB` from the board header |

## Redrawing the tree picture

`docs/img/menu-tree.svg` is generated from `hwfile.cpp`:

```sh
python3 tools/docs/render_tree.py docs/img/menu-tree.svg
```
