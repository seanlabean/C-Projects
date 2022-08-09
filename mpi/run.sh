#!/bin/bash

# Must use mpicc to compile.
mpicc hello_world.c -o hello_world.exe

echo "Compiled."
echo "Running..."

mpiexec hello_world.exe
