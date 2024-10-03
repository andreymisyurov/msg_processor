#include "file_parser.h"

#include <stdio.h>

void print_line(const char* line) {
    printf("%s", line);  // Выводим строку в терминал
}

int main() {
    if (parse_file("data_in.txt", print_line) != 0) {
        printf("error: processing\n");
        return -1;
    }

    return 0;
}
