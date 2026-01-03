#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <stdint.h>
#include <stdio.h>

typedef struct
{
	uint8_t* section;
	uint8_t* key;
	uint8_t* value;
} config_entry_t;

typedef struct
{
	config_entry_t** entries;
	uint32_t count;
} config_t;

void terminate(char* s);
uint8_t handle_user_input(int argc, char* argv[]);
// uint32_t config_load(config_t* cfg, const uint8_t* filename);
// uint8_t* config_get(config_t* cfg, const uint8_t** section, const uint8_t*
// key);

#endif
