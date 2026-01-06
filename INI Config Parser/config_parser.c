#include "config_parser.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE* open_file(char* argv);
static void find_section(char* line, char* section);
static void find_key(char* line, char* key);
static void find_val(char* line, char* val);
static void add_config_entry(struct config_t* cfg, char section[MAX_LINE],
                             char key[MAX_LINE], char value[MAX_LINE]);
static struct config_entry_t*
find_entry(struct config_t* cfg, char section[MAX_LINE], char key[MAX_LINE]);
static void config_export(struct config_t* cfg, char* filename);
static char* append_section(struct config_entry_t* entry, char* line_ptr);
static char* append_key_and_value(struct config_entry_t* entry, char* line_ptr);

void config_init(struct config_t* cfg)
{
	cfg->entries = NULL;
	cfg->count = 0;
}
void config_load(struct config_t* cfg, char* filename)
{
	FILE* fp = open_file(filename);
	char line[MAX_LINE];
	bool entered_section = false;
	char section[MAX_LINE];

	while (fgets(line, MAX_LINE, fp) != NULL) {
		if (strchr(line, '\n') == NULL)
			terminate("Error trying to parse line - line is too "
			          "long ( limit 120 characters )");

		if (*line == '[' && strchr(line, ']')) {
			entered_section = true;
			find_section(line, section);
		}
		if (entered_section) {
			if (strchr(line, '=') == NULL)
				continue;

			char key[MAX_LINE] = {0};
			find_key(line, key);

			char value[MAX_LINE] = {0};
			find_val(line, value);

			add_config_entry(cfg, section, key, value);
		}
	}

	fclose(fp);
}
void config_free(struct config_t* cfg)
{
	if (cfg == NULL || cfg->entries == NULL)
		return;
	while (cfg->count != 0) {
		cfg->count--;
		struct config_entry_t* entry = cfg->entries[cfg->count];

		free(entry->section);
		free(entry->key);
		free(entry->value);
		free(entry);
	}
	free(cfg->entries);
	cfg->entries = NULL;
}

void config_get(struct config_t* cfg, char section[MAX_LINE],
                char key[MAX_LINE])
{
	struct config_entry_t* entry = find_entry(cfg, section, key);
	if (entry == NULL)
		terminate("Error could not find Section / Key");
	printf("%s\n", entry->value);
}

void config_set(struct config_t* cfg, char section[MAX_LINE],
                char key[MAX_LINE], char value[MAX_LINE], char* filename)
{
	struct config_entry_t* entry = find_entry(cfg, section, key);
	if (entry == NULL)
		terminate("Error could not find Section / Key");
	free(entry->value);
	entry->value = strdup(value);
	config_export(cfg, filename);
	printf("Done setting %s\n", filename);
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
	else if (argc == 5 && strcmp(argv[2], "--set") == 0) {
		return 2;
		printf("--set\n");
	}
	else
		return 0;
}

void parse_args_get(char* argv[], char section[MAX_LINE], char key[MAX_LINE])
{
	uint32_t i = 0;
	uint32_t j = 0;

	for (; argv[3][i] != '.' && argv[3][i] != '\0'; i++)
		section[i] = argv[3][i];
	section[i] = '\0';

	if (argv[3][i] == '.')
		i++;

	for (; argv[3][i] != '\0'; i++, j++)
		key[j] = argv[3][i];
	key[j] = '\0';
}

void parse_args_set(char* argv[], char section[MAX_LINE], char key[MAX_LINE],
                    char value[MAX_LINE])
{
	uint32_t i = 0;
	uint32_t j = 0;
	uint32_t k = 0;
	uint32_t l = 0;

	for (; argv[3][i] != '.' && argv[3][i] != '\0'; i++)
		section[i] = argv[3][i];
	section[i] = '\0';

	if (argv[3][i] == '.')
		i++;

	for (; argv[3][i] != '\0'; i++, j++)
		key[j] = argv[3][i];
	key[j] = '\0';

	for (; argv[4][l] != '\0'; l++, k++)
		value[k] = argv[4][l];
	value[k] = '\0';
}

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
	while (*line != ']' && *line != '\0' && i < MAX_LINE - 1) {
		if (*line == ' ' || *line == '\n' || *line == '\t')
			terminate("Error syntax error while trying to parse a "
			          "section");
		section[i++] = *line++;
	}
	section[++i] = '\0';
}

