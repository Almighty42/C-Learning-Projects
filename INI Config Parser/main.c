#include "config_parser.h"

int main(int argc, char* argv[])
{
	struct config_t cfg;
	config_init(&cfg);

	char section[MAX_LINE];
	char key[MAX_LINE];

	switch (handle_user_input(argc, argv)) {
		case 1:
			parse_args(argv, section, key);
			config_load(&cfg, argv[1]);
			config_get(&cfg, section, key);
			break;
		case 2:
			parse_args(argv, section, key);
			config_load(&cfg, argv[1]);
			break;
		case 0:
			terminate("Usage: config_parser config_file.ini --get "
			          "/ --set "
			          "Section.Value");
	}
	config_free(&cfg);
}
