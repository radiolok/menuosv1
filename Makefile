# MenuOS v1 — one Makefile for every target.
#
#   make                                   # AVR, ATmega1280 (default)
#   make ARCH=avr MCU=atmega2560
#   make ARCH=msp430 MCU=msp430g2553 MSP430_GCC_ROOT=/opt/msp430-gcc
#   make ARCH=host test                    # run the menu tests on the PC
#   make ARCH=host sim                     # interactive simulator in a terminal
#
# Output goes to build/<arch>-<mcu>/.

ARCH ?= avr

ifeq ($(ARCH),avr)
  MCU      ?= atmega1280
  F_CPU    ?= 16000000UL
  CROSS    ?= avr-
endif
ifeq ($(ARCH),msp430)
  MCU      ?= msp430g2553
  CROSS    ?= msp430-elf-
endif
ifeq ($(ARCH),host)
  MCU      := native
  CROSS    ?=
endif

ifeq ($(filter $(ARCH),avr msp430 host),)
  $(error ARCH must be avr, msp430 or host)
endif

CC      := $(CROSS)gcc
CXX     := $(CROSS)g++
OBJCOPY := $(CROSS)objcopy
SIZE    := $(CROSS)size

BUILD   := build/$(ARCH)-$(MCU)
TARGET  := $(BUILD)/menuos

# ------------------------------------------------------------------ sources
CORE_SRC := \
	MenuOSv1.cpp \
	menuos/MTask.cpp \
	menuos/buttons.cpp \
	menuos/menu.cpp \
	menuos/config.cpp \
	menuos/hw/hwi.cpp \
	menuos/hw/driver/files/hwfile.cpp \
	menuos/util/console.cpp \
	menuos/apps/sampleapp.cpp

AVR_SRC := \
	menuos/port/avr/port_avr.cpp \
	menuos/hw/boards/rln_electro_2012.cpp \
	menuos/hw/driver/disp/hwdisp.cpp \
	menuos/hw/driver/disp/glcd/glcd.cpp \
	menuos/hw/driver/disp/glcd/glcd_Device.cpp \
	menuos/hw/driver/disp/glcd/gText.cpp \
	menuos/hw/driver/disp/glcd/include/pins_arduino.c \
	menuos/hw/driver/disp/tlcd/ElectroLCD.cpp \
	menuos/hw/driver/i2c/twi.cpp \
	menuos/hw/driver/i2c/Wire.cpp \
	menuos/hw/driver/DS1307.cpp \
	menuos/util/Print.cpp \
	menuos/util/uart.cpp \
	menuos/apps/clock.cpp

MSP430_SRC := \
	menuos/port/msp430/port_msp430.cpp \
	menuos/hw/boards/msp430_launchpad.cpp \
	menuos/hw/driver/disp/term/termdisp.cpp

HOST_SRC := \
	menuos/port/host/port_host.cpp \
	menuos/hw/boards/host_sim.cpp \
	menuos/hw/driver/disp/term/termdisp.cpp

SRC := $(CORE_SRC)
ifeq ($(ARCH),avr)
  SRC += $(AVR_SRC)
endif
ifeq ($(ARCH),msp430)
  SRC += $(MSP430_SRC)
endif
ifeq ($(ARCH),host)
  SRC += $(HOST_SRC)
endif

# ------------------------------------------------------------------ flags
INCLUDES := -I. -Imenuos

COMMON_FLAGS := -Os -g -Wall -ffunction-sections -fdata-sections $(INCLUDES) $(EXTRA_FLAGS)
CXX_ONLY     := -std=gnu++11 -fno-exceptions -fno-rtti -fno-threadsafe-statics
C_ONLY       := -std=gnu99
# The link step gets CXXFLAGS too, so -mmcu is already there: passing it twice
# makes msp430-elf-ld load the device linker script twice and fail.
LDFLAGS      := -Wl,--gc-sections

ifeq ($(ARCH),avr)
  COMMON_FLAGS += -mmcu=$(MCU) -DF_CPU=$(F_CPU)
endif

ifeq ($(ARCH),msp430)
  COMMON_FLAGS += -mmcu=$(MCU)
  ifneq ($(MSP430_GCC_ROOT),)
    COMMON_FLAGS += -I$(MSP430_GCC_ROOT)/include
    LDFLAGS      += -L$(MSP430_GCC_ROOT)/include
  endif
endif

ifeq ($(ARCH),host)
  COMMON_FLAGS := -O0 -g -Wall -Wextra -DMENUOS_HOST $(INCLUDES) $(EXTRA_FLAGS)
  LDFLAGS      :=
endif

CFLAGS   := $(COMMON_FLAGS) $(C_ONLY)
CXXFLAGS := $(COMMON_FLAGS) $(CXX_ONLY)

OBJ := $(patsubst %,$(BUILD)/%.o,$(basename $(SRC)))
DEP := $(OBJ:.o=.d)

# ------------------------------------------------------------------ rules
.PHONY: all clean size test sim help

ifeq ($(ARCH),host)
all: test
else
all: $(TARGET).elf $(TARGET).hex size
endif

$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(TARGET).elf: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) -Wl,-Map,$(TARGET).map -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

size: $(TARGET).elf
	@echo "== $(ARCH) / $(MCU)"
	@$(SIZE) $<

# ---- host only
$(BUILD)/test_menu: $(OBJ) $(BUILD)/test/test_menu.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/menuos-sim: $(OBJ) $(BUILD)/tools/sim/sim_main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(BUILD)/test_menu
	$(BUILD)/test_menu

sim: $(BUILD)/menuos-sim
	$(BUILD)/menuos-sim

clean:
	rm -rf build

help:
	@sed -n '1,12p' Makefile

-include $(DEP)
