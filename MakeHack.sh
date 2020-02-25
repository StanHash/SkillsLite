#!/bin/bash

# USAGE: ./MakeHack.sh [quick]
# If first argument is "quick", then this will not update text or tables

base_dir=$(dirname "$(readlink -f "$0")")

# defining buildfile config

main_event="$base_dir/Main.event"

source_rom="$base_dir/FE8U.gba"
target_rom="$base_dir/SKILLS-DEMO.gba"

# defining tools

ea_exe="$base_dir/Tools/EventAssembler/ColorzCore.exe"

# do the actual building

cd "$base_dir"

echo "Copying ROM"

cp -f "$source_rom" "$target_rom" || exit 2

echo "Assembling"

mono "$ea_exe" A FE8 "-output:$target_rom" "-input:$main_event" --nocash-sym

# TODO: generate patch (would require a linux version of ups)

echo "Done!"
