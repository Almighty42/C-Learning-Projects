#include "config_parser.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE* open_file(char* argv)
{
	FILE* fp = fopen(argv, "rb");
	if (fp == NULL)
		terminate("Error trying to open file");
	return fp;
}

static char* find_section(char* line)
{
	char* section;
	if (*line == '[')
		line++;
	while (*line != ']') {
		if (*line == ' ' || *line == '\n' || *line == '\t')
			terminate("Error syntax error while trying to parse a "
			          "section");
		*section = *line;
		section++;
		line++;
	}
	return section;
}

static char* find_key(char* line)
{
	char* key;
	while (*line != '=') {
		if (*line != ' ')
			*key = *line;
		key++;
		line++;
	}
	*(++key) = '\0';
	return key;
}

static char* find_val(char* line)
{
	char* val;
	while (*line != '=')
		line++;
	line++;
	while (*line != '\n') {
		if (*line != ' ')
			*val = *line;
		val++;
		line++;
	}
	*(++val) = '\0';
	return val;
}

void terminate(char* s)
{
	fprintf(stderr, "%s\n", s);
	exit(EXIT_FAILURE);
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
uint8_t config_load(struct config_t* cfg, char* filename)
{
	FILE* fp = open_file(filename);
	char line[MAX_LINE];
	bool entered_section = false;

	while (fgets(line, MAX_LINE, fp) != NULL) {
		char* section;
		if (*line == '[' && strchr(line, ']')) {
			entered_section = true;
			// section = allocate(section,
			// sizeof(find_section(line)));
			section = find_section(line);
		}
		if (entered_section) {
			if (strchr(line, '=') == NULL)
				continue;
			char* key;
			// key = allocate(key, sizeof(find_key(line)));
			key = find_key(line);

			char* value;
			// value = allocate(value, sizeof(find_value(line)));
			value = find_val(line);

			struct config_entry_t* entry;
			if ((entry = malloc(sizeof(struct config_entry_t))) ==
			    NULL)
				terminate("Error trying to allocate memory - "
				          "not enough memory");
			entry->section = section;
			entry->key = key;
			entry->value = value;
			cfg->count++;
			printf("Section - %s, key - %s, value - %s\n", section,
			       key, value);
			free(entry);
		}
	}
	// if (fgets(line, MAX_LINE, fp) == NULL)
	// 	terminate("Error in ini file, no line in the file can
	// exceed " 	          "120 characters");

	return 0;
}
// uint8_t* config_get(config_t* cfg, const uint8_t** section, const uint8_t*
// key)
// {
// 	return NULL;
// }
