#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "file_parser.h"


int parse_file(const char* filename, line_handler_func handler) {
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        printf("error: Can not open this file: %s.\n", filename);
        return -1;
    }

    char line[BUFFER_LEN] = {};
    DataUnit data_base[CAPACITY_MAX] = {};

    for (uint8_t count = 0;fgets(line, sizeof(line), input_file); ++count) {
        handler(line, &data_base[count]);
        if (fgets(line, sizeof(line), input_file))
            handler(line, &data_base[count]);
    }

    fclose(input_file);
    return 0;
}

