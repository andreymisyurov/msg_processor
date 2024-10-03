#include "file_parser.h"
#include "line_handler.h"

#include <stdio.h>

int main() {
    if (parse_file("data_in.txt", handle_line) != 0) {
        printf("error: processing\n");
        return -1;
    }

    return 0;
}
