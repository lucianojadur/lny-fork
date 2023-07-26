#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "output.h"


FILE* output(const char *character, const char *weapon, const char *set){
	char name[200];
	getcwd(name, 100);
	strcat(name, "/");
	strcat(name, character);
	strcat(name, "_");
	strcat(name, weapon);
	strcat(name, "_");
	strcat(name, set);
	strcat(name, ".csv");
	FILE *o = fopen(name, "w+");
	if (o == NULL)
		perror("Perror");

	return o;
}

void output_write_header(FILE *o){
	fprintf(o, "Dmg value, samples\n");
}

void output_write_line(FILE *o, unsigned int value, unsigned int n){
	fprintf(o, "%d,%d\n", value, n);
}

void output_write_character_stats(FILE *o, character_t *ch){
	fprintf(o, "\nLyney stats: \n");
	character_print_stats(ch, o);
}
