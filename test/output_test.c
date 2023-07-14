#include <errno.h>
#include <string.h>
#include "../src/output.h"
#include "testlib.h"
#include <stdio.h>
#include <stdlib.h>


#define MAX_LINE_LENGTH 10


typedef struct {
	char *value;
	char *n;
}line_t;

static void test01_file_creates_correctly();

static void test02_file_writes_a_line_correctly();

static void test03_write_multiple_lines();


int main(void){

	run_test(test01_file_creates_correctly);
	run_test(test02_file_writes_a_line_correctly);
	run_test(test03_write_multiple_lines);

	return 0;
}

static void test01_file_creates_correctly(){
	
	FILE * out = output("lyney", "signature", "lw");
	fclose(out);
	out = NULL;
	out = fopen("lyney_signature_lw.csv", "w");
	
	ASSERT_TRUE("TEST 01: Output file gets created correctly", out != NULL);
	fclose(out);
	out = NULL;
}

static void test02_file_writes_a_line_correctly(){
	FILE *out = output("lyney", "sig", "lw");
	if (out == NULL)
		fprintf(stderr, "error opening out: %s\n", strerror(errno));

	unsigned int value = 1000, n = 3;
	output_write_line(out, value, n);
	fclose(out);

	out = fopen("lyney_sig_lw.csv", "r");
	char expected_line[15];
	fgets(expected_line, MAX_LINE_LENGTH, out);

	ASSERT_TRUE("TEST 02: file written correctly", !strcmp(expected_line, "1000,3\n"));

	fclose(out);
}

static void test03_write_multiple_lines(){
	FILE *out = output("lyney", "sig", "sr");
	const int n_lines = 3;
	bool expected_value = false;

	for (int i = 0; i < n_lines; i++){
		output_write_line(out, (unsigned int)i*10, (unsigned int) i);	
	}
	fclose(out);
	//
	// this is fucking awful
	out = fopen("lyney_sig_sr.csv", "r");
	line_t lines[n_lines];
	char line[MAX_LINE_LENGTH];
	for (int i = 0; i < n_lines; i++){
		fgets(line, MAX_LINE_LENGTH, out);
		lines[i].value = strtok(line, ",");
		lines[i].n = strtok(NULL, "n");
		expected_value = (atoi(lines[i].value) == i*10) && (atoi(lines[i].n) == i);
	}

	ASSERT_TRUE("TEST 03: multiple lines written correctly in file", expected_value);
}
