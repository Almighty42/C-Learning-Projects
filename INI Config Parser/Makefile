CC = gcc
CFLAGS_DEBUG =  -std=c11 -g -fsanitize=address -O0 -DDEBUG
CFLAGS_PROD = -Wall -Wextra -std=c11 -O2 -DNDEBUG

TARGET = config_parser
SOURCES = main.c config_parser.c
HEADERS = config_parser.h

.DEFAULT_GOAL := prod

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
