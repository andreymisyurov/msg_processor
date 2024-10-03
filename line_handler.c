#include "line_handler.h"
#include <stdio.h>
#include <string.h>
#include <regex.h>

#define MESSAGE_REGEX "^mess=([0-9A-Fa-f]+)"
#define MASK_REGEX "^mask=([0-9A-Fa-f]{8})"

void handle_line(const char* line) {
    regex_t message_regex, mask_regex;
    regmatch_t pmatch[2];

    regcomp(&message_regex, MESSAGE_REGEX, REG_EXTENDED);
    regcomp(&mask_regex, MASK_REGEX, REG_EXTENDED);

    if (regexec(&message_regex, line, 2, pmatch, 0) == 0) {
        char message[256] = {0};
        int start = pmatch[1].rm_so;
        int end = pmatch[1].rm_eo;
        strncpy(message, line + start, end - start);
        printf("msg was processed: %s\n", message);
    } else if (regexec(&mask_regex, line, 2, pmatch, 0) == 0) {
        char mask[9] = {0};
        int start = pmatch[1].rm_so;
        int end = pmatch[1].rm_eo;
        strncpy(mask, line + start, end - start);
        printf("mask was processed: %s\n", mask);
    } else {
        printf("Неизвестная строка: %s\n", line);
    }

    regfree(&message_regex);
    regfree(&mask_regex);
}

