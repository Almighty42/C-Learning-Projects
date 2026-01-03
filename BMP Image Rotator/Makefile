CC = gcc
CFLAGS_DEBUG = -Wall -Wextra -Werror -std=c11 -g -fsanitize=address -O0 -DDEBUG
CFLAGS_PROD = -Wall -Wextra -std=c11 -O2 -DNDEBUG
TARGET = bmp_rotate
SOURCES = main.c bmp_rotate.c
HEADERS = bmp_rotate.h

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: $(TARGET)_debug

prod: CFLAGS += $(CFLAGS_PROD)
prod: $(TARGET)

$(TARGET)_debug: $(SOURCES) $(HEADERS) Makefile
	$(CC) $(CFLAGS) -o $@ $(SOURCES) 

$(TARGET): $(SOURCES) $(HEADERS) Makefile
	$(CC) $(CFLAGS) -o $@ $(SOURCES) -s

clean:
	rm -f $(TARGET) $(TARGET)_debug

.PHONY: debug prod clean
