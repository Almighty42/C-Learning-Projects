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

uint8_t parse_header(FILE* fp, struct bmp_header* header_buffer)
{
	if (header_buffer == NULL)
		return 1;
	if (fread(header_buffer, sizeof(struct bmp_header), 1, fp) != 1)
		return 2;
	if (memcmp(header_buffer->signature, "BM", 2) != 0 ||
	    header_buffer->header_size != 40 || header_buffer->planes != 1 ||
	    header_buffer->bits_per_pixel != 24 || header_buffer->width == 0 ||
	    header_buffer->height == 0 || header_buffer->data_offset < 54 ||
	    header_buffer->data_offset > 100)
		return 3;
	fseek(fp, 0, SEEK_END);
	long actual_size = ftell(fp);
	if (header_buffer->file_size != (uint32_t)actual_size)
		return 4;

	return 0;
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
		long position = data_offset + j * bytes_per_row + column * 3;
		fseek(fp, position, SEEK_SET);
		fread(row_ptr + j * 3, 3, 1, fp);
	}
	reverse_pixels(row_ptr, height * 3);
	return row_ptr;
}
