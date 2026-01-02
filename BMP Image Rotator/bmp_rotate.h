#ifndef BMP_ROTATE_H
#define BMP_ROTATE_H

#include <stdint.h>
#include <stdio.h>

struct bmp_header
{
	uint8_t signature[2];
	uint32_t file_size;
	uint32_t reserved;
	uint32_t data_offset;
	uint32_t header_size;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bits_per_pixel;
} __attribute__((packed));

void terminate(char* s);
uint8_t parse_header(FILE* fp, struct bmp_header* header_buffer);
uint8_t rotate_bmp_image(FILE* fp, uint32_t width, uint32_t height);
static void reverse_pixels(uint8_t* arr, uint32_t size);

#endif
