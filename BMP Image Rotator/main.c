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

	FILE* out;
	if ((out = fopen("out.bmp", "wb")) == NULL)
		terminate("Error trying to create out.bmp");

	struct bmp_header out_header = *header_buffer;
	out_header.width = height;
	out_header.height = width;
	uint32_t new_bytes_per_row = ((height * 3 + 3) / 4) * 4;
	out_header.file_size = 54 + new_bytes_per_row * width;
	fwrite(&out_header, sizeof(struct bmp_header), 1, out);

	uint8_t* tmp_row;
	for (uint32_t i = 0; i < width; i++) {
		if ((tmp_row = rotate_row(fp, width, height,
		                          header_buffer->data_offset, i)) ==
		    NULL)
			terminate(
			    "Error trying allocate memory - not enough memory");
		fwrite(tmp_row, height * 3, 1, out);

		uint32_t padding = new_bytes_per_row - (height * 3);
		for (uint32_t p = 0; p < padding; p++)
			fputc(0, out);

		free(tmp_row);
	}
	free(header_buffer);
	fclose(out);

	return 0;
}
