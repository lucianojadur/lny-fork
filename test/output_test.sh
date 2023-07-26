#!/bin/bash
gcc  ../src/output.c ../src/character.c output_test.c testlib.c printfmt.c -o test_output -std=c99 -pedantic -Wall
./output_calc