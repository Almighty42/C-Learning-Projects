#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <stdint.h>
#include <stdio.h>

#define MAX_LINE 120

struct config_entry_t
{
	char* section;
	char* key;
	char* value;
};

struct config_t
{
	struct config_entry_t** entries;
	uint32_t count;
};

void config_init(struct config_t* cfg);
void config_load(struct config_t* cfg, char* filename);
void config_free(struct config_t* cfg);
void config_get(struct config_t* cfg, char section[MAX_LINE],
                char key[MAX_LINE]);
void config_set(struct config_t* cfg, char section[MAX_LINE],
                char key[MAX_LINE], char value[MAX_LINE], char* filename);
uint8_t handle_user_input(int argc, char* argv[]);
void parse_args_get(char* argv[], char section[MAX_LINE], char key[MAX_LINE]);
void parse_args_set(char* argv[], char section[MAX_LINE], char key[MAX_LINE],
                    char value[MAX_LINE]);
void terminate(char* s);

#endif
