#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void terminate(char* m);

int main(int argc, char* argv[])
{
	if (argc != 2)
		terminate(
		    "Incorrect use of hexdump, please provide a argument.");

	FILE* file;

	// TODO: Create a dynamic buffer
	char buffer[1000];
	int bytes_read;
	int bytes_current = 0;

	file = fopen(argv[1], "r");

	if (!file)
		terminate("Error trying to open the file - check for "
		          "misspellings or if file exists.");

	while ((bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file)) > 0) {
		printf("%08x  ", bytes_current);
		char ASCII_list[1000];
		for (int i = 0; i < bytes_read; i++) {
			if (buffer[i] >= 32 && buffer[i] <= 127)
				ASCII_list[i] = buffer[i];
			else
				ASCII_list[i] = '.';
			if (i % 16 == 0 && i != 0) {
				printf("  ");
				printf("|");
				for (char j = 0; j < 16; j++) {
					putchar(ASCII_list[j + (i - 16)]);
				}
				printf("|");
				printf("\n");
				printf("%08x  ", bytes_current);
			}
			else if (i % 8 == 0 && i != 0)
				printf(" ");
			printf("%02x ", buffer[i] & 0xFF);
			bytes_current++;
		}
		printf("\n%08x  ", bytes_current);
	}
	printf("\n");
	fclose(file);
	return 0;
}

void terminate(char* m)
{
	printf("%s\n", m);
	exit(EXIT_FAILURE);
}
