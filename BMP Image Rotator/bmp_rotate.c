#include "bmp_rotate.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void terminate(char* s)
{
	fprintf(stderr, "%s\n", s);
	exit(EXIT_FAILURE);
}

struct bmp_header* create_header()
{
	struct bmp_header* header_buffer;
	if ((header_buffer = malloc(sizeof(struct bmp_header))) == NULL)
		terminate("Error trying allocate memory - not enough memory");
	return header_buffer;
}

FILE* open_file(char** argv)
{
	FILE* fp = fopen(*(argv + 1), "rb");
	if (fp == NULL)
		terminate("Error trying to open file");
	return fp;
}

FILE* create_out()
{
	FILE* out;
	if ((out = fopen("out.bmp", "wb")) == NULL)
		terminate("Error trying to create out.bmp");
	return out;
}

uint8_t parse_header(FILE* fp, struct bmp_header* header_buffer)
{
	if (header_buffer == NULL)
		return 1;
	if (fread(header_buffer, sizeof(struct bmp_header), 1, fp) != 1)
		return 2;
	if (memcmp(header_buffer->signature, "BM", 2) != 0 ||
	    header_buffer->bi_size != 40 || header_buffer->planes != 1 ||
	    header_buffer->bit_count != 24 || header_buffer->width == 0 ||
	    header_buffer->height == 0 ||
	    header_buffer->data_offset < BMP_HEADER_SIZE ||
	    header_buffer->data_offset > 100)
		return 3;
	fseek(fp, 0, SEEK_END);
	uint32_t actual_size = ftell(fp);
	if (header_buffer->file_size != (uint32_t)actual_size)
		return 4;
	fseek(fp, header_buffer->data_offset, SEEK_SET);
	return 0;
}

void handle_header_parsing(struct bmp_header* header_buffer, FILE* fp)
{
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
			free(header_buffer);
			terminate("Error size of file in header does "
			          "not match the "
			          "actual file size");
			break;
	}
}

void write_header(struct bmp_header* header_buffer, uint32_t width,
                  uint32_t height, FILE* out)
{
	struct bmp_header out_header = *header_buffer;
	out_header.width = height;
	out_header.height = width;
	uint32_t new_bytes_per_row = ((height * 3 + 3) / 4) * 4;
	out_header.file_size = BMP_HEADER_SIZE + new_bytes_per_row * width;
	fwrite(&out_header, sizeof(struct bmp_header), 1, out);
}

void write_body(struct bmp_header* header_buffer, uint32_t width,
                uint32_t height, FILE* fp, FILE* out)
{
	uint32_t new_bytes_per_row = ((height * 3 + 3) / 4) * 4;
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
}

static void reverse_pixels(uint8_t* arr, uint32_t size)
{
	uint8_t *start = arr, *end = arr + (size - 3);
	uint8_t temp_a;
	uint8_t temp_b;
	uint8_t temp_c;
	while (start < end) {
		temp_a = *start;
		temp_b = *(start + 1);
		temp_c = *(start + 2);

		*start = *end;
		*(start + 1) = *(end + 1);
		*(start + 2) = *(end + 2);

		*end = temp_a;
		*(end + 1) = temp_b;
		*(end + 2) = temp_c;

		start += 3;
		end -= 3;
	}
}

uint8_t* rotate_row(FILE* fp, uint32_t width, uint32_t height,
                    uint32_t data_offset, uint32_t column)
{
	uint32_t bytes_per_row = ((width * 3 + 3) / 4) * 4;
	uint8_t* row_ptr = malloc(height * 3);
	if (!row_ptr)
		return NULL;
	for (uint32_t j = 0; j < height; j++) {
		uint32_t position =
		    data_offset + j * bytes_per_row + column * 3;
		fseek(fp, position, SEEK_SET);
		fread(row_ptr + j * 3, 3, 1, fp);
	}
	reverse_pixels(row_ptr, height * 3);
	return row_ptr;
}
