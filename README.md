# C-Projects
C/C++ Projects I am making for fun and productivity

## sw/sw.c
I wanted a simple stop watch to better allocate my time throughout the day. I also used this as an opportunity to teach myself a little C.

### Build and run
You can build **sw** inplace by doing `cc sw.c -std=c89 -Os -DNDEBUG -g0 -Wall -o sw.exe`

And then run with `./sw.exe`

You can also install **sw** by running `./build.sh` which builds **sw** and copies the executable to $HOME/bin so you can easily call `sw.exe` from anywhere.

Enjoy!

## mpi/hello_world.c
As a part of my efforts to debug complicated high performance computing (HPC) software, I built an example to test whether the local message passing interface (MPI) install is working properly. This script assumes you have installed (or tried to install) [OpenMPI](https://open-mpi.org/). The script will by default use all accessible processors to print the node's size (total number of procs) and each processor's rank (identifying number).

### Build and run
You can build **hello_world.exe** by doing `mpicc hello_world.c -o hello_world.exe`

And then run with `mpiexec ./hello_world.exe`

You can also build and run the program by doing `./run.sh` which does the the compiling, building, and executing in one go.

Your ouptput is highly dependent on the machine you are using, but should look something similar to this:
```
Compiled.
Running...
size = 4 rank = 0
size = 4 rank = 1
size = 4 rank = 2
size = 4 rank = 3
```

## diy-lisp

https://www.buildyourownlisp.com/

## encrypt

Recently I've been watching Youtube videos on OpSec and encryption and I realized that I do not properly know how encryption works. This project means to teach me. I (will) have several iterations of encryption/decryption methods.

### ascii-shift.c

ASCII-shift is a very simple method of encryption in which all ASCII values of a text characters in a to-be encrypted file are shifted by some user-defined integer. The result, even with a small shift, is relatively unintelligable. I consider this a basic level of encryption akin to a [Caesar Cipher](https://en.wikipedia.org/wiki/Caesar_cipher). Decryption should be relatively simple and can be brute forced by either searching a range of integer shifts or testing for character-doublets (e.g. 'ee', 'oo', 'll') or the single characters 'i/I' and 'a/A'. This program allows for user control of the integer shift and handles simple type and missing file errors.

You can build **ascii-shift.exe** by doing `gcc ascii-shift.c -o ascii-shift.exe`

And then run by providing the file-to-encrypt, encrypted file name, and integer shift: `./ascii-shift.exe sample.txt sample-ascii-shift.txt 42`