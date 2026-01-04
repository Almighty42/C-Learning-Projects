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

void terminate(char* s);
uint8_t handle_user_input(int argc, char* argv[]);
uint8_t config_load(struct config_t* cfg, char* filename);
// uint8_t* config_get(config_t* cfg, const uint8_t** section, const uint8_t*
// key);

#endif
