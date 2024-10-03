#include "file_parser.h"
#include <stdio.h>

int parse_file(const char* filename, line_handler_func handler) {
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        printf("error: Can not open this file: %s.\n", filename);
        return -1;
    }

    char line[BUFFER_LEN] = {};

    while (fgets(line, sizeof(line), input_file)) {
        handler(line);
    }

    fclose(input_file);
    return 0;
}

