#!/bin/bash

# USAGE: ./MakeHack.sh [quick]
# If first argument is "quick", then this will not update text or tables

base_dir=$(dirname "$(readlink -f "$0")")

# defining buildfile config

main_event="$base_dir/Main.event"

source_rom="$base_dir/FE8U.gba"
target_rom="$base_dir/SKILLS-DEMO.gba"

text_main="$base_dir/Writans/Text/Main.txt"
text_installer="$base_dir/Writans/Text.event"
text_definitions="$base_dir/Writans/TextDefinitions.event"

# defining tools

ea_exe="$base_dir/Tools/EventAssembler/ColorzCore.exe"

textprocess_py="$base_dir/Tools/TextProcess/text-process-classic.py"
parsefile_exe="$base_dir/Tools/EventAssembler/Tools/ParseFile"

# finding correct python version

if hash python3 2> /dev/null; then
  python3_exe="python3"
elif hash python 2> /dev/null && [[ $(python -c 'import sys; print(int(sys.version_info[0] > 2))') -eq 1 ]]; then
  python3_exe="python"
else
  echo "MakeHack.sh requires python 3 to be installed!" 1>&2
  exit 1
fi

# do the actual building

if [[ $1 != quick ]]; then
  # make hack full: process other files before assembling

  echo "Processing text"

  # cd to allow ParseFile to locate ParseDefinitions.txt
  # all paths are abs so wd shouldn't matter otherwise
  cd "$(dirname "$text_main")"

  $python3_exe "$textprocess_py" "$(basename "$text_main")" \
    --installer "$text_installer" \
    --definitions "$text_definitions" \
    --parser-exe "$parsefile_exe"

fi

cd "$base_dir"

echo "Preparing ROM"

cp -f "$source_rom" "$target_rom" || exit 2

echo "Assembling"

mono "$ea_exe" A FE8 "-output:$target_rom" "-input:$main_event" --nocash-sym

# TODO: generate patch (would require a linux version of ups)

echo "Done!"
