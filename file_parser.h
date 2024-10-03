#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#define BUFFER_LEN 1024

typedef void (*line_handler_func)(const char* line);

int parse_file(const char* filename, line_handler_func handler);

#endif

