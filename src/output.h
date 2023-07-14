#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdbool.h>


FILE* output(const char *character, const char* weapon, const char* set);

void output_write_line(FILE *o, unsigned int value, unsigned int n);

#endif
