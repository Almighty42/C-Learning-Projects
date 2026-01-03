#include "bmp_rotate.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	// Check if user is using the program properly
	if (argc != 2)
		terminate("Usage: bmp_rotate image.bmp");

	FILE* fp = open_file(argv);

	struct bmp_header* header_buffer = create_header();

	handle_header_parsing(header_buffer, fp);

	// Initialize width, height
	uint32_t width = header_buffer->width;
	uint32_t height = header_buffer->height;

	// Set the fp pointer to the data part of source bmp image
	fseek(fp, header_buffer->data_offset, SEEK_SET);

	FILE* out = create_out();

	write_header(header_buffer, width, height, out);
	write_body(header_buffer, width, height, fp, out);

	free(header_buffer);
	fclose(fp);
	fclose(out);

	return 0;
}
