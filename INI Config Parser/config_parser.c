#include "config_parser.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void terminate(char* s)
{
	fprintf(stderr, "%s\n", s);
	exit(EXIT_FAILURE);
}

static FILE* open_file(char** argv)
{
	FILE* fp = fopen(*(argv + 1), "rb");
	if (fp == NULL)
		terminate("Error trying to open file");
	return fp;
}

uint8_t handle_user_input(int argc, char* argv[])
{
	if (argc == 4 && strcmp(argv[2], "--get") == 0) {
		return 1;
	}
	else if (argc == 4 && strcmp(argv[2], "--set") == 0) {
		return 2;
		printf("--set\n");
	}
	else
		return 0;
}
uint8_t config_load(config_t* cfg, char** filename)
{
	FILE* fp = open_file(filename);
	// TODO:
	// 1. Reads each line using fgets
	// 2. Parses each line
	// 3. Allocates memory for config_entry_t
	// 4. Adds entries to config
	// 5. Returns 0 on success, -1 on error
	return 0;
}
// uint8_t* config_get(config_t* cfg, const uint8_t** section, const uint8_t*
// key)
// {
// 	return NULL;
// }
