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

static void find_section(char* line, char* section)
{
	uint32_t i = 0;
	if (*line == '[')
		line++;
	while (*line != ']') {
		if (*line == ' ' || *line == '\n' || *line == '\t')
			terminate("Error syntax error while trying to parse a "
			          "section");
		section[i] = *line;
		i++;
		line++;
	}
	section[++i] = '\0';
}

static void find_key(char* line, char* key)
{
	uint32_t i = 0;
	while (*line != '=') {
		if (*line != ' ')
			key[i] = *line;
		i++;
		line++;
	}
	key[++i] = '\0';
}

static void find_val(char* line, char* val)
{
	uint32_t i = 0;
	while (*line != '=')
		line++;
	line++;
	while (*line != '\n') {
		if (*line == ' ') {
			line++;
			continue;
		}
		val[i] = *line;
		i++;
		line++;
	}
	val[++i] = '\0';
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

	if (fgets(line, MAX_LINE, fp) == NULL)
		terminate("Error in ini file, no line in the file can exceed "
		          "120 characters");

	while (fgets(line, MAX_LINE, fp) != NULL) {
		char section[MAX_LINE];
		if (*line == '[' && strchr(line, ']')) {
			entered_section = true;
			find_section(line, section);
		}
		if (entered_section) {
			if (strchr(line, '=') == NULL)
				continue;

			char key[MAX_LINE] = {0};
			find_key(line, key);

			char value[MAX_LINE] = {};
			find_val(line, value);

			struct config_entry_t* entry;
			if ((entry = malloc(sizeof(struct config_entry_t))) ==
			    NULL)
				terminate("Error trying to allocate memory - "
				          "not enough memory");

			entry->section = strdup(section);
			entry->key = strdup(key);
			entry->value = strdup(value);

			cfg->count++;

			printf("Section - %s, key - %s, value - %s\n", section,
			       key, value);
			free(entry);
		}
	}

	return 0;
}
// uint8_t* config_get(config_t* cfg, const uint8_t** section, const uint8_t*
// key)
// {
// 	return NULL;
// }
