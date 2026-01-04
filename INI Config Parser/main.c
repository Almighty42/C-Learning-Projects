#include "config_parser.h"
#include <stdlib.h>

int main(int argc, char* argv[])
{
	struct config_t* cfg;
	if ((cfg = malloc(sizeof(struct config_t))) == NULL)
		terminate(
		    "Error trying to allocate memory - not enough memory");
	switch (handle_user_input(argc, argv)) {
		case 1:
			config_load(cfg, argv[1]);
			break;
		case 2:
			break;
		case 0:
			terminate("Usage: config_parser config_file.ini --get "
			          "/ --set "
			          "Section.Value");
	}
	free(cfg);
}
