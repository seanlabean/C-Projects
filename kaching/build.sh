#!/bin/bash

# Clean
rm -f ./kaching.exe

# Build(debug)
# cc -std=c89 -DDEBUG -Wall -Wpedantic -Wshadow -Wextra -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion -Wvla -g -Og -fsanitize=address -fsanitize=undefined -o kaching kaching.c

# Build(fast)
cc kaching.c -std=c89 -Os -DNDEBUG -g0 -Wall -o kaching.exe

# Size
#echo "Size: $(du -sk ./kaching.exe)"

# Install
if [ -d "$HOME/bin" ] && [ -e ./kaching.exe ]
then
	cp ./kaching.exe $HOME/bin
    echo "Installed: $HOME/bin" 
fi

# Run
./kaching.exe