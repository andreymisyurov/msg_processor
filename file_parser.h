#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <stdint.h>

#define BUFFER_LEN 1024
#define CAPACITY_MAX 128

typedef struct Message {
    uint8_t type;
    uint8_t length;
    uint8_t payload[254];
    uint32_t crc32;
} Message;

typedef struct DataUnit {
    Message msg;
    uint32_t mask;
} DataUnit;

typedef void* (*line_handler_func)(const char* line, DataUnit* value);

int parse_file(const char* filename, line_handler_func handler);

#endif