static void find_key(char* line, char* key)
{
	uint32_t i = 0;
	while (*line != '=' && *line != '\0' && i < MAX_LINE - 1) {
		if (*line != ' ')
			key[i++] = *line++;
	}
	key[++i] = '\0';
}

static void find_val(char* line, char* val)
{
	uint32_t i = 0;
	while (*line != '=')
		line++;
	line++;
	while (*line != '\n' && *line != '\0' && i < MAX_LINE - 1) {
		if (*line == ' ') {
			line++;
			continue;
		}
		val[i++] = *line++;
	}
	val[++i] = '\0';
}

static void add_config_entry(struct config_t* cfg, char section[MAX_LINE],
                             char key[MAX_LINE], char value[MAX_LINE])
{
	struct config_entry_t* entry;
	if ((entry = malloc(sizeof(struct config_entry_t))) == NULL)
		terminate("Error trying to allocate memory - "
		          "not enough memory");

	entry->section = strdup(section);
	entry->key = strdup(key);
	entry->value = strdup(value);

	uint32_t new_count = cfg->count + 1;

	struct config_entry_t** new_entries =
	    realloc(cfg->entries, new_count * sizeof *cfg->entries);
	if (new_entries == NULL)
		terminate("Error trying to allocate memory - "
		          "not enough memory");

	cfg->entries = new_entries;
	cfg->entries[cfg->count] = entry;
	cfg->count = new_count;
}

static struct config_entry_t*
find_entry(struct config_t* cfg, char section[MAX_LINE], char key[MAX_LINE])
{
	for (uint32_t i = 0; i < cfg->count; i++) {
		if (strcmp(cfg->entries[i]->section, section) == 0 &&
		    strcmp(cfg->entries[i]->key, key) == 0)
			return cfg->entries[i];
	}
	return NULL;
}

static void config_export(struct config_t* cfg, char* filename)
{
	FILE* out = fopen(filename, "wb");
	if (out == NULL)
		terminate("Error trying to re-write file");
	char prev_section[MAX_LINE] = {0};
	char line[MAX_LINE];

	for (uint32_t i = 0; i < cfg->count; i++) {
		struct config_entry_t* entry = cfg->entries[i];

		char* line_ptr = line;

		if (prev_section[0] == 0) {
			line_ptr = append_section(entry, line_ptr);
			line_ptr = append_key_and_value(entry, line_ptr);
			strcpy(prev_section, entry->section);
		}
		else if (strcmp(prev_section, entry->section) == 0)
			line_ptr = append_key_and_value(entry, line_ptr);
		else {
			*line_ptr++ = '\n';
			line_ptr = append_section(entry, line_ptr);
			line_ptr = append_key_and_value(entry, line_ptr);
			strcpy(prev_section, entry->section);
		}

		uint32_t len = line_ptr - line;
		fwrite(line, 1, len, out);
	}

	fclose(out);
}

static char* append_section(struct config_entry_t* entry, char* line_ptr)
{
	char* tmp_ptr = entry->section;
	*line_ptr++ = '[';
	for (; *tmp_ptr != '\0'; tmp_ptr++, line_ptr++)
		*line_ptr = *tmp_ptr;
	*line_ptr++ = ']';
	*line_ptr++ = '\n';
	return line_ptr;
}

static char* append_key_and_value(struct config_entry_t* entry, char* line_ptr)
{
	char* tmp_ptr = entry->key;
	for (; *tmp_ptr != '\0'; tmp_ptr++, line_ptr++)
		*line_ptr = *tmp_ptr;
	*line_ptr++ = ' ';
	*line_ptr++ = '=';
	*line_ptr++ = ' ';
	tmp_ptr = entry->value;
	for (; *tmp_ptr != '\0'; tmp_ptr++, line_ptr++)
		*line_ptr = *tmp_ptr;
	*line_ptr++ = '\n';
	return line_ptr;
}
