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

uint8_t rotate_bmp_image(FILE* fp, uint32_t width, uint32_t height)
{
	uint8_t* source_line;
	uint8_t* destination_line;
	uint8_t temp[height * 3];
	for (uint32_t i = 0; i < width; i += 3) {
		for (uint32_t j = 0; j < width; j += 3) {
			fread(temp, 3, 1, fp);
			fseek(fp, width - 3, SEEK_CUR);
		}
		reverse_pixels(temp, height * 3);
		// TODO: Write to destination line and then
		// Write to stdout or just return result
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
