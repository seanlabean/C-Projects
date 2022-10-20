#!/bin/bash

# Lint
clang-format -i sw.c

# Clean
rm -f ./sw.exe

# Build(debug)
# cc -std=c89 -DDEBUG -Wall -Wpedantic -Wshadow -Wextra -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion -Wvla -g -Og -fsanitize=address -fsanitize=undefined -o sw sw.c

# Build(fast)
cc sw.c -std=c89 -Os -DNDEBUG -g0 -Wall -o sw.exe

# Size
echo "Size: $(du -sk ./sw.exe)"

# Plan9
# pcc sw.c -o sw

# Install
if [ -d "$HOME/bin" ] && [ -e ./sw.exe ]
then
	cp ./sw.exe $HOME/bin
    echo "Installed: $HOME/bin" 
fi

# Run
./sw.exe
