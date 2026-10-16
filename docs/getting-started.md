# Getting started

Everything is built by the top-level `Makefile`. Pick the architecture with `ARCH` and the
chip with `MCU`; output goes to `build/<arch>-<mcu>/`.

| Command | What it does |
|---|---|
| `make ARCH=host test` | builds the host port and runs the menu tests |
| `make ARCH=host sim` | interactive simulator in your terminal |
| `make ARCH=avr MCU=atmega1280` | firmware for the original board (default target) |
| `make ARCH=msp430 MCU=msp430g2553` | firmware for the MSP430 LaunchPad |
| `make clean` | removes `build/` |

Extra compiler flags can be passed with `EXTRA_FLAGS=...`, for example
`EXTRA_FLAGS=-DMENUOS_NO_LOG` to drop the UART log.

## Host: simulator and tests

Only a C++ compiler and `make` are needed.

```sh
make ARCH=host sim
```

```
MenuOS v1 simulator (host)

+----------------+
>Folder 1        <
|Folder 2        |
|                |
|                |
|                |
+----------------+
```

| Key | Button |
|---|---|
| ↓, `s`, `+` | `UP` — next item, increase value |
| ↑, `w`, `-` | `DOWN` — previous item, decrease value |
| →, `d`, Enter | `RIGHT` — open, next lower digit |
| ←, `a` | `LEFT` — next higher digit |
| `q`, Backspace | `RETURN` — back, save config |
| Esc Esc, Ctrl-C | quit |

`>` … `<` marks the selected row, `*` … `*` and `^` mark the digit being edited.

`make ARCH=host test` drives the same code with simulated button presses and checks the
screen after each step (`test/test_menu.cpp`). `test_menu --dump DIR` saves every checked
screen; `tools/docs/render_glcd.py` turns them into the GLCD pictures used in the README.

## AVR

Toolchain: `avr-gcc` + `avr-libc`.

```sh
sudo apt install gcc-avr avr-libc binutils-avr     # Debian / Ubuntu
make ARCH=avr MCU=atmega1280                        # or atmega2560
```

The default board is `rln_electro_2012`: KS0108 GLCD, five buttons on `PORTB[4:0]` to
ground, DS1307 RTC on TWI, 16 MHz crystal, log on USART0 at 38400 baud. `F_CPU` can be
changed with `make F_CPU=8000000UL`.

Flash with any programmer `avrdude` knows, for example:

```sh
avrdude -c usbasp -p m1280 -U flash:w:build/avr-atmega1280/menuos.hex:i
```

The original Atmel Studio 6 project (`MenuOSv1.atsln` / `MenuOSv1.cppproj`) is kept and
lists the new source files too.

## MSP430

Toolchain: TI **MSP430-GCC** (`msp430-elf-gcc`) with its support files (device headers and
linker scripts). Download it from TI, or reuse the commands from
[`.github/workflows/ci.yml`](../.github/workflows/ci.yml), which installs version 9.3.1.2.

```sh
make ARCH=msp430 MCU=msp430g2553 MSP430_GCC_ROOT=$HOME/ti/msp430-gcc
```

`MSP430_GCC_ROOT` is the install directory; its `include/` is added to the include and
linker paths. Supported chips: MSP430G2553 (and the other G2xx3 parts), MSP430F2616–F2619.

Board `msp430_launchpad`:

- buttons on `P2.0`…`P2.4` to ground (internal pull-ups):
  `RETURN`, `LEFT`, `DOWN`, `UP`, `RIGHT`;
- the menu is printed on `USCI_A0` at 9600 8N1 — on the G2 LaunchPad this is the
  back-channel UART (set the RXD/TXD jumpers to hardware UART);
- config values are kept in information flash segment D (64 bytes).

Open the LaunchPad's serial port in a terminal that understands ANSI escape codes
(`picocom -b 9600 /dev/ttyACM0`, PuTTY, …). Flash with `mspdebug`:

```sh
mspdebug rf2500 "prog build/msp430-msp430g2553/menuos.elf"
```

## CI

`.github/workflows/ci.yml` runs on every push and pull request:

- host tests and the simulator build;
- AVR firmware for ATmega1280 and ATmega2560 (`gcc-avr` from Ubuntu);
- MSP430 firmware for MSP430G2553 and MSP430F2618 (TI MSP430-GCC, cached).

Each job prints a size report in the run summary and uploads the `.elf`/`.hex` as artifacts.
