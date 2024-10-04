#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "file_parser.h"
#include "stack.h"

void print_line(const DataUnit* line) {
    printf("Message processed:\n");
    printf(" Type: %02X\n", line->msg.type);
    printf(" Length: %02X\n", line->msg.length);
    printf(" Payload: ");
    for (int i = 0; i < line->msg.length; i++)
        printf("%02X ", line->msg.payload[i]);
    printf("\n");
    printf(" CRC-32: %08X\n", line->msg.crc32);
    printf(" Mask: %08X\n", line->mask);
}

int parse_file(const char* filename, line_handler_func handler) {
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        printf("error: Can not open this file: %s.\n", filename);
        return -1;
    }

    char line[BUFFER_LEN] = {};
    Stack data_base;
    init_stack(&data_base);
    DataUnit elem = {};

    for (uint8_t count = 0;fgets(line, sizeof(line), input_file); ++count) {
        handler(line, &elem);
        if (fgets(line, sizeof(line), input_file))
            handler(line, &elem);
        push(&data_base, &elem);
    }

    fclose(input_file);
    free_stack(&data_base);
    return 0;
}

