#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "file_parser.h"
#include "stack.h"

int parse_file(const char* filename, line_handler_func handler) {
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        printf("error: Can not open this file: %s.\n", filename);
        return -1;
    }

    char line[BUFFER_LEN] = {};
    Stack *data_base = get_stack_instance();
    DataUnit elem = {};

    for (uint8_t count = 0; fgets(line, sizeof(line), input_file); ++count) {
        int check_result_1, check_result_2;
        check_result_1 = handler(line, &elem);
        if (fgets(line, sizeof(line), input_file)) {
            check_result_2 = handler(line, &elem);
        }
        if (check_result_1 < 0 || check_result_2 < 0 || check_result_1 == check_result_2) {
            printf("error: Incorect input data.\n");
            exit(EXIT_FAILURE);
        }
        push(&elem);
    }

    fclose(input_file);
    return 0;
}
