#!/bin/bash
gcc  ../src/character.c ../src/calc.c calc_test.c testlib.c printfmt.c -o test_calc -std=c99 -pedantic -Wall
./test_calc