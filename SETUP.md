
# SkillsLite setup

## For building the Demo

Building SkillsLite requires [Event Assembler](https://feuniverse.us/t/event-assembler/1749?u=stanh) with [ColorzCore](https://feuniverse.us/t/colorzcore/3970?u=stanh):

- Create a folder under `Tools` named `EventAssembler` (no space!)
- Unpack the contents of the Event Assembler archive into this new `EventAssembler` folder
- Put `ColorzCore.exe` inside that folder aswell.

Next, you will also need a fresh clean FE8(U) ROM. Take one of these and put it in the root folder under the name `FE8U.gba`.

Once that is done, simply run the `MakeHack.bat` (or `MakeHack.sh`) scripts to build the demonstration ROM (`SKILLS.gba`). You should also be able to make edits to tables, maps, text and events and have your changes applied once you build again.

TODO: make all in one tool package for this (with EA + ColorzCore + python tools)

## For building Wizardry

Building Wizardry requires a few more things:

- You will need to have [devkitARM](https://devkitpro.org/wiki/Getting_Started) installed and working.
- You will also need two additional local tools: [lyn](https://github.com/StanHash/lyn) under `Tools/EventAssembler/Tools` and [ea-dep](https://github.com/StanHash/ea-dep) simply under `Tools` (check the release tabs for binaries).
- If you haven't cloned the repository recursively, you will also need to initialize the CLib submodule. Type `git submodule update --init --recursive` or use your favourite git GUI to achieve this.

From that, you should be able to make use of the Makefile in the `Wizardry` folder. Open your terminal (Under Windows, you'll need the MSYS provided by devkitPro) and move to the `Wizardry` folder and type `make`. This should make all of the dmp and lyn.event files included directly or indirectly by `Wizardry.event`.
