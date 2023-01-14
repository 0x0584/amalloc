#!/bin/zsh
rm -f main
gcc -Iinclude main.c -o main -L. -lft_malloc_x86_64-Darwin
./main
