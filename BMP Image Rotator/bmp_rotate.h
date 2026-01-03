#ifndef BMP_ROTATE_H
#define BMP_ROTATE_H

#include <stdint.h>
#include <stdio.h>

#define BMP_HEADER_SIZE 54
#define MAX_IMAGE_DIM 10000

struct bmp_header
{
	uint8_t signature[2];
	uint32_t file_size;
	uint32_t reserved;
	uint32_t data_offset;
	uint32_t bi_size;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bit_count;
	uint32_t bi_compression;
	uint32_t bi_size_image;
	uint32_t bi_x_pels_per_meter;
	uint32_t bi_y_pels_per_meter;
	uint32_t bi_clr_used;
	uint32_t bi_clr_important;
} __attribute__((packed));

void terminate(char* s);
struct bmp_header* create_header();
FILE* create_out();
FILE* open_file(char** argv);
uint8_t parse_header(FILE* fp, struct bmp_header* header_buffer);
void handle_header_parsing(struct bmp_header* header_buffer, FILE* fp);
void write_header(struct bmp_header* header_buffer, uint32_t width,
                  uint32_t height, FILE* out);
void write_body(struct bmp_header* header_buffer, uint32_t width,
                uint32_t height, FILE* fp, FILE* out);
uint8_t* rotate_row(FILE* fp, uint32_t width, uint32_t height,
                    uint32_t data_offset, uint32_t column);

#endif
