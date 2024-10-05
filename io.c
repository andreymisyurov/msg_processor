#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "data_processor.h"
#include "io.h"
#include "logger.h"
#include "stack.h"

#define MESSAGE_REGEX "^mess=([0-9A-Fa-f]+)"
#define MASK_REGEX "^mask=([0-9A-Fa-f]{8})"

static uint8_t hex_to_uint8(const char* hex_str) {
    return (uint8_t)strtol(hex_str, NULL, 16);
}

static uint32_t hex_to_uint32(const char* hex_str) {
    return (uint32_t)strtol(hex_str, NULL, 16);
}

int handle_msg(const char* line, DataUnit* value) {
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
        log_message(LOG_LEVEL_WARNING, "warn: Undefined line: %s", line);
    }

    regfree(&message_regex);
    regfree(&mask_regex);

    return result;
}

int parse_file(const char* filename, line_handler_func handler) {
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        log_message(LOG_LEVEL_ERROR, "error: Can not open this file: %s.\n", filename);
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
            log_message(LOG_LEVEL_ERROR, "error: Incorect input data.");
            abort();
        }
        push(&elem);
    }

    fclose(input_file);
    return 0;
}

void write_results_to_file(DataUnit* data_unit, DataUnit* initial_unit, const char* filename) {
    FILE* output_file = fopen(filename, "w");

    if (output_file == NULL) {
        log_message(LOG_LEVEL_ERROR, "error: Could not open output file.");
        abort();
    }

    fprintf(output_file, "Message type: %02X\n", data_unit->msg.type);
    fprintf(output_file, "Initial length: %02X\n", initial_unit->msg.length);
    fprintf(output_file, "Initial CRC-32: %08X\n", initial_unit->msg.crc32);
    fprintf(output_file, "Initial data bytes: ");
    for (int i = 0; i < initial_unit->msg.length; ++i) {
        fprintf(output_file, "%02X ", initial_unit->msg.payload[i]);
    }
    fprintf(output_file, "\nModified length: %02X\n", data_unit->msg.length);
    fprintf(output_file, "Modified CRC-32: %08X\n", data_unit->msg.crc32);
    fprintf(output_file, "Modified data bytes with mask: ");
    for (int i = 0; i < data_unit->msg.length; ++i) {
        fprintf(output_file, "%02X ", data_unit->msg.payload[i]);
    }
    fclose(output_file);
}
