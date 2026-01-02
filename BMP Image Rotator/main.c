#include "bmp_rotate.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	if (argc != 2)
		terminate("Usage: bmp_rotate image.bmp");

	FILE* fp = fopen(argv[1], "rb");
	if (fp == NULL)
		terminate("Error trying to open file");

	struct bmp_header* header_buffer;
	if ((header_buffer = malloc(sizeof(struct bmp_header))) == NULL)
		terminate("Error trying allocate memory - not ");
	uint8_t parse_res = parse_header(fp, header_buffer);
	switch (parse_res) {
		case 1:
			terminate("Error trying allocate memory - not "
			          "enough memory");
			break;
		case 2:
			free(header_buffer);
			terminate("Error trying to read the header of "
			          "bmp file");
			break;
		case 3:
			free(header_buffer);
			terminate("Error validating bmp file");
			break;
		case 4:

			terminate("Error size of file in header does "
			          "not match the "
			          "actual file size");
	}

	uint32_t width = header_buffer->width;
	uint32_t height = header_buffer->height;

	fseek(fp, header_buffer->data_offset, SEEK_SET);
	uint8_t op_result = rotate_bmp_image(fp, width, height);
	return 0;
}
