# C-Projects
C/C++ Projects I am making for fun and productivity

## sw.c
I wanted a simple stop watch to better allocate my time throughout the day. I also used this as an opportunity to teach myself a little C.

### Build and run
You can build **sw** inplace by doing `cc sw.c -std=c89 -Os -DNDEBUG -g0 -Wall -o sw.exe`

And then run with `./sw.exe`

You can also install **sw** by running `./build.sh` which builds **sw** and copies the executable to $HOME/bin so you can easily call `sw.exe` from anywhere.

Enjoy!