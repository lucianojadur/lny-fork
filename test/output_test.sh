#!/bin/bash
gcc  ../src/output.c output_test.c testlib.c printfmt.c -o test -std=c99 -pedantic -Wall
