
.SUFFIXES:

# This makefile handles making all the asm binaries (mostly dmp and lyn.event files)
# Building those files can be done manually too, this is just convenience for high-end users

# ====================
# = TOOL DEFINITIONS =
# ====================

# This assumes the necessary tools are located under ../Tools
# This also requires devkitARM

ifeq ($(strip $(DEVKITARM)),)
  $(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/base_tools

TOOLS_DIR := $(realpath ../Tools)
EA_DIR := $(TOOLS_DIR)/EventAssembler
CLIB_DIR := $(TOOLS_DIR)/CLib

ifeq ($(OS),Windows_NT)
  EXE := .exe
  EA  := $(EA_DIR)/ColorzCore.exe
else
  EXE :=
  EA  := mono $(EA_DIR)/ColorzCore.exe
endif

EADEP    := $(TOOLS_DIR)/ea-dep$(EXE)
LYN      := $(EA_DIR)/Tools/lyn$(EXE)
PNG2DMP  := $(EA_DIR)/Tools/Png2Dmp$(EXE)
COMPRESS := $(EA_DIR)/Tools/compress$(EXE)

CACHE_DIR := .Cache
$(shell mkdir -p $(CACHE_DIR) > /dev/null)

# ===========================
# = LIST ALL FILES TO BUILD =
# ===========================

# We are going to be using the ea-dep tool to list all files included by the main event file
# In our case, the main event file is going to be Wizardry.event

MAIN_EVENT := Wizardry.event
ALL_FILES  := $(shell $(EADEP) $(MAIN_EVENT) -I "$(EA_DIR)" --add-missings)

# This is simply a target that depends on all the listed files
# Making this target (which is default) will also ensure all the listed files are made

all: $(ALL_FILES)

.PHONY: all

# =================
# = PATTERN RULES =
# =================

LYN_REFERENCE := $(CLIB_DIR)/reference/FE8U-20190316.o

# Setting C/ASM include directories up
INCLUDE_DIRS := $(CLIB_DIR)/CLib/include Include
INCFLAGS     := $(foreach dir, $(INCLUDE_DIRS), -I "$(dir)")

# setting up compilation flags
ARCH := -mcpu=arm7tdmi -mthumb -mthumb-interwork
CC_FLAGS := $(ARCH) $(INCFLAGS) -Wall -Os -mtune=arm7tdmi -ffreestanding -fomit-frame-pointer -mlong-calls
AS_FLAGS := $(ARCH) $(INCFLAGS)

# defining dependency flags
CC_DEPFLAGS = -MMD -MT "$*.o" -MT "$*.asm" -MF "$(CACHE_DIR)/$(notdir $*).d" -MP
AS_DEPFLAGS = --MD "$(CACHE_DIR)/$(notdir $*).d"

# ASM to OBJ rule
%.o: %.s
	$(AS) $(AS_FLAGS) $(AS_DEPFLAGS) -I $(dir $<) $< -o $@ $(ERROR_FILTER)

# C to OBJ rule
%.o: %.c
	$(CC) $(CC_FLAGS) $(CC_DEPFLAGS) -g -c $< -o $@ $(ERROR_FILTER)

# C to ASM rule
# Generating "asm" files instead of s files so that we can gitignore them no prob
%.asm: %.c
	$(CC) $(CC_FLAGS) $(CC_DEPFLAGS) -S $< -o $@ -fverbose-asm $(ERROR_FILTER)

# OBJ to event
%.lyn.event: %.o $(LYN_REFERENCE)
	$(LYN) $< $(LYN_REFERENCE) > $@

# OBJ to DMP rule
%.dmp: %.o
	$(OBJCOPY) -S $< -O binary $@

# PNG to 4bpp rule
%.4bpp: %.png
	$(PNG2DMP) $< -o $@

# PNG to gbapal rule
%.gbapal: %.png
	$(PNG2DMP) $< -po $@ --palette-only

# Anything to lz rule
%.lz: %
	$(COMPRESS) $< --to-stdout > $@
