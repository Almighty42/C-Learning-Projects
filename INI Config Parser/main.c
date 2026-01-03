#include "config_parser.h"

int main(int argc, char* argv[])
{
	switch (handle_user_input(argc, argv)) {
		case 1:
			break;
		case 2:
			break;
		case 0:
			terminate("Usage: config_parser config_file.ini --get "
			          "/ --set "
			          "Section.Value");
	}
}
