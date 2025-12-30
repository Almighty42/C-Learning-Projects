#include <ctype.h>
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
	char* ptr = buffer;
	char* prev_ptr = buffer;

	file = fopen(argv[1], "r");

	if (!file)
		terminate("Error trying to open the file - check for "
		          "misspellings or if file exists.");

	bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file);

	printf("%08x  ", bytes_current);
	while (bytes_current < bytes_read) {
		for (; ptr < prev_ptr + 8 && bytes_current < bytes_read;
		     ptr++) {
			printf("%02x ", *ptr & 0xFF);
			bytes_current++;
		}
		printf(" ");
		for (; ptr < prev_ptr + 16 && bytes_current < bytes_read;
		     ptr++) {
			printf("%02x ", *ptr & 0xFF);
			bytes_current++;
		}
		if (bytes_read == bytes_current)
			for (char i = ptr - prev_ptr; i < 16; i++)
				printf("   ");
		printf(" |");
		for (; prev_ptr < ptr; prev_ptr++) {
			if (isprint(*prev_ptr))
				putchar(*prev_ptr);
			else
				putchar('.');
		}
		printf("|");
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
