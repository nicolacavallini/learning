#!/bin/bash
gcc -g -O0 main.c  -o test.x  -lm && valgrind --leak-check=yes ./test.x
