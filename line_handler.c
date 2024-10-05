#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

#include "line_handler.h"

#define MESSAGE_REGEX "^mess=([0-9A-Fa-f]+)"
#define MASK_REGEX "^mask=([0-9A-Fa-f]{8})"

uint8_t hex_to_uint8(const char* hex_str) {
    return (uint8_t)strtol(hex_str, NULL, 16);
}

uint32_t hex_to_uint32(const char* hex_str) {
    return (uint32_t)strtol(hex_str, NULL, 16);
}

int handle_line(const char* line, DataUnit* value) {
    int result = -1;
    regex_t message_regex, mask_regex;
    regmatch_t pmatch[2];
    regcomp(&message_regex, MESSAGE_REGEX, REG_EXTENDED);
    regcomp(&mask_regex, MASK_REGEX, REG_EXTENDED);

    const char* message = line;
    if (regexec(&message_regex, line, 2, pmatch, 0) == 0) {
        message += pmatch[1].rm_so;
        sscanf(message, "%2hhx%2hhx", &value->msg.type, &value->msg.length);
        message += 4;
        
        for (int i = 0; i < value->msg.length; ++i, message += 2) {
            value->msg.payload[i] = hex_to_uint8((char[]){ message[0], message[1], '\0' });
        }

        value->msg.crc32 = 0;
        for (int i = 0; i < 8; ++i, ++message) {
            uint8_t hex_value = hex_to_uint8((char[]){*message, '\0'});
            value->msg.crc32 = (value->msg.crc32 << 4) | hex_value;
        }
        result = 1;
    } 
    else if (regexec(&mask_regex, line, 2, pmatch, 0) == 0) {        
        value->mask = hex_to_uint32(line + pmatch[1].rm_so);
        result = 2;
    } 
    else {
        printf("Undefined line: %s\n", line);
    }

    regfree(&message_regex);
    regfree(&mask_regex);

    return result;
}
