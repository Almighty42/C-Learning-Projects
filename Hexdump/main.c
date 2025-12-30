#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static void terminate(char* m);
static size_t get_file_size(FILE* file);

int main(int argc, char* argv[])
{
	if (argc != 2)
		terminate(
		    "Incorrect use of hexdump, please provide a argument.");

	FILE* file;
	file = fopen(argv[1], "rb");

	size_t bytes_read;
	size_t bytes_current = 0;
	size_t file_size = get_file_size(file);

	if (!file_size)
		return -1;
	char* buffer = malloc(file_size);
	if (buffer == NULL)
		terminate("Error - program ran out of memory");
	char* ptr = buffer;
	char* prev_ptr = buffer;

	if (!file)
		terminate("Error trying to open the file - check for "
		          "misspellings or if file exists.");
	bytes_read = fread(buffer, 1, file_size, file);

	printf("%08zx  ", bytes_current);
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
		printf("\n%08zx  ", bytes_current);
	}
	printf("\n");
	fclose(file);
	free(buffer);
	return 0;
}

static void terminate(char* m)
{
	printf("%s\n", m);
	exit(EXIT_FAILURE);
}

static size_t get_file_size(FILE* file)
{
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	if (size < 0)
		return 0;
	fseek(file, 0, SEEK_SET);
	return size;
}
