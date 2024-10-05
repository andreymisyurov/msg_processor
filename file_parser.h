#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <stdint.h>
#include "stack.h"

#define BUFFER_LEN 1024
#define CAPACITY_MAX 128

typedef int (*line_handler_func)(const char* line, DataUnit* value);

int parse_file(const char* filename, line_handler_func handler);
void print_line(const DataUnit* line);

#endif

